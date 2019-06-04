#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

char * okResp = "HTTP/1.1 200 OK\n\n";
char * notFoundResp = "HTTP/1.1 404 Not Found\n\n";
char * notAllowedResp = "HTTP/1.1 405 Method Not Allowed\n\n";

char * notFoundPage = "<!DOCTYPE html><html lang=\"en\"><head><meta charset\"utf-8\"><title>Index Page</title></head><body><h1 align=\"center\">File not found on this server.</h1> </body></html>\n";
char * notAllowedPage = "<!DOCTYPE html><html lang=\"en\"><head><meta charset\"utf-8\"><title>Index Page</title></head><body><h1 align=\"center\">Request verb not accepted.</h1> </body></html>\n";


void
exitIfError(int condition, char * msg) {
  if (condition) {
    perror(msg);
    exit(1);
  }
  return;
}

void
copyFileToSocket(FILE * fp, int sockfd) {
  size_t bufsize = 1024;
  char buf[bufsize];
  
  while (1) {
    int cnt = fread(buf,1,bufsize,fp);
    fprintf(stderr,"read %d\n",cnt);
    if (cnt <= 0) break;
    cnt = write(sockfd,buf,cnt);
    fprintf(stderr,"wrote %d\n",cnt);
  }
  fclose(fp);
  return;
}

#define BUFFER_SIZE 1024
void
handleClient(int clntfd) {
  printf("handleClient fd %d\n",clntfd);
  
  char * buffer = alloca(BUFFER_SIZE);
  bzero(buffer,BUFFER_SIZE);
  
  int cnt = read(clntfd,buffer,BUFFER_SIZE);
  fprintf(stderr,"read %d bytes from client socket\n",cnt);
  exitIfError(cnt < 0, "Error reading from client socket.");
  
  fprintf(stderr,"buffer \"%s\"\n",buffer);
  
  char * ptr = buffer;
  char * verb = strsep(&ptr," \t\r\n");
  char * path = strsep(&ptr," \t\r\n");
  
  if (strcmp(verb,"GET") == 0) {
    if (strcmp(path,"/") == 0) {
      strcat(path,"index.html");
    }
  
    fprintf(stderr,"returning %s\n",path);
  
    char * fullpath = alloca(BUFFER_SIZE + strlen(path));
    getcwd(fullpath,BUFFER_SIZE);
    strcat(fullpath,path);
  
    fprintf(stderr,"fullpath %s\n",fullpath);
  
    FILE * fp = fopen(fullpath,"r");
    if (fp) {
      write(clntfd,okResp,strlen(okResp));
      copyFileToSocket(fp,clntfd);
    }
    else {
      fprintf(stderr,"not found\n");
      write(clntfd,notFoundResp,strlen(notFoundResp));
      write(clntfd,notFoundPage,strlen(notFoundPage));
    }
  }
  else {
    write(clntfd,notAllowedResp,strlen(notAllowedResp));
    write(clntfd,notAllowedPage,strlen(notAllowedPage));
  }
  
  close(clntfd);
  fprintf(stderr,"Finished client\n");
  exit(0);
}

int
main(int argc, char * argv[]) {
  
  int sockfd = socket(AF_INET,SOCK_STREAM,0);
  exitIfError(sockfd < 0,"Error on socket open.");
  
  struct sockaddr_in srvrAddr;
  bzero((char *) &srvrAddr,sizeof(srvrAddr));
  int portNum = 8080;
  
  srvrAddr.sin_family = AF_INET;
  srvrAddr.sin_addr.s_addr = INADDR_ANY;
  srvrAddr.sin_port = htons(portNum);
  
  int ret = bind(sockfd, (struct sockaddr *) &srvrAddr, sizeof(srvrAddr));
  exitIfError(ret,"Error on bind.");
  
  listen(sockfd,5);
  
  fprintf(stderr,"listening on %d\n",portNum);
  
  struct sockaddr_in clntAddr;
  socklen_t clntLen = sizeof(clntAddr);
  
  signal(SIGCHLD,SIG_IGN);
  
  while (1) {
    int clntfd = accept(sockfd, (struct sockaddr *) &clntAddr, &clntLen);
    exitIfError(clntfd < 0,"Error on accept.");
    
    char clntip[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET,(struct inaddr *)&clntAddr.sin_addr,clntip,sizeof(clntip));
    fprintf(stderr,"accepted %s\n",clntip);
    
    int pid = fork();
    exitIfError(pid < 0,"Error on fork.");
    
    if (pid == 0) {
      close(sockfd);
      handleClient(clntfd);
      exit(0);
    }
    else {
      close(clntfd);
    }
  }
}
