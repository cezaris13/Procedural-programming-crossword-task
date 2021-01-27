#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 256
#define EMPTY 'O'
#define WALL 'X'
#define sthwrong printf("wrong\n")
char **finalMap=NULL;
int checkEmpty(char **map,int x_size,int y_size){
    for(int i=0;i<x_size;i++){
        for(int j=0;j<y_size;j++){
            if(map[i][j]==EMPTY){
                return 1;
            }
        }
    }
    return 0;
}
void printArray(char **arr,int size){
//    freopen("ans.txt","a",stdout);
    if(arr==NULL){
        printf("neimanoma sugeneruoti sprendimo\n");
        return;
    }
    for(int i=0;i<size;i++){
        for(int j=0;j<strlen(arr[i]);j++){
            printf("%c",(arr[i][j]==WALL?'_':arr[i][j]));
        }
        printf("\n");
    }
}
char **copyMap (char **map,int x_size,int y_size){
    char **temp=malloc(x_size*sizeof(char*));
    for(int i=0;i<x_size;i++){
        *(temp+i)= malloc(y_size*sizeof(char));
        for(int j=0;j<y_size;j++){
            temp[i][j]=map[i][j];
        }
    }
    return temp;
}
void readData(char *filename,int *count,char ***arr){
    char *line = NULL;//char'o rodykle kuri pakolkas rodo i nieka
    size_t line_buf_size = 1;//cia yra line dydis bitais, kai virsija eilutes ilgis line dydi realloc iskvieciamas ir padidina
    int line_size=1;//nuskaitytos eilutes dydis
    FILE *fd=fopen(filename,"r");
    if(fd==NULL){
        printf("ivyko klaida atidarant faila: %s\n",filename);
        return;
    }
    while(1){
        if(line_size>=0){
            char *sth=malloc(MAX_SIZE*sizeof(char));
            line_size = getline(&line, &line_buf_size, fd);
            int i=0;
            if(line_size>0){
                while(line[i]!='\n'){
                    sth[i]=line[i];
                    i++;
                }
                sth[i]='\0';
                *(*arr+(*count))=sth;
                (*count)++;
            }
            free(line);
            line=calloc(MAX_SIZE,sizeof(char));

        }
        else{
            break;
        }
    }
    fclose(fd);
}
char **checkVertically(int x, int y, char **map,char *currWord,int x_size,int y_size){
    char **tempMap=copyMap(map,x_size,y_size);
    int n=strlen(currWord);
    for (int i = 0; i < n; i++) {
        if (tempMap[x + i][y] == EMPTY ||  tempMap[x + i][y] == currWord[i]) {
            tempMap[x + i][y] = currWord[i];
        }
        else {
            tempMap[0][0] = '@';
            return tempMap;
        }
    }
//      if(x_size!=n && tempMap[n][y]==EMPTY){
//         tempMap[0][0] = '@';
//
//    }
    return tempMap;

}
char **checkHorizontally(int x, int y, char **map,char *currWord,int x_size,int y_size){
    char **tempMap=copyMap(map,x_size,y_size);
    int n=strlen(currWord);
    for (int i = 0; i < n; i++) {
        if (tempMap[x][y+i] == EMPTY ||  tempMap[x][y+i] == currWord[i]) {
            tempMap[x][y+i] = currWord[i];
        }
        else {
            tempMap[0][0] = '@';
            return tempMap;
        }
    }
//    if(y_size!=n && tempMap[x][n]==EMPTY){
//         tempMap[0][0] = '@';
//
//    }
    return tempMap;
}
void solveCrossword(char **words,int words_size, char **map,int index,int x_size, int y_size){
   if(!checkEmpty(map,x_size,y_size)){// jei masyvas uzpildytas arba zodziai pasibaige tada saugoti
        finalMap=copyMap(map,x_size,y_size);
        return;
   }
    if(index < words_size){
        int maxLen =  x_size- strlen(words[index]);//change later
        for (int i = 0; i < x_size; i++) {
            for (int j = 0; j <= maxLen; j++) {
                char **temp = checkVertically(j, i, map, words[index],x_size,y_size);
                if (temp[0][0] != '@') {
                    solveCrossword(words,words_size, temp, index + 1, x_size,y_size);
                }
                free(temp);
            }
        }
        for (int i = 0; i < x_size; i++) {
            for (int j = 0; j <= maxLen; j++) {
                char **temp = checkHorizontally(i, j,map, words[index],x_size,y_size);
                if (temp[0][0] != '@') {
                     solveCrossword(words,words_size, temp, index + 1, x_size,y_size);
                }
                free(temp);
            }
        }
    }
    else{
        return;
    }
    solveCrossword(words,words_size,map,index+1,x_size,y_size);
}

int main(){
    int n=0,k=0;
    char **words, **zemelapis;
    words=malloc( MAX_SIZE * sizeof ( unsigned char *) );
    zemelapis=malloc( MAX_SIZE * sizeof ( unsigned char *) );
    readData("words.txt",&n,&words);
    readData("crossword.txt",&k,&zemelapis);
    solveCrossword(words,n,zemelapis,0,k,strlen(zemelapis[0]));
    printArray(finalMap,k);
    return 0;
}
