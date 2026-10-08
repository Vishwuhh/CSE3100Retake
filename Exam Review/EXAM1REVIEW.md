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
positive then negative

->   <-
```
- this means that below is the **collision condition**
```C
top > 0 && current < 0
```
- the following **does not** collide
```C
← ←
→ →
← →
```
### General Particle Algorithm
```
IF current > 0
    PUSH

ELSE current < 0

    WHILE
        stack is not empty
        AND top > 0
        AND |top| < |current|

        POP top

    AFTER LOOP:

    CASE 1:
    top positive and same magnitude
        POP top
        current also disappears

    CASE 2:
    stack empty
    OR top negative
        PUSH current

    CASE 3:
    top positive and larger
        current disappears
        DO NOTHING
```
### Particle Template
```C
Node *fight(int particles[], int count)
{
    Node *stack = NULL;

    for (int i = 0; i < count; i++) {

        int current = particles[i];

        if (current > 0) {

            push(&stack, current);
        }

        else {

            while (stack != NULL &&
                   top(stack) > 0 &&
                   abs(top(stack)) < abs(current))
            {
                pop(&stack);
            }


            if (stack != NULL &&
                top(stack) > 0 &&
                abs(top(stack)) == abs(current))
            {
                pop(&stack);
            }

            else if (stack == NULL ||
                     top(stack) < 0)
            {
                push(&stack, current);
            }

            else {
                // current is destroyed
            }
        }
    }

    return stack;
}
```
### Why while Instead Of if
- if we're given the numbers {2, 4, 6, -10}
  - -10 could destroy {6, 4, 2}, meaning one particle could cause multiple collisions 
  - means that we would need a **while()** instead of **if()**
- exam clue
  - if one item could repeatedly do something, you would probably need a while loop
## Null Pointer Safety
```C
if(p != NULL && p->value > 0)
```
- this is correct, since C evaluates && from the left to the right 
- general rule
  - check pointer first, and then dereference second
## Digit Processing 
- extract the last digit
```C
digit = n % 10
```
- remove the last digit
```C
n /= 10
```
- universal digit loop
```C
while (n > 0)
{
    int digit = n % 10;

    // process digit

    n /= 10;
}
```
## Sum of Squares of Digits
```C
int digitSquareSum(int n)
{
    int sum = 0;

    while (n > 0)
    {
        int digit = n % 10;

        sum += digit * digit;

        n /= 10;
    }

    return sum;
}
```
## Happy Number Framework
- assignment facts
  - happy -> eventually reaches 1
  - unhappy -> eventually reaches 4
```C
while (n != 1 && n != 4)
{
    int sum = 0;

    while (n > 0)
    {
        int digit = n % 10;

        sum += digit * digit;

        n /= 10;
    }

    n = sum;

    printf("%d\n", n);
}
```
- then
```C
if (n == 1) {
    // happy
} else { 
    // unhappy
}
```
## Digit FRQ Translation Table 
- sum digits
  - sum += digit
- sum squares 
  - sum += digit * digit
- sum cubes
  - sum += digit * digit * digit 
- count even digits
```C
if(digit % 2 == 0) {
    count++
}
```
- largest digit
```C
if(digit > max) {
    max = digit
}
```
- reverse number
  - result = result*10 + digit
## Numeric Series Without pow()
- if problem contains 1/(C^i), dont calculate pow(C, i)
  - **use a running factor**
```C
double factor = 1.0;

for (int i = 0; i <= n; i++)
{
    // use factor

    factor /= C;
}
```
- example
```C
factor starts 1
then 1/C
then 1/C²
then 1/C³
```
## Pi Problem Framework
```C
double pi = 0.0;
double factor = 1.0;

for (int i = 0; i <= n; i++)
{
    double term =
        factor *
        (
          4.0 / (8*i + 1)
        - 2.0 / (8*i + 4)
        - 1.0 / (8*i + 5)
        - 1.0 / (8*i + 6)
        );

    pi += term;

    factor /= 16.0;
}
```
## Integer Divison
```C
double x = 1 / 2; // wrong, we need one of the integers be a double

double x = 1.0 / 2; // this leads to the correct answer (0.5)
```
- have to have at least one number be floating-point
## Recursion and Backtracking 
- odd-sum problem askes for a combination of distinct off values meeting a count and a sum
  - means that the question for each step is either "Take value" or "Skip value"
- this means that this is a **recursion/backtracking**
### Recursive Parameters as "What Remains?"
- for OddSumHelp(count, bound, value)
  - count = how many numbers still needed
  - bound = largest number still available
  - value = how much sum still needed
### Include Branch
- if using bound
```
one fewer num needed
target decreases
next allowable value increases
```
- meaning
```C
OddSumHelp(
    count - 1,
    bound - 2,
    value - bound
) // next lower odd number is bound - 2
```
### Skip Branch
- if not using bound
```C
OddSumHelp(
    count,
    bound - 2.
    value
)
```
### Base Cases
- success
```C
if (count == 0 && value == 0)
    return 1;
```
- failure
```C
if (count <= 0 ||
    bound <= 0 ||
    value <= 0)
{
    return 0;
}
```
- NOTE
  - check success **before** failure
    - count = 0 and value = 0 are valid states for the variables 
### Backtracking Template
```C
int solve(int count, int bound, int value)
{
    if (count == 0 && value == 0)
        return 1;

    if (count <= 0 ||
        bound <= 0 ||
        value <= 0)
        return 0;


    // TRY TAKING CURRENT OPTION FIRST
    if (solve(count - 1,
              NEXT_BOUND,
              value - CURRENT))
    {
        printf("%d ", CURRENT);
        return 1;
    }


    // OTHERWISE SKIP CURRENT OPTION
    return solve(count,
                 NEXT_BOUND,
                 value);
}
```
- adapt current and next_bound based on the problem
### Why "Take First"?
- solution preders including the largest possible odd number when possible
  - so do INCLUDE first before SKIP
### General Recusion Detector
- if saying things like
```
find exactly k things
choose distinct values
find any combination
include or exclude
return whether a solution exists
```
- think
  - Base Cases + Take + Skip
## Random Walk/Simulation
- HW2 asks for a 2D random walk from (0,0) until a boundary is reached
- general simulation formula
```C
INITIALIZE

WHILE not finished:

    generate event

    update state

    check stopping condition

    record result
```
### Random Direction
- int r = rand() % 4
  - gives either {0, 1, 2, 3} as possible values
  - each digit specifies a direction (up, down, left, right)
```C
if (r == 0) // down
    y--;

else if (r == 1) // right
    x++;

else if (r == 2) // up
    y++;

else // left
    x--;
```
### Visited Grid
- if the coordinate range inside the boundary is [-(n - 1), (n - 1)]
  - there are (2n - 1) possible positions per dimension
- grid size therefore would be 
```C
int size = 2*n - 1
```
- total cells would be (size)^2
### Negative Coordinates -> Array Index
- since the range of coordinates is [-(n - 1), (n - 1)], we can map index by 
```C
index = coordinate + (n - 1)
```
```
// how this maps to
coordinate  -2 -1 0 1 2
index        0  1 2 3 4
```
- general rule
  - ARRAY_INDEX = REAL_COORDINATE + OFFSET
### Don't Count Double Visits
- we cannot count repeated visits as unique if the probelm asks for unique coordinates visited 
```C
if (visited[row][col] == 0)
{
    visited[row][col] = 1;
    count++;
}
```
### Mark Starting Posiiton
- walker begins at the origin (0, 0), so
```C
visited[n-1][n-1] = 1;

int count = 1;
```
- do not need to wait until it returns to the starting point
### 2D Walk Skeleton
```C
double walk(int n)
{
    int size = 2*n - 1;

    int visited[size][size];

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            visited[i][j] = 0;


    int x = 0;
    int y = 0;

    int count = 1;

    visited[n-1][n-1] = 1;


    while (x != n &&
           x != -n &&
           y != n &&
           y != -n)
    {
        int r = rand() % 4;

        // move x/y


        if (x == n ||
            x == -n ||
            y == n ||
            y == -n)
        {
            break;
        }


        int row = x + n - 1;
        int col = y + n - 1;


        if (visited[row][col] == 0)
        {
            visited[row][col] = 1;
            count++;
        }
    }


    return (double)count /
           (size * size);
}
```
## Random-Number Range 
- rand() % N give us [0, (N - 1)]
  - if we want [1, 6], we could make [(rand() % 6) + 1]
## Structs
- HW3 uses arrays of player and property structures
```C
typedef struct Player
{
    int id;
    int loc;
    long balance;
} TPlayer;
```
- use structures when one object has **multiple** values   
```
// this can be replaced
player_id[]
player_location[]
player_balance[]

// with this
TPlayer players[];
```
### Struct Array Access
- TPlayer p[10] 
  - creates array of 10 player structures
- p[3].balance
  - sets balance for player 3
- p[3].loc
  - sets location for player 3
- TPlayer *x = &p[3]
  - creates pointer x with the address of player 3
- then
  - x->balance
    - goes to the TPlayer struct that x points to, and then accesses what its balance is 
```
x = pointer to the whole player struct
x->balance = balance field inside the struct x points to

TPlayer *x = &p[3];
x->balance  → p[3].balance
x->loc      → p[3].loc
x->id       → p[3].id
```
### Circular Board
- monopoly properties go
  - {0, 1, 2, ... , (n-1), 0, 1, ...}
- use
```C
newLocation =
    (oldLocation + steps) % n;
```
### Detecting Passing Zero
- before modulo
```C
int raw = oldLocation + steps;

if (raw >= n)
{
    // passed or reached start
}
```
- then
```C
location = raw % n
```
### Two Dice
```C
int d1 = rand() % 6 + 1; 
int d2 = rand() % 6 + 1;

int steps = d1 + d2; // you move as much as the two dies sums are
```
### Property Ownership Logic
- if a property is unowned
  - owner_id == -1
    - player 0 is a valid owner, so we have to set owner_id to be something negative 
- use
```C
if (prop[pos].owner_id == -1)
{
    prop[pos].owner_id = player.id;
}
```
- if the property is owned by you
```C
else if (prop[pos].owner_id == player.id)
{
    // nothing
}
```
- other person's property
```C
else
{
    // pay rent
}
```
### Generic Struct-Based Simulation
```
INITIALIZE ARRAY OF OBJECTS

FOR each round:

    FOR each object:

        generate event

        update position

        check special event

        update object

        update other object if needed

        if stopping condition:
            stop
```
## 3D Grids and Dynamic Memory
- HW 3 diffusion dynamically allocates storage for coordinates [-n, n] on (x, y, z)
  - number of coordinates per axis
    - int side = 2*n + 1
  - total locations
    - side * side * side
  - allocate
```C
int *grid =
    calloc(side * side * side,
           sizeof(int));
```
## Flatten 2D Array
- if given the row and column
- with the cols columns
  - index = row * cols + columns
## Flatten 3D Array
- for dimensions, X * Y * Z
- index
```C
index =
    x * Y * Z
    + y * Z
    + z;
```
- for a diffusion cube with equal side lengths
```C
index =
    X * side * side
    + Y * side
    + Z;

// where
X = x + n;
Y = y + n;
Z = z + n;
```
## Distance Without sqrt()
- we can square the distance formula to use it without having to use the sqrt() function
```C
if (x*x + y*y + z*z <= radius*radius)
{
    ...
}
```
- and for 2D
```C
if (x*x + y*y <= radius*radius)
```
## 3D Random Walk
```C
int r = rand() % 6;

if (r == 0)
    x--;
else if (r == 1)
    x++;
else if (r == 2)
    y--;
else if (r == 3)
    y++;
else if (r == 4)
    z--;
else
    z++;
```
- follow the exact direction mapping specified in question
## Malloc, Calloc, and Free
- malloc
  - memory values are not intialized
```C
int *p = malloc(n * sizeof(int));
```
- calloc
  - memory begins at zero
```C
int *p = calloc(n, sizeof(int));
```
- free
```C
free(p);
```
### When to Use Calloc
- useful for
```
frequency arrays
visited arrays
grids
counts
```
- since zero is usually the desired initial values 
## Hash Tables 
- HW4 represents an infected-hosts location table as an **array of linked-lists**
```
table[0] → node → node → NULL
table[1] → NULL
table[2] → node → NULL
table[3] → node → node → NULL
```
- this is a HASH TABLE with SEPERATE CHAINING
### Hash Table Purpose
- does
```C
key
 ↓
hash
 ↓
bucket
 ↓
search only that linked list
```
- and in general
  - int index = hash(key) % N
- then
  - Node *p = table[index];
- search bucket
```C
while (p != NULL)
{
    if (/* match */)
        return 1;

    p = p->next;
}
```
### Hash Table Insert
- we insert to the front
```C
void addFirst(Node **head, Node *newNode)
{
    newNode->next = *head;
    *head = newNode;
}
```
- hash-table bucket insertion uses the same linked-list operation as stack push
### Hash Table Search Template
```C
int search(Node *table[],
           int N,
           int key)
{
    int index = hash(key) % N;

    Node *p = table[index];

    while (p != NULL)
    {
        if (p->key == key)
            return 1;

        p = p->next;
    }

    return 0;
}
```
- if we store coordinates, we must compare coordinates too
```C
if (p->x == x && p->y == y)
```
### WHy Hash Table Size Matters
- a table with too few buckets creates long linked lists and slower searches
```
small N
→ many items per bucket
→ long list search
→ slower

larger useful N
→ fewer items per bucket
→ faster lookup
```
### enum
- HW4 uses enum TYPE {S, I ,R}
  - for susceptible, infected, and recovered hosts
```C
// use this for enum
host.type = S;
host.type = I;
host.type = R;

// instead of this
host.type = 0;
```
### Grid Wraparound 
- if coordinate range = [-k, k]
- moving right
```C
x++;

if (x > k)
    x = -k;
```
- moving left
```C
x++;

if (x < -k>)
    x = k;
```
- similarly for y
### Boundary vs. Wraparound
- HW2 random walk
  - touch boundary -> STOP
- HW4 epidemic
  - cross boundary -> WRAP AROUND
## Old State vs. New State
- susceptible hosts infested during a time step becomes contagious in the next
  - do not let newly changed objects immediately affect things that should still see the old round
- general concept
```C
CURRENT ROUND STATE
↓
calculate changes
↓
apply new state
↓
NEXT ROUND
```
- follow the round order given from the question
## Loop Selection
- when number of repitions is known
```C
for(int i = 0; i < n; i++)
```
- examples
```C
n particles
m trials
n simulation steps
every array element
```
- use while when
  - repetitions continue until something happen or when one item causes repeated events
```C
while (n != 1 && n != 4)

while (top is destroyed)
```
## Off-By-One Guide
- there is a difference between (i < n) and (i <= n)
  - if a problems says i = 0 through n, this means 
    - for(i = 0; i <= n; i++)
  - if processing n array elements from {0, .. , (n-1)}
    - for(i = 0; i < n; i++)
## Null/Empty Edge Case
- for linked lists 
  - if(head == NULL) means empty
```C
before head->value,

check head != null
```
- this is unless the problem explicitly guarantees nonempty
## Output Tracing Method
- create tables for tracking tracing
## FRQ Emergency Procedure: Sorting
```C
STEP 1
What is one element?

STEP 2
What property determines order?

STEP 3
Write property() helper.

STEP 4
Write comparator.

STEP 5
Call qsort.
```
- string array
```C
char *s1 = *(char **)a;
char *s2 = *(char **)b;
```
- struct array
```C
const Type *x = a;
const Type *y = b;
```
## FRQ Emergency Procedure: Linked List
- if need to add front
```C
new->next = *head;
*head = new;
```
- if remove front
```C
temp = *head;
*head = (*head)->next;
free(temp);
```
- if traversing linked list
```C
while (p != NULL)
{
    ...
    p = p->next;
}
```
## FRQ Emergency Procedure: Stack Problem 
- ask
```
Does newest previous item matter first?

Can current item repeatedly remove earlier items?
```
- if yes
```
use stack
```
- basic algorithm
```
for each current:

    resolve current against stack top

    while repeated interaction possible:
        pop

    if current survives:
        push
```
## FRQ Emergency Procedure: Digits
- start with
```C
while (n > 0)
{
    int digit = n % 10;

    // do something with digit

    n /= 10;
}
```
## FRQ Emergency Procedure: Recursion
- write
```C
int solve(...)
{
    if (SUCCESS)
        return 1;

    if (IMPOSSIBLE)
        return 0;

    if (solve(TAKE))
    {
        // record current choice
        return 1;
    }

    return solve(SKIP);
}
```
- then decide
```
what changes when TAKING?
what changes when SKIPPING?
```
## FRQ Emergency Procedure: Random Simulation
```C
initialize state;

while (!finished)
{
    int r = rand() % choices;

    update state;

    record result;
}

return result;
```
- then insert exact stopping condition
## FRQ Emergency Procedure: Struct Simulation
- use
```C
for (int round = 0; round < max_rounds; round++)
{
    for (int i = 0; i < num_objects; i++)
    {
        // generate event

        // update object[i]

        // inspect destination/other object

        // update interacting objects

        // check stopping condition
    }
}
```
## FRQ Emergency Procedure: Hash Table
```C
int index = hash(key) % N;
```
- insert
```C
newNode->next = table[index];
table[index] = newNode;
```
- search
```C
Node *p = table[index];

while (p != NULL)
{
    if (match)
        return 1;

    p = p->next;
}
```