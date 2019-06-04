#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ringbuf.h"

struct ringBuffer {
  size_t eltSize;
  size_t eltCnt;
  enum bufferMode mode;
  unsigned long head; // index of next available slot, only wrap when used
  unsigned long tail; // index of last occupied slot, only wrap when used
  void * contents;
};

#define BYTE_ADDR(x) ( (x) * ring->eltSize)
#define HEAD(ring) ( BYTE_ADDR(ring->head % ring->eltCnt) )
#define TAIL(ring) ( BYTE_ADDR(ring->tail % ring->eltCnt) )

ringBuffer *
createRingBuffer(size_t eltSize, size_t eltCnt, enum bufferMode mode) {
  struct ringBuffer * ring = malloc(sizeof(struct ringBuffer));
  if (!ring) return NULL;
  ring->eltSize = eltSize;
  ring->eltCnt = eltCnt;
  ring->mode = mode;
  ring->head = 0;
  ring->tail = 0;
  ring->contents = calloc(ring->eltCnt,ring->eltSize);
  return ring;
}

int
give(ringBuffer * ring, void * item) {
  if (ring->head - ring->tail >= ring->eltCnt) {
    if (ring->mode == MODE_FAIL) return -1;
    if (ring->mode == MODE_DISCARD_NEW) return 0;
    if (ring->mode == MODE_DISCARD_OLD) take(ring,NULL);
  };
  if (item) memcpy(&(ring->contents[HEAD(ring)]),item,ring->eltSize);
  ring->head++;
  return 0;
}

int
take(ringBuffer * ring, void * item) {
  if (ring->tail >= ring->head) return -1;
  if (item) memcpy(item,&(ring->contents[TAIL(ring)]),ring->eltSize);
  ring->tail++;
  if (ring->tail > ring->eltCnt) {
    ring->head -= ring->eltCnt;
    ring->tail -= ring->eltCnt;
  }
  return 0;
}

void
display(ringBuffer * ring, void (*displayData)(void *)) {
  fprintf(stderr,"head %ld tail %ld\n",ring->head,ring->tail);
  for (int i = 0; i < ring->eltCnt; i++) {
    fprintf(stderr,"%c%c  ",((ring->head % ring->eltCnt == i) ? 'h' : ' '),(ring->tail % ring->eltCnt == i) ? 't' : ' ');
    displayData(&(ring->contents[BYTE_ADDR(i)]));
    fprintf(stderr,"\n");
  }
  fprintf(stderr,"--------\n");
}
