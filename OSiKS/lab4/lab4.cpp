#include <iostream>
#include <string>
#include <windows.h>
#include <filesystem>
#include <vector>
#include <fstream>
#include <iomanip>

void process_file(std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    char header[14];
    file.read(header, 14);

    int count_ipv4 = 0, count_arp = 0, count_eth_dix = 0, count_eth_snap = 0, count_novell_802_2 = 0, count_novell_802_3 = 0;
    for (int i = 1; file && !file.eof(); i++, file.read(header, 14)) {
        int size = 14;

        std::cout << "Frame #" << std::dec << i << std::endl;

        std::cout << "  MAC addresses: ";
        for (int j = 0; j < 6; j++) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (0xFF & static_cast<unsigned int>(header[j]));
            if (j != 5) std::cout << ":";
        }
        std::cout << " <= ";
        for (int j = 0; j < 6; j++) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (0xFF & static_cast<unsigned int>(header[6 + j]));
            if (j != 5) std::cout << ":";
        }
        std::cout << std::endl;

        unsigned short type = ((0xFF & header[12]) << 8) | (0xFF & header[13]);
        if (type > 0x0600) {
            if (type == 0x800) {
                std::cout << "  Type: IPv4" << std::endl;

                char ip_header[20];
                file.read(ip_header, 20);
                size += 20;

                std::cout << "    IP addresses: ";
                for (int j = 0; j < 4; j++) {
                    std::cout << std::dec << (0xFF & static_cast<unsigned int>(ip_header[12 + j]));
                    if (j != 3) std::cout << ".";
                }
                std::cout << " => ";
                for (int j = 0; j < 4; j++) {
                    std::cout << std::dec << (0xFF & static_cast<unsigned int>(ip_header[16 + j]));
                    if (j != 3) std::cout << ".";
                }
                std::cout << std::endl;

                unsigned short total_length = ((0xFF & ip_header[2]) << 8) | (0xFF & ip_header[3]);
                file.seekg(total_length - 20, std::ios::cur);
                size += total_length - 20;
                count_ipv4++;
            }
            else if (type == 0x806) {
                std::cout << "  Type: ARP" << std::endl;

                file.seekg(28, std::ios::cur);
                size += 28;
                count_arp++;
            }
            else {
                std::cout << "  Type: Ethernet DIX (Ethernet II)" << std::endl;
                file.seekg(type, std::ios::cur);
                size += type;
                count_eth_dix++;
            }
        }
        else {
            char dsap_ssap[2];
            file.read(dsap_ssap, 2);
            size += 2;

            if (dsap_ssap[0] == 0xAA && dsap_ssap[1] == 0xAA) {
                std::cout << "  Type: Ethernet SNAP" << std::endl;
                file.seekg(type - 2, std::ios::cur);
                size += type - 2;
                count_eth_snap++;
            }
            else if (type <= 0x05DC) {
                std::cout << "  Type: Novell 802.2" << std::endl;
                file.seekg(type - 2, std::ios::cur);
                size += type - 2;
                count_novell_802_2++;
            }
            else if (type == 0xFFFF) {
                std::cout << "  Type: Novell 802.3" << std::endl;
                file.seekg(type - 2, std::ios::cur);
                size += type - 2;
                count_novell_802_3++;
            }
        }

        std::cout << "  Size: " << std::dec << size << std::endl;
    }

    std::cout << std::endl << "Processed frames: " << std::dec << count_ipv4 + count_arp + count_eth_dix + count_eth_snap + count_novell_802_2 + count_novell_802_3 << std::endl;
    std::cout << "  IPv4: " << count_ipv4 << std::endl;
    std::cout << "  ARP: " << count_arp << std::endl;
    std::cout << "  Ethernet DIX (Ethernet II): " << count_eth_dix << std::endl;
    std::cout << "  Ethernet SNAP: " << count_eth_snap << std::endl;
    std::cout << "  Novell 802.2: " << count_novell_802_2 << std::endl;
    std::cout << "  Novell 802.3: " << count_novell_802_3 << std::endl;

    file.close();
}

int main() {
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string filename;
    std::cout << "Filename: ";
    std::getline(std::cin, filename);

    process_file(filename);

    return 0;
}