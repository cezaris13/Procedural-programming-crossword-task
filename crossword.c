struct CrossWord
{
    char **map;
    int xSize, ySize;
};
typedef struct CrossWord CrossWord;

struct Words
{
    char **words;
    int size;
    int *lengthOccurence;
};
typedef struct Words Words;

struct WordPosition
{
    int x, y, length, isVertical;
};
typedef struct WordPosition WordPosition;

struct WordPositions
{
    WordPosition *positions;
    int length;
    int *lengthOccurence;
};
typedef struct WordPositions WordPositions;

// Iteratate through the crossword map and search if there's EMPTY symbol
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

// Given word orientation and starting position, check if word can fit in the empty space.
// If while checking, the letter in the map is not empty or the letter does not match with the i-th letter of the word, then return FAILED_MAP
// (Denoted by @ sign in map[0][0] position)
CrossWord checkWord(CrossWord crossWord, WordPosition word, char *wrd)
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

void solveCrossword(CrossWord crossWord, CrossWord *finalCrossWord, Words words, WordPositions wordPositions, int index)
{
    // if the crossword does not have EMPTY symbols, the solution was found, return
    if (!isEmpty(crossWord))
    {
        *finalCrossWord = copyCrossWord(crossWord);
        return;
    }

    // if index is out of range of list of words break
    if (index >= words.size)
        return;

    for (int i = 0; i < wordPositions.length; i++)
    {
        if (strlen(words.words[index]) == wordPositions.positions[i].length) // chia issue(I think)
        {
            CrossWord temp = checkWord(crossWord, wordPositions.positions[i], words.words[index]);
            // If the map is not failed, the word worked, moving on to the next one.
            if (temp.map[0][0] != FAILED_MAP)
                solveCrossword(temp, finalCrossWord, words, wordPositions, index + 1);
        }
    }

    if (index < words.size)
        solveCrossword(crossWord, finalCrossWord, words, wordPositions, index + 1);
}

// Finds the word placement, for coordinates x y, with length and either vertical or horizontal
void processWord(CrossWord *crossWord, WordPositions *wordPositions, int i, int j, int isVertical)
{
    int wordLength = 0;
    WordPosition wordPosition;
    wordPosition.x = i;
    wordPosition.y = j;
    wordPosition.isVertical = isVertical;

    if (isVertical)
    {
        // if the it is the corner or there's wall above the empty, go down while the you reach the wall or the end of map
        if (i == 0 || crossWord->map[i - 1][j] != EMPTY)
            for (; wordLength + i < crossWord->xSize; wordLength++)
                if (crossWord->map[i + wordLength][j] == WALL)
                    break;
    }
    else
    {
        // if the it is the corner or there's wall on the left of the empty, go right while the you reach the wall or the end of map
        if (j == 0 || crossWord->map[i][j - 1] != EMPTY)
            for (; wordLength + j < crossWord->ySize; wordLength++)
                if (crossWord->map[i][j + wordLength] == WALL)
                    break;
    }

    if (wordLength > 1)
    {
        wordPosition.length = wordLength;
        wordPositions->positions[wordPositions->length] = wordPosition;
        wordPositions->length++;
    }
}

// Finds all x y coordinates with length and orientation, where word could fit.
void findAllPossiblePlacements(CrossWord *crossWord, WordPositions *wordPositions)
{
    for (int i = 0; i < crossWord->xSize; i++)
    {
        for (int j = 0; j < crossWord->ySize; j++)
        {
            if (crossWord->map[i][j] == WALL)
                continue;

            // Process vertical words
            processWord(crossWord, wordPositions, i, j, 1);
            // Process horizontal words
            processWord(crossWord, wordPositions, i, j, 0);
        }
    }
}