#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "intstk.h"
#include "libircclient/include/libircclient.h"

char * nickname;
char * channel;
irc_session_t * session;
char * thought;
char * dp;

char *
strskip(const char * s, const char * t) {
  const char * p;
  if ((p = strstr(s,t))) {
    return (char *) p + strlen(t);
  }
  return NULL;
}

void
exitIfError(int condition, const char * msg, const char * err)
{
  if (condition) {
    fprintf(stderr,"%s : %s\n", msg,err);
    exit(1);
  }
  return;
}

void
log_event(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {

  char buf[512] = "";

  for (int cnt = 0; cnt < count; cnt++) {
    if (cnt) strcat (buf, "|");
    strcat (buf, params[cnt]);
  }

  fprintf(stderr,"Event \"%s\", origin: \"%s\", params: %d [%s]\n", event, origin, count, buf);
}

void
event_connect(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
  log_event(session,event,origin,params,count);
  // get the context data
  irc_cmd_join(session,channel,0);
}

void
event_join(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
  log_event(session, event, origin, params, count);
  irc_cmd_user_mode(session, "+i");
  irc_cmd_msg(session, params[0], "Hi all");
}

void
ALARMhandler(int sig){
  signal(SIGALRM, SIG_IGN);
  irc_cmd_msg(session, channel, "uuuuuuuuuuuuuuuUUUUUUUUUUuuuuuuuuuuuuUUUUUUUUUUUUUuuuuu");
  signal(SIGALRM, ALARMhandler);
}

int
priority(int op) {
  switch (op) {
    case '+': return 1;
    case '-': return 1;
    case '*': return 2;
    case '/': return 2;
    case '%': return 2;
    default: return 0;
  }
}

void
doOp(struct intStack * stk, int op) {
  if (stkEmpty(stk)) { irc_cmd_msg(session, channel, "syntax error\n"); return;}
  int v2 = stkPop(stk);
  if (stkEmpty(stk)) { irc_cmd_msg(session, channel, "syntax error\n"); return;}
  int v1 = stkPop(stk);
  switch (op) {
    case '+':
      stkPush(stk,v1+v2);
      break;
    case '-':
      stkPush(stk,v1-v2);
      break;
    case '*':
      stkPush(stk,v1*v2);
      break;
    case '/':
      stkPush(stk,v1/v2);
      break;
    case '%':
      stkPush(stk,v1 % v2);
  }
}

struct intStack {
  unsigned int orgSize;
  unsigned int max;
  unsigned int tos;
  int * stk;
};

intStack *
stkCreate(unsigned int size) {
  intStack * stk = malloc(sizeof(intStack));
  if (!stk) return NULL;
  stk->stk = malloc(size * sizeof(int));
  if (!stk->stk) {
    free(stk);
    return NULL;
  }
  stk->orgSize = size;
  stk->max = size;
  stk->tos = 0;
  return stk;
}

int
stkEmpty(intStack * stk) {
  return (stk->tos == 0);
}

int
stkPush(intStack * stk,int c) {
  if (stk->tos >= stk->max) {
    stk->max += stk->orgSize;
    int * newStk = realloc(stk->stk,stk->max*sizeof(int));
    if (!newStk) return 0;
    stk->stk = newStk;
  }
  stk->stk[stk->tos++] = c;
  return 1;
}

int
stkPop(intStack * stk) {
  if (!stkEmpty(stk)) return stk->stk[--(stk->tos)];
  return '\0';
}

int
stkTop(intStack * stk) {
  if (!stkEmpty(stk)) return stk->stk[stk->tos - 1];
  return '\0';
}

void
stkReset(intStack * stk) {
  stk->tos = 0;
}

int *
factorsOf(int x){
  int factos = 0;
  int i;
  for(i = 0; i <= x; i++){
    if(x%i == 0){
      factos++;
    }
  }
  int * facts = malloc(factos * sizeof(int));
  int count = 0;
  for(i = 0; i < x; i++){
    if (x % i == 0){
      facts[count++] = i;
    }
  }
  return facts;
}

int cmpfunc (const void * a, const void * b){
  return ( * ( int * ) a - * ( int * ) b );
  //what is even happening
}

void
event_channel(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
  log_event(session, event, origin, params, count);

  const char * cmd = params[1];
  char * p;
  char * bingoSheet;
  int * facts;
  int in;
  int intictac;
  char * b = strchr(origin,'!');
  char sender[16];
  strncpy(sender,origin,b-origin);
  sender[b-origin] = '\0';
  char msg[64];
  char str[64];
  printf("sender %s\n", sender);
  if ((p = strskip(cmd, nickname)) && strskip(p,"say hello")) {
    sprintf(msg,"hello %s", sender);
    irc_cmd_msg(session, params[0], msg);
    return;
  }

  if ((p = strskip(cmd, nickname)) && strskip(p, "time")) {
    time_t clock = time(NULL);
    sprintf(msg, "clock %ld\n", clock);
    irc_cmd_msg(session, params[0], msg);
    struct tm * tm = localtime(&clock);
    sprintf(msg, "tm sec: %d min: %d hour: %d\n", tm->tm_sec, tm->tm_min, tm->tm_hour);
    irc_cmd_msg(session, params[0], msg);
    char * format = ("%d/%m/%Y @ %H:%M:%S");
    char buf[32];
    strftime(buf, 32, format, tm);
    sprintf(msg, "time %s\n", buf);
    irc_cmd_msg(session, params[0], msg);
    return;
  }

  if ((p = strskip(cmd, nickname)) && strskip(p, "alarm")) {
    signal(SIGALRM, ALARMhandler);
    char * time = strdup(p);
    for(int i = 0; i <= strlen("alarm"); i++){
      time++;
    }
    int tim = atoi(time);
    alarm(tim);
    return;
  }

  if ((p = strskip(cmd, nickname)) && strskip(p, "remember")) {
    thought = strdup(p);
    for(int i = 0; i <= strlen("remember"); i++){
      thought++;
    }
    sprintf(msg, "SAVED %s", thought);
    irc_cmd_msg(session, params[0], msg);
    return;
  }

  if ((p = strskip(cmd, nickname)) && strskip(p, "remind")) {
    if(!thought){
      //not ever entered
      sprintf(str, "I have nothing for you sorry, try using \"%s remember <something>\"", nickname);
      irc_cmd_msg(session, params[0], str);
    }else{
      sprintf(str, "it was: %s ", thought);
      irc_cmd_msg(session, params[0], str);
    }
    return;
  }

  if ((p = strskip(cmd, nickname)) && strskip(p, "forget")) {
    irc_cmd_msg(session, params[0], "like it never even happened");
    thought = "";
    return;
  }

  if ((p = strskip(cmd, nickname)) && strskip(p, "solve this:")) {
    int solution;
    char * expr = strdup(p);
    for(int i = 0; i <= strlen("solve this:"); i++){
      expr++;
    }
    struct intStack * evalstk = stkCreate(5);
    struct intStack * opstk = stkCreate(5);
    int inDigits = 0;
    int c;
    int i = 0;
    for (int i = 0; i < strlen(expr); i++) {
      c = expr[i];
      switch (c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
          if (!inDigits) {
            stkPush(evalstk, 0);
          }
          int v = stkPop(evalstk);
          v = 10 * v + (c - '0');
          stkPush(evalstk,v);
          inDigits = 1;
        } break;
        case '+': case '-': case '*': case '/': case '%':
          inDigits = 0;
          while (!stkEmpty(opstk) && priority(stkTop(opstk)) >= priority(c)) {
            doOp(evalstk,stkPop(opstk));
          }
          stkPush(opstk,c);
          break;
        case '(':
          stkPush(opstk,c);
          inDigits = 0;
          break;
        case ')':
          while (!stkEmpty(opstk) && stkTop(opstk) != '(') {
            doOp(evalstk,stkPop(opstk));
          }
          stkPop(opstk);
          inDigits = 0;
          break;
        case '=':
          while (!stkEmpty(opstk)) {
            doOp(evalstk,stkPop(opstk));
          } if (stkEmpty(evalstk)) {
            sprintf(msg, "invalid expression");
            irc_cmd_msg(session, params[0], msg);
          } else {
            sprintf(str, "solution: %d", stkPop(evalstk));
            irc_cmd_msg(session, params[0], str);
          }
          inDigits = 0;
          break;
        case '\n':
          stkReset(evalstk);
          stkReset(opstk);
          break;
        default:
          break;
      }
    }
    return;
  }

  if ((p = strskip(cmd, nickname)) && strskip(p, "bingo")){
    irc_cmd_msg(session, params[0], "BingoBot I'm in!");
    count = 0;
    in = 1;
    return;
  }

  if (in && (p = strskip(cmd, nickname)) && strskip(p, "your number is")) {
    char * num = strdup(p);
    for(int i = 0; i <= strlen("your number is"); i++){
      num++;
    }
    sprintf(msg, "SAVED %s", num);
    irc_cmd_msg(session, params[0], msg);
    facts = factorsOf(atoi(num));
    bingoSheet = calloc(sizeof(facts), sizeof(int));
    thought = num;
    return;
  }

  if(in && strstr(cmd, "call")){
    char * temp = strdup(cmd);
    for(int i = 0; i <= strlen("call"); i++){
      temp++;
    }
    if(atoi(thought) % atoi(temp) == 0){
      bingoSheet[count++] = atoi(temp);
      if(count * sizeof(int) == sizeof(facts)){
        qsort(bingoSheet, count, sizeof(int), cmpfunc);
        if(memcmp(bingoSheet, facts, count) == 0){
          irc_cmd_msg(session, params[0], "Bingo!");
          count = 0;
          in = 0;
        }
      }
      for(int i = 0; i < count; i++){
        sprintf(str, "%d", bingoSheet[i]);
        irc_cmd_msg(session,params[0], str);
      }
    }
    return;
  }

  //here?
  int iter;
  int player;
  int go;
  int row;
  int column;
  int line;
  int winner;
  char * board[3];
  //here?
  if((p = strskip(cmd, nickname)) && strskip(p, "tictactoe")){
    intictac = 1;
    iter = 0;
    player = 0;
    row = 0;
    column = 0;
    line = 0;
    winner = 0;
    char board0[3] = {' ',' ',' '};
    char board1[3] = {' ',' ',' '};
    char board2[3] = {' ',' ',' '};
    board[0] = board0;
    board[1] = board1;
    board[2] = board2;
    sprintf(str, " %c | %c | %c", board[0][0], board[0][1], board[0][2]);
    irc_cmd_msg(session, params[0], str);
    sprintf(str, "-----------");
    irc_cmd_msg(session, params[0], str);
    sprintf(str, " %c | %c | %c", board[1][0], board[1][1], board[1][2]);
    irc_cmd_msg(session, params[0], str);
    sprintf(str, "-----------");
    irc_cmd_msg(session, params[0], str);
    sprintf(str, " %c | %c | %c", board[2][0], board[2][1], board[2][2]);
    irc_cmd_msg(session, params[0], str);
    player = iter % 2 + 1;
    char xo = (player == 1) ? 'X':'O';

    sprintf(str, "Player %d, please the (row, column) (0 indexing my dude) for where you want to place your %c: ", player, xo);
    irc_cmd_msg(session, params[0], str);
    iter++;
    return;
  }

  if (intictac && (p = strskip(cmd, nickname)) && strskip(p, "put me at (")) {
    char * place = strdup(p);
    for (int j = 0; j <= strlen("put me at ("); j++) {
      place++;
    }

    row = place[0] - '0';
    column = place[2] - '0';
    if(board[row][column] == 'X' || board[row][column] == 'O'){
      board[row][column] = (player == 1) ? 'X' : 'O';
    }

    if((board[0][0] == board[1][1] && board[0][0] == board[2][2]) || (board[0][2] == board[1][1] && board[0][2] == board[2][0])){
      winner = player;
    } else {
      for (line = 0; line <= 2; line++){
        if ((board[line][0] == board[line][1] && board[line][0] == board[line][2]) || (board[0][line] == board[1][line] && board[0][line] == board[2][line])){
          winner = player;
        }
      }
    }
    if (iter >= 9 && winner !=0){
      irc_cmd_msg(session, params[0], str);
      sprintf(str, " %c | %c | %c", board[0][0], board[0][1], board[0][2]);
      irc_cmd_msg(session, params[0], str);
      sprintf(str, "-----------");
      irc_cmd_msg(session, params[0], str);
      sprintf(str, " %c | %c | %c", board[1][0], board[1][1], board[1][2]);
      irc_cmd_msg(session, params[0], str);
      sprintf(str, "-----------");
      irc_cmd_msg(session, params[0], str);
      sprintf(str, " %c | %c | %c", board[2][0], board[2][1], board[2][2]);
      irc_cmd_msg(session, params[0], str);
      if(winner == 0){
        sprintf(str, "Draw");
        irc_cmd_msg(session, params[0], str);
      }else{
        sprintf(str, "player %d, YOU WON!", winner);
        irc_cmd_msg(session, params[0], str);
      }
      intictac = 0;
      iter = 0;
      player = 0;
      row = 0;
      column = 0;
      line = 0;
      winner = 0;
    }
    return;
  }
  // other command responses

  // default response
  if (strstr(cmd, nickname)) {
    irc_cmd_msg(session, params[0], "You called!");
  }
}

int
main(int argc, char * argv[]) {
  thought = "";
  if (argc != 4) {
    fprintf(stderr,"usage: %s server nick channel\n",argv[0]);
    return 1;
  }

  irc_callbacks_t callbacks;
  memset(&callbacks,0,sizeof(callbacks));
  callbacks.event_connect = event_connect;
  callbacks.event_join = event_join;
  callbacks.event_channel = event_channel;

  irc_session_t * s = irc_create_session(&callbacks);
  session = s;
  exitIfError(!s,"failed to create session", NULL);

  nickname = strdup(argv[2]);
  nickname = realloc(nickname, 64);
  channel = strdup(argv[3]);

  bool ret = irc_connect(s,argv[1],6667,0,nickname,0,0);
  exitIfError(ret,"connect failed",irc_strerror(irc_errno(s)));

  ret = irc_run(s);
  exitIfError(ret,"run failed",irc_strerror(irc_errno(s)));
  
  return 0;
}
