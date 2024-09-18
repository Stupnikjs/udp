const c = @cImport({
    @cInclude("stdio.h");
    @cInclude("stdlib.h");
    @cInclude("string.h");
    @cInclude("sys/socket.h");
    @cInclude("netinet/in.h");
    @cInclude("arpa/inet.h");
});


pub fn main() !void {
const BUFFER_SIZE: c_int = 1024;
var sockfd: c_int = c.socket(c.AF_INET, c.SOCK_DGRAM, 0);
if (sockfd < 0) {
    c.perror("Error creating socket");
    return 1;
}

var server_addr = c.struct_sockaddr_in{
    .sin_family = c.AF_INET,
    .sin_port = c.htons(c.PORT),
    .sin_addr = c.inet_addr("127.0.0.1"),
};

if (c.bind(sockfd, &server_addr, c.sizeof(c.struct_sockaddr_in)) < 0) {
    c.perror("Error binding socket");
    return 1;
}

c.printf("UDP Server listening on port %d...\n", c.PORT);

while (true) {
    var client_addr = c.struct_sockaddr_in{};
    var client_len: c_int = c.sizeof(c.struct_sockaddr_in);
    var buffer = [BUFFER_SIZE]c_char{};
    var bytes_received = c.recvfrom(sockfd, &buffer, &client_len, 0, &client_addr, &client_len);

    var message = c.malloc(bytes_received + 64); // Allocate memory for the message (received data + extra space for client address)
    c.sprintf(message, "Received message from %s: %s\n", c.inet_ntoa(client_addr.sin_addr), buffer);
    c.printf(message);
    c.sendto(sockfd, message, bytes_received, 0, &client_addr, client_len);
    c.free(message);
}
}