#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>


#define SOCKET_PATH "/tmp/unix_socket_example"
#define BUFFER_SIZE 1024
#define BACKLOG 5

int main(){
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];
    // Create a UNIX domain stream socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(server_fd == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Remove any existing socket file
    unlink(SOCKET_PATH);

    // Set up the socket address structure

    memset(&addr, 0 ,sizeof(struct sockaddr_un ));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    unlink(SOCKET_PATH);

    // Bind the socket to the specified path
    if(bind(server_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1){
        perror("bind");
        exit(EXIT_FAILURE);
    
    }

    /*Listen for incoming connections
     Accept maximum waiting connection in queue
    When multiple clients attempt to connect to the server at the same time
    but the server has not called Accept() in response, those connections are queued*/
    if (listen(server_fd, BACKLOG) == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Server is listening at %s.....\n", SOCKET_PATH);
    for(;;) { /*Handle client connections iteratively*/

        /*Accept a connection. The connection is returned on a new socket, client_fd
        ; the listening socket "server_fd" remains open and can be used to accept further 
        connections */
        client_fd = accept(server_fd, NULL, NULL); 
        if(client_fd == -1){
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("We see a client !!!\n");


        // Receive and echo messages from the client
        while(1){
            ssize_t num_read = read(client_fd, buffer, BUFFER_SIZE - 1); // Block here until receive some messages from client
            if(num_read == 0){
                printf("Client disconnect [%ld]", num_read);
                break;
            }

            if(num_read < 0){
                printf("Read error [%ld]", num_read);
                break;
            }

            buffer[num_read] = '\0';
            printf("Received from client: %s", buffer);

            char response[BUFFER_SIZE];
            snprintf(response, BUFFER_SIZE, "Server received: %s", buffer);
            write(client_fd, response, strlen(response));
        }
    }

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);

}