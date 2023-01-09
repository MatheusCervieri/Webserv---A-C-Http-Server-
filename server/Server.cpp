/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:16:53 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/09 09:38:59 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"



Server::Server() {
  // Default constructor
}

Server::Server(std::vector<Config> servers_conf) {
  this->servers_conf = servers_conf;
  this->start();
}

Server::~Server() {
  // Destructor
}

int Server::start()
{
    this->create_sockets();
    this->accept_connections();
    return (0);
}

void Server::accept_connections() {
    int nfds = this->sockets.size();
    struct pollfd fds[nfds];
    

    for (int i = 0; i < nfds; i++) {
      fds[i].fd = this->sockets[i];
      fds[i].events = POLLIN;
    }
  while (true)
  {
    
    // call the poll function
    /*The value of TIMEOUT_MS should be the maximum time in milliseconds that the poll function should wait for events to occur. This value can be set to any integer, depending on the specific requirements of your server. For example, you might set TIMEOUT_MS to 1000 to have the poll function wait for 1 second before returning. Alternatively, you might set TIMEOUT_MS to -1 to have the poll function wait indefinitely until an event occurs.

    It is important to consider the value of TIMEOUT_MS carefully, as it can affect the performance and responsiveness of your server. If you set TIMEOUT_MS to a low value, the server will be able to respond more quickly to incoming requests, but it may also consume more CPU resources. If you set TIMEOUT_MS to a high value, the server may be slower to respond to incoming requests, but it will consume fewer CPU resources.*/
    int ret = poll(fds, nfds, -1); //-1 is the value of the timeout.
    if (ret < 0) 
    {
      // there was an error with the poll function
      std::cerr << "Error with poll function" << std::endl;
      return;
    } 
    else if (ret == 0) 
    {
      // the poll function timed out
      return;
    } 
    else 
    {
        // iterate through the file descriptors and check the event flags
      for (int i = 0; i < nfds; i++) 
      {
        int sockfd = fds[i].fd;
        if (fds[i].revents & POLLIN) 
        {
        struct sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr); //store the size of the client adress
        //accept create a new socket!
        int newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if (newsockfd < 0) {
          // there was an error accepting the connection
          std::cerr << "Error accepting connection" << std::endl;
        } 
        else 
        {
            this->read_request_data(newsockfd, 1024);
            //handle request!
            //send a response based on the request!
            this->send_basic_response(newsockfd);
            //close(newsockfd);
            //this->close_sockets_fd();

        }
        }
      }
    }
  }

}

int Server::read_request_data(int socket ,int request_buf_size)
{
    char request_buf[request_buf_size]; // buffer to store the request data
    int bytes_received = recv(socket, request_buf, request_buf_size, 0);

    // check for errors
    if (bytes_received < 0) {
        std::cerr << "Error reading from connection" << std::endl;
        return 1;
    }

    std::cout << "Request Buff" << std::endl << request_buf << std::endl;
    
    
    
    return (0);
}

int Server::handle_request_data()
{
    return (0);   
}

int Server::send_basic_response(int socketfd)
{
    const char* response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 11\n\nHello World";
    if (send(socketfd, response, strlen(response), 0) == -1) 
    {
      std::cout << "send: " << strerror(errno) << std::endl;
      return (1);
    }
    return (0);
}

int Server::create_sockets() {
    std::vector<Config>::iterator it;
    for (it = this->servers_conf.begin(); it != this->servers_conf.end(); ++it) 
    {
        const Config& server_conf = *it;

        // create a new socket
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Error creating socket" << std::endl;
            return 1;
        }

        // bind the socket to the port specified in the configuration file
        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(server_conf.listen_port);
        
        if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "Error binding socket to port " << server_conf.listen_port << std::endl;
            return 1;
        }
        
        if (listen(sockfd, SOMAXCONN) < 0) 
        {
            std::cerr << "Error setting socket to listen" << std::endl;
            return 1;
        }

        // add the socket file descriptor to the vector
        this->sockets.push_back(sockfd);
    }
  return 0;
}

void Server::close_sockets_fd()
{
    std::vector<int>::iterator it;
    for (it = this->sockets.begin(); it != this->sockets.end(); ++it) 
    {
        int sockfd = *it;
        close(sockfd);
    }
}


