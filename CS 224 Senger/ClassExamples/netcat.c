#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void
exitIfError(int condition, char * msg) {
  if (condition) {
    perror(msg);
    exit(1);
  }
  return;
}

void
displayUsage() {
  fprintf(stderr,"usage: netcat -l -p # || netcat -p # -h ###.###.###.###\n");
  exit(1);
}

void
actAsServer(int portNum) {
  int sockfd = socket(AF_INET,SOCK_STREAM,0);
  exitIfError(sockfd < 0,"Error on socket open.");
  
  struct sockaddr_in srvrAddr;
  bzero((char *) &srvrAddr,sizeof(srvrAddr));
  
  srvrAddr.sin_family = AF_INET;
  srvrAddr.sin_addr.s_addr = INADDR_ANY;
  srvrAddr.sin_port = htons(portNum);
  
  int ret = bind(sockfd, (struct sockaddr *) &srvrAddr, sizeof(srvrAddr));
  exitIfError(ret,"Error on bind.");
  
  ret = listen(sockfd,5);
  exitIfError(ret != 0,"Error on listen.");
  
  fprintf(stderr,"listening on %d\n",portNum);
  
  struct sockaddr_in clntAddr;
  socklen_t clntLen = sizeof(clntAddr);
  
  int clntfd = accept(sockfd, (struct sockaddr *) &clntAddr, &clntLen);
  exitIfError(clntfd < 0,"Error on accept.");

  char clntip[INET_ADDRSTRLEN] = {0};
  inet_ntop(AF_INET,(struct inaddr *)&clntAddr.sin_addr,clntip,sizeof(clntip));
  fprintf(stderr,"accepted %s\n",clntip);

  int bufsize = 1024;
  char * buf = alloca(bufsize);
  while (1) {
    if (!fgets(buf,bufsize,stdin)) break;
    write(clntfd,buf,strlen(buf));
    fprintf(stderr,"cnt %lu\n",strlen(buf));
//    int cnt = fread(buf,1,bufsize,stdin);
//    if (cnt > 0) write(clntfd,buf,cnt);
//    fprintf(stderr,"cnt %d\n",cnt);
//    if (feof(stdin) || ferror(stdin)) break;
  }

  close(clntfd);
  close(sockfd);
}

void
actAsClient(int portNum, char * srvrIP) {
  int sockfd = socket(AF_INET,SOCK_STREAM,0);
  exitIfError(sockfd < 0,"Error on socket open.");
  
  struct sockaddr_in srvrAddr;
  bzero((char *) &srvrAddr,sizeof(srvrAddr));
  
  srvrAddr.sin_family = AF_INET;
  inet_aton(srvrIP,&srvrAddr.sin_addr);
  srvrAddr.sin_port = htons(portNum);
  
  int ret = connect(sockfd,(struct sockaddr *) &srvrAddr,sizeof(srvrAddr));
  exitIfError(ret < 0, "Error on connect.");
  
  int bufsize = 1024;
  char * buf = alloca(bufsize);
  while (1) {
    int cnt = read(sockfd,buf,bufsize);
    if (cnt > 0) fwrite(buf,1,cnt,stdout);
    fprintf(stderr,"cnt %d\n",cnt);
    if (cnt == 0) break;
  }
  
  close(sockfd);
}

int
main(int argc, char * argv[]) {
  // establish default values
  int listenMode = 0;
  int portNum = 5000;
  char hostIP[INET_ADDRSTRLEN];
  strcpy(hostIP,"");
  
  // process command line options
  opterr = 0;
  int c;
  while ((c = getopt(argc, argv, "lp:h:")) != -1)
    switch (c) {
      case 'l':
        listenMode = 1;
        break;
      case 'p':
        portNum = strtol(optarg,NULL,10);
        break;
      case 'h':
        strncpy(hostIP,optarg,INET_ADDRSTRLEN);
        break;
      case '?':
        if (optopt == 'p' || optopt == 'h')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      default:
        abort ();
    }
  
  // verify consistent options
  if (listenMode && strlen(hostIP) > 0) displayUsage();
  
  if (listenMode) {
    actAsServer(portNum);
  }
  else {
    actAsClient(portNum,hostIP);
  }
  
}
