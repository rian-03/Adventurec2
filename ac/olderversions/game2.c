#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//at this one I was making moving possible, since I didn't save the space files for the time, it doesn't run super good
//but it doesn't gotta, I mean it's an old and broken version.

int rprint(FILE* ptr, int rx, int ry, int w);
int dimread(FILE * space);
void moving(char move, int w, int h);

int x = 1;
int y = 1;
char move = ' ';

int objx[2];
int objy[2];

int main(void)
{
    while(move != 'l')
    {
        FILE *space = fopen("../spaces/outside.txt", "r");
        int w = dimread(space);
        int h = dimread(space);
            //to make this version runnable
            w = 26;
            h = 11;
        printf("w is %i and h is %i\n", w, h);
        printf("x is %i and y is %i\n", x, y);

        objx[0] = dimread(space);
        objx[1] = dimread(space);

        objy[0] = dimread(space);
        objy[1] = dimread(space);

        rprint(space, x, y, w);

        move = ' ';
        char idk[2];
        scanf("%s", idk);
        move = idk[0];

        moving(move, w, h);

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

int rprint(FILE* space, int rx, int ry, int w)
{
    char buffer = ' ';
    for(int a = 0; a < 1 + ry * w + rx; a++)
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
    return rx * ry;
}

void moving(char omove, int w, int h)
{

    if(omove == 'w' && y > 1)
    {
        y--;
        if (x > objx[0] && x < objx[1] && y > objy[0] && y < objy[1])
        {
            y++;
        }
    }
    else if(omove == 's' && y < h - 2)
    {
        y++;
        if (x > objx[0] && x < objx[1] && y > objy[0] && y < objy[1])
        {
            y--;
        }
    }
    else if(omove == 'a' && x > 1)
    {
        x--;
        if (x > objx[0] && x < objx[1] && y > objy[0] && y < objy[1])
        {
            x++;
        }
    }
    else if(omove =='d' && x < w - 3)
    {
        x++;
        if (x > objx[0] && x < objx[1] && y > objy[0] && y < objy[1])
        {
            x--;
        }
    }
}