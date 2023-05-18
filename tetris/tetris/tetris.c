#define _CRT_SECURE_NO_WARNINGS //scanf, strcpy ���� ����

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include <string.h>
#include <process.h>
#include <handleapi.h>



/// Ű���� ����
#define LEFT 75 // ���� ȭ��ǥ
#define RIGHT 77 // ���� ȭ��ǥ
#define UP 72 // ���� ȭ��ǥ
#define DOWN 80 // �Ʒ��� ȭ��ǥ
#define SPACE 32 // �����̽�
#define ESC 27 // �̽�������
#define ENTER 13 // ����

#define CURSOR_OFF "   "
#define CURSOR_ON_LEFT ">> "
#define CURSOR_ON_RIGHT " <<"

#define LEFT_CHAR "   �� "
#define UP_CHAR "   �� "
#define RIGHT_CHAR "   �� "
#define DOWN_CHAR "   �� "
#define SPACE_CHAR "SPACE"
#define ENTER_CHAR "ENTER"
#define ESC_CHAR " ESC "
#define INVALID_CHAR "ERROR"


#define false 0
#define true 1

#define CLOCKWISE 0
#define COUNTER_CLOCKWISE 1



#define MAIN_X_1 11
#define MAIN_Y_1 23


#define SHADOW_BLOCK -3 // �ϵ��� ��� �̸� �����ֱ� 
#define ACTIVE_BLOCK -2 // �����ǹ迭�� ����� ����� ���µ� 
#define CEILLING -1     // ����� �̵��� �� �ִ� ������ 0 �Ǵ� ���� ������ ǥ�� 
#define EMPTY 0         // ����� �̵��� �� ���� ������ ����� ǥ�� 
#define WALL 1
#define INACTIVE_BLOCK 2 // �̵��� �Ϸ�� ��ϰ� 

#define MAX_INPUT 1000
#define XY_MAX 50
#define COLOR_RESET "\033[0m"
#define PRINT_BLOCK(X) printf("%s��%s",X,COLOR_RESET)
#define PRINT_SHADOW_BLOCK(X) printf("%s��%s",X,COLOR_RESET)

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
int STATUS_Y_GOAL; //GOAL ����ǥ����ġY ��ǥ ����
int STATUS_Y_LEVEL; //LEVEL ����ǥ����ġY ��ǥ ����
int STATUS_Y_SCORE; //SCORE ����ǥ����ġY ��ǥ ����
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
    int b_type[2][7]; // ��Ʈ���� ���� ����
    int b_rotation; // ��� ȸ���� ���� 
    int b_now; // ���� �ε��� ���� 
    int bx, by; // �̵����� ����� �����ǻ��� x,y��ǥ�� ���� 
    int shadow_bx, shadow_by; //�̵����� ����� �������� �����ǻ��� x, y��ǥ�� ����
    int x, y;
    int new_block_on; // ���ο� ����� �ʿ����� �˷��ִ� flag
    int crush_on;//���� �̵����� ����� �浹�������� �˷��ִ� flag 
    int hard_drop_key_on;//hard drop�������� �˷��ִ� flag 
    int hold_block_type; // hold ���� ��� ����
    int new_hold_on;
    int hold_on;
    int t_spin_available;

    int combo; //�̾��� �޺� ����
    int sent_garbage; // ������ ���غ� ���� - line clear�� ���� ������ ���
    //�Ѱ��� ��� = sent_garbage�� ���� + combo ����
    int attack_on; // ������ ������ ���� flag

    struct Key_queue key_queue;

    int is_attack_mode; // ������ �ϴ� �������
    struct Player_info* target; // ������ �ϴ� Ÿ��
    int attacked; // ������ ���� �� ����صδ� ��


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
}; //��ϸ�� ���� 4*4������ ����� ǥ�� blcoks[b_type][b_rotation][i][j]�� ��� // ���ڰ� Ŀ���� �ð���� �۾����� �ݽð� ����

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
}; // block index = 0 �̸� ȸ�� X, index =1 �̸� 0��ǥ, �׿� 1��ǥ ���� �˻�
//data[index����][���� rotation index][�ð���� - 0, �ݽð���� - 1][5][2] 
//dx�� ���ϰ�, dy�� ������ �ֿ�����

int t_spin_damage[4] = { 0,2,4,6 };
int combo_damage[21] = { 0,0,1,1,2,2,3,3,4,4,4,5,5,5,5,5,5,5,5,5,5};
int line_damage[5] = { 0,0,1,2,4 };
int b_type[2][2][7] = { { { 0,1,2,3,4,5,6 } ,{ 0,1,2,3,4,5,6 } } ,{ { 0,1,2,3,4,5,6 } ,{ 0,1,2,3,4,5,6 } } }; // ��Ʈ���� ���� ����
int b_rotation[2]; // ��� ȸ���� ���� 
int b_now[2]; // ���� �ε��� ���� 
int bx[2], by[2]; // �̵����� ����� �����ǻ��� x,y��ǥ�� ���� 


int main_org[2][XY_MAX][XY_MAX]; //�������� ������ �����ϴ� �迭 ����Ϳ� ǥ���Ŀ� main_cpy�� ����� 
int main_cpy[2][XY_MAX][XY_MAX]; //�� maincpy�� �������� ����Ϳ� ǥ�õǱ� ���� ������ ������ ���� 
//main�� ��ü�� ��� ����Ϳ� ǥ������ �ʰ�(�̷��� �ϸ� ����Ͱ� �����Ÿ�) 
//main_cpy�� �迭�� ���ؼ� ���� �޶��� ���� ����Ϳ� ��ħ 




int key; //Ű����� �Է¹��� Ű���� ���� 
int new_block_on = 0; //���ο� ���� �ʿ����� �˸��� flag 
int crush_on = 0; //���� �̵����� ����� �浹�������� �˷��ִ� flag 
int level_up_on = 0; //���������� ����(���� ������ǥ�� �Ϸ�Ǿ�����) �˸��� flag 
int space_key_on = 0; //hard drop�������� �˷��ִ� flag 



int speed; //��������ӵ� 
int level; //���� level 
int level_goal; //���������� �Ѿ�� ���� ��ǥ���� 
int cnt; //���� �������� ������ �� ���� ���� 
int score; //���� ���� 
int last_score = 0; //�������������� 
int best_score = 0; //�ְ�������� 


int is_game_over;

GAME_OVER_FUNC* game_over_func;
HANDLE hThrd_bgm;
HANDLE hTHrd_input;


void init_data(void);

void title_scene(void);
void draw_title_scene(int x, int y, TITLE_MENU menu);


void mode_select_scene(void);

void drop_block(struct Player_info* player, int dx, int dy);

//�ܼ�â draw/erase �Լ�
void init_player(struct Player_info* player);

void draw_game_scene(void);
void draw_interface(int x, int y); // (x,y)�� �������� �⺻�������̽� �׸���
void erase_scene(int x, int y, int dx, int dy);
void draw_block(int x, int y, int type, int rotation);

char* key_string_set(int key);
char* key_set(int key);
// ��Ʈ�� Ÿ������ ��ȯ�ϴ� �Լ�

void setting_scene(void);
void draw_setting_scene_player(int x, int y, struct Player_info* player);
void setting_scene_set(int x, int y, KEY_TYPE type, struct Player_info* player); // �ɼ� �޾ƿ���
void draw_setting_scene(int x, int y, KEY_TYPE type, struct Player_info* player);
void setting_new_key(int x, int y, int* key_set, KEY_TYPE type, struct Player_info* player);

void reset_org_cpy(struct Player_info* player, int dx, int dy); //main_org[][]�� �ʱ�ȭ, dx, dy��ŭ
void save_org_cpy(struct Player_info* player, int dx, int dy); // ������(main_cpy[][]�� �ʱ�ȭ)
void shuffle_block(struct Player_info* player);
/*���� ��ƾ
* b_now++ �ϰ�
* b_now = 7�� ��� 
*�迭1 �� ����ϸ�
*�迭 2 -> �迭1�� ����
*�迭2 �����
*�ʱ� ���� -> �� �� �����ؾ���
* b_now =0���� �ٲٱ�
*/



void game_scene(MODE_FUNC f, OPTION_FUNC o);


void execute_2p_battle_game_func(GAME_FUNC f);

void init_game(int x, int y, struct Player_info* player, int dx, int dy);
void check_input(int x, int y, struct Player_info* player, int dx, int dy);
void update_game(int x, int y, struct Player_info* player, int dx, int dy);



void init_game_2p_battle_scene(void);
void game_2p_battle_scene(void);
void draw_game_2p_battle_scene(void);


void get_game_input(int p);

void draw_next(int x, int y, struct Player_info* player);
void draw_map(int x, int y, struct Player_info* player, int dx, int dy);

void draw_hold(struct Player_info* player, int dx, int dy);
void draw_next_block(int x, int y, struct Player_info* player);
void new_block(struct Player_info* player);// ���ο� ����� key ��������
void set_new_block(struct Player_info* player, int dx, int dy);// x,y �� ������
void erase_active_block(struct Player_info* player, int dx, int dy);
void set_shadow_block(struct Player_info* player);
void erase_shadow_block(struct Player_info* player, int dx, int dy);
void check_key(struct Player_info* player,int dx, int dt); // Ű����� Ű �޾ƿ���
int check_is_upper(int k); // Ű�����  �޾ƿ� Ű �빮�� �˻�
int check_is_rotatable(struct Player_info* player, int type,int rotation, int wise, int bx, int by,int dx, int dy); // ȸ�� ���� �ϴٸ� ����� x,y ���� �������� ����� ��ǥ�� �������� ��Ʈ���� ��ġ
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
int bpm = 125; // 16�� ��ǥ bpm
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


    int i;
    srand((unsigned)time(NULL)); //����ǥ����
    setcursortype(NOCURSOR); //Ŀ�� ����

    title_scene(); //����Ÿ��Ʋ ȣ��
    

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
    int x = 5; //Ÿ��Ʋȭ���� ǥ�õǴ� x��ǥ
    int y = 4; //Ÿ��Ʋȭ���� ǥ�õǴ� y��ǥ
    int cnt; //Ÿ��Ʋ �������� ���� ����
    TITLE_MENU menu = GAME_START;

    key = 0;

    gotoxy(x, y + 0); printf("����������������"); Sleep(100);
    gotoxy(x, y + 1); printf("�����  ����    ������"); Sleep(100);
    gotoxy(x, y + 2); printf("�����              ���  ��"); Sleep(100);
    gotoxy(x, y + 3); printf("������  ��  ��  ������"); Sleep(100);
    gotoxy(x, y + 4); printf("���  �������������"); Sleep(100);
    gotoxy(x + 5, y + 2); printf("T E T R I S"); Sleep(100);
    gotoxy(x, y + 6); printf(" Press Enter ");
    draw_title_scene(x, y, menu);

    for (cnt = 0;; cnt++) { //cnt�� 1�� ������Ű�鼭 ��� �ݺ�    //�ϳ��� ���߿��� �� ��¦�̴� �ִϸ��̼�ȿ��
        if (_kbhit()) {
            key = _getch();
            if (key == ENTER) break;
            if (key == 224) {
                do { key = _getch(); } while (key == 224);//����Ű���ð��� ����
                switch (key) {
                case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó����
                    menu = (menu + 1) % 3;
                    draw_title_scene(x, y, menu);
                    break;
                case UP: //���� ����Ű ��������
                    menu = (menu + 2) % 3;
                    draw_title_scene(x, y, menu);
                }
            }
            while (_kbhit()) _getch();
        }
        if (cnt % 200 == 0) { gotoxy(x + 4, y + 1); printf("��"); }       //cnt�� 200���� ������ �������� ���� ǥ��
        if ((cnt % 200 - 100) == 0) { gotoxy(x + 4, y + 1); printf("  "); } //�� ī��Ʈ���� 100ī��Ʈ �������� ���� ����
        if ((cnt % 350) == 0) { gotoxy(x + 13, y + 2); printf("��"); } //������ ������ �ð����� ����
        if ((cnt % 350 - 100) == 0) { gotoxy(x + 13, y + 2); printf("  "); }
        Sleep(10);
    }

    switch (menu) {
    case GAME_START:
        system("cls");
        hTHrd_input = (HANDLE)_beginthreadex(NULL, 0, get_game_input, 2, 0, NULL);
        game_scene(execute_2p_battle_game_func,option_2p_battle_game);
        //game_2p_battle_scene(); // ���߿� ���� ��������
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

//�������� - ���� Ű �Է� �� �ۼ���
void setting_scene() {
    int x = 5; //Ÿ��Ʋȭ���� ǥ�õǴ� x��ǥ
    int y = 3; //Ÿ��Ʋȭ���� ǥ�õǴ� y��ǥ
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
                do { key = _getch(); } while (key == 224);//����Ű���ð��� ����
                switch (key) {
                case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó����
                    type = type + 1;
                    if (type == 8) type = 1;
                    while (_kbhit()) _getch();
                    draw_setting_scene(x, y, type,player);
                    break;
                case UP: //���� ����Ű ��������
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
        gotoxy(x, y + 6); printf("��  %s    LEFT     :     %6s  %s   ��", " ->", key_set(player->left_key), " <-");
        //setting_new_key(x, y, &left_key[player], type, player);
        break;
    case RIGHT_KEY:
        gotoxy(x, y + 7); printf("��  %s   RIGHT     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(player->right_key), CURSOR_ON_RIGHT);
        break;
    case DOWN_KEY:
        gotoxy(x, y + 8); printf("��  %s    DOWN     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(player->down_key), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_KEY:
        gotoxy(x, y + 9); printf("��  %s HARD DROP   :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(player->hard_drop_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_KEY:
        gotoxy(x, y + 10); printf("��  %s    ȸ��     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(player->rotate_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_KEY:
        gotoxy(x, y + 11); printf("��  %s�ݽð� ȸ��  :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(player->rotate_counter_key), CURSOR_ON_RIGHT);
        break;
    case HOLD_KEY:
        gotoxy(x, y + 12); printf("��  %s    HOLD     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(player->hold_key), CURSOR_ON_RIGHT);
        break;
    }
}

void draw_setting_scene_player(int x, int y, struct Player_info* player) {
    if (player == P1) {
        gotoxy(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
        gotoxy(x, y + 1); printf("��                                      ��");
        gotoxy(x, y + 2); printf("��      +-----------------------+       ��");
        gotoxy(x, y + 3); printf("��      |   S E T T I N G - P1  |       ��");
        gotoxy(x, y + 4); printf("��      +-----------------------+       ��");
        gotoxy(x, y + 5); printf("��                                      ��");
        gotoxy(x, y + 15); printf("��                                      ��");
        gotoxy(x, y + 16); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
    }
    if (player == P2) {
        gotoxy(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
        gotoxy(x, y + 1); printf("��                                      ��");
        gotoxy(x, y + 2); printf("��      +-----------------------+       ��");
        gotoxy(x, y + 3); printf("��      |   S E T T I N G - P2  |       ��");
        gotoxy(x, y + 4); printf("��      +-----------------------+       ��");
        gotoxy(x, y + 5); printf("��                                      ��");
        gotoxy(x, y + 15); printf("��                                      ��");
        gotoxy(x, y + 16); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
    }
}

void setting_new_key(int x, int y, int* key_set, KEY_TYPE type, struct Player_info* player) {
    while (1) {
        if (_kbhit()) {
            key = _getch();
            
            if (key == 224) {
                do { key = _getch(); } while (key == 224);//����Ű���ð��� ����
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
    gotoxy(x, y + 6); printf("��  %s    LEFT     :     %6s  %s   ��", CURSOR_OFF, key_set(player->left_key), CURSOR_OFF);
    gotoxy(x, y + 7); printf("��  %s   RIGHT     :     %6s  %s   ��", CURSOR_OFF, key_set(player->right_key), CURSOR_OFF);
    gotoxy(x, y + 8); printf("��  %s    DOWN     :     %6s  %s   ��", CURSOR_OFF, key_set(player->down_key), CURSOR_OFF);
    gotoxy(x, y + 9); printf("��  %s HARD DROP   :     %6s  %s   ��", CURSOR_OFF, key_set(player->hard_drop_key), CURSOR_OFF);
    gotoxy(x, y + 10); printf("��  %s    ȸ��     :     %6s  %s   ��", CURSOR_OFF, key_set(player->rotate_key), CURSOR_OFF);
    gotoxy(x, y + 11); printf("��  %s�ݽð� ȸ��  :     %6s  %s   ��", CURSOR_OFF, key_set(player->rotate_counter_key), CURSOR_OFF);
    gotoxy(x, y + 12); printf("��  %s    HOLD     :     %6s  %s   ��", CURSOR_OFF, key_set(player->hold_key), CURSOR_OFF);
    gotoxy(x, y + 13); printf("��                                      ��");
    gotoxy(x, y + 14); printf("��      PRESS ESC TO BACK TO MAIN       ��");


    switch (type) {
    case LEFT_KEY:
        gotoxy(x, y + 6); printf("��  %s    LEFT     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(player->left_key), CURSOR_ON_RIGHT);
        break;
    case RIGHT_KEY:
        gotoxy(x, y + 7); printf("��  %s   RIGHT     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(player->right_key), CURSOR_ON_RIGHT);
        break;
    case DOWN_KEY:
        gotoxy(x, y + 8); printf("��  %s    DOWN     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(player->down_key), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_KEY:
        gotoxy(x, y + 9); printf("��  %s HARD DROP   :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(player->hard_drop_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_KEY:
        gotoxy(x, y + 10); printf("��  %s    ȸ��     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(player->rotate_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_KEY:
        gotoxy(x, y + 11); printf("��  %s�ݽð� ȸ��  :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(player->rotate_counter_key), CURSOR_ON_RIGHT);
        break;
    case HOLD_KEY:
        gotoxy(x, y + 12); printf("��  %s    HOLD     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(player->hold_key), CURSOR_ON_RIGHT);
        break;

    }
}

//�������� - ���� Ű �Է� �� �ۼ���


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
    for (i = 0; i < dy - 1; i++) { //�¿� ���� ����  
        player->main_org[i][0].b_status = WALL;
        player->main_org[i][dx - 1].b_status = WALL;

        
    }
    for (j = 0; j < dx; ++j) { //�ٴں��� ���� 
        player->main_org[dy - 1][j].b_status = WALL;

    }
}



void save_org_cpy(struct Player_info* player , int dx, int dy) {
    for (int i = 0; i < dx; i++) { //�������� �׸� �� main_cpy�� ����  
        for (int j = 0; j < dy; j++) {
            player->main_cpy[i][j].b_color = player->main_org[i][j].b_color;
            player->main_cpy[i][j].b_status = player->main_org[i][j].b_status;

        }
    }
}

void shuffle_block(struct Player_info* player) // ����� �Ϸ�
{

    for (int i = 0; i < 7; ++i) {
        player->b_type[0][i] = player->b_type[1][i]; // Ȧ��� b_type[0]���� �����ϱ� ������ b-type[1]�� ������ �Ѽյ��� �ʴ´�.
    }
    for (int i = 0; i < 7; ++i) {
        player->b_type[1][i] = i; // Ȧ��� ���� ���� ���̴� �� ����
    }
    
    int n, temp;
    for(int i = 0; i < 6; ++i) {
        n = rand() % (7 - i) + i;    // i ���� num-1 ���̿� ������ ���� ����

        temp = player->b_type[1][i];
        player->b_type[1][i] = player->b_type[1][n];
        player->b_type[1][n] = temp;


        
    }
    
}

void new_block(struct Player_info* player) { // ���ο� ��� �ε��� �����ϱ� //����� �Ϸ�
    player->b_now++;

    
    if (player->b_now >= 7) {
        shuffle_block(player);
        player->b_now = 0;
    }
    
}

void set_new_block(struct Player_info* player, int dx, int dy) { //���ο� ��� �׸���
    int i, j;
    player->bx = (dx / 2) - 1;
    player->by = 0;
    player->b_rotation = 0;


    new_block_on = 0;
    for (i = 0; i < 4; i++) { //������ bx, by��ġ�� ������  
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
    for (int i = 0; i < 4; i++) { //������ bx, by��ġ�� �ε��� ������  
        for (int j = 0; j < 4; j++) {
            if (blocks[type][rotation][i][j] == 1&& player->main_org[player->shadow_by + i][player->shadow_bx + j].b_status != ACTIVE_BLOCK) {
                player->main_org[player->shadow_by + i][player->shadow_bx + j].b_color = SHADOW_COLOR[type];
                player->main_org[player->shadow_by + i][player->shadow_bx + j].b_status = SHADOW_BLOCK;
            }
        }
    }

    
}

void erase_active_block(struct Player_info* player, int dx, int dy) {
    for (int i = 0; i < dy; i++) { //���� �������� ���� ���� 
        for (int j = 0; j < dx; j++) {
            if (player->main_org[i][j].b_status == ACTIVE_BLOCK) {
                player->main_org[i][j].b_status = EMPTY;
                player->main_org[i][j].b_color = COLOR_RESET;
            } 
        }
    }
}

void erase_shadow_block(struct Player_info* player, int dx, int dy) {
    
    for (int i = 0; i < dy; i++) { //���� �������� ���� ���� 
        for (int j = 0; j < dx; j++) {
            if (player->main_org[i][j].b_status == SHADOW_BLOCK) {
                player->main_org[i][j].b_status = EMPTY;
                player->main_org[i][j].b_color = COLOR_RESET;
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



void draw_game_2p_battle_scene(void) { // �⺻���� Ʋ �׸���, 
    int x = 3, y = 1;
    draw_map(x,y,P1, MAIN_X_1, MAIN_Y_1);
    draw_next(x+MAIN_X_1+1,y+2,P1, MAIN_X_1, MAIN_Y_1);
    draw_map(x+MAIN_X_1+10+x , y, P2, MAIN_X_1, MAIN_Y_1);
    draw_next(2*x + 2*MAIN_X_1 + 11, y + 2, P2, MAIN_X_1, MAIN_Y_1);
}

void draw_block(int x, int y, int type, int rotation) { //��� �׸���

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


void draw_next(int x, int y, struct Player_info* player) { //draw_next�� �ʱ�ȭ������ �׸��� (�����̴� �� ����)
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

    for (j = 1; j < dx - 1; j++) { //õ���� ��� ���ο���� �������� �������� ���� �׷��� 
        if (player->main_org[3][j].b_status == EMPTY) {
            player->main_org[3][j].b_status = CEILLING;
            player->main_org[3][j].b_color = COLOR_RESET;
        }  
    }
    for (i = 0; i < dy; i++) {
        for (j = 0; j < dx; j++) {
            if (has_change(&(player->main_cpy[i][j]), &(player->main_org[i][j]))){ //cpy�� ���ؼ� ���� �޶��� �κи� ���� �׷���.
                //�̰� ������ ��������ü�� ��� �׷��� �������� ��¦�Ÿ�
                gotoxy(x + j, y + i);
                switch (player->main_org[i][j].b_status) {
                case EMPTY: //��ĭ��� 
                    printf("  ");
                    break;
                case CEILLING: //õ���� 
                    printf(". ");
                    break;
                case WALL: //����� 
                    printf("��");
                    break;
                case INACTIVE_BLOCK: //���� �� ���  
                    //printf("��");
                    PRINT_BLOCK(player->main_org[i][j].b_color);
                    break;
                case ACTIVE_BLOCK: //�����̰��ִ� �� ���  
                    PRINT_BLOCK(player->main_org[i][j].b_color);
                    break;
                case SHADOW_BLOCK: //�����̰��ִ� �� ���  
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

int check_crush(struct Player_info* player,int type, int rotation, int bx, int by) { //������ ��ǥ�� ȸ�������� �浹�� �ִ��� �˻� 
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) { //������ ��ġ�� �����ǰ� ������� ���ؼ� ��ġ�� false�� ���� 
            if (blocks[type][rotation][i][j] == 1 && player->main_org[by + i][bx + j].b_status > 0) return FALSE;
        }
    }
    return TRUE; //�ϳ��� �Ȱ�ġ�� true���� 
}

int check_is_upper(int k) {// Ű�����  �޾ƿ� Ű �빮�� �˻�
    if (k >= 'a' && k <= 'z') {
        k = k - 'a' + 'A';
    }
    return k;
}


void drop_block(struct Player_info* player, int dx, int dy) {
    int i, j;

    if (player->crush_on && check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == true) crush_on = 0; //���� ��������� crush flag �� 
    if (player->crush_on && check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == false) { //���� ��������ʰ� crush flag�� ���������� 
        for (i = 0; i < dy; i++) { //���� �������� ���� ���� 
            for (j = 0; j < dx; j++) {
                if (player->main_org[i][j].b_status == ACTIVE_BLOCK) player->main_org[i][j].b_status = INACTIVE_BLOCK;
            }
        }
        player->crush_on = 0; //flag�� �� 
        check_line(player->x, player->y,player,dx,dy); //����üũ�� �� 
        //check_attack_on
        check_get_attacked(player->x, player->y, player, dx, dy);
        player->new_block_on = 1; //���ο� ������ flag�� ��    
        return; //�Լ� ���� 
    }
    if (check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == true) move_block(player,dx,dy, player->bx, player->by + 1); //���� ��������� ������ ��ĭ �̵� 
    if (check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == false) player->crush_on=1; //������ �̵��� �ȵǸ�  crush flag�� ��
}

void rotate_block(struct Player_info* player, int dx, int dy, int bx, int by, int rotate) {
    int rotation = player->b_rotation;
    if(rotate == CLOCKWISE){
        player->b_rotation = (rotation + 1) % 4;
    }
    else {
        player->b_rotation = (rotation + 3) % 4;
    }
    for (int i = 0; i < 4; i++) { //������ǥ�� ���� ���� 
        for (int j = 0; j < 4; j++) {
            if (blocks[player->b_type[0][player->b_now]][rotation][i][j] == 1) {
                player->main_org[player->by + i][player->bx + j].b_status = EMPTY;
                player->main_org[player->by + i][player->bx + j].b_color = COLOR_RESET;
            }
        }
    }
    player->bx = bx;
    player->by = by;
    for (int i = 0; i < 4; i++) { //������ǥ�� ���� ���� 
        for (int j = 0; j < 4; j++) {
            if (blocks[player->b_type[0][player->b_now]][player->b_rotation][i][j] == 1) {
                player->main_org[by + i][bx + j].b_status = ACTIVE_BLOCK;
                player->main_org[by + i][bx + j].b_color = COLOR[player->b_type[0][player->b_now]];
            }
        }
    }
}

void move_block(struct Player_info* player,int dx, int dy, int bx, int by) {
    for (int i = 0; i < 4; i++) { //������ǥ�� ���� ���� 
        for (int j = 0; j < 4; j++) {
            if (blocks[player->b_type[0][player->b_now]][player->b_rotation][i][j] == 1) {
                player->main_org[player->by + i][player->bx + j].b_status = EMPTY;
                player->main_org[player->by + i][player->bx + j].b_color = COLOR_RESET;
            } 
        }
    }
    for (int i = 0; i < 4; i++) { //������ǥ�� ���� ���� 
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
    player->hard_drop_key_on = 1; //�����̽�Ű flag�� ��� 
    while (player->crush_on == 0) { //�ٴڿ� ���������� �̵���Ŵ 
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

void attack(struct Player_info* targer, int damage) {

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
    if (player->hard_drop_key_on == 1) { //�����̽��ٸ� �������(hard drop) �߰��� �̵��� ȸ���Ҽ� ���� break; 
        player->hard_drop_key_on = 1;
        return;
    }
    if (player->new_block_on == 1) { //�����̽��ٸ� �������(hard drop) �߰��� �̵��� ȸ���Ҽ� ���� break; 
        return;
    }
    //Ű�Է�Ȯ�� 
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
            check_input(x, y, player, dx, dy); // �̹� Ȧ���� ���¿��� ������ Ȧ��Ű�� ����
            return;
        }
        hold(x, y, player, dx, dy);
        break;
    case BACK_KEY:
        garbage_block(x,y,P2,dx,dy);
        break;
    }
    draw_map(x,y, player, dx, dy);
    
    if (player->crush_on && check_crush(player, player->b_type[0][player->b_now], player->b_rotation, player->bx, player->by + 1) == false) {
        erase_shadow_block(player,dx,dy);
        if (player->b_type[0][player->b_now] == 6) {
            player->t_spin_available = 1;
        }
        Sleep(200);
    }
    //����� �浹���ΰ�� �߰��� �̵��� ȸ���� �ð��� ���� 
    
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

        for (int i = 0; i < player->attacked; ++i) {
            garbage_block(x, y, player, dx, dy);
        }
    }
    player->attacked = 0;
}

void send_attack(struct Player_info* player) {
    int damage = 0;
    if (player->attack_on == 1) {
        
        damage += player->sent_garbage;
        damage += combo_damage[player->combo];
        player->target->attacked += damage;
        
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



void get_game_input(int p) { //p=1 1�� �÷��̾� p=2 2���÷��̾�
    while (1) {
        if (_kbhit()) { //Ű�Է��� �ִ� ���  
            key = check_is_upper(_getch());
            if (key == 224) { //����Ű�ΰ�� 
                do { key = _getch(); } while (key == 224);//����Ű���ð��� ���� 
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
        
        //while (_kbhit()) _getch(); //Ű���۸� ��� 
    }
    
}


void game_scene(MODE_FUNC f, OPTION_FUNC o) {
    f(init_game);
    o();
    //HANDLE hThrd_getch = (HANDLE)_beginthreadex(NULL, 0, main_theme, 0, 0, NULL);
    while (1) {
        for (int i = 0; i < 20; ++i) {
            f(check_input);
            Sleep(25);
        }
        f(update_game);
        if (is_game_over) {
            return;
        }
        Sleep(100);
    }
}

void game_over_2p_battle_game(struct Player_info* player) {
    int winner;
    if (player == P1) winner = 2;
    else winner = 1;
    
    _endthreadex(hTHrd_input);
    int x = 5;
    int y = 5;
    gotoxy(x, y + 0); printf("�� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��"); //���ӿ��� �޼��� 
    gotoxy(x, y + 1); printf("��                              ��");
    gotoxy(x, y + 2); printf("��  +-----------------------+   ��");
    gotoxy(x, y + 3); printf("��  |  G A M E  O V E R..   |   ��");
    gotoxy(x, y + 4); printf("��  +-----------------------+   ��");
    gotoxy(x, y + 5); printf("��   WINNER IS PLAYER  %d        ��", winner);
    gotoxy(x, y + 6); printf("��                              ��");
    gotoxy(x, y + 7); printf("��  Press any key to restart..  ��");
    gotoxy(x, y + 8); printf("��                              ��");
    gotoxy(x, y + 9); printf("�� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��");

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
}



void check_line(int x, int y, struct Player_info* player, int dx, int dy) {
    int i, j, k, l;

    int block_amount; //������ ��ϰ����� �����ϴ� ���� 
    int line=0;

    for (i = dy - 2; i > 3;) { //i=MAIN_Y-2 : ���ʺ��� ��ĭ����,  i>3 : õ��(3)�Ʒ����� �˻� 
        block_amount = 0; //��ϰ��� ���� ���� �ʱ�ȭ 
        for (j = 1; j < dx - 1; j++) { //���� �������� ��ϰ��縦 �� 
            if (player->main_org[i][j].b_status > 0) block_amount++;
        }
        if (block_amount == dx - 2) { //����� ���� �� ��� 
            line++;
            for (k = i; k > 1; k--) { //������ ��ĭ�� ��� ����(������ õ���� �ƴ� ��쿡��) 
                for (l = 1; l < dx - 1; l++) {
                    if (player->main_org[k - 1][l].b_status != CEILLING) {
                        player->main_org[k][l].b_status = player->main_org[k - 1][l].b_status;
                        player->main_org[k][l].b_color = player->main_org[k - 1][l].b_color;
                    } 
                    if (player->main_org[k - 1][l].b_status == CEILLING) {
                        player->main_org[k][l].b_status = EMPTY;
                        player->main_org[k][l].b_color = COLOR_RESET;
                    } 
                    //������ õ���� ��쿡�� õ���� ��ĭ ������ �ȵǴϱ� ��ĭ�� ���� 
                }
            }
        }
        else i--;
    }
    if (line>0) { //�� ������ �ִ� ��� ������ ���� ��ǥ�� ���� ǥ����  
        player->combo++;
        if (player->t_spin_available == 1) {
            player->sent_garbage += t_spin_damage[line];
        }
        else {
            player->sent_garbage += line_damage[line];
        }
        
        draw_map(x, y, player,dx, dy);
    }
    else {
        player->attack_on = 1;
    }
    player->t_spin_available = 0;
}


/* �ڵ� ��������

void game_2p_battle_scene(void) {
    //���� ����
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
    //���� ��



    while (1) {
        for (int i = 0; i < 5; ++i) {
            //check_key(P1,MAIN_X_1, MAIN_Y_1);
        }
        drop_block(P1,MAIN_X_1,MAIN_Y_1);
        //drop_block(P2,MAIN_X_1,MAIN_Y_1);
        draw_map(x[0], y[0], P1, MAIN_X_1, MAIN_Y_1);
        draw_map(x[1], y[1], P2, MAIN_X_1, MAIN_Y_1);
        //execute_2p_battle_game_func(draw_map);
        if (P1->new_block_on) {
            new_block(P1);
            set_new_block(P1, MAIN_X_1, MAIN_Y_1);
            set_shadow_block(P1);
            P1->new_block_on = 0;
            draw_next(x[0] + MAIN_X_1 + 1, y[0] + 2, P1, MAIN_X_1, MAIN_Y_1);
        }
        Sleep(200);
    }
}

*/