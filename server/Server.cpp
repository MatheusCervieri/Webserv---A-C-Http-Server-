/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:16:53 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/05 10:00:43 by mvieira-         ###   ########.fr       */
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

        // add the socket file descriptor to the vector
        this->sockets.push_back(sockfd);
    }
  return 0;
}


