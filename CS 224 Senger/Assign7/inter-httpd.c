#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include <hiredis.h>

char * okResp = "HTTP/1.1 200 OK\n\n";
char * notFoundResp = "HTTP/1.1 404 Not Found\n\n";
char * notAllowedResp = "HTTP/1.1 405 Method Not Allowed\n\n";

char * pageHeader = "<!DOCTYPE html><html lang=\"en\"><head><meta charset\"utf-8\"><title>Index Page</title></head><body>";
char * pageFooter = "</body></html>\n";

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

// Redis connection info
redisContext *conn;
redisReply *reply;
redisReply * nextReply;
char * ipaddr = "138.49.184.162";
int port = 6379;

void
redisOpen() {
  struct timeval timeout = { 1, 500000 }; // 1.5 seconds
  conn = redisConnectWithTimeout(ipaddr, port, timeout);
  if (conn == NULL || conn->err) {
    if (conn) {
      printf("Connection error: %s\n", conn->errstr);
      redisFree(conn);
    } else {
      printf("Connection error: can't allocate redis context\n");
    }
    exit(1);
  }
}

void
redisClose() {
  redisFree(conn);
}

// HTTP response functions
void
returnWordsPage(int fd) {
  redisOpen();
  reply = redisCommand(conn, "ZRANGE words 0 -1");
  //-1 is max val i think
  //reply is now
  if (reply->type == REDIS_REPLY_ARRAY) {
    write(fd, okResp, strlen(okResp));
    write(fd,pageHeader,strlen(pageHeader));
    dprintf(fd, "<h2>All Words in All Intercepts</h2><br>");
    for (int j = 0; j < reply->elements; j++) {
      char * w = reply->element[j]->str;
      fprintf(stderr, "%u) %s\n", j, w);
      dprintf(fd, "<a href=\"words?%s\">%s</a> ", w, w);
    }
    write(fd, pageFooter, strlen(pageFooter));
  }
  else {
    // return error page for Redis reply
  }
  freeReplyObject(reply);
  redisClose();
}

//all Persons using WORD
//all Intercepts where WORD is used
void
returnWordPage(int fd, char * word) {
  redisOpen();
  reply = redisCommand(conn, "ZRANGE persons:%s 0 -1", word);
  if (reply->type == REDIS_REPLY_ARRAY) {
    write(fd, okResp, strlen(okResp));
    write(fd,pageHeader,strlen(pageHeader));
    dprintf(fd, "<h2>All Persons using Word \"%s\"</h2>", word);
    for (int j = 0; j < reply->elements; j++) {
      char * w = reply->element[j]->str;
      fprintf(stderr, "%u) %s\n", j, w);
      //if word link to persons?person
      //if intercept link to intercepts?some-intercept
      dprintf(fd, "<a href=\"persons?%s\">%s</a> ", w, w);
    }
  }
  else {
    // return error page for Redis reply
  }
  freeReplyObject(reply);
  reply = redisCommand(conn, "SMEMBERS intercepts:%s", word);
  if (reply->type == REDIS_REPLY_ARRAY) {
    dprintf(fd, "<h2>All intercepts containing word \"%s\"</h2>", word);
    for (int j = 0; j < reply->elements; j++) {
      char * w = reply->element[j]->str;
      fprintf(stderr, "%u) %s\n", j, w);
      dprintf(fd, "<a href=\"intercepts?%s\">%s</a> ", w, w);
    }
    write(fd, pageFooter, strlen(pageFooter));
  }
  else {
    // return error page for Redis reply
  }
  freeReplyObject(reply);
  redisClose();
}

//all Contacts of PERSON
//all Words PERSON has used
//all Intercepts PERSON is in
void
returnPersonPage(int fd, char * person) {
  redisOpen();
  reply = redisCommand(conn, "SMEMBERS contacts:%s", person);
  if (reply->type == REDIS_REPLY_ARRAY) {
    write(fd, okResp, strlen(okResp));
    write(fd,pageHeader,strlen(pageHeader));
    dprintf(fd, "<h2>All Contacts of \"%s\"</h2><br>", person);
    for (int j = 0; j < reply->elements; j++){
      char * w = reply->element[j]->str;
      fprintf(stderr, "%u) %s\n", j, w);
      //if word link to persons?person
      //if intercept link to intercepts?some-intercept
      dprintf(fd, "<a href=\"persons?%s\">%s</a> ", w, w);
    }
  }
  else{
    // return error page for Redis reply
  }
  freeReplyObject(reply);
  reply = redisCommand(conn, "ZRANGE words:%s 0 -1", person);
  if (reply->type == REDIS_REPLY_ARRAY) {
    dprintf(fd, "<h2>the Words that \"%s\" was caught using</h2><br>", person);
    for (int j = 0; j < reply->elements; j++){
      char * w = reply->element[j]->str;
      fprintf(stderr, "%u) %s\n", j, w);
      //if word link to persons?person
      //if intercept link to intercepts?some-intercept
      dprintf(fd, "<a href=\"words?%s\">%s</a> ", w, w);
    }
  }
  else{
    // return error page for Redis reply
  }
  freeReplyObject(reply);
  reply = redisCommand(conn, "SMEMBERS intercepts:%s", person);
  if (reply->type == REDIS_REPLY_ARRAY) {
    dprintf(fd, "<h2>Intercepts \"%s\" was in </h2><br>", person);
    for (int j = 0; j < reply->elements; j++){
      char * w = reply->element[j]->str;
      fprintf(stderr, "%u) %s\n", j, w);
      dprintf(fd, "<a href=\"intercepts?%s\">%s</a> ", w, w);
    }
    write(fd, pageFooter, strlen(pageFooter));
  }
  else{
    // return error page for Redis reply
  }
  freeReplyObject(reply);
  redisClose();
}


//all Persons involved in the Intercept (sender and recipient)
//all Words within this Intercept
void
returnInterceptPage(int fd, char * seq) {
  redisOpen();
  reply = redisCommand(conn, "HGETALL intercept:%s", seq);
  if(reply->type == REDIS_REPLY_ARRAY){
    write(fd, okResp, strlen(okResp));
    write(fd,pageHeader,strlen(pageHeader));
//    dprintf(fd, "%s", reply->element[0]->str);
//    dprintf(fd, "%s", reply->element[1]->str);
//    dprintf(fd, "%s", reply->element[2]->str);
//    dprintf(fd, "%s", reply->element[3]->str);
//    dprintf(fd, "%s", reply->element[4]->str);
//    dprintf(fd, "%s", reply->element[5]->str);
    for(int i = 0; i < 5; i++){
      char * key = reply->element[i]->str;
      //messag[e]
      //person[1]
      //person[2]
      switch(key[strlen(key) - 1]){
        case 'e':
          dprintf(fd, "MESSAGE: ");
          char * txt = reply->element[i+1]->str;
          fprintf(stderr, "%u) %s\n", i, txt);
          char * token = strsep(&txt, " ");
          while((token = strsep(&txt, " \t?.,!:;\"")) != NULL){
//            if(isalpha(token[strlen(token) - 1])){
            dprintf(fd, "<a href=\"words?%s\">%s</a> ", token, token);
//            }else{
//              char * tokencpy;
//              strncpy(tokencpy, token, strlen(token) - 1);
//              dprintf(fd, "<a href=\"words?%s\">%s</a> ", tokencpy, tokencpy);
//              dprintf(fd, "%s", strpbrk(token, "?,\".!;:"));
//            }
          }
          dprintf(fd, "\n");
          dprintf(fd, "\n");
          break;
        case '1':
          dprintf(fd, "<br>AUTHOR: ");
          char * p1 = reply->element[i+1]->str;
          fprintf(stderr, "%u) %s\n", i, p1);
          dprintf(fd, "<a href=\"persons?%s\">%s</a> ", p1, p1);
          dprintf(fd, "\n");
          dprintf(fd, "\n");
          break;
        case '2':
          dprintf(fd, "<br>RECIPIENT: ");
          char * p2 = reply->element[i+1]->str;
          fprintf(stderr, "%u) %s\n", i, p2);
          dprintf(fd, "<a href=\"persons?%s\">%s</a> ", p2, p2);
          dprintf(fd, "\n");
          dprintf(fd, "\n");
          break;
        default:
          //redis error page
          break;
      }
    }
    write(fd, pageFooter, strlen(pageFooter));
  }else{
    //error page for redis reply
  }
  freeReplyObject(reply);
  redisClose();
}

void
returnErrorPage(int fd, char * path) {
  write(fd,notFoundResp,strlen(notFoundResp));
  write(fd,pageHeader,strlen(pageHeader));
  dprintf(fd,"Unknown URL: %s", path);
  write(fd, pageFooter, strlen(pageFooter));
}

#define BUFFER_SIZE 1024
void
handleClient(int clntfd) {
  printf("handleClient fd %d\n", clntfd);

  char * buffer = alloca(BUFFER_SIZE);
  bzero(buffer,BUFFER_SIZE);

  int cnt = read(clntfd, buffer, BUFFER_SIZE);
  fprintf(stderr,"read %d bytes from client socket\n", cnt);
  exitIfError(cnt < 0, "Error reading from client socket.");

  fprintf(stderr,"buffer \"%s\"\n", buffer);

  char * ptr = buffer;
  char * verb = strsep(&ptr, " \t\r\n");
  char * path = strsep(&ptr, " \t\r\n");

  bool recog = false;

  if (strcmp(verb,"GET") == 0) {
    if (strcmp(path,"/words") == 0) {
      returnWordsPage(clntfd);
      recog = true;
    }
    if (strncmp(path,"/words?", 7) == 0) {
      returnWordPage(clntfd, path+7);
      recog = true;
    }
    if (strncmp(path,"/persons?",9) == 0) {
      returnPersonPage(clntfd,path+9);
      recog = true;
    }
    if (strncmp(path,"/intercepts?",12) == 0) {
      returnInterceptPage(clntfd,path+12);
      recog = true;
    }
    if (!recog) returnErrorPage(clntfd,path);
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
  //meh
  int iarg = 1;
  int ret = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(char *) &iarg, sizeof(iarg));
  iarg = 1;
  ret = setsockopt(sockfd,SOL_SOCKET,SO_REUSEPORT,(char *) &iarg, sizeof(iarg));

  ret = bind(sockfd, (struct sockaddr *) &srvrAddr, sizeof(srvrAddr));
  exitIfError(ret,"Error on bind.");
  //eggs
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
      //most IMPORTANT METHOD
      handleClient(clntfd);
      exit(0);
    }
    else {
      close(clntfd);
    }
  }
}
