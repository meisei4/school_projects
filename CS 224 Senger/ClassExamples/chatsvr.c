#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX_CLIENTS 100

struct client {
  int fd;
  char * name;
};

int maxClientFD = 0;

struct client clients[MAX_CLIENTS];
int nextClient = 0;

void
exitIfError(int condition, char * msg) {
  if (condition) {
    perror(msg);
    exit(1);
  }
  return;
}

void
addClients(fd_set * fds) {
  for (int i = 0; i < nextClient; i++) {
    FD_SET(clients[i].fd,fds);
    if (clients[i].fd > maxClientFD) maxClientFD = clients[i].fd;
  }
}

void
forwardMessage(int exceptIndex, char * name, char * buf, int buflen) {
  for (int j = 0; j < nextClient; j++) {
    if (j != exceptIndex) {
      dprintf(clients[j].fd,"%s says:: ",name);
      write(clients[j].fd,buf,buflen);
    }
  }
}

void
requireNickMessage(int clientIndex, char * buf) {
  char name[16];
  if (sscanf(buf,"nick %15s",name) == 1) {
    clients[clientIndex].name = strdup(name);
    char msg[32];
    sprintf(msg,"%s has joined\n",clients[clientIndex].name);
    forwardMessage(clientIndex,"SERVER",msg,strlen(msg));
  }
  else {
    dprintf(clients[clientIndex].fd,"Error: need \"nick name\" message first.\n");
  }
}

void
closeClient(int index) {
  
  close(clients[index].fd);
  char msg[32];
  sprintf(msg,"%s has left\n",clients[index].name);
  free(clients[index].name);
  if (nextClient-1 > index) {
    clients[index] = clients[nextClient-1];
  }
  nextClient--;
  forwardMessage(-1,"SERVER",msg,strlen(msg));
}

void
handleClients(fd_set * fds) {
  int bufsize = 1450;
  char * buf = alloca(bufsize);

  for (int i = 0; i < nextClient; i++) {
    if (FD_ISSET(clients[i].fd,fds)) {
      int cnt = read(clients[i].fd,buf,bufsize);
      if (cnt > 0) {
        if (!clients[i].name) {
          requireNickMessage(i,buf);
        }
        else {
          forwardMessage(i,clients[i].name,buf,cnt);
        }
      }
      else {
        closeClient(i);
      }
    }
  }
}

int
main(int argc, char * argv[]) {
  
  int sockfd = socket(AF_INET,SOCK_STREAM,0);
  exitIfError(sockfd < 0,"Error on socket open.");
  
  struct sockaddr_in srvrAddr;
  bzero((char *) &srvrAddr,sizeof(srvrAddr));
  int portNum = 5436;
  
  srvrAddr.sin_family = AF_INET;
  srvrAddr.sin_addr.s_addr = INADDR_ANY;
  srvrAddr.sin_port = htons(portNum);
  
  int ret = bind(sockfd, (struct sockaddr *) &srvrAddr, sizeof(srvrAddr));
  exitIfError(ret,"Error on bind.");
  
  listen(sockfd,5);
  
  fprintf(stderr,"listening on %d\n",portNum);
  
  struct sockaddr_in clntAddr;
  socklen_t clntLen = sizeof(clntAddr);
  
  while (1) {
    // setup FD set consisting of server socket and clients
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sockfd,&fds);
    maxClientFD = sockfd;
    addClients(&fds);
    int ret = select(maxClientFD+1,&fds,NULL,NULL,NULL);
    if (ret > 0) {
      if (FD_ISSET(sockfd,&fds)) {
        int clntfd = accept(sockfd, (struct sockaddr *) &clntAddr, &clntLen);
        exitIfError(clntfd < 0,"Error on accept.");
        
        char clntip[INET_ADDRSTRLEN] = {0};
        inet_ntop(AF_INET,(struct inaddr *)&clntAddr.sin_addr,clntip,sizeof(clntip));
        fprintf(stderr,"accepted %s\n",clntip);
        
        clients[nextClient].fd = clntfd;
        nextClient++;
      }
      else {
        handleClients(&fds);
      }
    }
  }
}
