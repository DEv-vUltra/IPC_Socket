#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_PATH "/tmp/unix_dgram_server"
#define CLIENT_PATH "/tmp/unix_dgram_client"
#define BUFFER_SIZE 256

int main() {
    int sockfd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    // Create a datagram socket in the Unix domain
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Remove any existing client socket file
    unlink(CLIENT_PATH);

    // Set up the client address structure
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, CLIENT_PATH, sizeof(client_addr.sun_path) - 1);

    // Bind the socket to the client address
    if (bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);

    // Get user input
    printf("Enter message to send: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

    // Send message to server
    sendto(sockfd, buffer, strlen(buffer), 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Receive response from server
    ssize_t recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, NULL, NULL); //Block here until receive data form server
    if (recv_len >= 0) {
        buffer[recv_len] = '\0';
        printf("Received from server: %s\n", buffer);
    } else {
        perror("recvfrom");
    }

    // Clean up
    close(sockfd);
    unlink(CLIENT_PATH);
    return 0;
}
