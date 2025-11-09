#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 6789
#define BUFFER_SIZE 65536

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; //IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    // inet_pton(AF_INET, "x.x.x.x", &server_addr.sin_addr); //Listen on the LAN network
    // inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); //Listen on the same host
    server_addr.sin_port = htons(PORT); // Should use htons

    // Bind
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept connection
    client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Print client info
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    //printf("Client connected: IP = %s, Port = %d\n", client_ip, ntohs(client_addr.sin_port));

    // Echo loop
    ssize_t bytes_read;
    while ((bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        //printf("Received from client: %s\n", buffer);

        char response[BUFFER_SIZE];
        //snprintf(response, BUFFER_SIZE, "Server receive: %s", buffer);
        write(client_fd, response, strlen(response));
    }

    printf("Client disconnected.\n");
    close(client_fd);
    close(server_fd);
    return 0;
}
