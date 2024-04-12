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
int dir;			//���Ʒ��� ��4��6��8��2 
int fra[30][20];
int X, Y;			//���õ�������� ����������� 
int x, y;			//�ߵ�����
int sco;            //�÷� 
int maxsco;			//��ѵ÷�
int l;				//�ߵó��� 
int p, q;			//��������
int End;			//����Ƿ�Ե�����
IMAGE img_beijing;	//����
IMAGE img_she;		//��
IMAGE img_shep;		//����
IMAGE img_sco;		//�÷���
IMAGE img_over;		//��������
IMAGE img_start;	//��ʼ����
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
			outtextxy(260, 185, "���ĵ÷�Ϊ��"); outtextxy(360, 185, str1 );
			outtextxy(260, 210, "��ʷ���Ϊ��"); outtextxy(360, 210, str2 );
	}
	FlushBatchDraw();
}

void updateWithoutInput()
{
	int m, n, i, j, a, b, max = 0;
	srand((unsigned)time(NULL));//����time��������ȡ�����
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
	if (kbhit())							//����û��������ݣ�����Ҫ�س��� 
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