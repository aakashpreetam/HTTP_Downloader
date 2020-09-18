/*
============================================================
Author: Aakash Preetam
Email ID: cs19mtech11012@iith.ac.in
Implement HTTP based Downloader to Download a Given Object
Complile: g++ Assignment1.cpp -o Assignment1
Run: ./Assignment1 http://www.iith.ac.in/~aravind/cs6010.html
============================================================
*/

#include <iostream>
#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>

using namespace std;

int download(char *line)
{
    struct addrinfo *result,*p;  //to store results
    struct addrinfo hints;       //to indicate information we want
    int sockfd;
    struct sockaddr_in *saddr;   //to reference address
    int s; //for error checking

    string url=line;
    size_t found = url.find_first_of(":");
    string protocol=url.substr(0,found); 
    string url_new=url.substr(found+3); //url_new is the url excluding the http part
    size_t found1 =url_new.find_first_of(":");
    //string port =url_new.substr(0,found1);

    size_t found2 = url_new.find_first_of("/");
    string domain =url_new.substr(found1+1,found2-found1-1);
    string path =url_new.substr(found2);
    size_t found3 = path.find_first_of(".");
    string extension=path.substr(found3+1);
    size_t found4 = path.find_last_of("/");
    string object_name=path.substr(found4+1,found3-found4-1);
    cout<< "Protocol: " <<protocol << endl;
    cout<< "Domain: " <<domain << endl;
    cout<< "Path: " << path << endl;
    cout<< "Extension: " << extension << endl;
    cout<< "Object Name: " << object_name << endl;

    memset(&hints,0,sizeof(struct addrinfo));  //zero out hints
    hints.ai_family = AF_INET; //we only want IPv4 addresses
    const char *hostname=const_cast<char*>(domain.c_str());
    const char *path_c=const_cast<char*>(path.c_str());
    const char *extension_c=const_cast<char*>(extension.c_str());
    const char *object_name_c=const_cast<char*>(object_name.c_str());

    //Convert the hostname to an address
    if( (s = getaddrinfo(hostname, "http", &hints, &result)) != 0) // returns 0 on success
    {
        fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(s));
        exit(1);
    }

    //convert generic socket address to inet socket address
    saddr = (struct sockaddr_in *) result->ai_addr;

    //print the address
    printf("IP Address: %s\n", inet_ntoa(saddr->sin_addr));

    //open a socket
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0))  < 0)
    {
        perror("socket");
        exit(1);
    }

    //connect to the server
    if(connect(sockfd, (struct sockaddr *) saddr, sizeof(*saddr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    char request[400];
    sprintf(request,"GET %s HTTP/1.1\r\nHost: %s \r\nConnection: Keep-Alive \r\n\r\n",path_c,hostname);

    if(write(sockfd,request,strlen(request)) < 0)
    {
        perror("send");
        return 1;
    }

    char server_reply[10000];
    int received_len=0;
    int flag_length=0;
    int total_length=0;
    char str[5];
    int file_size=0;
    int i=0;
    FILE *file=NULL;
    char filename[20];
    char *body;
    sprintf(filename,"%s.%s",object_name_c,extension_c);
    cout<<"Filename: " << filename << endl;
    file = fopen(filename, "ab");
    while(1)
    {
        received_len = recv(sockfd, server_reply , sizeof server_reply , 0);
        
        if( received_len < 0 ){
            puts("recv failed");
            break;
        }

        if(flag_length==0)
        {   
            flag_length=1;
            char *clen=strstr(server_reply,"Content-Length: ");
            clen=clen+16;
            while(*clen!='\r')
            {
                str[i]=*clen;
                i++;
                clen++;
            }
            file_size=atoi(str);
            body = strstr(server_reply, "\r\n\r\n");
            if(body!=NULL)
            {
                body=body+4; // move ahead 4 chars
                int len=body-server_reply;
                fwrite(body, received_len-len , 1, file);
                total_length=total_length+received_len-len;
            }
        }
        else
        {
            body=server_reply;
            fwrite(body, received_len , 1, file);
            total_length=total_length+received_len;
        }

        if( total_length >= file_size ){
            break;
        }
        bzero(server_reply,0);
        bzero(body,0);
        bzero(request,0);
    }

    cout<<"Content-Length: "<< file_size << endl;

    //free the addrinfo struct
    freeaddrinfo(result);
    freeaddrinfo(p);
    fclose(file);
    printf("Download Finished\n");
    return 0;
}

int main(int argc, char *argv[])
{

    if(argc<2)
    {
        printf("Enter File Name.\n");
        return 0;
    }

    FILE* tfile = fopen(argv[1], "r"); /* should check the result */
    char line[256];

    if(tfile == NULL)
    {
        perror("Error opening file");
        return(-1);
    }

    while(fgets(line, sizeof(line), tfile))
    {   
        // remove newline
        int len = strlen(line);
        if( line[len-1] == '\n' )
            line[len-1] = 0;
            
        printf("%s\n",line);
        
        download(line);        
    }

    fclose(tfile);

    return 0;
}