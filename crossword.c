// Iteratate through the crossword map and search if there's EMPTY symbol
int isEmpty(Crossword crossword)
{
    for (int i = 0; i < crossword.xSize; i++)
        for (int j = 0; j < crossword.ySize; j++)
            if (crossword.map[i][j] == EMPTY)
                return 1;
    return 0;
}

Crossword copyCrossWord(Crossword crossword)
{
    Crossword crossWordCopy;
    crossWordCopy.xSize = crossword.xSize;
    crossWordCopy.ySize = crossword.ySize;
    crossWordCopy.map = malloc((crossword.xSize + 1) * sizeof(unsigned char *));
    for (int i = 0; i < crossword.xSize; i++)
    {
        *(crossWordCopy.map + i) = malloc((crossword.ySize + 1) * sizeof(char));
        for (int j = 0; j < crossword.ySize; j++)
            crossWordCopy.map[i][j] = crossword.map[i][j];
        crossWordCopy.map[i][crossword.ySize] = '\0';
    }

    return crossWordCopy;
}

// Given word orientation and starting position, check if word can fit in the empty space.
// If while checking, the letter in the map is not empty or the letter does not match with the i-th letter of the word, then return FAILED_MAP
// (Denoted by @ sign in map[0][0] position)
Crossword checkWord(Crossword crossword, WordPosition word, char *wrd)
{
    // printArray(crossword);
    Crossword tempCrossWord = copyCrossWord(crossword);
    // printf("%s\n", wrd);
    // printf("vertical %d\n", word.isVertical);
    for (int i = 0; i < word.length; i++)
    {
        int x = word.x + word.isVertical * i;
        int y = word.y + !word.isVertical * i;

        // printf("(%d,%d), value: %c\n", x, y, tempCrossWord.map[x][y]);
        // printf("%d, %d\n", x < tempCrossWord.xSize, y < tempCrossWord.ySize);
        // printf("%d, %d\n", tempCrossWord.xSize, tempCrossWord.ySize);
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

int solveCrossword(Crossword crossword, Crossword *finalCrossWord, Words words, WordPositions wordPositions, int index)
{
    // if index is out of range of list of words break
    if (index >= words.size)
        return 0;

    // if the crossword does not have EMPTY symbols, the solution was found, return
    if (!isEmpty(crossword))
    {
        *finalCrossWord = copyCrossWord(crossword);
        return 1;
    }
    if (debug)
        printArray(crossword);

    for (int i = 0; i < wordPositions.length; i++)
    {
        if (strlen(words.words[index]) != wordPositions.positions[i].length)
            continue;

        Crossword temp = checkWord(crossword, wordPositions.positions[i], words.words[index]);
        // If the map is not failed, the word worked, moving on to the next one.
        if (temp.map[0][0] != FAILED_MAP)
            if (solveCrossword(temp, finalCrossWord, words, wordPositions, index + 1))
                return 1;
    }

    // reaching this means what word did not fit anywhere, returns unsuccess
    return 0;
}

// Finds the word placement, for coordinates x y, with length and either vertical or horizontal
void processWord(Crossword *crossword, WordPositions *wordPositions, int x, int y, int isVertical)
{
    int wordLength = 0;
    WordPosition wordPosition;
    wordPosition.x = x;
    wordPosition.y = y;
    wordPosition.isVertical = isVertical;

    if (isVertical)
    {
        // if the it is the corner or there's wall above the empty, go down while the you reach the wall or the end of map
        if (x == 0 || crossword->map[x - 1][y] != EMPTY)
            for (; wordLength + x < crossword->xSize; wordLength++)
                if (crossword->map[x + wordLength][y] == WALL)
                    break;
    }
    else
    {
        // if the it is the corner or there's wall on the left of the empty, go right while the you reach the wall or the end of map
        if (y == 0 || crossword->map[x][y - 1] != EMPTY)
            for (; wordLength + y < crossword->ySize; wordLength++)
                if (crossword->map[x][y + wordLength] == WALL)
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
void findAllPossibleWordPositions(Crossword *crossword, WordPositions *wordPositions)
{
    for (int i = 0; i < crossword->xSize - 1; i++)
    {
        for (int j = 0; j < crossword->ySize - 1; j++)
        {
            if (crossword->map[i][j] == WALL)
                continue;

            // Process vertical words
            processWord(crossword, wordPositions, i, j, 1);
            // Process horizontal words
            processWord(crossword, wordPositions, i, j, 0);
        }
    }
}