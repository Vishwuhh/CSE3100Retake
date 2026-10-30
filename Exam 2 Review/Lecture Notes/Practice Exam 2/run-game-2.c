#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//valgrind --track-fds=yes ./ file 
//gcc filename.c -0 filename      ./filename 

//entire code is being run by both parent and child process 
//it doesn't matter where you put wait wherever it encounters it it will waitpid (pid1,NULL,0);

// write an integer to a pipe
void write_int(int pd, int value) //function that writes and integer to pipe, takes in pipe and val
{ 
  write(pd, &value, sizeof(int)); 
}
// read an integer from a pipe
// the function returns the number of bytes read
int read_int(int pd, int *value)  //function that reads from pipe, takes in pipe and thing to read (variable v or c)
{ 
  return read(pd, value, sizeof(int)); 
}

double random_double() 
{ 
  return (double)rand() / RAND_MAX;
 }

int coin_flip(double p) //coin flip 0 tails, 1 is heads 
//simulates a coin flip for the player 
{ 
  return random_double() < p; 
}

void run_simulation(int n, double p) {
  //creating the first pipe 
  int pd1[2]; 
  // pipe creation
  if (pipe(pd1) < 0) 
  {
    perror("Error.");
    exit(-1);
  }

  //creating the second pipe 
  int pd2[2]; 
  // pipe creation
  if (pipe(pd2) < 0) {
    perror("Error.");
    exit(-1);
  }

  //parent - sends integer 1 to process A in pipe pd DOES THIS BELOW.
  /*
  close(pd1[0]); //close read end 
  write_int(pd1[1],1); //writes the value of 1 to the first pipe for player A
*/
  //INSIDE CHILD PROCESS FOR PLAYER A - two pipes for player 
  pid_t pid = fork(); //use pipe 1 pd1 here 
  if (pid == 0) 
  { //first child process will serve as player A

    // TODO
    // fill in code below
    // note this is player A
    //pd2 is used for Player A to read the value recieved by parent 
    //pd1 is used for Player A to write the value of coin flip back to parent
    close(pd1[0]); // close pipes and the ends that will not be used 
    close(pd2[1]); //since pd2 is the pipe that reads the integer from parent, close the write end 
    


    srand(3100); //setting the random seed 

    int v; //this serves as the variable for the integer that parent is going to send 
    // TODO

    // complete the following line of code
    //pd2[0] is defined here as the read end of the child 
    while (read_int(pd2[0], &v) != 0) //while the read pipe of pd2 isnt 0, while theres smthn to read 
    {
      // TODO
      // fill in code below
      //player is waiting for 1 (v) from parent

      int c = coin_flip(p); //storing the coin flip in a variable 
      write_int(pd1[1],c); // write the coin flip value to the parent pipe so pd1 write end of the pipe 2 

    }
    // TODO
    // fill in code below
    close(pd2[0]); 
    close(pd1[1]); //close the used fds 
    exit(EXIT_SUCCESS); // exit succesfuly 
  }
  // TODO
  // fill in code below
  //keep track of all the pipes 
  //wahtever we used in the child we want to lose in the parent process 
  close(pd2[0]); 
  close(pd1[1]);

  //these pipes are for player B
  int pd3[2]; //3rd pipe for communication between child to parent 
  // pipe creation
  if (pipe(pd3) < 0) {
    perror("Error.");
    exit(-1);
  }

  int pd4[2]; //4th pipe for child to parent backwards 
  // pipe creation
  if (pipe(pd4) < 0) {
    perror("Error.");
    exit(-1);
  }
  pid_t pid1 = fork();
// CHILD PROCESS FOR PLAYER B
  if (pid1 == 0) 
  {
    // TODO
    // fill in code below
    // note this is player B
    close(pd1[0]); // close the fds that we didnt get to close in the parent from player A
    close(pd2[1]);


    close(pd4[1]); //since pd4 pipe is reading, we close the write end of the pipe 
    close(pd3[0]); //since pd3 is writing we close the read end of the pipe 

    srand(3504);

    int v; //stores the value that is getting sent by the parent as a variable 
    // TODO
    // complete the following line of code
    //read end that is reading th value from the parent 
    //pd4 is reading from the parent bc of how it was defined here. 
    while (read_int(pd4[0], &v) != 0) { //while pd4 is not empty 
      // TODO
      // fill in code below
      int c = coin_flip(p); //storing the coin flip in a variable using the function to simulate coin flip 
      write_int(pd3[1],c); //pd3 is we are writing it back to the parent 
    }

    // TODO
    // fill in code below
    close(pd3[1]); // now we are done with this fd, so we close it 
    close(pd4[0]);//we used these fds 
    exit(EXIT_SUCCESS); // do not forget to exit the child process
  }
close(pd3[1]); //close the same ones in the parent process bc its outside the child 
    close(pd4[0]);

  int n1 = 0;
  int n2 = 0;
  int v1, v2; // 1s 

  while (n1 + n2 < n) {
    // TODO
    // finish the follow lines of code
    //**HERE IS WHERE THE PARENT IS SENDING PLAYER A AND B INTEGERS  */
    write_int(pd2[1], 1); // 1 and 2 player A
    write_int(pd4[1], 1); //B player B
    read_int(pd1[0], &v1);  //read end player A
    read_int(pd3[0], &v2);
    if (v1)
      n1++;
    else {
      if (v2)
        n2++;
    }
  }
  printf("prob of A wins = %.3lf\n", (double)(n1) / (n1 + n2));
  // TODO
  // fill in code below
  //need to add a wait pid here

  close(pd2[1]);
  close(pd4[1]);
  close(pd1[0]);
  close(pd3[0]);
}
