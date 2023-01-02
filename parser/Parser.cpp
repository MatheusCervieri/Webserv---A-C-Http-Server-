/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 09:43:44 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/02 17:21:21 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "webserv.hpp"


//Acabar o parser.
//Lidar com erros basicos do arquivo de configuração. 
//Fazer a conexão do parser com um servidor simples. 

//The first server for a host:port will be the default for this host:port (that means
//it will answer to all the requests that don’t belong to an other server).
//Limit client body size.
//• Setup routes with one or multiple of the following rules/configuration (routes wont
//be using regexp):
//The routes configuration. 

Parser::Parser()
{ 
  
}

Parser::Parser(char *file_name) : file_name(file_name)
{
  
}

Parser::~Parser() { }

int Parser::parse() {
  int server_flag = 0;
  int location_flag = 0; 
  Config current_server;
  Route current_route; 
  std::ifstream file(this->file_name);
  
  if (!file.is_open()) {
    std::cout << "Error opening configuration file: " << this->file_name << std::endl;
    return 1;
  }

  std::string line;
  while (std::getline(file, line)) {
  // Read a line from the file and store it in the 'line' string. The while loop will continue as long as there are more lines to read in the file.
  std::istringstream line_stream(line);
  // Create a new string stream object and initialize it with the contents of the 'line' string. This allows us to use the stream operators (e.g. '>>') to extract values from the string.
  std::string key;
  // Declare a string variable to store the key value for the current line.
  line_stream >> key;
  // Read the first word from the line and store it in the 'key' variable.
  if (key.empty()) 
    continue;
  // If the 'key' variable is empty, skip the rest of the loop body and move on to the next iteration.
  if (location_flag == 1)
  {
    
    if (key == "allow_methods") 
    {
      // If the 'key' variable is "allowed_method", read all the remaining values from the line and store them in the 'this->server_conf.allowed_methods' vector.
      std::string value;
      while (line_stream >> value) 
      {
        current_route.allowed_methods.push_back(value);
      }
    }
     else if (key == "root")
    {
      std::string value;
      line_stream >> value;
      if (!value.empty()) 
      current_route.root = value;
    }
     else if (key == "autoindex")
    {
      std::string value;
      line_stream >> value;
      if (!value.empty()) 
      current_route.autoindex = value;
    }
    else if (key == "client_max_body_size")
    {
      std::string value;
      line_stream >> value;
      if (!value.empty()) 
      current_route.root = atoi(value.c_str());
    }
    else if (key == "cgi")
    {
     std::string value;
      while (line_stream >> value) 
      {
        current_route.cgi.push_back(value);
      }
    }
    if (line.find('}') != std::string::npos) {
    location_flag = 0;
    current_server.routes.push_back(current_route);
    } 
    else {
    
    }

  }

  else 
  {
  
  if (key == "server") {
    if (server_flag == 0)
    {
      current_server = Config();
      server_flag = 1;
    }
    else
    {
      this->servers_conf.push_back(current_server);
      current_server = Config();
    }
  }
  else if (key == "location")
  {
    
    location_flag = 1;
    current_route = Route();
    std::string value;
    line_stream >> value;
    if (!value.empty()) 
    current_route.location_dir = value;
    
  }
  else if (key == "listen") 
  {
    // If the 'key' variable is "listen", read the next value from the line and store it in the 'this->server_conf.listen_port' field.
    std::string value;
    line_stream >> value;
    if (!value.empty()) 
     current_server.listen_port = atoi(value.c_str());
  } 
  else if (key == "server_name") 
  {
    // If the 'key' variable is "server_name", read the next value from the line and store it in the 'this->server_conf.server_name' field.
    std::string value;
    line_stream >> value;
    if (!value.empty()) 
     current_server.server_name = value;
  } 
  else if (key == "autoindex") 
  {
    // If the 'key' variable is "server_name", read the next value from the line and store it in the 'this->server_conf.server_name' field.
    std::string value;
    line_stream >> value;
    if (!value.empty()) 
     current_server.autoindex = value;
  } 
  else if (key == "index") 
  {
    // If the 'key' variable is "index", read all the remaining values from the line and store them in the 'this->server_conf.index_files' vector.
    std::string value;
    while (line_stream >> value) 
    {
     current_server.index_files.push_back(value);
    }
  } 
  else if (key == "allowed_method") 
  {
    // If the 'key' variable is "allowed_method", read all the remaining values from the line and store them in the 'this->server_conf.allowed_methods' vector.
    std::string value;
    while (line_stream >> value) 
    {
     current_server.allowed_methods.push_back(value);
    }
  } 
  else if (key == "root") {
    // If the 'key' variable is "root", read the next value from the line and store it in the 'this->server_conf.root_dir' field.
    std::string value;
    line_stream >> value;
    if (!value.empty()) 
     current_server.root_dir = value;
  } 
  else if (key == "error_page") {
    // If the 'key' variable is "error_page", read the next value from the line and store it in the 'this->server_conf.error_pages' vector.
    std::string value;
    line_stream >> value;
    if (!value.empty()) 
     current_server.error_pages.push_back(value);
    }
  }
  
  }
  this->servers_conf.push_back(current_server);
  return (0);
  }


std::vector<Config> Parser::getServersConf() const {
  return this->servers_conf;
}
