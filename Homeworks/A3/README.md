# TCP Handshake Client Implementation

The project implements the client side of a TCP three-way handshake using raw sockets.

## Overview

The client implements the TCP three-way handshake protocol:
1. Client sends SYN packet (SEQ=200)
2. Server responds with SYN-ACK packet (SEQ=400, ACK=201)
3. Client sends ACK packet (SEQ=600, ACK=401)

## Description for implemented functions

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

## Requirements

- Linux operating system
- Root privileges (required for raw sockets)
- g++ compiler with C++17 support

## Building the Project

Use the provided Makefile to build the client:

```bash
make client
```

To build both client and server:

```bash
make all
```

## Running the Implementation

1. First, start the server in one terminal:

```bash
sudo ./server
```

2. Then, run the client in another terminal:

```bash
sudo ./client
```

Note: Root privileges are required because the application uses raw sockets.

## Implementation Details

The client implementation follows these steps:

1. **Initialization**: Creates a raw socket and configures it to include custom IP headers
2. **SYN Packet**: Sends an SYN packet with sequence number 200
3. **SYN-ACK Processing**: Receives and processes the server's SYN-ACK response (sequence 400)
4. **ACK Packet**: Sends the final ACK with sequence number 600 to complete the handshake

## Debugging

The application prints detailed information about each step of the handshake process, including:
- Flags of sent and received TCP packets
- Sequence and acknowledgment numbers
- Success/failure status of each operation

## Troubleshooting

If you encounter issues:
- Ensure you're running with root privileges (`sudo`)
- Check that no firewall rules are blocking the communication
- Verify the server is running and listening on port 12345
- Make sure both client and server are using the same localhost address (127.0.0.1)