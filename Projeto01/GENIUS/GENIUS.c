#include "inc/JoystickPi.h"
#include "inc/ButtonPi.h"
#include "inc/BuzzerPi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/******************************
 * Definições de pinos
 ******************************/

/**
 * @brief Pino GPIO do LED verde.
 */
#define LED_GREEN_PIN 11

/**
 * @brief Pino GPIO do LED azul.
 */
#define LED_BLUE_PIN 12

/**
 * @brief Pino GPIO do LED vermelho.
 */
#define LED_RED_PIN 13

/**
 * @brief Pino GPIO do botão A.
 */
#define BUTTON_A_PIN 5

/**
 * @brief Pino GPIO do botão B.
 */
#define BUTTON_B_PIN 6

/******************************
 * Definições de cores
 ******************************/

/**
 * @brief Representa a cor verde.
 */
#define GREEN 0

/**
 * @brief Representa a cor azul.
 */
#define BLUE 1

/**
 * @brief Representa a cor vermelha.
 */
#define RED 2

/******************************
 * Estados do jogo
 ******************************/

/**
 * @brief Enumeração que define os possíveis estados do jogo.
 */
typedef enum {
    STATE_SHOW_SEQUENCE, // Exibindo a sequência de cores
    STATE_WAIT_INPUT,    // Aguardando a entrada do jogador
    STATE_CHECK_INPUT,   // Verificando a entrada do jogador
    STATE_GAME_OVER      // Fim de jogo
} GameState;

/******************************
 * Variáveis globais
 ******************************/

/**
 * @brief Array que armazena a sequência de cores gerada.
 */
int sequence[100];

/**
 * @brief Comprimento atual da sequência de cores.
 */
int sequence_length = 1;

/**
 * @brief Passo atual na sequência que o jogador está tentando reproduzir.
 */
int current_step = 0;

/**
 * @brief Estado atual do jogo.
 */
GameState game_state = STATE_SHOW_SEQUENCE;

/**
 * @brief Cor selecionada pelo jogador.
 */
int selected_color = GREEN;

/**
 * @brief Flag que indica se o botão B foi pressionado.
 */
bool button_b_pressed = false;

/******************************
 * Funções
 ******************************/

/**
 * @brief Toca um som correspondente à cor selecionada.
 * @param color A cor selecionada (GREEN, BLUE, ou RED).
 */
void play_color_sound(int color) {
    switch (color) {
        case GREEN:
            play_tone(BUZZER_PIN, 523, 200); // Toca um som para a cor verde
            break;
        case BLUE:
            play_tone(BUZZER_PIN, 659, 200); // Toca um som para a cor azul
            break;
        case RED:
            play_tone(BUZZER_PIN, 440, 200); // Toca um som para a cor vermelha
            break;
    }
}

/**
 * @brief Acende o LED correspondente à cor selecionada.
 * @param color A cor selecionada (GREEN, BLUE, ou RED).
 */
void light_up_led(int color) {
    gpio_put(LED_GREEN_PIN, color == GREEN);
    gpio_put(LED_BLUE_PIN, color == BLUE);
    gpio_put(LED_RED_PIN, color == RED);
}

/**
 * @brief Gera uma sequência aleatória de cores.
 */
void generate_sequence() {
    for (int i = 0; i < sequence_length; i++) {
        sequence[i] = rand() % 3; // Gera um número aleatório entre 0 e 2 (GREEN, BLUE, RED)
    }
}

/**
 * @brief Exibe a sequência de cores gerada.
 */
void show_sequence() {
    for (int i = 0; i < sequence_length; i++) {
        printf("Cor: %d\n", sequence[i]);
        light_up_led(sequence[i]); // Acende o LED correspondente
        play_color_sound(sequence[i]); // Toca o som correspondente
        sleep_ms(500); // Mantém o LED aceso por 500ms
        light_up_led(-1); // Apaga o LED
        sleep_ms(200); // Intervalo entre as cores
    }
    game_state = STATE_WAIT_INPUT; // Muda o estado para aguardar a entrada do jogador
}

/**
 * @brief Verifica se a cor selecionada pelo jogador está correta.
 */
void check_choice() {
    if (selected_color == sequence[current_step]) {
        printf("Cor correta!\n");
        play_color_sound(selected_color); // Toca o som da cor correta
        current_step++;
        if (current_step == sequence_length) {
            printf("Sequência completa! Próximo nível...\n");
            sequence_length++; // Aumenta o comprimento da sequência
            current_step = 0;
            game_state = STATE_SHOW_SEQUENCE; // Reinicia o ciclo
        } else {
            game_state = STATE_WAIT_INPUT; // Continua aguardando a próxima entrada
        }
    } else {
        printf("Erro! Fim de jogo.\n");
        play_tone(BUZZER_PIN, 220, 1000); // Toca um som de erro
        game_state = STATE_GAME_OVER; // Muda o estado para fim de jogo
    }
}

/**
 * @brief Reinicia o jogo, gerando uma nova sequência e resetando as variáveis.
 */
void reset_game() {
    sequence_length = 1; // Reinicia o comprimento da sequência
    current_step = 0; // Reinicia o passo atual
    game_state = STATE_SHOW_SEQUENCE; // Muda o estado para exibir a sequência
    generate_sequence(); // Gera uma nova sequência
    show_sequence(); // Exibe a nova sequência
}

/**
 * @brief Callback para o botão A. Muda o estado do jogo para verificar a entrada.
 */
void button_a_callback() {
    if (game_state == STATE_WAIT_INPUT) {
        game_state = STATE_CHECK_INPUT; // Muda o estado para verificar a entrada
    }
}

/**
 * @brief Callback para o botão B. Sinaliza que o botão B foi pressionado.
 */
void button_b_callback() {
    if (game_state == STATE_WAIT_INPUT) {
        button_b_pressed = true; // Sinaliza que o botão B foi pressionado
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
    // Inicializa a comunicação serial (para debug)
    stdio_init_all();

    // Inicializa a semente para números aleatórios
    srand(time(NULL));

    // Inicializa o joystick
    joystickPi_init();

    // Inicializa os LEDs
    gpio_init(LED_GREEN_PIN);
    gpio_init(LED_BLUE_PIN);
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);

    // Inicializa os botões
    ButtonPi button_a, button_b;
    ButtonPi_init(&button_a, BUTTON_A_PIN);
    ButtonPi_init(&button_b, BUTTON_B_PIN);

    // Registra os callbacks dos botões
    ButtonPi_attach_callback(&button_a, button_a_callback);
    ButtonPi_attach_callback(&button_b, button_b_callback);

    // Inicializa o buzzer
    initialize_pwm(BUZZER_PIN);

    // Gera a primeira sequência
    generate_sequence();
    show_sequence();

    // Loop principal do jogo
    while (1) {
        switch (game_state) {
            case STATE_SHOW_SEQUENCE:
                generate_sequence();
                show_sequence();
                break;

            case STATE_WAIT_INPUT:
                // Lê o estado do joystick para selecionar a cor
                joystick_state_t state = joystickPi_read();
                if (state.x < 1365) {
                    selected_color = GREEN;
                } else if (state.x < 2730) {
                    selected_color = BLUE;
                } else {
                    selected_color = RED;
                }

                // Acende o LED correspondente à cor selecionada
                light_up_led(selected_color);

                // Verifica se o botão B foi pressionado
                if (button_b_pressed) {
                    button_b_pressed = false;
                    game_state = STATE_CHECK_INPUT;
                }
                break;

            case STATE_CHECK_INPUT:
                check_choice();
                break;

            case STATE_GAME_OVER:
                printf("Pressione o botão A para reiniciar.\n");
                
                // Verifica se o botão A foi pressionado para reiniciar o jogo
                if (ButtonPi_read(&button_a)) {
                    reset_game();
                }
                break;
        }

        // Pequena pausa para evitar leituras excessivas
        sleep_ms(50);
    }

    return 0;
}
