//
// Created by mas on 9/5/22.
//

#include "Server.h"
#include "Constants.h"
#include "Logger.h"
#include <arpa/inet.h>
#include <unistd.h>

Server::Server(const Site &site, const std::string &ip_addr)
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
    // set socket reuseaddr
    int opt = 1;
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt REUSEPORT");
        throw std::runtime_error("setsockopt failed");
    }
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt REUSEADDR");
        throw std::runtime_error("setsockopt failed");
    }
    if (bind(m_server_fd, (struct sockaddr *) &m_addr, sizeof(m_addr)) < 0)
    {
        perror("bind failed");
        throw std::runtime_error("bind failed");
    }
}

std::string get_mime_type(const std::string &path)
{
    std::string extension = path.substr(path.find_last_of('.'));
    if (Constants::MIME_TYPES.find(extension) == Constants::MIME_TYPES.end())
    {
        return "application/octet-stream";
    }
    return Constants::MIME_TYPES.at(extension);
}

std::string Server::construct_response(const std::string &request)
{
    size_t after_get = request.find("GET ") + 4;
    std::string path = request.substr(after_get, request.find(' ', after_get) - after_get);
    Logger::log("Request: " + path);
    if (path == "/")    // root is index.html
    {
        path = "/index.html";
    }
    if (path.find('.') == std::string::npos)    // no extension for html because of htaccess
    {
        path += ".html";
    }
    std::string response_content;

    if (m_site.has_page(path))
    {
        auto page = m_site.get_page(path);
        if (utils::get_last_modified(page->get_path()) > page->get_last_modified())
        {
            page->render();
        }
        response_content = page->get_rendered();
    }
    else if (m_site.has_file(path))
    {
        response_content = m_site.get_file(path);
    }
    else
    {
        response_content = "404 Not Found";
        return "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " +
               std::to_string(response_content.length()) + "\n\n" + response_content;
    }
    return "HTTP/1.1 200 OK\nContent-Type: " + get_mime_type(path)  + "\nContent-Length: " +
           std::to_string(response_content.length()) + "\n\n" + response_content;
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
        sockaddr_in address{};
        socklen_t addrlen = sizeof(address);
        if ((new_socket = accept(m_server_fd, (struct sockaddr *) &address, &addrlen)) < 0)
        {
            throw std::runtime_error("accept failed");
        }
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);
        std::string request = buffer;
        // get the path from the request
        std::string response = construct_response(request);
        send(new_socket, response.c_str(), response.length(), 0);
        close(new_socket);
    }
}

Server::~Server()
{
}
