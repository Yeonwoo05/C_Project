#include<stdio.h>
#include<windows.h>
#include<time.h>
#pragma warning(disable : 4996)


#define LEFT 75 //�·� �̵�    //Ű���尪�� 
#define RIGHT 77 //��� �̵� 
#define UP 72 //���� �̵�
#define DOWN 80 //�Ʒ��� �̵�
#define SPACE 32 // Ȯ��
#define p 112 //�Ͻ����� 
#define P 80 //�Ͻ�����
#define ESC 27 //�������� 

#define false 0
#define true 1

#define MAIN_X 23 //������ ����ũ�� 
#define MAIN_Y 23 //������ ����ũ�� 
#define MAIN_X_ADJ 3 //������ ��ġ���� 
#define MAIN_Y_ADJ 3 //������ ��ġ���� 

#define WHITE 1 //��� �ٵϵ�
#define BLACK 2 //������ �ٵϵ�
//FILE* fp;

int difficulty;
int sum, win_sum;
int color = 0;
int bot_color = 0;
int x, y; //�̵����� Ű�� �����ǻ��� x,y��ǥ�� ����
int key; //Ű����� �Է¹��� Ű���� ���� 
int turn_cnt = 0;       // �����ϼ��� ����
int title(void);            // ���ӽ���ȭ�� 
void reset(void);           // ������ ����
void draw_map(void);        // ���� �ʸ� ǥ�� 
void draw_Interface(void);  // �������̽��� �׸� 
void upload_Interface(int bot_player);    // �������̽� ������Ʈ
void check_key(int bot_player, int* x, int* y);       // Ű����� Ű�� �Է¹��� 
void rule();
int win_if();
void win_lose_print(int win_lose);
void bot_chose();
int bot_attack();
int bot_defence();
int bot_player_check();
int main_cnt = 0;

int block[MAIN_X][MAIN_Y] = { 0 };
int bot_block[MAIN_X][MAIN_Y] = { 0 };      // ����ġ ���� �迭
int block_cnt = 0;


void gotoxy(int x, int y) { //gotoxy�Լ� 
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //Ŀ������� �Լ��� ���Ǵ� ������ 
void setcursortype(CURSOR_TYPE c) { //Ŀ������� �Լ� 
    CONSOLE_CURSOR_INFO CurInfo;

    switch (c) {
    case NOCURSOR:
        CurInfo.dwSize = 1;
        CurInfo.bVisible = FALSE;
        break;
    case SOLIDCURSOR:
        CurInfo.dwSize = 100;
        CurInfo.bVisible = TRUE;
        break;
    case NORMALCURSOR:
        CurInfo.dwSize = 20;
        CurInfo.bVisible = TRUE;
        break;
    }
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}
int main()
{
    srand(time(NULL));
    setcursortype(NOCURSOR); //Ŀ�� ���� 

    int main_robe;
    while (true)
    {
        system("cls");
        main_cnt++;
        main_robe = title(); //����Ÿ��Ʋ ȣ�� 

        if (main_robe == 1)
        {
            reset();
        }
        else if (main_robe == 2)
        {
            rule();
        }
        else if (main_robe == 3)
        {
            system("cls");
            break;
        }
    }
    return 0;

}


void draw_map()
{
    setcursortype(NOCURSOR);
    int x = 3;
    int y = 3;
    int MAX_X = MAIN_X + 1;
    int MAX_Y = MAIN_Y + 1;
    system("cls");
    for (int i = -2; i <= MAX_X; i++)
    {
        for (int j = -2; j <= MAX_Y; j++)
        {
            gotoxy(x + i, y + j);
            if (i == -2 && j == -2)
            {
                printf("��");
            }
            else if (i == -2 && j == MAX_Y)
            {
                printf("��");
            }
            else if (i == MAX_X && j == -2)
            {
                printf("��");
            }
            else if (i == MAX_X && j == MAX_Y) // �ٵ��� ���
            {
                printf("��");
            }
            else if (i == -2 || i == MAX_X)
            {
                printf("��");
            }
            else if (j == -2 || j == MAX_Y)
            {
                printf("��");
            }

            else if (i == -1 && j == -1)
            {
                printf("��");
            }
            else if (i == -1 && j == MAIN_Y)
            {
                printf("��");
            }
            else if (i == MAIN_X && j == -1)
            {
                printf("��");
            }
            else if (i == MAIN_X && j == MAIN_Y) // �ٵ��� ���
            {
                printf("��");
            }
            else if (i == -1)
            {
                printf("��");
            }
            else if (j == -1)
            {
                printf("��");
            }
            else if (i == MAIN_X)
            {
                printf("��");
            }
            else if (j == MAIN_Y)
            {
                printf("��");
            }
            else
            {
                printf("��");
            }
        }
    }
}

void reset()
{
    for (int i = 0; i < MAIN_X; i++)
    {
        for (int j = 0; j < MAIN_Y; j++)
        {
            block[i][j] = 0;
            bot_block[i][j] = 0;
        }
    }

    turn_cnt = 0;

    int x, y;
    x = MAIN_X / 2;
    y = MAIN_Y / 2;
    int win_lose = 0;
    int bot_player;
    bot_player = bot_player_check();
    color = 2;
    bot_color = 1;
    main_cnt++;
    draw_map();
    draw_Interface();
    while (1)
    {

        upload_Interface(bot_player);
        check_key(bot_player, &x, &y);
        //game_rule();
        if (bot_player == 2)
        {
            Sleep(rand() % 500);
            bot_chose();
        }
        win_lose = win_if();
        if (win_lose == 1)
        {
            break;
        }
        else if (win_lose == -1)
        {
            break;
        }

    }

    win_lose_print(win_lose);
}


int title()// Ÿ��Ʋ
{
    int x = 21; //Ÿ��Ʋȭ���� ǥ�õǴ� x��ǥ 
    int y = 4; //Ÿ��Ʋȭ���� ǥ�õǴ� y��ǥ 
    int check = 1;
    if (main_cnt == 1)
    {
        gotoxy(x + 5, y + 2); printf("C");              Sleep(150);     // ���� ���� �� ������� �ΰ�
        gotoxy(x + 5, y + 2); printf("C ��");            Sleep(150);
        gotoxy(x + 5, y + 2); printf("C �� N");          Sleep(150);
        gotoxy(x + 5, y + 2); printf("C �� N C");        Sleep(150);
        gotoxy(x + 5, y + 2); printf("C �� N C A");      Sleep(150);
        gotoxy(x + 5, y + 2); printf("C �� N C A V");    Sleep(150);
        gotoxy(x + 5, y + 2); printf("C �� N C A V E");  Sleep(150);
        for (int i = 0; i < 30; i++)
        {
            gotoxy(x + 6, i - 1);
            if (i != 7) printf("  ");
            gotoxy(x + 6, i);
            if (i < 7) printf("��");
            else printf("��");
            Sleep(50);
        }
        gotoxy(x + 6, 29);
        printf("  ");
    }
    
    gotoxy(x + 5, y + 2); printf("C �� N C A V E");;

    y = 10;
    x = 26;

    gotoxy(x, y - 1); printf("������������������������"); //����â
    gotoxy(x, y + 1); printf("������������������������");
    while (true)
    {

        gotoxy(27, 10); printf("S T A R T"); //����
        gotoxy(27, 12); printf("DIFFICULT"); // ���̵�
        gotoxy(27, 14); printf("E X   I T"); //������
        key = getch();

        if (key == SPACE) //check���� ���� ���� �Լ��� ���� ��ȯ
        {
            return check;
        }
        else if (key == DOWN && check < 3)
        {
            check++; //check�� 1����
            y += 2; //��ǥ�� 2����
            gotoxy(x, y - 3); printf("            ");
            gotoxy(x, y - 1); printf("������������������������");
            gotoxy(x, y + 1); printf("������������������������");
        }
        else if (key == UP && check > 1)
        {
            check--; //check�� 1����
            y -= 2; //��ǥ�� 2����
            gotoxy(x, y + 3); printf("            ");
            gotoxy(x, y - 1); printf("������������������������");
            gotoxy(x, y + 1); printf("������������������������");
        }

    }
}

void check_key(int bot_player, int* x, int* y) //�÷��̾��� ���ÿ� ���� x, y�� ����
{
    int key; // ���� ������ Ű�� �����ϴ� ����


    while (true)
    {
        gotoxy(MAIN_X + 13, 17); printf("                         ");

        key = getch(); //�Է�

        if (key == DOWN && MAIN_Y + 2 > *y) // key == DOWN : Ű�� �Ʒ� ȭ��ǥ�� ���ٸ�
        {                                   // MAIN_Y + 2 > *y : ����â�� ����� ���ϰ� �Ѵ�
            (*y)++;
            gotoxy(*x, *y - 1); printf("%s", (block[*x - 3][*y - 4] == 0) ? "�� " // �̵��ϸ� �������µ� �ڿ��� �����̿������� �Ǵ��Ͽ� �ٽ� ���
                : (block[*x - 3][*y - 4] == 1) ? "��" : "��");
        }
        else if (key == UP && 3 < *y)
        {
            (*y)--;
            gotoxy(*x, *y + 1); printf("%s", (block[*x - 3][*y - 2] == 0) ? "�� " // �̵��ϸ� �������µ� �ڿ��� �����̿������� �Ǵ��Ͽ� �ٽ� ���
                : (block[*x - 3][*y - 2] == 1) ? "��" : "��");;
        }

        else if (key == RIGHT && MAIN_X + 2 > *x)
        {
            (*x)++;
            gotoxy(*x - 1, *y); printf("%s", (block[*x - 4][*y - 3] == 0) ? "�� " // �̵��ϸ� �������µ� �ڿ��� �����̿������� �Ǵ��Ͽ� �ٽ� ���
                : (block[*x - 4][*y - 3] == 1) ? "��" : "��");;
        }
        else if (key == LEFT && 3 < *x)
        {
            (*x)--;
            gotoxy(*x + 1, *y); printf("%s", (block[*x - 2][*y - 3] == 0) ? "�� " // �̵��ϸ� �������µ� �ڿ��� �����̿������� �Ǵ��Ͽ� �ٽ� ���
                : (block[*x - 2][*y - 3] == 1) ? "��" : "��");
        }
        else if (key == ESC)
        {
            main();
        }

        if (bot_player == 2)
        {
            gotoxy(*x, *y); (color == 1) ? printf("��") : printf("��"); // Ŀ�� ��ġ�� Ȯ���ϴ� �Ǻ���
            if (key == SPACE && block[*x - 3][*y - 3] == 0)
            {
                block[*x - 3][*y - 3] = color; //�迭�� ��ġ ����
                break;
            }
        }

        else if (bot_player == 1)
        {
            gotoxy((*x), (*y)); (turn_cnt % 2 == 0) ? printf("��") : printf("��"); // Ŀ�� ��ġ�� Ȯ���ϴ� �Ǻ���
            if (key == SPACE && block[*x - 3][*y - 3] == 0)
            {
                block[*x - 3][*y - 3] = (turn_cnt % 2 == 0) ? 1 : 2; //�迭�� ��ġ ����
                break;
            }
        }
    }
}

void rule()
{

    system("cls");
    int x = 21; //Ÿ��Ʋȭ���� ǥ�õǴ� x��ǥ 
    int y = 3; //Ÿ��Ʋȭ���� ǥ�õǴ� y��ǥ 
    int check = 0;

    gotoxy(x + 4, y); printf("D I F F I C U L T");
    int cnt = 0;
    y = 7;
    x = 26;
    gotoxy(x, y + 1); printf("����������������������");
    gotoxy(x, y + 3); printf("����������������������");

    while (true)
    {

        gotoxy(27, 9);   printf("E A  S Y");
        gotoxy(27, 11);  printf("ME DI UM");
        gotoxy(27, 13);  printf("H A  R D");
        gotoxy(x - 1, 9 + difficulty * 2); printf("��");
        gotoxy(x + 6, 9 + difficulty * 2); printf("��");
        key = getch();

        if (key == SPACE && cnt == 1) //check���� ���� ���� �Լ��� ���� ��ȯ
        {
            difficulty = check - 1;
        }
        else if (key == DOWN && check < 3)
        {
            cnt = 1;
            check++; //check�� 1����
            y += 2; //��ǥ�� 2����
            gotoxy(x, y - 3); printf("            ");
            gotoxy(x, y - 1); printf("����������������������");
            gotoxy(x, y + 1); printf("����������������������");
        }
        else if (key == UP && check > 1)
        {
            check--; //check�� 1����
            y -= 2; //��ǥ�� 2����
            gotoxy(x, y + 3); printf("            ");
            gotoxy(x, y - 1); printf("����������������������");
            gotoxy(x, y + 1); printf("����������������������");
        }
        else if (key == LEFT) // Ű�� �޾��� �� ���� ����Ű��� return�� ���� main���� ���ư���
        {
            return;
        }
        gotoxy(x - 1, 9); printf("  ");
        gotoxy(x + 6, 9); printf("  ");
        gotoxy(x - 1, 11); printf("  ");
        gotoxy(x + 6, 11); printf("  ");
        gotoxy(x - 1, 13); printf("  ");
        gotoxy(x + 6, 13); printf("  ");
        gotoxy(x - 1, 9 + difficulty * 2); printf("��");
        gotoxy(x + 6, 9 + difficulty * 2); printf("��");

    }
}

void draw_Interface() // �������̽� �׸��°� 
{
    int x = MAIN_X + 11;
    int y = 7;
    gotoxy(x + 0, y - 4);  printf("    ������������������������������������������������������������");
    gotoxy(x + 0, y - 3);  printf("     �¡� CURRENT TURN [    ] �ߢ�  ");
    gotoxy(x + 0, y - 2);  printf("    ������������������������������������������������������������");
    gotoxy(x + 0, y + 0);  printf("O�������������������O");
    gotoxy(x + 0, y + 1);  printf(":                                    :");
    gotoxy(x + 0, y + 2);  printf(":                                    :");
    gotoxy(x + 0, y + 3);  printf(":        NOWTURN [          ]        :");
    gotoxy(x + 0, y + 4);  printf(":                                    :");
    gotoxy(x + 0, y + 5);  printf(":                                    :");
    gotoxy(x + 0, y + 6);  printf(":                                    :");
    gotoxy(x + 0, y + 7);  printf(":                                    :");
    gotoxy(x + 0, y + 8);  printf(":                                    :");
    gotoxy(x + 0, y + 9);  printf(":                                    :");
    gotoxy(x + 0, y + 10); printf(":                                    :");
    gotoxy(x + 0, y + 11); printf(":                                    :");
    gotoxy(x + 0, y + 12); printf(":                                    :");
    gotoxy(x + 0, y + 13); printf(":                                    :");
    gotoxy(x + 0, y + 14); printf(":                                    :");
    gotoxy(x + 0, y + 15); printf(":                                    :");
    gotoxy(x + 0, y + 16); printf(":                                    :");
    gotoxy(x + 0, y + 17); printf(":                                    :");
    gotoxy(x + 0, y + 18); printf("O�������������������O");
}


void upload_Interface(int bot_player) //���� �� ������ �������̽��� ������ ���ε�
{
    int x = MAIN_X + 13;
    int y = 7;
    gotoxy(x + 10, y - 3); printf(" %02d", ++turn_cnt);

    gotoxy(x + 8, y + 3); (turn_cnt % 2 == 0) ? printf("WHITE") : printf("BLACK");
    if (bot_player == 2)
    {
        gotoxy(x + 2, y + 7); printf("DIFFICULT [ %s ]", (difficulty == 0) ? "E A  S Y" : (difficulty == 1) ? "ME DI UM" : "H A  R D");
    }
}

int win_if() // �̱�� ����� ��
{
    int cnt = 0;
    for (int i = 0; i < MAIN_X; i++)
    {
        for (int j = 0; j < MAIN_Y; j++)
        {
            if ((block[i][j] == color
                && block[i][j + 1] == color
                && block[i][j + 2] == color
                && block[i][j + 3] == color
                && block[i][j + 4] == color
                && block[i][j + 5] != color
                && block[i][j - 1] != color) ||

                (block[i][j] == color
                    && block[i + 1][j] == color
                    && block[i + 2][j] == color
                    && block[i + 3][j] == color
                    && block[i + 4][j] == color
                    && block[i + 5][j] != color
                    && block[i - 1][j] != color) ||

                (block[i][j] == color
                    && block[i + 1][j + 1] == color
                    && block[i + 2][j + 2] == color
                    && block[i + 3][j + 3] == color
                    && block[i + 4][j + 4] == color
                    && block[i + 5][j + 5] != color
                    && block[i - 1][j - 1] != color) ||

                (block[i][j] == color
                    && block[i + 1][j - 1] == color
                    && block[i + 2][j - 2] == color
                    && block[i + 3][j - 3] == color
                    && block[i + 4][j - 4] == color
                    && block[i + 5][j - 5] != color
                    && block[i - 1][j + 1] != color))
            {
                return 1;
            }
            else if ((block[i][j] == bot_color
                && block[i][j + 1] == bot_color
                && block[i][j + 2] == bot_color
                && block[i][j + 3] == bot_color
                && block[i][j + 4] == bot_color
                && block[i][j + 5] != bot_color
                && block[i][j - 1] != bot_color) ||

                (block[i][j] == bot_color
                    && block[i + 1][j] == bot_color
                    && block[i + 2][j] == bot_color
                    && block[i + 3][j] == bot_color
                    && block[i + 4][j] == bot_color
                    && block[i + 5][j] != bot_color
                    && block[i - 1][j] != bot_color) ||

                (block[i][j] == bot_color
                    && block[i + 1][j + 1] == bot_color
                    && block[i + 2][j + 2] == bot_color
                    && block[i + 3][j + 3] == bot_color
                    && block[i + 4][j + 4] == bot_color
                    && block[i + 5][j + 5] != bot_color
                    && block[i - 1][j - 1] != bot_color) ||
                (block[i][j] == bot_color
                    && block[i + 1][j - 1] == bot_color
                    && block[i + 2][j - 2] == bot_color
                    && block[i + 3][j - 3] == bot_color
                    && block[i + 4][j - 4] == bot_color
                    && block[i + 5][j - 5] != bot_color
                    && block[i - 1][j + 1] != bot_color))
            {
                return -1;
            }
        }
    }
}

void win_lose_print(int win_lose)  // �̰��� �� ������ â
{

    x = 27;
    y = 10;
    gotoxy(x - 5, y - 2); printf("����������������������������������������������������");
    gotoxy(x - 5, y - 1); printf("                           ");
    gotoxy(x - 5, y + 1); printf("                           ");
    gotoxy(x - 5, y + 2); printf("����������������������������������������������������");

    if (win_lose == 1)
    {

        gotoxy(x - 5, y); printf("��       BLACK WIN       ��");


    }

    else if (win_lose == -1)
    {
        gotoxy(x - 5, y); printf("��       WHITE WIN       ��");
    }
    /* file_input(win_lose);*/
    Sleep(1500);
    key = getch();
}

int bot_player_check() // ��� ����
{
    system("cls");

    gotoxy(27, 10); printf("P L A Y E R");
    gotoxy(27, 15); printf("B O T _ A I");

    gotoxy(25, 15); printf("  ");
    gotoxy(33, 15); printf("  ");
    gotoxy(25, 10); printf("��");
    gotoxy(33, 10); printf("��");

    int check = 1;

    while (true)
    {
        key = getch();

        if (key == SPACE)
        {
            return check;
        }
        else if (key == DOWN && check < 2)
        {
            check++;
            gotoxy(25, 10); printf("  ");
            gotoxy(33, 10); printf("  ");
            gotoxy(25, 15); printf("��");
            gotoxy(33, 15); printf("��");
        }
        else if (key == UP && check > 1)
        {
            check--;

            gotoxy(25, 15); printf("  ");
            gotoxy(33, 15); printf("  ");
            gotoxy(25, 10); printf("��");
            gotoxy(33, 10); printf("��");
        }
    }
}

void bot_chose() // ���� ����� ��
{
    ++turn_cnt;
    int val = 0; //�ּ���
    static int x = 0, y = 0;// �ּ����� ��ǥ��

    if (turn_cnt != 2)
    {
        gotoxy(x + 3, y + 3); (bot_color == 1) ? printf("��") : printf("��");
    }

    for (int i = 0; i < MAIN_X; i++) //������ ���� ũ��
    {
        for (int j = 0; j < MAIN_Y; j++) //������ ���� ũ��
        {
            bot_block[i][j] = 0;
        }
    }


    for (int i = 0; i < MAIN_X; i++)
    {
        for (int j = 0; j < MAIN_Y; j++)
        {
            if (block[i][j] == color)
            {
                bot_block[i][j] = -10;

            }
            if (block[i][j] == bot_color)
            {
                bot_block[i][j] = 10;
            }
        }
    }

    int attack = bot_attack();
    int defence = bot_defence();
    int check = 0;

    if (attack > defence)
    {
        check = 1;

    }

    if (turn_cnt == 1)
    {
        check = 0;
    }

    if (check == 1)
    {
        for (int i = 0; i < MAIN_X; i++)
        {
            for (int j = 0; j < MAIN_Y; j++)
            {
                if (block[i][j] == bot_color)
                {
                    if (bot_block[i + 1][j] != 10 && bot_block[i + 1][j] != -10) bot_block[i + 1][j]++;
                    if (bot_block[i - 1][j] != 10 && bot_block[i - 1][j] != -10) bot_block[i - 1][j]++;
                    if (bot_block[i][j + 1] != 10 && bot_block[i][j + 1] != -10) bot_block[i][j + 1]++;
                    if (bot_block[i][j - 1] != 10 && bot_block[i][j - 1] != -10) bot_block[i][j - 1]++;
                    if (bot_block[i + 1][j + 1] != 10 && bot_block[i + 1][j + 1] != -10) bot_block[i + 1][j + 1]++;
                    if (bot_block[i + 1][j - 1] != 10 && bot_block[i + 1][j - 1] != -10) bot_block[i + 1][j - 1]++;
                    if (bot_block[i - 1][j + 1] != 10 && bot_block[i - 1][j + 1] != -10) bot_block[i - 1][j + 1]++;
                    if (bot_block[i - 1][j - 1] != 10 && bot_block[i - 1][j - 1] != -10) bot_block[i - 1][j - 1]++;
                }
            }
        }
    }

    else
    {
        for (int i = 0; i < MAIN_X; i++)
        {
            for (int j = 0; j < MAIN_Y; j++)
            {
                if (block[i][j] == color)
                {
                    if (bot_block[i + 1][j] != 10 && bot_block[i + 1][j] != -10) bot_block[i + 1][j]--;
                    if (bot_block[i - 1][j] != 10 && bot_block[i - 1][j] != -10) bot_block[i - 1][j]--;
                    if (bot_block[i][j + 1] != 10 && bot_block[i][j + 1] != -10) bot_block[i][j + 1]--;
                    if (bot_block[i][j - 1] != 10 && bot_block[i][j - 1] != -10) bot_block[i][j - 1]--;
                    if (bot_block[i + 1][j + 1] != 10 && bot_block[i + 1][j + 1] != -10) bot_block[i + 1][j + 1]--;
                    if (bot_block[i + 1][j - 1] != 10 && bot_block[i + 1][j - 1] != -10) bot_block[i + 1][j - 1]--;
                    if (bot_block[i - 1][j + 1] != 10 && bot_block[i - 1][j + 1] != -10) bot_block[i - 1][j + 1]--;
                    if (bot_block[i - 1][j - 1] != 10 && bot_block[i - 1][j - 1] != -10) bot_block[i - 1][j - 1]--;
                }
            }
        }
    }

    for (int i = 0; i < MAIN_X; i++)
    {
        for (int j = 0; j < MAIN_Y; j++)
        {
            if (block[i][j] == color)
            {
                bot_block[i][j] = -10;
            }
            if (block[i][j] == bot_color)
            {
                bot_block[i][j] = 10;
            }
        }
    }

    for (int i = 0; i < MAIN_X; i++)
    {
        for (int j = 0; j < MAIN_Y; j++)
        {
            if (bot_block[i][j] != 10 && bot_block[i][j] != -10
                && bot_block[i][j] > val && check == 1)
            {
                val = bot_block[i][j];
                x = i;
                y = j;
            }
            if (bot_block[i][j] != 10 && bot_block[i][j] != -10
                && bot_block[i][j] < val && check == 0)
            {
                val = bot_block[i][j];
                x = i;
                y = j;
            }
            if (bot_block[i][j] != 10 && bot_block[i][j] != -10
                && bot_block[i][j] == val)
            {
                if (rand() % 3 == 0)
                {
                    val = bot_block[i][j];
                    x = i;
                    y = j;
                }
            }
        }
    }

    block[x][y] = bot_color; //�迭 ����
    bot_block[x][y] = 10;
    gotoxy(x + 3, y + 3); (bot_color == 1) ? printf("��") : printf("��"); //���
}



int bot_attack()
{
    for (int i = 0; i < MAIN_X; i++)
    {
        for (int j = 0; j < MAIN_Y; j++)
        {
            if (difficulty >= 0)
            {
                if (difficulty >= 1)
                {
                    if (!(block[i][j - 1] == color && block[i][j + 3] == color) && block[i][j] == block[i][j + 1] && block[i][j + 1] == block[i][j + 2] && block[i][j + 2] == bot_color)
                    {
                        if (bot_block[i][j - 1] != 10 && bot_block[i][j - 1] != -10 && bot_block[i][j - 1] < 160) bot_block[i][j - 1] = 160;
                        if (bot_block[i][j + 3] != 10 && bot_block[i][j + 3] != -10 && bot_block[i][j + 3] < 160) bot_block[i][j + 3] = 160;
                    }
                    if (!(block[i - 1][j] == color && block[i + 3][j] == color) && block[i][j] == block[i + 1][j] && block[i + 1][j] == block[i + 2][j] && block[i + 2][j] == bot_color)
                    {
                        if (bot_block[i - 1][j] != 10 && bot_block[i - 1][j] != -10 && bot_block[i - 1][j] < 160) bot_block[i - 1][j] = 160;
                        if (bot_block[i + 3][j] != 10 && bot_block[i + 3][j] != -10 && bot_block[i + 3][j] < 160) bot_block[i + 3][j] = 160;
                    }
                    if (!(block[i - 1][j - 1] == color && block[i + 3][j + 3] == color) && block[i][j] == block[i + 1][j + 1] && block[i + 1][j + 1] == block[i + 2][j + 2] && block[i + 2][j + 2] == bot_color)
                    {
                        if (bot_block[i - 1][j - 1] != 10 && bot_block[i - 1][j - 1] != -10 && bot_block[i - 1][j - 1] < 160) bot_block[i - 1][j - 1] = 160;
                        if (bot_block[i + 3][j + 3] != 10 && bot_block[i + 3][j + 3] != -10 && bot_block[i + 3][j + 3] < 160) bot_block[i + 3][j + 3] = 160;
                    }
                    if (!(block[i - 1][j + 1] == color && block[i + 3][j - 3] == color) && block[i][j] == block[i + 1][j - 1] && block[i + 1][j - 1] == block[i + 2][j - 2] && block[i + 2][j - 2] == bot_color)
                    {
                        if (bot_block[i - 1][j + 1] != 10 && bot_block[i - 1][j + 1] != -10 && bot_block[i - 1][j + 1] < 160) bot_block[i - 1][j + 1] = 160;
                        if (bot_block[i + 3][j - 3] != 10 && bot_block[i + 3][j - 3] != -10 && bot_block[i + 3][j - 3] < 160) bot_block[i + 3][j - 3] = 160;
                    }
                    if ((block[i][j - 1] == color || block[i][j + 3] == color) && block[i][j] == block[i][j + 1] && block[i][j + 1] == block[i][j + 2] && block[i][j + 2] == bot_color)
                    {
                        if (bot_block[i][j - 1] != 10 && bot_block[i][j - 1] != -10 && block[i][j + 3] == color && bot_block[i][j - 1] < 35) bot_block[i][j - 1] = 35;
                        if (bot_block[i][j + 3] != 10 && bot_block[i][j + 3] != -10 && block[i][j - 1] == color && bot_block[i][j + 3] < 35) bot_block[i][j + 3] = 35;
                    }
                    if ((block[i - 1][j] == color || block[i + 3][j] == color) && block[i][j] == block[i + 1][j] && block[i + 1][j] == block[i + 2][j] && block[i + 2][j] == bot_color)
                    {
                        if (bot_block[i - 1][j] != 10 && bot_block[i - 1][j] != -10 && block[i + 3][j] == color && bot_block[i - 1][j] < 35) bot_block[i - 1][j] = 35;
                        if (bot_block[i + 3][j] != 10 && bot_block[i + 3][j] != -10 && block[i - 1][j] == color && bot_block[i + 3][j] < 35) bot_block[i + 3][j] = 35;
                    }
                    if ((block[i - 1][j - 1] == color || block[i + 3][j + 3] == color) && block[i][j] == block[i + 1][j + 1] && block[i + 1][j + 1] == block[i + 2][j + 2] && block[i + 2][j + 2] == bot_color)
                    {
                        if (bot_block[i - 1][j - 1] != 10 && bot_block[i - 1][j - 1] != -10 && block[i + 3][j + 3] == color && bot_block[i - 1][j - 1] < 35) bot_block[i - 1][j - 1] = 35;
                        if (bot_block[i + 3][j + 3] != 10 && bot_block[i + 3][j + 3] != -10 && block[i - 1][j - 1] == color && bot_block[i + 3][j + 3] < 35) bot_block[i + 3][j + 3] = 35;
                    }
                    if ((block[i - 1][j + 1] == color || block[i + 3][j - 3] == color) && block[i][j] == block[i + 1][j - 1] && block[i + 1][j - 1] == block[i + 2][j - 2] && block[i + 2][j - 2] == bot_color)
                    {
                        if (bot_block[i - 1][j + 1] != 10 && bot_block[i - 1][j + 1] != -10 && block[i + 3][j - 3] == color && bot_block[i - 1][j + 1] < 35) bot_block[i - 1][j + 1] = 35;
                        if (bot_block[i + 3][j - 3] != 10 && bot_block[i + 3][j - 3] != -10 && block[i - 1][j + 1] == color && bot_block[i + 3][j - 3] < 35) bot_block[i + 3][j - 3] = 35;
                    }
                    if (block[i][j] == block[i][j + 1] && block[i][j + 1] == block[i][j + 2] && block[i][j + 2] == block[i][j + 3] && block[i][j + 3] == bot_color)
                    {
                        if (bot_block[i][j - 1] != 10 && bot_block[i][j - 1] != -10 && bot_block[i][j - 1] < 250) bot_block[i][j - 1] = 250;
                        if (bot_block[i][j + 4] != 10 && bot_block[i][j + 4] != -10 && bot_block[i][j + 4] < 250) bot_block[i][j + 4] = 250;
                    }
                    if (block[i][j] == block[i + 1][j] && block[i + 1][j] == block[i + 2][j] && block[i + 2][j] == block[i + 3][j] && block[i + 3][j] == bot_color)
                    {
                        if (bot_block[i - 1][j] != 10 && bot_block[i - 1][j] != -10 && bot_block[i - 1][j] < 250) bot_block[i - 1][j] = 250;
                        if (bot_block[i + 4][j] != 10 && bot_block[i + 4][j] != -10 && bot_block[i + 4][j] < 250) bot_block[i + 4][j] = 250;
                    }
                    if (block[i][j] == block[i + 1][j + 1] && block[i + 1][j + 1] == block[i + 2][j + 2] && block[i + 2][j + 2] == block[i + 3][j + 3] && block[i + 3][j + 3] == bot_color)
                    {
                        if (bot_block[i - 1][j - 1] != 10 && bot_block[i - 1][j - 1] != -10 && bot_block[i - 1][j - 1] < 250) bot_block[i - 1][j - 1] = 250;
                        if (bot_block[i + 4][j + 4] != 10 && bot_block[i + 4][j + 4] != -10 && bot_block[i + 4][j + 4] < 250) bot_block[i + 4][j + 4] = 250;
                    }
                    if (block[i][j] == block[i + 1][j - 1] && block[i + 1][j - 1] == block[i + 2][j - 2] && block[i + 2][j - 2] == block[i + 3][j - 3] && block[i + 3][j - 3] == bot_color)
                    {
                        if (bot_block[i - 1][j + 1] != 10 && bot_block[i - 1][j + 1] != -10 && bot_block[i - 1][j + 1] < 250) bot_block[i - 1][j + 1] = 250;
                        if (bot_block[i + 4][j - 4] != 10 && bot_block[i + 4][j - 4] != -10 && bot_block[i + 4][j - 4] < 250) bot_block[i + 4][j - 4] = 250;
                    }
                    if (block[i][j] == block[i][j + 3] && block[i][j + 3] == bot_color && (block[i][j + 1] == bot_color || block[i][j + 2] == bot_color))
                    {
                        if (bot_block[i][j + 1] != 10 && bot_block[i][j + 1] != -10 && bot_block[i][j + 1] < 40 && (bot_block[i][j - 1] == color || bot_block[i][j + 4] == color)) bot_block[i][j + 1] = 40;
                        if (bot_block[i][j + 2] != 10 && bot_block[i][j + 2] != -10 && bot_block[i][j + 2] < 40 && (bot_block[i][j - 1] == color || bot_block[i][j + 4] == color)) bot_block[i][j + 2] = 40;
                        if (bot_block[i][j + 1] != 10 && bot_block[i][j + 1] != -10 && bot_block[i][j + 1] < 175 && !(bot_block[i][j - 1] == color || bot_block[i][j + 4] == color)) bot_block[i][j + 1] = 175;
                        if (bot_block[i][j + 2] != 10 && bot_block[i][j + 2] != -10 && bot_block[i][j + 2] < 175 && !(bot_block[i][j - 1] == color || bot_block[i][j + 4] == color)) bot_block[i][j + 2] = 175;
                    }
                    if (block[i][j] == block[i + 3][j] && block[i + 3][j] == bot_color && (block[i + 1][j] == bot_color || block[i + 2][j] == bot_color))
                    {
                        if (bot_block[i + 1][j] != 10 && bot_block[i + 1][j] != -10 && bot_block[i + 1][j] < 40 && (bot_block[i - 1][j] == color || bot_block[i + 4][j] == color)) bot_block[i + 1][j] = 40;
                        if (bot_block[i + 2][j] != 10 && bot_block[i + 2][j] != -10 && bot_block[i + 2][j] < 40 && (bot_block[i - 1][j] == color || bot_block[i + 4][j] == color)) bot_block[i + 2][j] = 40;
                        if (bot_block[i + 1][j] != 10 && bot_block[i + 1][j] != -10 && bot_block[i + 1][j] < 175 && !(bot_block[i - 1][j] == color || bot_block[i + 4][j] == color)) bot_block[i + 1][j] = 175;
                        if (bot_block[i + 2][j] != 10 && bot_block[i + 2][j] != -10 && bot_block[i + 2][j] < 175 && !(bot_block[i - 1][j] == color || bot_block[i + 4][j] == color)) bot_block[i + 2][j] = 175;
                    }
                    if (block[i][j] == block[i + 3][j + 3] && block[i + 3][j + 3] == bot_color && (block[i + 1][j + 1] == bot_color || block[i + 2][j + 2] == bot_color))
                    {
                        if (bot_block[i + 1][j + 1] != 10 && bot_block[i + 1][j + 1] != -10 && bot_block[i + 1][j + 1] < 40 && (bot_block[i - 1][j - 1] == color || bot_block[i + 4][j + 4] == color)) bot_block[i + 1][j + 1] = 40;
                        if (bot_block[i + 2][j + 2] != 10 && bot_block[i + 2][j + 2] != -10 && bot_block[i + 1][j + 2] < 40 && (bot_block[i - 1][j - 1] == color || bot_block[i + 4][j + 4] == color)) bot_block[i + 2][j + 2] = 40;
                        if (bot_block[i + 1][j + 1] != 10 && bot_block[i + 1][j + 1] != -10 && bot_block[i + 1][j + 1] < 175 && !(bot_block[i - 1][j - 1] == color || bot_block[i + 4][j + 4] == color)) bot_block[i + 1][j + 1] = 175;
                        if (bot_block[i + 2][j + 1] != 10 && bot_block[i + 2][j + 1] != -10 && bot_block[i + 1][j + 2] < 175 && !(bot_block[i - 1][j - 1] == color || bot_block[i + 4][j + 4] == color)) bot_block[i + 2][j + 2] = 175;
                    }
                    if (block[i][j] == block[i + 3][j - 3] && block[i + 3][j - 3] == bot_color && (block[i + 1][j - 1] == bot_color || block[i + 2][j - 2] == bot_color))
                    {
                        if (bot_block[i + 1][j - 1] != 10 && bot_block[i + 1][j - 1] != -10 && bot_block[i + 1][j - 1] < 40 && (bot_block[i - 1][j + 1] == color || bot_block[i + 4][j - 4] == color)) bot_block[i + 1][j - 1] = 40;
                        if (bot_block[i + 2][j - 2] != 10 && bot_block[i + 2][j - 2] != -10 && bot_block[i + 2][j - 2] < 40 && (bot_block[i - 1][j + 1] == color || bot_block[i + 4][j - 4] == color)) bot_block[i + 2][j - 2] = 40;
                        if (bot_block[i + 1][j - 1] != 10 && bot_block[i + 1][j - 1] != -10 && bot_block[i + 1][j - 1] < 175 && (bot_block[i - 1][j + 1] == color || bot_block[i + 4][j - 4] == color)) bot_block[i + 1][j - 1] = 175;
                        if (bot_block[i + 2][j - 2] != 10 && bot_block[i + 2][j - 2] != -10 && bot_block[i + 2][j - 2] < 175 && (bot_block[i - 1][j + 1] == color || bot_block[i + 4][j - 4] == color)) bot_block[i + 2][j - 2] = 175;
                    }

                    if (block[i][j] == block[i][j + 1] && block[i][j + 3] == block[i][j + 4] && block[i][j + 1] == block[i][j + 3] && block[i][j + 3] == bot_color)
                    {
                        if (bot_block[i][j + 2] != 10 && bot_block[i][j + 2] != -10 && bot_block[i][j + 2] < 250) bot_block[i][j + 2] = 250;
                    }
                    if (block[i][j] == block[i + 1][j] && block[i + 3][j] == block[i + 4][j] && block[i + 1][j] == block[i + 3][j] && block[i + 3][j] == bot_color)
                    {
                        if (bot_block[i + 2][j] != 10 && bot_block[i + 2][j] != -10 && bot_block[i + 2][j] < 250) bot_block[i + 2][j] = 250;
                    }
                    if (block[i][j] == block[i + 1][j + 1] && block[i + 3][j + 3] == block[i + 4][j + 4] && block[i + 1][j + 1] == block[i + 3][j + 3] && block[i + 3][j + 3] == bot_color)
                    {
                        if (bot_block[i + 2][j + 2] != 10 && bot_block[i + 2][j + 2] != -10 && bot_block[i + 2][j + 2] < 250) bot_block[i + 2][j + 2] = 250;
                    }
                    if (block[i][j] == block[i + 1][j - 1] && block[i + 3][j - 3] == block[i + 4][j - 4] && block[i + 1][j - 1] == block[i + 3][j - 3] && block[i + 3][j - 3] == bot_color)
                    {
                        if (bot_block[i + 2][j - 2] != 10 && bot_block[i + 2][j - 2] != -10 && bot_block[i + 2][j - 2] < 250) bot_block[i + 2][j - 2] = 250;
                    }

                    if (difficulty >= 2)
                    {
                        if (block[i][j] == block[i][j + 1] && block[i][j + 1] == bot_color)
                        {
                            if (bot_block[i][j - 1] != 10 && bot_block[i][j - 1] != -10 && bot_block[i][j - 1] < 30) bot_block[i][j - 1] = 30;
                            if (bot_block[i][j + 2] != 10 && bot_block[i][j + 2] != -10 && bot_block[i][j + 2] < 30) bot_block[i][j + 2] = 30;
                        }
                        if (block[i][j] == block[i + 1][j] && block[i + 1][j] == bot_color)
                        {
                            if (bot_block[i - 1][j] != 10 && bot_block[i - 1][j] != -10 && bot_block[i - 1][j] < 30) bot_block[i - 1][j] = 30;
                            if (bot_block[i + 2][j] != 10 && bot_block[i + 2][j] != -10 && bot_block[i + 2][j] < 30) bot_block[i + 2][j] = 30;
                        }
                        if (block[i][j] == block[i + 1][j + 1] && block[i + 1][j + 1] == bot_color)
                        {
                            if (bot_block[i - 1][j - 1] != 10 && bot_block[i - 1][j - 1] != -10 && bot_block[i - 1][j - 1] < 30) bot_block[i - 1][j - 1] = 30;
                            if (bot_block[i + 2][j + 2] != 10 && bot_block[i + 2][j + 2] != -10 && bot_block[i + 2][j + 2] < 30) bot_block[i + 2][j + 2] = 30;
                        }
                        if (block[i][j] == block[i + 1][j - 1] && block[i + 1][j - 1] == bot_color)
                        {
                            if (bot_block[i - 1][j + 1] != 10 && bot_block[i - 1][j + 1] != -10 && bot_block[i - 1][j + 1] < 30) bot_block[i - 1][j + 1] = 30;
                            if (bot_block[i + 2][j - 2] != 10 && bot_block[i + 2][j - 2] != -10 && bot_block[i + 2][j - 2] < 30) bot_block[i + 2][j - 2] = 30;
                        }

                        if (!(block[i][j - 1] == color && block[i][j + 2] == color) && block[i][j] == block[i][j + 1] && block[i][j + 1] == bot_color)
                        {
                            if (bot_block[i][j - 1] != 10 && bot_block[i][j - 1] != -10 && bot_block[i][j - 1] < 60) bot_block[i][j - 1] = 60;
                            if (bot_block[i][j + 2] != 10 && bot_block[i][j + 2] != -10 && bot_block[i][j + 2] < 60) bot_block[i][j + 2] = 60;
                        }
                        if (!(block[i - 1][j] == color && block[i + 2][j] == color) && block[i][j] == block[i + 1][j] && block[i + 1][j] == bot_color)
                        {
                            if (bot_block[i - 1][j] != 10 && bot_block[i - 1][j] != -10 && bot_block[i - 1][j] < 60) bot_block[i - 1][j] = 60;
                            if (bot_block[i + 2][j] != 10 && bot_block[i + 2][j] != -10 && bot_block[i + 2][j] < 60) bot_block[i + 2][j] = 60;
                        }
                        if (!(block[i - 1][j - 1] == color && block[i + 2][j + 2] == color) && block[i][j] == block[i + 1][j + 1] && block[i + 1][j + 1] == bot_color)
                        {
                            if (bot_block[i - 1][j - 1] != 10 && bot_block[i - 1][j - 1] != -10 && bot_block[i - 1][j - 1] < 60) bot_block[i - 1][j - 1] = 60;
                            if (bot_block[i + 2][j + 2] != 10 && bot_block[i + 2][j + 2] != -10 && bot_block[i + 2][j + 2] < 60) bot_block[i + 2][j + 2] = 60;
                        }
                        if (!(block[i - 1][j + 1] == color && block[i + 2][j - 2] == color) && block[i][j] == block[i + 1][j - 1] && block[i + 1][j - 1] == bot_color)
                        {
                            if (bot_block[i - 1][j + 1] != 10 && bot_block[i - 1][j + 1] != -10 && bot_block[i - 1][j + 1] < 60) bot_block[i - 1][j + 1] = 60;
                            if (bot_block[i + 2][j - 2] != 10 && bot_block[i + 2][j - 2] != -10 && bot_block[i + 2][j - 2] < 60) bot_block[i + 2][j - 2] = 60;
                        }

                        if (!(block[i][j - 1] == color && block[i][j + 3] == color) && block[i][j] == block[i][j + 2] && block[i][j + 2] == bot_color)
                        {
                            if (bot_block[i][j + 1] != 10 && bot_block[i][j + 1] != -10 && bot_block[i][j + 1] < 50) bot_block[i][j + 1] = 60;
                        }
                        if (!(block[i - 1][j] == color && block[i + 3][j] == color) && block[i][j] == block[i + 2][j] && block[i + 2][j] == bot_color)
                        {
                            if (bot_block[i + 1][j] != 10 && bot_block[i + 1][j] != -10 && bot_block[i + 1][j] < 50) bot_block[i + 1][j] = 60;
                        }
                        if (!(block[i - 1][j - 1] == color && block[i + 3][j + 3] == color) && block[i][j] == block[i + 2][j + 2] && block[i + 2][j + 2] == bot_color)
                        {
                            if (bot_block[i + 1][j + 1] != 10 && bot_block[i + 1][j + 1] != -10 && bot_block[i + 1][j + 1] < 50) bot_block[i + 1][j + 1] = 60;
                        }
                        if (!(block[i - 1][j + 1] == color && block[i + 3][j - 3] == color) && block[i][j] == block[i + 2][j - 2] && block[i + 2][j - 2] == bot_color)
                        {
                            if (bot_block[i + 1][j - 1] != 10 && bot_block[i + 1][j - 1] != -10 && bot_block[i + 1][j - 1] < 50) bot_block[i + 1][j - 1] = 60;
                        }

                        if (block[i][j] == block[i][j + 3] && block[i][j + 3] == bot_color && (block[i][j + 1] == bot_color || block[i][j + 2] == bot_color))
                        {
                            if (bot_block[i][j + 1] != 10 && bot_block[i][j + 1] != -10 && bot_block[i][j + 1] < 40 && (bot_block[i][j - 1] == color || bot_block[i][j + 4] == color)) bot_block[i][j + 1] = 40;
                            if (bot_block[i][j + 2] != 10 && bot_block[i][j + 2] != -10 && bot_block[i][j + 2] < 40 && (bot_block[i][j - 1] == color || bot_block[i][j + 4] == color)) bot_block[i][j + 2] = 40;
                            if (bot_block[i][j + 1] != 10 && bot_block[i][j + 1] != -10 && bot_block[i][j + 1] < 175 && !(bot_block[i][j - 1] == color || bot_block[i][j + 4] == color)) bot_block[i][j + 1] = 175;
                            if (bot_block[i][j + 2] != 10 && bot_block[i][j + 2] != -10 && bot_block[i][j + 2] < 175 && !(bot_block[i][j - 1] == color || bot_block[i][j + 4] == color)) bot_block[i][j + 2] = 175;
                        }
                        if (block[i][j] == block[i + 3][j] && block[i + 3][j] == bot_color && (block[i + 1][j] == bot_color || block[i + 2][j] == bot_color))
                        {
                            if (bot_block[i + 1][j] != 10 && bot_block[i + 1][j] != -10 && bot_block[i + 1][j] < 40 && (bot_block[i - 1][j] == color || bot_block[i + 4][j] == color)) bot_block[i + 1][j] = 40;
                            if (bot_block[i + 2][j] != 10 && bot_block[i + 2][j] != -10 && bot_block[i + 2][j] < 40 && (bot_block[i - 1][j] == color || bot_block[i + 4][j] == color)) bot_block[i + 2][j] = 40;
                            if (bot_block[i + 1][j] != 10 && bot_block[i + 1][j] != -10 && bot_block[i + 1][j] < 175 && !(bot_block[i - 1][j] == color || bot_block[i + 4][j] == color)) bot_block[i + 1][j] = 175;
                            if (bot_block[i + 2][j] != 10 && bot_block[i + 2][j] != -10 && bot_block[i + 2][j] < 175 && !(bot_block[i - 1][j] == color || bot_block[i + 4][j] == color)) bot_block[i + 2][j] = 175;
                        }
                        if (block[i][j] == block[i + 3][j + 3] && block[i + 3][j + 3] == bot_color && (block[i + 1][j + 1] == bot_color || block[i + 2][j + 2] == bot_color))
                        {
                            if (bot_block[i + 1][j + 1] != 10 && bot_block[i + 1][j + 1] != -10 && bot_block[i + 1][j + 1] < 40 && (bot_block[i - 1][j - 1] == color || bot_block[i + 4][j + 4] == color)) bot_block[i + 1][j + 1] = 40;
                            if (bot_block[i + 2][j + 2] != 10 && bot_block[i + 2][j + 2] != -10 && bot_block[i + 1][j + 2] < 40 && (bot_block[i - 1][j - 1] == color || bot_block[i + 4][j + 4] == color)) bot_block[i + 2][j + 2] = 40;
                            if (bot_block[i + 1][j + 1] != 10 && bot_block[i + 1][j + 1] != -10 && bot_block[i + 1][j + 1] < 175 && !(bot_block[i - 1][j - 1] == color || bot_block[i + 4][j + 4] == color)) bot_block[i + 1][j + 1] = 175;
                            if (bot_block[i + 2][j + 1] != 10 && bot_block[i + 2][j + 1] != -10 && bot_block[i + 1][j + 2] < 175 && !(bot_block[i - 1][j - 1] == color || bot_block[i + 4][j + 4] == color)) bot_block[i + 2][j + 2] = 175;
                        }
                        if (block[i][j] == block[i + 3][j - 3] && block[i + 3][j - 3] == bot_color && (block[i + 1][j - 1] == bot_color || block[i + 2][j - 2] == bot_color))
                        {
                            if (bot_block[i + 1][j - 1] != 10 && bot_block[i + 1][j - 1] != -10 && bot_block[i + 1][j - 1] < 40 && (bot_block[i - 1][j + 1] == color || bot_block[i + 4][j - 4] == color)) bot_block[i + 1][j - 1] = 40;
                            if (bot_block[i + 2][j - 2] != 10 && bot_block[i + 2][j - 2] != -10 && bot_block[i + 2][j - 2] < 40 && (bot_block[i - 1][j + 1] == color || bot_block[i + 4][j - 4] == color)) bot_block[i + 2][j - 2] = 40;
                            if (bot_block[i + 1][j - 1] != 10 && bot_block[i + 1][j - 1] != -10 && bot_block[i + 1][j - 1] < 175 && (bot_block[i - 1][j + 1] == color || bot_block[i + 4][j - 4] == color)) bot_block[i + 1][j - 1] = 175;
                            if (bot_block[i + 2][j - 2] != 10 && bot_block[i + 2][j - 2] != -10 && bot_block[i + 2][j - 2] < 175 && (bot_block[i - 1][j + 1] == color || bot_block[i + 4][j - 4] == color)) bot_block[i + 2][j - 2] = 175;
                        }

                        if (block[i][j] == block[i][j + 1] && block[i][j + 3] == block[i][j + 4] && block[i][j + 1] == block[i][j + 3] && block[i][j + 3] == bot_color)
                        {
                            if (bot_block[i][j + 2] != 10 && bot_block[i][j + 2] != -10 && bot_block[i][j + 2] < 250) bot_block[i][j + 2] = 250;
                        }
                        if (block[i][j] == block[i + 1][j] && block[i + 3][j] == block[i + 4][j] && block[i + 1][j] == block[i + 3][j] && block[i + 3][j] == bot_color)
                        {
                            if (bot_block[i + 2][j] != 10 && bot_block[i + 2][j] != -10 && bot_block[i + 2][j] < 250) bot_block[i + 2][j] = 250;
                        }
                        if (block[i][j] == block[i + 1][j + 1] && block[i + 3][j + 3] == block[i + 4][j + 4] && block[i + 1][j + 1] == block[i + 3][j + 3] && block[i + 3][j + 3] == bot_color)
                        {
                            if (bot_block[i + 2][j + 2] != 10 && bot_block[i + 2][j + 2] != -10 && bot_block[i + 2][j + 2] < 250) bot_block[i + 2][j + 2] = 250;
                        }
                        if (block[i][j] == block[i + 1][j - 1] && block[i + 3][j - 3] == block[i + 4][j - 4] && block[i + 1][j - 1] == block[i + 3][j - 3] && block[i + 3][j - 3] == bot_color)
                        {
                            if (bot_block[i + 2][j - 2] != 10 && bot_block[i + 2][j - 2] != -10 && bot_block[i + 2][j - 2] < 250) bot_block[i + 2][j - 2] = 250;
                        }
                    }
                }
            }
        }
    }

    int max = 0;

    for (int i = 0; i < MAIN_X; i++)
    {
        for (int j = 0; j < MAIN_Y; j++)
        {
            if (bot_block[i][j] >= 30)
            {
                if (bot_block[i][j] > max)
                {
                    max = bot_block[i][j];
                }
            }
        }
    }

    return max;
}

int bot_defence()
{
    for (int i = 0; i < MAIN_X; i++)
    {
        for (int j = 0; j < MAIN_Y; j++)
        {
            if (difficulty >= 0)
            {


                if (block[i][j] == block[i][j + 1] && block[i][j + 1] == block[i][j + 2] && block[i][j + 2] == block[i][j + 3] && block[i][j + 3] == color)
                {
                    if (bot_block[i][j - 1] != 10 && bot_block[i][j - 1] != -10 && bot_block[i][j - 1] > -200) bot_block[i][j - 1] = -200;
                    if (bot_block[i][j + 4] != 10 && bot_block[i][j + 4] != -10 && bot_block[i][j + 4] > -200) bot_block[i][j + 4] = -200;
                }
                if (block[i][j] == block[i + 1][j] && block[i + 1][j] == block[i + 2][j] && block[i + 2][j] == block[i + 3][j] && block[i + 3][j] == color)
                {
                    if (bot_block[i - 1][j] != 10 && bot_block[i - 1][j] != -10 && bot_block[i - 1][j] > -200) bot_block[i - 1][j] = -200;
                    if (bot_block[i + 4][j] != 10 && bot_block[i + 4][j] != -10 && bot_block[i + 4][j] > -200) bot_block[i + 4][j] = -200;
                }
                if (block[i][j] == block[i + 1][j + 1] && block[i + 1][j + 1] == block[i + 2][j + 2] && block[i + 2][j + 2] == block[i + 3][j + 3] && block[i + 3][j + 3] == color)
                {
                    if (bot_block[i - 1][j - 1] != 10 && bot_block[i - 1][j - 1] != -10 && bot_block[i - 1][j - 1] > -200) bot_block[i - 1][j - 1] = -200;
                    if (bot_block[i + 4][j + 4] != 10 && bot_block[i + 4][j + 4] != -10 && bot_block[i + 4][j + 4] > -200) bot_block[i + 4][j + 4] = -200;
                }
                if (block[i][j] == block[i + 1][j - 1] && block[i + 1][j - 1] == block[i + 2][j - 2] && block[i + 2][j - 2] == block[i + 3][j - 3] && block[i + 3][j - 3] == color)
                {
                    if (bot_block[i - 1][j + 1] != 10 && bot_block[i - 1][j + 1] != -10 && bot_block[i - 1][j + 1] > -200) bot_block[i - 1][j + 1] = -200;
                    if (bot_block[i + 4][j - 4] != 10 && bot_block[i + 4][j - 4] != -10 && bot_block[i + 4][j - 4] > -200) bot_block[i + 4][j - 4] = -200;
                }

                if (!(block[i][j - 1] == bot_color || block[i][j + 3] == bot_color) && block[i][j] == block[i][j + 1] && block[i][j + 1] == block[i][j + 2] && block[i][j + 2] == color)
                {
                    if (bot_block[i][j - 1] != 10 && bot_block[i][j - 1] != -10 && bot_block[i][j - 1] > -150) bot_block[i][j - 1] = -150;
                    if (bot_block[i][j + 3] != 10 && bot_block[i][j + 3] != -10 && bot_block[i][j + 3] > -150) bot_block[i][j + 3] = -150;
                }
                if (!(block[i - 1][j] == bot_color || block[i + 3][j] == bot_color) && block[i][j] == block[i + 1][j] && block[i + 1][j] == block[i + 2][j] && block[i + 2][j] == color)
                {
                    if (bot_block[i - 1][j] != 10 && bot_block[i - 1][j] != -10 && bot_block[i - 1][j] > -150) bot_block[i - 1][j] = -150;
                    if (bot_block[i + 3][j] != 10 && bot_block[i + 3][j] != -10 && bot_block[i + 3][j] > -150) bot_block[i + 3][j] = -150;
                }
                if (!(block[i - 1][j - 1] == bot_color || block[i + 3][j + 3] == bot_color) && block[i][j] == block[i + 1][j + 1] && block[i + 1][j + 1] == block[i + 2][j + 2] && block[i + 2][j + 2] == color)
                {
                    if (bot_block[i - 1][j - 1] != 10 && bot_block[i - 1][j - 1] != -10 && bot_block[i - 1][j - 1] > -150) bot_block[i - 1][j - 1] = -150;
                    if (bot_block[i + 3][j + 3] != 10 && bot_block[i + 3][j + 3] != -10 && bot_block[i + 3][j + 3] > -150) bot_block[i + 3][j + 3] = -150;
                }
                if (!(block[i - 1][j + 1] == bot_color || block[i + 3][j - 3] == bot_color) && block[i][j] == block[i + 1][j - 1] && block[i + 1][j - 1] == block[i + 2][j - 2] && block[i + 2][j - 2] == color)
                {
                    if (bot_block[i - 1][j + 1] != 10 && bot_block[i - 1][j + 1] != -10 && bot_block[i - 1][j + 1] > -150) bot_block[i - 1][j + 1] = -150;
                    if (bot_block[i + 3][j - 3] != 10 && bot_block[i + 3][j - 3] != -10 && bot_block[i + 3][j - 3] > -150) bot_block[i + 3][j - 3] = -150;
                }

                if (difficulty >= 1)
                {
                    if (block[i][j] == block[i][j + 3] && block[i][j + 3] == color && (block[i][j + 1] == color || block[i][j + 2] == color))
                    {
                        if (bot_block[i][j + 1] != 10 && bot_block[i][j + 1] != -10 && bot_block[i][j + 1] > -35 && (bot_block[i][j - 1] == bot_color || bot_block[i][j + 4] == bot_color)) bot_block[i][j + 1] = -35;
                        if (bot_block[i][j + 2] != 10 && bot_block[i][j + 2] != -10 && bot_block[i][j + 2] > -35 && (bot_block[i][j - 1] == bot_color || bot_block[i][j + 4] == bot_color)) bot_block[i][j + 2] = -35;
                        if (bot_block[i][j + 1] != 10 && bot_block[i][j + 1] != -10 && bot_block[i][j + 1] > -200 && (bot_block[i][j - 1] == color || bot_block[i][j + 4] == color)) bot_block[i][j + 1] = -200;
                        if (bot_block[i][j + 2] != 10 && bot_block[i][j + 2] != -10 && bot_block[i][j + 2] > -200 && (bot_block[i][j - 1] == color || bot_block[i][j + 4] == color)) bot_block[i][j + 2] = -200;
                        else if (bot_block[i][j + 1] != 10 && bot_block[i][j + 1] != -10 && bot_block[i][j + 1] > -170) bot_block[i][j + 1] = -170;
                        else if (bot_block[i][j + 2] != 10 && bot_block[i][j + 2] != -10 && bot_block[i][j + 2] > -170) bot_block[i][j + 2] = -170;
                    }
                    if (block[i][j] == block[i + 3][j] && block[i + 3][j] == color && (block[i + 1][j] == color || block[i + 2][j] == color))
                    {
                        if (bot_block[i + 1][j] != 10 && bot_block[i + 1][j] != -10 && bot_block[i + 1][j] > -35 && (bot_block[i - 1][j] == bot_color || bot_block[i + 4][j] == bot_color)) bot_block[i + 1][j] = -35;
                        if (bot_block[i + 2][j] != 10 && bot_block[i + 2][j] != -10 && bot_block[i + 2][j] > -35 && (bot_block[i - 1][j] == bot_color || bot_block[i + 4][j] == bot_color)) bot_block[i + 2][j] = -35;
                        if (bot_block[i + 1][j] != 10 && bot_block[i + 1][j] != -10 && bot_block[i + 1][j] > -200 && (bot_block[i - 1][j] == color || bot_block[i + 4][j] == color)) bot_block[i + 1][j] = -200;
                        if (bot_block[i + 2][j] != 10 && bot_block[i + 2][j] != -10 && bot_block[i + 2][j] > -200 && (bot_block[i - 1][j] == color || bot_block[i + 4][j] == color)) bot_block[i + 2][j] = -200;
                        else if (bot_block[i + 1][j] != 10 && bot_block[i + 1][j] != -10 && bot_block[i + 1][j] > -170) bot_block[i + 1][j] = -170;
                        else if (bot_block[i + 2][j] != 10 && bot_block[i + 2][j] != -10 && bot_block[i + 2][j] > -170) bot_block[i + 2][j] = -170;
                    }
                    if (block[i][j] == block[i + 3][j + 3] && block[i + 3][j + 3] == color && (block[i + 1][j + 1] == color || block[i + 2][j + 2] == color))
                    {
                        if (bot_block[i + 1][j + 1] != 10 && bot_block[i + 1][j + 1] != -10 && bot_block[i + 1][j + 1] > -35 && (bot_block[i - 1][j - 1] == bot_color || bot_block[i + 4][j + 4] == bot_color)) bot_block[i + 1][j + 1] = -35;
                        if (bot_block[i + 2][j + 2] != 10 && bot_block[i + 2][j + 2] != -10 && bot_block[i + 1][j + 2] > -35 && (bot_block[i - 1][j - 1] == bot_color || bot_block[i + 4][j + 4] == bot_color)) bot_block[i + 2][j + 2] = -35;
                        if (bot_block[i + 1][j + 1] != 10 && bot_block[i + 1][j + 1] != -10 && bot_block[i + 1][j + 1] > -200 && (bot_block[i - 1][j - 1] == color || bot_block[i + 4][j + 4] == color)) bot_block[i + 1][j + 1] = -200;
                        if (bot_block[i + 2][j + 2] != 10 && bot_block[i + 2][j + 2] != -10 && bot_block[i + 2][j + 2] > -200 && (bot_block[i - 1][j - 1] == color || bot_block[i + 4][j + 4] == color)) bot_block[i + 2][j + 2] = -200;
                        else if (bot_block[i + 1][j + 1] != 10 && bot_block[i + 1][j + 1] != -10 && bot_block[i + 1][j + 1] > -170) bot_block[i + 1][j + 1] = -170;
                        else if (bot_block[i + 2][j + 2] != 10 && bot_block[i + 2][j + 2] != -10 && bot_block[i + 1][j + 2] > -170) bot_block[i + 2][j + 2] = -170;
                    }
                    if (block[i][j] == block[i + 3][j - 3] && block[i + 3][j - 3] == color && (block[i + 1][j - 1] == color || block[i + 2][j - 2] == color))
                    {
                        if (bot_block[i + 1][j - 1] != 10 && bot_block[i + 1][j - 1] != -10 && bot_block[i + 1][j - 1] > -35 && (bot_block[i - 1][j + 1] == bot_color || bot_block[i + 4][j - 4] == bot_color)) bot_block[i + 1][j - 1] = -35;
                        if (bot_block[i + 2][j - 2] != 10 && bot_block[i + 2][j - 2] != -10 && bot_block[i + 2][j - 2] > -35 && (bot_block[i - 1][j + 1] == bot_color || bot_block[i + 4][j - 4] == bot_color)) bot_block[i + 2][j - 2] = -35;
                        if (bot_block[i + 1][j - 1] != 10 && bot_block[i + 1][j - 1] != -10 && bot_block[i + 1][j - 1] > -200 && (bot_block[i - 1][j + 1] == color || bot_block[i + 4][j - 4] == color)) bot_block[i + 1][j - 1] = -200;
                        if (bot_block[i + 2][j - 2] != 10 && bot_block[i + 2][j - 2] != -10 && bot_block[i + 2][j - 2] > -200 && (bot_block[i - 1][j + 1] == color || bot_block[i + 4][j - 4] == color)) bot_block[i + 2][j - 2] = -200;
                        else if (bot_block[i + 1][j - 1] != 10 && bot_block[i + 1][j - 1] != -10 && bot_block[i + 1][j - 1] > -170) bot_block[i + 1][j - 1] = -170;
                        else if (bot_block[i + 2][j - 2] != 10 && bot_block[i + 2][j - 2] != -10 && bot_block[i + 2][j - 2] > -170) bot_block[i + 2][j - 2] = -170;
                    }
                    if (difficulty >= 2)
                    {
                        if (!(block[i][j - 1] == bot_color && block[i][j + 2] == bot_color) && block[i][j] == block[i][j + 1] && block[i][j + 1] == color)
                        {
                            if (bot_block[i][j - 1] != 10 && bot_block[i][j - 1] != -10 && bot_block[i][j - 1] > -20) bot_block[i][j - 1] = -20;
                            if (bot_block[i][j + 2] != 10 && bot_block[i][j + 2] != -10 && bot_block[i][j + 2] > -20) bot_block[i][j + 2] = -20;
                        }
                        if (!(block[i - 1][j] == bot_color && block[i + 2][j] == bot_color) && block[i][j] == block[i + 1][j] && block[i + 1][j] == color)
                        {
                            if (bot_block[i - 1][j] != 10 && bot_block[i - 1][j] != -10 && bot_block[i - 1][j] > -20) bot_block[i - 1][j] = -20;
                            if (bot_block[i + 2][j] != 10 && bot_block[i + 2][j] != -10 && bot_block[i + 2][j] > -20) bot_block[i + 2][j] = -20;
                        }
                        if (!(block[i - 1][j - 1] == bot_color && block[i + 2][j + 2] == bot_color) && block[i][j] == block[i + 1][j + 1] && block[i + 1][j + 1] == color)
                        {
                            if (bot_block[i - 1][j - 1] != 10 && bot_block[i - 1][j - 1] != -10 && bot_block[i - 1][j - 1] > -20) bot_block[i - 1][j - 1] = -20;
                            if (bot_block[i + 2][j + 2] != 10 && bot_block[i + 2][j + 2] != -10 && bot_block[i + 2][j + 2] > -20) bot_block[i + 2][j + 2] = -20;
                        }
                        if (!(block[i - 1][j + 1] == bot_color && block[i + 2][j - 2] == bot_color) && block[i][j] == block[i + 1][j - 1] && block[i + 1][j - 1] == color)
                        {
                            if (bot_block[i - 1][j + 1] != 10 && bot_block[i - 1][j + 1] != -10 && bot_block[i - 1][j + 1] > -20) bot_block[i - 1][j + 1] = -20;
                            if (bot_block[i + 2][j - 2] != 10 && bot_block[i + 2][j - 2] != -10 && bot_block[i + 2][j - 2] > -20) bot_block[i + 2][j - 2] = -20;
                        }

                        if ((block[i][j - 1] == bot_color || block[i][j + 3] == bot_color) && block[i][j] == block[i][j + 1] && block[i][j + 1] == block[i][j + 2] && block[i][j + 2] == color)
                        {
                            if (bot_block[i][j - 1] != 10 && bot_block[i][j - 1] != -10 && block[i][j + 3] == bot_color && bot_block[i][j - 1] > -15) bot_block[i][j - 1] = -15;
                            if (bot_block[i][j + 3] != 10 && bot_block[i][j + 3] != -10 && block[i][j - 1] == bot_color && bot_block[i][j + 3] > -15) bot_block[i][j + 3] = -15;
                        }
                        if ((block[i - 1][j] == bot_color || block[i + 3][j] == bot_color) && block[i][j] == block[i + 1][j] && block[i + 1][j] == block[i + 2][j] && block[i + 2][j] == color)
                        {
                            if (bot_block[i - 1][j] != 10 && bot_block[i - 1][j] != -10 && block[i + 3][j] == bot_color && bot_block[i - 1][j] > -15) bot_block[i - 1][j] = -15;
                            if (bot_block[i + 3][j] != 10 && bot_block[i + 3][j] != -10 && block[i - 1][j] == bot_color && bot_block[i + 3][j] > -15) bot_block[i + 3][j] = -15;
                        }
                        if ((block[i - 1][j - 1] == bot_color || block[i + 3][j + 3] == bot_color) && block[i][j] == block[i + 1][j + 1] && block[i + 1][j + 1] == block[i + 2][j + 2] && block[i + 2][j + 2] == color)
                        {
                            if (bot_block[i - 1][j - 1] != 10 && bot_block[i - 1][j - 1] != -10 && block[i + 3][j + 3] == bot_color && bot_block[i - 1][j - 1] > -15) bot_block[i - 1][j - 1] = -15;
                            if (bot_block[i + 3][j + 3] != 10 && bot_block[i + 3][j + 3] != -10 && block[i - 1][j - 1] == bot_color && bot_block[i + 3][j + 3] > -15) bot_block[i + 3][j + 3] = -15;
                        }
                        if ((block[i - 1][j + 1] == bot_color || block[i + 3][j - 3] == bot_color) && block[i][j] == block[i + 1][j - 1] && block[i + 1][j - 1] == block[i + 2][j - 2] && block[i + 2][j - 2] == color)
                        {
                            if (bot_block[i - 1][j + 1] != 10 && bot_block[i - 1][j + 1] != -10 && block[i + 3][j - 3] == bot_color && bot_block[i - 1][j + 1] > -15) bot_block[i - 1][j + 1] = -15;
                            if (bot_block[i + 3][j - 3] != 10 && bot_block[i + 3][j - 3] != -10 && block[i - 1][j + 1] == bot_color && bot_block[i + 3][j - 3] > -15) bot_block[i + 3][j - 3] = -15;
                        }


                        if (!(block[i][j - 1] == bot_color || block[i][j + 3] == bot_color) && block[i][j] == block[i][j + 1] && block[i][j + 1] == block[i][j + 2] && block[i][j + 2] == color)
                        {
                            if (bot_block[i][j - 1] != 10 && bot_block[i][j - 1] != -10 && bot_block[i][j - 1] > -150) bot_block[i][j - 1] = -150;
                            if (bot_block[i][j + 3] != 10 && bot_block[i][j + 3] != -10 && bot_block[i][j + 3] > -150) bot_block[i][j + 3] = -150;
                        }
                        if (!(block[i - 1][j] == bot_color || block[i + 3][j] == bot_color) && block[i][j] == block[i + 1][j] && block[i + 1][j] == block[i + 2][j] && block[i + 2][j] == color)
                        {
                            if (bot_block[i - 1][j] != 10 && bot_block[i - 1][j] != -10 && bot_block[i - 1][j] > -150) bot_block[i - 1][j] = -150;
                            if (bot_block[i + 3][j] != 10 && bot_block[i + 3][j] != -10 && bot_block[i + 3][j] > -150) bot_block[i + 3][j] = -150;
                        }
                        if (!(block[i - 1][j - 1] == bot_color || block[i + 3][j + 3] == bot_color) && block[i][j] == block[i + 1][j + 1] && block[i + 1][j + 1] == block[i + 2][j + 2] && block[i + 2][j + 2] == color)
                        {
                            if (bot_block[i - 1][j - 1] != 10 && bot_block[i - 1][j - 1] != -10 && bot_block[i - 1][j - 1] > -150) bot_block[i - 1][j - 1] = -150;
                            if (bot_block[i + 3][j + 3] != 10 && bot_block[i + 3][j + 3] != -10 && bot_block[i + 3][j + 3] > -150) bot_block[i + 3][j + 3] = -150;
                        }
                        if (!(block[i - 1][j + 1] == bot_color || block[i + 3][j - 3] == bot_color) && block[i][j] == block[i + 1][j - 1] && block[i + 1][j - 1] == block[i + 2][j - 2] && block[i + 2][j - 2] == color)
                        {
                            if (bot_block[i - 1][j + 1] != 10 && bot_block[i - 1][j + 1] != -10 && bot_block[i - 1][j + 1] > -150) bot_block[i - 1][j + 1] = -150;
                            if (bot_block[i + 3][j - 3] != 10 && bot_block[i + 3][j - 3] != -10 && bot_block[i + 3][j - 3] > -150) bot_block[i + 3][j - 3] = -150;
                        }
                    }
                }
            }
        }
    }

    int min = 0;

    for (int i = 0; i < MAIN_X; i++)
    {
        for (int j = 0; j < MAIN_Y; j++)
        {
            if (bot_block[i][j] <= -30)
            {
                if (bot_block[i][j] < min)
                {
                    min = bot_block[i][j];
                }
            }
        }
    }

    return min * -1;
}