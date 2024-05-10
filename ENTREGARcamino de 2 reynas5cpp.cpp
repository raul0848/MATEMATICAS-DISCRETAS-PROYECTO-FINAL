#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define MAX_N 9 // Límite para el tamaño del tablero (n < 10)

// Declaración de funciones
void imprimirTablero(char tablero[MAX_N][MAX_N], int n);
void limpiarTablero(char tablero[MAX_N][MAX_N], int n, int filaR, int colR, int filaL, int colL);
bool reinasNoSeAtacan(int fila1, int col1, int fila2, int col2);
bool esMovimientoValido(char tablero[MAX_N][MAX_N], int fila, int col, int nuevaFila, int nuevaCol);
int moverReinas(char tablero[MAX_N][MAX_N], int *filaR, int *colR, int *filaL, int *colL, int n, int deltaFila, int deltaCol, int filaOriginalL, int colOriginalL);
void moverReinasEnDirecciones(char tablero[MAX_N][MAX_N], int *filaR, int *colR, int *filaL, int *colL, int n, int filaInicialR, int colInicialR, int filaInicialL, int colInicialL);
void checarReina2EnUltimaCasilla(char tablero[MAX_N][MAX_N], int *filaL, int *colL, int filaOriginalL, int colOriginalL, int n);
void checarPosicionReina2(char tablero[MAX_N][MAX_N], int *filaL, int *colL, int filaR, int colR, int filaOriginalL, int colOriginalL, int n);

// Función para imprimir el tablero con letras y números adecuados para filas y columnas
void imprimirTablero(char tablero[MAX_N][MAX_N], int n) {
    // Imprimir encabezado de columnas
    printf("  ");
    for (int col = 0; col < n; col++) {
        printf(" %d", col + 1);
    }
    printf("\n");

    // Imprimir el tablero con letras para filas y números para columnas
    for (int fila = 0; fila < n; fila++) {
        printf("%c ", 'A' + fila);
        for (int col = 0; col < n; col++) {
            printf("%c ", tablero[fila][col]);
        }
        printf("\n");
    }
    printf("\n");
}

// Función para limpiar el tablero y restablecer a sus posiciones originales
void limpiarTablero(char tablero[MAX_N][MAX_N], int n, int filaR, int colR, int filaL, int colL) {
    // Limpiar todo el tablero
    for (int fila = 0; fila < n; fila++) {
        for (int col = 0; col < n; col++) {
            tablero[fila][col] = '.';
        }
    }
    // Restablecer la posición inicial de la Reina 1 (L)
    tablero[filaR][colR] = 'L';
    // Restablecer la posición inicial de la Reina 2 (R)
    tablero[filaL][colL] = 'R';
}

// Verifica si dos posiciones de reinas no se atacan entre sí
bool reinasNoSeAtacan(int fila1, int col1, int fila2, int col2) {
    // Verifica si están en la misma fila, columna o diagonal
    return !(fila1 == fila2 || col1 == col2 || abs(fila1 - fila2) == abs(col1 - col2));
}

// Verifica si un movimiento es válido para la reina
bool esMovimientoValido(char tablero[MAX_N][MAX_N], int fila, int col, int nuevaFila, int nuevaCol) {
    // Verifica si los índices están dentro del tablero
    if (nuevaFila < 0 || nuevaFila >= MAX_N || nuevaCol < 0 || nuevaCol >= MAX_N) {
        return false;
    }
    // Verifica si la nueva casilla está vacía
    return tablero[nuevaFila][nuevaCol] == '.';
}

// Función para verificar si la Reina 2 está en la última fila o columna del tablero y moverla a su posición original si es necesario
void checarReina2EnUltimaCasilla(char tablero[MAX_N][MAX_N], int *filaL, int *colL, int filaOriginalL, int colOriginalL, int n) {
    // Verifica si la Reina 2 está en la última fila o última columna del tablero
    if (*filaL == n - 1 || *colL == n - 1) {
        // Si está en la última casilla, moverla a su posición original
        tablero[*filaL][*colL] = '.';
        *filaL = filaOriginalL;
        *colL = colOriginalL;
        tablero[*filaL][*colL] = 'R';
    }
}


// Función para verificar si la Reina 2 está en la misma fila, columna o diagonal que la Reina 1
// Si es así, moverla a su posición original y alejarla hacia la dirección contraria
void checarPosicionReina2(char tablero[MAX_N][MAX_N], int *filaL, int *colL, int filaR, int colR, int filaOriginalL, int colOriginalL, int n) {
    // Verificar si la Reina 2 está en la misma fila, columna o diagonal que la Reina 1
    bool mismaFila = (*filaL == filaR);
    bool mismaColumna = (*colL == colR);
    bool mismaDiagonal = (abs(*filaL - filaR) == abs(*colL - colR));

    // Si hay coincidencia, mover a la Reina 2 a su posición original
    if (mismaFila || mismaColumna || mismaDiagonal) {
        // Restaurar la posición original de la Reina 2
        tablero[*filaL][*colL] = '.';
        *filaL = filaOriginalL;
        *colL = colOriginalL;
        tablero[*filaL][*colL] = 'R';

        // Calcular la dirección contraria para alejar a la Reina 2
        int direccionContrariaFila = (*filaL < filaR) ? -1 : 1;
        int direccionContrariaCol = (*colL < colR) ? -1 : 1;

        // Calcular nuevas posiciones para alejar a la Reina 2
        int nuevaFilaL = *filaL + direccionContrariaFila * 2;
        int nuevaColL = *colL + direccionContrariaCol * 2;

        // Verificar si el movimiento es válido
        if (nuevaFilaL >= 0 && nuevaFilaL < n && nuevaColL >= 0 && nuevaColL < n && tablero[nuevaFilaL][nuevaColL] == '.') {
            // Mover a la Reina 2 a la nueva posición
            tablero[*filaL][*colL] = '.';
            *filaL = nuevaFilaL;
            *colL = nuevaColL;
            tablero[*filaL][*colL] = 'R';
        } else {
            // Si el movimiento no es válido, restablecer a la posición original
            tablero[*filaL][*colL] = 'R';
            *filaL = filaOriginalL;
            *colL = colOriginalL;
        }
    }
}

// Función para mover a las reinas
int moverReinas(char tablero[MAX_N][MAX_N], int *filaR, int *colR, int *filaL, int *colL, int n, int deltaFila, int deltaCol, int filaOriginalL, int colOriginalL) {
    int casillasRecorridas = 0;

    // Variables para las nuevas posiciones de las reinas
    int nuevaFilaR = *filaR + deltaFila;
    int nuevaColR = *colR + deltaCol;
    int nuevaFilaL = *filaL + deltaFila;
    int nuevaColL = *colL + deltaCol;

    // Recorre las casillas mientras el movimiento sea válido para la Reina 1
    while (esMovimientoValido(tablero, *filaR, *colR, nuevaFilaR, nuevaColR)) {
        // Mover la Reina 1
        tablero[*filaR][*colR] = '.';
        *filaR = nuevaFilaR;
        *colR = nuevaColR;
        tablero[*filaR][*colR] = 'L';

        // Si la posición de la Reina 2 es válida, moverla
        if (esMovimientoValido(tablero, *filaL, *colL, nuevaFilaL, nuevaColL)) {
            tablero[*filaL][*colL] = '.';
            *filaL = nuevaFilaL;
            *colL = nuevaColL;
            tablero[*filaL][*colL] = 'R';
        }
            
        // Checar si la Reina 2 está en modo de ataque con respecto a la Reina 1
        checarPosicionReina2(tablero, filaL, colL, *filaR, *colR, filaOriginalL, colOriginalL, n);


        // Imprimir el tablero después de mover las reinas
        imprimirTablero(tablero, n);

        // Incrementar el contador de casillas recorridas
        casillasRecorridas++;

        // Actualizar nuevas posiciones
        nuevaFilaR += deltaFila;
        nuevaColR += deltaCol;
        nuevaFilaL += deltaFila;
        nuevaColL += deltaCol;
    }

    return casillasRecorridas;
}

// Función para mover a las reinas en todas las direcciones
void moverReinasEnDirecciones(char tablero[MAX_N][MAX_N], int *filaR, int *colR, int *filaL, int *colL, int n, int filaInicialR, int colInicialR, int filaInicialL, int colInicialL) {
    const int direcciones[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    const char *nombresDirecciones[8] = {"izquierda", "derecha", "arriba", "abajo", "diagonal arriba izquierda", "diagonal arriba derecha", "diagonal abajo izquierda", "diagonal abajo derecha"};
    int totalCasillasRecorridas = 0;

    for (int d = 0; d < 8; d++) {
        printf("\nMoviendo reinas hacia la dirección %s...\n", nombresDirecciones[d]);

        // Mover las reinas en la dirección actual
        int casillasRecorridas = moverReinas(tablero, filaR, colR, filaL, colL, n, direcciones[d][0], direcciones[d][1], filaInicialL, colInicialL);
        totalCasillasRecorridas += casillasRecorridas;

        // Restablecer posiciones de las reinas
        *filaR = filaInicialR;
        *colR = colInicialR;
        *filaL = filaInicialL;
        *colL = colInicialL;

        // Mostrar el número de casillas recorridas en la dirección actual
        printf("\nDirección %s completada. Casillas recorridas: %d\n", nombresDirecciones[d], casillasRecorridas);

        // Restablecer el tablero a su estado original (con posiciones iniciales de reinas)
        limpiarTablero(tablero, n, filaInicialR, colInicialR, filaInicialL, colInicialL);

        // Imprimir el tablero restaurado
        printf("\nTablero restaurado a posiciones iniciales:\n");
        imprimirTablero(tablero, n);
    }

    // Mostrar el número total de casillas recorridas en todas las direcciones
    printf("\nNúmero total de casillas recorridas: %d\n", totalCasillasRecorridas);
}

int main() {
    int n;
    printf("\n************************ BIENVENIDO *************************");
    printf("\nESTE PROGRAMA CALCULA TODOS LOS POSIBLES CAMINOS DE DOS REINAS\n EN UN TABLERO DE AJEDREZ NxN\n CUANDO N<10 SIN QUE SE ATAQUEN ENTRE SI.\n");

    printf("\nIngrese el tamaño del tablero (n < 10): ");
    scanf("%d", &n);

    // Verificar que n esté dentro del rango permitido
    if (n <= 0 || n >= 10) {
        printf("\nEl tamaño del tablero debe ser entre 1 y 9. Saliendo...\n");
        return 1;
    }

    // Inicializa el tablero vacío
    char tablero[MAX_N][MAX_N];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            tablero[i][j] = '.';
        }
    }

    // Ingresar coordenadas de la Reina 1 (R)
    char letraFilaR;
    int colR;
    printf("\nIngrese la letra de la fila inicial de la Reina 1 (L): ");
    scanf(" %c", &letraFilaR);
    printf("Ingrese el número de la columna inicial de la Reina 1 (L): ");
    scanf("%d", &colR);

    // Convertir la letra de la fila a índice de fila
    int filaR = letraFilaR - 'A';
    colR--;

    // Verificar si la posición de la Reina 1 es válida
    if (filaR < 0 || filaR >= n || colR < 0 || colR >= n) {
        printf("\nPosición inicial de la Reina 1 no válida. Saliendo...\n");
        return 1;
    }

    // Ingresar coordenadas de la Reina 2 (R)
    char letraFilaL;
    int colL;
    printf("\nIngrese la letra de la fila inicial de la Reina 2 (R): ");
    scanf(" %c", &letraFilaL);
    printf("Ingrese el número de la columna inicial de la Reina 2 (R): ");
    scanf("%d", &colL);

    // Convertir la letra de la fila a índice de fila
    int filaL = letraFilaL - 'A';
    colL--;

    // Verificar si la posición de la Reina 2 es válida
    if (filaL < 0 || filaL >= n || colL < 0 || colL >= n) {
        printf("\nPosición inicial de la Reina 2 no válida. Saliendo...\n");
        return 1;
    }

    // Verificar si las posiciones iniciales son iguales o si las reinas se atacan
    if (filaR == filaL && colR == colL) {
        printf("\nPosiciones iniciales de las reinas son iguales. Saliendo...\n");
        return 1;
    }

    if (!reinasNoSeAtacan(filaR, colR, filaL, colL)) {
        printf("\nLas reinas se atacan en la posición inicial. Saliendo...\n");
        return 1;
    }

    // Colocar las reinas en el tablero
    tablero[filaR][colR] = 'L';
    tablero[filaL][colL] = 'R';

    // Imprimir el tablero inicial
    printf("\nTablero inicial:\n");
    imprimirTablero(tablero, n);

    // Mover reinas en todas las direcciones y mostrar los mensajes correspondientes
    moverReinasEnDirecciones(tablero, &filaR, &colR, &filaL, &colL, n, filaR, colR, filaL, colL);

    return 0;
}


