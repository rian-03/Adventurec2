#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//this one straight up doesn't compile, now, I could fix that, but...

int rprint(FILE *ptr, int rx, int ry, int w);
int dimread(FILE *space);
void moving(char move, int w, int h);
char* spacecat(char *buffer, int spaceindex);
void reading(FILE *space);
void doorcheck(int onetwomove);

int x = 1;
int y = 1;
char move = ' ';

int obj[4];
int doors[4];
int otherside[4];

char *spaces[] = {"outside", "room1", "room2"};
int currentspace = 0;

struct coin {
   int found;
   int where[2];
};
struct coin coins[] = {
    {1, {0, 0}},
    {1, {0, 0}},
    {1, {0, 0}}
};
int foundcoins = 0;

int main(void)
{
    while(move != 'l')
    {
        char spacefilename[32];
        FILE *space = fopen(spacecat(spacefilename, currentspace), "r");
        int w = dimread(space);
        int h = dimread(space);
        //printf("w is %i and h is %i\n", w, h);
        //printf("x is %i and y is %i\n", x, y);

        reading(space);
        //printf("ex%i ey%i lx%i ly%i space%i\n", doors[0], doors[1], doors[2], doors[3], currentspace);
        //printf("%s\n", spacecat(spacefilename, currentspace));

        rprint(space, x, y, w);

        if (foundcoins)
        {
            printf("coins: %i\n", foundcoins);
        }
        move = ' ';
        char idk[64];
        scanf("%64s", idk);
        move = idk[0];

        moving(move, w, h);

        printf("\n\n\n\n\n\n\n\n\n\n\n\n------------------------------\n");
        fclose(space);
    }
}


int dimread(FILE *space)
{
    char skip = 's';
    char b[] = "000";
    fread(&b, 1, 3, space);
    fread(&skip, 1, 1, space);
    return atoi(b);
}

int rprint(FILE *space, int rx, int ry, int w)
{
    int coinlocal = coins[currentspace].where[1] * w + coins[currentspace].where[0] + 1;
    int counter = 0;

    char buffer = ' ';
    for(int a = 0; a < 1 + ry * w + rx; a++)
    {
        fread(&buffer, 1, 1, space);
        printf("%c", buffer);
        counter++;
        if (!coins[currentspace].found && counter == coinlocal)
        {
            printf("%c ", 8);
        }
    }
    printf("%co", 8);
    while(fread(&buffer, 1, 1, space))
    {
        printf("%c", buffer);
        counter++;
        if (!coins[currentspace].found && counter == coinlocal)
        {
            printf("%c ", 8);
        }
    }
    printf("\n");
    return rx * ry;
}

void moving(char omove, int w, int h)
{
    if (x == coins[currentspace].where[0] && y == coins[currentspace].where[1] && coins[currentspace].found)
    {
        coins[currentspace].found = 0;
        foundcoins++;
    }
    if(omove == 'w')
    {
        y--;
        doorcheck(1);
        if (y == 0 || (x > obj[0] && x < obj[1] && y > obj[2] && y < obj[3]))
        {
            y++;
        }
    }
    else if(omove == 's')
    {
        y++;
        doorcheck(1);
        if (y == h - 1 || (x > obj[0] && x < obj[1] && y > obj[2] && y < obj[3]))
        {
            y--;
        }
    }
    else if(omove == 'a')
    {
        x -= 2;
        doorcheck(0);
        if (x==-1 || (x > obj[0] && x < obj[1] && y > obj[2] && y < obj[3]))
        {
            x += 2;
        }
        else if (x==0)
        {
            x++;
        }
    }
    else if(omove =='d' && x < w - 2)
    {
        x +=2;
        doorcheck(0);
        if (x == w - 1 || (x > obj[0] && x < obj[1] && y > obj[2] && y < obj[3]))
        {
            x -= 2;
        }
        else if (x == w - 2)
        {
            x -= 1;
        }
    }
    else if(omove =='q')
    {
        x--;
        doorcheck(1);
        if (x == 0 || (x > obj[0] && x < obj[1] && y > obj[2] && y < obj[3]))
        {
            x++;
        }
    }
    else if(omove =='f')
    {
        x++;
        doorcheck(1);
        if (x == w - 2 || (x > obj[0] && x < obj[1] && y > obj[2] && y < obj[3]))
        {
            x--;
        }
    }
}

char* spacecat(char *buffer, int spaceindex)
{
    char *templa = "spaces/";
    char *templb = ".txt";

    int counter = 0;
    while (templa[counter])
    {
        buffer[counter] = templa[counter];
        counter++;
    }
    while (spaces[spaceindex][counter - 7])
    {
        buffer[counter] = spaces[spaceindex][counter - 7];
        counter++;
    }
    for (int l = 0; templb[l]; l++)
    {
        buffer[counter + l] = templb[l];
    }
    buffer[counter + 4] = '\0';
    return buffer;
}

void reading(FILE *space)
{
    for (int i = 0; i < 4; i++)
    {
        obj[i] = dimread(space);
    }
    for (int i = 0; i < 4; i++)
    {
        doors[i] = dimread(space);
    }
    for (int i = 0; i < 4; i++)
    {
        otherside[i] = dimread(space);
    }
    coins[currentspace].where[0] = dimread(space);
    coins[currentspace].where[1] = dimread(space);
}

// 1 if one move and 0 if two moves
void doorcheck(int onetwomove)
{
    if (onetwomove)
    {
        if (x == doors[2] && y == doors[3])
        {
            currentspace++;
            x = otherside[2];
            y = otherside[3];
        }
        else if (x == doors[0] && y == doors[1])
        {
            currentspace--;
            x = otherside[0];
            y = otherside[1];
        }
    }
    else
    {
        if ((x == doors[2] || x == doors[2] + 1) && y == doors[3])
        {
            currentspace++;
            x = otherside[2];
            y = otherside[3];
        }
        else if ((x == doors[0] || x == doors[0] -1) && y == doors[1])
        {
            currentspace--;
            x = otherside[0];
            y = otherside[1];
        }
    }
}
