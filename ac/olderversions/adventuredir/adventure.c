#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//all function declarations after main in order
void menurun(void);
void reading();
void header(void);
int rprint(void);
void subprint(void);
void thingprintcheck(int counter);
void moving(void);
void movecheck(int *xory, int movemod);
int doorcheck(int onetwomove);
char *spacecat(char *buffer, int spaceindex);
char *textcat(char *buffer, char *filename);
void langread(void);
int labrynthgame(char codon[3], int counter);
void action(void);
int cmovecheck(int cindex, int axism, int axiss, int movemod);
int death (void);

int x = 1, y = 1, w, h;

//get rid of spaces w, h, obj, doors, otherside, coin from struct space
//doors will be 'd' chars, walls '-' and '|'
//spaces dont need a name, theyll just be 000 or 111 and so on

//coins should just count wether they were found, no preset coins, if a '$' is found in the game a new coin
// is saved as having been found (and added to cointotal variable?(or coins list?)).
//struct coin will now contain int space and int where[], so it can be hidden after found.
struct coin {
    int found, where[2];
};
struct space {
    char *name;
    int w, h;
    int obj[4], doors[4], otherside[4];
    struct coin coin;
    int creatures;
};
struct space spaces[] = {
    {"outside", 26, 11, {8, 21, 1, 9}, {0, 0, 9, 5}, {0, 0, 1, 3}, {1, {21, 3}}, 0},
    {"room1", 13, 7, {-1, -1, -1, -1}, {0, 3, 10, 5}, {8, 5, 1, 2}, {1, {10, 1}}, 0},
    {"room2", 13, 5, {-1, -1, -1, -1}, {0, 0, 11, 2}, {0, 0, 1, 6}, {1, {1, 1}}, 0},
    {"room3", 32, 11, {-1, -1, -1, -1}, {0, 6, 30, 5}, {10, 2, 1, 2}, {1, {26, 4}}, 0},
    {"room4 r0", 13, 6, {-1, -1, -1, -1}, {0, 2, 11, 3}, {29, 5, 1, 2}, {1, {1, 1}}, 0},
    {"room4 0", 13, 6, {-1, -1, -1, -1}, {0, 2, 11, 3}, {10, 3, 1, 2}, {1, {1, 1}}, 4},
    {"room4 r1", 14, 7, {-1, -1, -1, -1}, {0, 2, 12, 3}, {10, 3, 1, 1}, {1, {1, 1}}, 0},
    {"room4 1", 19, 6, {-1, -1, -1, -1}, {0, 1, 17, 3}, {11, 3, 1, 1}, {1, {16, 1}}, 4},
    {"room4 2", 19, 6, {-1, -1, -1, -1}, {0, 1, 17, 3}, {16, 3, 1, 1}, {1, {16, 1}}, 4},
    {"room5", 19, 6, {-1, -1, -1, -1}, {0, 1, 0, 0}, {16, 3, 8, 5}, {1, {16, 1}}, 0},
    {"wayout", 18, 7, {-1, -1, -1, -1}, {8, 6, 0, 0}, {16, 3, 0, 0}, {1, {12, 4}}, 0}
};
int spacen = 11;

//struct items are good, except items will have a space too
struct item {
    float bright;
    int dmg, heal, found, used, where[2];
    char move;
    char *name[3];
};
struct item items[] = {
    {10, 0, 0, 0, 0, {6, 9}, 0, {"lamp", "lamparina", "lámpara"}},
    {0, 0, 0, 0, 0, {0, 0}, 0, {"", "", ""}},
    {5, 0, 0, 0, 0, {10, 3}, 0, {"torch", "tocha", "antorcha"}},
    {-1, 0, 0, 0, 0, {29, 1}, 0, {"semi collon", "ponto e vírgula", "punto y coma"}},
    {5, 0, 0, 0, 0, {3, 1}, 0, {"torch", "tocha", "antorcha"}},
    {0, 2, 0, 0, 0, {1, 3}, 0, {"club", "graveto", "palo"}},
    {1, 0, 5, 0, 0, {6, 3}, 'e', {"potion (hp 5)", "poção (hp 5)", "poción (hp 5)"}},
    {1, 3, 0, 0, 0, {15, 4}, 0, {"sword", "espada", "espada"}},
    {1, 0, 5, 0, 0, {14, 2}, 'e', {"potion (hp 5)", "poção (hp 5)", "poción (hp 5)"}},
    {0, 0, 0, 1, 0, {13, 4}, 0, {"", "", ""}},
    {0, 0, 0, 0, 0, {0, 0}, 0, {"", "", ""}}
};

//creatures seem good
struct creature {
    int hp, hpwas, dmg, space, where[2], was[2];
    char *name;
};
struct creature creatures[] = {
    {1, 1, 1, 5, {4, 1}, {4, 1}, "skeleton 1"},
    {1, 1, 1, 5, {7, 1}, {7, 1}, "skeleton 2"},
    {1, 1, 1, 5, {2, 4}, {2, 4}, "skeleton 3"},
    {1, 1, 1, 5, {6, 4}, {6, 4}, "skeleton 4"},

    {2, 2, 2, 7, {5, 1}, {4, 1}, "zombie"},
    {1, 1, 1, 7, {7, 1}, {7, 1}, "skeleton 1"},
    {1, 1, 1, 7, {2, 4}, {2, 4}, "skeleton 2"},
    {1, 1, 1, 7, {6, 4}, {6, 4}, "skeleton 3"},

    {4, 4, 2, 8, {5, 1}, {4, 1}, "mummy"},
    {2, 2, 2, 8, {7, 1}, {7, 1}, "zombie"},
    {2, 2, 2, 8, {2, 4}, {2, 4}, "zombie"},
    {2, 2, 2, 8, {6, 4}, {6, 4}, "zombie"}
};

//only four moveopts now
struct moveopt {
    char key; int *axis; int mod;
};
struct moveopt moveset[] ={
    {'w', &y, -1}, {'s', &y, 1},
    {'a', &x, -2}, {'d', &x, 2},
    {'q', &x, -1}, {'f', &x, 1}
};

//ill check this out
char *menuopt[] = {
    "guide", "guia", "guía",
    "language", "língua", "lenguaje",
    "start", "começar", "empezar",
    "leave", "sair", "salir",
};
char *deathmsg[] = {
    "try again\n", "tente novamente\n", "inténtalo de nuevo\n"
}, *coins[] = {
    "coins", "moedas", "monedas"
}, *damage[] = {
    "damage", "dano", "daño"
};

char move = ' ', spacefilenamebuf[64], scanbuf = ' ';
int currentspace = 0, foundcoins = 0, coinlocal = 0, itemlocal = 0;
int movesetnum = 6, menu = 0, lang = 0, dif = 0;
int tcreatures = 12, lastspace = 5, hp = 10, hpwas = 10, tdmg = 0, tdmgwas = 0, lastvspace = 0, dmg = 1, dmgwas = 1;
float visibility = 1;


int main(void)
{
    langread();

    while (menu != 5)
    {
        menurun();
    }
    while (move != 'l' && move != 'L')
    {
        header();
        rprint();
        subprint();
        //debugging and general use printf
        //printf("x is %i and y is %i\n", x, y);
        moving();
        action();
    }
}


//the whole ass menu
void menurun()
{
    printf("(1)%s\n(2)%s\n(3)%s\n(4)%s\n",
    menuopt[0 + lang], menuopt[3 + lang], menuopt[6 + lang], menuopt[9 + lang]);

    move = getchar();
    while (getchar() != 10) {}
    if (atoi(&move)) {menu = atoi(&move);}
    printf("\n\n");

    if (menu == 4 || move == 'l')
    {
        menu = 5;
        move = 'l';
    }
    else if (menu == 1)
    {
        FILE *controlsf = fopen(textcat(spacefilenamebuf, "controls"), "r");
        while (fread(&scanbuf, 1, 1, controlsf))
        {
            printf("%c", scanbuf);
        }
        fclose(controlsf);
    }
    else if (menu == 2)
    {
        printf("(1)english\n(2)português\n(3)español\n");
        move = getchar();
        while (getchar() != 10) {}
        int test = atoi(&move);
        if (test > 0 && test < 4)
        {
            lang = test - 1;
        }
        FILE *lfile = fopen("lang.txt", "w");
        char langwrite = '0' + lang;
        fwrite(&langwrite, 1, 1, lfile);
        fclose(lfile);
    }
    else if (menu == 3)
    {
        //difficulties used to be (0)peaceful\n(1)easy\n(2)medium\n(3)hard\n
        if (lang == 0) {printf("difficulty:\n(0)story mode\n(1)strategy mode\n(2)strategy (hard)\n(3)strategy (very hard)\n");}
        if (lang == 1) {printf("dificuldade:\n(0)modo história\n(1)modo estratégia\n(2)estratégia (difícil)\n(3)estratégia (muito difícil)\n");}
        if (lang == 2) {printf("dificultad:\n(0)modo historia\n(1)modo estrategia\n(2)estrategia (difícil)\n(3)estrategia (muy dificil)\n");}
        move = getchar();
        while (getchar() != 10) {}
        int test = atoi(&move);
        if (test > -1 && test < 4)
        {
            dif = test;
        }
        menu = 5;
    }
}

//uses dimread to read from space file into global variables
void reading()
{
    w = spaces[currentspace].w;
    h = spaces[currentspace].h;
    coinlocal = spaces[currentspace].coin.where[1] * w + spaces[currentspace].coin.where[0];
    itemlocal = items[currentspace].where[1] * w + items[currentspace].where[0];
}

//helper for rprint, prints header with coins and items
void header(void)
{
    system("clear");
    printf("------------------------------\n");
    if (death())
    {
        printf("%s", deathmsg[lang]);
    }
    printf("%s: %i\n", coins[lang], foundcoins);
    printf("items: ");
    for (int i = 0; i < spacen - 2; i++)
    {
        if (items[i].found > items[i].used)
        {
            printf("%s, ", items[i].name[lang]);
            if (items[i].found - items[i].used > 1)
            {printf("%i", items[i].found - items[i].used);}
        }
    }
    if (dif > 0)
    {
        printf("\nhp: %i", hp - tdmg);
        printf("\n%s: %i", damage[lang], dmg);
    }
    printf("\n");
}

//room printer function, considers the room's coin location and the player location
int rprint()
{
    FILE *space = fopen(spacecat(spacefilenamebuf, currentspace), "r");
    reading();

    int counter = 0;
    char codon[3] = {' ', ' ', ' '};
    fread(&codon[2], 1, 1, space);

    if (!currentspace) {visibility += 20;}
    while (1)
    {
        codon[0] = codon[1]; codon[1] = codon[2];
        printf("%c", codon[1]);
        thingprintcheck(counter);
        if (!fread(&codon[2], 1, 1, space)) {printf("\n\n"); break;}
        if (labrynthgame(codon, counter)) {header(); rprint(); fclose(space); return 0;}
        counter++;
    }
    if (!currentspace) {visibility -= 20;}

    fclose(space);
    return counter;
}

//prints current spaces subs in correct language
void subprint()
{
    FILE *space = fopen(textcat(spacefilenamebuf, spaces[currentspace].name), "r");
    char buffer = ' ';
    while (fread(&buffer, 1, 1, space))
    {
        printf("%c", buffer);
    }
    fclose(space);
    printf("\n");
}

//checks if current character is the one of a found coin or item and blanks it out if so
void thingprintcheck(int counter)
{
    int cy = counter / w;
    int cx = counter - cy * w;
    //coin check
    if (!spaces[currentspace].coin.found && counter == coinlocal)
    {
        printf("%c ", 8);
    }
    //item check
    if (items[currentspace].found && counter == itemlocal)
    {
        printf("%c ", 8);
    }
    //creature check
    for (int i = 0; i < tcreatures; i++)
    {
        if (currentspace == creatures[i].space && creatures[i].hp > 0 &&
            cx == creatures[i].where[0] && cy == creatures[i].where[1])
        {
            printf("%c%i", 8, creatures[i].hp);
        }
    }
    if (counter == y * w + x)
    {
        printf("%co", 8);
    }
    //shadows //idea to make shadows a circle function :)did it . also item shadows currentspace &&
    int ix = items[currentspace].where[0];
    int iy = items[currentspace].where[1];
    if (((!items[currentspace].found && (cx < w - 1 &&
        ((cx - x)*(cx - x) / 4 + (cy - y)*(cy - y) > visibility) &&
        ((cx - ix)*(cx - ix) / 4 + (cy - iy)*(cy - iy) >= items[currentspace].bright))) ||
        (items[currentspace].found && cx < w - 1 &&
        ((cx - x)*(cx - x) / 4 + (cy - y)*(cy - y) > visibility))))
    {
            printf("%c#", 8);
    }
}

//& moved axis; +1 for +1, -2 for -2, etc //uses doorcheck, checks for walls, objs and coin position
void movecheck(int *xory, int movemod)
{
    if (doorcheck(movemod))
        {reading();}
    //borders check
    if (x == 0 || x == -1 || x == w - 1 || x == w - 2 || y == 0 || y == h - 1 ||
        (x > spaces[currentspace].obj[0] && x < spaces[currentspace].obj[1] &&
        y > spaces[currentspace].obj[2] && y < spaces[currentspace].obj[3]))
    {
        *xory -= movemod;
        if ((movemod < 0 && (x == 2 || (x == spaces[currentspace].obj[1] + 1 &&
        spaces[currentspace].obj[2] < y && spaces[currentspace].obj[3] > y))) ||
        (movemod > 0 && (x == w - 4 || (x == spaces[currentspace].obj[0] - 1 &&
        spaces[currentspace].obj[2] < y && spaces[currentspace].obj[3] > y))))
        {
            *xory += movemod / 2;
        }
    }
    //coin check
    if (coinlocal == y * w + x && spaces[currentspace].coin.found)
    {
        spaces[currentspace].coin.found = 0;
        foundcoins++;
    }
    //item check
    if (!items[currentspace].found && x == items[currentspace].where[0] && y == items[currentspace].where[1])
    {
        items[currentspace].found = 1;
        if (items[currentspace].bright > visibility)
            {visibility = items[currentspace].bright;}
        if (items[currentspace].dmg > dmg)
            {dmg = items[currentspace].dmg;}
    }
    //attack check
    for (int i = 0; i < tcreatures; i++)
    {
        if (creatures[i].hp > 0 && creatures[i].space == currentspace &&
            x == creatures[i].where[0] && y == creatures[i].where[1])
        {
            creatures[i].hp -= dmg;
            if (creatures[i].hp <= 0)
            {
                spaces[currentspace].creatures--;
                //determine lastspace
                if (spaces[currentspace].creatures <= 0)
                {
                    for (int j = 0; j < spacen; j++)
                    {
                        if (spaces[j].creatures > 0)
                        {
                            lastspace = j;
                            break;
                        }
                    }
                }
            }
            else
            {*xory -= movemod;}
        }
    }
    //check if ended game
    if (currentspace == spacen - 1 && y < 4)
    {
        move = 'l';
    }
}

//moving function, wasd for normal movement, qf for moving a single character horizontaly
void moving()
{
    move = ' ';
    move = getchar();
    while (getchar() != 10) {}

    for (int i = 0; i < movesetnum; i++)
    {
        if (move == moveset[i].key || move == moveset[i].key + ('A'-'a'))
        {
            *moveset[i].axis += moveset[i].mod;
            movecheck(moveset[i].axis, moveset[i].mod);
        }
    }
        //checking if using consumable item
    //semi collon to win game
    int smcl = spacen - 2;
    if (currentspace == smcl && x == items[smcl].where[0] && y == items[smcl].where[1] &&
        items[3].found && items[smcl].found)
    {
        items[3].found = 0, items[smcl].found = 0;
        items[smcl].where[0] = 0; items[smcl].where[1] = 0;
        x++;
        spaces[currentspace].doors[2] = 17;
        spaces[currentspace].doors[3] = 3;
        spaces[spacen - 2].name = "room5fixed";
    }
    //using items
    for (int i = 0; i < spacen; i++)
    {
        if ((move == items[i].move || move == items[i].move + ('A'-'a')) &&
            items[i].heal && items[i].found && !items[i].used)
        {
            hp += items[i].heal;
            items[i].used++;
            break;
        }
    }
}

// checks if current space is a door
int doorcheck(int movemod)
{
    int changed = 0;
    if (movemod * movemod == 4)
    {
        if ((x == spaces[currentspace].doors[2] || x == spaces[currentspace].doors[2] + 1) &&
        y == spaces[currentspace].doors[3])
        {
            x = spaces[currentspace].otherside[2], y = spaces[currentspace].otherside[3];
            currentspace++;
            if (currentspace > lastvspace && lastspace == currentspace)
                {hpwas = hp; tdmgwas = tdmg; dmgwas = dmg;}
            changed++;
        }
        else if ((x == spaces[currentspace].doors[0] || x == spaces[currentspace].doors[0] - 1) &&
        y == spaces[currentspace].doors[1])
        {
            x = spaces[currentspace].otherside[0], y = spaces[currentspace].otherside[1];
            currentspace--;
            changed++;
        }
    }
    else
    {
        if (x == spaces[currentspace].doors[2] && y == spaces[currentspace].doors[3])
        {
            x = spaces[currentspace].otherside[2], y = spaces[currentspace].otherside[3];
            currentspace++;
            if (currentspace > lastvspace && lastspace == currentspace)
                {hpwas = hp; tdmgwas = tdmg; dmgwas = dmg;}
            changed++;
        }
        else if (x == spaces[currentspace].doors[0] && y == spaces[currentspace].doors[1])
        {
            x = spaces[currentspace].otherside[0], y = spaces[currentspace].otherside[1];
            currentspace--;
            changed++;
        }
    }
    if (currentspace > lastvspace)
        {lastvspace = currentspace;}
    if (dif == 0 && currentspace == 5) {currentspace = 9; x = spaces[8].otherside[2]; y = spaces[8].otherside[3];}
    else if (dif == 0 && currentspace == 8) {currentspace = 4; x = spaces[5].otherside[0]; y = spaces[5].otherside[1];}
    else if (dif == 1 && currentspace == 7) {currentspace = 9; x = spaces[8].otherside[2]; y = spaces[8].otherside[3];}
    else if (dif == 1 && currentspace == 8) {currentspace = 6; x = spaces[7].otherside[0]; y = spaces[7].otherside[1];}
    else if (dif == 2 && currentspace == 8 && movemod > 0) {currentspace = 9; x = spaces[8].otherside[2]; y = spaces[8].otherside[3];}
    else if (dif == 2 && currentspace == 8 && movemod < 0) {currentspace = 7; x = spaces[8].otherside[0]; y = spaces[8].otherside[1];}
    return changed;
}

//concatenation function for making the space file name string in format: spaces/SPACENAME.txt
char *spacecat(char *buffer, int spaceindex)
{
    char *templa = "spacey/";
    char *templb = ".txt";
    int counter = 0;

    while (templa[counter])
    {
        buffer[counter] = templa[counter];
        counter++;
    }
    int spl = counter;
    while (spaces[spaceindex].name[counter - spl])
    {
        buffer[counter] = spaces[spaceindex].name[counter - 7];
        counter++;
    }
    for (int l = 0; templb[l]; l++)
    {
        buffer[counter + l] = templb[l];
    }
    buffer[counter + 4] = '\0';

    return buffer;
}

//concatenation function for making the space subtitle file name string in format: "subs/" + "SPACENAME" + "LANG" + ".txt"
char *textcat(char *buffer, char *filename)
{
    char *templa = "subs/";
    char *templb = ".txt";
    int counter = 0;

    while (templa[counter])
    {
        buffer[counter] = templa[counter];
        counter++;
    }
    buffer[counter] = '0' + lang;
    buffer[counter + 1] = '/';
    counter += 2;
    int spl = counter;
    while (filename[counter - spl])
    {
        buffer[counter] = filename[counter - spl];
        counter++;
    }
    for (int l = 0; templb[l]; l++)
    {
        buffer[counter + l] = templb[l];
    }
    buffer[counter + 4] = '\0';

    return buffer;
}

void langread()
{
    FILE *lfile = fopen("lang.txt", "r");
    lang = fgetc(lfile) - '0';
    fclose(lfile);
}

//labrynth game
int labrynthgame(char codon[3], int counter)
{
    if (currentspace != 3 || counter != y * w + x || (codon[1] != '=' && codon[1] != '|' && codon[1] != '+'))
    {
        return 0;
    }
    int moveindex = 0;
    for (int i = 0; i < movesetnum; i++)
    {
        if (move == moveset[i].key || move == moveset[i].key + ('A'-'a'))
        {
            moveindex = i;
        }
    }
    if (moveset[moveindex].axis == &y)
    {
        y -= moveset[moveindex].mod;
    }
    else
    {
        if (codon[1 - moveset[moveindex].mod / 2] == '-' ||
            codon[1 - moveset[moveindex].mod / 2] == '|' ||
            codon[1 - moveset[moveindex].mod / 2] == '+')
        {
            *moveset[moveindex].axis -= moveset[moveindex].mod;
        }
        else if (moveset[moveindex].mod > 1 || moveset[moveindex].mod < -1)
        {
            *moveset[moveindex].axis -= moveset[moveindex].mod / 2;
        }
        else
        {
            *moveset[moveindex].axis -= moveset[moveindex].mod;
        }
    }
    return 1;
}

//creature action
void action(void)
{
    for (int i = 0; i < tcreatures; i++)
    {
        //creatures move toward player
        if (creatures[i].hp > 0 && creatures[i].space == currentspace)
        {
            int cx = creatures[i].where[0], cy = creatures[i].where[1];
            if (cx > x && cmovecheck(i, 0, 1, -1))
                {creatures[i].where[0]--;}
            else if (cx < x && cmovecheck(i, 0, 1, 1))
                {creatures[i].where[0]++;}
            else if (cy > y && cmovecheck(i, 1, 0, -1))
                {creatures[i].where[1]--;}
            else if (cy < y && cmovecheck(i, 1, 0, 1))
                {creatures[i].where[1]++;}
        }
        //attack player
        if (creatures[i].hp > 0 && creatures[i].space == currentspace)
        {
            int cx = creatures[i].where[0], cy = creatures[i].where[1];
            if ((cx <= x + 1 && cx >= x - 1 && cy <= y + 1 && cy >= y - 1))
            {tdmg += creatures[i].dmg;}
        }
    }
}

//check if moving into other creatures
int cmovecheck(int cindex, int axism, int axiss, int movemod)
{
    int *axisis[] = {&x, &y};
    for (int i = 0; i < tcreatures; i++)
    {
        //would move into other creature
        if ((creatures[i].hp > 0 && i != cindex &&
            creatures[i].where[axism] == creatures[cindex].where[axism] + movemod &&
            creatures[i].where[axiss] == creatures[cindex].where[axiss]))
        {return 0;}
    }
    //would move into player (or out of game, just in case)
    if ((0 == creatures[cindex].where[axism] + movemod) ||
        (!axism && (w - 2 == creatures[cindex].where[0] + movemod)) ||
        (axism && (h - 1 == creatures[cindex].where[1] + movemod)) ||
        (*axisis[axism] == creatures[cindex].where[axism] + movemod &&
        *axisis[axiss] == creatures[cindex].where[axiss]))
    {return 0;}
    return 1;
}

//dying to the creatures
int death(void)
{
    //if dead
    if (tdmg >= hp)
    {
        //reset all creatures from further in the game
        for (int i = 0; i < tcreatures; i ++)
        {
            if (creatures[i].space >= lastspace)
            {
                if (creatures[i].hp <= 0) {spaces[creatures[i].space].creatures++;}
                creatures[i].hp = creatures[i].hpwas;
                creatures[i].where[0] = creatures[i].was[0];
                creatures[i].where[1] = creatures[i].was[1];
            }
        }
        //reset coins from further//reset items from further in the game
        for (int i = lastspace; i < spacen; i++)
        {
            if (!spaces[i].coin.found)
            {
                spaces[i].coin.found = 1;
                foundcoins--;
            }
            if (i != spacen - 2)
            {
                items[i].found = 0;
                items[i].used = 0;
            }
        }
        x = spaces[lastspace].otherside[0], y = spaces[lastspace].otherside[1];
        currentspace = lastspace - 1, hp = hpwas, tdmg = tdmgwas, dmg = dmgwas;
        return 1;
    }
    return 0;
}
