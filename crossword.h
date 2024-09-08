#ifndef CROSSWORD_H_INCLUDED
#define CROSSWORD_H_INCLUDED
#include "crossword.c"
int isEmpty(CrossWord crossWord);
CrossWord copyCrossWord(CrossWord crossWord);
CrossWord checkWord(CrossWord crossWord, WordPosition word, char *wrd);

void solveCrossword(CrossWord crossWord, CrossWord *finalCrossWord, Words words, WordPositions wordPositions, int index);
void findAllPossibleWordPositions(CrossWord* crossWord, WordPositions *wordPositions);
#endif