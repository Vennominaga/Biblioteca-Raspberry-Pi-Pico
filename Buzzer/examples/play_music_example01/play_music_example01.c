#include <stdio.h>
#include "pico/stdlib.h"
#include "inc/BuzzerPi.h"
#include "inc/melody.h"

/******************************
 * Documentação do Programa
 ******************************/

/**
 * @file main.c
 * @brief Programa para tocar melodias em um buzzer no Raspberry Pi Pico
 * 
 * Este programa utiliza a biblioteca `BuzzerPi` para tocar melodias em um buzzer conectado ao
 * Raspberry Pi Pico. Ele toca três melodias conhecidas em sequência, com um intervalo de 1 segundo
 * entre cada uma. As melodias são definidas em arrays de frequências e durações, armazenados no
 * arquivo `melody.h`.
 * 
 * Funcionalidades:
 * 1. Inicializa o PWM para controle do buzzer.
 * 2. Reproduz as melodias "Pirates of the Caribbean", "Marcha Imperial" e "Für Elise".
 * 3. Repete a sequência de melodias indefinidamente.
 */

/******************************
 * Função Principal
 ******************************/

/**
 * @brief Função principal do programa.
 * 
 * Inicializa o sistema, configura o buzzer e toca as melodias em sequência, repetindo o ciclo
 * indefinidamente.
 * 
 * @return Retorna 0 ao finalizar a execução (nunca ocorre, pois o programa entra em um loop infinito).
 */
int main() {
    stdio_init_all(); // Inicializa a comunicação serial (para debug, se necessário)

    initialize_pwm(BUZZER_PIN); // Inicializa o PWM no pino do buzzer

    // Loop principal do programa
    while (true) {
        // Toca a melodia "Pirates of the Caribbean"
        play_melody(BUZZER_PIN, PiratesCaribeanMelody, PiratesCaribeanDurations, 125.0f, sizeof(PiratesCaribeanMelody) / sizeof(PiratesCaribeanMelody[0]));
        sleep_ms(1000); // Intervalo de 1 segundo

        // Toca a melodia "Marcha Imperial"
        play_melody(BUZZER_PIN, MarchImperialMelody, MarchImperialDurations, 125.0f, sizeof(MarchImperialMelody) / sizeof(MarchImperialMelody[0]));
        sleep_ms(1000); // Intervalo de 1 segundo

        // Toca a melodia "Für Elise"
        play_melody(BUZZER_PIN, ForEliseMelody, ForEliseDurations, 125.0f, sizeof(ForEliseMelody) / sizeof(ForEliseMelody[0]));
        sleep_ms(1000); // Intervalo de 1 segundo
    }

    return 0; // Nunca alcançado, pois o programa está em um loop infinito
}
