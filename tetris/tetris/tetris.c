#define _CRT_SECURE_NO_WARNINGS //scanf, strcpy ���� ����

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>


/// Ű���� ����
#define LEFT 75 //���� ȭ��ǥ
#define RIGHT 77 //���� ȭ��ǥ
#define UP 72 //���� ȭ��ǥ
#define DOWN 80 //�Ʒ��� ȭ��ǥ
#define SPACE 32 //�����̽�
#define ESC 27 //�̽�������




#define false 0
#define true 1

#define ACTIVE_BLOCK -2 // �����ǹ迭�� ����� ����� ���µ� 
#define CEILLING -1     // ����� �̵��� �� �ִ� ������ 0 �Ǵ� ���� ������ ǥ�� 
#define EMPTY 0         // ����� �̵��� �� ���� ������ ����� ǥ�� 
#define WALL 1
#define INACTIVE_BLOCK 2 // �̵��� �Ϸ�� ��ϰ� 


#define XY_MAX 50
#define STATUS_X_ADJ(X_ADJ,X) X_ADJ+X+1 //��������ǥ�� ��ġ���� 

/*
int STATUS_Y_GOAL; //GOAL ����ǥ����ġY ��ǥ ���� 
int STATUS_Y_LEVEL; //LEVEL ����ǥ����ġY ��ǥ ����
int STATUS_Y_SCORE; //SCORE ����ǥ����ġY ��ǥ ����
*/



int blocks[7][4][4][4] = {
{{0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},{0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},
 {0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},{0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0}},
{{0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},{0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0},
{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
{{1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0},{0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}},
{{0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0},{0,1,0,0,0,1,1,0,0,0,1,0,0,0,0,0},
 {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0}},
{{0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0},
 {0,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0},{1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0}},
{{1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},{0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0},
 {0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0},{0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0}},
{{0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0},{0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0},
 {0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0},{0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0}}
}; //��ϸ�� ���� 4*4������ ����� ǥ�� blcoks[b_type][b_rotation][i][j]�� ��� // ���ڰ� Ŀ���� �ð���� �۾����� �ݽð� ����


int b_type[7] = {0,1,2,3,4,5,6}; //��� ������ ���� 
int b_rotation; //��� ȸ���� ���� 
int b_now; // ���� �ε��� ���� 
int bx, by; //�̵����� ����� �����ǻ��� x,y��ǥ�� ���� 

int main_org[XY_MAX][XY_MAX]; //�������� ������ �����ϴ� �迭 ����Ϳ� ǥ���Ŀ� main_cpy�� ����� 
int main_cpy[XY_MAX][XY_MAX]; //�� maincpy�� �������� ����Ϳ� ǥ�õǱ� ���� ������ ������ ���� 
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



void title(void); //���ӽ���ȭ�� 
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
