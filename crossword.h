#ifndef CROSSWORD_H_INCLUDED
#define CROSSWORD_H_INCLUDED
#include "crossword.c"
int isEmpty(CrossWord crossWord);
CrossWord copyCrossWord(CrossWord crossWord);
// CrossWord checkWord(CrossWord crossWord, Word word);
CrossWord checkWord(CrossWord crossWord, Word word, char *wrd);

void solveCrossword(char **words, int words_size, CrossWord crossWord,CrossWord *finalCrossWord, int index, int amount, Word *list);
void findAllPossiblePlacements(CrossWord crossWord, Word **words, int *amount);
#endif