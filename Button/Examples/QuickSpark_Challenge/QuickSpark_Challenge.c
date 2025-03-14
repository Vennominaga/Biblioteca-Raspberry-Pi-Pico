/*-------------DOCUMENTAÇÃO ---------------*/


#include "inc/ButtonPi.h"
#include "inc/gpio_irq_manager.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LED_PIN 11
#define TIMEOUT_MS 5000 

// Códigos de cores para o terminal (ANSI escape codes)
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"

absolute_time_t start_time;
bool game_active = false;
bool led_on = false; 
bool game_running = true; 
int round_number = 0;
int false_starts = 0; 
int64_t total_reaction_time = 0; 
int successful_rounds = 0; 

void print_table_header() 
{
    printf("+------------+-----------------+---------------------+----------------------+\n");
    printf("| Rodada     | Tempo de Reação | Status              | Ação                 |\n");
    printf("+------------+-----------------+---------------------+----------------------+\n");
}


void print_table_row(int round, int64_t reaction_time, const char* status, const char* action, const char* color) {
    printf("| %-10d | %-15lld | %s%-20s%s | %s%-20s%s |\n", 
           round, reaction_time, color, status, COLOR_RESET, color, action, COLOR_RESET);
}


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

void button_a_pressed() 
{
    if (!game_active && !led_on) {
      
        print_table_row(round_number, 0, "Falha", "Partida falsa", COLOR_YELLOW);
        false_starts++; 
        round_number++; 
    } else if (game_active) 
    {
        
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

void button_b_pressed() 
{
    
    game_running = false;
    game_active = false; // Garante que o jogo pare imediatamente
    led_on = false;
    gpio_put(LED_PIN, false); // Desliga o LED
    printf("\nJogo interrompido pelo usuário.\n");
}

void check_timeout() 
{
    while (game_active && game_running) 
    {
        absolute_time_t now = get_absolute_time();
        int64_t elapsed_time = absolute_time_diff_us(start_time, now) / 1000;

        
        if (elapsed_time > TIMEOUT_MS) 
        {
            print_table_row(round_number, TIMEOUT_MS, "Falha", "Tempo esgotado", COLOR_RED);
            game_active = false;
            led_on = false;
            gpio_put(LED_PIN, false); 
            round_number++; 
            break;
        }

        sleep_ms(10); 
    }
}

int main() {
    stdio_init_all(); 

   
    sleep_ms(1000);

    
    print_table_header();

    ButtonPi button_a;
    ButtonPi button_b;

    
    ButtonPi_init(&button_a, BUTTON_A_PIN); 
    ButtonPi_init(&button_b, BUTTON_B_PIN); 

    gpio_init(LED_PIN); 
    gpio_set_dir(LED_PIN, GPIO_OUT); 

   
    ButtonPi_attach_callback(&button_a, button_a_pressed);
    ButtonPi_attach_callback(&button_b, button_b_pressed);

    srand(time(NULL)); 

    while (game_running) 
    {
        if (!game_active) 
        {
            sleep_ms(rand() % 3000 + 1000); 
            gpio_put(LED_PIN, true); 
            led_on = true;
            start_time = get_absolute_time();
            game_active = true;

            
            check_timeout();
        }
        sleep_ms(10);
    }

   
    print_statistics();

    return 0;
}
