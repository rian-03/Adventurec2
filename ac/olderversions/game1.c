#include <stdio.h>
#include <stdlib.h>

//this was more of a proof of concept, the goal here is to be able to open the space file, read some data and print it
//the dimread(space) function would soon go on to become obsolete, and die.

int rprint(FILE* ptr, int x, int y, int w, int h);
int dimread(FILE * space);

int main(void)
{
    int x = 1;
    int y = 1;
    char move = ' ';

    while(move != 'l')
    {
        FILE *space = fopen("../spaces/room1.txt", "r");
        int w = dimread(space);
        int h = dimread(space);

        rprint(space, x, y, w, h);

        move = ' ';
        char idk[1];
        scanf("%s", idk);
        move = idk[0];

        printf("------------------------------\n");
        fclose(space);
    }
}


int dimread(FILE * space)
{
    char b[] = "000";
    fread(&b, 1, 3, space);
    char skip = 's';
    fread(&skip, 1, 1, space);
    return atoi(b);
}

int rprint(FILE* space, int x, int y, int w, int h)
{
    char buffer = ' ';
    for(int a = 0; a < 1 + y * w + x; a++)
    {
        fread(&buffer, 1, 1, space);
        printf("%c", buffer);
    }
    printf("%co", 8);
    while(fread(&buffer, 1, 1, space))
    {
        printf("%c", buffer);
    }
    printf("\n");
    return x * y;
}