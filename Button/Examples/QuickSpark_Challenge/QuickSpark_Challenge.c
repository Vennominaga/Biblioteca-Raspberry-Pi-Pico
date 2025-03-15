/*-------------DOCUMENTAÇÃO ---------------*/

/**
 * @file main.c
 * @brief Jogo de Teste de Reflexo
 * 
 * Este programa implementa um jogo de teste de reflexo, onde o jogador deve pressionar um botão
 * assim que um LED acender. O objetivo é medir o tempo de reação do jogador em várias rodadas.
 * 
 * O jogo funciona da seguinte forma:
 * 1. O LED acende após um intervalo aleatório (entre 1 e 4 segundos).
 * 2. O jogador deve pressionar o botão A o mais rápido possível após o LED acender.
 * 3. Se o jogador pressionar o botão antes do LED acender, é contabilizado como uma "partida falsa".
 * 4. Se o jogador não pressionar o botão dentro de 5 segundos após o LED acender, é contabilizado como "tempo esgotado".
 * 5. O jogo pode ser interrompido a qualquer momento pressionando o botão B.
 * 6. Ao final, o programa exibe estatísticas, incluindo o número de partidas falsas, rodadas bem-sucedidas
 *    e o tempo médio de reação.
 * 
 * O programa utiliza GPIO para controlar o LED e os botões, e ANSI escape codes para colorir a saída no terminal.
 */

#include "inc/ButtonPi.h"
#include "inc/gpio_irq_manager.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/******************************
 * Definições de pinos e constantes
 ******************************/

/**
 * @brief Pino GPIO do LED.
 */
#define LED_PIN 11

/**
 * @brief Tempo máximo (em milissegundos) para o jogador reagir após o LED acender.
 */
#define TIMEOUT_MS 5000

/******************************
 * Códigos de cores para o terminal (ANSI escape codes)
 ******************************/

/**
 * @brief Código para resetar a cor do terminal.
 */
#define COLOR_RESET "\033[0m"

/**
 * @brief Código para definir a cor do texto como vermelho.
 */
#define COLOR_RED "\033[31m"

/**
 * @brief Código para definir a cor do texto como verde.
 */
#define COLOR_GREEN "\033[32m"

/**
 * @brief Código para definir a cor do texto como amarelo.
 */
#define COLOR_YELLOW "\033[33m"

/******************************
 * Variáveis globais
 ******************************/

/**
 * @brief Tempo inicial de uma rodada.
 */
absolute_time_t start_time;

/**
 * @brief Indica se o jogo está ativo (LED aceso e aguardando reação).
 */
bool game_active = false;

/**
 * @brief Indica se o LED está aceso.
 */
bool led_on = false;

/**
 * @brief Indica se o jogo está em execução.
 */
bool game_running = true;

/**
 * @brief Número da rodada atual.
 */
int round_number = 0;

/**
 * @brief Contador de partidas falsas (quando o jogador pressiona o botão antes do LED acender).
 */
int false_starts = 0;

/**
 * @brief Soma total dos tempos de reação em todas as rodadas bem-sucedidas.
 */
int64_t total_reaction_time = 0;

/**
 * @brief Contador de rodadas bem-sucedidas.
 */
int successful_rounds = 0;

/******************************
 * Funções
 ******************************/

/**
 * @brief Imprime o cabeçalho da tabela de resultados.
 */
void print_table_header() {
    printf("+------------+-----------------+---------------------+----------------------+\n");
    printf("| Rodada     | Tempo de Reação | Status              | Ação                 |\n");
    printf("+------------+-----------------+---------------------+----------------------+\n");
}

/**
 * @brief Imprime uma linha da tabela de resultados.
 * @param round Número da rodada.
 * @param reaction_time Tempo de reação do jogador (em milissegundos).
 * @param status Status da rodada (Sucesso, Falha, etc.).
 * @param action Ação realizada pelo jogador.
 * @param color Cor do texto (usando ANSI escape codes).
 */
void print_table_row(int round, int64_t reaction_time, const char* status, const char* action, const char* color) {
    printf("| %-10d | %-15lld | %s%-20s%s | %s%-20s%s |\n", 
           round, reaction_time, color, status, COLOR_RESET, color, action, COLOR_RESET);
}

/**
 * @brief Imprime as estatísticas finais do jogo.
 */
void print_statistics() {
    printf("\n+---------------------+----------------------+\n");
    printf("| Estatísticas        | Valor                |\n");
    printf("+---------------------+----------------------+\n");
    printf("| Partidas falsas     | %-20d |\n", false_starts);
    printf("| Rodadas bem-sucedidas | %-20d |\n", successful_rounds);
    if (successful_rounds > 0) {
        int64_t average_reaction_time = total_reaction_time / successful_rounds;
        printf("| Média de reação     | %-20lld ms |\n", average_reaction_time);
    } else {
        printf("| Média de reação     | %-20s |\n", "N/A");
    }
    printf("+---------------------+----------------------+\n");
}

/**
 * @brief Callback para o botão A. Verifica se o jogador reagiu corretamente.
 */
void button_a_pressed() {
    if (!game_active && !led_on) {
        // Partida falsa: jogador pressionou o botão antes do LED acender
        print_table_row(round_number, 0, "Falha", "Partida falsa", COLOR_YELLOW);
        false_starts++; 
        round_number++; 
    } else if (game_active) {
        // Jogador reagiu corretamente
        absolute_time_t end_time = get_absolute_time();
        int64_t reaction_time = absolute_time_diff_us(start_time, end_time) / 1000;
        print_table_row(round_number, reaction_time, "Sucesso", "Botão pressionado", COLOR_GREEN);
        total_reaction_time += reaction_time; 
        successful_rounds++; 
        game_active = false;
        led_on = false;
        gpio_put(LED_PIN, false); 
        round_number++; 
    }
}

/**
 * @brief Callback para o botão B. Interrompe o jogo.
 */
void button_b_pressed() {
    game_running = false;
    game_active = false; // Garante que o jogo pare imediatamente
    led_on = false;
    gpio_put(LED_PIN, false); // Desliga o LED
    printf("\nJogo interrompido pelo usuário.\n");
}

/**
 * @brief Verifica se o tempo de reação do jogador expirou.
 */
void check_timeout() {
    while (game_active && game_running) {
        absolute_time_t now = get_absolute_time();
        int64_t elapsed_time = absolute_time_diff_us(start_time, now) / 1000;

        if (elapsed_time > TIMEOUT_MS) {
            // Tempo esgotado: jogador não reagiu a tempo
            print_table_row(round_number, TIMEOUT_MS, "Falha", "Tempo esgotado", COLOR_RED);
            game_active = false;
            led_on = false;
            gpio_put(LED_PIN, false); 
            round_number++; 
            break;
        }

        sleep_ms(10); // Pequena pausa para evitar uso excessivo da CPU
    }
}

/******************************
 * Função principal
 ******************************/

/**
 * @brief Função principal do jogo.
 * @return Retorna 0 ao finalizar a execução.
 */
int main() {
    stdio_init_all(); // Inicializa a comunicação serial (para debug)

    sleep_ms(1000); // Aguarda 1 segundo para estabilização

    print_table_header(); // Imprime o cabeçalho da tabela de resultados

    ButtonPi button_a;
    ButtonPi button_b;

    // Inicializa os botões
    ButtonPi_init(&button_a, BUTTON_A_PIN); 
    ButtonPi_init(&button_b, BUTTON_B_PIN); 

    gpio_init(LED_PIN); // Inicializa o pino do LED
    gpio_set_dir(LED_PIN, GPIO_OUT); // Configura o pino do LED como saída

    // Registra os callbacks dos botões
    ButtonPi_attach_callback(&button_a, button_a_pressed);
    ButtonPi_attach_callback(&button_b, button_b_pressed);

    srand(time(NULL)); // Inicializa a semente para números aleatórios

    while (game_running) {
        if (!game_active) {
            sleep_ms(rand() % 3000 + 1000); // Intervalo aleatório antes de acender o LED
            gpio_put(LED_PIN, true); // Acende o LED
            led_on = true;
            start_time = get_absolute_time(); // Registra o tempo inicial
            game_active = true;

            check_timeout(); // Verifica se o tempo de reação expirou
        }
        sleep_ms(10); // Pequena pausa para evitar uso excessivo da CPU
    }

    print_statistics(); // Imprime as estatísticas finais do jogo

    return 0;
}
