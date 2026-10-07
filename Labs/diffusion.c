#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Simulate one particle moving n steps in random directions
// The grid maps [-n, n] to [0, 2n]
void one_particle(int *grid, int n)
{
    int x = 0, y = 0, z = 0;
    int size = 2 * n + 1; // Width of the grid in one dimension

    for(int i = 0; i < n; i++)
    {
        int dir = rand() % 6;
        if(dir == 0) x--;      // Left
        else if(dir == 1) x++; // Right
        else if(dir == 2) y--; // Down
        else if(dir == 3) y++; // Up
        else if(dir == 4) z--; // Backward
        else if(dir == 5) z++; // Forward
    }

    // Convert coordinates (-n to n) to grid indices (0 to 2n)
    int idx_x = x + n;
    int idx_y = y + n;
    int idx_z = z + n;

    // Flat index for 3D array
    int index = idx_x * size * size + idx_y * size + idx_z;
    grid[index]++;
}

// Returns the fraction of particles within Euclidean distance r*n from the origin
double density(int *grid, int n, double r)
{
    int size = 2 * n + 1;
    long total_particles = 0;
    long particles_within_radius = 0;
    double radius_sq = (r * n) * (r * n); // Compare squared distances to avoid sqrt

    for(int x = -n; x <= n; x++)
    {
        for(int y = -n; y <= n; y++)
        {
            for(int z = -n; z <= n; z++)
            {
                int idx = (x + n) * size * size + (y + n) * size + (z + n);
                int count = grid[idx];
                
                if(count > 0)
                {
                    total_particles += count;
                    // Check if point is inside the sphere
                    if((double)(x*x + y*y + z*z) <= radius_sq)
                    {
                        particles_within_radius += count;
                    }
                }
            }
        }
    }

    if(total_particles == 0) return 0.0;
    return (double)particles_within_radius / total_particles;
}

// Use this function to print results
void print_result(int *grid, int n)
{
    printf("radius density\n");
    for(int k = 1; k <= 20; k++)
    {
        printf("%.2lf   %lf\n", 0.05*k, density(grid, n, 0.05*k));
    }
}

// Main diffusion simulation function
void diffusion(int n, int m)
{
    // Dynamically allocate 3D grid: size = (2n + 1)^3
    // Using calloc to initialize all counts to 0
    int size = 2 * n + 1;
    int *grid = (int *)calloc(size * size * size, sizeof(int));
    if(grid == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

	for(int i = 1; i <= m; i++) 
    {
        one_particle(grid, n);
    }

	print_result(grid, n);
    
    free(grid);
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Usage: %s n m\n", argv[0]);
		return 0; 
	}
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);

	assert(n >= 1 && n <= 50);
	assert(m >= 1 && m <= 1000000);
	srand(12345);
	diffusion(n, m);
	return 0;
}