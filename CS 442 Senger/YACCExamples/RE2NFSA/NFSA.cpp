#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
using namespace std;

#include "NFSA.h"

struct NFSA {
  int states;
  int letters;
  int initial;
  int final;
  vector< vector< set<int> > > transitions;
};

string stringForSet(set<int> & set) {
   stringstream result;
   copy(set.begin(),set.end(), ostream_iterator<int>(result, " "));
   return result.str();
}

struct NFSA *     NFSA_Init(int stateCnt) {
  printf("init cnt %d\n",stateCnt);
  struct NFSA * n = new NFSA;
  printf("init 1\n");
  n->states = stateCnt;
  printf("init 2\n");
  n->letters = 2;
  printf("init 3\n");
  n->transitions.resize(n->states);
  printf("init 4\n");
  for (int s = 0; s < n->states; s++) {
    printf("init 5\n");
    n->transitions[s].resize(n->letters+1);
    for (int l = 0; l < n->letters+1; l++) {
      printf("init 6  %d %d\n",s,l);
      n->transitions[s][l+1] = set<int>();
      printf("init 7\n");
    }
    printf("init 8\n");
  }
  printf("init 10\n");
  return n;
}

struct NFSA *     NFSA_Letter(int l) {
  printf("letter\n");
  struct NFSA * n = NFSA_Init(2);
  n->initial = 0;
  n->final = 1;
  // trans from
  n->transitions[n->initial][l+1].insert(n->final);
  return n;
}

struct NFSA *     NFSA_EmptyStr() {
  struct NFSA * n = NFSA_Init(2);
  n->initial = 0;
  n->final = 1;
  n->transitions[n->initial][0].insert(n->final);
  return n;
}

struct NFSA *     NFSA_EmptySet() {
  struct NFSA * n = NFSA_Init(2);
  n->initial = 0;
  n->final = 1;
  return n;
}

void copyTransitions(struct NFSA * A, int shift, struct NFSA * N) {
  for (int s = 0; s < A->states; s++) {
    vector<set<int> > row = A->transitions[s];
    for (int l = 0; l < A->letters+1; l++) {
      set<int> t = row[l];
      set<int> n = set<int>();
      for (auto i : t) {
        n.insert(i+shift);
      }
      N->transitions[s+shift][l] = n;
    }
  }
}

struct NFSA *     NFSA_ForUnion(struct NFSA * A, struct NFSA * B) {
  printf("union\n");
  struct NFSA * n = NFSA_Init(A->states + B->states + 2);
  n->initial = 0;
  n->final = 1;
  // shift state numbers
  int Ashift = 2;
  int Bshift = 2 + A->states;
  // transitions from new initial
  n->transitions[n->initial][0].insert(A->initial+Ashift);
  n->transitions[n->initial][0].insert(B->initial+Bshift);
  // copy A transitions over with state number adjustment
  copyTransitions(A,Ashift,n);
  // copy B transitions over with state number adjustment
  copyTransitions(B,Bshift,n);
  // transitions fromr old final states
  n->transitions[A->final+Ashift][0].insert(n->final);
  n->transitions[B->final+Bshift][0].insert(n->final);
  return n;
}

struct NFSA *     NFSA_ForConcat(struct NFSA * A, struct NFSA * B) {
  printf("concat\n");
  struct NFSA * n = NFSA_Init(A->states + B->states + 2);
  n->initial = 0;
  n->final = 1;
  // shift state numbers
  int Ashift = 2;
  int Bshift = 2 + A->states;
  // transitions from new initial
  n->transitions[n->initial][0].insert(A->initial+Ashift);
  // copy A transitions over with state number adjustment
  copyTransitions(A,Ashift,n);
  // copy B transitions over with state number adjustment
  copyTransitions(B,Bshift,n);
  // transitions fromr old final states
  n->transitions[A->final+Ashift][0].insert(B->initial+Bshift);
  n->transitions[B->final+Bshift][0].insert(n->final);
  return n;
}

struct NFSA *     NFSA_ForStar(struct NFSA * A) {
  printf("star %d\n",A->states);
  struct NFSA * n = NFSA_Init(A->states + 2);
  printf("start 0.5\n");
  n->initial = 0;
  n->final = 1;
  // shift state numbers
  int Ashift = 2;
  // transitions from new initial
  printf("star 1\n");
  n->transitions[n->initial][0].insert(A->initial+Ashift);
  n->transitions[n->initial][0].insert(n->final);
  n->transitions[n->final][0].insert(n->initial);
  // copy A transitions over with state number adjustment
  copyTransitions(A,Ashift,n);
  // transitions fromr old final states
  n->transitions[A->final+Ashift][0].insert(n->final);
  return n;
}

void              NFSA_Display(struct NFSA * A) {
  printf("   S|  lamda  |   '0'   |   '1'   |\n");
  for (int s = 0; s < A->states; s++) {
    vector<set<int> > row = A->transitions[s];
    char mark = ' ';
    if (s == A->initial) mark = '>';
    if (s == A->final) mark = '<';
    printf("%c%3d|",mark,s);
    for (int l = 0; l < A->letters+1; l++) {
      string str = stringForSet(row[l]);
      printf("%9s|",str.c_str());
    }
    printf("\n");
  }
}
