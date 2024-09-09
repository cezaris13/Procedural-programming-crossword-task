#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int debug = 0;
#define EMPTY 'O'
#define WALL 'X'
#define MAX_SIZE 512
#define FAILED_MAP '@'
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#include "structs.c"
#include "file_operations.h"
#include "crossword.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("No string provided!\n");
        return 0;
    }

    if (argc == 4)
        debug = strcmp(argv[3], "DEBUG") == 0;

    Words words;
    words.words = malloc(MAX_SIZE * sizeof(unsigned char *));
    words.size = 0;
    readData(argv[1], &(words.size), &(words.words));

    Crossword crossword;
    int linesCount = 0;
    crossword.map = malloc(MAX_SIZE * sizeof(unsigned char *));
    readData(argv[2], &linesCount, &(crossword.map));
    crossword.xSize = linesCount;
    crossword.ySize = strlen(crossword.map[0]);

    if (debug)
        printf("Get each word length occurence\n");
    // calculate each word length occurence
    int *wordOccurence = malloc(MAX(crossword.xSize, crossword.ySize) * sizeof(int));
    for (int i = 0; i < words.size; i++)
        wordOccurence[strlen(words.words[i])]++;
    words.lengthOccurence = wordOccurence;
    if (debug)
        printf("Sorting words by their occurence\n");
    // Sort words in ascending order by their length occurence
    for (int i = 0; i < words.size - 1; i++)
    {
        for (int j = i + 1; j < words.size; j++)
        {
            if (words.lengthOccurence[strlen(words.words[i])] > words.lengthOccurence[strlen(words.words[j])])
            {
                char *temp = words.words[i];
                words.words[i] = words.words[j];
                words.words[j] = temp;
            }
            else if (words.lengthOccurence[strlen(words.words[i])] == words.lengthOccurence[strlen(words.words[j])])
            {
                if (strlen(words.words[i]) < strlen(words.words[j]))
                {
                    char *temp = words.words[i];
                    words.words[i] = words.words[j];
                    words.words[j] = temp;
                }
            }
        }
    }
    if (debug)
        printf("Get each word possible positions\n");

    WordPositions wordPositions;
    wordPositions.positions = malloc(MAX_SIZE * sizeof(WordPosition));
    wordPositions.length = 0;
    findAllPossibleWordPositions(&crossword, &wordPositions);
    if (debug)
        printf("Sorting words positions by their occurence\n");

    int *wordPositionOccurence = malloc(MAX(crossword.xSize, crossword.ySize) * sizeof(int));
    for (int i = 0; i < wordPositions.length; i++)
        wordPositionOccurence[wordPositions.positions[i].length]++;
    wordPositions.lengthOccurence = wordPositionOccurence;

    // Sort wordPositions in ascending order by their length occurence
    for (int i = 0; i < wordPositions.length - 1; i++)
    {
        for (int j = i + 1; j < wordPositions.length; j++)
        {
            if (wordPositions.lengthOccurence[wordPositions.positions[i].length] > wordPositions.lengthOccurence[wordPositions.positions[j].length])
            {
                WordPosition temp = wordPositions.positions[i];
                wordPositions.positions[i] = wordPositions.positions[j];
                wordPositions.positions[j] = temp;
            }
        }
    }
    if (debug)
        printf("Words and word positions have been prepared, trying to solve the crossword\n");
    clock_t time = clock();

    Crossword finalCrossWord;
    solveCrossword(crossword, &finalCrossWord, words, wordPositions, 0);

    time = clock() - time;
    printArray(finalCrossWord);

    printf("%fsek.\n", ((double)time) / CLOCKS_PER_SEC);
    return 0;
}