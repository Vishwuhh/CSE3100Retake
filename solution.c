#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct People {
    char people;
    struct People *next;
} People; 

typedef struct Contact {
    char name;
    struct Contact *next;
} Contact;

void push(People **head, char value)
{
    People *newNode =
        malloc(sizeof(char));

    newNode->people = value;

    newNode->next = *head;

    *head = newNode;
}

int main(int argc, char*argv[]) {
    if(argc != 3) {
         printf("Usage: program input output\n");
        return 1;
    }

    FILE *in = fopen(argv[1], "r");

    if(in == NULL) {
        perror("Input");
        return 1;
    }
    FILE *out = fopen(argv[2], "w");

    if(out == NULL) {
        perror("Output");
        fclose(in);
        return 1;
    }
}