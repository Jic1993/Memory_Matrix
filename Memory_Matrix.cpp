#include <graphics.h>
#include <conio.h>
#include <cstdio>
#include <cstdlib>		/* srand, rand */
#include <ctime>		/* time */

#define TILE_LENGTH 36	// ÿ��С���ӵı߳�
#define MAX_LEVEL   15	// �������������
#define CHANCES     15	// һ����Ϸ���ܴ���

struct Board {
	int level; // ��ǰ��ϷLevel(���������Ľ���)
	int up, down, left, right;			// �ֱ�����������������ĸ���߿������
	int tile[MAX_LEVEL][MAX_LEVEL];		// ����״̬����Ҫ������ı��Ϊ1������Ҫ�ı��Ϊ0
	int rand_x[MAX_LEVEL], rand_y[MAX_LEVEL]; // ��ʾ������ɵļ�����ӵ�λ��
} board;

// ��������
int  judge();		// �ж���Ҽ����Ƿ���ȷ������ȷ����1�����򷵻�0
int  run_game();	// ����һ��level�׵ļ��������Ϸ������Ϸ�ɹ�����1��ʧ�ܷ���0
void run_memory_matrix();	// ���Ƽ��������Ϸ������

// ������
int main(){
	run_memory_matrix();
	return 1;
}



// ���Ƽ��������Ϸ������
void run_memory_matrix(){

	initgraph(800, 600);	// ��ʼ��һ��800x600��ͼ�ν���
	setcolor(LIGHTBLUE);
	settextstyle(40, 0, _T("Arial"));
	outtextxy(180, 300, _T("Welcome to Memory Matrix!"));
	Sleep(700);
	
	int chances;		// ��ʾ��Ϸ�Ļ�����
	board.level = 3;	// level��Ӧ�ھ���Ľ��������� level=3 ��ʾ��ǰ��Ϸ�ļ������Ϊ��СΪ 3*3.
	// һ����Ϸ�и�ÿλ���CHANCES�λ��ᡣ����ɹ�һ��,��Ϸ�ȼ�(���������)����,����,�½�
	for (chances = 1; chances <= CHANCES; chances++){
		Sleep(500);
		cleardevice();	// ÿ����Ϸǰ����
		if (run_game()){
			++board.level;
			cleardevice();
			
			settextstyle(40, 0, _T("Arial"));
			outtextxy(250, 300, _T("Success! Level up!"));
		}
		else{
			board.level = board.level>3?board.level-1:3;
			cleardevice();
			settextstyle(40, 0, _T("Arial"));
			outtextxy(250, 300, _T("Fail! Level down!"));
		}
	}

	cleardevice();
	Sleep(500);
	settextstyle(40, 0, _T("Arial"));
	outtextxy(250, 300, _T("Game Over!"));       //15����Ϸ��������Ϸ������
	settextstyle(20, 0, _T("Arial"));
	setcolor(WHITE);
	outtextxy(230, 400, _T("Press any key to exit..."));
	_getch();         //�ȴ��û�����
	closegraph();
}

//����һ��level�׵ļ��������Ϸ������Ϸ�ɹ�����1��ʧ�ܷ���0
int run_game(){
	//���ø��ӳ�ʼ״̬Ϊ0
	memset(board.tile, 0, sizeof(board.tile));
	//���������Ҫ����ĸ��ӣ������Ϊ1
	for(int i = 0; i < board.level; ++i){
		srand(time(NULL)+i);
		board.rand_x[i] = rand() % board.level;
		board.rand_y[i] = rand() % board.level;
		if (board.tile[board.rand_x[i]][board.rand_y[i]] == 1){ i--; continue; }
		board.tile[board.rand_x[i]][board.rand_y[i]] = 1;
	}
	// ������߿������
	board.left  = 400 - board.level * TILE_LENGTH / 2;
	board.right = 400 + board.level * TILE_LENGTH / 2;
	board.up    = 300 - board.level * TILE_LENGTH / 2;
	board.down  = 300 + board.level * TILE_LENGTH / 2;
	//������
	setlinestyle(PS_SOLID, 1);
	setcolor(LIGHTBLUE);
	for (int i = board.left; i <= board.right; i += TILE_LENGTH) line(i, board.up, i, board.down);
	for (int i = board.up;   i <= board.down;  i += TILE_LENGTH) line(board.left, i, board.right, i);
    // ������
	setlinestyle(PS_SOLID, 10);
	setcolor(LIGHTBLUE);
	line(board.left  - 5, board.up   - 5, board.right + 5, board.up   - 5);
	line(board.left  - 5, board.down + 5, board.right + 5, board.down + 5);
	line(board.left  - 5, board.up   - 5, board.left  - 5, board.down + 5);
	line(board.right + 5, board.up   - 5, board.right + 5, board.down + 5);

	Sleep(100);
	// �����Ҫ�������
	for(int i = 0; i < board.level; ++i)
	{
		setfillcolor(WHITE);
		bar(board.left + TILE_LENGTH * board.rand_x[i] + 1, board.up + TILE_LENGTH * board.rand_y[i] + 1,
		board.left + TILE_LENGTH * (board.rand_x[i]+1) - 1, board.up + TILE_LENGTH * (board.rand_y[i]+1)- 1);
	}
	// �ȴ�2s��������
	Sleep(2000);
	for(int i = 0; i < board.level; i++)
	{
		setfillcolor(BLACK);
		bar(board.left + TILE_LENGTH * board.rand_x[i] + 1, board.up + TILE_LENGTH * board.rand_y[i] + 1,
		board.left + TILE_LENGTH * (board.rand_x[i]+1) - 1, board.up + TILE_LENGTH * (board.rand_y[i]+1)- 1);
	}
	// �ж���Ҽ����Ƿ���ȷ
	return judge();
}

// �ж���Ҽ����Ƿ���ȷ, �����أ� 1--�ɹ�; 0--ʧ��
int judge() {
	MOUSEMSG m;	// ��������������Լ�����λ��
	int count = 0, tmpx, tmpy;
	while(1){
		m = GetMouseMsg();
		if(m.uMsg==WM_LBUTTONDOWN){
			if(board.left<m.x&&m.x<board.right && board.up<m.y&&m.y<board.down){
				tmpx = (m.x - board.left) / TILE_LENGTH;
				tmpy = (m.y - board.up  ) / TILE_LENGTH;
				if(board.tile[tmpx][tmpy]==0)	// ����������ĸ��Ӳ���"�������"����Ϸʧ�ܣ�����0
					return 0;
				else if(board.tile[tmpx][tmpy]==1){ // ���������������ȷ����Ǹø��ӣ�����count++,ֱ��(count==level)��Ϸ�ɹ�����1
					setfillcolor(WHITE);
					bar(board.left + TILE_LENGTH * tmpx + 1, board.up + TILE_LENGTH * tmpy + 1,
						board.left + TILE_LENGTH * (tmpx + 1) - 1, board.up + TILE_LENGTH * (tmpy + 1) - 1);
					board.tile[tmpx][tmpy] = -1;
					if(++count>=board.level){Sleep(500);return 1;}
				}
			}
		}
	}
}

