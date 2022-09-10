//
// Created by mas on 9/5/22.
//

#include "Server.h"
#include "Constants.h"
#include <arpa/inet.h>

Server::Server(const Site &site, const std::string& ip_addr)
: m_site(site)
{
    if (ip_addr.empty())
    {
        throw std::invalid_argument("ip_addr is empty");
    }
    m_addr.sin_family = AF_INET;
    if (ip_addr.find(':') == std::string::npos)
    {
        m_addr.sin_port = htons(Constants::DEFAULT_PORT);
        inet_pton(AF_INET, ip_addr.c_str(), &m_addr.sin_addr);
    }
    else
    {
        m_addr.sin_port = htons(std::stoi(ip_addr.substr(ip_addr.find(':') + 1)));
        inet_pton(AF_INET6, ip_addr.substr(0, ip_addr.find(':')).c_str(), &m_addr.sin_addr);
    }
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        throw std::runtime_error("socket failed");
    }
    if (bind(m_server_fd, (struct sockaddr *)&m_addr, sizeof(m_addr)) < 0)
    {
        throw std::runtime_error("bind failed");
    }
}

void Server::run()
{
    if (listen(m_server_fd, 3) < 0)
    {
        throw std::runtime_error("listen failed");
    }
    while (true)
    {
        int new_socket;
        sockaddr_in address;
        socklen_t addrlen = sizeof(address);
        if ((new_socket = accept(m_server_fd, (struct sockaddr *)&address, &addrlen)) < 0)
        {
            throw std::runtime_error("accept failed");
        }
        char buffer[1024] = { 0 };
        read(new_socket, buffer, 1024);
        std::string request = buffer;
        // get the path from the request
        std::string path = request.substr(request.find(' ') + 1);
        path = path.substr(0, path.find(' '));
        std::string response_content = m_site.get_page(path)->get_rendered();
        std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(response_content.length()) + "\n\n" + response_content;
        send(new_socket, response.c_str(), response.length(), 0);
        close(new_socket);
    }
}
