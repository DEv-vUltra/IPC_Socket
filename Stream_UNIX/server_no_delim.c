/*
 * SERVER – KHÔNG CÓ BỘ PHÂN TÁCH
 * Gửi 2 message liên tiếp: "Hello" và "World"
 * Client có thể nhận dính thành "HelloWorld"
 */

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define SOCKET_PATH "/tmp/unix_sock_no_delim"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;

    // Tạo socket UNIX dạng STREAM
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    // Xóa file socket cũ nếu tồn tại
    unlink(SOCKET_PATH);

    // Cấu hình địa chỉ socket
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    // Bind
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));

    // Lắng nghe kết nối
    listen(server_fd, 5);

    printf("Server waiting for connection...\n");
    client_fd = accept(server_fd, NULL, NULL);
    printf("Client connected!\n");

    // Gửi 2 chuỗi liên tiếp — KHÔNG phân tách
    send(client_fd, "Hello", 5, 0);
    send(client_fd, "World", 5, 0);

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
