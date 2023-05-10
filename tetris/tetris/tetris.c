#define _CRT_SECURE_NO_WARNINGS //scanf, strcpy 오류 무시

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include <string.h>


/// 키보드 값들
#define LEFT 75 //좌측 화살표
#define RIGHT 77 //우측 화살표
#define UP 72 //위쪽 화살표
#define DOWN 80 //아래쪽 화살표
#define SPACE 32 //스페이스
#define ESC 27 //이스케이프

#define CURSOR_OFF "   "
#define CURSOR_ON_LEFT ">> "
#define CURSOR_ON_RIGHT " <<"

#define LEFT_CHAR "   ←  "
#define UP_CHAR "   ↑  "
#define RIGHT_CHAR "   →  "
#define DOWN_CHAR "   ↓  "
#define SPACE_CHAR "SPACE"
#define ENTER_CHAR "ENTER"
#define INVALID_CHAR "ERROR"


#define false 0
#define true 1

#define ACTIVE_BLOCK -2 // 게임판배열에 저장될 블록의 상태들 
#define CEILLING -1     // 블록이 이동할 수 있는 공간은 0 또는 음의 정수료 표현 
#define EMPTY 0         // 블록이 이동할 수 없는 공간은 양수로 표현 
#define WALL 1
#define INACTIVE_BLOCK 2 // 이동이 완료된 블록값 


#define XY_MAX 50
#define STATUS_X_ADJ(X_ADJ,X) X_ADJ+X+1 //게임정보표시 위치조정 

typedef enum { GAME_START = 0, KEY_SETTING, EXIT }TITLE_MENU;
//typedef enum { LEFT_MENU = 0, RIGHT_MENU, DOWN_MENU, HARD_DROP_MENU, ROTATE_MENU, ROTATE_COUNTER_MENU, HOLD_MENU, PAUSE_MENU, BACK_MENU, MAIN_MENU }SETTING_MENU;
typedef enum { LEFT_KEY = 0, RIGHT_KEY, DOWN_KEY, HARD_DROP_KEY, ROTATE_KEY, ROTATE_COUNTER_KEY, HOLD_KEY, BACK_KEY } KEY_TYPE;
/*
int STATUS_Y_GOAL; //GOAL 정보표시위치Y 좌표 저장
int STATUS_Y_LEVEL; //LEVEL 정보표시위치Y 좌표 저장
int STATUS_Y_SCORE; //SCORE 정보표시위치Y 좌표 저장
*/



int blocks[7][4][4][4] = {
{{0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},{0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},
 {0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},{0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0}}, //O

{{0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},{0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0},
{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}}, // I

{{1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0},{0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}}, // Z

{{0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0},{0,1,0,0,0,1,1,0,0,0,1,0,0,0,0,0},
 {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0}}, // S

{{0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0},
 {0,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0},{1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0}}, // L

{{1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},{0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0},
 {0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0},{0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0}}, // J

{{0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0},{0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0},
 {0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0},{0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0}} // T
}; //블록모양 저장 4*4공간에 블록을 표현 blcoks[b_type][b_rotation][i][j]로 사용 // 숫자가 커지면 시계방향 작아지면 반시계 방향

int wall_kick_data[2][4][2][5][2] = {
    {{{{0,0},{-2,0},{1,0},{-2,-1},{1,2}},
    {{0,0},{-1,0},{2,0},{-1,2},{2,-1}}},

    {{{0,0},{-1,0},{2,0},{-1,2},{2,-1}},
    {{0,0},{2,0},{-1,0},{2,1},{-1,-2}}},

    {{{0,0},{2,0},{-1,0},{2,1},{-1,-2}},
    {{0,0},{1,0},{-2,0},{1,-2},{-2,1}}},

    {{{0,0},{1,0},{-2,0},{1,-2},{1,2}},
    {{0,0},{-2,0},{1,0},{-2,-1},{1,2}}}},


    {{{{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}},
    {{0,0},{1,0},{1,1},{0,-2},{1,-2}}},

    {{{0,0},{1,0},{1,-1},{0,2},{1,2}},
    {{0,0},{1,0},{1,-1},{0,2},{1,2}}},

    {{{0,0},{1,0},{1,1},{0,-2},{1,-2}},
    {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}}},

    {{{0,0},{-1,0},{-1,-1},{0,2},{-1,2}},
    {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}}}}
}; // block index = 0 이면 회전 X, index =1 이면 0번표, 그외 1번표 참조 검사
//data[index참조][현재 rotation index][시계방향 - 0, 반시계방향 - 1][5][2] 


int b_type[7] = { 0,1,2,3,4,5,6 }; //테트리스 가방 정렬
int b_rotation; //블록 회전값 저장 
int b_now; // 현재 인덱스 저장 
int bx, by; //이동중인 블록의 게임판상의 x,y좌표를 저장 

int main_org[2][XY_MAX][XY_MAX]; //게임판의 정보를 저장하는 배열 모니터에 표시후에 main_cpy로 복사됨 
int main_cpy[2][XY_MAX][XY_MAX]; //즉 maincpy는 게임판이 모니터에 표시되기 전의 정보를 가지고 있음 
//main의 전체를 계속 모니터에 표시하지 않고(이렇게 하면 모니터가 깜빡거림) 
//main_cpy와 배열을 비교해서 값이 달라진 곳만 모니터에 고침 




int key; //키보드로 입력받은 키값을 저장 
int new_block_on = 0; //새로운 블럭이 필요함을 알리는 flag 
int crush_on = 0; //현재 이동중인 블록이 충돌상태인지 알려주는 flag 
int level_up_on = 0; //다음레벨로 진행(현재 레벨목표가 완료되었음을) 알리는 flag 
int space_key_on = 0; //hard drop상태임을 알려주는 flag 

int speed; //게임진행속도 
int level; //현재 level 
int level_goal; //다음레벨로 넘어가기 위한 목표점수 
int cnt; //현재 레벨에서 제거한 줄 수를 저장 
int score; //현재 점수 
int last_score = 0; //마지막게임점수 
int best_score = 0; //최고게임점수 

void title_scene(void);
void game_scene(void);

void reset_org(void); //main_org[][]를 초기화
void reset_cpy(void); // 게임판(main_cpy[][]를 초기화)

//콘솔창 draw/erase 함수
void draw_title_scene(void);
void draw_game_scene(void);
void draw_map(int x, int y); // (x,y)를 기준으로 맵을 그림.
void draw_interface(int x, int y); // (x,y)를 기준으로 기본인터페이스 그리기
void erase_scene(int x, int y, int dx, int dy);
void draw_block(int x, int y, int b_type, int b_rotation);



int new_block();// 새로운 블록의 key 가져오기
int check_key(void); // 키보드로 키 받아오기
int check_is_upper(int key); // 키보드로  받아온 키 대문자 검사
int check_is_rotatable(int* x, int* y); // 회전 가능 하다면 변경된 x,y 값을 기준으로 





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

int main() {

}


/*
#define _CRT_SECURE_NO_WARINGS

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>

#define false 0
#define true 1

#define ACTIVE_BLOCK -2 // 게임판배열에 저장될 블록의 상태들
#define CEILLING -1     // 블록이 이동할 수 있는 공간은 0 또는 음의 정수료 표현
#define EMPTY 0         // 블록이 이동할 수 없는 공간은 양수로 표현
#define WALL 1
#define INACTIVE_BLOCK 2 // 이동이 완료된 블록값

#define MAIN_X 11 //게임판 가로크기
#define MAIN_Y 23 //게임판 세로크기
#define MAIN_X_ADJ 3 //게임판 위치조정
#define MAIN_Y_ADJ 1 //게임판 위치조정

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 //게임정보표시 위치조정

#define CURSOR_OFF "   "
#define CURSOR_ON_LEFT ">> "
#define CURSOR_ON_RIGHT " <<"

#define LEFT_CHAR "   ←  "
#define UP_CHAR "   ↑  "
#define RIGHT_CHAR "   →  "
#define DOWN_CHAR "   ↓  "
#define SPACE_CHAR "SPACE"
#define ENTER_CHAR "ENTER"
#define INVALID_CHAR "ERROR"
#define ESC_CHAR " ESC "




#define ENTER 13 ///'\r'
#define LEFT 75 //좌로 이동    //키보드값들
#define RIGHT 77 //우로 이동
#define UP 72 //회전
#define DOWN 80 //soft drop
#define ESC 27

typedef enum { GAME_START = 0, KEY_SETTING, EXIT }TITLE_MENU;
typedef enum { LEFT_MENU = 0, RIGHT_MENU, DOWN_MENU, HARD_DROP_MENU, ROTATE_MENU, ROTATE_COUNTER_MENU, HOLD_MENU, PAUSE_MENU, BACK_MENU, MAIN_MENU }SETTING_MENU;

//키보드 값들 _ 변경가능
int left_key[2] = { 224,LEFT };
int right_key[2] = { 224, RIGHT };
int down_key[2] = { 224, DOWN };
int hard_drop_key[2] = { ' ', };
int rotate_key[2] = { 'X', };
int rotate_counter_key[2] = { 'Z', };
int hold_key[2] = { 'C', };
int pause_key[2] = { 'P', };
int esc_key[2] = { ESC, };

int STATUS_Y_GOAL; //GOAL 정보표시위치Y 좌표 저장
int STATUS_Y_LEVEL; //LEVEL 정보표시위치Y 좌표 저장
int STATUS_Y_SCORE; //SCORE 정보표시위치Y 좌표 저장

int blocks[7][4][4][4] = {
{{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
 {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}},
{{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}},
{{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0},
 {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0}},
{{0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0},{0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0}},
{{0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0},{0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0},{0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}}
}; //블록모양 저장 4*4공간에 블록을 표현 blcoks[b_type][b_rotation][i][j]로 사용

int b_type; //블록 종류를 저장
int b_rotation; //블록 회전값 저장
int b_type_next; //다음 블록값 저장

int main_org[MAIN_Y][MAIN_X]; //게임판의 정보를 저장하는 배열 모니터에 표시후에 main_cpy로 복사됨
int main_cpy[MAIN_Y][MAIN_X]; //즉 maincpy는 게임판이 모니터에 표시되기 전의 정보를 가지고 있음
//main의 전체를 계속 모니터에 표시하지 않고(이렇게 하면 모니터가 깜빡거림)
//main_cpy와 배열을 비교해서 값이 달라진 곳만 모니터에 고침
int bx, by; //이동중인 블록의 게임판상의 x,y좌표를 저장

int key; //키보드로 입력받은 키값을 저장

int speed; //게임진행속도
int level; //현재 level
int level_goal; //다음레벨로 넘어가기 위한 목표점수
int cnt; //현재 레벨에서 제거한 줄 수를 저장
int score; //현재 점수
int last_score = 0; //마지막게임점수
int best_score = 0; //최고게임점수

int new_block_on = 0; //새로운 블럭이 필요함을 알리는 flag
int crush_on = 0; //현재 이동중인 블록이 충돌상태인지 알려주는 flag
int level_up_on = 0; //다음레벨로 진행(현재 레벨목표가 완료되었음을) 알리는 flag
int space_key_on = 0; //hard drop상태임을 알려주는 flag

void title(void); //게임시작화면
void title_set(int x, int y, TITLE_MENU menu);
void setting(void);
void reset(void); //게임판 초기화
void reset_main(void); //메인 게임판(main_org[][]를 초기화)
void reset_main_cpy(void); //copy 게임판(main_cpy[][]를 초기화)
void draw_map(void); //게임 전체 인터페이스를 표시
void draw_main(void); //게임판을 그림
void new_block(void); //새로운 블록을 하나 만듦
void check_key(void); //키보드로 키를 입력받음
void drop_block(void); //블록을 아래로 떨어트림
int check_crush(int bx, int by, int rotation); //bx, by위치에 rotation회전값을 같는 경우 충돌 판단
void move_block(int dir); //dir방향으로 블록을 움직임
void check_line(void); //줄이 가득찼는지를 판단하고 지움
void check_level_up(void); //레벨목표가 달성되었는지를 판단하고 levelup시킴
void check_game_over(void); //게임오버인지 판단하고 게임오버를 진행
void pause(void);//게임을 일시정지시킴
void key_upper_check(char* key, char input);
char* key_string_set(char key);
void setting_set(int x, int y, SETTING_MENU menu);
void setting_key_setting(int x, int y, SETTING_MENU menu);
char* key_set(int* key);


void gotoxy(int x, int y) { //gotoxy함수
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR = 0, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서숨기는 함수에 사용되는 열거형

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



int main() {
    int i;

    srand((unsigned)time(NULL)); //난수표생성
    setcursortype(NOCURSOR); //커서 없앰
    title(); //메인타이틀 호출
}

void key_upper_check(char* key, char input) {
    if ('A' < input < 'Z') {
        input = input - 'A' + 'a';
    }
    *key = input;
}

void title(void) {
    int x = 5; //타이틀화면이 표시되는 x좌표
    int y = 4; //타이틀화면이 표시되는 y좌표
    int cnt; //타이틀 프레임을 세는 변수
    TITLE_MENU menu = GAME_START;

    key = 0;

    gotoxy(x, y + 0); printf("■□□□■■■□□■■□□■■"); Sleep(100);
    gotoxy(x, y + 1); printf("■■■□  ■□□    ■■□□■"); Sleep(100);
    gotoxy(x, y + 2); printf("□□□■              □■  ■"); Sleep(100);
    gotoxy(x, y + 3); printf("■■□■■  □  ■  □□■□□"); Sleep(100);
    gotoxy(x, y + 4); printf("■■  ■□□□■■■□■■□□"); Sleep(100);
    gotoxy(x + 5, y + 2); printf("T E T R I S"); Sleep(100);
    gotoxy(x, y + 6); printf("> Press Enter <");
    title_set(x, y, menu);

    for (cnt = 0;; cnt++) { //cnt를 1씩 증가시키면서 계속 반복    //하나도 안중요한 별 반짝이는 애니메이션효과
        if (_kbhit()) {
            key = _getch();
            if (key == ENTER) break;
            if (key == 224) {
                do { key = _getch(); } while (key == 224);//방향키지시값을 버림
                switch (key) {
                case DOWN: //아래쪽 방향키 눌렀을때-위와 동일하게 처리됨
                    menu = (menu + 1) % 3;
                    title_set(x, y, menu);
                    break;
                case UP: //위쪽 방향키 눌렀을때
                    menu = (menu + 2) % 3;
                    title_set(x, y, menu);
                }
            }
            while (_kbhit()) _getch();
        }
        if (cnt % 200 == 0) { gotoxy(x + 4, y + 1); printf("★"); }       //cnt가 200으로 나누어 떨어질때 별을 표시
        if ((cnt % 200 - 100) == 0) { gotoxy(x + 4, y + 1); printf("  "); } //위 카운트에서 100카운트 간격으로 별을 지움
        if ((cnt % 350) == 0) { gotoxy(x + 13, y + 2); printf("☆"); } //윗별과 같지만 시간차를 뒀음
        if ((cnt % 350 - 100) == 0) { gotoxy(x + 13, y + 2); printf("  "); }
        Sleep(10);
    }

    switch (menu) {
    case GAME_START:
        break;
    case KEY_SETTING:
        system("cls");
        setting();
        break;
    case EXIT:
        system("cls");
        exit(0);
        break;
    }

}

void title_set(int x, int y, TITLE_MENU menu) {
    switch (menu) {
    case GAME_START:
        gotoxy(x, y + 9); printf(CURSOR_ON_LEFT); printf("Game Start"); printf(CURSOR_ON_RIGHT);
        gotoxy(x, y + 11); printf(CURSOR_OFF); printf("Key Setting"); printf(CURSOR_OFF);
        gotoxy(x, y + 13); printf(CURSOR_OFF); printf("Exit"); printf(CURSOR_OFF);
        break;
    case KEY_SETTING:
        gotoxy(x, y + 9); printf(CURSOR_OFF); printf("Game Start"); printf(CURSOR_OFF);
        gotoxy(x, y + 11); printf(CURSOR_ON_LEFT); printf("Key Setting"); printf(CURSOR_ON_RIGHT);
        gotoxy(x, y + 13); printf(CURSOR_OFF); printf("Exit"); printf(CURSOR_OFF);
        break;
    case EXIT:
        gotoxy(x, y + 9); printf(CURSOR_OFF); printf("Game Start"); printf(CURSOR_OFF);
        gotoxy(x, y + 11); printf(CURSOR_OFF); printf("Key Setting"); printf(CURSOR_OFF);
        gotoxy(x, y + 13); printf(CURSOR_ON_LEFT); printf("Exit"); printf(CURSOR_ON_RIGHT);
    }
}

void setting() {
    int x = 5; //타이틀화면이 표시되는 x좌표
    int y = 3; //타이틀화면이 표시되는 y좌표
    SETTING_MENU menu = LEFT_MENU;
    gotoxy(x, y + 0); printf("▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤");
    gotoxy(x, y + 1); printf("▤                                     ▤");
    gotoxy(x, y + 2); printf("▤      +-----------------------+      ▤");
    gotoxy(x, y + 3); printf("▤      |     S E T T I N G     |      ▤");
    gotoxy(x, y + 4); printf("▤      +-----------------------+      ▤");
    gotoxy(x, y + 5); printf("▤                                     ▤");
    gotoxy(x, y + 17); printf("▤                                     ▤");
    gotoxy(x, y + 18); printf("▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤ ▤");
    setting_key_setting(x, y, menu);


}

void setting_new_key(int x, int y, int* key_set, SETTING_MENU menu) {
    while (1) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) {
                do { key = _getch(); } while (key == 224);//방향키지시값을 버림
                switch (key) {
                case DOWN: //아래쪽 방향키 눌렀을때-위와 동일하게 처리됨
                    gotoxy(x + 14, y + 6 + menu); printf("%s", DOWN_CHAR);
                    key_set[0] = 224;
                    key_set[1] = DOWN;
                    setting_key_setting(x, y, menu);
                    break;

                }
            }

            while (_kbhit()) _getch();
        }
    }
    //gotoxy(x+14, y); printf("S");
    //gotoxy(x+13, y); printf("ERROR");
}

void setting_key_setting(int x, int y, SETTING_MENU menu) {
    setting_set(x, y, menu);
    while (1) {
        if (_kbhit()) {
            key = _getch();
            if (key == ENTER) break;
            if (key == 224) {
                do { key = _getch(); } while (key == 224);//방향키지시값을 버림
                switch (key) {
                case DOWN: //아래쪽 방향키 눌렀을때-위와 동일하게 처리됨
                    menu = (menu + 1) % 10;
                    setting_set(x, y, menu);
                    break;
                case UP: //위쪽 방향키 눌렀을때
                    menu = (menu + 9) % 10;
                    setting_set(x, y, menu);
                }
            }
            while (_kbhit()) _getch();
        }
    }
    switch (menu) {
    case LEFT_MENU:
        gotoxy(x, y + 6); printf("▤  %s    LEFT     :     %6s  %s  ▤", " ->", key_set(left_key), " <-");
        setting_new_key(x, y, left_key, menu);
        break;
    case RIGHT_MENU:
        gotoxy(x, y + 7); printf("▤  %s   RIGHT     :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(right_key), CURSOR_ON_RIGHT);
        break;
    case DOWN_MENU:
        gotoxy(x, y + 8); printf("▤  %s    DOWN     :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(down_key), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_MENU:
        gotoxy(x, y + 9); printf("▤  %s HARD DROP   :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(hard_drop_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_MENU:
        gotoxy(x, y + 10); printf("▤  %s    회전     :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(rotate_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_MENU:
        gotoxy(x, y + 11); printf("▤  %s반시계 회전  :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(rotate_counter_key), CURSOR_ON_RIGHT);
        break;
    case HOLD_MENU:
        gotoxy(x, y + 12); printf("▤  %s    HOLD     :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(hold_key), CURSOR_ON_RIGHT);
        break;
    case PAUSE_MENU:
        gotoxy(x, y + 13); printf("▤  %s   PAUSE     :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(pause_key), CURSOR_ON_RIGHT);
        break;
    case BACK_MENU:
        gotoxy(x, y + 14); printf("▤  %s    BACK     :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(esc_key), CURSOR_ON_RIGHT);
        break;
    case MAIN_MENU:
        system("cls");
        title();
        break;

    }
}

char* key_set(int* key) {
    if (key[0] == 224) {
        switch (key[1]) {
        case LEFT:
            return LEFT_CHAR;
        case RIGHT:
            return RIGHT_CHAR;
        case UP:
            return UP_CHAR;
        case DOWN:
            return DOWN_CHAR;
        }
    }
    if (key[0] == -1) {
        return INVALID_CHAR;
    }
    else if (key[0] == ' ') {
        return SPACE_CHAR;
    }
    else if (key[0] == '\r') {
        return ENTER_CHAR;
    }
    else if (key[0] == ESC) {
        return ESC_CHAR;
    }
    else {
        return key_string_set(key[0]);
    }
}

char* key_string_set(int key) {
    static char key_string[6] = { "     " };
    key_string[2] = key;
    return key_string;
}

void setting_set(int x, int y, SETTING_MENU menu) {
    gotoxy(x, y + 6); printf("▤  %s    LEFT     :     %6s  %s  ▤", CURSOR_OFF, key_set(left_key), CURSOR_OFF);
    gotoxy(x, y + 7); printf("▤  %s   RIGHT     :     %6s  %s  ▤", CURSOR_OFF, key_set(right_key), CURSOR_OFF);
    gotoxy(x, y + 8); printf("▤  %s    DOWN     :     %6s  %s  ▤", CURSOR_OFF, key_set(down_key), CURSOR_OFF);
    gotoxy(x, y + 9); printf("▤  %s HARD DROP   :     %6s  %s  ▤", CURSOR_OFF, key_set(hard_drop_key), CURSOR_OFF);
    gotoxy(x, y + 10); printf("▤  %s    회전     :     %6s  %s  ▤", CURSOR_OFF, key_set(rotate_key), CURSOR_OFF);
    gotoxy(x, y + 11); printf("▤  %s반시계 회전  :     %6s  %s  ▤", CURSOR_OFF, key_set(rotate_counter_key), CURSOR_OFF);
    gotoxy(x, y + 12); printf("▤  %s    HOLD     :     %6s  %s  ▤", CURSOR_OFF, key_set(hold_key), CURSOR_OFF);
    gotoxy(x, y + 13); printf("▤  %s   PAUSE     :     %6s  %s  ▤", CURSOR_OFF, key_set(pause_key), CURSOR_OFF);
    gotoxy(x, y + 14); printf("▤  %s    BACK     :     %6s  %s  ▤", CURSOR_OFF, key_set(esc_key), CURSOR_OFF);
    gotoxy(x, y + 15); printf("▤                                     ▤");
    gotoxy(x, y + 16); printf("▤     %s    BACK TO MAIN     %s     ▤", CURSOR_OFF, CURSOR_OFF);

    switch (menu) {
    case LEFT_MENU:
        gotoxy(x, y + 6); printf("▤  %s    LEFT     :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(left_key), CURSOR_ON_RIGHT);
        break;
    case RIGHT_MENU:
        gotoxy(x, y + 7); printf("▤  %s   RIGHT     :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(right_key), CURSOR_ON_RIGHT);
        break;
    case DOWN_MENU:
        gotoxy(x, y + 8); printf("▤  %s    DOWN     :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(down_key), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_MENU:
        gotoxy(x, y + 9); printf("▤  %s HARD DROP   :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(hard_drop_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_MENU:
        gotoxy(x, y + 10); printf("▤  %s    회전     :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(rotate_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_MENU:
        gotoxy(x, y + 11); printf("▤  %s반시계 회전  :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(rotate_counter_key), CURSOR_ON_RIGHT);
        break;
    case HOLD_MENU:
        gotoxy(x, y + 12); printf("▤  %s    HOLD     :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(hold_key), CURSOR_ON_RIGHT);
        break;
    case PAUSE_MENU:
        gotoxy(x, y + 13); printf("▤  %s   PAUSE     :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(pause_key), CURSOR_ON_RIGHT);
        break;
    case BACK_MENU:
        gotoxy(x, y + 14); printf("▤  %s    BACK     :     %6s  %s  ▤", CURSOR_ON_LEFT, key_set(esc_key), CURSOR_ON_RIGHT);
        break;
    case MAIN_MENU:
        gotoxy(x, y + 16); printf("▤     %s    BACK TO MAIN     %s     ▤", CURSOR_ON_LEFT, CURSOR_ON_RIGHT);
        break;
    }



}

*/