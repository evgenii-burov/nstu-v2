# file_name = input('Enter the name of a binary file to read (ethersXX.bin): ')
file_name = "ethers05.bin"
frame_count = 0

ipv4_count = 0
ethernet_802_2_count = 0
ethernet_802_3_count = 0
arp_count = 0
ethernet_II_count = 0
ethernet_snap_count = 0

MAXIMUM_DATA_LENGTH = '05dc'
IPV4_TYPE = '0800'
ARP_TYPE = '0806'
#VALID_ETHER_TYPE = int('0600', 16)
ETHERNET_SNAP = 'aaaa'
RAW_802_3 = 'ffff'

total_file_size = 0

try:
    with open(file_name, "rb") as file:
        while True:
            frame_count += 1
            mac_destination = file.read(6).hex()
            mac_sender = file.read(6).hex()
            if not mac_destination or not mac_sender:
                frame_count -= 1
                break
            frame_length = file.read(2).hex()
            print(f'Frame #{frame_count}')
            print(f'MAC destination address: {':'.join([mac_destination[i:i+2] for i in range(0, len(mac_destination), 2)]).upper()}')
            print(f'MAC sender address: {':'.join([mac_sender[i:i+2] for i in range(0, len(mac_sender), 2)]).upper()}')

            frame_size = 14

            if int(frame_length, 16) > int(MAXIMUM_DATA_LENGTH, 16):
                ethernet_II_count += 1

                if frame_length == IPV4_TYPE:
                    ipv4_count += 1
                    print('Frame type: Ethernet II IPv4')
                    ip_header = file.read(20)
                    if len(ip_header) < 20:
                        break
                        
                    ihl = ip_header[0] & 0x0F
                    ip_header_bytes = ihl * 4
                    
                    total_length = int.from_bytes(ip_header[2:4], byteorder='big')
                    
                    print(f"  Total Length: {total_length}")
                    print(f"  IP Header Length: {ip_header_bytes} bytes")
                    
                    source_ip = '.'.join(str(b) for b in ip_header[12:16])
                    destination_ip = '.'.join(str(b) for b in ip_header[16:20])
                    print(f'IPv4 destination address: {destination_ip}')
                    print(f'IPv4 sender address: {source_ip}')

                    ip_payload_bytes = total_length - ip_header_bytes
                    file.seek(ip_payload_bytes, 1)
                    frame_size += 20 + ip_payload_bytes

                elif frame_length == ARP_TYPE:
                    arp_count += 1
                    print('Frame type: Ethernet II ARP')
                    file.seek(28, 1)
                    frame_size += 28

                else:
                    print('Frame type: Ethernet II UNKNOWN')

            else:
                dsap_ssap = file.read(2).hex()
                frame_size += 2

                if dsap_ssap == RAW_802_3:
                    ethernet_802_3_count += 1
                    print('Frame type: Ethernet 802.3 (Raw)')

                elif dsap_ssap == ETHERNET_SNAP:
                    ethernet_snap_count += 1
                    print('Frame type: Ethernet SNAP')
                
                else:
                    ethernet_802_2_count += 1
                    print('Frame type: Ethernet 802.2')
                
                file.seek(int(frame_length, 16) - 2, 1)
                frame_size += int(frame_length, 16) - 2
            print(f'Frame size: {frame_size}')
            total_file_size += frame_size
            print('-'*50)

except FileNotFoundError:
    print(f"Error: The file {file_name} was not found.")
except Exception as e:
    print(f"Error processing file: {e}")

print(f'\nSummary:')
print(f'Total frames: {frame_count}')
print(f'Ethernet 802.2: {ethernet_802_2_count}')
print(f'Ethernet 802.3 (Raw): {ethernet_802_3_count}')
print(f'Ethernet II: {ethernet_II_count}')
print(f'\tIPv4: {ipv4_count}')
print(f'\tARP: {arp_count}')
print(f'Ethernet SNAP: {ethernet_snap_count}')
print(f'Total file size: {total_file_size}')