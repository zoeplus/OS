#include <stdlib.h>

int main() {
    int* pi = (int *)malloc(sizeof(int));
    pi = NULL;
    free(pi);
    return 0;
}
