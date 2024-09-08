#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 512
#define EMPTY 'O'
#define WALL 'X'
#define FAILEDMAP '2'
// words file "words.txt"
// crossword matrix file "crossword.txt"
struct word
{
    int x, y;
    int length;
    char orientation;
};
typedef struct word word;
char **finalMap = NULL; // final matrix
int x_size, y_size;     // crossword dimensions
int checkEmpty(char **map);
char **checkWord(int x, int y, char **map, char *currWord, char orientation);
char **copyMap(char **map);
void printArray(char **arr, int size);
void readData(char *filename, int *line_count, char ***arr);
void solveCrossword(char **words, int words_size, char **map, int index, int amount, word *list);
void findAllPossiblePlacements(word **list, int *amount, char **map);
int main()
{
    word *allWords = malloc(MAX_SIZE * sizeof(word));
    int wordCount = 0, linesCount = 0;
    int amount = 0;
    char **wordList, **crossWord;
    wordList = malloc(MAX_SIZE * sizeof(unsigned char *));
    crossWord = malloc(MAX_SIZE * sizeof(unsigned char *));
    readData("/home/pijus/Desktop/Programming/kryziazodis/test4/words.txt", &wordCount, &wordList);
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
    readData("/home/pijus/Desktop/Programming/kryziazodis/test4/crossword.txt", &linesCount, &crossWord);
    x_size = linesCount;
    y_size = strlen(crossWord[0]);
    findAllPossiblePlacements(&allWords, &amount, crossWord);
    for (int i = 0; i < amount - 1; i++)
    { // sort word in descending order by their size
        for (int j = i + 1; j < amount; j++)
        {
            if (allWords[i].length < allWords[j].length)
            {
                word temp = allWords[i];
                allWords[i] = allWords[j];
                allWords[j] = temp;
            }
        }
    }
    //    for(int i=0;i<amount;i++){
    //        printf("x: %d y: %d length: %d orientation: %c\n",allWords[i].x,allWords[i].y,allWords[i].length,allWords[i].orientation);
    //    }
    solveCrossword(wordList, wordCount, crossWord, 0, amount, allWords);
    printArray(finalMap, linesCount);
    return 0;
}
int checkEmpty(char **map)
{
    for (int i = 0; i < x_size; i++)
    {
        for (int j = 0; j < y_size; j++)
        {
            if (map[i][j] == EMPTY)
            {
                return 1;
            }
        }
    }
    return 0;
}
char **checkWord(int x, int y, char **map, char *currWord, char orientation)
{
    int v = (orientation == 'v' ? 1 : 0); // vertical - v=1 h=0; horizontal v=0,h=1;
    int h = (orientation == 'v' ? 0 : 1);
    char **tempMap = copyMap(map);
    int n = strlen(currWord);
    for (int i = 0; i < n; i++)
    {
        if (tempMap[x + v * i][y + h * i] == EMPTY || tempMap[x + v * i][y + h * i] == currWord[i])
        { // checking if
            tempMap[x + v * i][y + h * i] = currWord[i];
        }
        else
        {
            tempMap[0][0] = FAILEDMAP;
            return tempMap;
        }
    }
    return tempMap;
}
char **copyMap(char **map)
{
    char **temp = malloc((x_size + 1) * sizeof(unsigned char *));
    for (int i = 0; i < x_size; i++)
    {
        *(temp + i) = malloc((y_size + 1) * sizeof(char));
        for (int j = 0; j < y_size; j++)
        {
            temp[i][j] = map[i][j];
        }
        temp[i][y_size] = '\0';
    }
    return temp;
}
void printArray(char **arr, int size)
{
    //    freopen("ans.txt","a",stdout);
    if (arr == NULL)
    {
        printf("neimanoma sugeneruoti sprendimo\n");
        return;
    }
    for (int i = 0; i < size; i++)
    {
        printf("%s\n", arr[i]);
    }
}
void readData(char *filename, int *line_count, char ***arr)
{
    char *line = NULL;
    size_t line_buf_size = 1;
    int line_size = 1;
    FILE *fd = fopen(filename, "r");
    if (fd == NULL)
    {
        printf("ivyko klaida atidarant faila: %s\n", filename);
        return;
    }
    while (line_size >= 0)
    {
        char *sth;
        line_size = getline(&line, &line_buf_size, fd);
        int i = 0;
        if (line_size > 0)
        {
            if (strlen(line) < MAX_SIZE)
            {
                sth = line;
                sth[strlen(line) - 1] = '\0';
                *(*arr + (*line_count)) = sth;
                (*line_count)++;
            }
            else
            {
                printf("zodis %s virsijo maximalu zodzio ilgi: %d\n", line, MAX_SIZE);
            }
        }
        line = NULL;
    }
    fclose(fd);
}
void solveCrossword(char **words, int words_size, char **map, int index, int amount, word *list)
{
    if (!checkEmpty(map))
    { // if matrix doesn't have any empty symbols- that means crossword is solved
        finalMap = copyMap(map);
        //        printArray(map,x_size);
        //        printf("\n");
        return;
    }
    if (index < words_size)
    {
        for (int i = 0; i < amount; i++)
        {
            if (strlen(words[index]) == list[i].length)
            {
                char **temp = checkWord(list[i].x, list[i].y, map, words[index], list[i].orientation);
                if (temp[0][0] != FAILEDMAP)
                {
                    solveCrossword(words, words_size, temp, index + 1, amount, list);
                }
            }
        }
    }
    else
    { // if all words have been used
        return;
    }
    if (index < words_size)
    {
        solveCrossword(words, words_size, map, index + 1, amount, list); // if element was not added to the matrix skip it
    }
}
void findAllPossiblePlacements(word **list, int *amount, char **map)
{
    for (int i = 0; i < x_size; i++)
    {
        for (int j = 0; j < y_size; j++)
        {
            int k = 0;
            word temp, temp1;
            temp.x = i;
            temp.y = j;
            temp.orientation = 'v';
            if (i == 0 || map[i - 1][j] != EMPTY)
            {
                while (k + i < x_size)
                {
                    if (WALL == map[i + k][j])
                    {
                        break;
                    }
                    k++;
                }
            }
            if (k > 1)
            {
                temp.length = k;
                *(*list + *amount) = temp;
                (*amount)++;
            }
            k = 0;
            temp1.x = i;
            temp1.y = j;
            temp1.orientation = 'h';
            if (j == 0 || map[i][j - 1] != EMPTY)
            {
                while (k + j < y_size)
                {
                    if (WALL == map[i][j + k])
                    {
                        break;
                    }
                    k++;
                }
            }
            if (k > 1)
            {
                temp1.length = k;
                *(*list + *amount) = temp1;
                (*amount)++;
            }
        }
    }
}
