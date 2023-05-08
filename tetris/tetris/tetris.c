#define _CRT_SECURE_NO_WARNINGS //scanf, strcpy 오류 무시

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>


/// 키보드 값들
#define LEFT 75 //좌측 화살표
#define RIGHT 77 //우측 화살표
#define UP 72 //위쪽 화살표
#define DOWN 80 //아래쪽 화살표
#define SPACE 32 //스페이스
#define ESC 27 //이스케이프




#define false 0
#define true 1

#define ACTIVE_BLOCK -2 // 게임판배열에 저장될 블록의 상태들 
#define CEILLING -1     // 블록이 이동할 수 있는 공간은 0 또는 음의 정수료 표현 
#define EMPTY 0         // 블록이 이동할 수 없는 공간은 양수로 표현 
#define WALL 1
#define INACTIVE_BLOCK 2 // 이동이 완료된 블록값 


#define XY_MAX 50
#define STATUS_X_ADJ(X_ADJ,X) X_ADJ+X+1 //게임정보표시 위치조정 

/*
int STATUS_Y_GOAL; //GOAL 정보표시위치Y 좌표 저장 
int STATUS_Y_LEVEL; //LEVEL 정보표시위치Y 좌표 저장
int STATUS_Y_SCORE; //SCORE 정보표시위치Y 좌표 저장
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
}; //블록모양 저장 4*4공간에 블록을 표현 blcoks[b_type][b_rotation][i][j]로 사용 // 숫자가 커지면 시계방향 작아지면 반시계 방향


int b_type[7] = {0,1,2,3,4,5,6}; //블록 종류를 저장 
int b_rotation; //블록 회전값 저장 
int b_now; // 현재 인덱스 저장 
int bx, by; //이동중인 블록의 게임판상의 x,y좌표를 저장 

int main_org[XY_MAX][XY_MAX]; //게임판의 정보를 저장하는 배열 모니터에 표시후에 main_cpy로 복사됨 
int main_cpy[XY_MAX][XY_MAX]; //즉 maincpy는 게임판이 모니터에 표시되기 전의 정보를 가지고 있음 
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



void title(void); //게임시작화면 
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
