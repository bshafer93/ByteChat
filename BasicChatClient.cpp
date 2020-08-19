#include "BasicChatClient.h"

BasicChatClient::BasicChatClient()
{
#ifdef _WIN32
    // Initialsize Sockets DLL for Windows
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed:" << std::endl;
    }
#endif



}

BasicChatClient::~BasicChatClient()
{
#ifdef _WIN32
    closesocket(sockfd);
#elif __linux__
    close(socketfd);
#endif

}

void BasicChatClient::Initialize()
{

    //Open TCP Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "socket() failed..." << std::endl;
    }

    //Fill out details of the server.
    serverInfo.sin_family = AF_INET;
#ifdef _WIN32
    InetPton(AF_INET, L"161.35.232.244", &serverInfo.sin_addr.s_addr);
#elif __linux__
    serveAddrInfo.sin_addr.s_addr = inet_addr("161.35.232.244");
#endif
    serverInfo.sin_port = htons(PORT);

    //Connect
    if (connect(sockfd, (struct sockaddr*) & serverInfo, sizeof(serverInfo)) == -1) {
        closesocket(sockfd);
        std::cout << "connect() failed..." << std::endl;
    }
    else {
        std::cout << "Connected to server!" << std::endl;
        serverConnected = true;
    }



}

void BasicChatClient::RecvMsg()
{
   
            std::vector<Packet> recPackets;

            int BytesRead =0;
            char buff[1024] = { 0 };
            int bytesRecv = recv(sockfd, buff, 1023, 0);
            if (bytesRecv <= 0) {
                serverConnected = false;
                return;
            }
            std::cout << "Bytes recived:" << bytesRecv << std::endl;


            
            while (BytesRead < bytesRecv) {
                recPackets.push_back(Packet(buff + BytesRead ));
                BytesRead += recPackets.back().size;
            
            
            }
       
            for (auto p : recPackets) {

                switch (p.msgtype) {
                    case Packet::MSGTYPE::STRING:
                        //Store Incoming Messages
                        AddMessageToLog(p.data.str);
                        //Print to console for debugging
                        PrintMsg(*p.data.str);
                        break;
                    default:
                        break;
                }
            
            }

     


      



}

void BasicChatClient::SendMsg(std::string& msg)
{


    if (send(sockfd, msg.c_str(), msg.length(), 0) == -1) {
        std::cout << "send() failed..." << std::endl;
    };
    std::string log = "Message \"" + msg + "\" sent at " + GetCurTime();
    PrintMsg(log);
}

void BasicChatClient::PrintMsg(std::string msg)
{
    std::cout << msg << std::endl;
}

void BasicChatClient::SendPacket(int serverSockfd, Packet* p)
{


    if (send(serverSockfd, (char*)p->BuildPacket(), p->size, 0) == -1) {
        std::cout << "send() failed..." << std::endl;

    }
    else {

        std::string log = "Message sent at " + GetCurTime();
        PrintMsg(log);

    }



}

void BasicChatClient::SendString( std::string msg)
{
    Packet s(Packet::MSGTYPE::STRING, msg);

    if (send(sockfd, (char*)s.BuildPacket(), s.size, 0) == -1) {

        std::cout << "send() failed..." << std::endl;
    }

}

void BasicChatClient::SendCommand(int serverfd,Packet::COMMANDTYPE cmdtype, std::string arg)
{
    Packet cmd(Packet::MSGTYPE::COMMAND, cmdtype, arg);

    if (send(serverfd, (char*)cmd.BuildPacket(), cmd.size, 0) == -1) {

        std::cout << "send() failed..." << std::endl;
    }


}


void BasicChatClient::Run()
{

    while (serverConnected) {
        std::future<void> rec = std::async(std::launch::async,&BasicChatClient::RecvMsg, this);
        
    }

    std::cout << "Server disconnected" << std::endl;

}


void BasicChatClient::ParseUserInput(std::string line) {

    if (line[0] == '\\') {
        //TO LOOK AT...
        if (strncmp(line.c_str(), "\\setname", 8) == 0) {
            SendCommand(sockfd,Packet::COMMANDTYPE::SETNAME ,line.substr(9, line.size() - 8));

        }
        else if (strncmp(line.c_str(), "\\help", 5) == 0) {
            SendCommand(sockfd, Packet::COMMANDTYPE::HELP, "");
        }
        else if (strncmp(line.c_str(), "\\showonline", 11) == 0) {
            SendCommand(sockfd, Packet::COMMANDTYPE::LSONLINE, "");
        }
        else if (strncmp(line.c_str(), "\\setfontcolor", 13) == 0) {

            std::cout << "setting font color" << std::endl;
           //SendCommand(sockfd, Packet::COMMANDTYPE::FONTCOLOR, "");
        }
    }
    else {
        SendString(line);
    }

}

void BasicChatClient::AddMessageToLog(std::string* msg)
{
    
    message_log.push_back(*msg);
    if (message_log.size() >= 49) { 
        message_log.erase(message_log.begin(), message_log.begin()+1);
    }
}


std::string BasicChatClient::GetCurTime()
{
    std::chrono::system_clock::time_point cur = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(cur);
    char str[26];
    ctime_s(str, 26, &t);
    std::string time(str);
    return time;
}
