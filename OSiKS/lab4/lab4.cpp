#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

const int ETHERNET_HEADER_SIZE = 14;
const int IPV4_HEADER_SIZE = 20;
const int ARP_HEADER_SIZE = 28;
const unsigned short ETHERTYPE_IPV4 = 0x0800;
const unsigned short ETHERTYPE_ARP = 0x0806;
const unsigned char SNAP_SAP = 0xAA;
const unsigned short NOVELL_802_3_TYPE = 0xFFFF;

void print_mac_address(const char* mac_address) {
    for (int j = 0; j < 6; j++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (0xFF & static_cast<unsigned int>(mac_address[j]));
        if (j != 5) std::cout << ":";
    }
}

void print_ip_address(const char* ip_address) {
    for (int j = 0; j < 4; j++) {
        std::cout << std::dec << (0xFF & static_cast<unsigned int>(ip_address[j]));
        if (j != 3) std::cout << ".";
    }
}

void process_file(std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    int count_ipv4 = 0, count_arp = 0, count_eth_dix = 0, count_eth_snap = 0, count_novell_802_2 = 0, count_novell_802_3 = 0;
    int i = 1;
    while (file) {
        char header[ETHERNET_HEADER_SIZE];
        file.read(header, ETHERNET_HEADER_SIZE);

        if (file.eof()) break; // Exit if end of file is reached.
        if (file.fail()) {
            std::cerr << "Error reading Ethernet header." << std::endl;
            break;
        }

        int size = ETHERNET_HEADER_SIZE;

        std::cout << "Frame #" << std::dec << i++ << std::endl; // Increment i here

        std::cout << "  MAC addresses: ";
        print_mac_address(header);
        std::cout << " <= ";
        print_mac_address(header + 6);
        std::cout << std::endl;

        unsigned short type = ((0xFF & header[12]) << 8) | (0xFF & header[13]);
        std::cout << "  EtherType: 0x" << std::hex << std::setw(4) << std::setfill('0') << type << std::endl; // Print EtherType

        if (type > 0x0600) {
            if (type == ETHERTYPE_IPV4) {
                std::cout << "  Type: IPv4" << std::endl;

                char ip_header[IPV4_HEADER_SIZE];
                file.read(ip_header, IPV4_HEADER_SIZE);
                if (file.fail()) {
                    std::cerr << "Error reading IPv4 header." << std::endl;
                    break;
                }
                size += IPV4_HEADER_SIZE;

                std::cout << "    IP addresses: ";
                print_ip_address(ip_header + 12);
                std::cout << " => ";
                print_ip_address(ip_header + 16);
                std::cout << std::endl;

                unsigned short total_length = ((0xFF & ip_header[2]) << 8) | (0xFF & ip_header[3]);

                // IMPORTANT:  Account for IP header length (IHL)
                unsigned char ihl = ip_header[0] & 0x0F;  // Lower 4 bits of first byte
                int ip_header_length = ihl * 4;

                file.seekg(total_length - ip_header_length, std::ios::cur);
                size += total_length - ip_header_length;
                count_ipv4++;
            }
            else if (type == ETHERTYPE_ARP) {
                std::cout << "  Type: ARP" << std::endl;

                file.seekg(ARP_HEADER_SIZE, std::ios::cur);
                if (file.fail()) {
                    std::cerr << "Error skipping ARP header." << std::endl;
                    break;
                }
                size += ARP_HEADER_SIZE;
                count_arp++;
            }
            else {
                std::cout << "  Type: Ethernet DIX (Ethernet II)" << std::endl;
                // Check if the 'type' (length field) is a sane value before seeking
                if (type > 65535) {
                    std::cerr << "Invalid Ethernet II length: " << type << std::endl;
                    break;
                }

                file.seekg(type - ETHERNET_HEADER_SIZE, std::ios::cur);  //Type is length so need to subtract
                if (file.fail()) {
                    std::cerr << "Error skipping Ethernet II data." << std::endl;
                    break;
                }
                size += type - ETHERNET_HEADER_SIZE; //type is length of packet so minus ethernet header to get extra length added
                count_eth_dix++;
            }
        }
        else {
            char dsap_ssap[2];
            file.read(dsap_ssap, 2);
            if (file.fail()) {
                std::cerr << "Error reading DSAP/SSAP." << std::endl;
                break;
            }
            size += 2;

            if (dsap_ssap[0] == SNAP_SAP && dsap_ssap[1] == SNAP_SAP) {
                std::cout << "  Type: Ethernet SNAP" << std::endl;
                file.seekg(type - 2, std::ios::cur);
                if (file.fail()) {
                    std::cerr << "Error skipping Ethernet SNAP data." << std::endl;
                    break;
                }
                size += type - 2;
                count_eth_snap++;
            }
            else if (type <= 0x05DC) {
                std::cout << "  Type: Novell 802.2" << std::endl;
                file.seekg(type - 2, std::ios::cur);
                if (file.fail()) {
                    std::cerr << "Error skipping Novell 802.2 data." << std::endl;
                    break;
                }
                size += type - 2;
                count_novell_802_2++;
            }
            else if (type == NOVELL_802_3_TYPE) {
                std::cout << "  Type: Novell 802.3" << std::endl;
                file.seekg(type - 2, std::ios::cur);
                if (file.fail()) {
                    std::cerr << "Error skipping Novell 802.3 data." << std::endl;
                    break;
                }
                size += type - 2;
                count_novell_802_3++;
            }
        }

        std::cout << "  Size: " << std::dec << size << std::endl;
    }

    std::cout << "Summary:" << std::endl;
    std::cout << "  IPv4: " << count_ipv4 << std::endl;
    std::cout << "  ARP: " << count_arp << std::endl;
    std::cout << "  Ethernet DIX: " << count_eth_dix << std::endl;
    std::cout << "  Ethernet SNAP: " << count_eth_snap << std::endl;
    std::cout << "  Novell 802.2: " << count_novell_802_2 << std::endl;
    std::cout << "  Novell 802.3: " << count_novell_802_3 << std::endl;


    file.close();  //Close file!
}

int main() {
    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;
    process_file(filename);
    return 0;
}
