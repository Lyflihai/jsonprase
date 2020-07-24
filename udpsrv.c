#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>


#define ERR_EXIT(m)\
        do \
        {\
                perror(m); \
                exit(EXIT_FAILURE); \
        }while(0);

int main(void)
{
	int sockfd;
	if((sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
		ERR_EXIT("socket");
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5501);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	
	struct tunnel_route_header{
        	unsigned int peer_ip;
        	unsigned short peer_port;
	};

	/*Tunnel header for remote*/
	struct tunnel_inter_header{
        	unsigned char service_flags;
        	unsigned int dst_ip;
        	unsigned short dst_port;
        	unsigned int src_ip;
       		 unsigned short src_port;
	};

        
       
        char payload[100];
	struct tunnel_route_header rhdr;
	struct tunnel_inter_header ihdr;
	char temp[120];
	char recvbuf[120];
	rhdr.peer_ip = inet_addr("192.168.0.1");
	rhdr.peer_port = htons(5000);
	ihdr.service_flags = 0b1010;
	ihdr.dst_ip = inet_addr("10.0.0.1");
	ihdr.dst_port = htons(8585);
	ihdr.src_ip = inet_addr("10.0.0.2");
	ihdr.src_port = htons(8080);
	strncpy(payload,"zhangsan123",sizeof(payload));
	memset(temp,0,sizeof(temp));
	memcpy(temp,&rhdr,sizeof(struct tunnel_route_header));
	memcpy(temp+sizeof(struct tunnel_route_header),&ihdr,sizeof(struct tunnel_inter_header));
	memcpy(temp+sizeof(struct tunnel_route_header)+sizeof(struct tunnel_inter_header),&payload,sizeof(payload));
	struct sockaddr_in cliaddr;
	socklen_t len;
	
	char sendbuf[100];
		len = sizeof(servaddr);
		printf("Cli:");
		sendto(sockfd,temp,100,0,(struct sockaddr*)&servaddr,len);
		printf("Cli data has been sended!\n");
		recvfrom(sockfd,recvbuf,100,0,(struct sockaddr*)&servaddr,&len);
		printf("Cli:%s\n",recvbuf);
	close(sockfd);
	return 0;
}
