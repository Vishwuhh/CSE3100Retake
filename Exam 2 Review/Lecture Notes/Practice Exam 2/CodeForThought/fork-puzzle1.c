//In this assignment, we use fork() to craete multiple processes to solve a puzzle.
//The puzzle is specified by the following array
//int a[] = {3, 6, 4, 1, 3, 4, 2, 5, 3, 0};
//A walker walks along the index of this array.
//He starts at the index 0, where a[0] is 3, which indicates that
//the walker can move 3 steps. If the walker move to the left, he would be at index
//-3, which is out of the range. Hence, he can only move to the right, after he makes the move,//he will be at index 3. Since a[3] = 1, he can move to the left or right by one move.

//Note whenever he makes his move, he should be in the range [0, 9]. He is not allowd to move 
//outside of this range.

//The goal of the puzzle is to for the walker to reach index 9, where a[9] = 0. And he will
//stop there.

//What we need to do is to find the solutions of this puzzle. We limit the solutions to have
//at most 10 moves.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
void print_solution(int b[], int moves) //takes the array b and integer for number of moves which is 10 
{
    for(int k = 0; k<moves; k++)
    {
        printf("->%d ", b[k]); // print the index and the value of it out, this is how many steps
    }
    printf("\n");
}

int main(int argc, char *argv[]) //these are the arguements that the process is taking.
{
    //We use the array a to describe the puzzle
    int a[] = {3, 6, 4, 1, 3, 4, 2, 5, 3, 0}; 
    //We use the array b to save the moves the walker makes
    //Essentially, we save each index that the walker reaches in array b
    //For example, b[0] = 3 since the walker goes to index 3 for the first move
    int b[10];    //b is an array of 10 integers
    int cur = 0; //stores the current location
    int moves = 0; //stores the number of moves 

    int n = 10;

    for(int i = 0; i< n; i++) //until the counter is less than 10
    {
		pid_t pid; //this is a process id called pid, of the parent process 
    	pid = fork(); //forks the process that we have, now this process has the same code 
        // if pid ==0 its child process so its different process 
        //ONE WAY - if its in CHILD Process 
        if(pid == 0) //if its in the child process right now 
        {   
            //If we find a solution, we print the solution 
            if(a[cur]==0) // if the current value is equal to 0 
            {
                b[moves - 1] = cur; // then we switch the current to be the previous value in list 
                print_solution(b, moves); 
                return 0;
            }

            else if(cur + a[cur] >= 0 && cur + a[cur] <n) 
            //if the current steps + value is more than 0 and its less than 10 
            {
                //Add your code here
                //then we want to increment the cur[a] += moves
                cur = cur + a[cur]; //increment current location by the number of steps
                b[moves] =cur; //since b saves the moves the walker makes 
                moves++; //moves is the number of moves player has mde 

            
            }
            
        }
        else //now e are where the PID is not equal to 0 so this means the parent process
        {
			int status; // this tells us the status of the child process if its done or not 
			//wait for the child process to finish
			waitpid(pid, &status, 0); 
            //If we find a solution, we print the solution
            if(a[cur]==0) // if we reach the final index 
            {
                b[moves - 1] = cur;
                print_solution(b, moves);
                return 0;
            }
            else if(cur - a[cur] >= 0 && cur - a[cur] <n)
            //else if we do not reach the final location 
            //we want to move left 
            {
                //Add your code here
                cur = cur - a[cur]; //current location is goign to be that minus the value of the index
                b[moves] = cur; // set the moves qual to this value of current 
                moves++; //increment it by 1 


            }

        }
    }
    return 0;
}
/*
Note run your code the following different ways and observe the outputs
./fork-puzzle
./fork-puzzle |sort
./fork-puzzle |sort| uniq
*/
