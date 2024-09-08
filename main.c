#include <string.h>
#include <time.h>

#define EMPTY 'O'
#define WALL 'X'
#define MAX_SIZE 512
#define FAILED_MAP '@'
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#include "file_operations.h"
#include "crossword.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("No string provided!\n");
        return 0;
    }

    Words words;
    words.words = malloc(MAX_SIZE * sizeof(unsigned char *));
    words.size = 0;
    readData(argv[1], &(words.size), &(words.words));

    CrossWord crossWord;
    int linesCount = 0;
    crossWord.map = malloc(MAX_SIZE * sizeof(unsigned char *));
    readData(argv[2], &linesCount, &(crossWord.map));
    crossWord.xSize = linesCount;
    crossWord.ySize = strlen(crossWord.map[0]);

    // calculate each word length occurence
    int *wordOccurence = malloc(MAX(crossWord.xSize, crossWord.ySize) * sizeof(int));
    for (int i = 0; i < words.size; i++)
        wordOccurence[strlen(words.words[i])]++;
    words.lengthOccurence = wordOccurence;

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
        }
    }

    WordPositions wordPositions;
    wordPositions.positions = malloc(MAX_SIZE * sizeof(WordPosition));
    wordPositions.length = 0;
    findAllPossibleWordPositions(&crossWord, &wordPositions);

    int *wordPositionOccurence = malloc(MAX(crossWord.xSize, crossWord.ySize) * sizeof(int));
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
    clock_t time = clock();

    CrossWord finalCrossWord;
    solveCrossword(crossWord, &finalCrossWord, words, wordPositions, 0);

    time = clock() - time;
    printArray(finalCrossWord.map, linesCount);

    printf("%fsek.\n", ((double)time) / CLOCKS_PER_SEC);
    return 0;
}