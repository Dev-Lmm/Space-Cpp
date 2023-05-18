#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80


using namespace std;

void gotoXY(int x, int y)
{
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;

	SetConsoleCursorPosition(hCon, dwPos);
}

void ocultarCursor()
{
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 50;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hCon,&cci);
}

void margen()
{
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

class nave
{
	int x,y;
	int vida;
	int nVida;
public:
	nave(int _x, int _y, int _vida, int _nVida);
	int posX(){return x;}
	int posY(){return y;}
	int gameover(){return nVida;}
	void pintar();
	void borrar();
	void mover();
	void mostrarvida();
	void morir();
	void restVida(){ vida--;}
};

nave::nave(int _x, int _y, int _vida, int _nVida):x(_x),y(_y),vida(_vida),nVida(_nVida){}

void nave::pintar()
{
	gotoXY(x,y);	printf("  ^");
	gotoXY(x,y+1);	printf(" < >");
	gotoXY(x,y+2);	printf("<< >>");
}

void nave::borrar()
{
	gotoXY(x,y);	printf("     ");
	gotoXY(x,y+1);	printf("     ");
	gotoXY(x,y+2);	printf("     ");
}
void nave::mover()
{
	if(kbhit())
		{
			borrar();
			char tecla = getch();
			gotoXY(x,y); printf(" ");
			if(tecla == IZQUIERDA && x>3) x--;
			if(tecla == DERECHA && x+5<77) x++;
			if(tecla == ARRIBA &&	y>4) y--;
			if(tecla == ABAJO && y+3<33) y++;
			//if (tecla == 'e') vida--;
			pintar();
			mostrarvida();
		}
}

void nave::mostrarvida()
{
	gotoXY(50,2); printf("Vidas: %d",nVida);
	gotoXY(64,2); printf("Salud" );
	gotoXY(70,2); printf("     ");
	for(int i =0; i<vida;i++)
	{
		gotoXY(70+i,2); printf("X");
	}
}

void nave::morir()
{
	if(vida == 0)
	{
		borrar();
		gotoXY(x,y);	printf(" * *");
		gotoXY(x,y+1);	printf("* * *");
		gotoXY(x,y+2);	printf(" * *");
		Sleep(200);

		borrar();
		gotoXY(x,y);	printf("* * *");
		gotoXY(x,y+1);	printf(" *** ");
		gotoXY(x,y+2);	printf("* * *");
		Sleep(200);
		borrar();

		nVida--;
		vida = 3;
		pintar();
	}
}

class ovni
{
	int x,y;
 public:
 ovni(int _x, int _y):x(_x),y(_y){}
 void pintar();
 void mover();	
 void impacto(class nave &_n);
 int Xov(){return x;}
 int Yov(){return y;}
};

void ovni::pintar()
{
	gotoXY(x,y);	printf("%c",184);
}

void ovni:: impacto(class nave &_n)
{
	if(x >= _n.posX() && x < _n.posX()+6 && y >= _n.posY() && y <= _n.posY()+2)
	{
		_n.restVida();
		_n.borrar();
		_n.pintar();
		_n.mostrarvida();
		x = rand() % 71+4;
		y = 4;
	}
}

void ovni::mover()
{
	gotoXY(x,y);	printf(" ");
	y++;
	if(y > 32)
	{
		x = rand() % 71+4;
		y = 4;
	}
	pintar();
}
class bala
{
	int x,y;
public:
	bala(int _x, int _y):x(_x),y(_y){}
	int posX(){return x;}
	int posY(){return y;}
	void movimiento();
	bool outR();
	
};

void bala::movimiento()
{
	gotoXY(x,y); printf(" ");
	/*if(y > 4)*/ 
	y--;
	gotoXY(x,y); printf("*");
}

bool bala::outR()
{
	if(y == 4) return true;
	return false;	
	

}

int main(int argc, char const *argv[])
{

	ocultarCursor();
	margen();
	nave _n(37,27,3,3);
	_n.pintar();
	_n.mostrarvida();

	//ovni asteroide(10,4);
	list<ovni*>O;
	list<ovni*>::iterator itA;
	for (int i = 0; i < 5; ++i)
	{
		O.push_back(new ovni(rand()%75+3, 4));
	}

	list<bala*>B;
	list<bala*>::iterator it;
	
	int puntos = 0;
	bool gg = false;
	while(! gg)
	{
		gotoXY(4,2);printf("Puntos: %d", puntos);
		if(kbhit())
		{
			char tecla = getch();
			if(tecla == 'a')
			{
				B.push_back(new bala(_n.posX()+2 , _n.posY()-1));
			}
		}

		for(it = B.begin(); it != B.end(); it++)
		{
			(*it)->movimiento();
			if((*it)->outR())
			{
				gotoXY((*it)->posX(), (*it)->posY());	printf(" ");
				delete(*it);
				it = B.erase(it);
			} 
		}



		/*asteroide.mover();
		asteroide.impacto(_n);*/

		for(itA = O.begin(); itA != O.end(); itA++)
		{
			(*itA)->mover();
			(*itA)->impacto(_n);
		}

		for(itA = O.begin(); itA != O.end(); itA++)
		{
			for(it = B.begin(); it != B.end(); it++)
			{
				if((*itA)-> Xov() == (*it)-> posX() && ((*itA)->Yov()+1== (*it)->posY() || (*itA)->Yov()  == (*it)->posY()))
				{
					gotoXY((*it)->posX(), (*it)->posY()); printf(" ");
					delete(*it);
					it = B.erase(it);

					O.push_back(new ovni(rand()% 75 + 3, 4));
					gotoXY((*itA)->Xov(), (*itA)->Yov()); printf(" ");
					delete(*itA);
					itA = O.erase(itA);

					puntos++;
					
				}
			}
		}
		_n.mover();
		_n.morir();
		if(_n.gameover() == 0)
		{
			gg = true;
		}
		
		Sleep(50);
	}
	system("cls");

	return 0;
}
