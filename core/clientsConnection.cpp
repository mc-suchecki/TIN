/// @author Jacek Witkowski
///
/// @brief Implementation of the ClientsConnection class

#include <stdio.h>
#include <sys/socket.h>

void Connection::init() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    //TODO throw Exception;
  }

  struct sockaddr_in servAddr;
  memset(&servAddr, 0, sizeof(servAddr)); //TODO replace with generic fill()
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portsNr);

  if(inet_pton(AF_INET, IP_ADDRESS, &servAddr.sin_addr)<=0) {
    //TODO throw Exception;
  }

  if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
    //TODO throw Exception;
  }

}

void Connection::killAll() {
  //TODO
}

void Connection::execute(string &command) {
  //TODO n = write(sockfd,buffer,strlen(buffer));
  /*    if (n < 0) 
  error("ERROR writing to socket");
  bzero(buffer,256);
  n = read(sockfd,buffer,255);
  if (n < 0) 
    error("ERROR reading from socket"); */
}