#include <stdio.h>
#include <stdlib.h>

double two_d_random(int n)
{

	//Fill in code below
	//When deciding which way to go for the next step, generate a random number as follows.
	//r = rand() % 4;
	//Treat r = 0, 1, 2, 3 as up, right, down and left respectively.

	//The random walk should stop once the x coordinate or y coordinate reaches $-n$ or $n$. 
	//The function should return the fraction of the visited $(x, y)$ coordinates inside (not including) the square.

	int side = 2*n - 1;

	int visited[side][side]; // visited[row][column] 
	// 0 = not visited, 1 = visited
	int i, j;

	for(i = 0; i < side; i++) {
		for(j = 0; j < side; j++) {
			visited[i][j] = 0;
		}
	}

	int x, y = 0;
	int count = 1; // origin is the first visited point

	visited[n-1][n-1] = 1;

	while(x != -n && x != n && y != -n && y != n) {
		int r = rand() % 4; 
	   if(r == 0){
            y--;
        }
        else if(r == 1){
            x++;
        }
        else if(r == 2){
            y++;
        }
        else{
            x--;
        }


		if(x == -n || x == n || y == -n || y == n) {
			break;
		}

		int row = y + (n-1);
		int col = x + (n-1);

		if(visited[row][col] == 0) {
			visited[row][col] = 1;
			count++;
		}
	}

	int total = side * side;

	return (double) count/total;
	
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

