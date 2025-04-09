# TCP Handshake Client Implementation

The project implements the client side of a TCP three-way handshake using raw sockets.

## 1. Assignment Features Implemented

- Client-side implementation of a modified TCP three-way handshake using raw sockets
- SYN packet construction with correct sequence number (200)
- Reception and parsing of SYN-ACK response from server with correct sequence number (400)
- Final ACK packet transmission with correct sequence number (600)
- Complete handshake process with proper verification


## 2. Design Decisions

### Raw Socket Usage
- Used raw sockets to have full control over IP and TCP headers, allowing manual construction of packets with specific sequence numbers
- This approach provides deeper understanding of the TCP protocol compared to using standard socket APIs

### Single-Threaded Implementation
- The client uses a single-threaded approach as the handshake process is sequential by nature
- No need for multiple threads since we're only handling one connection and following a strict request-response pattern

### Packet Filtering
- Implemented packet filtering to only process packets from the expected server and port

### Sequence Numbers
- Used predefined sequence numbers as specified by examining server code:
  - Client SYN: SEQ=200
  - Server SYN-ACK: SEQ=400, ACK=201
  - Client ACK: SEQ=600, ACK=401

## 3. Implementation
---

### **Function: `print_tcp_flags`**
**Purpose:** Display the TCP control flags and sequence number.

#### **Algorithm:**
1. Read the values of the following flags from the TCP header:
   - SYN
   - ACK
   - FIN
   - RST
   - PSH
2. Read the sequence number from the TCP header.
3. Convert the sequence number from network to host byte order.
4. Print the values of the flags and sequence number to the console.

---

###  **Function: `send_syn`**
**Purpose:** Send a SYN packet from the client to the server to initiate a TCP 3-way handshake.

#### **Algorithm:**
1. Create a buffer to hold both IP and TCP headers.
2. Initialize the IP header:
   - Set version to IPv4.
   - Set header length to 5 words.
   - Set total length to size of packet buffer.
   - Assign ID, TTL, and TCP protocol.
   - Set source IP as client IP (`127.0.0.1`).
   - Set destination IP as the server's address.
3. Initialize the TCP header:
   - Set source and destination ports.
   - Set sequence number to a constant (e.g., 200).
   - Set acknowledgment number to 0.
   - Set data offset to 5.
   - Set SYN flag to 1.
   - Leave other flags as 0.
   - Set TCP window size.
4. Set checksum to 0 (let kernel handle it).
5. Send the packet using `sendto()` to the server.
6. If sending fails, print error and exit.
7. Else, print that SYN packet was sent with its sequence number.

---

###  **Function: `send_ack`**
**Purpose:** Send the final ACK packet to complete the TCP 3-way handshake.

#### **Algorithm:**
1. Create a buffer to hold both IP and TCP headers.
2. Initialize the IP header:
   - Set version, header length, total length, TTL, ID, and TCP protocol.
   - Set source IP as client IP.
   - Set destination IP from the server’s address.
3. Initialize the TCP header:
   - Use the received TCP header to extract destination and source ports (reverse them).
   - Set a fixed sequence number (e.g., 600).
   - Set acknowledgment number to (received sequence number + 1).
   - Set data offset to 5.
   - Set ACK flag to 1.
   - Leave SYN and other flags as 0.
   - Set TCP window size.
4. Set checksum to 0.
5. Send the ACK packet using `sendto()`.
6. If sending fails, print error and exit.
7. Else, print the sent ACK and sequence number.

---

###  **Function: `main()`**
**Purpose:** Simulate a TCP 3-way handshake using raw sockets (SYN → SYN-ACK → ACK).

#### **Algorithm:**
1. Create a raw socket using `IPPROTO_TCP`.
2. Enable IP header inclusion in socket options.
3. Define and initialize the server’s socket address (loopback address and server port).
4. Send the initial SYN packet using `send_syn()`.
5. Enter a loop to wait for a SYN-ACK:
   - Use `recvfrom()` to receive packets.
   - If receive fails, print error and retry.
   - Cast buffer to IP and TCP headers.
   - Filter packets:
     - Must be TCP packets destined for client port.
     - Must originate from the defined server port.
   - Display TCP flags using `print_tcp_flags()`.
   - If packet is SYN-ACK and has expected sequence number (400):
     - Print details.
     - Call `send_ack()` to complete handshake.
     - Exit loop.
6. Print success message.
7. Close the socket and return.

---
### Code Flow
1. Socket creation and configuration
2. SYN packet construction and transmission
3. Waiting for SYN-ACK response
4. Filtering incoming packets to find the correct SYN-ACK
5. Verifying sequence numbers from server
6. Constructing and sending final ACK
7. Confirming handshake completion

## 4. Testing

### Correctness Testing
- Tested the client against the provided server implementation
- Verified correct sequence number handling
- Confirmed proper flag settings in all packets
- Checked proper handshake completion verification

### Testing Process
1. Build the project as mentioned 
2. Start the server in one terminal
3. Run the client in another terminal
4. Observe log messages from both client and server
5. Verify that all three handshake steps complete successfully

## 5. Restrictions

- The implementation is designed to work with the specific server provided in the assignment
- Only works with the predefined sequence numbers (200, 400, 600)
- Limited to localhost communication (127.0.0.1)
- Fixed client and server ports (CLIENT_PORT=54321, SERVER_PORT=12345)

## 6. Challenges

### Understanding Raw Sockets
- Learning the detailed structure of IP and TCP headers
- Figuring out the correct byte order (network vs host) for various fields

### Packet Filtering
- Ensuring we only process relevant packets in a raw socket environment where we receive all TCP traffic
- Implementing proper filtering logic to identify the expected SYN-ACK response

### Sequence Number Handling
- Understanding the expected sequence number flow for the modified handshake
- Ensuring proper acknowledgment of received sequence numbers

## 7. Requirements

- Linux operating system
- Root privileges (required for raw sockets)
- g++ compiler with C++17 support

## 8. Building the Project

Use the provided Makefile to build the client:

```bash
make client
```

To build both client and server:

```bash
make all
```

## 9. Running the Implementation

1. First, start the server in one terminal:

```bash
sudo ./server
```

2. Then, run the client in another terminal:

```bash
sudo ./client
```

Note: Root privileges are required because the application uses raw sockets.


## 10. Debugging

The application prints detailed information about each step of the handshake process, including:
- Flags of sent and received TCP packets
- Sequence and acknowledgment numbers
- Success/failure status of each operation

## 11. Troubleshooting

If you encounter issues:
- Ensure you're running with root privileges (`sudo`)
- Check that no firewall rules are blocking the communication
- Verify the server is running and listening on port 12345
- Make sure both client and server are using the same localhost address (127.0.0.1)

## 12. Contribution of Each Member

| Member | Contribution |
|--------|--------------|
| Chinmay Pillai (200298) | 1/3 |
| Ashutosh Dwivedi (200214) | 1/3 |
| Shubham Kumar (200967) | 1/3 |

The team members have given equal contribution throughout the project. All the task inclusive but not limited to coding, understanding of code, creation of readme file etc. involves each team member at some point of time.

## 13. Declaration

We declare that we did not indulge in plagiarism and all code has been written by our team members. Any references used have been properly cited in the Sources Referred section.

## 14. Feedback

- The assignment provided a valuable hands-on experience with low-level networking concepts
- Examining the server code helped understand the expected client behavior
- It would be helpful to have additional guidance on debugging raw socket implementations


## 15. Sources Referred

- Lecture notes
- Linux raw socket programming documentation
- Linux man pages for socket, iphdr, and tcphdr structures