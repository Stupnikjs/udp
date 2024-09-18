#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  // for close()

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char* message = "Hello from UDP Client!";
    
    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);  // Server IP address

    // Send message to the server
    sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("Message sent to server: %s\n", message);

    // Receive response from the server
    socklen_t server_len = sizeof(server_addr);
    int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &server_len);
    
    if (recv_len < 0) {
        perror("Error receiving data");
    } else {
        buffer[recv_len] = '\0';  // Null-terminate the received string
        printf("Received message from server: %s\n", buffer);
    }

    // Close the socket
    close(sockfd);
    return 0;
}
