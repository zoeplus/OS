#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    int* data = (int *)malloc(sizeof(int) * 100);
    memset(data, 0, sizeof(int) * 100); // or using for loop to init
    printf("data[100]: %d\n", data[100]);
    free(data);
    return 0;
}
