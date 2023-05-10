#define _CRT_SECURE_NO_WARNINGS //scanf, strcpy ���� ����

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include <string.h>


/// Ű���� ����
#define LEFT 75 //���� ȭ��ǥ
#define RIGHT 77 //���� ȭ��ǥ
#define UP 72 //���� ȭ��ǥ
#define DOWN 80 //�Ʒ��� ȭ��ǥ
#define SPACE 32 //�����̽�
#define ESC 27 //�̽�������

#define CURSOR_OFF "   "
#define CURSOR_ON_LEFT ">> "
#define CURSOR_ON_RIGHT " <<"

#define LEFT_CHAR "   ��  "
#define UP_CHAR "   ��  "
#define RIGHT_CHAR "   ��  "
#define DOWN_CHAR "   ��  "
#define SPACE_CHAR "SPACE"
#define ENTER_CHAR "ENTER"
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
//typedef enum { LEFT_MENU = 0, RIGHT_MENU, DOWN_MENU, HARD_DROP_MENU, ROTATE_MENU, ROTATE_COUNTER_MENU, HOLD_MENU, PAUSE_MENU, BACK_MENU, MAIN_MENU }SETTING_MENU;
typedef enum { LEFT_KEY = 0, RIGHT_KEY, DOWN_KEY, HARD_DROP_KEY, ROTATE_KEY, ROTATE_COUNTER_KEY, HOLD_KEY, BACK_KEY } KEY_TYPE;
/*
int STATUS_Y_GOAL; //GOAL ����ǥ����ġY ��ǥ ����
int STATUS_Y_LEVEL; //LEVEL ����ǥ����ġY ��ǥ ����
int STATUS_Y_SCORE; //SCORE ����ǥ����ġY ��ǥ ����
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


int b_type[7] = { 0,1,2,3,4,5,6 }; //��Ʈ���� ���� ����
int b_rotation; //��� ȸ���� ���� 
int b_now; // ���� �ε��� ���� 
int bx, by; //�̵����� ����� �����ǻ��� x,y��ǥ�� ���� 

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
void game_scene(void);

void reset_org(void); //main_org[][]�� �ʱ�ȭ
void reset_cpy(void); // ������(main_cpy[][]�� �ʱ�ȭ)

//�ܼ�â draw/erase �Լ�
void draw_title_scene(void);
void draw_game_scene(void);
void draw_map(int x, int y); // (x,y)�� �������� ���� �׸�.
void draw_interface(int x, int y); // (x,y)�� �������� �⺻�������̽� �׸���
void erase_scene(int x, int y, int dx, int dy);
void draw_block(int x, int y, int b_type, int b_rotation);



int new_block();// ���ο� ����� key ��������
int check_key(void); // Ű����� Ű �޾ƿ���
int check_is_upper(int key); // Ű�����  �޾ƿ� Ű �빮�� �˻�
int check_is_rotatable(int* x, int* y); // ȸ�� ���� �ϴٸ� ����� x,y ���� �������� 





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

#define ACTIVE_BLOCK -2 // �����ǹ迭�� ����� ����� ���µ�
#define CEILLING -1     // ����� �̵��� �� �ִ� ������ 0 �Ǵ� ���� ������ ǥ��
#define EMPTY 0         // ����� �̵��� �� ���� ������ ����� ǥ��
#define WALL 1
#define INACTIVE_BLOCK 2 // �̵��� �Ϸ�� ��ϰ�

#define MAIN_X 11 //������ ����ũ��
#define MAIN_Y 23 //������ ����ũ��
#define MAIN_X_ADJ 3 //������ ��ġ����
#define MAIN_Y_ADJ 1 //������ ��ġ����

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 //��������ǥ�� ��ġ����

#define CURSOR_OFF "   "
#define CURSOR_ON_LEFT ">> "
#define CURSOR_ON_RIGHT " <<"

#define LEFT_CHAR "   ��  "
#define UP_CHAR "   ��  "
#define RIGHT_CHAR "   ��  "
#define DOWN_CHAR "   ��  "
#define SPACE_CHAR "SPACE"
#define ENTER_CHAR "ENTER"
#define INVALID_CHAR "ERROR"
#define ESC_CHAR " ESC "




#define ENTER 13 ///'\r'
#define LEFT 75 //�·� �̵�    //Ű���尪��
#define RIGHT 77 //��� �̵�
#define UP 72 //ȸ��
#define DOWN 80 //soft drop
#define ESC 27

typedef enum { GAME_START = 0, KEY_SETTING, EXIT }TITLE_MENU;
typedef enum { LEFT_MENU = 0, RIGHT_MENU, DOWN_MENU, HARD_DROP_MENU, ROTATE_MENU, ROTATE_COUNTER_MENU, HOLD_MENU, PAUSE_MENU, BACK_MENU, MAIN_MENU }SETTING_MENU;

//Ű���� ���� _ ���氡��
int left_key[2] = { 224,LEFT };
int right_key[2] = { 224, RIGHT };
int down_key[2] = { 224, DOWN };
int hard_drop_key[2] = { ' ', };
int rotate_key[2] = { 'X', };
int rotate_counter_key[2] = { 'Z', };
int hold_key[2] = { 'C', };
int pause_key[2] = { 'P', };
int esc_key[2] = { ESC, };

int STATUS_Y_GOAL; //GOAL ����ǥ����ġY ��ǥ ����
int STATUS_Y_LEVEL; //LEVEL ����ǥ����ġY ��ǥ ����
int STATUS_Y_SCORE; //SCORE ����ǥ����ġY ��ǥ ����

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
}; //��ϸ�� ���� 4*4������ ����� ǥ�� blcoks[b_type][b_rotation][i][j]�� ���

int b_type; //��� ������ ����
int b_rotation; //��� ȸ���� ����
int b_type_next; //���� ��ϰ� ����

int main_org[MAIN_Y][MAIN_X]; //�������� ������ �����ϴ� �迭 ����Ϳ� ǥ���Ŀ� main_cpy�� �����
int main_cpy[MAIN_Y][MAIN_X]; //�� maincpy�� �������� ����Ϳ� ǥ�õǱ� ���� ������ ������ ����
//main�� ��ü�� ��� ����Ϳ� ǥ������ �ʰ�(�̷��� �ϸ� ����Ͱ� �����Ÿ�)
//main_cpy�� �迭�� ���ؼ� ���� �޶��� ���� ����Ϳ� ��ħ
int bx, by; //�̵����� ����� �����ǻ��� x,y��ǥ�� ����

int key; //Ű����� �Է¹��� Ű���� ����

int speed; //��������ӵ�
int level; //���� level
int level_goal; //���������� �Ѿ�� ���� ��ǥ����
int cnt; //���� �������� ������ �� ���� ����
int score; //���� ����
int last_score = 0; //��������������
int best_score = 0; //�ְ��������

int new_block_on = 0; //���ο� ���� �ʿ����� �˸��� flag
int crush_on = 0; //���� �̵����� ����� �浹�������� �˷��ִ� flag
int level_up_on = 0; //���������� ����(���� ������ǥ�� �Ϸ�Ǿ�����) �˸��� flag
int space_key_on = 0; //hard drop�������� �˷��ִ� flag

void title(void); //���ӽ���ȭ��
void title_set(int x, int y, TITLE_MENU menu);
void setting(void);
void reset(void); //������ �ʱ�ȭ
void reset_main(void); //���� ������(main_org[][]�� �ʱ�ȭ)
void reset_main_cpy(void); //copy ������(main_cpy[][]�� �ʱ�ȭ)
void draw_map(void); //���� ��ü �������̽��� ǥ��
void draw_main(void); //�������� �׸�
void new_block(void); //���ο� ����� �ϳ� ����
void check_key(void); //Ű����� Ű�� �Է¹���
void drop_block(void); //����� �Ʒ��� ����Ʈ��
int check_crush(int bx, int by, int rotation); //bx, by��ġ�� rotationȸ������ ���� ��� �浹 �Ǵ�
void move_block(int dir); //dir�������� ����� ������
void check_line(void); //���� ����á������ �Ǵ��ϰ� ����
void check_level_up(void); //������ǥ�� �޼��Ǿ������� �Ǵ��ϰ� levelup��Ŵ
void check_game_over(void); //���ӿ������� �Ǵ��ϰ� ���ӿ����� ����
void pause(void);//������ �Ͻ�������Ŵ
void key_upper_check(char* key, char input);
char* key_string_set(char key);
void setting_set(int x, int y, SETTING_MENU menu);
void setting_key_setting(int x, int y, SETTING_MENU menu);
char* key_set(int* key);


void gotoxy(int x, int y) { //gotoxy�Լ�
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR = 0, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //Ŀ������� �Լ��� ���Ǵ� ������

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
    int i;

    srand((unsigned)time(NULL)); //����ǥ����
    setcursortype(NOCURSOR); //Ŀ�� ����
    title(); //����Ÿ��Ʋ ȣ��
}

void key_upper_check(char* key, char input) {
    if ('A' < input < 'Z') {
        input = input - 'A' + 'a';
    }
    *key = input;
}

void title(void) {
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
    gotoxy(x, y + 6); printf("> Press Enter <");
    title_set(x, y, menu);

    for (cnt = 0;; cnt++) { //cnt�� 1�� ������Ű�鼭 ��� �ݺ�    //�ϳ��� ���߿��� �� ��¦�̴� �ִϸ��̼�ȿ��
        if (_kbhit()) {
            key = _getch();
            if (key == ENTER) break;
            if (key == 224) {
                do { key = _getch(); } while (key == 224);//����Ű���ð��� ����
                switch (key) {
                case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó����
                    menu = (menu + 1) % 3;
                    title_set(x, y, menu);
                    break;
                case UP: //���� ����Ű ��������
                    menu = (menu + 2) % 3;
                    title_set(x, y, menu);
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
    int x = 5; //Ÿ��Ʋȭ���� ǥ�õǴ� x��ǥ
    int y = 3; //Ÿ��Ʋȭ���� ǥ�õǴ� y��ǥ
    SETTING_MENU menu = LEFT_MENU;
    gotoxy(x, y + 0); printf("�� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��");
    gotoxy(x, y + 1); printf("��                                     ��");
    gotoxy(x, y + 2); printf("��      +-----------------------+      ��");
    gotoxy(x, y + 3); printf("��      |     S E T T I N G     |      ��");
    gotoxy(x, y + 4); printf("��      +-----------------------+      ��");
    gotoxy(x, y + 5); printf("��                                     ��");
    gotoxy(x, y + 17); printf("��                                     ��");
    gotoxy(x, y + 18); printf("�� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��");
    setting_key_setting(x, y, menu);


}

void setting_new_key(int x, int y, int* key_set, SETTING_MENU menu) {
    while (1) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) {
                do { key = _getch(); } while (key == 224);//����Ű���ð��� ����
                switch (key) {
                case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó����
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
                do { key = _getch(); } while (key == 224);//����Ű���ð��� ����
                switch (key) {
                case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó����
                    menu = (menu + 1) % 10;
                    setting_set(x, y, menu);
                    break;
                case UP: //���� ����Ű ��������
                    menu = (menu + 9) % 10;
                    setting_set(x, y, menu);
                }
            }
            while (_kbhit()) _getch();
        }
    }
    switch (menu) {
    case LEFT_MENU:
        gotoxy(x, y + 6); printf("��  %s    LEFT     :     %6s  %s  ��", " ->", key_set(left_key), " <-");
        setting_new_key(x, y, left_key, menu);
        break;
    case RIGHT_MENU:
        gotoxy(x, y + 7); printf("��  %s   RIGHT     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(right_key), CURSOR_ON_RIGHT);
        break;
    case DOWN_MENU:
        gotoxy(x, y + 8); printf("��  %s    DOWN     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(down_key), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_MENU:
        gotoxy(x, y + 9); printf("��  %s HARD DROP   :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(hard_drop_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_MENU:
        gotoxy(x, y + 10); printf("��  %s    ȸ��     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(rotate_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_MENU:
        gotoxy(x, y + 11); printf("��  %s�ݽð� ȸ��  :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(rotate_counter_key), CURSOR_ON_RIGHT);
        break;
    case HOLD_MENU:
        gotoxy(x, y + 12); printf("��  %s    HOLD     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(hold_key), CURSOR_ON_RIGHT);
        break;
    case PAUSE_MENU:
        gotoxy(x, y + 13); printf("��  %s   PAUSE     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(pause_key), CURSOR_ON_RIGHT);
        break;
    case BACK_MENU:
        gotoxy(x, y + 14); printf("��  %s    BACK     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(esc_key), CURSOR_ON_RIGHT);
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
    gotoxy(x, y + 6); printf("��  %s    LEFT     :     %6s  %s  ��", CURSOR_OFF, key_set(left_key), CURSOR_OFF);
    gotoxy(x, y + 7); printf("��  %s   RIGHT     :     %6s  %s  ��", CURSOR_OFF, key_set(right_key), CURSOR_OFF);
    gotoxy(x, y + 8); printf("��  %s    DOWN     :     %6s  %s  ��", CURSOR_OFF, key_set(down_key), CURSOR_OFF);
    gotoxy(x, y + 9); printf("��  %s HARD DROP   :     %6s  %s  ��", CURSOR_OFF, key_set(hard_drop_key), CURSOR_OFF);
    gotoxy(x, y + 10); printf("��  %s    ȸ��     :     %6s  %s  ��", CURSOR_OFF, key_set(rotate_key), CURSOR_OFF);
    gotoxy(x, y + 11); printf("��  %s�ݽð� ȸ��  :     %6s  %s  ��", CURSOR_OFF, key_set(rotate_counter_key), CURSOR_OFF);
    gotoxy(x, y + 12); printf("��  %s    HOLD     :     %6s  %s  ��", CURSOR_OFF, key_set(hold_key), CURSOR_OFF);
    gotoxy(x, y + 13); printf("��  %s   PAUSE     :     %6s  %s  ��", CURSOR_OFF, key_set(pause_key), CURSOR_OFF);
    gotoxy(x, y + 14); printf("��  %s    BACK     :     %6s  %s  ��", CURSOR_OFF, key_set(esc_key), CURSOR_OFF);
    gotoxy(x, y + 15); printf("��                                     ��");
    gotoxy(x, y + 16); printf("��     %s    BACK TO MAIN     %s     ��", CURSOR_OFF, CURSOR_OFF);

    switch (menu) {
    case LEFT_MENU:
        gotoxy(x, y + 6); printf("��  %s    LEFT     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(left_key), CURSOR_ON_RIGHT);
        break;
    case RIGHT_MENU:
        gotoxy(x, y + 7); printf("��  %s   RIGHT     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(right_key), CURSOR_ON_RIGHT);
        break;
    case DOWN_MENU:
        gotoxy(x, y + 8); printf("��  %s    DOWN     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(down_key), CURSOR_ON_RIGHT);
        break;
    case HARD_DROP_MENU:
        gotoxy(x, y + 9); printf("��  %s HARD DROP   :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(hard_drop_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_MENU:
        gotoxy(x, y + 10); printf("��  %s    ȸ��     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(rotate_key), CURSOR_ON_RIGHT);
        break;
    case ROTATE_COUNTER_MENU:
        gotoxy(x, y + 11); printf("��  %s�ݽð� ȸ��  :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(rotate_counter_key), CURSOR_ON_RIGHT);
        break;
    case HOLD_MENU:
        gotoxy(x, y + 12); printf("��  %s    HOLD     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(hold_key), CURSOR_ON_RIGHT);
        break;
    case PAUSE_MENU:
        gotoxy(x, y + 13); printf("��  %s   PAUSE     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(pause_key), CURSOR_ON_RIGHT);
        break;
    case BACK_MENU:
        gotoxy(x, y + 14); printf("��  %s    BACK     :     %6s  %s  ��", CURSOR_ON_LEFT, key_set(esc_key), CURSOR_ON_RIGHT);
        break;
    case MAIN_MENU:
        gotoxy(x, y + 16); printf("��     %s    BACK TO MAIN     %s     ��", CURSOR_ON_LEFT, CURSOR_ON_RIGHT);
        break;
    }



}

*/