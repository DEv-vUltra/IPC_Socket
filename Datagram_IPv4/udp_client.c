#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6789
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    printf("Connected to UDP server. Type messages (Ctrl+D to quit):\n");

    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));

        socklen_t server_len = sizeof(server_addr);
        ssize_t bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                                          (struct sockaddr *)&server_addr, &server_len);
        if (bytes_received < 0) {
            perror("recvfrom");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("From server: %s", buffer);
    }

    printf("\nDisconnected.\n");
    close(sockfd);
    return 0;
}
