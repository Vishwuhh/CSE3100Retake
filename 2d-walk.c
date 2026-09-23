#include <stdio.h>
#include <stdlib.h>

double two_d_random(int n)
{
    int size = 2 * n + 1;
    
    int visited[size][size];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            visited[i][j] = 0;
        }
    }

    int x = 0; 
    int y = 0;
    int visited_count = 0;

    while (x > -n && x < n && y > -n && y < n) {
        
        if (visited[y + n][x + n] == 0) {
            visited[y + n][x + n] = 1;
            visited_count++;
        }

        int r = rand() % 4;
        if (r == 0) {
            y++;       // up
        } else if (r == 1) {
            x++;       // right
        } else if (r == 2) {
            y--;       // down
        } else if (r == 3) {
            x--;       // left
        }
    }

    double total_inside = (2.0 * n - 1.0) * (2.0 * n - 1.0);
    
    return (double)visited_count / total_inside;
}

//Do not change the code below
int main(int argc, char *argv[])
{
	int trials = 1000;
	int i, n, seed;
	if (argc == 2) seed = atoi(argv[1]);
	else seed = 12345;

	srand(seed);
	for(n=1; n<=64; n*=2)
	{	
		double sum = 0.;
		for(i=0; i < trials; i++)
		{
			double p = two_d_random(n);
			sum += p;
		}
		printf("%d %.3lf\n", n, sum/trials);
	}
	return 0;
}