#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    int n = 0;
    int sum = 0; 
    while(n < 200) {
        if(n % 2 == 0) {
            sum += n;
            n++;
        }
        n++;
    }
    printf("%d\n", sum);
    return 0;
}