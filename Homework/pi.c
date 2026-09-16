#include <stdio.h>
#include <stdlib.h>

int main()
{
	int n, i;

	printf("n = ");
	scanf("%d", &n);

	double pi = 0.;
	double factor = 1.0;
	//TODO
	//add code below 
	do {
		double temp = factor*(((4/(8*i+1)) - (2/(8*i+4)) - (1/(8*i+5)) - (1/(8*i+6))));
		pi += temp;
		factor = factor/16;
		i++;
	} while(i <= n);

	printf("PI = %.10f\n", pi);
	return 0;
}
