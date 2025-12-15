#include <iostream>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <fcntl.h>

const int ICMP_TIMESTAMP_REQUEST = 13;
const int ICMP_TIMESTAMP_REPLY = 14;
const int ICMP_HEADER_SIZE = 8;
const int ICMP_TIMESTAMP_SIZE = 20;

bool timeout_expired = false;

struct icmp_timestamp_packet {
    icmphdr header;
    uint32_t originate_timestamp;
    uint32_t receive_timestamp;
    uint32_t transmit_timestamp;
};

void handle_timeout(int signal) {
    timeout_expired = true;
}

void set_timeout_handler(int timeout) {
    struct sigaction signal_action;
    signal_action.sa_handler = &handle_timeout;
    signal_action.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &signal_action, nullptr);
    alarm(timeout);
}

void set_nonblocking(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}

unsigned short icmp_checksum(unsigned short* buf, int length) {
    unsigned long sum = 0;
    while (length > 1) {
        sum += *buf++;
        length -= 2;
    }
    if (length > 0) {
        sum += *buf & ntohs(0xFF00);
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}

uint32_t get_timestamp_milliseconds() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    struct tm* timeinfo = gmtime(&tv.tv_sec);
    uint32_t seconds_since_midnight = timeinfo->tm_hour * 3600 +
        timeinfo->tm_min * 60 +
        timeinfo->tm_sec;

    return (seconds_since_midnight * 1000) + (tv.tv_usec / 1000);
}

std::string format_timestamp(uint32_t timestamp_ms) {
    uint32_t hours = timestamp_ms / 3600000;
    uint32_t minutes = (timestamp_ms % 3600000) / 60000;
    uint32_t seconds = (timestamp_ms % 60000) / 1000;
    uint32_t milliseconds = timestamp_ms % 1000;

    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%02u:%02u:%02u.%03u",
        hours, minutes, seconds, milliseconds);
    return std::string(buffer);
}

int ping(std::string hostname, int count) {
    struct timeval start, end;
    struct icmp_timestamp_packet packet;
    struct sockaddr_in addr;
    char buffer[64];

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(hostname.c_str());

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock == -1) {
        std::cout << "socket() error" << std::endl;
        return 1;
    }
    set_nonblocking(sock);

    std::cout << "PING (timestamp) " << hostname << " (" << inet_ntoa(addr.sin_addr) << ") " << ICMP_TIMESTAMP_SIZE << " bytes of data." << std::endl;

    int packets_sent = 0;
    int packets_received = 0;

    for (int i = 1; i <= count; ++i) {
        gettimeofday(&start, nullptr);

        packet.header.type = ICMP_TIMESTAMP_REQUEST;
        packet.header.code = 0;
        packet.header.checksum = 0;
        packet.header.un.echo.id = getpid();
        packet.header.un.echo.sequence = i;

        packet.originate_timestamp = htonl(get_timestamp_milliseconds());
        packet.receive_timestamp = 0;
        packet.transmit_timestamp = 0;

        packet.header.checksum = icmp_checksum(reinterpret_cast<unsigned short*>(&packet), sizeof(packet));

        if (sendto(sock, &packet, sizeof(packet), 0, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
            std::cout << "sendto() error" << std::endl;
            return 1;
        }
        set_timeout_handler(1);
        packets_sent++;

        while (!timeout_expired) {
            if (recvfrom(sock, &buffer, sizeof(buffer), 0, nullptr, nullptr) >= 0) {
                gettimeofday(&end, nullptr);

                struct iphdr* ip = (struct iphdr*)buffer;
                struct icmphdr* icmp_header = (struct icmphdr*)(buffer + (ip->ihl * 4));

                if (icmp_header->type == ICMP_TIMESTAMP_REPLY &&
                    icmp_header->un.echo.id == getpid()) {

                    struct icmp_timestamp_packet* reply_packet =
                        (struct icmp_timestamp_packet*)icmp_header;

                    uint32_t originate_ts = ntohl(reply_packet->originate_timestamp);
                    uint32_t receive_ts = ntohl(reply_packet->receive_timestamp);
                    uint32_t transmit_ts = ntohl(reply_packet->transmit_timestamp);

                    double rtt = (end.tv_sec - start.tv_sec) * 1000.0 +
                        (end.tv_usec - start.tv_usec) / 1000.0;

                    std::cout << ICMP_TIMESTAMP_SIZE << " bytes from " << hostname
                        << " (" << inet_ntoa(addr.sin_addr) << "): icmp_seq=" << i
                        << " ttl=" << (int)ip->ttl << " time=" << rtt << " ms" << std::endl;

                    std::cout << "    Originate timestamp: " << format_timestamp(originate_ts)
                        << " UTC" << std::endl;
                    std::cout << "    Receive timestamp:   " << format_timestamp(receive_ts)
                        << " UTC" << std::endl;
                    std::cout << "    Transmit timestamp:  " << format_timestamp(transmit_ts)
                        << " UTC" << std::endl;

                    packets_received++;
                    break;
                }
            }
        }

        alarm(0);
        usleep(1000000);
    }

    std::cout << std::endl << "--- " << hostname << " ping statistics ---" << std::endl;
    std::cout << packets_sent << " packets transmitted, " << packets_received
        << " received, " << 100 - packets_received * 100 / packets_sent
        << "% packet loss, time " << (count * 1000) << "ms" << std::endl;

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("error: ip not specified\n");
        return 1;
    }

    return ping(std::string(argv[1]), 3);
}