
#include<stdlib.h>
#include<string.h>
int safe_strcmp(const char *a, const char *b) {
    if (a == NULL && b == NULL) return 0;
    return strcmp(a, b);
}