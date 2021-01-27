#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 512
#define EMPTY 'O'
#define WALL 'X'
#define FAILEDMAP '@'
#define sthwrong printf("wrong\n")

struct crossWord{
    char **map;
    int x_size,y_size;
};
typedef struct crossWord crossWord;
// char **finalMap=NULL;
crossWord finalMap;
int checkEmpty(crossWord map){
    for(int i=0;i<map.x_size;i++){
        for(int j=0;j<map.y_size;j++){
            if(map.map[i][j]==EMPTY){
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
       printf("%s\n",arr[i]);
    }
}
char **copyMap (char **map,int x_size,int y_size){
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
void readData(char *filename,int *count,char ***arr){
    char *line = NULL;//char'o rodykle kuri pakolkas rodo i nieka
    size_t line_buf_size = 1;//cia yra line dydis bitais, kai virsija eilutes ilgis line dydi realloc iskvieciamas ir padidina
    int line_size=1;//nuskaitytos eilutes dydis
    FILE *fd=fopen(filename,"r");
    if(fd==NULL){
        printf("ivyko klaida atidarant faila: %s\n",filename);
        return;
    }
    while(line_size>=0){
        char *sth;
        line_size = getline(&line, &line_buf_size, fd);
        int i=0;
        if(line_size>0){
            if(strlen(line)<MAX_SIZE){
//                    if(*count==MAX_SIZE){
//                        break;
//                    }
                sth=line;
                sth[strlen(line)-1]='\0';

                *(*arr+(*count))=sth;
                (*count)++;
            }
            else{
                printf("zodis %s virsijo maximalu zodzio ilgi: %d\n",line,MAX_SIZE);
            }
        }
        line=NULL;
    }
    fclose(fd);
}
crossWord checkWord(int x, int y, char *currWord,crossWord map,char orientation){
    int v=(orientation=='v'?1:0);//vertical - v=1 h=0; horizontal v=0,h=1;
    int h=(orientation=='v'?0:1);
    crossWord tempMap=map;//copyMap(map,x_size,y_size);
    tempMap.map=malloc( (map.x_size+1) * sizeof ( unsigned char *) );
    // tempMap.map=map.map;
    for(int i=0;i<tempMap.x_size;i++){
        tempMap.map[i]=malloc((map.y_size+1)*sizeof(char));
        for(int j=0;j<tempMap.y_size;i++){
             tempMap.map[i][j]=map.map[i][j];
        }
        // tempMap.map[i]=map.map[i];
    //     memcpy(tempMap.map[i],map.map[i],tempMap.y_size);
    }
    int n=strlen(currWord);
    for (int i = 0; i < n; i++) {
        if (tempMap.map[x + v*i][y+h*i] == EMPTY ||  tempMap.map[x + v*i][y+h*i] == currWord[i]) {
            tempMap.map[x + v*i][y+h*i] = currWord[i];
        }
        else {
            tempMap.map[0][0] = FAILEDMAP;
            return tempMap;
        }
    }
//      if(x_size!=n && tempMap[n][y]==EMPTY){
//         tempMap[0][0] = FAILEDMAP;
//
//    }
    return tempMap;
}
void solveCrossword(char **words,int words_size,int index,crossWord map){
   if(!checkEmpty(map)){// jei masyvas uzpildytas arba zodziai pasibaige tada saugoti
        finalMap=map;//copyMap(map,x_size,y_size);
        return;
   }
    if(index < words_size){
        int maxLenx = map.x_size-strlen(words[index]);//change later
        int maxLeny = map.y_size-strlen(words[index]);
        for (int i = 0; i < map.y_size; i++) {
            for (int j = 0; j <= maxLenx; j++) {
                crossWord temp = checkWord(j, i, words[index],map,'v');
                if (temp.map[0][0] != FAILEDMAP) {
                    solveCrossword(words,words_size, index + 1,temp);
                }
                // free(temp);
            }
        }
        for (int i = 0; i < map.x_size; i++) {
            for (int j = 0; j <= maxLeny; j++) {
                crossWord temp = checkWord(i, j,words[index],map,'h');
                if (temp.map[0][0] != FAILEDMAP) {
                     solveCrossword(words,words_size, index + 1, temp);
                }
                // free(temp);
            }
        }
    }
    else{
        return;
    }
    solveCrossword(words,words_size,index+1,map);
}
int main(){
    int n=0,k=0;
    char **words, **zemelapis;
    crossWord structmap;
    words=malloc( MAX_SIZE * sizeof ( unsigned char *) );
    zemelapis=malloc( MAX_SIZE * sizeof ( unsigned char *) );
    readData("words2.txt",&n,&words);
    for(int i=0;i<n-1;i++){//simple bubble sort to sort words in descending order by their size(faster solve)
        for(int j=i+1;j<n;j++){
            if(strlen(words[i])<strlen(words[j])){
                char *temp=words[i];
                words[i]=words[j];
                words[j]=temp;
            }
        }
    }
    readData("crossword2.txt",&k,&zemelapis);
    structmap.map=copyMap(zemelapis,k,strlen(zemelapis[0]));
    structmap.x_size=k;
    structmap.y_size=strlen(zemelapis[0]);
    solveCrossword(words,n,0,structmap);
    printArray(finalMap.map,k);
    return 0;
}
