#include "dataTypes/array.h"
#include <stdio.h>

int main() {

    int * arr = newArray(10, sizeof(int));

    for (int i = 0; i < len(arr); ++i) {
        arr[i] = i * i;
    }


    for (int i = 0; i < len(arr); ++i) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }

    freeArray(arr);
}