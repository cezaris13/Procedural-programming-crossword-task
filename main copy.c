#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_SIZE 256
#define EMPTY 'O'
#define WALL 'X'
clock_t t;
struct mapStruct
{
    int x_size;
    int y_size;
    char **map;
};
typedef struct mapStruct mapStruct;

mapStruct finalMap;
// char **finalMap=NULL;

int checkEmpty(mapStruct crossWord)
{
    for (int i = 0; i < crossWord.x_size; i++)
    {
        for (int j = 0; j < crossWord.y_size; j++)
        {
            if (crossWord.map[i][j] == EMPTY)
            {
                return 1;
            }
        }
    }
    return 0;
}
void printArray(char **arr, int size)
{
    // freopen("ans.txt","a",stdout);
    if (arr == NULL)
    {
        printf("neimanoma sugeneruoti sprendimo\n");
        return;
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < strlen(arr[i]); j++)
        {
            printf("%c", (arr[i][j] == WALL ? '_' : arr[i][j]));
        }
        printf("\n");
    }
}
char **copyMap(char **map, int x_size, int y_size)
{
    char **temp = malloc(x_size * sizeof(char *));
    for (int i = 0; i < x_size; i++)
    {
        *(temp + i) = malloc(y_size * sizeof(char));
        // for(int j=0;j<y_size;j++){
        //     temp[i][j]=map[i][j];
        // }
        memcpy(*(temp + i), *(map + i), y_size);
    }
    return temp;
}
void readData(char *filename, int *count, char ***arr)
{
    char *line = NULL;        // char'o rodykle kuri pakolkas rodo i nieka
    size_t line_buf_size = 1; // cia yra line dydis bitais, kai virsija eilutes ilgis line dydi realloc iskvieciamas ir padidina
    int line_size = 1;        // nuskaitytos eilutes dydis
    FILE *fd = fopen(filename, "r");
    if (fd == NULL)
    {
        printf("ivyko klaida atidarant faila: %s\n", filename);
        return;
    }
    while (1)
    {
        if (line_size >= 0)
        {
            char *sth = malloc(MAX_SIZE * sizeof(char));
            line_size = getline(&line, &line_buf_size, fd);
            int i = 0;
            if (line_size > 0)
            {
                while (line[i] != '\n')
                {
                    sth[i] = line[i];
                    i++;
                }
                sth[i] = '\0';
                *(*arr + (*count)) = sth;
                (*count)++;
            }
            free(line);
            line = calloc(MAX_SIZE, sizeof(char));
        }
        else
        {
            break;
        }
    }
    fclose(fd);
}
mapStruct checkWord(int x, int y, char *currWord, mapStruct crossWord, char orientation)
{
    int v = (orientation == 'v' ? 1 : 0); // vertical - v=1 h=0; horizontal v=0,h=1;
    int h = (orientation == 'v' ? 0 : 1);
    // char **tempMap=copyMap(crossWord);
    mapStruct tempMap;
    memcpy(tempMap.map, crossWord.map, crossWord.x_size);
    printf("testing\n");
    printArray(tempMap.map, tempMap.x_size);
    printf("______________\n");
    int n = strlen(currWord);
    for (int i = 0; i < n; i++)
    {
        if (tempMap.map[x + v * i][y + h * i] == EMPTY || tempMap.map[x + v * i][y + h * i] == currWord[i])
        {
            tempMap.map[x + v * i][y + h * i] = currWord[i];
        }
        else
        {
            tempMap.map[0][0] = '@';
            return tempMap;
        }
    }
    //      if(x_size!=n && tempMap[n][y]==EMPTY){
    //         tempMap[0][0] = '@';
    //
    //    }
    return tempMap;
}
void solveCrossword(char **words, int words_size, int index, mapStruct crossWord)
{ // todo non square map of 'x' and 'o'
    //    printArray(crossWord.map,crossWord.x_size);
    //    printf("\n");
    if (!checkEmpty(crossWord))
    {                         // jei masyvas uzpildytas arba zodziai pasibaige tada saugoti
        finalMap = crossWord; // copyMap(map,x_size,y_size);
        return;
    }
    if (index < words_size)
    {
        int maxLenx = crossWord.x_size - strlen(words[index]); // change later
        int maxLeny = crossWord.y_size - strlen(words[index]); // change later
        for (int i = 0; i < crossWord.x_size; i++)
        {
            for (int j = 0; j <= maxLenx; j++)
            {
                mapStruct temp = checkWord(j, i, words[index], crossWord, 'v');
                if (temp.map[0][0] != '@')
                {
                    solveCrossword(words, words_size, index + 1, temp);
                }
                // free(temp);
            }
        }
        for (int i = 0; i < crossWord.x_size; i++)
        { // y_size perhaps TODO
            for (int j = 0; j <= maxLeny; j++)
            {
                mapStruct temp = checkWord(i, j, words[index], crossWord, 'h');
                if (temp.map[0][0] != '@')
                {
                    solveCrossword(words, words_size, index + 1, temp);
                }
                // free(temp);
            }
        }
    }
    else
    {
        return;
    }
    solveCrossword(words, words_size, index + 1, crossWord);
}

int main()
{
    int n = 0, k = 0;
    char **words, **zemelapis;
    words = malloc(MAX_SIZE * sizeof(unsigned char *));
    zemelapis = malloc(MAX_SIZE * sizeof(unsigned char *));
    readData("words.txt", &n, &words);
    readData("crossword.txt", &k, &zemelapis);
    mapStruct crossWord;
    crossWord.map = copyMap(zemelapis, k, strlen(zemelapis[0]));
    crossWord.x_size = k;
    crossWord.y_size = strlen(zemelapis[0]);
    t = clock();
    printArray(zemelapis, k);
    printf("\n");
    printArray(crossWord.map, k);
    printf("\n");
    zemelapis[0][0] = '@';
    printArray(zemelapis, k);
    printf("\n");
    printArray(crossWord.map, k);
    printf("\n");

    // tmp2=tmp1;
    // solveCrossword(words,n,0,crossWord);
    // char **tmp=copyMap(zemelapis,k,strlen(zemelapis[0]));
    // printArray(tmp1.map,k);
    t = clock() - t;
    // printArray(finalMap.map,k);

    printf("%fsek.\n", ((double)t) / CLOCKS_PER_SEC);
    return 0;
}
