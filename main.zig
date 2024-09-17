const std = @import("std");
const net = @import("net");
const mem = @import("mem");

const PORT: usize = 8080;
const BUFFER_SIZE: usize = 1024;

var sockfd = net.socket(.{ .family = net.AddressFamily.INET4, .type = net.SocketType.DGRAM, .protocol = .{} });
if (sockfd == -1) {
    std.debug.print("Error creating socket: {}\n", .{@errorName(std.os.errno)});
    return 1;
}

var server_addr = net.sockaddr_in{
    .sin_family = net.AddressFamily.INET4,
    .sin_port = net.htons(PORT),
    .sin_addr = net.inet4_pton(.{ .ip_v4 = std.net.ip_v4{ .octets = [127, 0, 0, 1] } }),
};

if (net.bind(sockfd, &server_addr, .PORT) != 0) {
    std.debug.print("Error binding socket: {}\n", .{@errorName(std.os.errno)});
    return 1;
}

std.debug.print("UDP Server listening on port {}...\n", .{PORT});

while (true) {
    var client_addr = net.sockaddr_in{};
    var client_len = net.SizeOf(net.sockaddr_in);
    var buffer = [BUFFER_SIZE]u8{};
    const bytes_received = net.recvfrom(sockfd, &buffer, &client_addr, &client_len);

    var message = std.mem.convertToString(net.ip_v4_to_str(&client_addr.sin_addr));
    message.append(": ");
    message.append(buffer[0..bytes_received]);
    std.debug.print("Received message from {}: {}\n", .{message});

    const response: []const u8 = "Hello from Zig UDP Server!";
    _ = net.sendto(sockfd, &response, .client_addr, .client_len);
}