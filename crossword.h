#ifndef CROSSWORD_H_INCLUDED
#define CROSSWORD_H_INCLUDED
#include "crossword.c"
int isEmpty(Crossword crossword);
Crossword copyCrossWord(Crossword crossword);
Crossword checkWord(Crossword crossword, WordPosition word, char *wrd);

int solveCrossword(Crossword crossword, Crossword *finalCrossWord, Words words, WordPositions wordPositions, int index);
void findAllPossibleWordPositions(Crossword* crossword, WordPositions *wordPositions);
#endif