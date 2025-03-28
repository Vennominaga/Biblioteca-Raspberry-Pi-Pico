/**
 * @file Genius_2.c
 * @brief Implementação do jogo Genius (Simon Says) para Raspberry Pi Pico
 * 
 * Este código implementa um jogo de memória de sequência de cores usando:
 * - Matriz LED 5x5 RGB para exibição
 * - Joystick para seleção de cores
 * - Dois botões para confirmação e controle
 * - Buzzer para feedback sonoro
 * - Display OLED SSD1306 para informações do jogo
 * - Watchdog para reinício por inatividade
 * 
 * O jogo gera sequências aleatórias de cores que o jogador deve repetir corretamente.
 * A cada rodada bem-sucedida, a sequência aumenta em dificuldade.
 */

#include "inc/JoystickPi.h"
#include "inc/ButtonPi.h"
#include "inc/BuzzerPi.h"
#include "inc/MatrizRGBPI.h"
#include "inc/ssd1306.h"
#include "inc/ssd1306_fonts.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/watchdog.h"

/******************************
 * Definições de pinos
 ******************************/
#define BUTTON_A_PIN 5  // Pino do botão A (confirmação)
#define BUTTON_B_PIN 6  // Pino do botão B (alternativo)

/******************************
 * Definições de cores
 ******************************/
#define GREEN 0    // Índice para cor verde
#define BLUE 1     // Índice para cor azul
#define RED 2      // Índice para cor vermelha

// Definições de cores RGB para a matriz LED
#define COLOR_WHITE 20, 20, 20
#define COLOR_GREEN 0, 255, 0
#define COLOR_BLUE 0, 0, 255
#define COLOR_RED 255, 0, 0
#define COLOR_OFF 0, 0, 0

/******************************
 * Estados do jogo
 ******************************/
typedef enum {
    STATE_SHOW_SEQUENCE, // Exibindo a sequência de cores
    STATE_WAIT_INPUT,    // Aguardando entrada do jogador
    STATE_CHECK_INPUT,   // Verificando a entrada
    STATE_GAME_OVER,     // Fim de jogo (erro do jogador)
    STATE_ROUND_COMPLETE // Rodada completada com sucesso
} GameState;

/******************************
 * Variáveis globais
 ******************************/
int sequence[100];             // Armazena a sequência de cores
int sequence_length = 1;       // Tamanho atual da sequência
int current_step = 0;          // Passo atual na sequência
GameState game_state = STATE_SHOW_SEQUENCE; // Estado inicial do jogo
int selected_color = GREEN;    // Cor selecionada pelo jogador
bool button_b_pressed = false; // Flag para botão B pressionado
int round_number = 1;          // Número da rodada atual
bool reset_requested = false;   // Flag para reiniciar o jogo
bool check_input_requested = false; // Flag para verificar entrada
bool game_over_shown = false;  // Flag para controle de exibição de Game Over
absolute_time_t last_activity_time; // Tempo da última atividade
const uint32_t INACTIVITY_TIMEOUT_MS = 30000; // 30 segundos de timeout

/******************************
 * Protótipos de funções
 ******************************/
void update_activity_time();
bool inactivity_timeout();
void init_display();
void update_display();
void show_game_over_display();
void show_timeout_display();
void play_color_sound(int color);
void light_up_matrix(int color);
void show_white_matrix();
void show_round_message();
void generate_sequence();
void show_sequence();
void check_choice();
void reset_game();
void button_a_callback();
void button_b_callback();

/**
 * @brief Atualiza o tempo da última atividade do usuário
 */
void update_activity_time() {
    last_activity_time = get_absolute_time();
}

/**
 * @brief Verifica se o tempo de inatividade foi excedido
 * @return true se o timeout foi atingido, false caso contrário
 */
bool inactivity_timeout() {
    return absolute_time_diff_us(last_activity_time, get_absolute_time()) > INACTIVITY_TIMEOUT_MS * 1000;
}

/**
 * @brief Inicializa o display OLED com mensagem inicial
 */
void init_display() {
    ssd1306_Init();
    ssd1306_Fill(Black);
    ssd1306_SetCursor(10, 10);
    ssd1306_WriteString("Color Sequence", Font_7x10, White);
    ssd1306_SetCursor(10, 25);
    ssd1306_WriteString("Game", Font_7x10, White);
    ssd1306_UpdateScreen();
    sleep_ms(2000);
}

/**
 * @brief Atualiza o display com informações da rodada atual
 */
void update_display() {
    char round_str[20];
    snprintf(round_str, sizeof(round_str), "Round: %d", round_number);
    
    ssd1306_Fill(Black);
    ssd1306_SetCursor(10, 10);
    ssd1306_WriteString("Playing...", Font_7x10, White);
    ssd1306_SetCursor(10, 25);
    ssd1306_WriteString(round_str, Font_7x10, White);
    ssd1306_UpdateScreen();
}

/**
 * @brief Exibe tela de Game Over no display
 */
void show_game_over_display() {
    ssd1306_Fill(Black);
    ssd1306_SetCursor(10, 10);
    ssd1306_WriteString("Game Over!", Font_7x10, White);
    ssd1306_SetCursor(10, 25);
    ssd1306_WriteString("Press to restart", Font_7x10, White);
    ssd1306_UpdateScreen();
}

/**
 * @brief Exibe mensagem de timeout no display
 */
void show_timeout_display() {
    ssd1306_Fill(Black);
    ssd1306_SetCursor(10, 10);
    ssd1306_WriteString("Timeout!", Font_7x10, White);
    ssd1306_SetCursor(10, 25);
    ssd1306_WriteString("Restarting...", Font_7x10, White);
    ssd1306_UpdateScreen();
    sleep_ms(2000);
}

/**
 * @brief Toca um som correspondente à cor selecionada
 * @param color Cor selecionada (GREEN, BLUE ou RED)
 */
void play_color_sound(int color) {
    switch (color) {
        case GREEN: play_tone(BUZZER_PIN, 523, 200); break; // Nota C5
        case BLUE:  play_tone(BUZZER_PIN, 659, 200); break; // Nota E5
        case RED:   play_tone(BUZZER_PIN, 440, 200); break; // Nota A4
    }
}

/**
 * @brief Acende a matriz LED com a cor especificada no centro (3x3)
 * @param color Cor a ser exibida (GREEN, BLUE ou RED)
 */
void light_up_matrix(int color) {
    MatrizRGBPI_Clear();
    
    uint8_t r, g, b;
    switch (color) {
        case GREEN: r = 0; g = 255; b = 0; break;
        case BLUE:  r = 0; g = 0; b = 255; break;
        case RED:   r = 255; g = 0; b = 0; break;
        default:    r = 0; g = 0; b = 0; break;
    }
    
    // Acende o centro 3x3 da matriz
    for (int x = 1; x < 4; x++) {
        for (int y = 1; y < 4; y++) {
            int pos = getIndex(x, y);
            MatrizRGBPI_SetLED(pos, r, g, b);
        }
    }
    
    MatrizRGBPI_Write();
}

/**
 * @brief Mostra toda a matriz LED branca
 */
void show_white_matrix() {
    MatrizRGBPI_Clear();
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            int pos = getIndex(x, y);
            MatrizRGBPI_SetLED(pos, COLOR_WHITE);
        }
    }
    MatrizRGBPI_Write();
}

/**
 * @brief Exibe mensagem da rodada atual com scroll na matriz LED
 */
void show_round_message() {
    char round_msg[20];
    snprintf(round_msg, sizeof(round_msg), "Round %d", round_number);
    MatrizRGBPI_displayStringWithScroll(round_msg, 150, COLOR_GREEN);
}

/**
 * @brief Gera sequência aleatória de cores
 */
void generate_sequence() {
    for (int i = 0; i < sequence_length; i++) {
        sequence[i] = rand() % 3; // Gera valores 0, 1 ou 2
    }
}

/**
 * @brief Exibe a sequência de cores para o jogador memorizar
 */
void show_sequence() {
    for (int i = 0; i < sequence_length; i++) {
        light_up_matrix(sequence[i]);
        play_color_sound(sequence[i]);
        sleep_ms(500);
        MatrizRGBPI_Clear();
        MatrizRGBPI_Write();
        sleep_ms(200);
    }
    game_state = STATE_WAIT_INPUT; // Muda para estado de espera por input
}

/**
 * @brief Verifica se a cor selecionada pelo jogador está correta
 */
void check_choice() {
    if (selected_color == sequence[current_step]) {
        play_color_sound(selected_color);
        current_step++;
        
        if (current_step == sequence_length) {
            game_state = STATE_ROUND_COMPLETE; // Rodada completa
        } else {
            game_state = STATE_WAIT_INPUT; // Continua a rodada
        }
    } else {
        play_tone(BUZZER_PIN, 220, 1000); // Som de erro
        game_state = STATE_GAME_OVER;
        game_over_shown = false;
    }
}

/**
 * @brief Reinicia o jogo com valores iniciais
 */
void reset_game() {
    sequence_length = 1;
    current_step = 0;
    round_number = 1;
    MatrizRGBPI_Clear();
    MatrizRGBPI_Write();
    generate_sequence();
    game_state = STATE_SHOW_SEQUENCE;
    reset_requested = false;
    update_display();
    update_activity_time();
}

/**
 * @brief Callback para o botão A (confirmação)
 */
void button_a_callback() {
    if (game_state == STATE_WAIT_INPUT) {
        check_input_requested = true;
    }
    else if (game_state == STATE_GAME_OVER) {
        reset_requested = true;
    }
    update_activity_time();
}

/**
 * @brief Callback para o botão B (alternativo)
 */
void button_b_callback() {
    if (game_state == STATE_WAIT_INPUT) {
        button_b_pressed = true;
    }
    else if (game_state == STATE_GAME_OVER) {
        reset_requested = true;
    }
    update_activity_time();
}

/**
 * @brief Função principal do jogo
 * @return int Não retorna (loop infinito)
 */
int main() {
    // Inicializa hardware e periféricos
    stdio_init_all();
    srand(time(NULL));

    // Configura watchdog para reinício após inatividade
    watchdog_enable(INACTIVITY_TIMEOUT_MS + 2000, 1);

    // Inicializa componentes
    init_display();
    joystickPi_init();
    MatrizRGBPI_Init(LED_PIN);
    
    // Configura botões
    ButtonPi button_a, button_b;
    ButtonPi_init(&button_a, BUTTON_A_PIN);
    ButtonPi_init(&button_b, BUTTON_B_PIN);
    ButtonPi_attach_callback(&button_a, button_a_callback);
    ButtonPi_attach_callback(&button_b, button_b_callback);
    
    // Inicializa buzzer
    initialize_pwm(BUZZER_PIN);
    
    // Configura tempo inicial
    update_activity_time();
    
    // Gera e mostra primeira sequência
    generate_sequence();
    show_sequence();
    update_display();

    // Loop principal do jogo
    while (1) {
        // Verifica inatividade
        if (inactivity_timeout()) {
            reset_game();
            watchdog_update();
            show_timeout_display();
            continue;
        }

        // Processa reinício
        if (reset_requested) {
            reset_game();
            sleep_ms(100);
            watchdog_update();
            continue;
        }

        // Processa verificação de input
        if (check_input_requested) {
            check_input_requested = false;
            game_state = STATE_CHECK_INPUT;
        }

        // Máquina de estados do jogo
        switch (game_state) {
            case STATE_SHOW_SEQUENCE:
                show_sequence();
                update_display();
                break;

            case STATE_WAIT_INPUT:
                // Lê joystick para seleção de cor
                joystick_state_t state = joystickPi_read();
                if (state.x < 1365) {
                    selected_color = GREEN;
                } else if (state.x < 2730) {
                    selected_color = BLUE;
                } else {
                    selected_color = RED;
                }

                light_up_matrix(selected_color);

                if (button_b_pressed) {
                    button_b_pressed = false;
                    game_state = STATE_CHECK_INPUT;
                }
                break;

            case STATE_CHECK_INPUT:
                check_choice();
                break;

            case STATE_ROUND_COMPLETE:
                show_round_message();
                show_white_matrix();
                sleep_ms(1000);
                
                // Prepara próxima rodada
                sequence_length++;
                current_step = 0;
                round_number++;
                game_state = STATE_SHOW_SEQUENCE;
                generate_sequence();
                update_display();
                break;

            case STATE_GAME_OVER:
                if (!game_over_shown) {
                    MatrizRGBPI_displayStringWithScroll("Game Over", 150, COLOR_RED);
                    show_game_over_display();
                    game_over_shown = true;
                }
                break;
        }

        // Mantém watchdog atualizado
        watchdog_update();
        sleep_ms(50);
    }

    return 0;
}
