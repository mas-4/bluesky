//
// Created by mas on 9/5/22.
//

#ifndef BLUESKY_SERVER_H
#define BLUESKY_SERVER_H

#include <unordered_map>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include "Site.h"


class Server
{
private:
    Site m_site;
    sockaddr_in m_addr = { 0 };
    int m_server_fd;
public:

    Server(const Site &site, const std::string& ip_addr);
    ~Server() = default;
    void run();
};


#endif //BLUESKY_SERVER_H
