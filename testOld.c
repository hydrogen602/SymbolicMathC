#include <stdio.h>

//Tim's new comment
//Tim's newer comment, made in VSCode

int nextStep(int x) {
    printf("%d\n", x);

    if (x == 1) {
        return 1;
    }
    else if (x % 2 == 0) {
        return nextStep(x / 2) + 1;
    }
    else {
        return nextStep(3*x+1) + 1;
    }
}

int main() {

    nextStep(5);
}
