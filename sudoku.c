#include <stdio.h>
#include <stdlib.h>
// #include <conio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>

int _read_key(void);

void draw_map(int map[][9], int x, int y)
{
    printf("|");
    for (int y_ = 0; y_ < 3; y_++) {
        for (int i = 0; i < 9 * 3 + 2; i++)
        {
            printf("—");
        }
        printf("|\n|");
        for (int yy = 0; yy < 3; yy++) {
            for (int x_ = 0; x_ < 3; x_++) {
                for (int xx = 0; xx < 3; xx++) {
                    
                    char p = ' ';
                    if (map[yy+y_*3][xx+x_*3] != -1) {
                        p = map[yy+y_*3][xx+x_*3] + '0';
                    }
                    
                    if ((yy+y_*3) == y && (xx+x_*3) == x) {
                        printf("[%c]", p);
                    }
                    else {
                        printf(" %c ", p);
                    }
                }
                printf("|");
            }
            printf("\n|");
        }
    }
    for (int i = 0; i < 9 * 3 + 2; i++)
    {
        printf("—");
    }
    printf("|\n");
}

_Bool check_to_place(int map[][9], int x, int y, int num) {
    for (int yy = 0; yy < 9; yy++) {
        if (map[yy][x] == num) {
            return 0;
        }
    }
    for (int xx = 0; xx < 9; xx++) {
        if (map[y][xx] == num) {
            return 0;
        }
    }
    for (int yy = 0; yy<3; yy++) {
        for (int xx = 0; xx<3; xx++) {
            //printf("[%i, %i] ", yy+(y/3)*3, xx+(x/3)*3);
            if (map[yy+(y/3)*3][xx+(x/3)*3] == num) {
                return 0;
            }
        }
    }
    return 1;
}

int recurse_xod(int map[][9], int solve[][9]) {
    int solutions = 0;
    
    int map_[9][9];
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            map_[y][x] = map[y][x];
        }
    }
    
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            if (map[y][x] == -1) {
                for (int t = 1; t <= 9; t++) {
                    if (check_to_place(map_, x, y, t)) {
                        map_[y][x] = t;
                        //draw_map(map_, x, y);
                        solutions += recurse_xod(map_, solve);
                        map_[y][x] = -1;
                    }
                }
                return solutions;
            }
        }
    }
    //draw_map(map, -1, -1);
    //system("pause");
    
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            solve[y][x] = map_[y][x];
        }
    }
    
    return 1;
}

void rand_map(int map[][9], int solve[][9]) {
    int map_[9][9];
    do {
        for (int y = 0; y < 9; y++)
        {
            for (int x = 0; x < 9; x++)
            {
                map_[y][x] = -1;
            }
        }
        
        for (int i = 0; i < 81; i++) {
            int x = rand()%9, y = rand()%9, t = rand()%9 + 1;
            if (check_to_place(map_, x, y, t)) {
                map_[y][x] = t;
            }
        }
        
    } while (recurse_xod(map_, solve) != 1);
    
    //printf("Finded\n");
    
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            map[y][x] = map_[y][x];
        }
    }
}

int main()
{
    int map[9][9], right_choice[9][9];
    
    srand((int)time(NULL));
    
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            map[y][x] = -1;
        }
    }
    rand_map(map, right_choice);
    
    int x = 0, y = 0, xp = 3;
    while (1)
    {
        printf("XP: %i\n", xp);
        //draw_map(right_choice, x, y);
        draw_map(map, x, y);

        char tmp = _read_key();
        system("clear");
        if (tmp == 'a')
        {
            if (x-1 >= 0) {
                x--;
            }
        }
        else if (tmp == 'd')
        {
            if (x+1 < 9) {
                x++;
            }
        }
        else if (tmp == 'w')
        {
            if (y-1 >= 0) {
                y--;
            }
        }
        else if (tmp == 's')
        {
            if (y+1 < 9) {
                y++;
            }
        }
        else if ('1' <= tmp && tmp <= '9') {
            int t = tmp - '0';
            if (right_choice[y][x] == t) {
                map[y][x] = t;
            }
            else {
                xp--;
            }
        }
        else if (tmp == '0')
        {
            map[y][x] = -1;
        }
        if (tmp == '\n' || xp <= 0)
        {
            break;
        }
    }
    
    /*
    system("cls");
    printf("%i\n", recurse_xod(map, map));
    //*/
    
    printf("Game over\n");
    return 0;
}

int _read_key(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        return ch;  // Возвращаем ASCII‑код нажатой клавиши
    }

    return 0;  // Ничего не нажато
}
