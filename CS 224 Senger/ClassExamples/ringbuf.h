#ifndef RING_BUF_H
#define RING_BUF_H

typedef struct ringBuffer ringBuffer;

enum bufferMode {MODE_FAIL, MODE_DISCARD_NEW, MODE_DISCARD_OLD};

// create and return of ring buffer
ringBuffer * createRingBuffer(size_t eltSize, size_t eltCnt, enum bufferMode mode);

// give a data item to the ring buffer, return 0 is success, !0 is failure
int          give(ringBuffer * ring, void * item);

// take a data item from the ring buffer, return 0 is success, !0 is failure
int          take(ringBuffer * ring, void * item);

// display the contents of the buffer for diagnostics purposes, displayData()
// is a user supplied function to present individual elements of the buffer
void         display(ringBuffer * ring, void (*displayData)(void *));

#endif
