# Exam 1 Lecture Review
## C Operators, Types, and Tracing
### Basic Types
```C
int x;
char c;
float f;
double d;
long n;
```
- statically typed, meanign expressions and variables have types that are checked during compilation
- char
  - one byte and represents ASCII character
```C
'0' through '9'
'A' through 'Z'
'a' through 'z'

// instead of memorizing ASCII values
if (c >= 'a' && c <= 'z')
// digit character to actual number
char c = '8';
int x = c - '0';
// x = 8
```
- integer division
```C
5 / 2 /// leads to 2 NOT 2.5

5.0 / 2 // leads to 2.5 instead of 2
```
- dangerous casting
```C
x = 1;
y = 2;
double z = (double)(x / y); // performs integer division BEFORE converting

double z = (double)x / y;
```
- sizeof
  - returns the number of bytes used by a type or object
```C
sizeof(int)
sizeof(double)
sizeof(x)

// used in
int arr[10];

int n = sizeof(arr) / sizeof(arr[0]);
```
- this calculates the num of array elements **only when arr is still an actual array in that scope**
- pre-increment vs. post-increment 
```C
i++ // use i, then increase
++i // increase i, then use

int i = 4;
int x = i++; // leads to x = 4 and i = 5

int i = 4;
int x = ++i; // leads to x = i = 5
```
- assignment vs. equality
```C
x = 5 // x is assigned value of 5

x == 5 // tests whether x is equal to 5

if(x = 5) // legal and assigns 5, which is nonzero and true
```
- logical vs bitwise
```C
&&
||
!
```
```C
&
|
^
~
<<
>>
```
- true and false in C
  - for conditions 
```
0 = false
anything nonzero = true
```
  - therefore, if(5) runs but if(0) does not
## Control Flow
- if
```C
if (condition) {
    // runs if condition != 0
}
// if-else
if (condition) {
    ...
}
else {
    ...
}
// multiple cases
if (x == 0) {
    ...
}
else if (x == 1) {
    ...
}
else {
    ...
}
```
- dangling else
```C
if (a)
    if (b)
        x++;
    else /// else attaches to the nearest unmatched if
        y++;

// safer
if (a) {
    if (b) {
        x++;
    }
    else {
        y++;
    }
}
```
- ternary operator
  - short if/else expression
```C
condition ? value_if_true : value_if_false

// which is equivalent to
if (a < b)
    min = a;
else
    min = b;
```
- while
  - used when the number of iterations is unknown
```C
while (condition) {
    ...
}

while (n > 0) {
    ...
}
```
- do-while
  - runs the body at least once since the condition is tested afterwards
```C
do {
    ...
} while (condition);
```
- for
  - when we know/count iterations
```C
for (initialization; condition; update) {
    ...
}

// equivalent to
int i = 0;

while (i < n) {
    ...
    i++;
}
```
- break
  - exits the nearest loop or switch
```C
while (...) {

    if (condition)
        break;
}
```
- continue
  - skips the rest of the current iterations and begins the next one
```C
for (int i = 0; i < 10; i++) {

    if (i == 5)
        continue; // does not print 5

    printf("%d\n", i);
}
```
- swtich
```C
switch (x) {

case 0:
    ...
    break; // prevents fall-through
 
case 1:
    ...
    break; // prevents fall-through

default:
    ...
}
```
## Functions
- function structure
```C
return_type function_name(parameters)
{
    ...
    return value;
}

// example
int add(int a, int b)
{
    return a + b;
}
int result = add(3, 4); // call
```
- void
  - for when functions return nothing
```C
void printHello(void)
{
    printf("Hello\n");
}
```
- return
  - when return executes, the function **immediately ends** and anything afterwards doesn't execute
```C
return value;
```
- function prototype
  - function defintion comes **after** the first call
```C
int main(void)
{
    printf("%d", add(2, 3));
}

int add(int a, int b)
{
    return a + b;
}
```
- prototype tells compiler the return type and parameter types
- functions cannot be defiend inside other functions
  - they are defined **seperately**
- pass by value
  - primitive values are copied
```C
void change(int x)
{
    x = 100;
}

int a = 5;
change(a); // a remains 5 since x was an independent copy
```
- local variables
```C
void func(void)
{
    int x = 10;
}
``` 
- static local variable
```C
void func(void)
{
    static int count = 0;

    count++; // only visible inside the function, but value persits between function calls

    printf("%d\n", count);
}
```
- global variable
  - declared outside functions
  - exists for the program duration and can be accessed by functions
```C
int x = 5;

int main(void)
{
    ...
}
```
- static global
  - declared outside of functions but limited in visibility to same source file
```C
static int x = 5;
```
- function call stack
  - each function call gets a stack frame with things like copied args, local vars, and return information
    - discarded when function end 
## Arrays and Strings
- array basics 
```C
int a[5]; // indices a[0] to a[4]
```
- array indices start at zero and go to (n-1)
```C
int a[5] = {1, 2, 3, 4, 5}

int b[4] = {1, 2} // leads to it having value of {1, 2, 0, 0}
```
- strings are character arrays
```
"hello" = {'h', 'e', 'l', 'l', 'o', '\0'}
```
- \0 is the endline character, so 'hello' needs to be stored in an array initialized as a[6] NOT a[5]
- traverse string 
```C
for (int i = 0; s[i] != '\0'; i++) {

    // process s[i]
}

s[i] = one char
s = whole string 
```
- array size
  - inside the scope where the array exists
```C
int a[10];

int count =
    sizeof(a) / sizeof(a[0]);
```
- one passed to a function, we cant use sizeof() since the function gets a pointer to the start of the array
```C
void printArray(int a[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d\n", a[i]);
}
```
## Pointers
- two core operators
  - & = address of something
  - "*" = the value stored at addresss
```C
int x = 10;

int *p = &x; // p stores address of x
*p // give value stored at address p, which is the address of x, which stores 10
*p = 25; // changes the value of x by going to the address stored at p, and changing the value from 10 to 25
```
- pointer picture
  - p doesn't contain 10, it contrains the address where 10 is stored
- pointer declaration trap
  - int *a, b;
    - a = int*
    - b = int
    - ONLY ONE is a pointer
  - int *a, *b;
    - both are pointers
- passing pointer to modify original
```C
void change(int *p)
{
    *p = 100;
}

// then call
int x = 5;

change(&x);
```
- output is x = 100, because 
  - &x -> address of x
  - p -> recieves address
  - *p -> accesses original x 
- arrays and pointers
  - arrays passed to functions are references/pointers to the original array instead of independent copies
```C
void change(int a[])
{
    a[0] = 100;
}

int a[3] = {1,2,3};

change(a);

// leads to
100 2 3 // function access the oriinal array
```
## Pointer Arithmetic
- pointer +1
- if there exists int *p;
  - p + 1 doesn't mean one byte forward, it means move forward **one int**
    - it moves forward by sizeof(int)
```
pointer + k

moves by

k × sizeof(pointed-to type)
```
- array index and pointer equivalent
  - a[i] is equivalent to *(a + i)
    - a[3] means access the item three elements from the start
- pointer subtraction
  - if two same-type pointers point into the same array like region
    - (last - first) gives number of **elements** not bytes between them
- array vs pointer
  - arrays and pointers are similar due to array decay, but are not the same objects
## Memory: Stack/Heap/Static
- stack
  - contains
    - func local vars
    - func call frames
    - automatic arrays
  - automatic
    - created when the function begins, and is wiped when function ends 
  - **dont return a pointer to an ordinary local variable**
- heap
  - dynamic allocation
  - malloc(), calloc(), realloc()
  - memory remains until it is freed by programmer
- static/global
  - contains
    - global vars
    - static vars
    - string literals
  - lasts for the duration of the program 
- malloc()
```C
int *a =
    malloc(n * sizeof(int));

// general form
TYPE *p =
    malloc(NUMBER * sizeof(TYPE));
```
- consider allocation failure
  - malloc requests bytes on heap, and returns NULL if allocation fails 
```C
if (a == NULL) {
    perror("malloc");
    exit(1);
}
```
- calloc
  - contents are initialized to zero, unlike in malloc
    - good for
      - visited arrays
      - frequency arrays
      - count arrays
      - zero-filled grids
```C
int *a =
    calloc(n, sizeof(int));
```
- realloc
  - resizing things
  - returned pointer may be different 
```C
p = realloc(p, newSize);

// safer general pattern
int *temp = realloc(p, newSize);

if (temp != NULL) {
    p = temp;
}
```
- free
  - free all memory allocated, and only free memory allocated using malloc/calloc/realloc
  - double freeing is **invalid**
```C
free(p);
p = NULL;
```
- dynamic 2D array
```C
int **a =
    malloc(rows * sizeof(int *));

for (int i = 0; i < rows; i++) {

    a[i] =
        malloc(cols * sizeof(int));
}

// free in reverse structure
for (int i = 0; i < rows; i++) {
    free(a[i]);
}

free(a);
```
## Structs
- define structs
  - aggregates related variables, and they can be of different types
```C
typedef struct {
    int id;
    int score;
    char grade;
} Student;
```
- . operator
  - if actual struct
```C
Student s;

// use
s.id
s.score
```
- -> operator
  - if pointer
```C
Student *p = &s // the value stored at address p is the address of s 

//use
p->id
p->score // equivalent to (*p).score
```
- important pointer to struct picture
  - Student *x = &students[3];
    - x points to the entire player/student struct, not to one field
  - x -> score
    - go to the struct x points to, and then access score
```C
// to make a pointer to the score
int *scorePtr =
    &students[3].score;
```
- structs passed by value
```C
void change(Student s)
{
    s.score = 100;
}
```
- original struct is not change, since struct assignment and function passing copies the members
```C
// to modify the original
void change(Student *s)
{
    s->score = 100;
}

change(&student); // then call
```
## Linked Lists
- node template
```C
typedef struct node {
    int value;
    struct node *next;
} Node;
```
- visualization
```
+-------+------+     +-------+------+
| value | next | --> | value | next | --> NULL
+-------+------+     +-------+------+
```
- head
  - Node *head = NULL
    - empty list
  - IMPORTANT
    - head is a POINTER to the first node, and is NOT a node itself
- create node
```C
Node *createNode(int value)
{
    Node *p = malloc(sizeof(Node));

    if (p == NULL)
        exit(1);

    p->value = value;
    p->next = NULL;

    return p;
}
```
- add to front
  - what if we want to add a node to an already made linked list
```C
// create new node
8
// make the new node
newNode->next = *head;
// now
newNode
 ↓
 8 → 5 → 3 → NULL

head
 ↓
 5 → 3 → NULL

// then
*head = newNode;
// so now
head
 ↓
 8 → 5 → 3 → NULL
```
- general code
```C
void push(Node **head, int value)
{
    Node *newNode =
        malloc(sizeof(Node));

    newNode->value = value;

    newNode->next = *head;

    *head = newNode;
}
```
- why Node**?
  - assume that the caller has Node *head;
    - to change the actual headm the function needs its address &head
    - type: Node**
    - so, 
      - void push(Node **head, ...)
  - rule 
```
Need to change NODE contents?
Node *

Need to change HEAD POINTER itself?
Node **
```
- remove front 
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
- so memorize
  - SAVE, MOVE, FREE
- traverse list
```C
Node *p = head;

while (p != NULL) {

    printf("%d\n", p->value);

    p = p->next;
}
```
- find last
```C
Node *findLast(Node *head)
{
    if (head != NULL) {

        while (head->next != NULL) {
            head = head->next;
        }
    }

    return head;
}
```
- append
```C
Node *append(Node *head, Node *newNode)
{
    Node *last = findLast(head);

    if (last == NULL)
        return newNode;

    last->next = newNode;
    newNode->next = NULL;

    return head;
}
```
## ENUMS and const
- enum
```C
typedef enum {
    RED,
    GREEN,
    BLUE
} Color;
```
- by default
  - RED = 0
  - GREEN = 1
  - BLUE = 2
- integer-like but makes the code more readable
```C
// example
enum TYPE {S, I , R} 
// better than remembering 
0 = susceptible
1 = infected
2 = recovered
```
- const pointer cheatsheet
```C
const int *p; // pointer to constant int
```
- can change p put cannot change through pointer *p
  - DATA is constant 
```C
int * const p = &x; // constant pointer
```
- cannot change p, but can change *p
  - ADDRESS is constant 
```C
const int * const p = &x;
```
- cannot change p or *p
## Function Pointers and qsort
- function pointers 
  - int (*func)(int, int)
    - func is a pointer to a funtion that accepts two ints and returns an int 
  - introduced so generic functions like qsort() can get custom behaviors 
- qsort
  - does not know the element type or how the elements should be ranked, the comparator supplies that information
```C
qsort(
    array,
    number_of_elements,
    size_of_one_element,
    comparator
);

// example
qsort(a,
      n,
      sizeof(int),
      compareInt);
```
- comparator rules 
```C
int compare(const void *a,
            const void *b)

// returns 
negative → a comes BEFORE b
0        → equal
positive → a comes AFTER b

// integer ascending 
int compareInt(const void *a,
               const void *b)
{
    int A = *(int *)a;
    int B = *(int *)b;

    return A - B;
}
```
- A-B rule
  - for small integer properties, A-B means ascending 
  - the reverse, B-A, means descending 
## Practice Exam FRQ1: Sort Strings 
- command line arguments 
  - argv[0] = program name
  - actual inputs begin at argv[1]
  - num user inputs = argc - 1
```C
int main(int argc, char *argv[])

// example
./place abc XYZ hello

// returns and is
argc = 4

argv[0] = "./place"
argv[1] = "abc"
argv[2] = "XYZ"
argv[3] = "hello"
```
- atoi
  - command line values are strings 
```C
int n = atoi(argv[1]); // converts "25" to 25
```
- count lowercase
```C
int countLower(char *s)
{
    int count = 0;

    for (int i = 0;
         s[i] != '\0';
         i++)
    {
        if (s[i] >= 'a' &&
            s[i] <= 'z')
        {
            count++;
        }
    }

    return count;
}
```
- string comparator - the char** rule 
  - argv contains char*, so each item being sorted is also a char*
  - qsort() gives the address of the item tho, so the address of a char* is char**
```C
char *s1 = *(char **)a;
char *s2 = *(char **)b;
```
- full comparator
```C
int compare(const void *a,
            const void *b)
{
    char *s1 = *(char **)a;
    char *s2 = *(char **)b;

    int A = countLower(s1);
    int B = countLower(s2);

    return A - B;
}
```
- sorting command-line strings
```C
qsort(
    &argv[1],
    argc - 1,
    sizeof(char *),
    compare
);

// interpret as
&argv[1]
→ start at first USER string

argc - 1
→ number of user strings

sizeof(char *)
→ each array element is a string pointer

compare
→ ranking rule
```
- full FRQ template
```C
#include <stdio.h>
#include <stdlib.h>

int property(char *s)
{
    int count = 0;

    for (int i = 0;
         s[i] != '\0';
         i++)
    {
        if (/* CONDITION */)
            count++;
    }

    return count;
}


int compare(const void *a,
            const void *b)
{
    char *s1 = *(char **)a;
    char *s2 = *(char **)b;

    int A = property(s1);
    int B = property(s2);

    return A - B;
}


int main(int argc, char *argv[])
{
    qsort(
        &argv[1],
        argc - 1,
        sizeof(char *),
        compare
    );

    ...
}
```
- if the exam changes lowercase to digits
  - only change the condition of the if(CONDITION) section
## PRACTICE EXAM FRQ2: Stack
- collision recognition
```
// this is only recognized as a collision
positive     negative

   →            ←
top > 0 && current < 0

// the rest do not collide
← ←
→ →
← →
```
- mass
  - sign is direction, so mass is abs(value)
```
-8

direction = left
mass = 8
```
- core collision algorithm 
  - for every current
```
IF current > 0:
    PUSH it

ELSE:

    WHILE:
        stack not empty
        AND top > 0
        AND abs(top) < abs(current)

        POP top

    AFTER LOOP:

    IF top positive
       AND equal magnitude:
           POP
           current disappears

    ELSE IF stack empty
         OR top negative:
           PUSH current

    ELSE:
           current loses
           do nothing
```
- why while?
  - if an input is {2, 4, 6, -10}
    - -10 destroys all the other particles, meaning it goes through *multiple collisions*
    - needs a while loop to cover those cases, instead of a if statement 
- complete fight skeleton
```C
ParticleSet *fight(int *particles,
                   int count)
{
    ParticleSet *stack = NULL;

    for (int i = 0; i < count; i++)
    {
        int current = particles[i];

        if (current > 0)
        {
            addToSet(&stack,
                     current);
        }

        else
        {
            while (
                !isSetEmpty(stack)
                &&
                topOfTheSet(stack) > 0
                &&
                abs(topOfTheSet(stack))
                    < abs(current)
            )
            {
                removeFromSet(&stack);
            }


            if (
                !isSetEmpty(stack)
                &&
                topOfTheSet(stack) > 0
                &&
                abs(topOfTheSet(stack))
                    == abs(current)
            )
            {
                removeFromSet(&stack);
            }

            else if (
                isSetEmpty(stack)
                ||
                topOfTheSet(stack) < 0
            )
            {
                addToSet(&stack,
                         current);
            }

            // otherwise:
            // current loses
        }
    }

    return stack;
}
```
## File I/O
- open file
```C
FILE *fp =
    fopen("file.txt", "r");

// always check
if (fp == NULL)
{
    perror("file.txt");
    return 1;
}
```
- file modes
  - 'r' - read exisiting file
  - 'r+' - read and write
  - 'w' - write; create/truncate
    - can erase or truncate existing contents 
  - 'w+' - read\write; create/truncate
  - 'a' - append; create if needed
  - 'a+' - read and append 
- close
```C
fclose(fp);

// good pattern
FILE *fp = fopen(...);

if (fp == NULL)
    ...

// use file

fclose(fp);
```
- fgets
  - reads until one of the following happens 
    - (n-1) characters
    - newline
    - EOF (end of file)
```C
char buffer[100];

while (fgets(buffer,
             sizeof(buffer),
             fp) != NULL)
{
    printf("%s", buffer);
}
```
- character at a time input
  - fgets() returns int, not char, so it can represent EOF
```C
int c;

while ((c = fgetc(fp)) != EOF)
{
    putchar(c);
}
```
- formatted file i/o
  - like scanf and printf but with a file 
```C
fscanf(fp, "%d", &x);

fprintf(fp, "%d\n", x);
```
- fseek/ftell
```C
long pos = ftell(fp); // current position
fseek(fp, 0, SEEK_SET); // to move
fseek(fp, 200, SEEK_CUR); // 200 bytes forward
fseek(fp, -1, SEEK_END); // one byte before the end 
```
- feof
```C
feof(fp) // checks EOF after a read qattempt has encountered the end 

// safer pattern instead of desiging the loop around feof() before reading
while (fgets(buffer,
             sizeof(buffer),
             fp) != NULL)
{
    ...
}
```
- errno/perror
  - some library failures set errno, perror("open failed") prints readable message associated with the current error
