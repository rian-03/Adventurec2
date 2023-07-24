#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

//this was like, a very very old version, I started it not too long after we made mario less, I was trying to make
//snake, since I loved that as a kid, but didn't pursue that idea

const int h = 8;

int main(void)
{
    int coins = 0;

    char move = 'r';
    int x = 0;
    int y = 0;

    int xtem = (rand() / ((double) RAND_MAX + 1)) * h;
    int ytem = (rand() / ((double) RAND_MAX + 1)) * h;

    char *bac = malloc(4);
    sprintf(bac, "%c%c%c", 8, 8, 8);

    while (move != 'l')
    {
        for (int i = 0; i < h; i ++)
        {
            for (int j = 0; j < h; j++)
            {
                printf("   ");
            }

            if (i == ytem)
            {
                for (int j = 0; j < h - xtem; j++)
                {
                    printf("%s", bac);
                }
                printf(" x ");
                for (int j = 0; j < h - xtem - 1; j++)
                {
                    printf("   ");
                }
            }

            if (i == y)
            {
                for (int j = 0; j < h - x; j++)
                {
                    printf("%s", bac);
                }
                printf(" o ");
                for (int j = 0; j < h - x - 1; j++)
                {
                    printf("   ");
                }
            }

            if (i == y && y == ytem && xtem > x)
            {
                for (int j = 0; j < h - xtem; j++)
                {
                    printf("%s", bac);
                }
                printf(" x ");
                for (int j = 0; j < h - xtem - 1; j++)
                {
                    printf("   ");
                }
            }

            if (x == xtem && y == ytem)
            {
                xtem = (rand() / ((double) RAND_MAX + 1)) * h;
                ytem = (rand() / ((double) RAND_MAX + 1)) * h;
                coins++;
            }

            printf(".");

            if (i == 0)
            {
                printf("coins: %i", coins);
            }

            printf("\n");
        }

        printf("move: ");
        scanf("%c", &move);
        printf("\n");
        //move = getchar();

        //move = get_char("move: ");
        //switch (move)
        //{
            //case 'w':
            if (move == 'w' && y > 0)
            {
                y--;
            }
            //break;
            //case 'a':
            if (move == 'a' && x > 0)
            {
                x--;
            }
            //break;
            //case 's':
            if (move == 's' && y < h - 1)
            {
                y++;
            }
            //break;
            //case 'd':
            if (move =='d' && x < h - 1)
            {
                x++;
            }
        //}
    }

//    for(char movin =)

    free(bac);
}