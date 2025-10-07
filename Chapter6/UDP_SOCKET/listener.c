/*
** listener.c -- a datagram sockets "server" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT "4950" // the port users will be connecting to

#define MAXBUFLEN 100


// Return a pointer to the IP address (IPv4 or IPv6) inside a sockaddr structure.
void *get_in_addr(struct sockaddr *sa){
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void){
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
  hints.ai_socktype = SOCK_DGRAM;// use UDP
  hints.ai_flags = AI_PASSIVE; // use my IP
  struct addrinfo *servinfo;
  int rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo);
  if (rv != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  struct addrinfo *p;//just an iterator
  int sockfd;
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("listener: socket");
      continue;
    }
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("listener: bind");
      continue;
    }
    break;
  }if (p == NULL) {
    fprintf(stderr, "listener: failed to bind socket\n");
    return 2;
  }

  freeaddrinfo(servinfo); // all done with this structure

  printf("listener: waiting to recvfrom...\n");

  while(1){
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len = sizeof their_addr;
    int numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len);
    if (numbytes == -1) {
      perror("recvfrom");
      exit(1);
    }

    char s[INET6_ADDRSTRLEN];
    const void *ip_address_pointer = get_in_addr((struct sockaddr *)&their_addr);
    const char *client_address = inet_ntop(their_addr.ss_family, ip_address_pointer, s, sizeof s);
    printf("listener: got packet from %s\n", client_address);
    printf("listener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buf);
  }
  close(sockfd);

  return 0;
}
