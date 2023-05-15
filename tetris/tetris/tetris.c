#define _CRT_SECURE_NO_WARNINGS //scanf, strcpy ���� ����

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include <string.h>


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


#define P1 0
#define P2 1

#define MAIN_X_1 11
#define MAIN_Y_1 23


#define ACTIVE_BLOCK -2 // �����ǹ迭�� ����� ����� ���µ� 
#define CEILLING -1     // ����� �̵��� �� �ִ� ������ 0 �Ǵ� ���� ������ ǥ�� 
#define EMPTY 0         // ����� �̵��� �� ���� ������ ����� ǥ�� 
#define WALL 1
#define INACTIVE_BLOCK 2 // �̵��� �Ϸ�� ��ϰ� 


#define XY_MAX 50

typedef enum { GAME_START = 0, KEY_SETTING, EXIT }TITLE_MENU;
typedef enum { LEFT_KEY = 0, RIGHT_KEY, DOWN_KEY, HARD_DROP_KEY, ROTATE_KEY, ROTATE_COUNTER_KEY, HOLD_KEY, BACK_KEY } KEY_TYPE;


/*
int STATUS_Y_GOAL; //GOAL ����ǥ����ġY ��ǥ ����
int STATUS_Y_LEVEL; //LEVEL ����ǥ����ġY ��ǥ ����
int STATUS_Y_SCORE; //SCORE ����ǥ����ġY ��ǥ ����
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
}; //��ϸ�� ���� 4*4������ ����� ǥ�� blcoks[b_type][b_rotation][i][j]�� ��� // ���ڰ� Ŀ���� �ð���� �۾����� �ݽð� ����



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

void title_scene(void);
void draw_title_scene(int x, int y, TITLE_MENU menu);


void mode_select_scene(void);



//�ܼ�â draw/erase �Լ�

void draw_game_scene(void);
void draw_interface(int x, int y); // (x,y)�� �������� �⺻�������̽� �׸���
void erase_scene(int x, int y, int dx, int dy);
void draw_block(int x, int y, int type, int rotation);

char* key_string_set(int key);
char* key_set(int key);
// ��Ʈ�� Ÿ������ ��ȯ�ϴ� �Լ�

void setting_scene(void);
void draw_setting_scene_player(int x, int y, int player);
void setting_scene_set(int x, int y, KEY_TYPE type, int player); // �ɼ� �޾ƿ���
void draw_setting_scene(int x, int y, KEY_TYPE type, int player);
void setting_new_key(int x, int y, int* key_set, KEY_TYPE type, int player);

void reset_org_cpy(int player, int dx, int dy); //main_org[][]�� �ʱ�ȭ, dx, dy��ŭ
void save_org_cpy(int player, int dx, int dy); // ������(main_cpy[][]�� �ʱ�ȭ)
void shuffle_block(int player);
/*���� ��ƾ
* b_now++ �ϰ�
* b_now = 7�� ��� 
*�迭1 �� ����ϸ�
*�迭 2 -> �迭1�� ����
*�迭2 �����
*�ʱ� ���� -> �� �� �����ؾ���
* b_now =0���� �ٲٱ�
*/


void game_2p_battle_scene(void);
void draw_game_2p_battle_scene(void);
void draw_next(int x, int y, int player);
void draw_map(int x, int y, int player, int dx, int dy);



int new_block();// ���ο� ����� key ��������
int check_key(void); // Ű����� Ű �޾ƿ���
int check_is_upper(int key); // Ű�����  �޾ƿ� Ű �빮�� �˻�
int check_is_rotatable(int* x, int* y); // ȸ�� ���� �ϴٸ� ����� x,y ���� �������� ����� ��ǥ�� �������� ��Ʈ���� ��ġ

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


int main() {
    HANDLE hThrd;
    //main_theme(0);
    _beginthreadex(NULL, 0, main_theme, 0,0,NULL);

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
        game_2p_battle_scene(); // ���߿� ���� ��������
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
                do { key = _getch(); } while (key == 224);//����Ű���ð��� ����
                switch (key) {
                case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó����
                    type = (type + 1) % 7;
                    while (_kbhit()) _getch();
                    draw_setting_scene(x, y, type,player);
                    break;
                case UP: //���� ����Ű ��������
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
        gotoxy(x, y + 6); printf("��  %s    LEFT     :     %6s  %s   ��", " ->", key_set(left_key[player]), " <-");
        //setting_new_key(x, y, &left_key[player], type, player);
        break;
    case RIGHT_KEY:
        gotoxy(x, y + 7); printf("��  %s   RIGHT     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(right_key[player]), CURSOR_ON_RIGHT);
        break;
    case DOWN_KEY:
        gotoxy(x, y + 8); printf("��  %s    DOWN     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(down_key[player]), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_KEY:
        gotoxy(x, y + 9); printf("��  %s HARD DROP   :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(hard_drop_key[player]), CURSOR_ON_RIGHT);
        break;
    case ROTATE_KEY:
        gotoxy(x, y + 10); printf("��  %s    ȸ��     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(rotate_key[player]), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_KEY:
        gotoxy(x, y + 11); printf("��  %s�ݽð� ȸ��  :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(rotate_counter_key[player]), CURSOR_ON_RIGHT);
        break;
    case HOLD_KEY:
        gotoxy(x, y + 12); printf("��  %s    HOLD     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(hold_key[player]), CURSOR_ON_RIGHT);
        break;
    }
}

void draw_setting_scene_player(int x, int y, int player) {
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

void setting_new_key(int x, int y, int* key_set, KEY_TYPE type, int player) {
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

void draw_setting_scene(int x, int y, KEY_TYPE type,int player) {
    gotoxy(x, y + 6); printf("��  %s    LEFT     :     %6s  %s   ��", CURSOR_OFF, key_set(left_key[player]), CURSOR_OFF);
    gotoxy(x, y + 7); printf("��  %s   RIGHT     :     %6s  %s   ��", CURSOR_OFF, key_set(right_key[player]), CURSOR_OFF);
    gotoxy(x, y + 8); printf("��  %s    DOWN     :     %6s  %s   ��", CURSOR_OFF, key_set(down_key[player]), CURSOR_OFF);
    gotoxy(x, y + 9); printf("��  %s HARD DROP   :     %6s  %s   ��", CURSOR_OFF, key_set(hard_drop_key[player]), CURSOR_OFF);
    gotoxy(x, y + 10); printf("��  %s    ȸ��     :     %6s  %s   ��", CURSOR_OFF, key_set(rotate_key[player]), CURSOR_OFF);
    gotoxy(x, y + 11); printf("��  %s�ݽð� ȸ��  :     %6s  %s   ��", CURSOR_OFF, key_set(rotate_counter_key[player]), CURSOR_OFF);
    gotoxy(x, y + 12); printf("��  %s    HOLD     :     %6s  %s   ��", CURSOR_OFF, key_set(hold_key[player]), CURSOR_OFF);
    gotoxy(x, y + 13); printf("��                                      ��");
    gotoxy(x, y + 14); printf("��      PRESS ESC TO BACK TO MAIN       ��");


    switch (type) {
    case LEFT_KEY:
        gotoxy(x, y + 6); printf("��  %s    LEFT     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(left_key[player]), CURSOR_ON_RIGHT);
        break;
    case RIGHT_KEY:
        gotoxy(x, y + 7); printf("��  %s   RIGHT     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(right_key[player]), CURSOR_ON_RIGHT);
        break;
    case DOWN_KEY:
        gotoxy(x, y + 8); printf("��  %s    DOWN     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(down_key[player]), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_KEY:
        gotoxy(x, y + 9); printf("��  %s HARD DROP   :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(hard_drop_key[player]), CURSOR_ON_RIGHT);
        break;
    case ROTATE_KEY:
        gotoxy(x, y + 10); printf("��  %s    ȸ��     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(rotate_key[player]), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_KEY:
        gotoxy(x, y + 11); printf("��  %s�ݽð� ȸ��  :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(rotate_counter_key[player]), CURSOR_ON_RIGHT);
        break;
    case HOLD_KEY:
        gotoxy(x, y + 12); printf("��  %s    HOLD     :     %6s  %s   ��", CURSOR_ON_LEFT, key_set(hold_key[player]), CURSOR_ON_RIGHT);
        break;

    }
}

//�������� - ���� Ű �Է� �� �ۼ���


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
    for (i = 1; i < dy - 1; i++) { //�¿� ���� ����  
        main_org[player][i][0] = WALL;
        main_org[player][i][dx-1] = WALL;
    }
    for (j = 0; j < dx; ++j) { //�ٴں��� ���� 
        main_org[player][dy-1][j] = WALL;
    }
}



void save_org_cpy(int player , int dx, int dy) {
    for (int i = 0; i < dx; i++) { //�������� �׸� �� main_cpy�� ����  
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
        n = rand() % (7 - i) + i;    // i ���� num-1 ���̿� ������ ���� ����
        temp =b_type[player][1][i];
        b_type[player][1][i] = b_type[player][1][n];
        b_type[player][1][n] = temp;
    }
    
}


void game_2p_battle_scene(void) {
    //���� ����
    b_now[P1] = 0;
    b_now[P2] = 0;
    reset_org_cpy(P1,MAIN_X_1,MAIN_Y_1);
    reset_org_cpy(P2,MAIN_X_1,MAIN_Y_1);
    shuffle_block(P1);
    shuffle_block(P1);
    shuffle_block(P2);
    shuffle_block(P2);
    //���� ��
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
                gotoxy(x + j, y + i); printf("��");
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

    for (j = 1; j < dx - 1; j++) { //õ���� ��� ���ο���� �������� �������� ���� �׷��� 
        if (main_org[player][3][j] == EMPTY)  main_org[player][3][j] = CEILLING;
    }
    for (i = 0; i < dy; i++) {
        for (j = 0; j < dx; j++) {
            if (main_cpy[player][i][j] != main_org[player][i][j]){ //cpy�� ���ؼ� ���� �޶��� �κи� ���� �׷���.
                //�̰� ������ ��������ü�� ��� �׷��� �������� ��¦�Ÿ�
                gotoxy(x + j, y + i);
                switch (main_org[player][i][j]) {
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
                    printf("��");
                    break;
                case ACTIVE_BLOCK: //�����̰��ִ� �� ���  
                    printf("��");
                    break;
                }
            }
        }
    }
    save_org_cpy(player, dx, dy);

}