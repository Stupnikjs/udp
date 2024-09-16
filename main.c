#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  // Add this header to use close()


#define PORT 8080
#define BUFFER_SIZE 1024


int main() {
    int sockfd, connfd;

    // sockaddr_in TYPE 
    struct sockaddr_in server_addr, client_addr;
      
    // socketlen_t TYPE
    socklen_t client_len = sizeof(client_addr);
    
     
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    // SOCK_DGRAM smecify UDP protocol
    // IPPROTO_UDP to 0 also mean UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    
    // Bind socket to address and port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    printf("UDP Server listening on port %d...\n", PORT);

    while (1) {
        // Receive datagram from client
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);

        // Process incoming data
        printf("Received message from client: %s\n", buffer);

        // Send response back to client
        char* response = "Hello from UDP Server!";
        sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&client_addr, client_len);
    }

    close(sockfd);
    return 0;
    
}


// send data with this command echo "Hello from netcat!" | nc -u 127.0.0.1 8080
