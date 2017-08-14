#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <Windows.h>


using namespace std;


const int DIM=4;

const int META=2048000;




//CARACTERES GRÁFICOS

const char eI = char (218);	// Esquina S izda
const char ei = char (192);	// Esquina I izda
const char eD = char (191);	// Esquina S dcha
const char ed = char (217);	// Esquina I dcha
const char lh = char (196);	// Linea horizontal
const char lv = char (179);	// Linea vertical
const char td = char (195);	// T hacia la dcha
const char t =  char (194);	// T
const char ta = char (193);	// T hacia arriba
const char ti = char (180);	// T hacia la izda
const char x =  char (197);	// Cruz



//TECLAS ESPECIALES

const int TE_Salir = VK_ESCAPE;
const int TE_Arriba = VK_UP;
const int TE_Abajo = VK_DOWN;
const int TE_Derecha = VK_RIGHT;
const int TE_Izquierda = VK_LEFT;



typedef int tTablero [DIM][DIM];

typedef struct {

	int x;

	int y;

} tCoord;

typedef enum {Arriba, Abajo, Derecha, Izquierda} tDireccion;

typedef enum {OP_Arriba, OP_Abajo, OP_Derecha, OP_Izquierda, OP_Salir} tOpcion;

typedef enum { Pierde, Gana, Jugando } tResultado;

bool movImposible (const tTablero tablero);


void inicializa (tTablero tablero){

	for (int fila=0; fila<DIM; fila++)
		for (int col=0; col<DIM; col++){

			tablero[fila][col]=1;

		} //Tablero vacío


		srand(time(NULL));

		int prob4=rand() % (20); //Entre 0 y 19
		int randf=rand() % (DIM);
		int randc=rand() % (DIM);
		int randf2=rand() % (DIM);
		int randc2=rand() % (DIM); //Entre 0 y DIM-1

		tablero[randf][randc]=2;

		while (randf==randf2 && randc==randc2){

			randf2=rand() % (DIM);
			randc2=rand() % (DIM);

		} //Si coinciden en la misma casilla, genera otra

		if (prob4==10) tablero[randf2][randc2]=4;

		else tablero[randf2][randc2]=2;

}


void nuevaFicha (tTablero tablero){

	srand(time(NULL));

	int prob4=rand() % (20); //Entre 0 y 19
	int randf=rand() % (DIM);
	int randc=rand() % (DIM);


	while (tablero[randf][randc]!=1){

		randf=rand() % (DIM);
		randc=rand() % (DIM);

	} //Si la casilla no esta vacía, genera otra

	if (prob4==10) tablero[randf][randc]=4;

	else tablero[randf][randc]=2;

}

int log2(int num){

	int exp=0;

	while (num%2==0){

		num=num/2;
		exp++;

	}

	if (num==1) return exp;

	else return -1; //ERROR: num no es potencia de 2
}

void backgroundTextAtt (int color) {

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 | (color << 4));

}



void visualiza (const tTablero tablero, int fila){


	for (int i=0; i<DIM; i++){

		cout << lv << setw(6); backgroundTextAtt (log2(tablero[fila][i])); cout << " "; backgroundTextAtt (0);

	} cout << lv << endl; backgroundTextAtt (0);


	for (int i=0; i<DIM; i++){

		cout << lv << setw(4); backgroundTextAtt (log2(tablero[fila][i]));

		if (tablero[fila][i]!=1) cout << tablero[fila][i];
		else cout << " "; 

		cout << setw(2); cout << " "; backgroundTextAtt (0); 

	} cout << lv << endl; backgroundTextAtt (0);

	for (int i=0; i<DIM; i++){

		cout << lv << setw(6); backgroundTextAtt (log2(tablero[fila][i])); cout << " "; backgroundTextAtt (0);

	} cout << lv << endl; backgroundTextAtt (0);

}


void bordeSuperior () {

	cout << eI ;

	for (int i=0; i<DIM-1; i++){

		cout << lh << lh << lh << lh << lh << lh << t;

	}

	cout << lh << lh << lh << lh << lh << lh << eD << endl; backgroundTextAtt (0);

}


void bordeInferior () {

	cout << ei ;

	for (int i=0; i<DIM-1; i++){

		cout << lh << lh << lh << lh << lh << lh<< ta;

	}

	cout << lh << lh << lh << lh << lh << lh << ed << endl;  backgroundTextAtt (0);

}

void bordeInter () {

	cout << td ;

	for (int i=0; i<DIM-1; i++){

		cout << lh << lh << lh << lh << lh << lh << x; 

	}

	cout << lh << lh << lh << lh << lh << lh << ti << endl; backgroundTextAtt (0);

}




void visualiza (const tTablero tablero, int puntos, int totalPuntos){

	cout << endl << setw(6) << left << "Jugada: " << puntos << setw(6)  << right << "  Total: " << totalPuntos << setw(6) << right << "  Meta: " << META << endl;

	bordeSuperior(); 

	for (int i=0; i<DIM; i++){

		visualiza (tablero, i);

		if (i<DIM-1) bordeInter(); 


	}


	bordeInferior();


}


bool carga (tTablero tablero, int & puntos){

	ifstream archivo;

	string fileName;

	int meta, dim, num;

	cout << "Nombre archivo: ";

	cin >> fileName;

	archivo.open(fileName);

	if (archivo.is_open()) {

		archivo >> meta >> dim;

		if (meta==META && dim==DIM){


			for (int i=0; i<DIM; i++)

				for (int j=0; j<DIM; j++){

					archivo >> num;

					tablero[i][j]=num;
				}

				archivo >> num; puntos=num;

				return true;

		}

		else return false;
	}

	else return false;
}



bool guarda (const tTablero tablero, int puntos){

	ofstream archivo;

	string fileName;

	cout << "\nNombre archivo: ";

	cin >> fileName;

	archivo.open(fileName);

	archivo << META << " " <<  DIM << endl;

	for (int i=0; i<DIM; i++)

		for (int j=0; j<DIM; j++){

			archivo << tablero[i][j] << endl;

		}


		archivo << puntos;

		return true;

}




int getKey() {

	int key;

	INPUT_RECORD record; DWORD num;
	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

	do {
		ReadConsoleInput(hStdIn, &record, 1, &num);
	} while(record.EventType != KEY_EVENT );

	if (record.Event.KeyEvent.uChar.AsciiChar == 0) { // teclas especiales
		ReadConsoleInput(hStdIn, &record, 1, &num); // leer otro
		key = record.Event.KeyEvent.wVirtualKeyCode;
	}

	else key = record.Event.KeyEvent.uChar.AsciiChar;

	return key;
}


tOpcion menu() {

	cout << "Usa las flechas (Esc para salir)\n";

	int op;

	op=getKey();

	tOpcion opcion;

	if (op==TE_Salir) opcion = OP_Salir;

	else if (op==TE_Arriba) opcion = OP_Arriba;

	else if (op==TE_Abajo) opcion = OP_Abajo;

	else if (op==TE_Derecha) opcion = OP_Derecha;

	else opcion = OP_Izquierda;

	return opcion;

}



void obtenerCoordMov (tDireccion dir, int ind, tCoord & ini, tCoord & incr) {


	if (dir==Derecha){ 

		ini.x=ind;
		ini.y=DIM-1;

		incr.x=0;
		incr.y=-1;

	}

	else if (dir==Izquierda){

		ini.x=ind;
		ini.y=0;

		incr.x=0;
		incr.y=1;

	}


	else if (dir==Abajo) {

		ini.x=DIM-1;
		ini.y=ind;

		incr.x=-1;
		incr.y=0;

	}


	else if (dir==Arriba){

		ini.x=0;
		ini.y=ind;

		incr.x=1;
		incr.y=0;

	}

}




/*bool mueveFichas (tTablero tablero, tDireccion direccion){

	bool output=false;

	tCoord ini;

	tCoord incr;

	tCoord libre;

	int tope=1;


	for (int ind = 0; ind < DIM; ind ++){

		obtenerCoordMov(direccion, ind, ini, incr);

		libre=ini;

		for (int i = 0; i < DIM; i++){

			if (tablero[ini.x][ini.y]!=1){


				if (tablero[libre.x][libre.y]==1) {

					tablero[libre.x][libre.y]=tablero[ini.x][ini.y];

					tablero[ini.x][ini.y]=1;

				}

				libre.x+=incr.x; libre.y+=incr.y;

			}

			ini.x+=incr.x;

			ini.y+=incr.y;

		}
	}


	return output;
}

*/





int mayor (const tTablero tablero){

	int mayor=1;

	for (int x=0; x < DIM; x++)

		for (int y=0; y < DIM; y++)

			if (tablero[x][y]>mayor) mayor=tablero[x][y];

	
	return mayor;

}



bool lleno (const tTablero tablero){

	int x=0;

	int y;

	bool vacia=false;

	while (x<DIM && !vacia){

		y=0;

		while (y<DIM && !vacia){

			if (tablero[x][y]==1) vacia = true;

			else y++;

		}

		if (!vacia) x++;

	}

	return !vacia; // Si ha encontrado una casilla vacía (vacia=true), devuelve false

}



tResultado mueveFichas (tTablero tablero, tDireccion direccion, int & puntos, int & total, bool & mov){  // El parámetro mov devuelve true si se ha producido algún movimiento, o false en caso contrario

	puntos=0;

	mov=false;

	tCoord ini;

	tCoord incr;

	tCoord libre;

	int tope;

	for (int ind = 0; ind < DIM; ind ++){

		obtenerCoordMov(direccion, ind, ini, incr);

		libre=ini; // libre será la posición de la "primera" casilla libre en la fila o columna ind segun la direccion dada

		tope=tablero[ini.x][ini.y]; // tope sera el valor de la "ultima" casilla (la anterior a la libre) con una ficha

		for (int i = 0; i < DIM; i++){

			if (tablero[ini.x][ini.y]!=1){ // Si en la posicion no hay ficha, no hay nada que hacer


				if ((libre.x!=ini.x || libre.y!=ini.y) && tablero[ini.x][ini.y]!=tope) { // Si la ficha va solo a moverse se intercambien el valor de libre (1) y la ficha en cuestion
					
						tablero[libre.x][libre.y]=tablero[ini.x][ini.y];

						tablero[ini.x][ini.y]=1;

						tope=tablero[libre.x][libre.y];

						mov=true;

					}

				 
				else if (tablero[ini.x][ini.y]==tope && i!=0) { // Si la ficha va a moverse y a combinarse, la posicion de la ficha pasará a estar vacía y en la anterior a la libre pasará a estar la suma de la fichas más la que había, y el tope pasa a valer 0 para que no vuelva a combinarse

						tablero[libre.x-incr.x][libre.y-incr.y]+=tablero[ini.x][ini.y];

						tablero[ini.x][ini.y]=1;

						tope=0;
						
						puntos+=tablero[libre.x-incr.x][libre.y-incr.y];

						total+=puntos;

						libre.x=libre.x-incr.x; libre.y=libre.y-incr.y; // Se resta ya que despues se incrementa, y en este caso queremos que la posicion de libre no cambie

						mov=true;

						

					}

				else tope=tablero[ini.x][ini.y]; // si no se mueve, esa ficha pasa a ser el valor tope

				
				libre.x+=incr.x; libre.y+=incr.y;  //Como en la posición había una ficha, la libre quedará ocupada (salvo que haya una combinación) y entonces libre pasara a ser la casilla siguiente

			
			}
				
			ini.x+=incr.x;

			ini.y+=incr.y;  //Procesamos la siguente ficha

		}

	}


	tResultado resultado;

	if (mayor(tablero)>=META) resultado=Gana;

	else if (movImposible(tablero)) resultado=Pierde;

	else resultado=Jugando;



	return resultado;


}


bool movImposible (const tTablero tablero){   // Devuelve true si al jugador no le quedan movimientos disponibles

	bool imposible;

	if (!lleno(tablero)) imposible = false;   //Si el tablero no está lleno, el jugador puede mover alguna ficha

	else {									// Aun con el tablero lleno, es posible que se puedan hacer combinaciones (busca 2 casillas adyacentes iguales)

		bool adyIguales=false;

		int i, j;

		i=0;

		while (i<DIM && !adyIguales){

			j=0;

			while (j<DIM && !adyIguales){

				if ((i<DIM-1 &&  tablero[i][j]==tablero[i][j+1]) || ( j<DIM-1 && tablero[i][j]==tablero[i+1][j])) adyIguales=true;

				else j++;

			}

			i++;
		}
		

		imposible=!adyIguales;
	}

	return imposible;

}

		


void fontConsole(bool console){
CONSOLE_FONT_INFOEX cfi;
cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
cfi.FontWeight = 400;
if (console) {
cfi.nFont = 1;
cfi.dwFontSize.X = 12; cfi.dwFontSize.Y = 20;
cfi.FontFamily = 54;
wcscpy_s(cfi.FaceName, L"Lucida Console"); // consola
}
else {
cfi.nFont = 8;
cfi.dwFontSize.X = 12; cfi.dwFontSize.Y = 16;
cfi.FontFamily = 48;
wcscpy_s(cfi.FaceName, L"Terminal"); // mapa de bits
}
SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &cfi);
}





void clearConsole() { system("cls"); }





int main(){

	tTablero tablero;

	int puntos=0, totalPuntos=0;

	char op, op2, otra;

	bool mov;
	
	fontConsole(true);

	otra='s';

	while (otra=='s'){

		cout << "1. Nueva partida\n2. Cargar partida\n";

		cin >> op;	

		if (op=='1') {

			inicializa(tablero);

			clearConsole();

			visualiza(tablero, puntos, totalPuntos);

		}


		else if (op=='2') {

			if (carga(tablero, puntos));

			else inicializa(tablero);

			clearConsole();

			visualiza(tablero, puntos, totalPuntos);

		}

		tResultado resultado=Jugando;


		tDireccion dir;

		int tecla=menu();

		while (tecla!=4 && resultado==Jugando){


			dir=tDireccion(tecla);

		
				resultado=mueveFichas(tablero, dir, puntos, totalPuntos, mov);


				clearConsole();

			
				if (mov) nuevaFicha(tablero);


				visualiza(tablero, puntos, totalPuntos);



				if (resultado==Jugando) tecla=menu();

		}


			if (resultado == Gana) cout << "\nEnhorabuena, has ganado!!\n";

			else if (resultado==Pierde) cout << "\nLo siento, has perdido\n";

			else { 
			
				cout << "\nDesea guardar la partida (s/n)?: ";

				cin >> op2;

				if (op2=='s') {

					guarda(tablero, puntos);

				}

				
			}
			
			
			cout << "\nDesea jugar otra partida (s/n)?: ";

				cin >> otra;

				clearConsole();


	}

	return 0;

}
