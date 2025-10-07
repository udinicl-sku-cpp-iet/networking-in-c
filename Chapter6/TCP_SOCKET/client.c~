/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>


#define PORT "3490" // the port client will be connecting to

#define MAXDATASIZE 128 // max number of bytes we can get at once


// Return a pointer to the IP address (IPv4 or IPv6) inside a sockaddr structure.
void *get_in_addr(struct sockaddr *sa){
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
  int sockfd;

  if (argc != 2) {
    fprintf(stderr,"usage: ./client <hostname>\n");
    exit(1);
  }
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  struct addrinfo  *servinfo;
  int rv = getaddrinfo(argv[1], PORT, &hints, &servinfo); 
  if (rv != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  struct addrinfo *p;// iterator on linked list
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }
    char s[INET6_ADDRSTRLEN];
    inet_ntop(p->ai_family,get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    printf("client: attempting connection to %s\n", s);

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      perror("client: connect");
      close(sockfd);
      continue;
    }
    break;
  }

  if (p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    return 2;
  }
  
  char s[INET6_ADDRSTRLEN];
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
  printf("client: connected to %s\n", s);

  freeaddrinfo(servinfo); 
  char buf[MAXDATASIZE];
  int numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0);
  if ( numbytes == -1) {
    perror("recv");
    exit(1);
  }

  buf[numbytes] = '\0';

  printf("client: received '%s'\n",buf);

  close(sockfd);

  return 0;
}
