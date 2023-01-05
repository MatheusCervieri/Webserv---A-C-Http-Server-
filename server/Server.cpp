/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:16:53 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/05 11:01:51 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*To create a simple HTTP server using the above structs, you can follow these steps:


Set the socket to listen for incoming connections.
In a loop, accept incoming connections and create a new thread or process to handle the request.
In the thread or process handling the request, read the HTTP request and parse it to extract the request method, URL, headers, and body.
Use the URL and the defined routes in the configuration file to determine the location of the requested resource and the allowed HTTP methods for that route.
If the request method is allowed for the route, check if the route specifies any CGI programs that need to be executed. If a CGI program is specified, execute it and send the output as the response body.
If no CGI program is specified, check if the route specifies a root directory. If a root directory is specified, search for the requested resource in that directory and send the file as the response body if it exists.
If the request method is not allowed for the route or the requested resource does not exist, send an appropriate error response.
Close the socket and clean up any resources used by the thread or process.*/


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
    return (0);
}

void Server::accept_connections() {
  while (true) {
    // iterate through all of the sockets
    std::vector<int>::iterator it;
    for (it = this->sockets.begin(); it != this->sockets.end(); ++it) {
      int sockfd = *it;
      // accept an incoming connection
      struct sockaddr_in cli_addr;
      socklen_t clilen = sizeof(cli_addr);
      //accept create a new socket!
      int newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);

      if (newsockfd < 0) {
        // there was an error accepting the connection
        std::cerr << "Error accepting connection" << std::endl;
      } else {
        pid_t pid = fork();

        if (pid < 0) {
            // error occurred
            std::cerr << "Error creating new process" << std::endl;
        } else if (pid == 0) {
            // this is the child process
            this->read_request_data(newsockfd, 1024);
            // handle the request here
        } else {
            // this is the parent process
            // continue accepting connections
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

    
    return (0);
}

int Server::handle_request_data()
{
    
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


