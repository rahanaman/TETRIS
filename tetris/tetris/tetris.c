#define _CRT_SECURE_NO_WARNINGS //scanf, strcpy ���� ����

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include <string.h>

DWORD WINAPI ThreadFunc(LPVOID);

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

#define LEFT_CHAR "   ��  "
#define UP_CHAR "   ��  "
#define RIGHT_CHAR "   ��  "
#define DOWN_CHAR "   ��  "
#define SPACE_CHAR "SPACE"
#define ENTER_CHAR "ENTER"
#define ESC_CHAR " ESC "
#define INVALID_CHAR "ERROR"


#define false 0
#define true 1



#define ACTIVE_BLOCK -2 // �����ǹ迭�� ����� ����� ���µ� 
#define CEILLING -1     // ����� �̵��� �� �ִ� ������ 0 �Ǵ� ���� ������ ǥ�� 
#define EMPTY 0         // ����� �̵��� �� ���� ������ ����� ǥ�� 
#define WALL 1
#define INACTIVE_BLOCK 2 // �̵��� �Ϸ�� ��ϰ� 


#define XY_MAX 50
#define STATUS_X_ADJ(X_ADJ,X) X_ADJ+X+1 //��������ǥ�� ��ġ���� 

typedef enum { GAME_START = 0, KEY_SETTING, EXIT }TITLE_MENU;
typedef enum { LEFT_KEY = 0, RIGHT_KEY, DOWN_KEY, HARD_DROP_KEY, ROTATE_KEY, ROTATE_COUNTER_KEY, HOLD_KEY, BACK_KEY } KEY_TYPE;


/*
int STATUS_Y_GOAL; //GOAL ����ǥ����ġY ��ǥ ����
int STATUS_Y_LEVEL; //LEVEL ����ǥ����ġY ��ǥ ����
int STATUS_Y_SCORE; //SCORE ����ǥ����ġY ��ǥ ����
*/

int left_key = 128 + LEFT;
int right_key = 128 + RIGHT;
int down_key = 128 + DOWN;
int hard_drop_key = ' ';
int rotate_key = 'X';
int rotate_counter_key = 'Z';
int hold_key = 'C';
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


int b_type[7] = { 0,1,2,3,4,5,6 }; // ��Ʈ���� ���� ����
int b_rotation; // ��� ȸ���� ���� 
int b_now; // ���� �ε��� ���� 
int bx, by; // �̵����� ����� �����ǻ��� x,y��ǥ�� ���� 


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

void setting_scene(void);

void game_scene(void);


void reset_org(void); //main_org[][]�� �ʱ�ȭ
void reset_cpy(void); // ������(main_cpy[][]�� �ʱ�ȭ)

//�ܼ�â draw/erase �Լ�
void draw_title_scene(int x, int y, TITLE_MENU menu);
void draw_setting_scene(int x, int y, KEY_TYPE type);
void draw_game_scene(void);
void draw_map(int x, int y); // (x,y)�� �������� ���� �׸�.
void draw_interface(int x, int y); // (x,y)�� �������� �⺻�������̽� �׸���
void erase_scene(int x, int y, int dx, int dy);
void draw_block(int x, int y, int b_type, int b_rotation);

char* key_string_set(int key);
char* key_set(int key);
// ��Ʈ�� Ÿ������ ��ȯ�ϴ� �Լ�

void setting_set(int x, int y, KEY_TYPE type);
void setting_key_setting(int x, int y, KEY_TYPE type);
void title_set(int x, int y, TITLE_MENU menu);

void setting_scene_set(int x, int y, KEY_TYPE type);

int new_block();// ���ο� ����� key ��������
int check_key(void); // Ű����� Ű �޾ƿ���
int check_is_upper(int key); // Ű�����  �޾ƿ� Ű �빮�� �˻�
int check_is_rotatable(int* x, int* y); // ȸ�� ���� �ϴٸ� ����� x,y ���� �������� ����� ��ǥ�� �������� ��Ʈ���� ��ġ

unsigned main_theme(void* arg);
int bpm = 200; // 16�� ��ǥ bpm
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

        Beep(1318.5100f, 6 * bpm);
        Beep(1046.5020f, 6 * bpm);
        Beep(1174.6590f, 6 * bpm);
        Beep(987.7666f, 6 * bpm);
        Beep(1046.5020f, 6 * bpm);
        Beep(880.0000f, 6 * bpm);
        Beep(830.6100f, 6 * bpm);
        Beep(987.7666f, 6 * bpm);
        Beep(1318.5100f, 6 * bpm);
        Beep(1046.5020f, 6 * bpm);
        Beep(1174.6590f, 6 * bpm);
        Beep(987.7666f, 6 * bpm);
        Beep(1046.5020f, 4 * bpm);
        Beep(1318.5100f, 4 * bpm);
        Beep(1760.0000f, 4 * bpm);
        Beep(1760.0000f, 4 * bpm);
        Beep(1661.2200f, 6 * bpm);


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
    //CloseHandle(hThrd);
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


void setting_scene() {
    int x = 5; //Ÿ��Ʋȭ���� ǥ�õǴ� x��ǥ
    int y = 3; //Ÿ��Ʋȭ���� ǥ�õǴ� y��ǥ
    KEY_TYPE key = LEFT_KEY;
    gotoxy(x, y + 0); printf("�� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��");
    gotoxy(x, y + 1); printf("��                                     ��");
    gotoxy(x, y + 2); printf("��      +-----------------------+      ��");
    gotoxy(x, y + 3); printf("��      |     S E T T I N G     |      ��");
    gotoxy(x, y + 4); printf("��      +-----------------------+      ��");
    gotoxy(x, y + 5); printf("��                                     ��");
    gotoxy(x, y + 15); printf("��                                     ��");
    gotoxy(x, y + 16); printf("�� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��");
    setting_key_setting(x, y, key);


}

void setting_new_key(int x, int y, int* key_set, KEY_TYPE type) {
    while (1) {
        if (_kbhit()) {
            key = _getch();
            
            if (key == 224) {
                do { key = _getch(); } while (key == 224);//����Ű���ð��� ����
                key_set = 180 + DOWN;
                setting_key_setting(x, y, type);
            }

            while (_kbhit()) _getch();
        }
    }
    //gotoxy(x+14, y); printf("S");
    //gotoxy(x+13, y); printf("ERROR");
}

void setting_key_setting(int x, int y, KEY_TYPE type) {
    setting_set(x, y, type);
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
                    setting_set(x, y, type);
                    break;
                case UP: //���� ����Ű ��������
                    type = (type + 6) % 7;
                    setting_set(x, y, type);
                }
            }
            while (_kbhit()) _getch();
        }
    }
    switch (type) {
    case LEFT_KEY:
        gotoxy(x, y + 6); printf("��  %s    LEFT     :     %6s  %s  ��", " ->", key_set(left_key), " <-");
        setting_new_key(x, y, left_key, type);
        break;
    case RIGHT_KEY:
        gotoxy(x, y + 7); printf("��  %s   RIGHT     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(right_key), CURSOR_ON_RIGHT);
        break;
    case DOWN_KEY:
        gotoxy(x, y + 8); printf("��  %s    DOWN     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(down_key), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_KEY:
        gotoxy(x, y + 9); printf("��  %s HARD DROP   :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(hard_drop_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_KEY:
        gotoxy(x, y + 10); printf("��  %s    ȸ��     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(rotate_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_KEY:
        gotoxy(x, y + 11); printf("��  %s�ݽð� ȸ��  :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(rotate_counter_key), CURSOR_ON_RIGHT);
        break;
    case HOLD_KEY:
        gotoxy(x, y + 12); printf("��  %s    HOLD     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(hold_key), CURSOR_ON_RIGHT);
        break;


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

void setting_set(int x, int y, KEY_TYPE type) {
    gotoxy(x, y + 6); printf("��  %s    LEFT     :     %6s  %s  ��", CURSOR_OFF, key_set(left_key), CURSOR_OFF);
    gotoxy(x, y + 7); printf("��  %s   RIGHT     :     %6s  %s  ��", CURSOR_OFF, key_set(right_key), CURSOR_OFF);
    gotoxy(x, y + 8); printf("��  %s    DOWN     :     %6s  %s  ��", CURSOR_OFF, key_set(down_key), CURSOR_OFF);
    gotoxy(x, y + 9); printf("��  %s HARD DROP   :     %6s  %s  ��", CURSOR_OFF, key_set(hard_drop_key), CURSOR_OFF);
    gotoxy(x, y + 10); printf("��  %s    ȸ��     :     %6s  %s  ��", CURSOR_OFF, key_set(rotate_key), CURSOR_OFF);
    gotoxy(x, y + 11); printf("��  %s�ݽð� ȸ��  :     %6s  %s  ��", CURSOR_OFF, key_set(rotate_counter_key), CURSOR_OFF);
    gotoxy(x, y + 12); printf("��  %s    HOLD     :     %6s  %s  ��", CURSOR_OFF, key_set(hold_key), CURSOR_OFF);
    gotoxy(x, y + 13); printf("��                                     ��");
    gotoxy(x, y + 14); printf("��      PRESS ESC TO BACK TO MAIN      ��");


    switch (type) {
    case LEFT_KEY:
        gotoxy(x, y + 6); printf("��  %s    LEFT     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(left_key), CURSOR_ON_RIGHT);
        break;
    case RIGHT_KEY:
        gotoxy(x, y + 7); printf("��  %s   RIGHT     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(right_key), CURSOR_ON_RIGHT);
        break;
    case DOWN_KEY:
        gotoxy(x, y + 8); printf("��  %s    DOWN     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(down_key), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_KEY:
        gotoxy(x, y + 9); printf("��  %s HARD DROP   :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(hard_drop_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_KEY:
        gotoxy(x, y + 10); printf("��  %s    ȸ��     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(rotate_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_KEY:
        gotoxy(x, y + 11); printf("��  %s�ݽð� ȸ��  :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(rotate_counter_key), CURSOR_ON_RIGHT);
        break;
    case HOLD_KEY:
        gotoxy(x, y + 12); printf("��  %s    HOLD     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(hold_key), CURSOR_ON_RIGHT);
        break;

    }

}
