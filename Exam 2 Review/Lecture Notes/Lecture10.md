# Lecture 10: Redirection
> What happens if we want processes to give us outputs in different places?
> 
> Why would we want input/output from different places?
- we can run different operations at the same time (parallel)
  - need different file descriptors for input and output
## How to Redirect Things?
- simple example with built in UNIX function "sort"
```UNIX
$ sort <file.txt> sorted.txt
```
- syntax
  - "<" : take input from file filename 
  - ">" : sends output to the file filename
  - "2>" : sends errors to the file filename 
## Sorting Example (without redirection)
- we can view the contents of textfile in command line using 'cat' command
- we can sort the contents of txt file using 'sort' command
- sort the contents of txt file and redirects output to a NEW file location
## Shell Redirections
- available when executing commands in shell (ex. bash)
  - implemented with **close, open, dup** technique
```C
$ command < infile > outfile
// < infile, takes input from file infile 
// > outfile, sends output to file outfile 
```
- other variants
  - ">>" outfile - appends output to file outfile
  - "2>" outfile - sends errors to file outfile
  - "&>" outfile - sends both output and errors to file outfile
## File Descriptor Table
- each process has a **file descriptor table**
  - hilds indices of entries into the **open file table** managed by OS
- **open file table**
  - records mode of opened fil and holds index into the **inode table** with actual file name and location on disk
## Duplicating File Descriptors 
- do not change descriptor table directly, but uses open() and close() plus two new functions
```C
#include <unistd.h>

int dup(int oldfd);
int dup2(int oldfd, int newfd);
```
### Using dup() in Practice
- first open text file which we call 'dup.txt' for writing to
- when opening file, we get file descritptor fd
- we call dup to copy file descriptor, and now fd and copy_fd can be used interchangably
- we can use copy_fd and write to 'dup.txt'
  - can also use original fd to write the text file 'dup.txt'
```C
int main() {
    // open() returns file descritptor fd to file 'dup.txt'
    int fd = open('dup.txt', O_WRONLY | O_CREAT, S_IRUSR| S_IWUSR);
    if(fd < 0) {
        printf('Cannot open file\n');
        return -1;
    }
    // dup() create copy of fd as copy_fd, and both can be used interchangeably 
    int copy_fd = dup(fd);
    // write() will write given string into file refered by fd
    // last paramter refers to number of characters needing to be written
    write( copy_fd, 'This is the output for the file', 22); // can also be written with the original fd as well
    close(copy_fd);
    close(fd);
    return 0; 
}
```
## Example of Using dup2()
- dup2() performs the same thing as dup(), but instead of using the lowest-numbered unsed fd, it *uses the one specified in newfd*
  - fd newfd is adjusted so that it referes to the **same open fd** as oldfd
```C
int main() {
    printf("Before)
}
```