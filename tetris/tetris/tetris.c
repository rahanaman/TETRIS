#define _CRT_SECURE_NO_WARNINGS //scanf, strcpy 오류 무시

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include <string.h>


/// 키보드 값들
#define LEFT 75 // 좌측 화살표
#define RIGHT 77 // 우측 화살표
#define UP 72 // 위쪽 화살표
#define DOWN 80 // 아래쪽 화살표
#define SPACE 32 // 스페이스
#define ESC 27 // 이스케이프
#define ENTER 13 // 엔터

#define CURSOR_OFF "   "
#define CURSOR_ON_LEFT ">> "
#define CURSOR_ON_RIGHT " <<"

#define LEFT_CHAR "   ← "
#define UP_CHAR "   ↑ "
#define RIGHT_CHAR "   → "
#define DOWN_CHAR "   ↓ "
#define SPACE_CHAR "SPACE"
#define ENTER_CHAR "ENTER"
#define ESC_CHAR " ESC "
#define INVALID_CHAR "ERROR"


#define false 0
#define true 1


#define P1 0
#define P2 1

#define MAIN_X_1 11
#define MAIN_Y_1 23


#define ACTIVE_BLOCK -2 // 게임판배열에 저장될 블록의 상태들 
#define CEILLING -1     // 블록이 이동할 수 있는 공간은 0 또는 음의 정수료 표현 
#define EMPTY 0         // 블록이 이동할 수 없는 공간은 양수로 표현 
#define WALL 1
#define INACTIVE_BLOCK 2 // 이동이 완료된 블록값 


#define XY_MAX 50

typedef enum { GAME_START = 0, KEY_SETTING, EXIT }TITLE_MENU;
typedef enum { LEFT_KEY = 0, RIGHT_KEY, DOWN_KEY, HARD_DROP_KEY, ROTATE_KEY, ROTATE_COUNTER_KEY, HOLD_KEY, BACK_KEY } KEY_TYPE;


/*
int STATUS_Y_GOAL; //GOAL 정보표시위치Y 좌표 저장
int STATUS_Y_LEVEL; //LEVEL 정보표시위치Y 좌표 저장
int STATUS_Y_SCORE; //SCORE 정보표시위치Y 좌표 저장
*/

int left_key[2] = { 128 + LEFT, '4'};
int right_key[2] = { 128 + RIGHT ,'6'};
int down_key[2] = { 128 + DOWN, '5'};
int hard_drop_key[2] = { ' ', '0'};
int rotate_key[2] = { 'X', '9'};
int rotate_counter_key[2] = {'Z', '7'};
int hold_key[2] = { 'C',ENTER};
int pause_key = 'P';
int esc_key = ESC;

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
//dx는 더하고, dy는 빼야함 주요요요요용


int b_type[2][2][7] = { { { 0,1,2,3,4,5,6 } ,{ 0,1,2,3,4,5,6 } } ,{ { 0,1,2,3,4,5,6 } ,{ 0,1,2,3,4,5,6 } } }; // 테트리스 가방 정렬
int b_rotation[2]; // 블록 회전값 저장 
int b_now[2]; // 현재 인덱스 저장 
int bx[2], by[2]; // 이동중인 블록의 게임판상의 x,y좌표를 저장 


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
void draw_title_scene(int x, int y, TITLE_MENU menu);


void mode_select_scene(void);



//콘솔창 draw/erase 함수

void draw_game_scene(void);
void draw_interface(int x, int y); // (x,y)를 기준으로 기본인터페이스 그리기
void erase_scene(int x, int y, int dx, int dy);
void draw_block(int x, int y, int type, int rotation);

char* key_string_set(int key);
char* key_set(int key);
// 스트링 타입으로 변환하는 함수

void setting_scene(void);
void draw_setting_scene_player(int x, int y, int player);
void setting_scene_set(int x, int y, KEY_TYPE type, int player); // 옵션 받아오기
void draw_setting_scene(int x, int y, KEY_TYPE type, int player);
void setting_new_key(int x, int y, int* key_set, KEY_TYPE type, int player);

void reset_org_cpy(int player, int dx, int dy); //main_org[][]를 초기화, dx, dy만큼
void save_org_cpy(int player, int dx, int dy); // 게임판(main_cpy[][]를 초기화)
void shuffle_block(int player);
/*셔플 루틴
* b_now++ 하고
* b_now = 7일 경우 
*배열1 다 사용하면
*배열 2 -> 배열1로 복사
*배열2 재셔플
*초기 셔플 -> 두 번 수행해야함
* b_now =0으로 바꾸기
*/


void game_2p_battle_scene(void);
void draw_game_2p_battle_scene(void);
void draw_next(int x, int y, int player);
void draw_map(int x, int y, int player, int dx, int dy);



int new_block();// 새로운 블록의 key 가져오기
int check_key(void); // 키보드로 키 받아오기
int check_is_upper(int key); // 키보드로  받아온 키 대문자 검사
int check_is_rotatable(int* x, int* y); // 회전 가능 하다면 변경된 x,y 값을 기준으로 변경된 좌표를 기준으로 테트리스 배치

unsigned main_theme(void* arg);
int bpm = 125; // 16분 음표 bpm
//1 bpm 16 2 bpm 8 4bpm 4


unsigned main_theme(void* arg) {
    while (1) {
        Beep(1318.5100f, 4 * bpm);
        Beep(987.7666f, 2 * bpm);
        Beep(1046.5020f, 2 * bpm);
        Beep(1174.6590f, 2 * bpm);
        Beep(1318.5100f, bpm);
        Beep(1174.6590f, bpm);
        Beep(1046.5020f, 2 * bpm);
        Beep(987.7666f, 2 * bpm);
        Beep(880.0000f, 4 * bpm);
        Beep(880.0000f, 2 * bpm);
        Beep(1046.5020f, 2 * bpm);
        Beep(1318.5100f, 4 * bpm);
        Beep(1174.6590f, 2 * bpm);
        Beep(1046.5020f, 2 * bpm);
        Beep(987.7666f, 4 * bpm);
        Beep(987.7666f, bpm);
        Beep(987.7666f, bpm);
        Beep(1046.5020f, 2 * bpm);
        Beep(1174.6590f, 4 * bpm);
        Beep(1318.5100f, 4 * bpm);
        Beep(1046.5020f, 4 * bpm);
        Beep(880.0000f, 4 * bpm);
        Beep(880.0000f, 6 * bpm);

        Beep(1174.6590f, 4 * bpm);
        Beep(1174.6590f, 2 * bpm);
        Beep(1396.9130f, 2 * bpm);
        Beep(1760.0000f, 4 * bpm);
        Beep(1567.9820f, 2 * bpm);
        Beep(1396.9130f, 2 * bpm);
        Beep(1318.5100f, 4 * bpm);
        Beep(1318.5100f, 2 * bpm);
        Beep(1046.5020f, 2 * bpm);
        Beep(1318.5100f, 4 * bpm);
        Beep(1174.6590f, 2 * bpm);
        Beep(1046.5020f, 2 * bpm);
        Beep(987.7666f, 4 * bpm);
        Beep(987.7666f, 2*bpm);
        Beep(1046.5020f, 2 * bpm);
        Beep(1174.6590f, 4 * bpm);
        Beep(1318.5100f, 4 * bpm);
        Beep(1046.5020f, 4 * bpm);
        Beep(880.0000f, 4 * bpm);
        Beep(880.0000f, 6 * bpm);

        Beep(1318.5100f, 8 * bpm);
        Beep(1046.5020f, 8 * bpm);
        Beep(1174.6590f, 8 * bpm);
        Beep(987.7666f, 8 * bpm);
        Beep(1046.5020f, 8 * bpm);
        Beep(880.0000f, 8 * bpm);
        Beep(830.6100f, 8 * bpm);
        Beep(987.7666f, 8 * bpm);
        Beep(1318.5100f, 8 * bpm);
        Beep(1046.5020f, 8 * bpm);
        Beep(1174.6590f, 8 * bpm);
        Beep(987.7666f, 8 * bpm);
        Beep(1046.5020f, 4 * bpm);
        Beep(1318.5100f, 4 * bpm);
        Beep(1760.0000f, 4 * bpm);
        Beep(1760.0000f, 4 * bpm);
        Beep(1661.2200f, 8 * bpm);


    }


}



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
    HANDLE hThrd;
    //main_theme(0);
    _beginthreadex(NULL, 0, main_theme, 0,0,NULL);

    int i;
    srand((unsigned)time(NULL)); //난수표생성
    setcursortype(NOCURSOR); //커서 없앰

    title_scene(); //메인타이틀 호출
}

void draw_title_scene(int x, int y, TITLE_MENU menu) {
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


void title_scene(void) {
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
    gotoxy(x, y + 6); printf(" Press Enter ");
    draw_title_scene(x, y, menu);

    for (cnt = 0;; cnt++) { //cnt를 1씩 증가시키면서 계속 반복    //하나도 안중요한 별 반짝이는 애니메이션효과
        if (_kbhit()) {
            key = _getch();
            if (key == ENTER) break;
            if (key == 224) {
                do { key = _getch(); } while (key == 224);//방향키지시값을 버림
                switch (key) {
                case DOWN: //아래쪽 방향키 눌렀을때-위와 동일하게 처리됨
                    menu = (menu + 1) % 3;
                    draw_title_scene(x, y, menu);
                    break;
                case UP: //위쪽 방향키 눌렀을때
                    menu = (menu + 2) % 3;
                    draw_title_scene(x, y, menu);
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
        system("cls");
        game_2p_battle_scene(); // 나중에 모드로 수정예정
        break;
    case KEY_SETTING:
        system("cls");
        setting_scene();
        break;
    case EXIT:
        system("cls");
        exit(0);
        break;
    }

}

//수정예정 - 세팅 키 입력 덜 작성함
void setting_scene() {
    int x = 5; //타이틀화면이 표시되는 x좌표
    int y = 3; //타이틀화면이 표시되는 y좌표
    KEY_TYPE key = LEFT_KEY;
    int player = P1;
    draw_setting_scene_player(x, y, player);
    setting_scene_set(x, y, key,player);
}

void setting_scene_set(int x, int y, KEY_TYPE type, int player) {
    draw_setting_scene(x, y, type,player);
    while (1) {
        if (_kbhit()) {
            key = _getch();
            if (key == ESC) {
                system("cls");
                title_scene();
            }
            if (key == ENTER) break;
            if (key == 224) {
                do { key = _getch(); } while (key == 224);//방향키지시값을 버림
                switch (key) {
                case DOWN: //아래쪽 방향키 눌렀을때-위와 동일하게 처리됨
                    type = (type + 1) % 7;
                    while (_kbhit()) _getch();
                    draw_setting_scene(x, y, type,player);
                    break;
                case UP: //위쪽 방향키 눌렀을때
                    type = (type + 6) % 7;
                    while (_kbhit()) _getch();
                    draw_setting_scene(x, y, type,player);
                    break;
                case RIGHT:
                case LEFT:
                    player = 1 - player;
                    draw_setting_scene_player(x, y, player);
                    draw_setting_scene(x, y, type,player);
                    break;

                }
            }

        }
    }
    switch (type) {
    case LEFT_KEY:
        gotoxy(x, y + 6); printf("▤  %s    LEFT     :     %6s  %s   ▤", " ->", key_set(left_key[player]), " <-");
        //setting_new_key(x, y, &left_key[player], type, player);
        break;
    case RIGHT_KEY:
        gotoxy(x, y + 7); printf("▤  %s   RIGHT     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(right_key[player]), CURSOR_ON_RIGHT);
        break;
    case DOWN_KEY:
        gotoxy(x, y + 8); printf("▤  %s    DOWN     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(down_key[player]), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_KEY:
        gotoxy(x, y + 9); printf("▤  %s HARD DROP   :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(hard_drop_key[player]), CURSOR_ON_RIGHT);
        break;
    case ROTATE_KEY:
        gotoxy(x, y + 10); printf("▤  %s    회전     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(rotate_key[player]), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_KEY:
        gotoxy(x, y + 11); printf("▤  %s반시계 회전  :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(rotate_counter_key[player]), CURSOR_ON_RIGHT);
        break;
    case HOLD_KEY:
        gotoxy(x, y + 12); printf("▤  %s    HOLD     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(hold_key[player]), CURSOR_ON_RIGHT);
        break;
    }
}

void draw_setting_scene_player(int x, int y, int player) {
    if (player == P1) {
        gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
        gotoxy(x, y + 1); printf("▤                                      ▤");
        gotoxy(x, y + 2); printf("▤      +-----------------------+       ▤");
        gotoxy(x, y + 3); printf("▤      |   S E T T I N G - P1  |       ▤");
        gotoxy(x, y + 4); printf("▤      +-----------------------+       ▤");
        gotoxy(x, y + 5); printf("▤                                      ▤");
        gotoxy(x, y + 15); printf("▤                                      ▤");
        gotoxy(x, y + 16); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
    }
    if (player == P2) {
        gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
        gotoxy(x, y + 1); printf("▤                                      ▤");
        gotoxy(x, y + 2); printf("▤      +-----------------------+       ▤");
        gotoxy(x, y + 3); printf("▤      |   S E T T I N G - P2  |       ▤");
        gotoxy(x, y + 4); printf("▤      +-----------------------+       ▤");
        gotoxy(x, y + 5); printf("▤                                      ▤");
        gotoxy(x, y + 15); printf("▤                                      ▤");
        gotoxy(x, y + 16); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
    }
}

void setting_new_key(int x, int y, int* key_set, KEY_TYPE type, int player) {
    while (1) {
        if (_kbhit()) {
            key = _getch();
            
            if (key == 224) {
                do { key = _getch(); } while (key == 224);//방향키지시값을 버림
                key_set = 180 + DOWN;
                draw_setting_scene(x, y, type,player);
            }

            while (_kbhit()) _getch();
        }
    }
}

char* key_set(int key) {
    if (key > 128) {
        switch (key-128) {
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
    if (key == -1) {
        return INVALID_CHAR;
    }
    else if (key == ' ') {
        return SPACE_CHAR;
    }
    else if (key == ENTER) {
        return ENTER_CHAR;
    }
    else if (key == ESC) {
        return ESC_CHAR;
    }
    else {
        return key_string_set(key);
    }
}

char* key_string_set(int key) {
    static char key_string[6] = { "     " };
    key_string[2] = key;
    return key_string;
}

void draw_setting_scene(int x, int y, KEY_TYPE type,int player) {
    gotoxy(x, y + 6); printf("▤  %s    LEFT     :     %6s  %s   ▤", CURSOR_OFF, key_set(left_key[player]), CURSOR_OFF);
    gotoxy(x, y + 7); printf("▤  %s   RIGHT     :     %6s  %s   ▤", CURSOR_OFF, key_set(right_key[player]), CURSOR_OFF);
    gotoxy(x, y + 8); printf("▤  %s    DOWN     :     %6s  %s   ▤", CURSOR_OFF, key_set(down_key[player]), CURSOR_OFF);
    gotoxy(x, y + 9); printf("▤  %s HARD DROP   :     %6s  %s   ▤", CURSOR_OFF, key_set(hard_drop_key[player]), CURSOR_OFF);
    gotoxy(x, y + 10); printf("▤  %s    회전     :     %6s  %s   ▤", CURSOR_OFF, key_set(rotate_key[player]), CURSOR_OFF);
    gotoxy(x, y + 11); printf("▤  %s반시계 회전  :     %6s  %s   ▤", CURSOR_OFF, key_set(rotate_counter_key[player]), CURSOR_OFF);
    gotoxy(x, y + 12); printf("▤  %s    HOLD     :     %6s  %s   ▤", CURSOR_OFF, key_set(hold_key[player]), CURSOR_OFF);
    gotoxy(x, y + 13); printf("▤                                      ▤");
    gotoxy(x, y + 14); printf("▤      PRESS ESC TO BACK TO MAIN       ▤");


    switch (type) {
    case LEFT_KEY:
        gotoxy(x, y + 6); printf("▤  %s    LEFT     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(left_key[player]), CURSOR_ON_RIGHT);
        break;
    case RIGHT_KEY:
        gotoxy(x, y + 7); printf("▤  %s   RIGHT     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(right_key[player]), CURSOR_ON_RIGHT);
        break;
    case DOWN_KEY:
        gotoxy(x, y + 8); printf("▤  %s    DOWN     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(down_key[player]), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_KEY:
        gotoxy(x, y + 9); printf("▤  %s HARD DROP   :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(hard_drop_key[player]), CURSOR_ON_RIGHT);
        break;
    case ROTATE_KEY:
        gotoxy(x, y + 10); printf("▤  %s    회전     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(rotate_key[player]), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_KEY:
        gotoxy(x, y + 11); printf("▤  %s반시계 회전  :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(rotate_counter_key[player]), CURSOR_ON_RIGHT);
        break;
    case HOLD_KEY:
        gotoxy(x, y + 12); printf("▤  %s    HOLD     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(hold_key[player]), CURSOR_ON_RIGHT);
        break;

    }
}

//수정예정 - 세팅 키 입력 덜 작성함


void reset_org_cpy(int player, int dx, int dy) {
    int i, j;
    for (i = 0; i < dy; ++i) {
        for (j = 0; j < dx; ++j) {
            main_org[player][i][j] = EMPTY;
            main_cpy[player][i][j] = 100;
        }
    }
    for (j = 0; j < dx; ++j) {
        main_org[player][3][j] = CEILLING;
    }
    for (i = 1; i < dy - 1; i++) { //좌우 벽을 만듦  
        main_org[player][i][0] = WALL;
        main_org[player][i][dx-1] = WALL;
    }
    for (j = 0; j < dx; ++j) { //바닥벽을 만듦 
        main_org[player][dy-1][j] = WALL;
    }
}



void save_org_cpy(int player , int dx, int dy) {
    for (int i = 0; i < dx; i++) { //게임판을 그린 후 main_cpy에 복사  
        for (int j = 0; j < dy; j++) {
            main_cpy[player][i][j] = main_org[player][i][j];
        }
    }
}

void shuffle_block(int player)
{

    for (int i = 0; i < 7; ++i) {
        b_type[player][0][i] = b_type[player][1][i];
    }
    int n, temp;
    for(int i = 0; i < 6; ++i) {
        n = rand() % (7 - i) + i;    // i 부터 num-1 사이에 임의의 정수 생성
        temp =b_type[player][1][i];
        b_type[player][1][i] = b_type[player][1][n];
        b_type[player][1][n] = temp;
    }
    
}


void game_2p_battle_scene(void) {
    //리셋 시작
    b_now[P1] = 0;
    b_now[P2] = 0;
    reset_org_cpy(P1,MAIN_X_1,MAIN_Y_1);
    reset_org_cpy(P2,MAIN_X_1,MAIN_Y_1);
    shuffle_block(P1);
    shuffle_block(P1);
    shuffle_block(P2);
    shuffle_block(P2);
    //리셋 끝
    draw_game_2p_battle_scene();
}

void draw_game_2p_battle_scene(void) {
    int x = 3, y = 1;
    draw_map(x,y,P1, MAIN_X_1, MAIN_Y_1);
    draw_next(x+MAIN_X_1+1,y+2,P1, MAIN_X_1, MAIN_Y_1);
    draw_map(x+MAIN_X_1+10+x , y, P2, MAIN_X_1, MAIN_Y_1);
    draw_next(2*x + 2*MAIN_X_1 + 11, y + 2, P2, MAIN_X_1, MAIN_Y_1);
}

void draw_block(int x, int y, int type, int rotation) {

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (blocks[type][rotation][i][j] == 1) {
                gotoxy(x + j, y + i); printf("■");
            }
        }
    }
}

void draw_next(int x, int y, int player) {
    gotoxy(x, y ); printf("+-  N E X T  -+ ");
    gotoxy(x, y + 1); printf("|             | ");
    gotoxy(x, y + 2); printf("|             | ");
    gotoxy(x, y + 3); printf("+-- -  -  - --+ ");
    gotoxy(x, y + 4); printf("+-  N E X T  -+ ");
    gotoxy(x, y + 5); printf("|             | ");
    gotoxy(x, y + 6); printf("|             | ");
    gotoxy(x, y + 7); printf("+-- -  -  - --+ ");
    gotoxy(x, y + 8); printf("+-  N E X T  -+ ");
    gotoxy(x, y + 9); printf("|             | ");
    gotoxy(x, y + 10); printf("|             | ");
    gotoxy(x, y + 11); printf("+-- -  -  - --+ ");
    gotoxy(x, y + 12); printf("+-  N E X T  -+ ");
    gotoxy(x, y + 13); printf("|             | ");
    gotoxy(x, y + 14); printf("|             | ");
    gotoxy(x, y + 15); printf("+-- -  -  - --+ ");
    gotoxy(x, y + 16); printf("+-  N E X T  -+ ");
    gotoxy(x, y + 17); printf("|             | ");
    gotoxy(x, y + 18); printf("|             | ");
    gotoxy(x, y + 19); printf("+-- -  -  - --+ ");
    draw_block(x + 2, y + 1, *((*b_type[player]) + b_now[player] + 1), 0);
    draw_block(x + 2, y + 5, *((*b_type[player]) + b_now[player] + 2), 0);
    draw_block(x + 2, y + 9, *((*b_type[player]) + b_now[player] + 3), 0);
    draw_block(x + 2, y + 13, *((*b_type[player]) + b_now[player] + 4), 0);
    draw_block(x + 2, y + 17, *((*b_type[player]) + b_now[player] + 5), 0);
}

void draw_map(int x, int y,int player, int dx, int dy) {
    int i, j;

    for (j = 1; j < dx - 1; j++) { //천장은 계속 새로운블럭이 지나가서 지워지면 새로 그려줌 
        if (main_org[player][3][j] == EMPTY)  main_org[player][3][j] = CEILLING;
    }
    for (i = 0; i < dy; i++) {
        for (j = 0; j < dx; j++) {
            if (main_cpy[player][i][j] != main_org[player][i][j]){ //cpy랑 비교해서 값이 달라진 부분만 새로 그려줌.
                //이게 없으면 게임판전체를 계속 그려서 느려지고 반짝거림
                gotoxy(x + j, y + i);
                switch (main_org[player][i][j]) {
                case EMPTY: //빈칸모양 
                    printf("  ");
                    break;
                case CEILLING: //천장모양 
                    printf(". ");
                    break;
                case WALL: //벽모양 
                    printf("▩");
                    break;
                case INACTIVE_BLOCK: //굳은 블럭 모양  
                    printf("□");
                    break;
                case ACTIVE_BLOCK: //움직이고있는 블럭 모양  
                    printf("■");
                    break;
                }
            }
        }
    }
    save_org_cpy(player, dx, dy);

}