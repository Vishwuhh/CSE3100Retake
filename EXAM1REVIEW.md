# CSE3100 EXAM 1 FRQ REVIEW 

## Pre FRQ Checklist
```C
1. INPUT
   What data am I given?

2. OUTPUT
   What must I return/print?

3. PATTERN
   Sorting?
   Linked list?
   Stack?
   Digits?
   Recursion?
   Simulation?
   Structs?
   Hashing?

4. STOPPING CONDITION
   When does the loop/recursion end?
```
### Problem Lookup Table
“sort according to...” = 
qsort+ comparator

“command-line arguments” =	argc, argv, atoi

“count letters/digits in a string” = loop until '\0'

“most recent / nearest previous item” = stack

“repeatedly destroys/removes previous items”	= stack + while

“change the head of a linked list” = Node**

“each digit of an integer” =	% 10, /= 10

“find a combination”	= recursion/backtracking

“include or skip”	recursion
“random movement”	= simulation

“coordinates can be negative” = add offset

“circular board” = %

“multiple fields describe one thing” = struct

“status such as S/I/R” = enum

“fast lookup by location/key”= hash table

“distance from origin” = squared distance

“no pow()” = running multiplier/factor
## Command-Line Arguments 
```C
int main(int argc, char *argv[])
```
- this means that 
  - argv[0] = program name
  - argv[1] = first arg
  - argv[2] = second arg
  - number of arguments = **argc - 1**
```C
./program 10 20 hello
```
- arguments
  - argv[0] = ./program
  - argv[1] = 10
  - argv[2] = 20
  - argv[3] = "hello"
- to convert a numberic command-line string, we can use
```C
int n = atoi(argv[1]) // this is for the example, and converts the string "10" into int 10
```
### FRQ Template
```C
int main(int argc, char *argv[])
{
    if (argc != EXPECTED_NUMBER) {
        return -1;
    }

    int a = atoi(argv[1]); // string to int
    int b = atoi(argv[2]); // string to int 

    ...
}
```
- if a program needs three actual arguments
> program + 3 args = argc == 4
## C Strings
- characters followed by '\0'
```C
for (int i = 0; s[i] != '\0'; i++) {
    // use s[i]
}
```
- above is the universal string loop
- use for 
  - count lowercase
  - count uppercase
  - count digits
  - count vowels
  - count certain characters
  - find length manually
### Example: count lowercase
```C
int countLower(char *s)
{
    int count = 0;

    for (int i = 0; s[i] != '\0'; i++) { // universal loop

        if (s[i] >= 'a' && s[i] <= 'z') {
            count++;
        }
    }

    return count;
}
```
- s[i] is the string being looked at 
- if a different condition needed, change the inner if-case
```C
if (s[i] >= '0' && s[i] <= '9') // digits

if (s[i] >= 'A' && s[i] <= 'Z') // uppercase

if (s[i] == 'a') // specific letter
```
## qsort()
- used in practice exam 1
  - plainly:
    - calculate property, and then sort by that property
```C
qsort(array,
      number_of_elements,
      sizeof(element_type),
      comparator); // general form

qsort(&argv[1],
      argc - 1,
      sizeof(char *),
      compare); // command line strings
```
### qsort() Comparator Rules 
- comparator
```C
int compare(const void *a, const void *b)
```
- the return meaning
  - negative = a goes before b
  - zero = a and b are equal
  - positive = a goes after b
```C
return A - B; // ascending rule
return B - A; // descending rule 
```
### Example
```C
int compare(const void *a, const void *b)
{
    char *s1 = *(char **)a;
    char *s2 = *(char **)b;

    int A = countLower(s1);
    int B = countLower(s2);

    return A - B;
}
```
## The char** Rule
- if sorting char *strings[]
  - each element is char* so qsort passes the **address of the element**
- the comparator recieves char**
```C
char *s1 = *(char **)a;
char *s2 = *(char **)b;
```
- look at the example for qsort() comparator rules
- MEMORIZE THIS
```
ARRAY ELEMENT        CAST COMPARATOR

int                  int *
double               double *
char *               char **
Student              Student *
```
## Complete qsort() String Template
```C
int property(char *s)
{
    int count = 0;

    for (int i = 0; s[i] != '\0'; i++) {

        if (/* CONDITION */) {
            count++;
        }
    }

    return count;
}

int compare(const void *a, const void *b)
{
    char *s1 = *(char **)a;
    char *s2 = *(char **)b;

    int x = property(s1);
    int y = property(s2);

    return x - y;
}
```
- change property into what you need
```C
qsort(&argv[1], // function call to qsort
      argc - 1, // num user args
      sizeof(char *), // shows each item sorted is string pointer
      compare); // function that decides order
```
## Solving Similar Sorting FRQs
- we only need to change specific parts of the template, NOT THE WHOLE THING
## qsort With Structs
```C
typedef struct {
    int id;
    int score;
} Student;
```
- and we define 
```C
Student arr[100];
```
- each element is Student, so the comparator would be
```C
int compare(const void *a, const void *b)
{
    const Student *s1 = a;
    const Student *s2 = b;

    return s1->score - s2->score;
}
```
- we **do not** use Student** because *the array holds actual structs*
  - does NOT hold pointers to structs
## Linked Lists Core Template
- practice exam 2 uses 
```C
typedef struct particle {
    int mass;
    struct particle *next;
} ParticleSet;
```
- this can be generalized into
```C
typedef struct node {
    int value;
    struct node *next;
} Node;
```
## . Versus ->
```C
Node n; // if given this
n.value // use this

Node *p; // if given this
p->value // use this 
```
### Rule Regulation
```
actual struct uses .
pointer       uses ->
```
## Why Node**?
- lets assume 
```C
  Node *head = NULL; 
```
- if a function must change head, it requires **&head**, which has a type **Node****
```C
void push(Node **head, int value)
```
- REMEMBER THIS
```
Function only READS/traverses head:
    Node *

Function must CHANGE head:
    Node **
```
## Push to Front
- MEMORIZE EXACTLY 
```C
void push(Node **head, int value)
{
    Node *newNode = malloc(sizeof(Node));

    newNode->value = value;

    newNode->next = *head;

    *head = newNode;
}
```
- visual flow
```C
Before:

head
 ↓
5 → 3 → NULL // head stores 5

new node = 8

newNode->next = *head

8 → 5 → 3 // head still points to 5

*head = newNode // changes head to 8 not 5

head
 ↓
8 → 5 → 3 → NULL // 8 added in front of old head, and is now the new head
```
## Pop From Front
```C
void pop(Node **head)
{
    if (*head == NULL)
        return;

    Node *temp = *head;

    *head = (*head)->next;

    free(temp);
}
```
- flow
  - SAVE, MOVE, FREE
- we cannot lose the old node before freeing it 
## Empty/Top/Free
```C
int empty(Node *head)
{
    return head == NULL;
}
```
- top
```C
int top(Node *head)
{
    return head->value;
}
```
- free everything
```C
void freeList(Node **head)
{
    while (*head != NULL) {
        pop(head);
    }
}
```
- practice exam starters left stack functions to be implemented 
## Stack Recognition Rule 
- use stacks when 
  - newest or nearest previous item needs to be handled first
  - incoming item may repeatedly remove earlier items 
- examples
  - particle collisions
  - matching parantheses
  - undo
  - backtracking history
  - cancelling adjacent opposites
## Particle Collision Problem 
- pracitce exam 2 states that
```
positive = moving right
negative = moving left

absolute value = mass
```
- smaller mass would die in collision; if masses were equal, they *both* die
### Only Collision Arrangement
```C

```