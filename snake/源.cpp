#include<stdio.h>
#include<stdlib.h> 
#include<graphics.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<iostream>
#include<fstream>
using namespace std;

#pragma warning(disable:4996)
#pragma comment(lib,"Winmm.lib")
int dir;			//控制方向 左4右6上8下2 
int fra[30][20];
int X, Y;			//引用的随机变量 积分球的坐标 
int x, y;			//蛇的坐标
int sco;            //得分 
int maxsco;			//最佳得分
int l;				//蛇得长度 
int p, q;			//蛇身坐标
int End;			//检测是否吃到身体
IMAGE img_beijing;	//背景
IMAGE img_she;		//蛇
IMAGE img_shep;		//蛇身
IMAGE img_sco;		//得分球
IMAGE img_over;		//结束画面
IMAGE img_start;	//开始画面
IMAGE img_load;
void startup()
{
	int i, j;
	l = 3;
	End = 0;
	for (i = 0; i < 30; i++){
		for (j = 0; j < 20; j++)
			fra[i][j] = 0;
	}
	fra[10][10] = l;
	fra[9][10] = l - 1;
	fra[8][10] = l - 2;
	X = 400; Y = 200;
	x = 200; y = 200;
	dir = 6;
	initgraph(600, 400);
	loadimage(&img_load, "..\\res\\loading.png");
	putimage(0, 0, &img_load);
	mciSendString("open ..\\music\\back.mp3 alias bkmusic", NULL, 0, NULL);
	mciSendString("play bkmusic repeat", NULL, 0, NULL);
	loadimage(&img_beijing, "..\\res\\beijing.png");
	loadimage(&img_she, "..\\res\\she.png");
	loadimage(&img_sco, "..\\res\\sco.png");
	loadimage(&img_shep, "..\\res\\shep.png");
	loadimage(&img_over, "..\\res\\gameover.png");
	loadimage(&img_start, "..\\res\\start.png");
	putimage(0, 0, &img_start);
	system("pause");
	BeginBatchDraw();
}

void outscomax()
{
	ofstream fout;
	fout.open("..\\snake\\maxsco.txt");
	fout << maxsco << endl;
	fout.close();
}

void inscomax()
{
	ifstream fin;
	fin.open("..\\snake\\maxsco.txt");
	fin >> maxsco;
	fin.close();
}

void show()
{
	int i, j;
	putimage(0, 0, &img_beijing);
	putimage(X, Y, &img_sco);
	putimage(x, y, &img_she);
	for (i = 0; i < 30; i++){
		for (j = 0; j < 20; j++){
			if (fra[i][j]>0 && fra[i][j]<l){
				putimage(i * 20, j * 20, &img_shep);
			}
		}
	}
	if (x<0 || y<0 || x>600 || y>400 || End == 1){
		putimage(0, 0, &img_over);
		if (sco>maxsco){
			maxsco = sco;
			outscomax();
		}
		char str1[100]; char str2[100];
		sco = l - 3;
		sprintf_s(str1, "%d", sco);
		sprintf_s(str2, "%d", maxsco);
			outtextxy(260, 185, "您的得分为："); outtextxy(360, 185, str1 );
			outtextxy(260, 210, "历史最佳为："); outtextxy(360, 210, str2 );
	}
	FlushBatchDraw();
}

void updateWithoutInput()
{
	int m, n, i, j, a, b, max = 0;
	srand((unsigned)time(NULL));//调用time函数来获取随机数
	Sleep(300);
	if (x == X&&y == Y){
		m = rand() % 30;
		n = rand() % 20;
		X = m * 20;
		Y = n * 20;
		l++;
		for (i = 0; i < 30; i++){
			for (j = 0; j<20; j++)
			if (fra[i][j]>0)
				fra[i][j] += 1;
		}
	}
	for (i = 0; i<30; i++){
		for (j = 0; j<20; j++)
		if (fra[i][j]>0){
			fra[i][j] -= 1;
		}
	}
	for (i = 0; i < 30; i++){
		for (j = 0; j < 20; j++){
			if (fra[i][j] >= max){
				max = fra[i][j];
				a = i; b = j;
			}
		}
	}
	if (dir == 6){
		x += 20;
		if (fra[a + 1][b]>0)
			End = 1;
		fra[a + 1][b] = fra[a][b] + 1;
	}
	else if (dir == 4){
		x -= 20;
		if (fra[a - 1][b]>0)
			End = 1;
		fra[a - 1][b] = fra[a][b] + 1;
	}
	else if (dir == 8){
		y -= 20;
		if (fra[a][b - 1]>0)
			End = 1;
		fra[a][b - 1] = fra[a][b] + 1;
	}
	else if (dir == 2){
		y += 20;
		if (fra[a][b + 1]>0)
			End = 1;
		fra[a][b + 1] = fra[a][b] + 1;
	}
}

void updateWithInput()
{
	char input;
	if (kbhit())							//检测用户输入内容（不需要回车） 
	{
		input = getch();
		if (input == 'w'&&dir != 2){
			dir = 8;
		}
		if (input == 's'&&dir != 8){
			dir = 2;
		}
		if (input == 'a'&&dir != 6){
			dir = 4;
		}
		if (input == 'd'&&dir != 4){
			dir = 6;
		}
	}
}

int main()
{
	startup();
	inscomax();
	while (1)
	{
		show();
		updateWithoutInput();
		updateWithInput();
	}
	return(0);
}