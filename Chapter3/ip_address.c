#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>


struct sockaddr_in sa; // IPv4
struct sockaddr_in6 sa6; // IPv6
char ip4[INET_ADDRSTRLEN];
char ip6[INET6_ADDRSTRLEN];

int main(){

  if (inet_pton(AF_INET, "10.12.110.255", &(sa.sin_addr)) > 0){
    puts("Ipv4 conversion succes");
    inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);
    printf("The IPv4 address is: %s\n", ip4);
  }else{
    puts("Ipv4 conversion error");
  }
  if (inet_pton(AF_INET6, "2001:db8:5413:4028::9db9", &(sa6.sin6_addr)) > 0){
    puts("Ipv6 conversion succes");
    inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);

    printf("The address is: %s\n", ip6);
  }else{
    puts("Ipv6 conversion error");
  }
  
}
