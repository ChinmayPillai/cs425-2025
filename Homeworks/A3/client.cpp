#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 12345  // Server's listening port
#define CLIENT_PORT 54321  // Client's port

// Function to print TCP header flags
void print_tcp_flags(struct tcphdr *tcp) {
    std::cout << "[+] TCP Flags: "
              << " SYN: " << tcp->syn
              << " ACK: " << tcp->ack
              << " FIN: " << tcp->fin
              << " RST: " << tcp->rst
              << " PSH: " << tcp->psh
              << " SEQ: " << ntohl(tcp->seq) << std::endl;
}

// Function to send SYN packet to initiate handshake
void send_syn(int sock, struct sockaddr_in *server_addr) {

    // Allocate memory for packet
    char packet[sizeof(struct iphdr) + sizeof(struct tcphdr)];
    memset(packet, 0, sizeof(packet));

    // Set up IP and TCP header pointers
    struct iphdr *ip = (struct iphdr *)packet;
    struct tcphdr *tcp = (struct tcphdr *)(packet + sizeof(struct iphdr));

    // Fill IP header
    ip->ihl = 5;                                    // Header length (5 words * 4 bytes/word = 20 bytes)
    ip->version = 4;                                // IPv4 
    ip->tos = 0;                                    // Type of service
    ip->tot_len = htons(sizeof(packet));            //Length of the packet
    ip->id = htons(12345);                          // Unique ID for the packet
    ip->frag_off = 0;                               // Fragment offset
    ip->ttl = 64;                                   // Time to live
    ip->protocol = IPPROTO_TCP;
    // Send ACK from client (source) to server (destination)
    ip->saddr = inet_addr("127.0.0.1");             // Client address
    ip->daddr = server_addr->sin_addr.s_addr;       // Server address

    // Fill TCP header
    tcp->source = htons(CLIENT_PORT);               // Source is Client port
    tcp->dest = htons(SERVER_PORT);                 // Destination is Server port    
    tcp->seq = htonl(200);                          // Set sequence number to 200 for SYN packet
    tcp->ack_seq = 0;                               // Not an ACK
    tcp->doff = 5;                                  // Data offset (5 words * 4 bytes/word = 20 bytes)
    //Set Appropriate Flags
    tcp->syn = 1;                                   // SYN flag true
    tcp->ack = 0;                                   // Not an ACK
    tcp->window = htons(8192);                      // Window size
    tcp->check = 0;                                 // Kernel will compute checksum if needed
    // tcp->urg_ptr = 0;

    // Send packet
    if (sendto(sock, packet, sizeof(packet), 0, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0) {
        // If sendto fails, print error message and exit
        perror("sendto() failed");
        exit(EXIT_FAILURE);
    } else {
        // If sendto succeeds, print the sent packet information
        std::cout << "[+] Sent SYN packet with SEQ=" << ntohl(tcp->seq) << std::endl;
    }
}

// Function to send ACK packet to complete handshake
void send_ack(int sock, struct sockaddr_in *server_addr, struct tcphdr *received_tcp) {
    // Allocate memory for the packet
    char packet[sizeof(struct iphdr) + sizeof(struct tcphdr)];
    memset(packet, 0, sizeof(packet));

    // Set up IP and TCP header pointers
    struct iphdr *ip = (struct iphdr *)packet;
    struct tcphdr *tcp = (struct tcphdr *)(packet + sizeof(struct iphdr));

    // Fill IP header
    ip->ihl = 5;                                    // Header length (5 words * 4 bytes/word = 20 bytes)
    ip->version = 4;                                // IPv4
    ip->tos = 0;                                    // Type of service
    ip->tot_len = htons(sizeof(packet));            // Length of the packet
    ip->id = htons(12346);                          // Unique ID for the packet
    ip->frag_off = 0;                               // Fragment offset
    ip->ttl = 64;                                   // Time to live
    ip->protocol = IPPROTO_TCP;                     // TCP protocol
    // Send ACK from client (source) to server (destination)
    ip->saddr = inet_addr("127.0.0.1");             // Client address
    ip->daddr = server_addr->sin_addr.s_addr;       // Server address

    // Fill TCP header

    //Set source and destination ports by switching received TCP ports
    tcp->source = received_tcp->dest;               // Source port
    tcp->dest = received_tcp->source;               // Destination port
    tcp->seq = htonl(600);                          // Set sequence number to 600
    tcp->ack_seq = htonl(ntohl(received_tcp->seq) + 1);  // Set ACK seq number to server's sequence number + 1
    //Set Appropriate Flags
    tcp->doff = 5;                                  // Data offset (5 words * 4 bytes/word = 20 bytes)
    tcp->syn = 0;                                   // Not SYN
    tcp->ack = 1;                                   // ACK flag
    tcp->window = htons(8192);                      // Window size
    tcp->check = 0;                                 // Kernel will compute the checksum
    // tcp->urg_ptr = 0;

    // Send packet
    if (sendto(sock, packet, sizeof(packet), 0, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0) {
        // If sendto fails, print error message and exit
        perror("sendto() failed");
        exit(EXIT_FAILURE);
    } else {
        // If sendto succeeds, print the sent packet information
        std::cout << "[+] Sent ACK packet with SEQ=" << ntohl(tcp->seq) 
                  << " and ACK=" << ntohl(tcp->ack_seq) << std::endl;
    }
}

int main() {
    // Create raw socket
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Enable IP header inclusion
    int one = 1;
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("setsockopt() failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send SYN packet
    send_syn(sock, &server_addr);

    // Receive and process SYN-ACK
    char buffer[65536];
    struct sockaddr_in source_addr;
    socklen_t addr_len = sizeof(source_addr);
    bool handshake_complete = false;

    // Wait for SYN-ACK and send final ACK
    while (!handshake_complete) {

        // Receive packet
        int data_size = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&source_addr, &addr_len);

        // Check if packet was successfully received
        if (data_size < 0) {
            perror("Packet reception failed");
            continue;
        }

        // Cast buffer to IP and TCP headers
        struct iphdr *ip = (struct iphdr *)buffer;
        struct tcphdr *tcp = (struct tcphdr *)(buffer + (ip->ihl * 4));

        // Filter packets: only process TCP packets from the server to our client port
        if (ntohs(tcp->dest) != CLIENT_PORT || ntohs(tcp->source) != SERVER_PORT) {
            continue;
        }

        // Print received packet information
        print_tcp_flags(tcp);

        // If it's a SYN-ACK packet with expected sequence number (400)
        // then print and send final ACK to complete the handshake
        if (tcp->syn == 1 && tcp->ack == 1 && ntohl(tcp->seq) == 400) {
            std::cout << "[+] Received SYN-ACK from server with SEQ=" << ntohl(tcp->seq) 
                      << " and ACK=" << ntohl(tcp->ack_seq) << std::endl;
            
            // Send final ACK to complete the handshake
            send_ack(sock, &server_addr, tcp);

            // Update flag to get out of the loop
            handshake_complete = true;
        }
    }

    std::cout << "[+] TCP three-way handshake completed successfully!" << std::endl;
    close(sock);
    return 0;
}