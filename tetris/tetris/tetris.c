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



#define MAIN_X_1 11
#define MAIN_Y_1 23


#define SHADOW_BLOCK -3 // 하드드롭 결과 미리 보여주기 
#define ACTIVE_BLOCK -2 // 게임판배열에 저장될 블록의 상태들 
#define CEILLING -1     // 블록이 이동할 수 있는 공간은 0 또는 음의 정수료 표현 
#define EMPTY 0         // 블록이 이동할 수 없는 공간은 양수로 표현 
#define WALL 1
#define INACTIVE_BLOCK 2 // 이동이 완료된 블록값 


#define XY_MAX 50
#define COLOR_RESET "\033[0m"
#define PRINT_BLOCK(X) printf("%s▣%s",X,COLOR_RESET)
#define PRINT_SHADOW_BLOCK(X) printf("%s■%s",X,COLOR_RESET)

char COLOR[7][20] = { "\033[38;2;240;240;0m","\033[38;2;0;240;240m","\033[38;2;240;80;80m","\033[38;2;80;240;80m","\033[38;2;240;160;0m","\033[38;2;80;80;240m","\033[38;2;160;0;240m" };
char SHADOW_COLOR[7][20] = { "\033[38;2;240;240;100m","\033[38;2;100;240;240m","\033[38;2;240;180;180m","\033[38;2;180;240;180m","\033[38;2;240;160;100m","\033[38;2;180;180;240m","\033[38;2;160;100;240m" };

typedef enum { GAME_START = 0, KEY_SETTING, EXIT }TITLE_MENU;
typedef enum { LEFT_KEY = 0, RIGHT_KEY, DOWN_KEY, HARD_DROP_KEY, ROTATE_KEY, ROTATE_COUNTER_KEY, HOLD_KEY, BACK_KEY } KEY_TYPE;

typedef void* GAME_FUNC(int x, int y, struct Player_info* player, int dx, int dy);
typedef void* MODE_FUNC(GAME_FUNC);


/*
int STATUS_Y_GOAL; //GOAL 정보표시위치Y 좌표 저장
int STATUS_Y_LEVEL; //LEVEL 정보표시위치Y 좌표 저장
int STATUS_Y_SCORE; //SCORE 정보표시위치Y 좌표 저장
*/

struct block_info{
    int b_status;
    char* b_color;
};

struct Player_info {
    int left_key;
    int right_key;
    int down_key;
    int hard_drop_key;
    int rotate_key;
    int rotate_counter_key;
    int hold_key;
    int pause_key;
    int esc_key;
    struct block_info main_org[XY_MAX][XY_MAX];
    struct block_info main_cpy[XY_MAX][XY_MAX];
    int b_type[2][7]; // 테트리스 가방 정렬
    int b_rotation; // 블록 회전값 저장 
    int b_now; // 현재 인덱스 저장 
    int bx, by; // 이동중인 블록의 게임판상의 x,y좌표를 저장 
    int shadow_bx, shadow_by; //이동중인 블록의 쉐도우의 게임판상의 x, y좌표를 저장
    int x, y;
    int new_block_on; // 새로운 블록이 필요함을 알려주는 flag
    int crush_on;//현재 이동중인 블록이 충돌상태인지 알려주는 flag 
    int space_key_on;//hard drop상태임을 알려주는 flag 


    int combo; //이어진 콤보 개수
    int sent_garbage; // 보내는 방해블럭 개수 - line clear에 대한 점수만 등록
    //총공격 블록 = sent_garbage의 개수 + combo 개수
    int attack_on; // 공격을 보내는 시점 flag


}Player_info[2];

struct Player_info *P1 = &Player_info[0];
struct Player_info *P2 = &Player_info[1];

int has_change(struct block_info* org, struct block_info* cpy);

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
{{0,1,1,0,
  0,1,1,0,
  0,0,0,0,
  0,0,0,0},{
      
  0,1,1,0,
  0,1,1,0,
  0,0,0,0,
  0,0,0,0}, {
      
  0,1,1,0,
  0,1,1,0,
  0,0,0,0,
  0,0,0,0},{
      
  0,1,1,0,
  0,1,1,0,
  0,0,0,0,
  0,0,0,0}}, //O

{{0,0,0,0,
  1,1,1,1,
  0,0,0,0,
  0,0,0,0},{
      
  0,0,1,0,
  0,0,1,0,
  0,0,1,0,
  0,0,1,0},{
      
  0,0,0,0,
  0,0,0,0,
  1,1,1,1,
  0,0,0,0},{
      
  0,1,0,0,
  0,1,0,0,
  0,1,0,0,
  0,1,0,0}}, // I

{{1,1,0,0,
  0,1,1,0,
  0,0,0,0,
  0,0,0,0},{
      
  0,0,1,0,
  0,1,1,0,
  0,1,0,0,
  0,0,0,0},{
         
  0,0,0,0,
  1,1,0,0,
  0,1,1,0,
  0,0,0,0},{
      
  0,0,0,0,
  0,0,1,0,
  0,1,1,0,
  0,1,0,0}}, // Z

{{0,1,1,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0},{
      
  0,1,0,0,
  0,1,1,0,
  0,0,1,0,
  0,0,0,0}, {
      
  0,0,0,0,
  0,1,1,0,
  1,1,0,0,
  0,0,0,0},{
          
  1,0,0,0,
  1,1,0,0,
  0,1,0,0,
  0,0,0,0}}, // S


{{0,0,1,0,
  1,1,1,0,
  0,0,0,0,
  0,0,0,0},{
  
  0,1,0,0,
  0,1,0,0,
  0,1,1,0,
  0,0,0,0}, {
      
  0,0,0,0,
  1,1,1,0,
  1,0,0,0,
  0,0,0,0},{
      
  1,1,0,0,
  0,1,0,0,
  0,1,0,0,
  0,0,0,0}}, // L

{{1,0,0,0,
  1,1,1,0,
  0,0,0,0,
  0,0,0,0},{
      
  0,1,1,0,
  0,1,0,0,
  0,1,0,0,
  0,0,0,0}, {
  
  0,0,0,0,
  1,1,1,0,
  0,0,1,0,
  0,0,0,0},{
      
  0,1,0,0,
  0,1,0,0,
  1,1,0,0,
  0,0,0,0}}, // J


{{0,1,0,0,
  1,1,1,0,
  0,0,0,0,
  0,0,0,0},{
      
  0,1,0,0,
  0,1,1,0,
  0,1,0,0,
  0,0,0,0}, {
      
  0,0,0,0,
  1,1,1,0,
  0,1,0,0,
  0,0,0,0},{
      
  0,1,0,0,
  1,1,0,0,
  0,1,0,0,
  0,0,0,0}} // T
}; //블록모양 저장 4*4공간에 블록을 표현 blcoks[b_type][b_rotation][i][j]로 사용 // 숫자가 커지면 시계방향 작아지면 반시계 방향

struct color {
    int r, g, b;
}b_color[7] = { {240,240,0},{0,240,240},{240,0,0},{0,240,0},{240,160,0},{0,0,240},{160,0,240} };





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


void init_data(void);

void title_scene(void);
void draw_title_scene(int x, int y, TITLE_MENU menu);


void mode_select_scene(void);

void drop_block(struct Player_info* player, int dx, int dy);

//콘솔창 draw/erase 함수

void draw_game_scene(void);
void draw_interface(int x, int y); // (x,y)를 기준으로 기본인터페이스 그리기
void erase_scene(int x, int y, int dx, int dy);
void draw_block(int x, int y, int type, int rotation);

char* key_string_set(int key);
char* key_set(int key);
// 스트링 타입으로 변환하는 함수

void setting_scene(void);
void draw_setting_scene_player(int x, int y, struct Player_info* player);
void setting_scene_set(int x, int y, KEY_TYPE type, struct Player_info* player); // 옵션 받아오기
void draw_setting_scene(int x, int y, KEY_TYPE type, struct Player_info* player);
void setting_new_key(int x, int y, int* key_set, KEY_TYPE type, struct Player_info* player);

void reset_org_cpy(struct Player_info* player, int dx, int dy); //main_org[][]를 초기화, dx, dy만큼
void save_org_cpy(struct Player_info* player, int dx, int dy); // 게임판(main_cpy[][]를 초기화)
void shuffle_block(struct Player_info* player);
/*셔플 루틴
* b_now++ 하고
* b_now = 7일 경우 
*배열1 다 사용하면
*배열 2 -> 배열1로 복사
*배열2 재셔플
*초기 셔플 -> 두 번 수행해야함
* b_now =0으로 바꾸기
*/



void game_scene(MODE_FUNC f);


void execute_2p_battle_game_func(GAME_FUNC f);

void init_game(int x, int y, struct Player_info* player, int dx, int dy);
void check_input(int x, int y, struct Player_info* player, int dx, int dy);
void update_game(int x, int y, struct Player_info* player, int dx, int dy);



void init_game_2p_battle_scene(void);
void game_2p_battle_scene(void);
void draw_game_2p_battle_scene(void);




void draw_next(int x, int y, struct Player_info* player);
void draw_map(int x, int y, struct Player_info* player, int dx, int dy);


void draw_next_block(int x, int y, struct Player_info* player);
void new_block(struct Player_info* player);// 새로운 블록의 key 가져오기
void set_new_block(struct Player_info* player, int dx, int dy);// x,y 다 결정됨
void set_shadow_block(struct Player_info* player);
void check_key(int dx, int dt); // 키보드로 키 받아오기
int check_is_upper(int k); // 키보드로  받아온 키 대문자 검사
int check_is_rotatable(int* x, int* y); // 회전 가능 하다면 변경된 x,y 값을 기준으로 변경된 좌표를 기준으로 테트리스 배치
int check_crush(struct Player_info* player, int type, int rotation, int bx, int by);
void move_block(struct Player_info* player, int dx, int dy,int bx, int by);
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


void init_data(void) {
    Player_info[0].down_key = 128 + DOWN;
    Player_info[0].esc_key = ESC;
    Player_info[0].hard_drop_key = SPACE;
    Player_info[0].hold_key = 'C';
    Player_info[0].left_key = 128 + LEFT;
    Player_info[0].pause_key = 'P';
    Player_info[0].right_key = 128 + RIGHT;
    Player_info[0].rotate_counter_key = 'Z';
    Player_info[0].rotate_key = 'X';
    Player_info[0].b_now = 0;
    Player_info[0].b_rotation = 0;
    Player_info[0].bx = 0;
    Player_info[0].by = 0;
    Player_info[0].shadow_bx = 0;
    Player_info[0].shadow_by = 0;



    Player_info[1].down_key = '5';
    Player_info[1].esc_key = ESC;
    Player_info[1].hard_drop_key = '0';
    Player_info[1].hold_key = ENTER;
    Player_info[1].left_key = '4';
    Player_info[1].pause_key = 'P';
    Player_info[1].right_key = '6';
    Player_info[1].rotate_counter_key = '7';
    Player_info[1].rotate_key = '9';
    Player_info[1].b_now = 0;
    Player_info[1].b_rotation = 0;
    Player_info[1].bx = 0;
    Player_info[1].by = 0;
    Player_info[1].shadow_bx = 0;
    Player_info[1].shadow_by = 0;


    for (int i = 0; i < 7; ++i) {
        Player_info[0].b_type[0][i] = i;
        Player_info[0].b_type[1][i] = i;
        Player_info[1].b_type[0][i] = i;
        Player_info[1].b_type[1][i] = i;
    }

}

int main() {

    HANDLE hThrd;
    //main_theme(0);
    _beginthreadex(NULL, 0, main_theme, 0,0,NULL);
    init_data();

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
        game_scene(execute_2p_battle_game_func);
        //game_2p_battle_scene(); // 나중에 모드로 수정예정
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
    struct Player_info* player = P1;
    draw_setting_scene_player(x, y, player);
    setting_scene_set(x, y, key,player);
}

void setting_scene_set(int x, int y, KEY_TYPE type, struct Player_info* player) {
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
                    if (player == P1) {
                        player ++;
                    }
                    else {
                        player--;
                    }

                    
                    draw_setting_scene_player(x, y, player);
                    draw_setting_scene(x, y, type,player);
                    break;

                }
            }

        }
    }
    switch (type) {
    case LEFT_KEY:
        gotoxy(x, y + 6); printf("▤  %s    LEFT     :     %6s  %s   ▤", " ->", key_set(player->left_key), " <-");
        //setting_new_key(x, y, &left_key[player], type, player);
        break;
    case RIGHT_KEY:
        gotoxy(x, y + 7); printf("▤  %s   RIGHT     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(player->right_key), CURSOR_ON_RIGHT);
        break;
    case DOWN_KEY:
        gotoxy(x, y + 8); printf("▤  %s    DOWN     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(player->down_key), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_KEY:
        gotoxy(x, y + 9); printf("▤  %s HARD DROP   :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(player->hard_drop_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_KEY:
        gotoxy(x, y + 10); printf("▤  %s    회전     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(player->rotate_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_KEY:
        gotoxy(x, y + 11); printf("▤  %s반시계 회전  :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(player->rotate_counter_key), CURSOR_ON_RIGHT);
        break;
    case HOLD_KEY:
        gotoxy(x, y + 12); printf("▤  %s    HOLD     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(player->hold_key), CURSOR_ON_RIGHT);
        break;
    }
}

void draw_setting_scene_player(int x, int y, struct Player_info* player) {
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

void setting_new_key(int x, int y, int* key_set, KEY_TYPE type, struct Player_info* player) {
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

void draw_setting_scene(int x, int y, KEY_TYPE type,struct Player_info* player) {
    gotoxy(x, y + 6); printf("▤  %s    LEFT     :     %6s  %s   ▤", CURSOR_OFF, key_set(player->left_key), CURSOR_OFF);
    gotoxy(x, y + 7); printf("▤  %s   RIGHT     :     %6s  %s   ▤", CURSOR_OFF, key_set(player->right_key), CURSOR_OFF);
    gotoxy(x, y + 8); printf("▤  %s    DOWN     :     %6s  %s   ▤", CURSOR_OFF, key_set(player->down_key), CURSOR_OFF);
    gotoxy(x, y + 9); printf("▤  %s HARD DROP   :     %6s  %s   ▤", CURSOR_OFF, key_set(player->hard_drop_key), CURSOR_OFF);
    gotoxy(x, y + 10); printf("▤  %s    회전     :     %6s  %s   ▤", CURSOR_OFF, key_set(player->rotate_key), CURSOR_OFF);
    gotoxy(x, y + 11); printf("▤  %s반시계 회전  :     %6s  %s   ▤", CURSOR_OFF, key_set(player->rotate_counter_key), CURSOR_OFF);
    gotoxy(x, y + 12); printf("▤  %s    HOLD     :     %6s  %s   ▤", CURSOR_OFF, key_set(player->hold_key), CURSOR_OFF);
    gotoxy(x, y + 13); printf("▤                                      ▤");
    gotoxy(x, y + 14); printf("▤      PRESS ESC TO BACK TO MAIN       ▤");


    switch (type) {
    case LEFT_KEY:
        gotoxy(x, y + 6); printf("▤  %s    LEFT     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(player->left_key), CURSOR_ON_RIGHT);
        break;
    case RIGHT_KEY:
        gotoxy(x, y + 7); printf("▤  %s   RIGHT     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(player->right_key), CURSOR_ON_RIGHT);
        break;
    case DOWN_KEY:
        gotoxy(x, y + 8); printf("▤  %s    DOWN     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(player->down_key), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_KEY:
        gotoxy(x, y + 9); printf("▤  %s HARD DROP   :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(player->hard_drop_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_KEY:
        gotoxy(x, y + 10); printf("▤  %s    회전     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(player->rotate_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_KEY:
        gotoxy(x, y + 11); printf("▤  %s반시계 회전  :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(player->rotate_counter_key), CURSOR_ON_RIGHT);
        break;
    case HOLD_KEY:
        gotoxy(x, y + 12); printf("▤  %s    HOLD     :     %6s  %s   ▤", CURSOR_ON_LEFT, key_set(player->hold_key), CURSOR_ON_RIGHT);
        break;

    }
}

//수정예정 - 세팅 키 입력 덜 작성함


void reset_org_cpy(struct Player_info* player, int dx, int dy) {
    int i, j;
    for (i = 0; i < dy; ++i) {
        for (j = 0; j < dx; ++j) {
            player->main_org[i][j].b_status = EMPTY;
            player->main_org[i][j].b_color = COLOR_RESET;
            player->main_cpy[i][j].b_status = 100;
            player->main_cpy[i][j].b_color = COLOR_RESET;

        }
    }
    for (j = 0; j < dx; ++j) {
        player->main_org[3][j].b_status = CEILLING;

    }
    for (i = 1; i < dy - 1; i++) { //좌우 벽을 만듦  
        player->main_org[i][0].b_status = WALL;
        player->main_org[i][dx - 1].b_status = WALL;

        
    }
    for (j = 0; j < dx; ++j) { //바닥벽을 만듦 
        player->main_org[dy - 1][j].b_status = WALL;

    }
}



void save_org_cpy(struct Player_info* player , int dx, int dy) {
    for (int i = 0; i < dx; i++) { //게임판을 그린 후 main_cpy에 복사  
        for (int j = 0; j < dy; j++) {
            player->main_cpy[i][j].b_color = player->main_org[i][j].b_color;
            player->main_cpy[i][j].b_status = player->main_org[i][j].b_status;

        }
    }
}

void shuffle_block(struct Player_info* player) // 디버깅 완료
{

    for (int i = 0; i < 7; ++i) {
        player->b_type[0][i] = player->b_type[1][i];
    }
    int n, temp;
    for(int i = 0; i < 6; ++i) {
        n = rand() % (7 - i) + i;    // i 부터 num-1 사이에 임의의 정수 생성

        temp = player->b_type[1][i];
        player->b_type[1][i] = player->b_type[1][n];
        player->b_type[1][n] = temp;


        
    }
    
}

void new_block(struct Player_info* player) { // 새로운 블록 인덱스 설정하기 //디버깅 완료
    player->b_now++;

    
    if (player->b_now >= 7) {
        shuffle_block(player);
        player->b_now = 0;
    }
    
}

void set_new_block(struct Player_info* player, int dx, int dy) { //새로운 블록 그리기
    int i, j;
    player->bx = (dx / 2) - 1;
    player->by = 0;
    player->b_rotation = 0;


    new_block_on = 0;
    for (i = 0; i < 4; i++) { //게임판 bx, by위치에 블럭생성  
        for (j = 0; j < 4; j++) {
            if (blocks[player->b_type[0][player->b_now]][player->b_rotation][i][j] == 1) {
                player->main_org[player->by + i][player->bx + j].b_color = COLOR[player->b_type[0][player->b_now]];
                player->main_org[player->by + i][player->bx + j].b_status = ACTIVE_BLOCK;
            }
        }
    }


}

void set_shadow_block(struct Player_info* player) {
    int type = player->b_type[0][player->b_now];
    int rotation = player->b_rotation;
    player->shadow_bx = player->bx;
    player->shadow_by = player->by;
    while (1) {
        if (!check_crush(player,type, rotation, player->shadow_bx, player->shadow_by+1)) break;
        player->shadow_by++;
    }
    for (int i = 0; i < 4; i++) { //게임판 bx, by위치에 셰도우 블럭생성  
        for (int j = 0; j < 4; j++) {
            if (blocks[type][rotation][i][j] == 1&& player->main_org[player->shadow_by + i][player->shadow_bx + j].b_status != ACTIVE_BLOCK) {
                player->main_org[player->shadow_by + i][player->shadow_bx + j].b_color = SHADOW_COLOR[type];
                player->main_org[player->shadow_by + i][player->shadow_bx + j].b_status = SHADOW_BLOCK;
            }
        }
    }

    
}

void debug() {
    for (int i = 0; i < 7; ++i) {
        printf("%d,", Player_info[0].b_type[0][i]);
    }
    /*
    for (int i = 0; i < 14; ++i) {
        printf("%d,", *((*b_type[1]) + i));
    }
    printf("...");
    printf("%d, %d", b_now[0], b_now[1]);
    printf("\n");
    */
}

void init_game_2p_battle_scene(void) {

}

void game_scene(MODE_FUNC f) {

}

void game_2p_battle_scene(void) {
    //리셋 시작
    new_block_on = 1;
    int x[2], y[2];
    x[0] = 3;
    y[0] = 1;
    x[1] = MAIN_X_1 + 16;
    y[1] = 1;
    crush_on = 0;
    reset_org_cpy(P1,MAIN_X_1,MAIN_Y_1);
    reset_org_cpy(P2,MAIN_X_1,MAIN_Y_1);
    shuffle_block(P1);
    shuffle_block(P1);
    shuffle_block(P2);
    shuffle_block(P2);
    //debug();
    set_new_block(P1, MAIN_X_1, MAIN_Y_1);
    set_new_block(P2, MAIN_X_1, MAIN_Y_1);
    set_shadow_block(P1);
    set_shadow_block(P2);
    Player_info[0].x = 3;
    Player_info[0].y = 1;
    Player_info[1].x = 2 * MAIN_X_1 + 17;
    Player_info[1].y = 1;
    draw_game_2p_battle_scene();
    //리셋 끝



    while (1) {
        for (int i = 0; i < 5; ++i) {
            check_key(MAIN_X_1, MAIN_Y_1);
        }
        drop_block(P1,MAIN_X_1,MAIN_Y_1);
        //drop_block(P2,MAIN_X_1,MAIN_Y_1);
        draw_map(x[0], y[0], P1, MAIN_X_1, MAIN_Y_1);
        draw_map(x[1], y[1], P2, MAIN_X_1, MAIN_Y_1);
        //execute_2p_battle_game_func(draw_map);
        if (new_block_on) {
            new_block(P1);
            set_new_block(P1, MAIN_X_1, MAIN_Y_1);
            set_shadow_block(P1);
            new_block_on = 0;
        }
        Sleep(200);
    }
}

void draw_game_2p_battle_scene(void) { // 기본적인 틀 그리기, 
    int x = 3, y = 1;
    draw_map(x,y,P1, MAIN_X_1, MAIN_Y_1);
    draw_next(x+MAIN_X_1+1,y+2,P1, MAIN_X_1, MAIN_Y_1);
    draw_map(x+MAIN_X_1+10+x , y, P2, MAIN_X_1, MAIN_Y_1);
    draw_next(2*x + 2*MAIN_X_1 + 11, y + 2, P2, MAIN_X_1, MAIN_Y_1);
}

void draw_block(int x, int y, int type, int rotation) { //블록 그리기

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (blocks[type][rotation][i][j] == 1) {
                //SET_FG_COLOR(b_color[type].r, b_color[type].g, b_color[type].b);
                gotoxy(x + j, y + i); PRINT_BLOCK(COLOR[type]);
            }
        }
    }
}

void draw_next(int x, int y, struct Player_info* player) { //draw_next는 초기화에서만 그리기 (깜빡이는 것 방지)
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
    draw_next_block(x,y,player);
}

void draw_next_block(int x, int y, struct Player_info* player) {
    draw_block(x + 2, y + 1, *((*(player->b_type)) + player->b_now + 1), 0);
    draw_block(x + 2, y + 5, *((*(player->b_type)) + player->b_now + 2), 0);
    draw_block(x + 2, y + 9, *((*(player->b_type)) + player->b_now + 3), 0);
    draw_block(x + 2, y + 13, *((*(player->b_type)) + player->b_now + 4), 0);
    draw_block(x + 2, y + 17, *((*(player->b_type)) + player->b_now + 5), 0);
}

void draw_map(int x, int y,struct Player_info* player, int dx, int dy) {
    int i, j;

    for (j = 1; j < dx - 1; j++) { //천장은 계속 새로운블럭이 지나가서 지워지면 새로 그려줌 
        if (player->main_org[3][j].b_status == EMPTY) {
            player->main_org[3][j].b_status = CEILLING;
            player->main_org[3][j].b_color = COLOR_RESET;
        }  
    }
    for (i = 0; i < dy; i++) {
        for (j = 0; j < dx; j++) {
            if (has_change(&(player->main_cpy[i][j]), &(player->main_org[i][j]))){ //cpy랑 비교해서 값이 달라진 부분만 새로 그려줌.
                //이게 없으면 게임판전체를 계속 그려서 느려지고 반짝거림
                gotoxy(x + j, y + i);
                switch (player->main_org[i][j].b_status) {
                case EMPTY: //빈칸모양 
                    printf("  ");
                    break;
                case CEILLING: //천장모양 
                    printf(". ");
                    break;
                case WALL: //벽모양 
                    printf("□");
                    break;
                case INACTIVE_BLOCK: //굳은 블럭 모양  
                    //printf("□");
                    PRINT_BLOCK(player->main_org[i][j].b_color);
                    break;
                case ACTIVE_BLOCK: //움직이고있는 블럭 모양  
                    PRINT_BLOCK(player->main_org[i][j].b_color);
                    break;
                case SHADOW_BLOCK: //움직이고있는 블럭 모양  
                    PRINT_SHADOW_BLOCK(player->main_org[i][j].b_color);
                    break;
                }
            }
        }
    }
    save_org_cpy(player, dx, dy);

}

int has_change(struct block_info* org, struct block_info* cpy) {
    if (org->b_color != cpy->b_color || org->b_status != cpy->b_status) return 1;
    return 0;
}

int check_crush(struct Player_info* player,int type, int rotation, int bx, int by) { //지정된 좌표와 회전값으로 충돌이 있는지 검사 
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) { //지정된 위치의 게임판과 블럭모양을 비교해서 겹치면 false를 리턴 
            if (blocks[type][rotation][i][j] == 1 && player->main_org[by + i][bx + j].b_status > 0) return false;
        }
    }
    return true; //하나도 안겹치면 true리턴 
}

int check_is_upper(int k) {// 키보드로  받아온 키 대문자 검사
    if (k >= 'a' && k <= 'z') {
        k = k - 'a' + 'A';
    }
    return k;
}

void check_key(int dx, int dy) {
    key = 0; //키값 초기화  

    if (_kbhit()) { //키입력이 있는 경우  
        key = check_is_upper(_getch());
        if (key == 224) { //방향키인경우 
            do { key = _getch(); } while (key == 224);//방향키지시값을 버림 
            switch (key + 128) {
            }                   //바닥에 닿은 경우 위쪽으로 한칸띄워서 회전이 가능하면 그렇게 함(특수동작)
        }
        else { //방향키가 아닌경우 
            switch (key) {
            case SPACE: //스페이스키 눌렀을때
                //space_key_on = 1; //스페이스키 flag를 띄움 
                while (P1->crush_on == 0) { //바닥에 닿을때까지 이동시킴 
                    drop_block(P1, dx, dy);
                }
            }
        }
    }
    while (_kbhit()) _getch(); //키버퍼를 비움 
}



void execute_key(struct Player_info* player,KEY_TYPE type) {
    if (type == BACK_KEY) {
        // 게임 일시 정지, 나가거나 이어서 하기 가능
    }
}



void drop_block(struct Player_info* player, int dx, int dy) {
    int i, j;

    if (player->crush_on && check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == true) crush_on = 0; //밑이 비어있으면 crush flag 끔 
    if (player->crush_on && check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == false) { //밑이 비어있지않고 crush flag가 켜저있으면 
        for (i = 0; i < dy; i++) { //현재 조작중인 블럭을 굳힘 
            for (j = 0; j < dx; j++) {
                if (player->main_org[i][j].b_status == ACTIVE_BLOCK) player->main_org[i][j].b_status = INACTIVE_BLOCK;
            }
        }
        player->crush_on = 0; //flag를 끔 
        //check_line(); //라인체크를 함 
        player->new_block_on = 1; //새로운 블럭생성 flag를 켬    
        return; //함수 종료 
    }
    if (check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == true) move_block(player,dx,dy, player->bx, player->by + 1); //밑이 비어있으면 밑으로 한칸 이동 
    if (check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == false) player->crush_on=1; //밑으로 이동이 안되면  crush flag를 켬
}

void move_block(struct Player_info* player,int dx, int dy, int bx, int by) {
    for (int i = 0; i < 4; i++) { //현재좌표의 블럭을 지움 
        for (int j = 0; j < 4; j++) {
            if (blocks[player->b_type[0][player->b_now]][player->b_rotation][i][j] == 1) {
                player->main_org[player->by + i][player->bx + j].b_status = EMPTY;
                player->main_org[player->by + i][player->bx + j].b_color = COLOR_RESET;
            } 
        }
    }
    for (int i = 0; i < 4; i++) { //현재좌표의 블럭을 지움 
        for (int j = 0; j < 4; j++) {
            if (blocks[player->b_type[0][player->b_now]][player->b_rotation][i][j] == 1) {
                player->main_org[by + i][bx + j].b_status = ACTIVE_BLOCK;
                player->main_org[by + i][bx + j].b_color = COLOR[player->b_type[0][player->b_now]];
            }
        }
    }
    player->bx = bx;
    player->by = by;

    
    //draw_map(3,1, P1, MAIN_X_1, MAIN_Y_1);
}


void execute_2p_battle_game_func(GAME_FUNC f) {
    f(3,1,P1,MAIN_X_1,MAIN_Y_1);
    f(16+MAIN_X_1,1,P2,MAIN_X_1,MAIN_Y_1);
}

void init_game(int x, int y, struct Player_info* player, int dx, int dy) {
    reset_org_cpy(player,dx, dy);
    shuffle_block(player);
    shuffle_block(player);
    //debug();
    set_new_block(player, dx, dy);
    set_shadow_block(player);
    draw_map(x, y, player, dx, dy);
    draw_next(x + dx + 1, y + 2, player, dx, dy);
}

void check_input(int x, int y, struct Player_info* player, int dx, int dy) {
    check_key(); //키입력확인 
    draw_main(); //화면을 그림 
    Sleep(speed); //게임속도조절 
    if (crush_on && check_crush(bx, by + 1, b_rotation) == false) Sleep(100);
    //블록이 충돌중인경우 추가로 이동및 회전할 시간을 갖음 
    if (space_key_on == 1) { //스페이스바를 누른경우(hard drop) 추가로 이동및 회전할수 없음 break; 
        space_key_on = 0;
    }
}

void update_game(int x, int y, struct Player_info* player, int dx, int dy) {
    drop_block(player,dx, dy);
    if (player->new_block_on) {
        new_block(P1);
        set_new_block(P1, dx, dy);
        set_shadow_block(P1);
        player->new_block_on = 0;
    }
    draw_map(x,y, player, dx, dy);
    while (1) {
        for (int i = 0; i < 5; ++i) {
            check_key(MAIN_X_1, MAIN_Y_1);
        }
        drop_block(P1, MAIN_X_1, MAIN_Y_1);
        //drop_block(P2,MAIN_X_1,MAIN_Y_1);
       
        draw_map(x[1], y[1], P2, MAIN_X_1, MAIN_Y_1);
        //execute_2p_battle_game_func(draw_map);
        
        Sleep(200);
    }
}

void game_scene(MODE_FUNC f) {
    f(init_game);

    while (1) {
        for (int i = 0; i < 5; ++i) {
            f(check_input);
        }
        f(update_game);
    }
}