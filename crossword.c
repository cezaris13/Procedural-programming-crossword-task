struct CrossWord
{
    char **map;
    int xSize, ySize;
};
typedef struct CrossWord CrossWord;

struct Word
{
    int x, y, length, isVertical;
    // char *word;
};
typedef struct Word Word;

// iteratate through the crossword map and search if there's EMPTY symbol
int isEmpty(CrossWord crossWord)
{
    for (int i = 0; i < crossWord.xSize; i++)
        for (int j = 0; j < crossWord.ySize; j++)
            if (crossWord.map[i][j] == EMPTY)
                return 1;
    return 0;
}

CrossWord copyCrossWord(CrossWord crossWord)
{
    CrossWord crossWordCopy;
    crossWordCopy.xSize = crossWord.xSize;
    crossWordCopy.ySize = crossWord.ySize;
    crossWordCopy.map = malloc((crossWord.xSize + 1) * sizeof(unsigned char *));
    for (int i = 0; i < crossWord.xSize; i++)
    {
        *(crossWordCopy.map + i) = malloc((crossWord.ySize + 1) * sizeof(char));
        for (int j = 0; j < crossWord.ySize; j++)
            crossWordCopy.map[i][j] = crossWord.map[i][j];
        crossWordCopy.map[i][crossWord.ySize] = '\0';
    }

    return crossWordCopy;
}

CrossWord checkWord(CrossWord crossWord, Word word, char *wrd)
{
    CrossWord tempCrossWord = copyCrossWord(crossWord);
    for (int i = 0; i < word.length; i++)
    {
        int x = word.x + word.isVertical * i;
        int y = word.y + !word.isVertical * i;
        if (tempCrossWord.map[x][y] == EMPTY || tempCrossWord.map[x][y] == wrd[i])
        {
            tempCrossWord.map[x][y] = wrd[i];
        }
        else
        {
            tempCrossWord.map[0][0] = FAILED_MAP;
            break;
        }
    }
    return tempCrossWord;
}

void solveCrossword(char **words, int wordsSize, CrossWord crossWord, CrossWord *finalCrossWord, int index, int amount, Word *list)
{
    if (!isEmpty(crossWord))
    {
        *finalCrossWord = copyCrossWord(crossWord);
        return;
    }

    if (index >= wordsSize)
        return;

    for (int i = 0; i < amount; i++)
    {
        if (strlen(words[index]) == list[i].length)
        {
            CrossWord temp = checkWord( crossWord, list[i], words[index]);
            if (temp.map[0][0] != FAILED_MAP)
                solveCrossword(words, wordsSize, temp, finalCrossWord, index + 1, amount, list);
        }
    }

    if (index < wordsSize)
        solveCrossword(words, wordsSize, crossWord, finalCrossWord, index + 1, amount, list);
}

void findAllPossiblePlacements(CrossWord crossWord, Word **words, int *amount)
{
    for (int i = 0; i < crossWord.xSize; i++)
    {
        for (int j = 0; j < crossWord.ySize; j++)
        {
            int k = 0;
            Word temp, temp1;
            temp.x = i;
            temp.y = j;
            temp.isVertical = 1;
            if (i == 0 || crossWord.map[i - 1][j] != EMPTY)
            {
                while (k + i < crossWord.xSize)
                {
                    if (WALL == crossWord.map[i + k][j])
                        break;
                    k++;
                }
            }
            if (k > 1)
            {
                temp.length = k;
                *(*words + *amount) = temp;
                (*amount)++;
            }
            k = 0;
            temp1.x = i;
            temp1.y = j;
            temp1.isVertical = 0;
            if (j == 0 || crossWord.map[i][j - 1] != EMPTY)
            {
                while (k + j < crossWord.ySize)
                {
                    if (WALL == crossWord.map[i][j + k])
                        break;
                    k++;
                }
            }
            if (k > 1)
            {
                temp1.length = k;
                *(*words + *amount) = temp1;
                (*amount)++;
            }
        }
    }
}