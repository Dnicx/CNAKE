#include <cstdio>
#include <conio.h>
#include <windows.h>
#include <utility>
#include <queue>
#include <stdlib.h>
#include <time.h>

#define hi 22
#define wi 78

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define GROUND 0
#define WALL 1
#define APPLE 2

#define APPLESCORE 10

#define RUN_SLEEP 10
#define LOOP_SLEEP 40

#define ARROW_UP 'H'
#define ARROW_DOWN 'P'
#define ARROW_LEFT 'K'
#define ARROW_RIGHT 'M'

using namespace std;

HANDLE console = /**/GetStdHandle(/**/STD_OUTPUT_HANDLE);
/**/COORD CursorPosition;


void gotoxy(int y,int x){
    CursorPosition.X = x;
    CursorPosition.Y = y;
    /**/SetConsoleCursorPosition(console,CursorPosition);
}

int dir[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

float speed_level[4] = {1.3, 1, 0.8, 0.5};
int speed;

int  cer_dir = RIGHT, end_game = 0;

int map[hi+1][wi+1];

int score = 0;

queue <pair <int, int> > body;

void quit();
void apple();
void print();
void run();
void start();
void game_over();
int menu();
void clear();
void make_menu();
void game_loop();
void draw_game_over();
void print_score();
void add_score();
// int select(int n, const char *head, const char **choice);
void set_speed();

void clear()  {
	gotoxy(0, 0);
	for (int i=0; i<hi; i++) {
		for (int i=0; i<wi; i++) {
			printf(" ");
		}
		printf("\n");
	}
}

void quit() {
	clear();
	gotoxy(hi/2, 15);
	printf("Are you sure you want to quit?");
	gotoxy((hi/2)+1, 15);
	printf("           Y / N");
	int Y = 26, N = 30;
	char con, cer = Y;
	gotoxy((hi/2)+1, Y);
	while (con != 13) {
		if (kbhit()) {
			con = getch();
			if (con == 'M' || con == 'K') {
				if (cer == Y) {
					gotoxy((hi/2)+1, N);
					cer = N;
				}
				else {
					gotoxy((hi/2)+1, Y);
					cer = Y;
				}
			}
			if (con == 13) {
				if (cer == N) ;
				else {
					clear();
					gotoxy((hi/2), 15);
					printf("Exiting game");
					gotoxy((hi/2)+1, 15);
					printf("Thank you for playing");
					gotoxy((hi/2)+2, 15);
					printf("By Teerapat Prompunjai");
					gotoxy(20, 0);
					Sleep(3000);
					end_game = 1;
				}
			}
		}
	}
	
}

void start() {
	gotoxy(0, 0);
	for (int i=0; i<=hi; i++) {
		for (int j=0; j<=wi; j++) {
			if (i >= hi || i <= 0 || j >= wi || j <= 0) { 
				printf("#");
				map[i][j] = 1;
			}
			else {
				printf(" ");
				map[i][j] = 0;
			}
		}
		printf("\n");
	}
	while (!body.empty()) {
		body.pop();
	}
	body.push(make_pair(15, 11));
	body.push(make_pair(15, 12));
	body.push(make_pair(15, 13));
	body.push(make_pair(15, 14));
	body.push(make_pair(15, 15));
	gotoxy(15, 11);
	printf("*****");
	gotoxy(hi, wi);
	cer_dir = 1;
	apple();
	score = 0;
	Sleep(100);
}

void draw_game_over() {
	gotoxy(hi/2, 15);
	printf("Game Over!!");
	gotoxy((hi/2)+1, 15);
	printf("Try again? Y / N");
}

void game_over() {
	draw_game_over();
	const int Y = 26, N = 30;
	char con, cer = Y;
	gotoxy((hi/2)+1, Y);
	while (con != 13) {
		if (kbhit()) {
			con = getch();
			if (con == 'M' || con == 'K') {
				if (cer == Y) {
					cer = N;
				}
				else {
					cer = Y;
				}
			}
			if (con == 13) {
				if (cer == Y) start();
				else {
					quit();
					if (end_game) return;
					cer = Y;
					con = 0;
					clear();
					draw_game_over();
				}
			}
			gotoxy((hi/2)+1, cer);
		}
	}
	
}

void apple() {
	int appx, appy;
	srand(time(NULL));
	appx = (rand()%(wi-3))+2;
	appy = (rand()%(hi-3))+2;
	gotoxy(appy, appx);
	printf("@");
	map[appy][appx] = APPLE;
}

void run() {
	//creat front
	int x = body.back().second  + dir[cer_dir][1];
	int y = body.back().first + dir[cer_dir][0];
	body.push(make_pair(y, x));
	gotoxy(y, x);
	printf("*");
	if (map[y][x] == WALL) game_over();
	else {
		if (map[y][x] == APPLE) {
			x = body.back().second  + dir[cer_dir][1];
			y = body.back().first + dir[cer_dir][0];
			body.push(make_pair(y, x));
			gotoxy(y, x);
			printf("*");
			map[y][x] = WALL;
			add_score();
			apple();
		}
		map[y][x] = WALL;
	}
	//if (y >= hi || y <= 0 || x >= wi || x <= 0) game_over();
	//delete back
	x = body.front().second;
	y = body.front().first;
	
	gotoxy(y, x);
	map[y][x] = GROUND;
	printf(" ");
	body.pop();
	if (cer_dir ==  GROUND || cer_dir == APPLE) Sleep(RUN_SLEEP);
}

void print() {
	gotoxy(0, 0);
	for (int i=0; i<=hi; i++) {
		for (int j=0; j<=wi; j++) {
			printf("%d", map[i][j]);
		}
		printf("\n");
	}
}

int menu() {
	gotoxy(15, 15);
	printf("MENU");
	gotoxy(16, 16);
	printf("Play");
	gotoxy(17, 16);
	printf("set speed");
	gotoxy(18, 16);
	printf("difficulty");
	gotoxy(19, 16);
	printf("Exit");
	gotoxy(16, 14);
	printf(">");
	int con, cur = 16;
	while (con != 13) {
		if (kbhit()) {
			con = getch();
			switch(con) {
				case 'H': gotoxy(cur, 14);
						  printf(" ");
						  cur--;
						  break;
				case 'P': gotoxy(cur, 14);
						  printf(" ");
						  cur++;
						  break;
			}
			if (cur > 19) cur = 16;
			if (cur < 16) cur = 19;
			gotoxy(cur, 14);
			printf(">");
		}
	}
	return cur-15;
}

void make_menu() {
	clear();
	switch (menu()) {
		case 1: game_loop();break;
		case 2: set_speed();break;
		//case 3: set_difficulty();break;
		case 4:quit();break;
	}
}

void game_loop() {
	char con = 'M';
	if(!end_game) start();
	while (!end_game) {
		run();
		if (kbhit()) {
			con = getch();
			switch(con) {
				case 'H':if (cer_dir != 2) cer_dir = 0;break;
				case 'M':if (cer_dir != 3) cer_dir = 1;break;
				case 'P':if (cer_dir != 0) cer_dir = 2;break;
				case 'K':if (cer_dir != 1) cer_dir = 3;break;
				case 27 : /*pause()*/quit();break;
				case ' ': while(!kbhit());break;
				case 'x':print();
				
			}
		}
		print_score();
		if (cer_dir == UP || cer_dir == DOWN) Sleep(LOOP_SLEEP*1.3*speed_level[speed]);
		else Sleep(LOOP_SLEEP*speed_level[speed]);
	}
}

int select(int n, const char *head, char *choice) {
	clear();
	gotoxy(10, 20);
	printf("%s", head);
	int i = 0;
	int cur = 0;
	while (choice[i++] != NULL) {
		gotoxy(11+i, 21);
		printf("%s", choice[i]); 
	}
	char con;
	while (con != 13) {
		if (kbhit()) {
			con = getch();
			switch(con) {
				case 'H': gotoxy(cur+11, 19);
						  printf(" ");
						  cur--;
						  break;
				case 'P': gotoxy(cur+11, 19);
						  printf(" ");
						  cur++;
						  break;
			}
			if (cur > n) cur = 0;
			if (cur < 0) cur = n-1;
			gotoxy(cur+11, 19);
			printf(">");
		}
	}
	return cur;
	getch();
}

void set_speed() {
	char *level[4] = {{"1"}, {"2"}, {"3"}, {"4"}};
	speed = select(4, "Speed", &level);
}

void print_score() {
	gotoxy(hi+1, wi-20);
	printf("SCORE = %d", score);
}

void add_score() {
	score += APPLESCORE;
}


int main() {
	make_menu();
	return 0;
}
