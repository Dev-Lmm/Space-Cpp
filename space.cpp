#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80


using namespace std;

void gotoXY(int x, int y) {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;

	SetConsoleCursorPosition(hCon, dwPos);
}

void hiddenCursor() {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 50;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hCon,&cci);
}

void margin() {
	for(int i = 2; i <78; i++)
	{
		gotoXY(i,3); printf("%c",205);
		gotoXY(i,33); printf("%c",205);
	}

	for(int i = 4; i <33; i++)
	{
		gotoXY(2,i); printf("%c",186);
		gotoXY(77,i); printf("%c",186);
	}

	gotoXY(2,3); printf("%c", 201);
	gotoXY(2,33); printf("%c", 200);
	gotoXY(77,3); printf("%c", 187);
	gotoXY(77,33); printf("%c", 188);
}

class nave {
	int x,y;
	int life;
	int nLife;
public:
	nave(int _x, int _y, int _life, int _nLife);
	int posX() {return x;}
	int posY() {return y;}
	int gameover() {return nLife;}
	void exhibit();
	void deleted();
	void move();
	void life();
	void dead();
	void subtractLife() { life--;}
};

nave::nave(int _x, int _y, int _life, int _nLife):x(_x),y(_y),life(_life),nLife(_nLife) {}

void nave::exhibit() {
	gotoXY(x,y);	printf("  ^");
	gotoXY(x,y+1);	printf(" < >");
	gotoXY(x,y+2);	printf("<< >>");
}

void nave::deleted() {
	gotoXY(x,y);	printf("     ");
	gotoXY(x,y+1);	printf("     ");
	gotoXY(x,y+2);	printf("     ");
}
void nave::move() {
	if(kbhit())
		{
			deleted();
			char key = getch();
			gotoXY(x,y); printf(" ");
			if(key == LEFT && x>3) x--;
			if(key == RIGHT && x+5<77) x++;
			if(key == UP &&	y>4) y--;
			if(key == DOWN && y+3<33) y++;
			//if (key == 'e') life--;
			exhibit();
			life();
		}
}

void nave::life() {
	gotoXY(50,2); printf("lifes: %d",nLife);
	gotoXY(64,2); printf("Salud" );
	gotoXY(70,2); printf("     ");
	for(int i =0; i<life;i++)
	{
		gotoXY(70+i,2); printf("X");
	}
}

void nave::dead() {
	if(life == 0)
	{
		deleted();
		gotoXY(x,y);	printf(" * *");
		gotoXY(x,y+1);	printf("* * *");
		gotoXY(x,y+2);	printf(" * *");
		Sleep(200);

		deleted();
		gotoXY(x,y);	printf("* * *");
		gotoXY(x,y+1);	printf(" *** ");
		gotoXY(x,y+2);	printf("* * *");
		Sleep(200);
		deleted();

		nLife--;
		life = 3;
		exhibit();
	}
}

class ovni {
	int x,y;
 public:
 ovni(int _x, int _y):x(_x),y(_y) {}
 void exhibit();
 void move();	
 void collision(class nave &_n);
 int Xov() {return x;}
 int Yov() {return y;}
};

void ovni::exhibit() {
	gotoXY(x,y);	printf("%c",184);
}

void ovni:: collision(class nave &_n) {
	if(x >= _n.posX() && x < _n.posX()+6 && y >= _n.posY() && y <= _n.posY()+2)
	{
		_n.subtractLife();
		_n.deleted();
		_n.exhibit();
		_n.life();
		x = rand() % 71+4;
		y = 4;
	}
}

void ovni::move() {
	gotoXY(x,y);	printf(" ");
	y++;
	if(y > 32)
	{
		x = rand() % 71+4;
		y = 4;
	}
	exhibit();
}
class bullet {
	int x,y;
public:
	bullet(int _x, int _y):x(_x),y(_y) {}
	int posX() {return x;}
	int posY() {return y;}
	void moving();
	bool outR();
	
};

void bullet::moving() {
	gotoXY(x,y); printf(" ");
	/*if(y > 4)*/ 
	y--;
	gotoXY(x,y); printf("*");
}

bool bullet::outR() {
	if(y == 4) return true;
	return false;	
	

}

int main(int argc, char const *argv[]) {

	hiddenCursor();
	margin();
	nave _n(37,27,3,3);
	_n.exhibit();
	_n.life();

	//ovni asteroide(10,4);
	list<ovni*>O;
	list<ovni*>::iterator itA;
	for (int i = 0; i < 5; ++i)	{
		O.push_back(new ovni(rand()%75+3, 4));
	}

	list<bullet*>B;
	list<bullet*>::iterator it;
	
	int points = 0;
	bool gg = false;
	while(! gg)	{
		gotoXY(4,2);
		printf("Puntos: %d", points);
		if(kbhit()) {
			char key = getch();
			if(key == 'a') {
				B.push_back(new bullet(_n.posX()+2 , _n.posY()-1));
			}
		}

		for(it = B.begin(); it != B.end(); it++) {
			(*it)->moving();
			if((*it)->outR()) {
				gotoXY((*it)->posX(), (*it)->posY());	printf(" ");
				delete(*it);
				it = B.erase(it);
			} 
		}

		for(itA = O.begin(); itA != O.end(); itA++)	{
			(*itA)->move();
			(*itA)->collision(_n);
		}

		for(itA = O.begin(); itA != O.end(); itA++)	{
			for(it = B.begin(); it != B.end(); it++) {
				if((*itA)-> Xov() == (*it)-> posX() && ((*itA)->Yov()+1== (*it)->posY() || (*itA)->Yov()  == (*it)->posY())) {
					gotoXY((*it)->posX(), (*it)->posY()); printf(" ");
					delete(*it);
					it = B.erase(it);

					O.push_back(new ovni(rand()% 75 + 3, 4));
					gotoXY((*itA)->Xov(), (*itA)->Yov()); printf(" ");
					delete(*itA);
					itA = O.erase(itA);

					points++;
				}
			}
		}
		_n.move();
		_n.dead();
		if(_n.gameover() == 0) {
			gg = true;
		}
		
		Sleep(50);
	}
	system("cls");

	return 0;
}
