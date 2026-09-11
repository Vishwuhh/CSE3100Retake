// Only this line of comment is provided 
#include <stdio.h>

int main(void){
    double total = 0.0;
    double average = 0.0;
    double x = 0;
    int i = 1;
    while(scanf("%lf", &x) == 1) {
        total = total + x;
        average = (total)/i; 
        i++;
        printf("Total=%f Average=%f\n", total, average);
    }
}