#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 256
#define EMPTY 'O'
#define WALL 'X'
#define FAILEDMAP '@'
char **finalMap=NULL;//final matrix
int x_size, y_size;//crossword dimensions
int  checkEmpty(char **map);
char **checkWord(int x, int y, char **map,char *currWord,char orientation);
char **copyMap (char **map);
void printArray(char **arr, int size);
void readData(char *filename, int *line_count, char ***arr);
void solveCrossword(char **words, int words_size, char **map, int index);

int main(){
    int wordCount=0,linesCount=0;
    char **wordList, **crossWord;
    wordList=malloc(MAX_SIZE*sizeof(unsigned char *));
    crossWord=malloc(MAX_SIZE*sizeof(unsigned char *));
    readData("words2.txt",&wordCount,&wordList);
    for(int i=0;i<wordCount-1;i++){//sort word in descending order by their size
        for(int j=i+1;j<wordCount;j++){
            if(strlen(wordList[i])<strlen(wordList[j])){
                char *temp=wordList[i];
                wordList[i]=wordList[j];
                wordList[j]=temp;
            }
        }
    }
    readData("crossword2.txt",&linesCount,&crossWord);
    x_size=linesCount;
    y_size=strlen(crossWord[0]);
    solveCrossword(wordList,wordCount,crossWord,0);
    printArray(finalMap,linesCount);
    return 0;
}
int checkEmpty(char **map){
    for(int i=0;i<x_size;i++){
        for(int j=0;j<y_size;j++){
            if(map[i][j]==EMPTY){
                return 1;
            }
        }
    }
    return 0;
}
char **checkWord(int x, int y, char **map,char *currWord,char orientation){
    int v=(orientation=='v'?1:0);//vertical - v=1 h=0; horizontal v=0,h=1;
    int h=(orientation=='v'?0:1);
    if(x-v>=0 && y-h>=0 && map[x-v][y-h]==EMPTY){// check if element above of left is EMPTY if yes we can skip checking
        char **temp=malloc(1*sizeof(char*));
        temp[0]=malloc(1*sizeof(char));
        temp[0][0]=FAILEDMAP;
        return temp;
    }
    if(x+v<x_size &&y +h<y_size && map[x+v][y+h]==WALL){//if next element below or right is wall skip checking
        char **temp=malloc(1*sizeof(char*));
        temp[0]=malloc(1*sizeof(char));
        temp[0][0]=FAILEDMAP;
        return temp;
    }
    char **tempMap=copyMap(map);
    int n=strlen(currWord);
    for (int i=0;i<n;i++){
        if (tempMap[x+v*i][y+h*i]==EMPTY ||  tempMap[x+v*i][y+h*i]==currWord[i]){// checking if
            tempMap[x+v*i][y+h*i]=currWord[i];
        }
        else {
            tempMap[0][0]=FAILEDMAP;//using '@' to show that the word could not be inserted
            return tempMap;
        }
    }
    return tempMap;
}
char **copyMap (char **map){
    char **temp=malloc((x_size+1) * sizeof (unsigned char*));
    for(int i=0;i<x_size;i++){
        *(temp+i)= malloc((y_size+1)*sizeof(char));
        for(int j=0;j<y_size;j++){
            temp[i][j]=map[i][j];
        }
        temp[i][y_size]='\0';
    }
    return temp;
}
void printArray(char **arr,int size){
    //freopen("ans.txt","w",stdout);
    if(arr==NULL){
        printf("neimanoma sugeneruoti sprendimo\n");
        return;
    }
    for(int i=0;i<size;i++){
       printf("%s\n",arr[i]);
    }
}
void readData(char *filename,int *line_count,char ***arr){
    char *line=NULL;
    size_t line_buf_size=1;
    int line_size=1;
    FILE *fd=fopen(filename,"r");
    if(fd==NULL){
        printf("ivyko klaida atidarant faila: %s\n",filename);
        return;
    }
    while(line_size>=0){
        char *sth;
        line_size=getline(&line, &line_buf_size, fd);
        int i=0;
        if(line_size>0){
            if(strlen(line)<MAX_SIZE){
                sth=line;
                sth[strlen(line)-1]='\0';
                *(*arr+(*line_count))=sth;
                (*line_count)++;
            }
            else{
                printf("zodis %s virsijo maximalu zodzio ilgi: %d\n",line,MAX_SIZE);
            }
        }
        line=NULL;
    }
    fclose(fd);
}
void solveCrossword(char **words,int words_size, char **map,int index){
   if(!checkEmpty(map)){//if matrix doesn't have any empty symbols- that means crossword is solved
        finalMap=copyMap(map);
        return;
   }
    if(index<words_size){
        int maxLenx=x_size-strlen(words[index]);
        int maxLeny=y_size-strlen(words[index]);
        for (int i=0;i<y_size;i++){//check possible word placements vertically
            for (int j=0;j<=maxLenx;j++){
                if(map[j][i]!=WALL){//skip checking part if map[j][i] is WALL
                    char **temp=checkWord(j, i, map, words[index],'v');
                    if (temp[0][0] != FAILEDMAP) {
                        solveCrossword(words,words_size, temp, index + 1);
                    }
                    free(temp);
                }
            }
        }
        for (int i=0; i<x_size;i++){//check possible word placements horizontally
            for (int j=0;j<=maxLeny;j++){
                if(map[i][j]!=WALL){
                    char **temp=checkWord(i, j, map, words[index],'h');
                    if (temp[0][0] != FAILEDMAP){
                        solveCrossword(words,words_size, temp, index + 1);
                    }
                    free(temp);
                }

            }
        }
    }
    else{//if all words have been used
        return;
    }
    solveCrossword(words,words_size,map,index+1);//if element was not added to the matrix skip it
}
