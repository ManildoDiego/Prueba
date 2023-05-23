#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define N 15
#define CHECK_MOV(mov) (mov != 'A' && mov != 'S' && mov != 'D' && mov != 'W')

struct Coordenadas_t {
    int x;
    int y;
};

// imprime el tablero
void print_tablero(const char t[N][N], const Coordenadas_t pos_tortuga, const Coordenadas_t pos_liebre) {
	for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
                // compruebo si es la posicion de la liebre
                // o si es la posicion de la tortuga
            if (i == pos_liebre.x && j == pos_liebre.y) {
                printf("L ");
                continue;
            }

            if (i == pos_tortuga.x && j == pos_tortuga.y) {
                printf("T ");
                continue;
            }

            // si no imprimo el tablero
            printf("%c ", t[i][j]);
        }

        putchar('\n');
	}
}

void inicializar_matriz(char t[N][N]) {
    // llena la matriz de letras 'X'
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            t[i][j] = 254;
        }
	}
    
    srand(time(0));
    const size_t cant_pozos = 2;
    const size_t cant_comodines = 2;
    size_t i, j;

    // declaro que la esquina de la derecha abajo es la meta
    t[N-1][N-1] = 'M';
    
    // relleno los pozos
    for (size_t k = 0; k < cant_pozos; k++) {
        do {
            i = rand() % N;
            j = rand() % N;
        } while (t[i][i] == 'O' || t[i][j] == 'M' || (i == 0 && j == 0)); // lo hago mientras no hay un pozo en esa posicion, ni sea la meta, ni sea el inicio

        t[i][j] = 'O';
    } 

    // relleno los comodines
    for (size_t k = 0; k < cant_comodines; k++) {
        do {
            i = rand() % N;
            j = rand() % N;
        } while (t[i][i] == 'O' || t[i][j] == 'C' || t[i][j] == 'M' || (i == 0 && j == 0)); // mientras que no haya un pozo, ni un comodin, ni la meta, ni el inicio

        t[i][j] = 'C';
    }
}

// devuelve el nombre del equipo, si es 1, Tortuga, si es 2 Liebre
const char* nombre_equipo(const int equipo) { return (equipo == 1) ? "Tortuga" : "Liebre"; }

// cambia el valor del equipo, si es 1 pasa a ser 2 y visceversa
int cambiar_equipo(const int equipo_actual) { return (equipo_actual == 1) ? 2 : 1; }

bool gano(const Coordenadas_t tortuga, const Coordenadas_t liebre) {
    const size_t meta = N-1;
    return ((tortuga.x == meta && tortuga.y == meta) || (liebre.x == meta && liebre.y == meta));
}

int main() {
    srand(time(0));
	char tablero[N][N]; // declaro el tablero

    Coordenadas_t pos_tortuga;
    Coordenadas_t pos_liebre;

    // inicializo las posiciones en {0, 0}
    pos_tortuga.x = 0;
    pos_tortuga.y = 0;
    pos_liebre.x = 0;
    pos_liebre.y = 0;

    int equipo = 1; // equipo, 1 = Tortuga, 2 = Liebre

    inicializar_matriz(tablero);

    while (!gano(pos_tortuga, pos_liebre)) {
    	system("cls");

        // obtengo el nombre del equipo
        const char* nombre = nombre_equipo(equipo);
        // y el primer caracter
        const char jugador_actual = nombre[0];

        printf("Juega %s\n", nombre);
        
        // obtengo un numero al azar del 1 al 6
        size_t dado = (rand() % 5) + 1;
        
        printf("%s lanzo el dado!\n", nombre);
        printf("Debera avanzar %llu casillas\n", dado);
        printf("Ingrese la posicion que desea moverse\n");
        printf("W - Arriba\n");
        printf("S - Abajo\n");
        printf("D - Derecha\n");
        printf("A - Izquierda\n");

        char mov_realizado;
        
        do {
            printf("Ingrese el movimiento que desea realizar: ");
            scanf(" %c", &mov_realizado);

            // paso la tecla a mayuscula por si la puso en minuscula
            if (mov_realizado >= 'a' && mov_realizado <= 'z') {
                mov_realizado ^= 0x20;
            }

            // compruebo si realizo un movimiento incorrecto
            if (CHECK_MOV(mov_realizado)) {
                printf("Movimiento invalido!\n");
            }
        } while (CHECK_MOV(mov_realizado)); // pregunto el movimiento mientras no sea el correcto
	
        // si toco la D se mueve a la derecha
		if (mov_realizado == 'D') {
            // si el jugador actual es la Liebre le sumo a la liebre las posiciones y si es la Tortuga, se la sumo a la Tortuga
            (jugador_actual == 'L') 
            ? pos_liebre.y += dado
            : pos_tortuga.y += dado;
		}

        // si toco la A se mueve a la izquierda
        else if (mov_realizado == 'A') {
            // si el jugador actual es la Liebre le resto a la liebre las posiciones y si es la Tortuga, se la resto a la Tortuga
            (jugador_actual == 'L') 
            ? pos_liebre.y -= dado
            : pos_tortuga.y -= dado;
		}
        
        // si toco la S se mueve a la izquierda
        else if (mov_realizado == 'S') {
            // si el jugador actual es la Liebre le suma a la liebre las posiciones y si es la Tortuga, se la suma a la Tortuga
            (jugador_actual == 'L') 
            ? pos_liebre.x += dado
            : pos_tortuga.x += dado;
		}

        // si toco la W se mueve a la izquierda
        else if (mov_realizado == 'W') {
            // si el jugador actual es la Liebre le resto a la liebre las posiciones y si es la Tortuga, se la resto a la Tortuga
            (jugador_actual == 'L') 
            ? pos_liebre.x -= dado
            : pos_tortuga.x -= dado;
		}
        
        // compruebo si la liebre no se sale de la matriz, si se sale, le arreglo la posicion
        if (pos_liebre.x >= (N-1)) pos_liebre.x = N-1;
        if (pos_liebre.x <= 0)     pos_liebre.x = 0;
        if (pos_liebre.y >= (N-1)) pos_liebre.y = N-1;
        if (pos_liebre.y <= 0)     pos_liebre.y = 0;

        // compruebo si la tortuga no se sale de la matriz, si se sale, le arreglo la posicion
        if (pos_tortuga.x >= (N-1)) pos_tortuga.x = N-1;
        if (pos_tortuga.x <= 0)     pos_tortuga.x = 0;
        if (pos_tortuga.y >= (N-1)) pos_tortuga.y = N-1;
        if (pos_tortuga.y <= 0)     pos_tortuga.y = 0;

        // si la posicion de la liebre o tortuga es un pozo muere
        if (tablero[pos_tortuga.x][pos_tortuga.y] == 'O' || tablero[pos_liebre.x][pos_liebre.y] == 'O') {
            printf("Perdio %s! Cayo en una bomba\n", nombre);
            return 0;
        }


    	system("cls");

        print_tablero(tablero, pos_tortuga, pos_liebre);
        
        system("pause");
        
        equipo = cambiar_equipo(equipo);
    }

    system("cls");
    printf("GANO %s\n", nombre_equipo(cambiar_equipo(equipo)));
    print_tablero(tablero, pos_tortuga, pos_liebre);
}
