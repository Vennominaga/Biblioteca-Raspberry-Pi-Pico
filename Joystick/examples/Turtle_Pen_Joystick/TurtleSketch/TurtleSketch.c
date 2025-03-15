#include <stdio.h>
#include "inc/JoystickPi.h"

/******************************
 * Documentação do Programa
 ******************************/

/**
 * @file main.c
 * @brief Simulação de Tartaruga Gráfica
 * 
 * Este programa simula uma "tartaruga gráfica", onde o usuário controla o movimento de uma tartaruga
 * em uma matriz 10x10 usando um joystick. A tartaruga pode se mover para cima, baixo, esquerda e direita,
 * e pode deixar um rastro ('*') na matriz quando a "caneta" está abaixada.
 * 
 * Funcionalidades:
 * 1. Movimento da tartaruga controlado pelo joystick.
 * 2. Caneta que pode ser levantada ou abaixada para desenhar na matriz.
 * 3. Exibição da matriz em tempo real no terminal, com bordas estilizadas.
 * 4. Atualização contínua da posição da tartaruga e estado da caneta.
 * 
 * O joystick controla o movimento da tartaruga, e o botão do joystick alterna o estado da caneta
 * (levantada ou abaixada). Quando a caneta está abaixada, a tartaruga deixa um rastro ('*') na matriz.
 */

/******************************
 * Definições e Constantes
 ******************************/

/**
 * @brief Tamanho da matriz (10x10).
 */
#define MATRIX_SIZE 10

/******************************
 * Variáveis Globais
 ******************************/

/**
 * @brief Matriz que representa o "quadro" onde a tartaruga desenha.
 */
char matrix[MATRIX_SIZE][MATRIX_SIZE];

/**
 * @brief Posição horizontal (eixo X) da tartaruga na matriz.
 */
int turtle_x = 0;

/**
 * @brief Posição vertical (eixo Y) da tartaruga na matriz.
 */
int turtle_y = 0;

/**
 * @brief Estado da caneta (true = abaixada, false = levantada).
 */
bool pen_down = false;

/******************************
 * Funções
 ******************************/

/**
 * @brief Inicializa a matriz com espaços em branco.
 */
void initialize_matrix() {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = ' ';
        }
    }
}

/**
 * @brief Exibe a matriz no terminal, com bordas estilizadas e informações da tartaruga.
 */
void print_matrix() {
    // Desenha a borda superior da matriz
    printf("┌");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf("───");
        if (i < MATRIX_SIZE - 1) printf("┬");
    }
    printf("┐\n");

    // Desenha as linhas da matriz
    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf("│");
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf(" %c ", matrix[i][j]);
            printf("│");
        }
        printf("\n");

        // Desenha as divisórias entre as linhas
        if (i < MATRIX_SIZE - 1) {
            printf("├");
            for (int j = 0; j < MATRIX_SIZE; j++) {
                printf("───");
                if (j < MATRIX_SIZE - 1) printf("┼");
            }
            printf("┤\n");
        }
    }

    // Desenha a borda inferior da matriz
    printf("└");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf("───");
        if (i < MATRIX_SIZE - 1) printf("┴");
    }
    printf("┘\n");

    // Exibe informações adicionais
    printf("Posição da tartaruga: [%d][%d]\n", turtle_x, turtle_y);
    printf("Estado da caneta: %s\n", pen_down ? "Para baixo (escrevendo)" : "Para cima (não escrevendo)");
}

/**
 * @brief Move a tartaruga na matriz com base no estado do joystick.
 * @param state Estado atual do joystick (valores de X, Y e botão).
 */
void move_turtle(joystick_state_t state) {
    // Mapeia os valores do joystick para movimentos na matriz
    int16_t x_mapped = joystickPi_map_value(state.x, 0, 4095, -2, 2);
    int16_t y_mapped = joystickPi_map_value(state.y, 4095, 0, -1, 2);

    // Atualiza a posição da tartaruga
    turtle_x += x_mapped;
    turtle_y += y_mapped;

    // Limita a posição da tartaruga aos limites da matriz
    if (turtle_x < 0) turtle_x = 0;
    if (turtle_x >= MATRIX_SIZE) turtle_x = MATRIX_SIZE - 1;
    if (turtle_y < 0) turtle_y = 0;
    if (turtle_y >= MATRIX_SIZE) turtle_y = MATRIX_SIZE - 1;

    // Se a caneta estiver abaixada, desenha na matriz
    if (pen_down) {
        matrix[turtle_y][turtle_x] = '*';
    }
}

/******************************
 * Função Principal
 ******************************/

/**
 * @brief Função principal do programa.
 * @return Retorna 0 ao finalizar a execução.
 */
int main() {
    stdio_init_all(); // Inicializa a comunicação serial (para debug)

    joystickPi_init(); // Inicializa o joystick

    initialize_matrix(); // Inicializa a matriz com espaços em branco

    // Loop principal do programa
    while (1) {
        // Lê o estado atual do joystick
        joystick_state_t state = joystickPi_read();

        // Move a tartaruga com base no estado do joystick
        move_turtle(state);

        // Verifica se o botão do joystick foi pressionado para alternar o estado da caneta
        if (state.button) {
            pen_down = !pen_down;
            // Aguarda o botão ser solto para evitar múltiplas alternâncias
            while (joystickPi_read_button()) {}
        }

        // Limpa a tela e exibe a matriz atualizada
        printf("\033[H\033[J"); // Códigos ANSI para limpar a tela
        print_matrix();

        // Pequena pausa para evitar atualizações excessivas
        sleep_ms(100);
    }

    return 0;
}
