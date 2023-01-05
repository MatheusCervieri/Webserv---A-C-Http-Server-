/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 09:28:15 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/05 09:59:25 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.hpp"

class Server {

  private:
    std::vector<Config> servers_conf;
    std::vector<int> sockets;

  public:
    Server();
    Server(std::vector<Config> servers_conf);
    ~Server();
    int start();
    int create_sockets();
};

#endif