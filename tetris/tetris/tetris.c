#define _CRT_SECURE_NO_WARNINGS //scanf, strcpy 오류 무시

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include <string.h>
#include <process.h>
#include <handleapi.h>



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

#define CLOCKWISE 0
#define COUNTER_CLOCKWISE 1



#define MAIN_X_1 12
#define MAIN_Y_1 23


#define SHADOW_BLOCK -3 // 하드드롭 결과 미리 보여주기 
#define ACTIVE_BLOCK -2 // 게임판배열에 저장될 블록의 상태들 
#define CEILLING -1     // 블록이 이동할 수 있는 공간은 0 또는 음의 정수료 표현 
#define EMPTY 0         // 블록이 이동할 수 없는 공간은 양수로 표현 
#define WALL 1
#define INACTIVE_BLOCK 2 // 이동이 완료된 블록값 

#define MAX_INPUT 1000
#define XY_MAX 50
#define COLOR_RESET "\033[0m"
#define PRINT_BLOCK(X) printf("%s▣%s",X,COLOR_RESET)
#define PRINT_SHADOW_BLOCK(X) printf("%s■%s",X,COLOR_RESET)

char COLOR[7][20] = { "\033[38;2;240;240;0m","\033[38;2;0;240;240m","\033[38;2;240;80;80m","\033[38;2;80;240;80m","\033[38;2;240;160;0m","\033[38;2;80;80;240m","\033[38;2;160;0;240m" };
char SHADOW_COLOR[7][20] = { "\033[38;2;240;240;100m","\033[38;2;100;240;240m","\033[38;2;240;180;180m","\033[38;2;180;240;180m","\033[38;2;240;160;100m","\033[38;2;180;180;240m","\033[38;2;160;100;240m" };

typedef enum { GAME_START = 0, KEY_SETTING, EXIT }TITLE_MENU;
typedef enum { NO_KEY = 0,LEFT_KEY , RIGHT_KEY, DOWN_KEY, HARD_DROP_KEY, ROTATE_KEY, ROTATE_COUNTER_KEY, HOLD_KEY, BACK_KEY } KEY_TYPE;

typedef void* GAME_FUNC(int x, int y, struct Player_info* player, int dx, int dy);
typedef void* MODE_FUNC(GAME_FUNC);
typedef void* OPTION_FUNC(void);
typedef void* GAME_OVER_FUNC(struct Player_info* player);


struct Key_queue {
    KEY_TYPE key[MAX_INPUT];
    int front;
    int rear;
};



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
    int hard_drop_key_on;//hard drop상태임을 알려주는 flag 
    int hold_block_type; // hold 중인 블록 종류
    int new_hold_on;
    int hold_on;
    int t_spin_available;
    int is_t_spin; // 주변 블록 3개 막힘

    int combo; //이어진 콤보 개수
    int sent_garbage; // 보내는 방해블럭 개수 - line clear에 대한 점수만 등록
    //총공격 블록 = sent_garbage의 개수 + combo 개수
    int attack_on; // 공격을 보내는 시점 flag

    struct Key_queue key_queue;

    int is_attack_mode; // 공격을 하는 모드인지
    struct Player_info* target; // 공격을 하는 타겟
    int attacked; // 공격을 받은 양 기억해두는 곳

    int damage;
    


}Player_info[2];

struct Player_info *P1 = &Player_info[0];
struct Player_info *P2 = &Player_info[1];

int has_change(struct block_info* org, struct block_info* cpy);

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
      
  0,1,0,0,
  1,1,0,0,
  1,0,0,0,
  0,0,0,0}}, // Z

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

int t_spin_damage[4] = { 0,2,4,6 };
int combo_damage[21] = { 0,0,1,1,2,2,3,3,4,4,4,5,5,5,5,5,5,5,5,5,5};
int line_damage[5] = { 0,0,1,2,4 };



int key; //키보드로 입력받은 키값을 저장 

int frame_time = 5; // 키 입력 단위 시간 // 블럭이 떨어지는데 걸리는 시간 50 틱, 추가 회전 시간 100틱

int is_game_over;

GAME_OVER_FUNC* game_over_func;
HANDLE hThrd_bgm;
HANDLE hTHrd_input;


void init_data(void);

void title_scene(void);
void draw_title_scene(int x, int y, TITLE_MENU menu);


void mode_select_scene(void);



//콘솔창 draw/erase 함수
void init_player(struct Player_info* player);

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



void game_scene(MODE_FUNC f, OPTION_FUNC o);


void execute_2p_battle_game_func(GAME_FUNC f);

void init_game(int x, int y, struct Player_info* player, int dx, int dy);
void check_input(int x, int y, struct Player_info* player, int dx, int dy);
void update_game(int x, int y, struct Player_info* player, int dx, int dy);

void drop_block(struct Player_info* player, int dx, int dy);




void get_game_input(int p);

void draw_next(int x, int y, struct Player_info* player);
void draw_map(int x, int y, struct Player_info* player, int dx, int dy);

void draw_hold(struct Player_info* player, int dx, int dy);
void draw_next_block(int x, int y, struct Player_info* player);
void new_block(struct Player_info* player);// 새로운 블록의 key 가져오기
void set_new_block(struct Player_info* player, int dx, int dy);// x,y 다 결정됨
void erase_active_block(struct Player_info* player, int dx, int dy);
void set_shadow_block(struct Player_info* player);
void erase_shadow_block(struct Player_info* player, int dx, int dy);
void check_key(struct Player_info* player,int dx, int dt); // 키보드로 키 받아오기
int check_is_upper(int k); // 키보드로  받아온 키 대문자 검사
int check_is_rotatable(struct Player_info* player, int type,int rotation, int wise, int bx, int by,int dx, int dy); // 회전 가능 하다면 변경된 x,y 값을 기준으로 변경된 좌표를 기준으로 테트리스 배치
int check_crush(struct Player_info* player, int type, int rotation, int bx, int by);
void move_block(struct Player_info* player, int dx, int dy,int bx, int by);
void check_line(int x, int y, struct Player_info* player, int dx, int dy);
int check_game_over(struct Player_info* player, int dx, int dy);

void check_get_attacked(int x, int y, struct Player_info* player, int dx, int dy);
void send_attack(struct Player_info* player);

void left(struct Player_info* player, int dx, int dy);
void right(struct Player_info* player, int dx, int dy);
void down(struct Player_info* player, int dx, int dy);
void hold(int x, int y, struct Player_info* player, int dx, int dy);
void hard_drop(struct Player_info* player, int dx, int dy);
void rotate(struct Player_info* player, int dx, int dy);
void rotate_counter(struct Player_info* player, int dx, int dy);

void option_2p_battle_game(void);


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
    Player_info[0].rotate_key = UP+128;
    Player_info[0].b_now = 0;
    Player_info[0].b_rotation = 0;
    Player_info[0].bx = 0;
    Player_info[0].by = 0;
    Player_info[0].shadow_bx = 0;
    Player_info[0].shadow_by = 0;



    Player_info[1].down_key = '5';
    Player_info[1].esc_key = -1;
    Player_info[1].hard_drop_key = '0';
    Player_info[1].hold_key = ENTER;
    Player_info[1].left_key = '4';
    Player_info[1].pause_key = 'P';
    Player_info[1].right_key = '6';
    Player_info[1].rotate_counter_key = '7';
    Player_info[1].rotate_key = '8';
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

void enqueue(struct Key_queue* q, KEY_TYPE type) {
    if ((q->rear - q->front + MAX_INPUT) % MAX_INPUT == MAX_INPUT - 1) return;
    q->rear++;
    q->key[q->rear] = type;
    
    return;
}

KEY_TYPE dequeue(struct Key_queue* q) {
    if (q->front == q->rear) return NO_KEY;
    else {
        q->front++;
        return q->key[q->front];
    }
}

void init_queue(struct Key_queue* q) {
    q->rear = 0;
    q->front = 0;
}

int main() {
    hThrd_bgm = (HANDLE)_beginthreadex(NULL, 0, main_theme, 0,0,NULL);
    init_data();
    system("mode con: cols=150 lines=40");
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
        game_scene(execute_2p_battle_game_func,option_2p_battle_game);
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
                    type = type + 1;
                    if (type == 8) type = 1;
                    while (_kbhit()) _getch();
                    draw_setting_scene(x, y, type,player);
                    break;
                case UP: //위쪽 방향키 눌렀을때
                    type -= 1;
                    if (type == 0) type = 7;
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
    for (i = 0; i < dy - 1; i++) { //좌우 벽을 만듦  
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
        player->b_type[0][i] = player->b_type[1][i]; // 홀드는 b_type[0]에만 접근하기 때문에 b-type[1]의 가방은 훼손되지 않는다.
    }
    for (int i = 0; i < 7; ++i) {
        player->b_type[1][i] = i; // 홀드로 인해 가방 꼬이는 것 방지
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

void erase_active_block(struct Player_info* player, int dx, int dy) {
    for (int i = 0; i < dy; i++) { //현재 조작중인 블럭을 굳힘 
        for (int j = 0; j < dx; j++) {
            if (player->main_org[i][j].b_status == ACTIVE_BLOCK) {
                player->main_org[i][j].b_status = EMPTY;
                player->main_org[i][j].b_color = COLOR_RESET;
            } 
        }
    }
}

void erase_shadow_block(struct Player_info* player, int dx, int dy) {
    
    for (int i = 0; i < dy; i++) { //현재 조작중인 블럭을 굳힘 
        for (int j = 0; j < dx; j++) {
            if (player->main_org[i][j].b_status == SHADOW_BLOCK) {
                player->main_org[i][j].b_status = EMPTY;
                player->main_org[i][j].b_color = COLOR_RESET;
            } 
        }
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

void draw_hold(int x, int y, struct Player_info* player) {
    gotoxy(x, y); printf("+-  H O L D  -+ ");
    gotoxy(x, y + 1); printf("|             | ");
    gotoxy(x, y + 2); printf("|             | ");
    gotoxy(x, y + 3); printf("+-- -  -  - --+ ");
    if (player->hold_block_type == -1) return;
    draw_block(x + 2, y + 1, player->hold_block_type, 0);
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
            if (blocks[type][rotation][i][j] == 1 && player->main_org[by + i][bx + j].b_status > 0) return FALSE;
        }
    }
    return TRUE; //하나도 안겹치면 true리턴 
}

int check_is_upper(int k) {// 키보드로  받아온 키 대문자 검사
    if (k >= 'a' && k <= 'z') {
        k = k - 'a' + 'A';
    }
    return k;
}


void drop_block(struct Player_info* player, int dx, int dy) {
    int i, j;

    if (player->crush_on && check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == true) player->crush_on = 0; //밑이 비어있으면 crush flag 끔 
    if (player->crush_on && check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == false) { //밑이 비어있지않고 crush flag가 켜저있으면 
        for (i = 0; i < dy; i++) { //현재 조작중인 블럭을 굳힘 
            for (j = 0; j < dx; j++) {
                if (player->main_org[i][j].b_status == ACTIVE_BLOCK) player->main_org[i][j].b_status = INACTIVE_BLOCK;
            }
        }
        player->crush_on = 0; //flag를 끔 
        check_line(player->x, player->y,player,dx,dy); //라인체크를 함 
        //check_attack_on
        check_get_attacked(player->x, player->y, player, dx, dy);
        player->new_block_on = 1; //새로운 블럭생성 flag를 켬    
        return; //함수 종료 
    }
    if (check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == true) move_block(player,dx,dy, player->bx, player->by + 1); //밑이 비어있으면 밑으로 한칸 이동 
    if (check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == false) player->crush_on=1; //밑으로 이동이 안되면  crush flag를 켬
}

void rotate_block(struct Player_info* player, int dx, int dy, int bx, int by, int rotate) {

    if (player->b_type[0][player->b_now]==6) {
        int cnt = 0;
        if (player->main_org[by + 0][bx + 0].b_status > 0) cnt++;
        if (player->main_org[by + 2][bx + 0].b_status > 0) cnt++;
        if (player->main_org[by + 0][bx + 2].b_status > 0) cnt++;
        if (player->main_org[by + 2][bx + 2].b_status > 0) cnt++;
        if (cnt >= 3) {
            player->is_t_spin = 1;
        }
        else {
            player->is_t_spin = 0;
        }
    }
    int rotation = player->b_rotation;
    if(rotate == CLOCKWISE){
        player->b_rotation = (rotation + 1) % 4;
    }
    else {
        player->b_rotation = (rotation + 3) % 4;
    }
    for (int i = 0; i < 4; i++) { //현재좌표의 블럭을 지움 
        for (int j = 0; j < 4; j++) {
            if (blocks[player->b_type[0][player->b_now]][rotation][i][j] == 1) {
                player->main_org[player->by + i][player->bx + j].b_status = EMPTY;
                player->main_org[player->by + i][player->bx + j].b_color = COLOR_RESET;
            }
        }
    }
    player->bx = bx;
    player->by = by;
    for (int i = 0; i < 4; i++) { //현재좌표의 블럭을 지움 
        for (int j = 0; j < 4; j++) {
            if (blocks[player->b_type[0][player->b_now]][player->b_rotation][i][j] == 1) {
                player->main_org[by + i][bx + j].b_status = ACTIVE_BLOCK;
                player->main_org[by + i][bx + j].b_color = COLOR[player->b_type[0][player->b_now]];
            }
        }
    }
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
    f(10,1,P1,MAIN_X_1,MAIN_Y_1);
    f(29+MAIN_X_1,1,P2,MAIN_X_1,MAIN_Y_1);
}

void init_game(int x, int y, struct Player_info* player, int dx, int dy) {
    player->x = x;
    player->y = y;
    init_player(player);
    reset_org_cpy(player,dx, dy);
    shuffle_block(player);
    shuffle_block(player);
    //debug();
    set_new_block(player, dx, dy);
    set_shadow_block(player);
    draw_hold(x-9, y +3 ,player);
    draw_map(x, y, player, dx, dy);
    draw_next(x + dx + 1, y + 2, player, dx, dy);
    
    
}



void init_player(struct Player_info* player) {
    player->attack_on = 0;
    player->b_now = 0;
    player->b_rotation = 0;
    player->combo = 0;
    player->crush_on = 0;
    player->new_block_on = 0;
    player->sent_garbage = 0;
    player->shadow_bx = 0;
    player->shadow_by = 0;
    player->bx = 0;
    player->by = 0;
    player->hold_block_type = -1;
    player->hold_on = 0;
    player->new_hold_on = 0;
    player->combo = 0;
    player->attack_on = 0;
    player->sent_garbage = 0;
    player->new_hold_on = 0;
    player->attacked = 0;
    init_queue(&player->key_queue);
    
}
void left(struct Player_info* player, int dx, int dy) {
    if (check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx - 1, player->by) == TRUE) {
        move_block(player, dx, dy, player->bx-1,player->by);
        erase_shadow_block(player, dx, dy);
        set_shadow_block(player);
    }
}
void right(struct Player_info* player, int dx, int dy) {
    if (check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx + 1, player->by) == TRUE) {
        move_block(player, dx, dy, player->bx + 1, player->by);
        erase_shadow_block(player, dx, dy);
        set_shadow_block(player);
    }
}

void down(struct Player_info* player, int dx, int dy) {
    if (check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by+1) == TRUE) {
        move_block(player, dx, dy, player->bx, player->by+1);
        erase_shadow_block(player, dx, dy);
        set_shadow_block(player);
    }
}
void hold(int x, int y, struct Player_info* player, int dx, int dy){
    erase_shadow_block(player, dx, dy);
    erase_active_block(player, dx, dy);
    player->hold_on = 1;
    if (player->hold_block_type == -1) {
        player->new_hold_on = 1;
        player->new_block_on = 1;
        player->hold_block_type = player->b_type[0][player->b_now];
    }
    else {
        int block = player->hold_block_type;
        player->hold_block_type = player->b_type[0][player->b_now];
        player->b_type[0][player->b_now] = block;
        player->bx = (dx / 2) - 1;
        player->by = 0;
    }
    draw_hold(x - 9, y + 3, player);
}
void hard_drop(struct Player_info* player, int dx, int dy) {
    player->hard_drop_key_on = 1; //스페이스키 flag를 띄움 
    while (player->crush_on == 0) { //바닥에 닿을때까지 이동시킴 
        drop_block(player, dx, dy);
    }
}


int check_is_rotatable(struct Player_info* player, int type, int rotation, int wise, int bx, int by, int dx, int dy) {

    int index = 1;
    if (type == 1) index = 0;
    int rot;
    if (wise == CLOCKWISE) {
        rot = (rotation + 1) % 4;
    }
    else {
        rot = (rotation + 3) % 4;
    }
    for (int i = 0; i < 5; ++i) {
        if (bx + wall_kick_data[index][rotation][wise][i][0]<0 || bx + wall_kick_data[index][rotation][wise][i][0]>dx || by - wall_kick_data[index][rotation][wise][i][1]<0 || by - wall_kick_data[index][rotation][wise][i][1]>dy) continue; //
        if (check_crush(player, type, rot, bx + wall_kick_data[index][rotation][wise][i][0], by - wall_kick_data[index][rotation][wise][i][1]) == TRUE)
            return i;
    }
    return -1;
}

void rotate(struct Player_info* player, int dx, int dy) {
    int type = player->b_type[0][player->b_now];
    if (type == 0) return;
    int rotation = player->b_rotation;
    rotation = (rotation + 1) % 4;
    int i = check_is_rotatable(player, type, player->b_rotation, CLOCKWISE, player->bx, player->by,dx, dy);
    int index = 1;
    if (type == 1) index = 0;
    if (i == -1) return;
    else {
        rotate_block(player, dx, dy, player->bx + wall_kick_data[index][player->b_rotation][CLOCKWISE][i][0], player->by - wall_kick_data[index][player->b_rotation][CLOCKWISE][i][1], CLOCKWISE);
        erase_shadow_block(player, dx, dy);
        set_shadow_block(player);
    }
    
}


void rotate_counter(struct Player_info* player, int dx, int dy) {
    int type = player->b_type[0][player->b_now];
    if (type == 0) return;
    int rotation = player->b_rotation;
    rotation = (rotation + 3) % 4;
    int i = check_is_rotatable(player, type, player->b_rotation, COUNTER_CLOCKWISE, player->bx, player->by,dx ,dy);
    int index = 1;
    if (type == 1) index = 0;
    if (i == -1) return;
    else {
        rotate_block(player, dx, dy, player->bx + wall_kick_data[index][player->b_rotation][COUNTER_CLOCKWISE][i][0], player->by - wall_kick_data[index][player->b_rotation][COUNTER_CLOCKWISE][i][1], COUNTER_CLOCKWISE);
        erase_shadow_block(player, dx, dy);
        set_shadow_block(player);
    }
}


void garbage_block(int x, int y, struct Player_info* target,int dx, int dy) {
    for (int i = 4; i < dy; ++i) {
        for (int j = 1; j < dx-1; ++j) {
            if (target->main_org[i - 1][j].b_status == ACTIVE_BLOCK || target->main_org[i][j].b_status == ACTIVE_BLOCK) continue;
            target->main_org[i - 1][j].b_color = target->main_org[i][j].b_color;
            target->main_org[i - 1][j].b_status = target->main_org[i][j].b_status;
        }
    }
    int n = (rand() % (dx - 2)) + 1;
    target->main_org[dy - 2][n].b_status = EMPTY;
    target->main_org[dy - 2][n].b_color = COLOR_RESET;
    for (int j = 1; j < dx - 1; ++j) {
        if (j == n) continue;
        target->main_org[dy - 2][j].b_status = INACTIVE_BLOCK;
        target->main_org[dy - 2][j].b_color = COLOR_RESET;
    }
    draw_map(target-> x, target->y,target, dx, dy);
}

void check_input(int x, int y, struct Player_info* player, int dx, int dy) {
    if (player->hard_drop_key_on == 1) { //스페이스바를 누른경우(hard drop) 추가로 이동및 회전할수 없음 break; 
        player->hard_drop_key_on = 1;
        return;
    }
    if (player->new_block_on == 1) { //스페이스바를 누른경우(hard drop) 추가로 이동및 회전할수 없음 break; 
        return;
    }
    //키입력확인 
    KEY_TYPE type = dequeue(&player->key_queue);
    if (type== NO_KEY) return;

    switch (type) {
    case LEFT_KEY:
        left(player, dx, dy);
        break;
    case RIGHT_KEY:
        right(player, dx, dy);
        break;
    case DOWN_KEY:
        down(player, dx, dy);
        break;
    case HARD_DROP_KEY:
        hard_drop(player, dx, dy);
        break;
    case ROTATE_KEY:
        rotate(player,dx,dy);
        break;
    case ROTATE_COUNTER_KEY:
        rotate_counter(player, dx, dy);
        break;
    case HOLD_KEY:
        if (player->hold_on) {
            check_input(x, y, player, dx, dy); // 이미 홀드한 상태에서 누르는 홀드키는 무시
            return;
        }
        hold(x, y, player, dx, dy);
        break;
    case BACK_KEY:
        garbage_block(x,y,P2,dx,dy);
        break;
    }
    draw_map(x,y, player, dx, dy);
    init_queue(&player->key_queue);
    
    if (player->crush_on && check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == false) {
        erase_shadow_block(player,dx,dy);
        
        Sleep(200);
    }
    //블록이 충돌중인경우 추가로 이동및 회전할 시간을 갖음 
    
}

void set_new_block(struct Player_info* player, int dx, int dy) { 
    int i, j;
    player->bx = (dx / 2) - 1;
    player->by = 0;
    player->b_rotation = 0;


    player->new_block_on = 0;
    for (i = 0; i < 4; i++) { //
        for (j = 0; j < 4; j++) {
            if (blocks[player->b_type[0][player->b_now]][player->b_rotation][i][j] == 1) {
                player->main_org[player->by + i][player->bx + j].b_color = COLOR[player->b_type[0][player->b_now]];
                player->main_org[player->by + i][player->bx + j].b_status = ACTIVE_BLOCK;
            }
        }
    }


}

void update_game(int x, int y, struct Player_info* player, int dx, int dy) {
    if (!player->new_hold_on) {
        drop_block(player, dx, dy);
    }   
    draw_map(x, y, player, dx, dy);
    send_attack(player);
    if (check_game_over(player, dx, dy) == TRUE) {
        is_game_over = 1;
        game_over_func(player);
        return;
    }
    if (player->new_block_on) {
        new_block(player);
        set_new_block(player, dx, dy);
        player->new_block_on = 0;
        player->hard_drop_key_on = 0;
        player->new_hold_on=0;
        player->hold_on = 0;
        draw_next(x + dx + 1, y + 2, player, dx,dy);
    }
    erase_shadow_block(player,dx,dy);
    set_shadow_block(player);
    draw_map(x,y, player, dx, dy);

    
}


void check_get_attacked(int x, int y, struct Player_info* player, int dx, int dy) {
    if (player->attacked > 0) {
        if (player->sent_garbage > 0) {
            if (player->attacked>=player->sent_garbage) {
                player->attacked -= player->sent_garbage;
                player->sent_garbage = 0;
            }
            else {
                player->sent_garbage -= player->attacked;
                player->attacked = 0;
            }
        }
        for (int i = 0; i < player->attacked; ++i) {
            garbage_block(x, y, player, dx, dy);
        }
    }
    player->attacked = 0;
}

void send_attack(struct Player_info* player) {
    if (player->attack_on == 1) {
        
        player->target->attacked += player->sent_garbage;
        
        player->sent_garbage = 0;
        player->combo = 0;
        player->attack_on = 0;
    }
    
}

int check_game_over(struct Player_info* player, int dx, int dy) {
    for (int i = 1; i < dx - 2; i++) {
        if (player->main_org[3][i].b_status > 0) {
            return TRUE;
        }
    }
    return FALSE;
}



void get_game_input(int p) { //p=1 1인 플레이어 p=2 2인플레이어
    while (1) {
        if (_kbhit()) { //키입력이 있는 경우  
            key = check_is_upper(_getch());
            if (key == 224) { //방향키인경우 
                do { key = _getch(); } while (key == 224);//방향키지시값을 버림 
                key += 128;
            }
            else if (key >= 128) continue;
            for (int i = 0; i < p; ++i) {

                if (key == Player_info[i].left_key) {
                    enqueue(&(Player_info[i].key_queue), LEFT_KEY);
                }
                if (key == Player_info[i].right_key) {
                    enqueue(&(Player_info[i].key_queue), RIGHT_KEY);
                }
                if (key == Player_info[i].down_key) {
                    enqueue(&(Player_info[i].key_queue), DOWN_KEY);
                }
                if (key == Player_info[i].hard_drop_key) {
                    enqueue(&(Player_info[i].key_queue), HARD_DROP_KEY);
                }
                if (key == Player_info[i].rotate_key) {
                    enqueue(&(Player_info[i].key_queue), ROTATE_KEY);
                }
                if (key == Player_info[i].rotate_counter_key) {
                    enqueue(&(Player_info[i].key_queue), ROTATE_COUNTER_KEY);
                }
                if (key == Player_info[i].hold_key) {
                    enqueue(&(Player_info[i].key_queue), HOLD_KEY);
                }
                if (key == Player_info[i].esc_key) {
                    enqueue(&(Player_info[i].key_queue), BACK_KEY);
                }
            }
        }
        
        
        while (_kbhit()) _getch(); //키버퍼를 비움 
    }
    
}




void game_scene(MODE_FUNC f, OPTION_FUNC o) {
    f(init_game);
    o();
    //HANDLE hThrd_getch = (HANDLE)_beginthreadex(NULL, 0, main_theme, 0, 0, NULL);
    while (1) {
        for (int i = 0; i < 50; ++i) {
            f(check_input);
            Sleep(frame_time);
        }
        f(update_game);
        if (is_game_over) {
            return;
        }
        Sleep(100*frame_time);
    }
}

void game_over_2p_battle_game(struct Player_info* player) {
    int winner;
    if (player == P1) winner = 2;
    else winner = 1;
    
    system("cls");
    int x = 5;
    int y = 5;
    gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤"); //게임오버 메세지 
    gotoxy(x, y + 1); printf("▤                              ▤");
    gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
    gotoxy(x, y + 3); printf("▤  |  G A M E  O V E R..   |   ▤");
    gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
    gotoxy(x, y + 5); printf("▤   WINNER IS PLAYER  %d        ▤", winner);
    gotoxy(x, y + 6); printf("▤                              ▤");
    gotoxy(x, y + 7); printf("▤  Press any key to restart..  ▤");
    gotoxy(x, y + 8); printf("▤                              ▤");
    gotoxy(x, y + 9); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
    _endthreadex(hTHrd_input);
    while(1){}
    
}

void option_2p_battle_game(void) {
    P1->is_attack_mode = 1;
    P2->is_attack_mode = 1;
    P1->attack_on = 0;
    P2->attack_on = 0;
    P1->target = P2;
    P2->target = P1;
    game_over_func = game_over_2p_battle_game;
    is_game_over = 0;
    hTHrd_input = (HANDLE)_beginthreadex(NULL, 0, get_game_input, 2, 0, NULL);
}



void check_line(int x, int y, struct Player_info* player, int dx, int dy) {
    int i, j, k, l;

    int block_amount; //한줄의 블록갯수를 저장하는 변수 
    int line=0;

    for (i = dy - 2; i > 3;) { //i=MAIN_Y-2 : 밑쪽벽의 윗칸부터,  i>3 : 천장(3)아래까지 검사 
        block_amount = 0; //블록갯수 저장 변수 초기화 
        for (j = 1; j < dx - 1; j++) { //벽과 벽사이의 블록갯루를 셈 
            if (player->main_org[i][j].b_status > 0) block_amount++;
        }
        if (block_amount == dx - 2) { //블록이 가득 찬 경우 
            line++;
            for (k = i; k > 1; k--) { //윗줄을 한칸씩 모두 내림(윗줄이 천장이 아닌 경우에만) 
                for (l = 1; l < dx - 1; l++) {
                    if (player->main_org[k - 1][l].b_status != CEILLING) {
                        player->main_org[k][l].b_status = player->main_org[k - 1][l].b_status;
                        player->main_org[k][l].b_color = player->main_org[k - 1][l].b_color;
                    } 
                    if (player->main_org[k - 1][l].b_status == CEILLING) {
                        player->main_org[k][l].b_status = EMPTY;
                        player->main_org[k][l].b_color = COLOR_RESET;
                    } 
                    //윗줄이 천장인 경우에는 천장을 한칸 내리면 안되니까 빈칸을 넣음 
                }
            }
        }
        else i--;
    }
    if (line>0) { //줄 삭제가 있는 경우 점수와 레벨 목표를 새로 표시함  
        player->combo++;
        if (player->is_t_spin == 1) {
            player->sent_garbage += t_spin_damage[line];
        }
        else {
            player->sent_garbage += line_damage[line];
        }
        player->attack_on = 1;
        draw_map(x, y, player,dx, dy);
    }
    else {
        player->sent_garbage += combo_damage[player->combo];
        player->combo = 0;
        player->attack_on = 1;
    }
    player->is_t_spin = 0;
}




