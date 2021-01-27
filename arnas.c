# include <stdio.h>
# include <stdlib.h>
# include <string.h>
 
#define MAX_SIZE 1000
#define MAX_LENGTH 100
 
char **readMatrix(FILE *matrixPtr, int *maxLength, int *maxDepth, int *onesLeft);
char **readWords(FILE *listPtr, int *wordCount);
 
char **copyMatrix(char **matrix, int maxWidth, int maxDepth);
char **copyList(char **list, int wordCounter);
 
void printMatrix(char **matrix, int maxWidth, int maxDepth);
void printList(char **list, int wordCount);
 
int solveCrossword(char **matrix, char **list, int maxWidth, int maxDepth, int wordCount, int lastX, int lastY, int onesLeft);
 
int writeToMatrixH(char ***matrix, char *word, int y, int x, int length, int maxWidth, int maxDepth, int *onesLeft);
int writeToMatrixV(char ***matrix, char *word, int y, int x, int length, int maxWidth, int maxDepth, int *onesLeft);
 
void removeWord(char ***list, int *wordCount, int number);
 
 
int main ()
{
    FILE *matrixPtr;
    FILE *listPtr;
    matrixPtr = fopen("crossword11.txt", "r");
    listPtr = fopen("words11.txt", "r");
    char **matrix, **list;
    int maxWidth = 0, maxDepth = 0, wordCount = 0, onesLeft = 0;
    matrix = readMatrix(matrixPtr, &maxWidth, &maxDepth, &onesLeft);
    //printMatrix(matrix, maxWidth, maxDepth);
    //printf("vienetu %d ilgis %d ir gylis %d\n", onesLeft, maxWidth, maxDepth);
 
    list = readWords(listPtr, &wordCount);
    //printList(list, wordCount);
 
    int i = solveCrossword(matrix, list, maxWidth, maxDepth, wordCount, 0, 0, onesLeft);
 
    if(!i)
        printf("Nera sprendiniu");
    free(matrix);
    free(list);
 
    return 0;
}
char **readMatrix(FILE *matrixPtr, int *maxLength, int *maxDepth, int *onesLeft)
{
    if(matrixPtr == NULL)
        return 0;
    char **matrix;
    int i = 0;
    matrix = malloc(MAX_SIZE);
    char temp;
    char *str;
    str = malloc(sizeof(char)*MAX_LENGTH);
    while(1)
    {
        matrix[i] = malloc(sizeof(char)*MAX_LENGTH);
        fscanf(matrixPtr, "%s", str);
        int y;
        for(y = 0; *str!='\0'; str++, y++)
        {
            if(*str == EOF || *str == '\n') // pasiekta eilutes pabaiga
                break;
            else if(*str == '0' || *str == '1') //butinas prideti simbolis i zemelapi
                {
                    if(*str == '1')
                        (*onesLeft)++;
                    if(y+1 > *maxLength)
                        *maxLength = y+1;
                    matrix[i][y] = *str;
                }
        }
        if(feof(matrixPtr)) //jei baigiasi duomenys iseiname
        {
            *maxDepth = i;
            return matrix;
        }
        i++;
    }
    return matrix;
}
char **readWords(FILE *listPtr, int *wordCount)
{
    if(listPtr == NULL)
        return 0;
    char **list;
    list = malloc(MAX_SIZE);
    char *str;
    str = malloc(sizeof(char)*(MAX_LENGTH+1));
    int i = 0;
    while(1)
    {
        list[i] = malloc(MAX_LENGTH);
        fgets(list[i], 10, listPtr);
        if(list[i][strlen(list[i])-1] == '\n')
        list[i][strlen(list[i])-1] = '\0';
        i++;
        if(feof(listPtr))
            break;
    }
    *wordCount = i;
    return list;
}
void printMatrix(char **matrix, int maxWidth, int maxDepth) //spausdina matrica
{
    for(int i=0; i<maxDepth; ++i)
    {
        for(int y=0; y<maxWidth; ++y)
            printf("%c", matrix[i][y]);
        printf("\n");
    }
    printf("\n");
}
void printList(char **list, int wordCount) //spausdina zodziu sarasa
{
    for(int i=0; i<wordCount; ++i)
        printf("%s\n", list[i]);
    printf("\n");
}
int solveCrossword(char **matrix, char **list, int maxWidth, int maxDepth, int wordCount, int lastX, int lastY, int onesLeft) //grazina 1, jei pavyko isspresti, 0 jei nepavyko
{
    if(onesLeft == 0)
    {
        printMatrix(matrix, maxWidth, maxDepth);
        return 1;
    }
    for(int y=0; y<maxDepth; ++y)
    {
        for(int x=0; x<maxWidth; ++x)
        {
            if(matrix[y][x] == '1') //radome, jog kazkur yra zodis
            {
                int length;
                if(x > 0 && matrix[y][x-1] != 0 || x < maxWidth-1 && matrix[y][x+1] != '0') //horizontalus zodis
                {
                    int start = x;
                    while(matrix[y][start-1] != '0') //ieskome starto
                    {
                        if(start > 0)
                            start--;
                        else
                            break;
                    }
                    if(matrix[y][start] == '0')
                        start++;
                    int end = start;
                    while(matrix[y][end] != '0') //ieskome pabaigos
                    {
                        if(end == maxWidth - 1)
                            break;
                        if(end < maxWidth-1)
                            end++;
                        else
                            break;
                    }
                    if(end == maxWidth)
                        end--;
                    if(matrix[y][end] == '0')
                        end--;
                    length = end - start + 1;
                    if(length > 1)//jei ilgis 1, tai galbut tai buvo vertikalus zodis
                    {
                        for(int i=0; i<wordCount; i++)
                            if(strlen(list[i]) == length)//randame tokio paties ilgio zodi
                            {
                                char **matrix2 = copyMatrix(matrix, maxWidth, maxDepth);
                                int onesLeft2 = onesLeft, wordCount2 = wordCount;
                                if(writeToMatrixH(&matrix2, list[i], start, y, length, maxWidth, maxDepth, &onesLeft2)) //jei pavyko irasyti zodi
                                {
                                    char **list2 = copyList(list, wordCount);
                                    removeWord(&list2,  &wordCount2, i);// si atkomentavus nebeveikia, taciau dabar istrina ne visus zodzius
                                    if(onesLeft2 == 0) //jei kryziazodis issprestas
                                    {
                                        printMatrix(matrix2, maxWidth, maxDepth);
                                        return 1;
                                    }
                                    else
                                    {
                                        if(solveCrossword(matrix2, list2, maxWidth, maxDepth, wordCount2, x+length-1, y, onesLeft2))//sprendziame toliau
                                            return 1;
                                    }
                                    free(list2);
                                }
                                free(matrix2);
                            }
                    return 0; //jei nera tinkamo zodzio uzpildyti
                    }
                }
                if(y > 0 && matrix[y-1][x] != '0' || y < maxDepth-1 && matrix[y+1][x] != '0') //vertikalus zodis
                {
                    int start = y;
                    while(matrix[start][x] != '0') //ieskome pradzios
                    {
                        if(start > 0)
                            start--;
                        else
                            break;
                    }
                    if(matrix[start][x] == '0')
                        start++;
                    int end = start;
                    while(matrix[end][x] != '0') //ieskome pabaigos
                    {
                        if(end == maxDepth - 1)
                            break;
                        if(end < maxDepth)
                            end++;
                        else
                            break;
                    }
                    if(end == maxDepth)
                        end--;
                    if(matrix[end][x] == '0')
                        end--;
                    length = end - start + 1;
                    for(int i=0; i<wordCount; i++)
                        if(strlen(list[i]) == length) //randami tokio ilgio zodi
                        {
                            char **matrix2 = copyMatrix(matrix, maxWidth, maxDepth);
                            int onesLeft2 = onesLeft, wordCount2 = wordCount;
                            printList(list, wordCount2);
                            if(writeToMatrixV(&matrix2, list[i], x, start, length, maxWidth, maxDepth, &onesLeft2)) //jei pavyko irasyti zodi
                            {
                                char **list2 = copyList(list, wordCount);
                                removeWord(&list2,  &wordCount2, i);
                                if(onesLeft2 == 0)
                                {
                                    printMatrix(matrix2, maxWidth, maxDepth);
                                    return 1;
                                }
                                else
                                {
                                    if(solveCrossword(matrix2, list2, maxWidth, maxDepth, wordCount2, x, y, onesLeft2) == 1)//bandome isspresti dalinai uzpildyta kryziazodi
                                    {
                                        free(list2);
                                        free(matrix2);
                                        return 1;
                                    }
                                }
                                free(list2);
                            }
                            free(matrix2);
                        }
                    return 0;
                }
                length = 1; //reiskia turime 1 raides zodi
                for(int i=0; i<wordCount; i++)
                    if(strlen(list[i]) == length)//surandame savo sarase vienos raides zodi
                    {
                        char **matrix2 = copyMatrix(matrix, maxWidth, maxDepth);
                        int onesLeft2 = onesLeft, wordCount2 = wordCount;
                        if(writeToMatrixV(&matrix2, list[i], x, y, length, maxWidth, maxDepth, &onesLeft2)) //jei pavyko irasyti zodi
                        {
                            char **list2 = copyList(list, wordCount);
                            removeWord(&list2,  &wordCount2, i);
                            if(onesLeft2 == 0)
                            {
                                printMatrix(matrix2, maxWidth, maxDepth);
                                return 1;
                            }
                            else
                            {
                                if(solveCrossword(matrix2, list2, maxWidth, maxDepth, wordCount2, x, y, onesLeft) == 1)//sprendziame toliau
                                {
                                    free(list2);
                                    free(matrix2);
                                    return 1;
                                }
                            }
                            free(list2);
                        }
                        free(matrix2);
                    }
            }
        }
    }
    return 0;
}
int writeToMatrixH(char ***matrix, char *word, int x, int y, int length, int maxWidth, int maxDepth, int *onesLeft) //grazina vieneta, jei galima tai parasyti, 0, jei ne
{
    int letterCount = 0, onesCount = 0;
    int possible = 1;
    for(int xx = x; xx<maxWidth, letterCount < length; xx++, letterCount++) //horizontaliai paraso zodi
    {
        if((*matrix)[y][xx] == '1')
        {
            onesCount++; //skaiciuoja vienetus, kad galetu atimti ju kiek is buvusiu
            (*matrix)[y][xx] = word[letterCount];
        }
        else if((*matrix)[y][xx] == word[letterCount])
                ;
            else //jeigu ne '1' ir nesutampa simboliai, zodis netinkamas
            {
                possible = 0;
                break;
            }
    }
    if(possible)
        *onesLeft -= onesCount;
    //printMatrix(*matrix, maxWidth, maxDepth);
    return possible;
}
int writeToMatrixV(char ***matrix, char *word, int x, int y, int length, int maxWidth, int maxDepth, int *onesLeft) //returns 1 if it is possible to write that, 0 otherwise
{
    int letterCount = 0, onesCount = 0;
    int possible = 1;
    for(int yy = y; yy<maxDepth, letterCount < length; ++yy, letterCount++)
    {
        if((*matrix)[yy][x] == '1') // 1 keiciame i raide
        {
            onesCount++;
            (*matrix)[yy][x] = word[letterCount];
        }
        else if((*matrix)[yy][x] == word[letterCount]) //jei jau yra raide tikriname ar tokia kokios reikia
                ;
            else
            {
                possible = 0;
                break;
            }
    }
    if(possible)
        *onesLeft = *onesLeft - onesCount;
 
    //printMatrix(*matrix, maxWidth, maxDepth);
    return possible;
}
void removeWord(char ***list, int *wordCount, int number)//deda paskutini zodi i istrinamo vieta
{
    (*wordCount)--;
    if(*wordCount == 0 || number == *wordCount )
        return;
    for(int i = 0; i<strlen((*list)[*wordCount]); ++i)
    {
        (*list)[number][i] = (*list)[*wordCount][i];
    }
    (*list)[number][strlen((*list)[*wordCount])]= '\0'; //string pabaigos zenklas
}
char **copyMatrix(char **matrix, int maxWidth, int maxDepth) //kopijuoja matrica
{
    char **copy;
    copy = malloc(MAX_SIZE);
    for(int y=0; y<maxDepth; ++y)
    {
        copy[y] = malloc(MAX_LENGTH);
        for(int x=0; x < maxWidth; ++x)
            copy[y][x] = matrix[y][x];
    }
    return copy;
}
char **copyList(char **list, int wordCount) //kopijuoja sarasa
{
    char **copy;
    copy = malloc(MAX_SIZE);
    for(int i=0; i<wordCount; ++i)
    {
        int y;
        copy[i] = malloc(MAX_LENGTH);
        for(y=0; list[i][y] !='\0'; ++y)
        {
            copy[i][y] = list[i][y];
        }
        copy[i][y] = '\0';
    }
    return copy;
} 
