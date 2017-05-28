#include <graphics.h>
#include <conio.h>
#include <cstdio>
#include <cstdlib>	/* srand, rand */
#include <ctime>	/* time */

#define TILE_LENGTH 36	// 每个小格子的边长
#define MAX_LEVEL   15	// 记忆矩阵最大阶数
#define CHANCES     15	// 一轮游戏的总次数

struct Board{
	int level;	// 当前游戏Level(即记忆矩阵的阶数)
	int up, down, left, right;		// 分别代表矩阵的上下左右四个外边框的坐标
	int tile[MAX_LEVEL][MAX_LEVEL];	// 格子状态，需要被记忆的标记为1，不需要的标记为0
	int rand_x[MAX_LEVEL], rand_y[MAX_LEVEL]; // 表示随机生成的记忆格子的位子
} board;

// 函数申明
int  judge();		// 判断玩家记忆是否正确。如正确返回1；否则返回0
int  run_game();	// 运行一次level阶的记忆矩阵游戏，如游戏成功返回1；失败返回0
void run_memory_matrix();	// 控制记忆矩阵游戏的流程

// 主程序
int main(){
	run_memory_matrix();
	return 1;
}



// 控制记忆矩阵游戏的流程
void run_memory_matrix(){

	initgraph(800, 600);	// 初始化一个800x600的图形界面
	setcolor(LIGHTBLUE);
	settextstyle(40, 0, _T("Arial"));
	outtextxy(180, 300, _T("Welcome to Memory Matrix!"));
	Sleep(700);
	
	board.level = 3;	// level对应于矩阵的阶数，比如 level=3 表示当前游戏的记忆矩阵为大小为 3*3.
	// 一轮游戏中给每位玩家CHANCES次机会。如果成功一次,游戏等级(即矩阵阶数)上升,否则,下降
	for(int i = 0; i < CHANCES; ++i){
		Sleep(500);
		cleardevice();	// 每次游戏前清屏
		if (run_game()){
			++board.level;
			cleardevice();
			settextstyle(40, 0, _T("Arial"));
			outtextxy(250, 300, _T("Success! Level up!"));
		}else{
			board.level = board.level>3?board.level-1:3;
			cleardevice();
			settextstyle(40, 0, _T("Arial"));
			outtextxy(250, 300, _T("Fail! Level down!"));
		}
	}

	// CHANCES次游戏后，整个游戏结束
	cleardevice();
	Sleep(500);
	settextstyle(40, 0, _T("Arial"));
	outtextxy(270, 300, _T("Game Over!"));
	settextstyle(20, 0, _T("Arial"));
	setcolor(WHITE);
	outtextxy(280, 400, _T("Press any key to exit..."));
	_getch();	// 等待用户按键
	closegraph();
}

// 运行一次level阶的记忆矩阵游戏，如游戏成功返回1；失败返回0
int run_game(){
	// 设置格子初始状态为0
	memset(board.tile, 0, sizeof(board.tile));
	// 矩阵外边框的坐标
	board.left  = 400 - board.level * TILE_LENGTH / 2;
	board.right = 400 + board.level * TILE_LENGTH / 2;
	board.up    = 300 - board.level * TILE_LENGTH / 2;
	board.down  = 300 + board.level * TILE_LENGTH / 2;
	// 画格子
	setlinestyle(PS_SOLID, 1);
	setcolor(LIGHTBLUE);
	for (int i = board.left; i <= board.right; i += TILE_LENGTH) line(i, board.up, i, board.down);
	for (int i = board.up;   i <= board.down;  i += TILE_LENGTH) line(board.left, i, board.right, i);
	// 画粗体外边框
	setlinestyle(PS_SOLID, 10);
	setcolor(LIGHTBLUE);
	line(board.left  - 5, board.up   - 5, board.right + 5, board.up   - 5);
	line(board.left  - 5, board.down + 5, board.right + 5, board.down + 5);
	line(board.left  - 5, board.up   - 5, board.left  - 5, board.down + 5);
	line(board.right + 5, board.up   - 5, board.right + 5, board.down + 5);

	Sleep(100);
	// 随机生成需要记忆的格子，并标记为白色
	srand(time(NULL));
	for (int i = 0; i < board.level; ++i){
		board.rand_x[i] = rand() % board.level;
		board.rand_y[i] = rand() % board.level;
		if (board.tile[board.rand_x[i]][board.rand_y[i]] == 1){ i--; continue; }
		board.tile[board.rand_x[i]][board.rand_y[i]] = 1;
		setfillcolor(WHITE);
		bar(board.left + TILE_LENGTH * board.rand_x[i] + 1, board.up + TILE_LENGTH * board.rand_y[i] + 1,
			board.left + TILE_LENGTH * (board.rand_x[i] + 1) - 1, board.up + TILE_LENGTH * (board.rand_y[i] + 1) - 1);
		Sleep(100);
	}
	// 等待2s后清除标记
	Sleep(2000);
	for(int i = 0; i < board.level; i++)
	{
		setfillcolor(BLACK);
		bar(board.left + TILE_LENGTH * board.rand_x[i] + 1, board.up + TILE_LENGTH * board.rand_y[i] + 1,
		board.left + TILE_LENGTH * (board.rand_x[i]+1) - 1, board.up + TILE_LENGTH * (board.rand_y[i]+1)- 1);
	}
	// 判断玩家记忆是否正确
	return judge();
}

// 判断玩家记忆是否正确, 并返回： 1--成功; 0--失败
int judge(){
	MOUSEMSG m;	// 接收鼠标的左键，以及坐标位子
	int count = 0, tmpx, tmpy;
	while(1){
		m = GetMouseMsg();
		if(m.uMsg==WM_LBUTTONDOWN){
			if(board.left<m.x&&m.x<board.right && board.up<m.y&&m.y<board.down){
				tmpx = (m.x - board.left) / TILE_LENGTH;
				tmpy = (m.y - board.up  ) / TILE_LENGTH;
				if(board.tile[tmpx][tmpy]==0)	// 如果鼠标所点的格子不是"记忆格子"，游戏失败，返回0
					return 0;
				else if(board.tile[tmpx][tmpy]==1){	// 如果鼠标所点格子正确，标记该格子，并且count++,直到(count==level)游戏成功返回1
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

