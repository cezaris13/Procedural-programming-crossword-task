#include <stdlib.h>
#include <stdio.h>

void printArray(char **solution, int size)
{
    if (solution == NULL)
    {
        printf("No solution has been generated\n");
        return;
    }
    for (int i = 0; i < size; i++)
        printf("%s\n", solution[i]);
}

void readData(char *filename, int *count, char ***lines)
{
    char *line = NULL;
    size_t lineBufSize = 1;
    int lineSize = 1;

    FILE *fd = fopen(filename, "r");
    if (fd == NULL)
    {
        printf("There was a problem while opening file: %s\n", filename);
        return;
    }

    while (lineSize >= 0)
    {
        lineSize = getline(&line, &lineBufSize, fd);
        int i = 0;
        if (lineSize > 0)
        {
            if (strlen(line) >= MAX_SIZE)
            {
                printf("Word %s is longer than MAX_SIZE: %d\n", line, MAX_SIZE);
                continue;
            }

            line[strlen(line) - 1] = '\0';

            *(*lines + (*count)) = line;
            (*count)++;
        }
        line = NULL;
    }
    fclose(fd);
}