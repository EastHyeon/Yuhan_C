#include <stdio.h>

int main(){
    int a = 10;
    int *b = &a;
    *b = 15;
    printf("%d", a);
}