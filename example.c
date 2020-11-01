

int arrayExample() {
    int * arr = newArray(10, sizeof(int));

    for (unsigned int i = 0; i < len(arr); ++i) {
        arr[i] = i * i;
    }


    for (unsigned int i = 0; i < len(arr); ++i) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }

    freeArray(arr);
}