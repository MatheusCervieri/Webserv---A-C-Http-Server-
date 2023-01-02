#include "webserv.hpp"

#define PORT 8000
#define BACKLOG 10
#define BUFFER_SIZE 1024

//A server listen for connection on a specif port, and for each connection, it reads the 
//request from the client and sends a response back.
//To test this simple server we can use curl http://localhost:8000/

int basic_server() {

  int sockfd, new_fd; //this store the socked file descriptor and the new connection descriptor.

  struct sockaddr_in server_addr, client_addr; //this store the server adress and the client adress

  socklen_t sin_size; //store the size of the client adress struct.

  int yes = 1;
  
  char buffer[BUFFER_SIZE]; //store the request of the client;

  int nbytes; //Number of bytis received from the client. 

  // This function creates a socket and returns a file descriptor;
    //Adress Family: AF_INET, which refers to the Internet Protocol version 4 (IPv4) protocol.
    //The type of socket: SOCK_STREAM, which refers to a stream socket. Stream sockets are used for connection-oriented protocols, such as TCP.
    //The protocol --> system will choose the apropiate protocol. 
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        std::cout << "socket: " << strerror(errno) << std::endl;
        return (1);
    }


  // Set socket options to reuse the address and port. This is useful when the server needs to be restarted quickly and allows the server to bind to the same adress and port without having to wait for the operation systm to release than
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    std::cout << "setsockopt: " << strerror(errno) << std::endl;
    exit(1);
  }

    // Initialize the server address struct
    //The memset function is called to clear the memory for the server_addr struct.
    //The sin_family field of the server_addr struct is set to AF_INET, indicating that the server will use an IPv4 address.
    //The sin_port field of the server_addr struct is set to the value of PORT, in network byte order.
    //The sin_addr.s_addr field of the server_addr struct is set to INADDR_ANY, indicating that the server will listen on all available network interfaces.
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind the socket to the address and port
  /*In networking, the term "bind" refers to the process of associating a socket with a specific network address and port number. When a socket is bound to a specific address and port,
   it can only receive data that is sent to that address and port.
   */
  //It uses the server adress struct.
  if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    std::cout << "bind: " << strerror(errno) << std::endl;
    exit(1);
  }

  // Listen for incoming connections
  // The backlog size determines the maximum size of this queue. If the queue is full and a new connection request arrives, the connection request will be rejected.
  if (listen(sockfd, BACKLOG) == -1) {
    std::cout << "listen: " << strerror(errno) << std::endl;
    exit(1);
  }

  printf("Listening on port %d\n", PORT);

  // Accept incoming connections
  sin_size = sizeof(client_addr);
  //It enters a loop to accept incoming connections and handle them. The loop continues until the accept function returns -1, indicating an error.
    //Within the loop, it reads the request from the client using the recv function and stores it in a buffer.
  while ((new_fd = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size)) != -1) {
    // Read the request from the client
    if ((nbytes = recv(new_fd, buffer, BUFFER_SIZE, 0)) == -1) 
    {
      std::cout << "recv: " << strerror(errno) << std::endl;
      continue;
    }

    // Print the request
    printf("Received request:\n%s", buffer);

    // Send the response
    const char* response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 11\n\nHello World";
    if (send(new_fd, response, strlen(response), 0) == -1) {
      std::cout << "send: " << strerror(errno) << std::endl;
      continue;
    }

    close(new_fd);
  }

  close(sockfd);

  return 0;
}