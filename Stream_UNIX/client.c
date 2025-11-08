#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>


#define SOCKET_PATH "/tmp/unix_socket_example"
#define BUFFER_SIZE 1024


int main(){
    int sock_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    // Create a UNIX domain stream socket 
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sock_fd  == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address 
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    //Connect to server

    if(connect(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1){
        perror("connect");
        exit(EXIT_FAILURE);
    }
    
    while(1){
        printf("Enter message to send (Ctrl + D to quit): ");
        if(fgets(buffer, BUFFER_SIZE, stdin) == NULL) break; // Ctrl + D to quit

        write(sock_fd, buffer, strlen(buffer));

        ssize_t num_read = read(sock_fd, buffer, BUFFER_SIZE - 1);

        if(num_read < 0){
            printf("Read error\n");
            break;
        }

        buffer[num_read] = '\0';
        printf("Response from server: [%s]", buffer);
    }

    //Clean up
    close(sock_fd);
    return 0;
}




