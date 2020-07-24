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

/*Tunnel header for route*/
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

void print_routehdr(char *buffer,int size)
{
	struct tunnel_route_header *rhdr = (struct tunnel_route_header *)buffer;
	
	printf("Struct Route Header\n");
	printf("peer_ip:%u\n",ntohl ((unsigned int)rhdr->peer_ip));
	printf("peer_port:%u\n",ntohs((unsigned short)rhdr->peer_port));
}

void print_interhdr(char *buffer,int size){
	struct tunnel_inter_header *ihdr = (struct tunnel_inter_header *)(buffer+sizeof(struct tunnel_route_header));
	printf("Struct Inter Header\n");
	printf("dst_ip:%u\n",ihdr->dst_ip);
	printf("dst_port:%u\n",ntohs((unsigned short)ihdr->dst_port));
	printf("src_ip:%u\n",ihdr->src_ip);
	printf("src_port:%u\n",ntohs((unsigned short)ihdr->src_port));
}

void print_payload(char *buffer,int size){
	char *msg = (buffer+sizeof(struct tunnel_route_header)+sizeof(struct tunnel_inter_header));
	printf("payload is:%s\n",msg);
}
int main(void)
{
	int sockfd;
	if((sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
		ERR_EXIT("socket");
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
		ERR_EXIT("bind");

	char recvbuf[120];
	struct sockaddr_in cliaddr;
	socklen_t len;	
	
	while(1)
	{
		len = sizeof(servaddr);
		
		
		recvfrom(sockfd,recvbuf,120,0,(struct sockaddr*)&cliaddr,&len);
		print_routehdr(recvbuf,6);
		print_interhdr(recvbuf,12);
		print_payload(recvbuf,100);
		printf("\n");
	}
	close(sockfd);
	return 0;
}
