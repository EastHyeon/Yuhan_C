#include <stdio.h>

int add(int a, int b){
    return a + b;
}

int initValue(int* i){
    *i = (int*)(0x0123ABCF);
    
    return 0;
}

int main(){
    int a = 10;
    int b = 7;

    printf("%d", add(a, b));
    initValue(&a);
    printf("%d", a);

    return 0;
}