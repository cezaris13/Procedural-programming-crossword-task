#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 256
void readData(char *filename,int *count,char **words[]){
    char *line = NULL;//char'o rodykle kuri pakolkas rodo i nieka
    size_t line_buf_size = 1;//cia yra line dydis bitais, kai virsija eilutes ilgis line dydi realloc iskvieciamas ir padidina
    int line_size=1;//nuskaitytos eilutes dydis
    FILE *fd=fopen(filename,"r");
    if(fd==NULL){
        printf("kazkas neto\n");
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
                *(words+(*count))=sth;
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
int main()
{
//    char *line = NULL;//char'o rodykle kuri pakolkas rodo i nieka
//    size_t line_buf_size = 0;//cia yra line dydis bitais, kai virsija eilutes ilgis line dydi realloc iskvieciamas ir padidina
//    int line_size=1;//nuskaitytos eilutes dydis
    int n=0,k=0;
    char *words[10], *zemelapis[6];
    readData("words.txt",&n,&words);
     for(int i=0;i<n;i++){
        printf("%d.%s\n",i+1,words[i]);
    }
    readData("crossword.txt",&k,&zemelapis);
     for(int i=0;i<k;i++){
        printf("%d.%s\n",i+1,zemelapis[i]);
    }
//    FILE *fdd=fopen("crossword.txt","r");
//    line_size=1;
//    while(1){
//        if(line_size>0){
//            line_size = getline(&line, &line_buf_size, fdd);
//            zemelapis[k]=malloc(MAX_SIZE*sizeof(char));
//            int i=0;
//            while(line[i]!='\n'){
//                zemelapis[k][i]=line[i];
//                i++;
//            }
//
//            k++;
//            free(line);
//              printf("%s\n",line);
//            line=calloc(MAX_SIZE,sizeof(char));
//        }
//        else{
//            break;
//        }
//    }
//    for(int i=0;i<6;i++){
//            for(int j=0;j<6;j++){
//                printf("%c ",zemelapis[i][j]);
//            }
//            printf("\n");
//    }
//    fclose(fdd);
    return 0;
}
