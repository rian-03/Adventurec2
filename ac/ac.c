#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//all function declarations after main in order
void menurun(void);
void header(void);
int rprint(void);
void subprint(void);
int printcheck(int counter, char current);
void moving(void);
void movecheck(int *xory, int movemod);
char *newspace(char *buffer);
char *textcat(char *buffer, char *filename);
void langread(void);
void action(void);
int cmovecheck(int cindex, int axism, int axiss, int movemod);
int death (void);
int trading(void);
int creaturemoving(void);
int input_effect(int input_size);

int x = 2, y = 1, w = 28, h = 14;

struct space{
    int x;
    int y;
    int z;
    int in;
};
struct space space = {50, 50, 50, 0};
int spaceabrev = 50 + 5000 + 500000;

struct coin {
    int spaceabrev;
    int where;
};
struct coin coins[128];
int coinsf = 0;
int coinsu = 0;

struct item {
    char key, move;
    int found, used, bright, dmg, heal, price;
    char *name[3];
};
struct item items[] = {
   //key mov fd us br dm hl pr  name
    {'l', 0 , 0, 0, 4, 0, 0, 5 , {"lamp", "lamparina", "lámpara"}},
    {'t', 0 , 0, 0, 3, 0, 0, 0 , {"torch", "tocha", "antorcha"}},
    {'c', 0 , 0, 0, 0, 2, 0, 0 , {"club", "graveto", "palo"}},
    {'e','e', 0, 0, 2, 0, 5, 10, {"potion (hp 5)", "poção (hp 5)", "poción (hp 5)"}},
    {'s', 0 , 0, 0, 2, 4, 0, 10, {"sword", "espada", "espada"}},
    {'b', 0 , 0, 0, 0, 0, 0, 10, {"bow", "", ""}},
    {'a', 0 , 0, 0, 0, 2, 0, 1 , {"arrow", "", ""}},
};
int itemsn = 7;

struct founditem {
    int id, spaceabrev, where;
};
struct founditem founditems[16];
int founditemsn = 0;

struct lsource {
    float bright;
    int spaceabrev, x, y, item;
};
struct lsource lsources[] = {
    {4, 505050, 6, 9, 0},
    {3, 1505049, 24, 9, 1},
    {5, 1525049, 3, 1, 1},
    //{2},
    //{2},
    //{2},
    {4, 505150, 13, 6, -1},
};
int lsourcen = 4;
struct lsource activelsource = {0, 0, 0, 0, 0};

struct creature {
    int spaceabrev;
    int counter, where;
    int a;
    int moven;
};
struct creature creatures[100];
int creaturen = 0;

struct moveopt {
    char key; int *axis; int mod;
};
struct moveopt moveset[] ={
    {'w', &y, -1}, {'s', &y, 1},
    {'a', &x, -2}, {'d', &x, 2},
};

char *menuopt[] = {
    "guide", "guia", "guía",
    "language", "língua", "lenguaje",
    "start", "começar", "empezar",
    "leave", "sair", "salir",
};
char *deathmsg[] = {
    "try again\n", "tente novamente\n", "inténtalo de nuevo\n"
}, *coinsmsg[] = {
    "coins", "moedas", "monedas"
}, *damagemsg[] = {
    "damage", "dano", "daño"
};

//probably gonna change
char input[11];
char move = ' ', fnbuffer[32];
int movesetnum = 4, menu = 0, lang = 0, dif = 0;
int tcreatures = 12, lastspace = 5, hp = 10, hpwas = 10, tdmg = 0, tdmgwas = 0, lastvspace = 0, dmg = 1, dmgwas = 1;
float v = 2;
int moven = 0;


int main(void)
{
    newspace(fnbuffer);
    while (move != 'l' && move != 'L')
    {
        rprint();
        if (creaturemoving()) {system("clear"); printf("you died! :(\n"); return 0;}
        //TODO? vvv inefficient solution vvv
        rprint();

        //debugging printfs:

        //printf("x is %i and y is %i\n", x, y);
        //printf("fname:%s abrev:%i\n", fnbuffer, spaceabrev);
        printf("coins:%i hp:%i dmg:%i vis:%i\n", coinsf - coinsu, hp, dmg, (int)v);
        for (int i = 0; i < creaturen; i++)
        {
            if (creatures[i].spaceabrev == spaceabrev)
            {
                printf("%i: x%i y%i\n", creatures[i].a, creatures[i].where - creatures[i].where/w*w, creatures[i].where/w);
            }
        }
        printf("%i\n", coins[coinsf-1].spaceabrev);
        //for (int i = 0; i < founditemsn; i++) {if ((items[founditems[i].id].found - items[founditems[i].id].used) > 0) {printf("%s", items[founditems[i].id].name[0]);}}

        moving();
    }
}


void moving()
{
    int input_size = 0;
    while (1)
    {
        input[input_size] = getchar();
        if (input[input_size] == 10 || input_size >= 10)
        {input[input_size] = '\0'; break;}
        input_size++;
    }
    move = input[0];
    int walked = 0;

    for (int i = 0; i < movesetnum; i++)
    {
        if (move == moveset[i].key || move == moveset[i].key + ('A'-'a'))
        {
            *moveset[i].axis += moveset[i].mod;
            moven = i;
            walked = 1;
            break;
        }
    }
    if (!walked)
    {
        input_effect(input_size);
    }
}

int rprint()
{
    system("clear");
    FILE *spacef = NULL;
    spacef = fopen(fnbuffer, "r");
    if (spacef == NULL) {
        FILE *template = fopen("template.txt", "r");
        FILE *brand_new_space = fopen(fnbuffer, "w");
        char buffer = ' ';

        while(1) {
            if (!fread(&buffer, sizeof(char), 1, template)) {break;}
            fwrite(&buffer, sizeof(char), 1, brand_new_space);
        }
        fclose(template);
        fclose(brand_new_space);
        spacef = fopen(fnbuffer, "r");
    }

    int counter = 0;
    char current = ' ';

    //if (!space.in && space.z >= 50) {v += 2;}
    while (1)
    {
        if (fread(&current, 1, 1, spacef))
        {
            printf("%c", current);
        }
        else {printf("\n\n"); break;}

        if (printcheck(counter, current))
        {
            rprint();
            break;
        }
        counter++;
    }
    //if (!space.in && space.z >= 50) {v -= 2;}
    fclose(spacef);
    return counter;
}

//checks if current character is the one of a found coin or item and blanks it out if so
//also lots of other checks
//when this returns a truthy value the room is printed again (because it changed something)
//otherwise it does something and the print continues
int printcheck(int counter, char current)
{
    //found coin check
    if (current == '$')
    {
        for (int i = 0; i < coinsf; i++)
        {
            if (coins[i].where == counter && coins[i].spaceabrev == spaceabrev)
            {
                current = ' ';
                printf("%c ", 8);
                break;
            }
        }
    }
    //found item check
    for (int i = 0; i < founditemsn; i++)
    {
        if (founditems[i].where == counter && founditems[i].spaceabrev == spaceabrev)
        {
            current = ' ';
            printf("%c ", 8);
        }
    }
    //new creature check
    if (current > '0' && current <= '9')
    {
        printf("%c ", 8);
        int new = 1;
        for (int i = 0; i < creaturen; i++)
        {
            if (creatures[i].spaceabrev == spaceabrev && creatures[i].counter == counter)
            {new = 0; break;}
        }
        if (new)
        {
            creatures[creaturen].spaceabrev = spaceabrev;
            creatures[creaturen].counter = counter;
            creatures[creaturen].where = counter;
            creatures[creaturen].a = current - '0';
            creaturen++;
        }
    }

    //player checks
    if (counter == w * y + x)
    {
        //path, go outside or inside
        if (current == 'p')
        {
            if (space.in == 1) {space.in = 0; *moveset[moven].axis+=moveset[moven].mod;}
            else {space.in = 1; *moveset[moven].axis+=moveset[moven].mod;}
            newspace(fnbuffer);
            return 1;
        }
        //space edge
        else if (current == '*')
        {
            int modd = moveset[moven].mod;
            //do not judge my weird little goblin equations
            if (moveset[moven].axis == &x) {space.x += modd / 2; x = ((modd - 2)*(6 - w) + 8)/4;}
            else if (moveset[moven].axis == &y) {space.y += modd; y = ((modd - 1)/2)*(3 - h) + 1;}
            newspace(fnbuffer);
            return 1;
        }
        //path up on z axis
        else if (current == 'u')
        {
            space.z++;
            *moveset[moven].axis+=moveset[moven].mod;
            newspace(fnbuffer);
            return 1;
        }
        //path down on z axis
        else if (current == 'd')
        {
            space.z--;
            *moveset[moven].axis+=moveset[moven].mod;
            newspace(fnbuffer);
            return 1;
        }
        //some kind of wall or creature, undo
        else if (current == '-' || current == '|' || current == '+' || current == '#' || current == '\\' || current == '_')
        {
            *moveset[moven].axis -= moveset[moven].mod;
            return 1;
        }
        //new coin check
        else if (current == '$')
        {
            coins[coinsf].where = counter;
            coins[coinsf].spaceabrev = spaceabrev;
            coinsf++;
        }
        //trader check
        else if (current == 'T')
        {
            //função para trading
            if (trading())
            {return 1;}
        }
        //stepped over creature
        for (int i = 0; i < creaturen; i++)
        {
            //player on creature
            if (creatures[i].where == counter && creatures[i].spaceabrev == spaceabrev && creatures[i].a > 0)
            {
                creatures[i].a -= dmg;
                *moveset[moven].axis -= moveset[moven].mod;
                return 1;
            }
        }
        //new item check
        for (int i = 0; i < itemsn; i++)
        {
            if (current == items[i].key)
            {
                if (items[i].bright && activelsource.item == i) {activelsource.bright = 0;}
                founditems[founditemsn].spaceabrev = spaceabrev;
                founditems[founditemsn].where = counter;
                founditems[founditemsn].id = i;
                founditemsn++;
                items[i].found++;
                if (items[i].dmg > dmg) {dmg = items[i].dmg;}
                else if (items[i].bright > v) {v = items[i].bright; return 1;}
            }
        }
        printf("%co", 8);
    }

    //existing creature check
    for (int i = 0; i < creaturen; i++)
    {
        if (creatures[i].spaceabrev == spaceabrev && creatures[i].where == counter && creatures[i].a > 0)
        {
            //move creature back if entering wall or player or creature
            int goback = 0;
            if (current == '-' || current == '|' || current == '+' || current == '#' || current == '\\' || current == '_' ||
                current == 'p' || current == 'u' || current == 'd')
            {goback = 1;}
            for (int j = 0; j < creaturen; j++)
            {
                if (i - j && creatures[j].spaceabrev == spaceabrev && creatures[j].where == counter && creatures[j].a > 0)
                {goback = 1; break;}
            }
            if (goback)
            {
                if (moveset[creatures[i].moven].key == 's' || moveset[creatures[i].moven].key == 'w')
                {
                    creatures[i].where -= moveset[creatures[i].moven].mod*w;
                    if (creatures[i].where - creatures[i].where/w*w < x)
                    {creatures[i].where += 2;}
                    else if (creatures[i].where - creatures[i].where/w*w > x)
                    {creatures[i].where -= 2;}
                }
                else if (moveset[creatures[i].moven].key == 'a' || moveset[creatures[i].moven].key == 'd')
                {
                    creatures[i].where -= moveset[creatures[i].moven].mod;
                    if (creatures[i].where/w < y)
                    {creatures[i].where += w;}
                    else if (creatures[i].where/w > y)
                    {creatures[i].where -= w;}
                }
                return 1;
            }
            //print creatures action value(? you know, just 'a')
            printf("%c%i", 8, creatures[i].a);
            break;
        }
    }

    //shadows //idea to make shadows a circle function :)did it . also lsource shadows
    int cy = counter / w, cx = counter - cy * w;
    int lx = activelsource.x, ly = activelsource.y;
    int lv = activelsource.bright;
    if  ((cy < h && cx < w - 1 && ((cx - x)*(cx - x) / 4 + (cy - y)*(cy - y) > v * v)) &&
        ((!lv) || (lv && ((cx - lx)*(cx - lx) / 4 + (cy - ly)*(cy - ly) > lv * lv))))
    {
            printf("%c#", 8);
    }
    return 0;
}

//number of characters in the buffer is spaces/int,int,int.txt, so 7+3*(2)+2+4(+1)=18
//concatenation function for making the space file name string in format: spaces/SPACENAME.txt
char *newspace(char *buffer)
{
    char *templa = "spaces/";
    char *templb = ".txt";
    int counter = 0;

    spaceabrev = space.z + space.y * 100 + space.x * 10000 + space.in * 1000000;
    activelsource.bright = 0;
    for (int i = 0; i < lsourcen; i++)
    {
        if (lsources[i].spaceabrev == spaceabrev && (!(lsources[i].item + 1) || items[lsources[i].item].found == 0))
        {
            activelsource = lsources[i];
        }
    }

    while (templa[counter])
    {
        buffer[counter] = templa[counter];
        counter++;
    }
    int split = counter;
    for (int i = 0; i < 3; i++)
    {
        buffer[counter] = *(&space.x + i) / 10 + '0' ;
        counter++;
        buffer[counter] = *(&space.x + i) - (*(&space.x + i) / 10 * 10) + '0';
        counter++;
        buffer[counter] = ',';
        counter++;
    }
    if (space.in) {buffer[counter] = '1';}
    else {buffer[counter] = '0';}
    counter++;

    split = counter;
    while (templb[counter - split])
    {
        buffer[counter] = templb[counter - split];
        counter++;
    }
    buffer[counter] = '\0';

    return buffer;
}

//idea for specific function that returns spaces/5050500.txt string
//                               but also subs/0/5050500.txt string
char *dirspacetxt(char *prefix, char *buffer)
{
    char *txt = ".txt";
    int counter = 0;

    while (prefix[counter])
    {
        buffer[counter] = prefix[counter];
        counter++;
    }
    int split = counter;
    for (int i = 0; i < 3; i++)
    {
        buffer[counter] = *(&space.x + i) / 10 + '0' ;
        counter++;
        buffer[counter] = *(&space.x + i) - (*(&space.x + i) / 10 * 10) + '0';
        counter++;
        buffer[counter] = ',';
        counter++;
    }
    if (space.in) {buffer[counter] = '1';}
    else {buffer[counter] = '0';}
    counter++;

    split = counter;
    while (txt[counter - split])
    {
        buffer[counter] = txt[counter - split];
        counter++;
    }
    buffer[counter] = '\0';

    return buffer;
}

//função para trading
int trading()
{
    char sellorbuy = 'b';
    while (1)
    {
        system("clear");
        printf("Howdy trav'ler. do my products peak your interest?\n\n");

        for (int i = 0; i < itemsn; i++)
        {
            if (items[i].price)
            {printf("%c - %s: %i coins\n", items[i].key, items[i].name[lang], items[i].price);}
        }
        printf("\nn - no\nN - no(rude)\n\ncoins you have: %i\nitems you have:\n", coinsf - coinsu);
        for (int i = 0; i < itemsn; i++)
        {
            if (items[i].found > items[i].used)
            {
                printf("%s: %i\n", items[i].name[lang], items[i].found - items[i].used);
            }
        }

        //get input
        move = getchar();
        while (getchar() != 10) {}

        if (move == 'n' || move == 'N')
        {*moveset[moven].axis -= moveset[moven].mod; return 1; break;}

        for (int i = 0; i < itemsn; i++)
        {
            if (move == items[i].key && coinsf - coinsu >= items[i].price)
            {
                coinsu += items[i].price;
                items[i].found++;
                break;
            }
        }
    }
}

//moves, attacks, etc for the creatures
int creaturemoving()
{
    int cx = 0;
    int cy = 0;
    for (int i = 0; i < creaturen; i++)
    {
        if (creatures[i].spaceabrev == spaceabrev && creatures[i].a > 0)
        {
            //creatures x and y, creatures x and y in relation to player
            cx = creatures[i].where - creatures[i].where/w*w;
            cy = creatures[i].where/w;
            int cxx = (cx - x)/2, cyy = cy - y;

            //creature adjacent to player, attack
            if ((cx == x && (cy - y)*(cy - y) == 1) || (cy == y && (cx - x)*(cx - x) == 4))
            {
                hp -= creatures[i].a;
                if (!(hp > 0)) {return 1;}
            }

            //creature moving towards player
            else if (cyy >= cxx && cyy >= (-1)*cxx && !(cx == x && cy == y + 1)) {creatures[i].where -= w; creatures[i].moven = 0;}
            else if (cyy <= cxx && cyy <= (-1)*cxx && !(cx == x && cy == y - 1)) {creatures[i].where += w; creatures[i].moven = 1;}
            else if (cyy <= cxx && cyy >= (-1)*cxx && !(cy == y && cx == x + 2)) {creatures[i].where -= 2; creatures[i].moven = 2;}
            else if (cyy >= cxx && cyy <= (-1)*cxx && !(cy == y && cx == x - 2)) {creatures[i].where += 2; creatures[i].moven = 3;}

            //check if entered another creature
            for (int j = 0; j < creaturen; j++)
            {
                if (i - j && creatures[j].spaceabrev == spaceabrev && creatures[i].where == creatures[j].where  && creatures[j].a > 0)
                {
                    if (moveset[creatures[i].moven].key == 's' || moveset[creatures[i].moven].key == 'w')
                    {
                        creatures[i].where -= moveset[creatures[i].moven].mod*w;
                        if (creatures[i].where - creatures[i].where/w*w < x)
                        {creatures[i].where += 2;}
                        else if (creatures[i].where - creatures[i].where/w*w > x)
                        {creatures[i].where -= 2;}
                    }
                    else if (moveset[creatures[i].moven].key == 'a' || moveset[creatures[i].moven].key == 'd')
                    {
                        creatures[i].where -= moveset[creatures[i].moven].mod;
                        if (creatures[i].where/w < y)
                        {creatures[i].where += w;}
                        else if (creatures[i].where/w > y)
                        {creatures[i].where -= w;}
                    }
                break;
                }
            }
        }
    }

    return 0;
}

//special effects and attacks and whatnot
int input_effect(int input_size)
{
    if (input[0] == 'e')
    {
        for (int i = 0, id = 0; i < founditemsn; i++)
        {
            id = founditems[i].id;
            if ((items[id].found - items[id].used > 0) && items[id].heal)
            {
                hp += items[id].heal;
                items[id].used++;
                break;
            }
        }

        return 1;
    }

    //arrows and fire bolt should be connected by being projectiles eventually
    if (input[0] == 'm')
    {
        if (input_size == 7 && input[1] == 'f' && input[2] == 'r')
        {
            if (input[3] == 'b' && input[4] == 'l' && input[5] == 't')
            {
                for (int i = 0; i < creaturen; i++)
                {
                    if (creatures[i].spaceabrev && creatures[i].a > 0)
                    {
                        int cy = creatures[i].where / w;
                        int cx = creatures[i].where - cy * w;
                        if (cx == x && cy < y && input[6] == 'w') {creatures[i].a -= 1;}
                        else if (cx == x && cy > y && input[6] == 's') {creatures[i].a -= 1;}
                        else if (cy == y && cx < x && input[6] == 'a') {creatures[i].a -= 1;}
                        else if (cy == y && cx > x && input[6] == 'd') {creatures[i].a -= 1;}
                    }
                }
            }
        }

        return 1;
    }

    //not working after first hit
    if (input[0] == 'b' && items[5].found > items[5].used && items[6].found > items[6].used)
    {
        int hitcreature_distances[creaturen][2];
        int hitcreaturen = 0;
        int closest_creature = 0;
        for (int i = 0; i < creaturen; i++)
        {
            if (creatures[i].spaceabrev && creatures[i].a > 0)
            {
                int cy = creatures[i].where / w;
                int cx = creatures[i].where - cy * w;
                if      (cx == x && cy < y && input[1] == 'w') {
                    hitcreature_distances[hitcreaturen][0] = y - cy;
                    hitcreature_distances[hitcreaturen][1] = i;
                    hitcreaturen++;
                }
                else if (cx == x && cy > y && input[1] == 's') {
                    hitcreature_distances[hitcreaturen][0] = cy - y;
                    hitcreature_distances[hitcreaturen][1] = i;
                    hitcreaturen++;
                }
                else if (cy == y && cx < x && input[1] == 'a') {
                    hitcreature_distances[hitcreaturen][0] = x - cx;
                    hitcreature_distances[hitcreaturen][1] = i;
                    hitcreaturen++;
                }
                else if (cy == y && cx > x && input[1] == 'd') {
                    hitcreature_distances[hitcreaturen][0] = cx - x;
                    hitcreature_distances[hitcreaturen][1] = i;
                    hitcreaturen++;
                }
            }
        }
        for (int i = 0; i < hitcreaturen; i++)
        {
            if (hitcreature_distances[i][0] < hitcreature_distances[closest_creature][0])
            {
                closest_creature = i;
            }
        }
        if (hitcreaturen) {creatures[closest_creature].a -= 2; items[6].used++;}

        return 1;
    }

    return 0;
}