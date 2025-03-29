#include <stdio.h>
#include "inc/Joystick.h"

// Função para exibir uma barra horizontal
void print_horizontal_bar(int value, int max_length) {
    int bar_length = joystickPi_map_value(value, 0, 4095, 0, max_length);
    printf("[");
    for (int i = 0; i < max_length; i++) {
        if (i < bar_length) {
            printf("=");
        } else {
            printf(" ");
        }
    }
    printf("]\n");
}

// Função para exibir uma barra vertical
void print_vertical_bar(int value, int max_height) {
    int bar_height = joystickPi_map_value(value, 0, 4095, 0, max_height);
    for (int i = max_height; i >= 0; i--) {
        if (i < bar_height) {
            printf("||\n");
        } else {
            printf("  \n");
        }
    }
    printf("--\n");
}

int main() {
    stdio_init_all();
    // Inicializa o joystick
    joystick_init();

    while (true) {
        // Lê o estado atual do joystick
        joystick_state_t state = joystick_read();

        // Obtém a direção do joystick
        Direction dir = joystick_get_direction(state);

        // Exibe a direção no console
        switch (dir) {
            case CENTRALIZADO:
                printf("Direção: CENTRALIZADO\n");
                break;
            case CIMA:
                printf("Direção: CIMA\n");
                break;
            case BAIXO:
                printf("Direção: BAIXO\n");
                break;
            case ESQUERDA:
                printf("Direção: ESQUERDA\n");
                break;
            case DIREITA:
                printf("Direção: DIREITA\n");
                break;
            case DIREITA_CIMA:
                printf("Direção: DIREITA-CIMA\n");
                break;
            case DIREITA_BAIXO:
                printf("Direção: DIREITA-BAIXO\n");
                break;
            case ESQUERDA_CIMA:
                printf("Direção: ESQUERDA-CIMA\n");
                break;
            case ESQUERDA_BAIXO:
                printf("Direção: ESQUERDA-BAIXO\n");
                break;
            default:
                printf("Direção: DESCONHECIDA\n");
                break;
        }

        // Exibe a barra horizontal para o eixo X
        printf("Eixo X: ");
        print_horizontal_bar(state.x, 20);

        // Exibe a barra vertical para o eixo Y
        printf("Eixo Y:\n");
        print_vertical_bar(state.y, 10);

        // Aguarda um pouco antes de ler novamente (evita flood no console)
        sleep_ms(200);

        // Limpa o console para atualizar a exibição
        printf("\033[H\033[J"); // Código ANSI para limpar o console
    }

    return 0;
}