# file_name = input('Enter the name of a binary file to read (ethersXX.bin): ')
file_name = "ethers09.bin"
frame_count = 0

ipv4_count = 0
novell802_2_count = 0
novell802_3_count = 0
arp_count = 0
ethernet_II_count = 0
ethernet_snap_count = 0

MAXIMUM_FRAME_LENGTH = int('05dc', 16)
IPV4_TYPE = '0800'
ARP_TYPE = '0806'
VALID_ETHER_TYPE = int('0600', 16)
ETHERNET_SNAP = 'aaaa'
NOVELL_802_3 = 'ffff'

try:
    with open(file_name, "rb") as file:
        while True:
            # Read destination and source MAC addresses
            destination_address = file.read(6)
            sender_address = file.read(6)
            
            # Check if we've reached end of file
            if not destination_address or not sender_address:
                break
                
            frame_count += 1
            print(f'\nFrame #{frame_count}')
            print(f'MAC addresses:')
            print(f'  Destination address: {destination_address.hex()}')
            print(f'  Sender address: {sender_address.hex()}')

            # Read the next 2 bytes which could be length or type
            length_or_type_bytes = file.read(2)
            if not length_or_type_bytes:
                break
                
            length_or_type = length_or_type_bytes.hex()
            length_or_type_int = int(length_or_type, 16)

            # Check if it's an Ethernet II frame (type field > 0x0600)
            if length_or_type_int > VALID_ETHER_TYPE:
                if length_or_type == IPV4_TYPE:
                    # IPv4 frame
                    ip_header = file.read(20)
                    if len(ip_header) < 20:
                        break
                        
                    # Extract IP header length
                    ihl = ip_header[0] & 0x0F  # IP Header Length in 4-byte words
                    ip_header_bytes = ihl * 4
                    
                    # Extract total length
                    total_length = int.from_bytes(ip_header[2:4], byteorder='big')
                    
                    print(f"  Total Length: {total_length}")
                    print(f"  IP Header Length: {ip_header_bytes} bytes")
                    
                    # Calculate and skip IP payload
                    ip_payload_bytes = total_length - ip_header_bytes
                    file.seek(ip_payload_bytes, 1)
                    ipv4_count += 1
                    print("  Frame type: IPv4")
                
                elif length_or_type == ARP_TYPE:
                    # ARP frame - skip 28 bytes for ARP payload
                    file.seek(28, 1)
                    arp_count += 1
                    print("  Frame type: ARP")

                else:
                    # Other Ethernet II frame
                    # For Ethernet II, the type field tells us what comes next,
                    # but we don't know the length, so we need to read until end of frame
                    # In practice, you might need different handling here
                    ethernet_II_count += 1
                    print("  Frame type: Ethernet II")
                    # Note: We can't easily skip here without knowing frame length
                    # This is a limitation of the current approach
                    
            else:
                # IEEE 802.3 frame with length field
                frame_length = length_or_type_int
                
                # Read DSAP and SSAP
                dsap_ssap_bytes = file.read(2)
                if not dsap_ssap_bytes:
                    break
                    
                dsap_ssap = dsap_ssap_bytes.hex()
                
                if dsap_ssap == ETHERNET_SNAP:
                    # Ethernet SNAP frame
                    # Skip the remaining frame (minus the 2 bytes we already read for DSAP/SSAP)
                    remaining_bytes = frame_length - 2
                    file.seek(remaining_bytes, 1)
                    ethernet_snap_count += 1
                    print("  Frame type: Ethernet SNAP")

                elif frame_length <= MAXIMUM_FRAME_LENGTH:
                    # Novell 802.2 frame
                    # Skip the remaining frame (minus the 2 bytes we already read for DSAP/SSAP)
                    remaining_bytes = frame_length - 2
                    file.seek(remaining_bytes, 1)
                    novell802_2_count += 1
                    print("  Frame type: Novell 802.2")
                
                elif length_or_type == NOVELL_802_3:
                    # Novell 802.3 frame
                    # Skip the remaining frame (minus the 2 bytes we already read for DSAP/SSAP)
                    remaining_bytes = frame_length - 2
                    file.seek(remaining_bytes, 1)
                    novell802_3_count += 1
                    print("  Frame type: Novell 802.3")
                else:
                    # Unknown or unhandled frame type
                    print(f"  Unknown frame type, length: {frame_length}")
                    # Skip the remaining frame
                    remaining_bytes = frame_length - 2
                    file.seek(remaining_bytes, 1)

except FileNotFoundError:
    print(f"Error: The file {file_name} was not found.")
except Exception as e:
    print(f"Error processing file: {e}")

print(f'\nSummary:')
print(f'Total frames: {frame_count}')
print(f'IPv4: {ipv4_count}')
print(f'ARP: {arp_count}')
print(f'Novell 802.2: {novell802_2_count}')
print(f'Novell 802.3: {novell802_3_count}')
print(f'Ethernet II: {ethernet_II_count}')
print(f'Ethernet SNAP: {ethernet_snap_count}')