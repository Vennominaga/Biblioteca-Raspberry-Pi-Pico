#include "pico/stdlib.h"
#include "inc/ButtonPi.h"
#include <stdio.h>

/******************************
 * Documentação do Programa
 ******************************/

/**
 * @file main.c
 * @brief Controle de LEDs com Botões
 * 
 * Este programa controla dois LEDs (um vermelho e um azul) usando dois botões. Cada botão está associado
 * a um LED específico:
 * 1. O botão no pino 5 controla o LED vermelho (pino 13).
 * 2. O botão no pino 6 controla o LED azul (pino 12).
 * 
 * Funcionalidades:
 * 1. Quando o botão 5 é pressionado, o LED azul é apagado e o LED vermelho é alternado (aceso/apagado).
 * 2. Quando o botão 6 é pressionado, o LED vermelho é apagado e o LED azul é alternado (aceso/apagado).
 * 3. Mensagens são exibidas no terminal indicando qual botão foi pressionado e o estado dos LEDs.
 * 
 * O programa utiliza a biblioteca ButtonPi para gerenciar os botões e callbacks para tratar os eventos
 * de pressionamento.
 */

/******************************
 * Funções de Callback
 ******************************/

/**
 * @brief Callback para o botão no pino 5.
 * 
 * Apaga o LED azul (pino 12) e alterna o estado do LED vermelho (pino 13).
 * Exibe uma mensagem no terminal indicando que o botão foi pressionado.
 */
void button_5_callback(void) {
    gpio_put(12, 0); // Apaga o LED azul
    gpio_put(13, !gpio_get(13)); // Alterna o estado do LED vermelho
    printf("Botão no pino 5 pressionado! LED vermelho aceso.\n");
}

/**
 * @brief Callback para o botão no pino 6.
 * 
 * Apaga o LED vermelho (pino 13) e alterna o estado do LED azul (pino 12).
 * Exibe uma mensagem no terminal indicando que o botão foi pressionado.
 */
void button_6_callback(void) {
    gpio_put(13, 0); // Apaga o LED vermelho
    gpio_put(12, !gpio_get(12)); // Alterna o estado do LED azul
    printf("Botão no pino 6 pressionado! LED azul aceso.\n");
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

    // Inicializa os pinos dos LEDs
    gpio_init(13); // LED vermelho
    gpio_init(12); // LED azul
    gpio_set_dir(13, GPIO_OUT); // Configura o pino 13 como saída
    gpio_set_dir(12, GPIO_OUT); // Configura o pino 12 como saída

    // Inicializa os botões
    ButtonPi btn5;
    ButtonPi btn6;

    ButtonPi_init(&btn5, 5); // Inicializa o botão no pino 5
    ButtonPi_init(&btn6, 6); // Inicializa o botão no pino 6

    // Registra os callbacks dos botões
    ButtonPi_attach_callback(&btn5, button_5_callback);
    ButtonPi_attach_callback(&btn6, button_6_callback);

    // Loop principal do programa
    while (1) {
        tight_loop_contents(); // Mantém o programa em execução
    }

    return 0;
}
