# FRQ Style Questions

## FRQ 1
- Write a program that sorts command-line strings in ascending order by their number of digit characters. If the digit counts tie, sort by string length, then alphabetically.
- framework
```
1. Write a helper that calculates the property.
2. Comparator receives addresses of two strings.
3. Convert void * arguments into string pointers.
4. Compare the property values.
5. Call qsort() on argv[1] onward.
```
- code
```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countDigits(const char *s)
{
    int count = 0;

    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] >= '0' && s[i] <= '9')
            count++;
    }

    return count;
}

int compare(const void *a, const void *b)
{
    const char *s1 = *(char *const *)a;
    const char *s2 = *(char *const *)b;

    int A = countDigits(s1);
    int B = countDigits(s2);

    // First: digit count ascending
    if (A != B)
        return (A > B) - (A < B);

    // Second: string length ascending
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    if (len1 != len2)
        return (len1 > len2) - (len1 < len2);

    // Third: alphabetical
    return strcmp(s1, s2);
}

int main(int argc, char *argv[])
{
    qsort(&argv[1],
          argc - 1,
          sizeof(char *),
          compare);

    for (int i = 1; i < argc; i++)
        printf("%s\n", argv[i]);

    return 0;
}
```
## FRQ 2
- Given an array of students, sort them by score from highest to lowest. If two students have equal scores, sort them by ascending ID.
- framework
```
1. Define struct.
2. Comparator receives pointers to struct elements.
3. Extract score for both.
4. Compare scores.
5. If tied, compare IDs.
6. Call qsort().
```
- solution
```C
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int score;
} Student;

int compare(const void *a, const void *b)
{
    const Student *s1 = a;
    const Student *s2 = b;

    // Score descending
    if (s1->score != s2->score)
        return (s2->score > s1->score)
             - (s2->score < s1->score);

    // ID ascending
    return (s1->id > s2->id)
         - (s1->id < s2->id);
}

int main(void)
{
    Student students[] = {
        {3, 85},
        {1, 95},
        {4, 85},
        {2, 90}
    };

    int n = sizeof(students) /
            sizeof(students[0]);

    qsort(students, n,
          sizeof(Student), compare);

    for (int i = 0; i < n; i++) {
        printf("%d %d\n",
               students[i].id,
               students[i].score);
    }

    return 0;
}
```
- adaptation guide
  - use relational comparisons when theres larger integer values
```
Sorting a struct array:
    const StructType *x = a;
    const StructType *y = b;

Sorting a string pointer array:
    char *x = *(char **)a;
    char *y = *(char **)b;

Ascending:
    A before B when A < B

Descending:
    A before B when A > B
```
## FRQ 3
- Robots are represented by signed integers. Positive robots move right; negative robots move left. When robots collide, the weaker robot is destroyed. Equal-strength robots destroy each other. Return the surviving robots in original left-to-right order.
- framework
```
1. Make a linked-list stack.
2. For every robot:
      positive → push

      negative → fight stack top

3. Pop smaller positive robots repeatedly.
4. Equal strength → destroy both.
5. Stack empty or negative top → push current.
6. Otherwise current robot dies.
```
- solution
```C
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

void push(Node **head, int value)
{
    Node *p = malloc(sizeof(Node));

    if (p == NULL)
        exit(1);

    p->value = value;
    p->next = *head;
    *head = p;
}

void pop(Node **head)
{
    if (*head == NULL)
        return;

    Node *temp = *head;
    *head = (*head)->next;

    free(temp);
}

Node *fight(int arr[], int n)
{
    Node *stack = NULL;

    for (int i = 0; i < n; i++) {

        int current = arr[i];
        int alive = 1;

        while (alive &&
               stack != NULL &&
               stack->value > 0 &&
               current < 0)
        {
            int topMass = abs(stack->value);
            int curMass = abs(current);

            if (topMass < curMass) {
                pop(&stack);
            }
            else if (topMass == curMass) {
                pop(&stack);
                alive = 0;
            }
            else {
                alive = 0;
            }
        }

        if (alive)
            push(&stack, current);
    }

    return stack;
}

// Print bottom-to-top to restore original order
void printReverse(Node *head)
{
    if (head == NULL)
        return;

    printReverse(head->next);
    printf("%d ", head->value);
}

void freeList(Node **head)
{
    while (*head != NULL)
        pop(head);
}

int main(void)
{
    int arr[] = {4, 8, -3, -10, 6};
    int n = sizeof(arr) / sizeof(arr[0]);

    Node *result = fight(arr, n);

    printReverse(result);
    printf("\n");

    freeList(&result);

    return 0;
}
```
- adaptation guide
```
Stack problem:
    push(item)
    while interaction possible:
        compare current with top
        pop if necessary
    push current if it survives

Change:
    collision condition
    strengths/values comparison
    survival rule
```
## FRQ 4
- Write a function that removes every node containing a multiple of 3, including if the head must be deleted.
- framework
```
1. Start at the head pointer.
2. Check current node.
3. If it matches:
      save node
      connect previous link to next
      free node
4. Otherwise move to next link.
5. Continue until NULL.
```
- solution
```C
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

void push(Node **head, int value)
{
    Node *p = malloc(sizeof(Node));

    if (p == NULL)
        exit(1);

    p->value = value;
    p->next = *head;
    *head = p;
}

void removeMultiples(Node **head)
{
    Node **current = head;

    while (*current != NULL) {

        if ((*current)->value % 3 == 0) {

            Node *temp = *current;

            *current = temp->next;

            free(temp);
        }
        else {
            current = &(*current)->next;
        }
    }
}

void printList(Node *head)
{
    while (head != NULL) {
        printf("%d ", head->value);
        head = head->next;
    }
}

void freeList(Node *head)
{
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void)
{
    Node *head = NULL;

    int values[] = {3, 4, 6, 7, 9};
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = n - 1; i >= 0; i--)
        push(&head, values[i]);

    removeMultiples(&head);

    printList(head);
    printf("\n");

    freeList(head);

    return 0;
}
```
- adaptation guide
  - Node curent** technique handles deleting first, middle, or last node without a specific head case
```C
// change
if ((*current)->value % 3 == 0)

// to for
// Remove even nodes
if ((*current)->value % 2 == 0)

// Remove negative nodes
if ((*current)->value < 0)

// Remove a specific value
if ((*current)->value == target)
```
## FRQ 5
- Write a recursive function that selects exactly count distinct positive odd integers, no larger than bound, whose sum is target. Print one solution in ascending order.
- framework
```
1. Success: count == 0 AND target == 0.
2. Failure: impossible state.
3. Try including largest odd number.
4. If successful, print after recursion.
5. Otherwise skip largest number.
6. Move bound down by 2.
```
- solution
```C
#include <stdio.h>

int solve(int count, int bound, int target)
{
    // SUCCESS
    if (count == 0)
        return target == 0;

    // FAILURE
    if (count < 0 ||
        bound <= 0 ||
        target <= 0)
        return 0;

    // Make bound odd
    if (bound % 2 == 0)
        bound--;

    // CHOICE 1: TAKE bound
    if (solve(count - 1,
              bound - 2,
              target - bound))
    {
        printf("%d ", bound);
        return 1;
    }

    // CHOICE 2: SKIP bound
    return solve(count,
                 bound - 2,
                 target);
}

int main(void)
{
    int count = 3;
    int bound = 11;
    int target = 21;

    int found = solve(count, bound, target);

    if (!found)
        printf("No solution");

    printf("\n");

    return 0;
}
```
### FRQ 6
- Given an array of positive integers, determine whether exactly k distinct array positions can be selected to sum to a target. Print one solution.
- framework
```
At each index:
    TAKE arr[index]
    OR
    SKIP arr[index]

TAKE:
    index + 1
    k - 1
    target - arr[index]

SKIP:
    index + 1
    k unchanged
    target unchanged
```
- solution
```C
#include <stdio.h>

int solve(int arr[], int n,
          int index, int k,
          int target,
          int chosen[], int depth)
{
    if (k == 0) {
        if (target == 0) {

            for (int i = 0; i < depth; i++)
                printf("%d ", chosen[i]);

            return 1;
        }

        return 0;
    }

    if (index == n ||
        k < 0 ||
        target < 0 ||
        n - index < k)
        return 0;

    // TAKE current number
    chosen[depth] = arr[index];

    if (solve(arr, n,
              index + 1,
              k - 1,
              target - arr[index],
              chosen,
              depth + 1))
        return 1;

    // SKIP current number
    return solve(arr, n,
                 index + 1,
                 k,
                 target,
                 chosen,
                 depth);
}

int main(void)
{
    int arr[] = {2, 4, 5, 7, 9};
    int n = sizeof(arr) / sizeof(arr[0]);

    int chosen[5];

    if (!solve(arr, n, 0, 3, 16,
               chosen, 0))
    {
        printf("No solution");
    }

    printf("\n");

    return 0;
}
```
- adaptation guide
```
Any combination-selection problem:

if SUCCESS:
    return 1

if IMPOSSIBLE:
    return 0

TRY TAKE
if successful:
    return 1

TRY SKIP
return its result
```
## FRQ 7
- Write a program that prints every happy number between 1 and limit. A happy number eventually becomes 1 when repeatedly replaced by the sum of the squares of its digits.
- framework
```
1. Extract each digit with n % 10.
2. Square digit and add to sum.
3. Remove digit with n /= 10.
4. Repeat until 1 or 4.
5. Check all numbers in range.
```
- solution
```C
#include <stdio.h>

int digitSquareSum(int n)
{
    int sum = 0;

    while (n > 0) {

        int digit = n % 10;

        sum += digit * digit;

        n /= 10;
    }

    return sum;
}

int isHappy(int n)
{
    if (n <= 0)
        return 0;

    while (n != 1 && n != 4) {
        n = digitSquareSum(n);
    }

    return n == 1;
}

int main(void)
{
    int limit = 50;

    for (int i = 1; i <= limit; i++) {

        if (isHappy(i))
            printf("%d ", i);
    }

    printf("\n");

    return 0;
}
```
- adaptation guide
```C
// Sum of digits
sum += digit;

// Sum of cubes
sum += digit * digit * digit;

// Count even digits
if (digit % 2 == 0)
    count++;

// Find maximum digit
if (digit > max)
    max = digit;
```
## FRQ 8
- Simulate players moving around a circular property board. Each player rolls two dice. Passing the start gives money. Players claim unowned properties and pay rent to other players.
- framework
```
1. Initialize players and properties.
2. For each turn:
      roll dice
      update position with modulo
      award money for passing start
      inspect new property

3. If unowned: claim it.
4. If own: nothing.
5. If another player owns: pay rent.
6. Repeat until end condition.
```
- solution
```C
#include <stdio.h>
#include <stdlib.h>

#define PLAYERS 3
#define TRACK 10

typedef struct {
    int id;
    int loc;
    int balance;
} Player;

typedef struct {
    int owner;
    int rent;
} Property;

int main(void)
{
    srand(42);

    Player p[PLAYERS];
    Property prop[TRACK];

    // Initialize players
    for (int i = 0; i < PLAYERS; i++) {
        p[i].id = i;
        p[i].loc = 0;
        p[i].balance = TRACK;
    }

    // Initialize properties
    for (int i = 0; i < TRACK; i++) {
        prop[i].owner = -1;
        prop[i].rent = i + 1;
    }

    int bankrupt = 0;

    for (int round = 0;
         round < 20 && !bankrupt;
         round++)
    {
        for (int i = 0; i < PLAYERS; i++) {

            int dice =
                (rand() % 6 + 1)
                + (rand() % 6 + 1);

            int raw = p[i].loc + dice;

            // Award money for completed laps
            p[i].balance +=
                (raw / TRACK) * TRACK;

            p[i].loc = raw % TRACK;

            int pos = p[i].loc;
            int owner = prop[pos].owner;

            if (owner == -1) {

                // Claim property
                prop[pos].owner = p[i].id;
            }

            else if (owner == p[i].id) {

                // Own property: do nothing
            }

            else {

                int rent = prop[pos].rent;

                if (p[i].balance < rent) {
                    bankrupt = 1;
                    break;
                }

                p[i].balance -= rent;
                p[owner].balance += rent;
            }
        }
    }

    for (int i = 0; i < PLAYERS; i++) {
        printf("Player %d: %d\n",
               p[i].id, p[i].balance);
    }

    return 0;
}
```
- adaptation guide
```C
// Circular movement
position = (position + steps) % n;

// Check if passing start
if (oldPosition + steps >= n)

// Dice 1 through 6
rand() % 6 + 1

// Access struct field
player.balance

// Access through pointer
playerPtr->balance

// Three ownership cases
if (unowned)
else if (mine)
else
```
## FRQ 9
- Maintain a hash table storing occupied (x,y) coordinates. Implement insertion, searching, and freeing the entire table.
- framework
```
1. Convert coordinates to hash index.
2. Look at table[index].
3. Search linked list for matching key.
4. To insert:
      allocate node
      connect to old bucket head
      update bucket head
5. Free every bucket when finished.
```
- solution
```C
#include <stdio.h>
#include <stdlib.h>

#define N 101

typedef struct Node {
    int x, y;
    struct Node *next;
} Node;

Node *table[N] = {NULL};

unsigned int hash(int x, int y)
{
    unsigned int a =
        (unsigned int)x * 73856093u;

    unsigned int b =
        (unsigned int)y * 19349663u;

    return (a ^ b) % N;
}

int exists(int x, int y)
{
    unsigned int index = hash(x, y);

    Node *p = table[index];

    while (p != NULL) {

        if (p->x == x && p->y == y)
            return 1;

        p = p->next;
    }

    return 0;
}

void insert(int x, int y)
{
    if (exists(x, y))
        return;

    unsigned int index = hash(x, y);

    Node *p = malloc(sizeof(Node));

    if (p == NULL)
        exit(1);

    p->x = x;
    p->y = y;

    // Insert at bucket head
    p->next = table[index];
    table[index] = p;
}

void freeTable(void)
{
    for (int i = 0; i < N; i++) {

        Node *p = table[i];

        while (p != NULL) {

            Node *temp = p;
            p = p->next;

            free(temp);
        }

        table[i] = NULL;
    }
}

int main(void)
{
    insert(2, 3);
    insert(-1, 5);
    insert(7, 8);

    printf("%d\n", exists(2, 3)); // 1
    printf("%d\n", exists(4, 4)); // 0

    freeTable();

    return 0;
}
```
- adaptation guide
```C
// Hash index
index = hash(key) % N;

// Linked-list insertion
newNode->next = table[index];
table[index] = newNode;

// Lookup
Node *p = table[index];

while (p != NULL) {
    if (MATCH)
        return 1;

    p = p->next;
}

// Free one linked list
while (p != NULL) {
    temp = p;
    p = p->next;
    free(temp);
}
```
## FRQ 10
- Read integers from an input file. For every even integer, write its square into a second file.
- framework
```
1. Open input file with "r".
2. Check input != NULL.
3. Open output file with "w".
4. Repeatedly fscanf() integers.
5. Check condition.
6. fprintf() result.
7. Close both files.
```
- solution
```C
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: program input output\n");
        return 1;
    }

    FILE *in = fopen(argv[1], "r");

    if (in == NULL) {
        perror("Input");
        return 1;
    }

    FILE *out = fopen(argv[2], "w");

    if (out == NULL) {
        perror("Output");
        fclose(in);
        return 1;
    }

    int x;

    while (fscanf(in, "%d", &x) == 1) {

        if (x % 2 == 0)
            fprintf(out, "%d\n", x * x);
    }

    fclose(in);
    fclose(out);

    return 0;
}
```
- adaptation guide
```C
// Open for reading
FILE *fp = fopen(name, "r");

// Open for writing (overwrites)
FILE *fp = fopen(name, "w");

// Open for appending
FILE *fp = fopen(name, "a");

// Read integer
fscanf(fp, "%d", &x);

// Read double
fscanf(fp, "%lf", &x);

// Write integer
fprintf(fp, "%d\n", x);

// Read line
fgets(buffer, sizeof(buffer), fp);

// Close
fclose(fp);
```
## FRQ 11
- Write a program that counts how many lines are in a text file and determines the maximum line length, excluding newline characters.
- framework
```
1. Open file.
2. Read one character at a time.
3. If newline:
      update maximum
      increment line count
      reset current length
4. Otherwise increment length.
5. Handle final line without newline.
6. Close file.
```
- solution
```C
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    FILE *fp = fopen(argv[1], "r");

    if (fp == NULL) {
        perror("File");
        return 1;
    }

    int c;

    int lines = 0;
    int current = 0;
    int longest = 0;

    while ((c = fgetc(fp)) != EOF) {

        if (c == '\n') {

            lines++;

            if (current > longest)
                longest = current;

            current = 0;
        }
        else {
            current++;
        }
    }

    // Final line may not end in '\n'
    if (current > 0) {

        lines++;

        if (current > longest)
            longest = current;
    }

    printf("Lines: %d\n", lines);
    printf("Longest: %d\n", longest);

    fclose(fp);

    return 0;
}
```
- adaptation guide
```C
// Read one character
int c = fgetc(fp);

// End-of-file loop
while ((c = fgetc(fp)) != EOF)

// Count a specific character
if (c == TARGET)
    count++;

// Count lines
if (c == '\n')
    lines++;

// Write a character
fputc(c, outputFile);
```
## FRQ 12
- Read integers until -1 is entered. Store them in a dynamically allocated array that doubles its capacity whenever it becomes full. Print all numbers and their average.
- framework
```
1. malloc initial capacity.
2. Read value.
3. If array full:
      double capacity
      realloc safely
4. Store value.
5. Stop at sentinel.
6. Calculate result.
7. Free array.
```
- solution
```C
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int capacity = 4;
    int n = 0;

    int *arr =
        malloc(capacity * sizeof(int));

    if (arr == NULL)
        return 1;

    int x;

    while (scanf("%d", &x) == 1 && x != -1) {

        if (n == capacity) {

            int newCapacity = capacity * 2;

            int *temp =
                realloc(arr,
                        newCapacity * sizeof(int));

            if (temp == NULL) {
                free(arr);
                return 1;
            }

            arr = temp;
            capacity = newCapacity;
        }

        arr[n] = x;
        n++;
    }

    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
        sum += arr[i];
    }

    if (n > 0)
        printf("\nAverage: %.2f\n", sum / n);

    free(arr);

    return 0;
}
```
- adaptation guide
```C
// Create array
int *a = malloc(capacity * sizeof(int));

// Expand array
int *temp =
    realloc(a, newCapacity * sizeof(int));

if (temp != NULL)
    a = temp;

// Add item
a[n] = value;
n++;

// Release array
free(a);
```
## FRQ 13
- Write a function that accepts an integer array and another function. Apply that function to every array element.
- framework
```
1. Write one or more transformation functions.
2. Define a function-pointer parameter.
3. Loop through array.
4. Call callback on each element.
5. Store returned result.
```
- solution
```C
#include <stdio.h>

int square(int x)
{
    return x * x;
}

int addThree(int x)
{
    return x + 3;
}

void apply(int arr[], int n,
           int (*func)(int))
{
    for (int i = 0; i < n; i++) {
        arr[i] = func(arr[i]);
    }
}

int main(void)
{
    int arr[] = {1, 2, 3, 4};
    int n = 4;

    // Square each element
    apply(arr, n, square);

    // Add 3 to each result
    apply(arr, n, addThree);

    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);

    printf("\n");

    return 0;
}
```
- adaptation guide
```C
// Function pointer accepting int,
// returning int:
int (*func)(int);

// Function pointer accepting two ints:
int (*func)(int, int);

// Invoke function pointer:
result = func(value);

// Pass an actual function:
apply(arr, n, square);
```