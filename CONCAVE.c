#include<stdio.h>
#include<windows.h>
#include<time.h>
#pragma warning(disable : 4996)


#define LEFT 75 //좌로 이동    //키보드값들 
#define RIGHT 77 //우로 이동 
#define UP 72 //위로 이동
#define DOWN 80 //아래로 이동
#define SPACE 32 // 확정
#define p 112 //일시정지 
#define P 80 //일시정지
#define ESC 27 //게임종료 

#define false 0
#define true 1

#define MAIN_X 23 //게임판 가로크기 
#define MAIN_Y 23 //게임판 세로크기 
#define MAIN_X_ADJ 3 //게임판 위치조정 
#define MAIN_Y_ADJ 3 //게임판 위치조정 

#define WHITE 1 //흰색 바둑돌
#define BLACK 2 //검정색 바둑돌
//FILE* fp;

int difficulty;
int sum, win_sum;
int color = 0;
int bot_color = 0;
int x, y; //이동중인 키의 게임판상의 x,y좌표를 저장
int key; //키보드로 입력받은 키값을 저장 
int turn_cnt = 0;       // 현재턴수를 저장
int title(void);            // 게임시작화면 
void reset(void);           // 게임판 관리
void draw_map(void);        // 게임 맵를 표시 
void draw_Interface(void);  // 인터페이스를 그림 
void upload_Interface(int bot_player);    // 인터페이스 업데이트
void check_key(int bot_player, int* x, int* y);       // 키보드로 키를 입력받음 
void rule();
int win_if();
void win_lose_print(int win_lose);
void bot_chose();
int bot_attack();
int bot_defence();
int bot_player_check();
int main_cnt = 0;

int block[MAIN_X][MAIN_Y] = { 0 };
int bot_block[MAIN_X][MAIN_Y] = { 0 };      // 가중치 저장 배열
int block_cnt = 0;


void gotoxy(int x, int y) { //gotoxy함수 
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서숨기는 함수에 사용되는 열거형 
void setcursortype(CURSOR_TYPE c) { //커서숨기는 함수 
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
    setcursortype(NOCURSOR); //커서 없앰 

    int main_robe;
    while (true)
    {
        system("cls");
        main_cnt++;
        main_robe = title(); //메인타이틀 호출 

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
                printf("┏");
            }
            else if (i == -2 && j == MAX_Y)
            {
                printf("┗");
            }
            else if (i == MAX_X && j == -2)
            {
                printf("┓");
            }
            else if (i == MAX_X && j == MAX_Y) // 바둑판 모양
            {
                printf("┛");
            }
            else if (i == -2 || i == MAX_X)
            {
                printf("┃");
            }
            else if (j == -2 || j == MAX_Y)
            {
                printf("━");
            }

            else if (i == -1 && j == -1)
            {
                printf("┏");
            }
            else if (i == -1 && j == MAIN_Y)
            {
                printf("┗");
            }
            else if (i == MAIN_X && j == -1)
            {
                printf("┓");
            }
            else if (i == MAIN_X && j == MAIN_Y) // 바둑판 모양
            {
                printf("┛");
            }
            else if (i == -1)
            {
                printf("├");
            }
            else if (j == -1)
            {
                printf("┬");
            }
            else if (i == MAIN_X)
            {
                printf("┤");
            }
            else if (j == MAIN_Y)
            {
                printf("┴");
            }
            else
            {
                printf("┼");
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


int title()// 타이틀
{
    int x = 21; //타이틀화면이 표시되는 x좌표 
    int y = 4; //타이틀화면이 표시되는 y좌표 
    int check = 1;
    if (main_cnt == 1)
    {
        gotoxy(x + 5, y + 2); printf("C");              Sleep(150);     // 게임 시작 시 띄워지는 로고
        gotoxy(x + 5, y + 2); printf("C ○");            Sleep(150);
        gotoxy(x + 5, y + 2); printf("C ○ N");          Sleep(150);
        gotoxy(x + 5, y + 2); printf("C ○ N C");        Sleep(150);
        gotoxy(x + 5, y + 2); printf("C ○ N C A");      Sleep(150);
        gotoxy(x + 5, y + 2); printf("C ○ N C A V");    Sleep(150);
        gotoxy(x + 5, y + 2); printf("C ○ N C A V E");  Sleep(150);
        for (int i = 0; i < 30; i++)
        {
            gotoxy(x + 6, i - 1);
            if (i != 7) printf("  ");
            gotoxy(x + 6, i);
            if (i < 7) printf("●");
            else printf("○");
            Sleep(50);
        }
        gotoxy(x + 6, 29);
        printf("  ");
    }
    
    gotoxy(x + 5, y + 2); printf("C ● N C A V E");;

    y = 10;
    x = 26;

    gotoxy(x, y - 1); printf("┌──────────┐"); //선택창
    gotoxy(x, y + 1); printf("└──────────┘");
    while (true)
    {

        gotoxy(27, 10); printf("S T A R T"); //시작
        gotoxy(27, 12); printf("DIFFICULT"); // 난이도
        gotoxy(27, 14); printf("E X   I T"); //나가기
        key = getch();

        if (key == SPACE) //check값에 따라서 메인 함수에 값을 반환
        {
            return check;
        }
        else if (key == DOWN && check < 3)
        {
            check++; //check값 1증가
            y += 2; //좌표값 2증가
            gotoxy(x, y - 3); printf("            ");
            gotoxy(x, y - 1); printf("┌──────────┐");
            gotoxy(x, y + 1); printf("└──────────┘");
        }
        else if (key == UP && check > 1)
        {
            check--; //check값 1감소
            y -= 2; //좌표값 2감소
            gotoxy(x, y + 3); printf("            ");
            gotoxy(x, y - 1); printf("┌──────────┐");
            gotoxy(x, y + 1); printf("└──────────┘");
        }

    }
}

void check_key(int bot_player, int* x, int* y) //플레이어의 선택에 따라 x, y값 증감
{
    int key; // 내가 선택한 키를 저장하는 변수


    while (true)
    {
        gotoxy(MAIN_X + 13, 17); printf("                         ");

        key = getch(); //입력

        if (key == DOWN && MAIN_Y + 2 > *y) // key == DOWN : 키가 아래 화살표와 같다면
        {                                   // MAIN_Y + 2 > *y : 게임창을 벗어나지 못하게 한다
            (*y)++;
            gotoxy(*x, *y - 1); printf("%s", (block[*x - 3][*y - 4] == 0) ? "┼ " // 이동하면 지워지는데 뒤에가 무엇이였는지를 판단하여 다시 출력
                : (block[*x - 3][*y - 4] == 1) ? "●" : "○");
        }
        else if (key == UP && 3 < *y)
        {
            (*y)--;
            gotoxy(*x, *y + 1); printf("%s", (block[*x - 3][*y - 2] == 0) ? "┼ " // 이동하면 지워지는데 뒤에가 무엇이였는지를 판단하여 다시 출력
                : (block[*x - 3][*y - 2] == 1) ? "●" : "○");;
        }

        else if (key == RIGHT && MAIN_X + 2 > *x)
        {
            (*x)++;
            gotoxy(*x - 1, *y); printf("%s", (block[*x - 4][*y - 3] == 0) ? "┼ " // 이동하면 지워지는데 뒤에가 무엇이였는지를 판단하여 다시 출력
                : (block[*x - 4][*y - 3] == 1) ? "●" : "○");;
        }
        else if (key == LEFT && 3 < *x)
        {
            (*x)--;
            gotoxy(*x + 1, *y); printf("%s", (block[*x - 2][*y - 3] == 0) ? "┼ " // 이동하면 지워지는데 뒤에가 무엇이였는지를 판단하여 다시 출력
                : (block[*x - 2][*y - 3] == 1) ? "●" : "○");
        }
        else if (key == ESC)
        {
            main();
        }

        if (bot_player == 2)
        {
            gotoxy(*x, *y); (color == 1) ? printf("▼") : printf("▽"); // 커서 위치를 확인하는 판별식
            if (key == SPACE && block[*x - 3][*y - 3] == 0)
            {
                block[*x - 3][*y - 3] = color; //배열에 위치 저장
                break;
            }
        }

        else if (bot_player == 1)
        {
            gotoxy((*x), (*y)); (turn_cnt % 2 == 0) ? printf("▼") : printf("▽"); // 커서 위치를 확인하는 판별식
            if (key == SPACE && block[*x - 3][*y - 3] == 0)
            {
                block[*x - 3][*y - 3] = (turn_cnt % 2 == 0) ? 1 : 2; //배열에 위치 저장
                break;
            }
        }
    }
}

void rule()
{

    system("cls");
    int x = 21; //타이틀화면이 표시되는 x좌표 
    int y = 3; //타이틀화면이 표시되는 y좌표 
    int check = 0;

    gotoxy(x + 4, y); printf("D I F F I C U L T");
    int cnt = 0;
    y = 7;
    x = 26;
    gotoxy(x, y + 1); printf("┌─────────┐");
    gotoxy(x, y + 3); printf("└─────────┘");

    while (true)
    {

        gotoxy(27, 9);   printf("E A  S Y");
        gotoxy(27, 11);  printf("ME DI UM");
        gotoxy(27, 13);  printf("H A  R D");
        gotoxy(x - 1, 9 + difficulty * 2); printf("▶");
        gotoxy(x + 6, 9 + difficulty * 2); printf("◁");
        key = getch();

        if (key == SPACE && cnt == 1) //check값에 따라서 메인 함수에 값을 반환
        {
            difficulty = check - 1;
        }
        else if (key == DOWN && check < 3)
        {
            cnt = 1;
            check++; //check값 1증가
            y += 2; //좌표값 2증가
            gotoxy(x, y - 3); printf("            ");
            gotoxy(x, y - 1); printf("┌─────────┐");
            gotoxy(x, y + 1); printf("└─────────┘");
        }
        else if (key == UP && check > 1)
        {
            check--; //check값 1감소
            y -= 2; //좌표값 2감소
            gotoxy(x, y + 3); printf("            ");
            gotoxy(x, y - 1); printf("┌─────────┐");
            gotoxy(x, y + 1); printf("└─────────┘");
        }
        else if (key == LEFT) // 키를 받았을 때 왼쪽 방향키라면 return을 통해 main으로 돌아간다
        {
            return;
        }
        gotoxy(x - 1, 9); printf("  ");
        gotoxy(x + 6, 9); printf("  ");
        gotoxy(x - 1, 11); printf("  ");
        gotoxy(x + 6, 11); printf("  ");
        gotoxy(x - 1, 13); printf("  ");
        gotoxy(x + 6, 13); printf("  ");
        gotoxy(x - 1, 9 + difficulty * 2); printf("▶");
        gotoxy(x + 6, 9 + difficulty * 2); printf("◁");

    }
}

void draw_Interface() // 인터페이스 그리는거 
{
    int x = MAIN_X + 11;
    int y = 7;
    gotoxy(x + 0, y - 4);  printf("    ┏────────────────────────────┓");
    gotoxy(x + 0, y - 3);  printf("     ◈◆ CURRENT TURN [    ] ◆◈  ");
    gotoxy(x + 0, y - 2);  printf("    ┗────────────────────────────┛");
    gotoxy(x + 0, y + 0);  printf("O〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓O");
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
    gotoxy(x + 0, y + 18); printf("O〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓O");
}


void upload_Interface(int bot_player) //턴이 돌 때마다 인터페이스의 정보를 업로드
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

int win_if() // 이기는 경우의 수
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

void win_lose_print(int win_lose)  // 이겼을 때 나오는 창
{

    x = 27;
    y = 10;
    gotoxy(x - 5, y - 2); printf("┏━━━━━━━━━━━━━━━━━━━━━━━━┓");
    gotoxy(x - 5, y - 1); printf("                           ");
    gotoxy(x - 5, y + 1); printf("                           ");
    gotoxy(x - 5, y + 2); printf("┗━━━━━━━━━━━━━━━━━━━━━━━━┛");

    if (win_lose == 1)
    {

        gotoxy(x - 5, y); printf("◇       BLACK WIN       ◇");


    }

    else if (win_lose == -1)
    {
        gotoxy(x - 5, y); printf("◆       WHITE WIN       ◆");
    }
    /* file_input(win_lose);*/
    Sleep(1500);
    key = getch();
}

int bot_player_check() // 모드 선택
{
    system("cls");

    gotoxy(27, 10); printf("P L A Y E R");
    gotoxy(27, 15); printf("B O T _ A I");

    gotoxy(25, 15); printf("  ");
    gotoxy(33, 15); printf("  ");
    gotoxy(25, 10); printf("▶");
    gotoxy(33, 10); printf("◁");

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
            gotoxy(25, 15); printf("▶");
            gotoxy(33, 15); printf("◁");
        }
        else if (key == UP && check > 1)
        {
            check--;

            gotoxy(25, 15); printf("  ");
            gotoxy(33, 15); printf("  ");
            gotoxy(25, 10); printf("▶");
            gotoxy(33, 10); printf("◁");
        }
    }
}

void bot_chose() // 봇을 골랐을 때
{
    ++turn_cnt;
    int val = 0; //최선수
    static int x = 0, y = 0;// 최선수의 좌표값

    if (turn_cnt != 2)
    {
        gotoxy(x + 3, y + 3); (bot_color == 1) ? printf("●") : printf("○");
    }

    for (int i = 0; i < MAIN_X; i++) //게임판 가로 크기
    {
        for (int j = 0; j < MAIN_Y; j++) //게임판 세로 크기
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

    block[x][y] = bot_color; //배열 저장
    bot_block[x][y] = 10;
    gotoxy(x + 3, y + 3); (bot_color == 1) ? printf("◆") : printf("○"); //출력
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