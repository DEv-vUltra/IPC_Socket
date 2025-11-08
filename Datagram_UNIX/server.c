#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_PATH "/tmp/unix_dgram_server"
#define BUFFER_SIZE 256

int main() {
    int sockfd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];

    // Create a datagram socket in the Unix domain
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Remove any existing socket file
    unlink(SERVER_PATH);

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running and waiting for messages from client...\n");

    while (1) {
        client_len = sizeof(client_addr);

        // Receive message from client
        ssize_t recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                                    (struct sockaddr *)&client_addr, &client_len);
        if (recv_len < 0) {
            perror("recvfrom");
            continue;
        }

        buffer[recv_len] = '\0'; // Null-terminate the received string
        printf("Received: %s\n", buffer);

        // Print client address (sun_path)
        if (client_addr.sun_path[0] != '\0') {
            printf("Client address (sun_path): %s\n", client_addr.sun_path);
            printf("Send back to client....\n");
        } else {
            printf("ERROR!!! Cannot send back to Client. Client address is unnamed or abstract.\n");
        }


        // Prepare response message
        char response[BUFFER_SIZE];
        snprintf(response, BUFFER_SIZE, "Server receive: %s", buffer);

        // Send response back to client
        sendto(sockfd, response, strlen(response), 0,
               (struct sockaddr *)&client_addr, client_len);
    }

    // Clean up
    close(sockfd);
    unlink(SERVER_PATH);
    return 0;
}
