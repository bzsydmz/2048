#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

int board[4][4];//4*4����
int score;//����
struct History {//��ʷ����
	int hustory_board[4][4];
	int history_score;
}H[6], h;
void hideCursor() {//���ع��
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = 0;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}
void moveCursor(SHORT x, SHORT y) {//���ƹ��λ��
	COORD pos = { x, y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}
void drawBoard() {//��ӡ���� 
	moveCursor(0, 0);
	printf("Score:%d           \n", score);
	int i, j;
	for (i = 0; i < 4; i++) {
		printf("+----+----+----+----+\n");
		for (j = 0; j < 4; j++) {
			printf("|");
			if (board[i][j])
				printf("%4d", board[i][j]);
			else
				printf("    ");
		}
		printf("|\n");
	}
	printf("+----+----+----+----+\n");
	printf("����˵����\n");
	printf("ʹ��'w''s''a''d'�����ƶ�\n");
	printf("��z��������һ��\n");
	printf("���ո�����¿�ʼ��Ϸ\n");
	printf("��Esc���˳���Ϸ\n");
}
void addRandom() { //�������������
	srand((int)time(NULL));
	while (1) {
		int i = rand() % 4;
		int j = rand() % 4;
		if (!board[i][j]) {
			if (rand() % 3)
				board[i][j] = 2;
			else
				board[i][j] = 4;
			break;
		}
	}
}
int* rowSet(int x, int y, int z) {
	if (z == 'a' || z == 'd')
		return &board[x][y];
	else if (z == 'w' || z == 's')
		return &board[y][x];
}
void Move(char c) {
	int i, j1, j, j2;
	for (i = 0; i < 4; i++) {
		int to_move[4] = { 0 };
		for (j1 = 0, j2 = 0; j1 < 4; j1++) {
			if (*rowSet(i, j1, c)) {
				to_move[j2++] = *rowSet(i, j1, c);
				*rowSet(i, j1, c) = 0;
			}
		}
		for (j = 0; j < 3; j++) {
			if (to_move[j] == to_move[j + 1]) {
				to_move[j] *= 2;
				score += to_move[j];
				to_move[j + 1] = 0;//402
			}
		}
		if (c == 'a' || c == 'w') {
			for (j1 = 0, j2 = 0; j1 < 4; j1++) {
				if (to_move[j1]) {
					*rowSet(i, j2, c) = to_move[j1];
					j2++;
				}
			}
		}
		else {
			for (j1 = 3, j2 = 3; j1 >= 0; j1--) {
				if (to_move[j1]) {
					*rowSet(i, j2, c) = to_move[j1];//���ҵ������µ��� 
					j2--;
				}
			}
		}
	}
}
void Store(int x[4][4], int y[4][4]) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			x[i][j] = y[i][j];
		}
	}
}
void storeHistory() {//ʼ�մ洢����ߵ����� 
	int i;
	for (i = 0; i < 5; i++) {
		Store(H[i].hustory_board, H[i + 1].hustory_board);
		H[i].history_score = H[i + 1].history_score;
	}
	Store(H[5].hustory_board, h.hustory_board);
	H[5].history_score = h.history_score;
}
void getHistory() {//���� 
	Store(board, H[5].hustory_board);
	score = H[5].history_score;
	int i;
	for (i = 5; i; i--) {
		Store(H[i].hustory_board, H[i - 1].hustory_board);
		H[i].history_score = H[i - 1].history_score;
	}
}
void initBoard() {//��ʼ�����̽���
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			board[i][j] = 0;
		}
	}
	score = 0;
	addRandom();
	addRandom();
	drawBoard();
	for (i = 0; i < 6; i++) {
		Store(H[i].hustory_board, board);
	}
}
int checkIfEnd() {//�ж���Ӯ������7ʤ����0ʧ�ܣ�1������Ϸ
	int i, j;
	int if_full = 0;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (board[i][j] == 2048) {
				return 7;
			}
			else if (board[i][j] == 0) {
				if_full = 1;
			}
		}
	}
	return if_full;
}
int checkIfMoved() { //�Ƿ���Ч�ƶ�����
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (board[i][j] != h.hustory_board[i][j]) {
				return 1;
			}
		}
	}
	return 0;
}
int Game() {
	initBoard();
	while (true) {
		int input = _getch();
		if (input == 'w' || input == 's' || input == 'a' || input == 'd') {
			Store(h.hustory_board, board);
			h.history_score = score;//�ݴ��ƶ�ǰ����Ϣ����ֹ��Ч��������һ����������ʷ��¼
			Move(input);
			drawBoard();
			if (checkIfEnd() == 7) {
				moveCursor(0, 10);
				printf("Victory!    \n");
				printf("                        \n");
				break;
			}
			else if (checkIfEnd() == 0) {
				moveCursor(0, 10);
				printf("Defeat!     \n");
				printf("                        \n");
				break;
			}
			if (checkIfMoved()) {
				addRandom();
				Sleep(350);
				drawBoard();
				storeHistory();
			}
		}
		else if (input == 'z') {
			getHistory();
			drawBoard();
		}
		else if (input == ' ') {
			initBoard();
		}
		else if (input == 27) {
			system("cls");
			printf("\nYou Have Logged Out\n\n");
			return 1;
		}
	}
	return 0;
}
int main(){
	hideCursor();
	printf("Welcome To 2048\n\n");
	Sleep(717);
	printf("loading.");
	int i;
	for (i = 0; i < 5; i++)
	{
		Sleep(200);
		printf(".");
	}
	moveCursor(0, 2);
	printf("Press Enter to Start\n\n");
	printf("Press Esc to Exit\n");
	while (1) {
		int input = _getch();
		if (input == 13) {
			system("cls");
			if (Game()) {
				break;
			}
			else {
				printf("Press Enter To Start Again\n");
				printf("                       \n");
				printf("Press Esc To Exit\n");
			}
		}
		else if (input == 27) {
			system("cls");
			printf("You Have Logged Out\n");
			break;
		}
	}
	return 0;
}
