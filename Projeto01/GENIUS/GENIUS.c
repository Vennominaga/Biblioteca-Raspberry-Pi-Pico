#include "inc/JoystickPi.h"
#include "inc/ButtonPi.h"
#include "inc/BuzzerPi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição dos pinos
#define LED_GREEN_PIN 11
#define LED_BLUE_PIN 12
#define LED_RED_PIN 13
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

// Definição das cores
#define GREEN 0
#define BLUE 1
#define RED 2

// Estados do jogo
typedef enum {
    STATE_SHOW_SEQUENCE, // Exibindo a sequência
    STATE_WAIT_INPUT,    // Aguardando a entrada do jogador
    STATE_CHECK_INPUT,   // Verificando a entrada do jogador
    STATE_GAME_OVER      // Fim de jogo
} GameState;


int sequence[100]; 
int sequence_length = 1; 
int current_step = 0; 
GameState game_state = STATE_SHOW_SEQUENCE; 
int selected_color = GREEN; 
bool button_b_pressed = false; 


void play_color_sound(int color) {
    switch (color) {
        case GREEN:
            play_tone(BUZZER_PIN, 523, 200); 
            break;
        case BLUE:
            play_tone(BUZZER_PIN, 659, 200); 
            break;
        case RED:
            play_tone(BUZZER_PIN, 440, 200); 
            break;
    }
}


void light_up_led(int color) {
    gpio_put(LED_GREEN_PIN, color == GREEN);
    gpio_put(LED_BLUE_PIN, color == BLUE);
    gpio_put(LED_RED_PIN, color == RED);
}

void generate_sequence() {
    for (int i = 0; i < sequence_length; i++) {

        sequence[i] = rand() % 3; 
        
    }
}

void show_sequence() 
{
    for (int i = 0; i < sequence_length; i++) {
        printf("Cor: %d\n", sequence[i]);
        light_up_led(sequence[i]);
        play_color_sound(sequence[i]);
        sleep_ms(500); 
        light_up_led(-1); 
        sleep_ms(200); 
    }
    game_state = STATE_WAIT_INPUT; 
}


void check_choice() {
    if (selected_color == sequence[current_step]) {
        printf("Cor correta!\n");
        play_color_sound(selected_color);
        current_step++;
        if (current_step == sequence_length) {
            printf("Sequência completa! Próximo nível...\n");
            sequence_length++;
            current_step = 0;
            game_state = STATE_SHOW_SEQUENCE; 
        } else {
            game_state = STATE_WAIT_INPUT; 
        }
    } else {
        printf("Erro! Fim de jogo.\n");
        play_tone(BUZZER_PIN, 220, 1000); 
        game_state = STATE_GAME_OVER; 
    }
}

void reset_game() {
    sequence_length = 1; 
    current_step = 0; 
    game_state = STATE_SHOW_SEQUENCE;
    generate_sequence(); 
    show_sequence(); 
}

void button_a_callback() {
    if (game_state == STATE_WAIT_INPUT) {
        game_state = STATE_CHECK_INPUT; 
    }
}


void button_b_callback() {
    if (game_state == STATE_WAIT_INPUT) {
        button_b_pressed = true; 
    }
}

int main() {
    stdio_init_all();

    
    srand(time(NULL));

   
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

    while (1) {
        switch (game_state) {
            case STATE_SHOW_SEQUENCE:
                generate_sequence();
                show_sequence();
                break;

            case STATE_WAIT_INPUT:
                
                joystick_state_t state = joystickPi_read();
                if (state.x < 1365) {
                    selected_color = GREEN;
                } else if (state.x < 2730) {
                    selected_color = BLUE;
                } else {
                    selected_color = RED;
                }

               
                light_up_led(selected_color);

                
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
                
                if (ButtonPi_read(&button_a)) {
                    reset_game();
                }
                break;
        }

    
        sleep_ms(50);
    }

    return 0;
}