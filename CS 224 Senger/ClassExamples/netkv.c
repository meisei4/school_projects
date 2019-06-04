#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "hashtbl.h"
#include "hashfn.h"

void
exitIfError(int condition, char * msg) {
  if (condition) {
    perror(msg);
    exit(1);
  }
  return;
}

#define TABLE_SIZE 1024

hashtbl * kvTbl;

hashtbl * cmdTbl;
enum Commands {ERR_CMD = 0, SET_CMD, GET_CMD, DEL_CMD};

void
initCommands() {
  cmdTbl = createTable(2*DEL_CMD,FNV_1a);
  setValueForKey(cmdTbl,"set",(void *)SET_CMD);
  setValueForKey(cmdTbl,"get",(void *)GET_CMD);
  setValueForKey(cmdTbl,"del",(void *)DEL_CMD);
}

void
processCommand(int fd, char * cmd) {
  char * ptr = cmd;
  char * verb = strsep(&ptr," \n");
  switch ((int) valueForKey(cmdTbl,verb,NULL)) {
    case SET_CMD: {
      char * key = strsep(&ptr," \n");
      char * val = strsep(&ptr," \n");
      if (key && val) {
        setValueForKey(kvTbl,strdup(key),strdup(val));
        dprintf(fd,"OK\n");
      }
      else {
        dprintf(fd,"?\n");
      }
    } break;
    case GET_CMD: {
      bool present;
      char * key = strsep(&ptr," \n");
      if (key) {
        char * val = valueForKey(kvTbl,key,&present);
        if (present) {
          dprintf(fd,"OK\n");
          dprintf(fd,"%s\n",val);
        }
        else {
          dprintf(fd,"NOT FOUND\n");
        }
      }
      else {
        dprintf(fd,"?\n");
      }
    } break;
    case DEL_CMD: {
      char * key = strsep(&ptr," \n");
      if (key) {
        removeKey(kvTbl,key);
        dprintf(fd,"OK\n");
      }
      else {
        dprintf(fd,"?\n");
      }
    } break;
    case ERR_CMD: {
      dprintf(fd,"?\n");
    } break;
  }
}

// key/values
// max-client-fd num
// fd:#.ip ip
// fd:#.port port
#define EXPECTED_CLIENTS 256
hashtbl * clntTbl;

void
addClient(int fd, char * ip, int port) {
  fprintf(stderr,"addClient %d %s %d\n",fd,ip,port);
  long maxClientFD = (int)valueForKey(clntTbl,"max-client-fd",NULL);
  if (fd > maxClientFD) maxClientFD = fd;
  setValueForKey(clntTbl,"max-client-fd",(void *)maxClientFD);

  char * ipkey = strf(NULL,"fd:%d.ip",fd);
  fprintf(stderr,"ipkey %s\n",ipkey);
  if (valueForKey(clntTbl,ipkey,NULL)) {
    fprintf(stderr,"fd %d already exists\n",fd);
    free(ipkey);
    close(fd);
    return;
  }
  
  setValueForKey(clntTbl,ipkey,strdup(ip));
  setValueForKey(clntTbl,strf(NULL,"fd:%d.port",fd),(void *)(long)port);
}

void
closeClient(int fd) {
  close(fd);
  
  char ipkey[16];
  char portkey[16];
  strf(ipkey,"fd:%d.ip",fd);
  strf(portkey,"fd:%d.port",fd);
  fprintf(stderr,"closeClient %d %s %d\n",fd,valueForKey(clntTbl,ipkey,NULL),(int)valueForKey(clntTbl,portkey,NULL));
  
  free(removeKey(clntTbl,ipkey));
  removeKey(clntTbl,portkey);
}

void
includeFDs(fd_set * fds) {
  int maxClientFD = (int)valueForKey(clntTbl,"max-client-fd",NULL);
  for (int i = 3; i <= maxClientFD; i++) {
    char key[16];
    if (!valueForKey(clntTbl,strf(key,"fd:%d.ip",i),NULL)) continue;
    FD_SET(i,fds);
  }
}

void
handleClients(fd_set * fds) {
  int bufsize = 256;
  char * buf = alloca(bufsize);
  
  int maxClientFD = (int)valueForKey(clntTbl,"max-client-fd",NULL);
  for (int fd = 0; fd <= maxClientFD; fd++) {
    char key[16];
    if (valueForKey(clntTbl,strf(key,"fd:%d.ip",fd),NULL) && FD_ISSET(fd,fds)) {
      int cnt = read(fd,buf,bufsize);
      // this is naive, cannot assume entire command is in
      // buffer or that buffer does not contain multiple commands
      if (cnt > 0) {
        buf[cnt] = '\0';
        processCommand(fd,buf);
      }
      else {
        closeClient(fd);
      }
    }
  }
}

int
main(int argc, char * argv[]) {
  
  initCommands();
  
  clntTbl = createTable(EXPECTED_CLIENTS,FNV_1a);
  
  kvTbl = createTable(TABLE_SIZE,FNV_1a);
  
  int sockfd = socket(AF_INET,SOCK_STREAM,0);
  exitIfError(sockfd < 0,"Error on socket open.");
  
  struct sockaddr_in srvrAddr;
  bzero((char *) &srvrAddr,sizeof(srvrAddr));
  int portNum = 5436;
  
  srvrAddr.sin_family = AF_INET;
  srvrAddr.sin_addr.s_addr = INADDR_ANY;
  srvrAddr.sin_port = htons(portNum);
  
  int iarg = 1;
  int ret = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(char *) &iarg, sizeof(iarg));
  
  iarg = 1;
  ret = setsockopt(sockfd,SOL_SOCKET,SO_REUSEPORT,(char *) &iarg, sizeof(iarg));
  
  ret = bind(sockfd, (struct sockaddr *) &srvrAddr, sizeof(srvrAddr));
  exitIfError(ret,"Error on bind.");
  
  listen(sockfd,5);
  
  fprintf(stderr,"listening on %d\n",portNum);
  
  struct sockaddr_in clntAddr;
  socklen_t clntLen = sizeof(clntAddr);
  
  setValueForKey(clntTbl,"max-client-fd",(void *)(long)sockfd);

  while (1) {
    // setup FD set consisting of server socket and clients
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sockfd,&fds);
    includeFDs(&fds);
    int ret = select((int)valueForKey(clntTbl,"max-client-fd",NULL)+1,&fds,NULL,NULL,NULL);
    if (ret > 0) {
      if (FD_ISSET(sockfd,&fds)) {
        int clntfd = accept(sockfd, (struct sockaddr *) &clntAddr, &clntLen);
        exitIfError(clntfd < 0,"Error on accept.");
        
        char clntip[INET_ADDRSTRLEN] = {0};
        inet_ntop(AF_INET,(struct inaddr *)&clntAddr.sin_addr,clntip,sizeof(clntip));
        int clntport = ntohs(clntAddr.sin_port);
        addClient(clntfd,clntip,clntport);
      }
      else {
        handleClients(&fds);
      }
    }
  }
}
