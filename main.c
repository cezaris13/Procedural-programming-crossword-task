#include <string.h>
#include <time.h>

#define EMPTY 'O'
#define MAX_SIZE 512
#define WALL 'X'
#define EMPTY 'O'
#define FAILED_MAP '@'

#include "file_operations.h"
#include "crossword.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("No string provided!\n");
        return 0;
    }

    char **wordList;
    int wordCount = 0;
    wordList = malloc(MAX_SIZE * sizeof(unsigned char *));
    readData(argv[1], &wordCount, &wordList);

    CrossWord crossWord;
    int linesCount = 0;
    crossWord.map = malloc(MAX_SIZE * sizeof(unsigned char *));
    readData(argv[2], &linesCount, &(crossWord.map));
    crossWord.xSize = linesCount;
    crossWord.ySize = strlen(crossWord.map[0]);

    for (int i = 0; i < wordCount - 1; i++)
    { // sort word in descending order by their size
        for (int j = i + 1; j < wordCount; j++)
        {
            if (strlen(wordList[i]) < strlen(wordList[j]))
            {
                char *temp = wordList[i];
                wordList[i] = wordList[j];
                wordList[j] = temp;
            }
        }
    }

    Word *allWords = malloc(MAX_SIZE * sizeof(Word));
    int amount = 0;
    findAllPossiblePlacements(crossWord, &allWords, &amount);
    for (int i = 0; i < amount - 1; i++)
    { // sort word in descending order by their size
        for (int j = i + 1; j < amount; j++)
        {
            if (allWords[i].length < allWords[j].length)
            {
                Word temp = allWords[i];
                allWords[i] = allWords[j];
                allWords[j] = temp;
            }
        }
    }
    clock_t t = clock();

    CrossWord finalCrossWord;
    solveCrossword(wordList, wordCount, crossWord, &finalCrossWord, 0, amount, allWords);

    t = clock() - t;
    printArray(finalCrossWord.map, linesCount);

    printf("%fsek.\n", ((double)t) / CLOCKS_PER_SEC);
    return 0;
}