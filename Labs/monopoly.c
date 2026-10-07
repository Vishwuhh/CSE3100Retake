#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// There will be m players in an array
typedef struct Player 
{
	int id;
	int loc;
	long balance;
} TPlayer;

// There will be n properties in an array
typedef struct Property
{
	int id;
	int owner_id;
	int rent;
} TProperty;

// Transaction logic: p1 pays p2
// Returns 1 if successful, 0 if p1 goes bankrupt
int transaction(TPlayer *p1, TPlayer *p2, int amount)
{
    if(p1->balance >= amount)
    {
        p1->balance -= amount;
        p2->balance += amount;
        return 1;
    }
    else
    {
        // p1 goes bankrupt, gives remaining money to p2
        p2->balance += p1->balance;
        p1->balance = 0;
        return 0;
    }
}

// Simulates one round of turns for all players
int one_round(int m, int n, TPlayer p[], TProperty prop[])
{
	for(int i = 0; i < m; i++)
	{
		int steps = rand() % 6 + 1 + rand() % 6 + 1;
        int old_loc = p[i].loc;
        int new_loc = (old_loc + steps) % n;
        p[i].loc = new_loc;

        // Check if player passed or landed on Start (property 0)
        // Since steps < n (max 12, min n=13), wrapping happens when new_loc < old_loc
        if(new_loc < old_loc)
        {
            p[i].balance += n;
        }

        // Handle Property Logic
        int current_prop_id = p[i].loc;
        TProperty *current_prop = &prop[current_prop_id];

        if(current_prop->owner_id == -1)
        {
            // Unowned property: Player buys it (becomes owner)
            current_prop->owner_id = p[i].id;
        }
        else if(current_prop->owner_id != p[i].id)
        {
            // Owned by someone else: Pay rent
            TPlayer *owner = &p[current_prop->owner_id];
            if(transaction(&p[i], owner, current_prop->rent) == 0)
            {
                // Transaction failed, player went bankrupt
                return 0; 
            }
        }
	}
	return 1;
}

// Used for printing out results
void print_result(int m, TPlayer p[])
{
	printf("      id    balance\n");
	for(int i = 0; i < m; i++)
	{
		printf("%8d %8ld\n", p[i].id, p[i].balance);
	}
	long sum = 0;
	long max = 0;
	for(int i = 0; i < m; i++)
	{
		sum += p[i].balance;
		if(p[i].balance > max) max = p[i].balance;
	}
	printf("average: %f max: %ld, max/average = %lf\n", (double)sum/m, max, (double)max*m/sum); 
}

// max_rounds is needed because the game may never finish
void monopoly(int m, int n, TPlayer p[], TProperty prop[], int max_rounds)
{
	srand(12345);
	int rounds = 1;
	while(one_round(m, n, p, prop) && rounds < max_rounds)
	{
        rounds ++;
	}

	print_result(m, p);
	printf("after %d rounds\n", rounds);
}

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		printf("Usage: %s m n rounds\n", argv[0]);
		return -1;
	}
	int m = atoi(argv[1]);
	int n = atoi(argv[2]);
	int rounds = atoi(argv[3]);
	assert(n >= 13);
	assert(m >= 1);
	assert(rounds >= 1);

	TPlayer p[m];
	TProperty prop[n];

	for(int i = 0; i < n; i++)
	{
		prop[i].id = i;
		prop[i].owner_id = -1;
		prop[i].rent = i + 1;
	}

	for(int j = 0; j < m; j++)
	{
		p[j].id = j;
		p[j].loc = 0;
		p[j].balance = n;
	}
	monopoly(m, n, p, prop, rounds);
	return 0;	
}