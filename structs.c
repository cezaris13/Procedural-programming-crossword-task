struct Crossword
{
    char **map;
    int xSize, ySize;
};
typedef struct Crossword Crossword;

struct Words
{
    char **words;
    int size;
    int *lengthOccurence;
};
typedef struct Words Words;

struct WordPosition
{
    int x, y, length, isVertical;
};
typedef struct WordPosition WordPosition;

struct WordPositions
{
    WordPosition *positions;
    int length;
    int *lengthOccurence;
};
typedef struct WordPositions WordPositions;