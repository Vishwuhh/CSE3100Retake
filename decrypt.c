#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#define MAX 10240
#define MAX_WORD_COUNT 60000
#define MAX_WORD_LENGTH 80

char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];
int word_count = 0;

void read_file_to_array(char *filename)
{
    FILE *fp;

    fp = fopen(filename, "r");
    if(fp==NULL)
    {
        printf("Cannot open file %s.\n", filename);
        exit(-1);
    }

    while (word_count < MAX_WORD_COUNT &&
           fscanf(fp, "%79s", words[word_count]) == 1)
    {
        word_count++;
    }
    fclose(fp);
}

int in_dict(char *word)
{

    int low = 0;
    int high = word_count - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(word, words[mid]);

        if (cmp == 0)
            return 1;
        if (cmp < 0)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return 0;
}

void decryption(unsigned char key, unsigned char shift, const int *encrypted, int len, char *decrypted)
{
    for (int i = 0; i < len; i++)
    {

        decrypted[i] = (char)(((unsigned int)encrypted[i] ^ key) >> shift);
    }
    decrypted[len] = '\0';
}

int message_score(const char *msg)
{
    int score = 0;
    char word[MAX_WORD_LENGTH];
    int length = 0;
    int too_long = 0;

    for (int i = 0; ; i++)
    {
        unsigned char c = (unsigned char)msg[i];

        if (c != '\0' && c < 128 && !isprint(c) && !isspace(c))
            return 0;

        if (isalpha(c))
        {
            if (length < MAX_WORD_LENGTH - 1)
                word[length++] = (char)tolower(c);
            else
                too_long = 1;
        }
        else
        {
            if (length > 0 && !too_long)
            {
                word[length] = '\0';
                if (in_dict(word))
                    score++;
            }
            length = 0;
            too_long = 0;

            if (c == '\0')
                break;
        }
    }
    return score;
}

void search(const int *encrypted, int len, char *message)
{
	char decrypted[MAX];

    int max_score = 0;
    strcpy(message, "");
    for(int k = 0; k <= 255; k++)
    {
        for(unsigned char shift = 0; shift <= 24; shift++)
        {
            decryption(k, shift, encrypted, len, decrypted);
			int score = message_score(decrypted);
			if(score > max_score)
			{
				max_score = score;
				strcpy(message, decrypted);
			}
        }
    }
}

int read_encrypted(char *filename, int *encrypted)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    int total_bytes = 0;
    int capacity = (MAX - 1) * (int)sizeof(int);

    while (total_bytes < capacity)
    {
        ssize_t n = read(fd, (char *)encrypted + total_bytes,
                         capacity - total_bytes);
        if (n < 0)
        {
            perror("read");
            close(fd);
            exit(EXIT_FAILURE);
        }
        if (n == 0)
            break;
        total_bytes += (int)n;
    }
    close(fd);

    if (total_bytes % (int)sizeof(int) != 0)
    {
        fprintf(stderr, "Invalid encrypted file: incomplete integer.\n");
        exit(EXIT_FAILURE);
    }

    return total_bytes / (int)sizeof(int);
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("%s encrypted-message\n", argv[0]);
		return 0;
	}

	read_file_to_array("dict.txt");

	int encrypted[MAX];
	int len = read_encrypted(argv[1], encrypted);

	char message[MAX];
	strcpy(message, "");
	search(encrypted, len, message);
	printf("%s\n", message);
	return 0;
}
