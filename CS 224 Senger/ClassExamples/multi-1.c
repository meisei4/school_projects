#include "multi-1.h"

int invocationCount = 0;

char multi_1_global_str[] = "abcd";
int multi_1_global_int = 0;
static int multi_1_static_int = -1;

void
countInvocations() {
  invocationCount++;
}
