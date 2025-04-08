# TCP Handshake Client Implementation

This project implements the client side of a TCP three-way handshake using raw sockets as part of Assignment 3 for CS425: Computer Networks.

## Overview

The client implements the TCP three-way handshake protocol:
1. Client sends SYN packet (SEQ=200)
2. Server responds with SYN-ACK packet (SEQ=400, ACK=201)
3. Client sends ACK packet (SEQ=600, ACK=401)

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