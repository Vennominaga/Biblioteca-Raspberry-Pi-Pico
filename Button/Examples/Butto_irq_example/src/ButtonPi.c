#include "inc/ButtonPi.h"
#include "inc/gpio_irq_manager.h"

/******************************
 * Documentação do Arquivo
 ******************************/

/**
 * @file ButtonPi.c
 * @brief Implementação da biblioteca ButtonPi para gerenciamento de botões no Raspberry Pi Pico
 * 
 * Este arquivo contém a implementação das funções declaradas em `ButtonPi.h` para gerenciar botões
 * conectados ao Raspberry Pi Pico. Ele permite inicializar botões, ler seu estado e registrar
 * funções de callback que serão chamadas quando o botão for pressionado.
 * 
 * Funcionalidades:
 * 1. Inicialização de botões em um pino GPIO específico.
 * 2. Leitura do estado atual do botão (pressionado ou não pressionado).
 * 3. Registro de funções de callback para tratar eventos de pressionamento usando interrupções.
 * 
 * A biblioteca utiliza interrupções para detectar bordas de descida (quando o botão é pressionado),
 * garantindo que os callbacks sejam chamados de forma eficiente e sem a necessidade de polling.
 */

/******************************
 * Funções
 ******************************/

/**
 * @brief Inicializa um botão em um pino GPIO específico.
 * 
 * Configura o pino como entrada, habilita o resistor de pull-up interno e inicializa o estado do botão.
 * 
 * @param btn Ponteiro para a estrutura ButtonPi que representa o botão.
 * @param pin Pino GPIO ao qual o botão está conectado.
 */
void ButtonPi_init(ButtonPi *btn, uint pin) {
    btn->pin = pin; // Armazena o pino GPIO na estrutura
    btn->last_state = gpio_get(pin); // Inicializa o último estado lido do botão

    gpio_init(pin); // Inicializa o pino GPIO
    gpio_set_dir(pin, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(pin); // Habilita o resistor de pull-up interno (assumindo que o botão está conectado ao GND)
}

/**
 * @brief Lê o estado atual do botão.
 * 
 * Retorna `true` se o botão estiver pressionado e `false` caso contrário.
 * 
 * @param btn Ponteiro para a estrutura ButtonPi que representa o botão.
 * @return true se o botão estiver pressionado, false caso contrário.
 */
bool ButtonPi_read(ButtonPi *btn) {
    return !gpio_get(btn->pin); // Retorna o estado do botão (invertido, pois o botão está em pull-up)
}

/**
 * @brief Registra uma função de callback para ser chamada quando o botão for pressionado.
 * 
 * Configura uma interrupção na borda de descida (quando o botão é pressionado) para chamar a função
 * de callback fornecida. A interrupção é gerenciada pela biblioteca `gpio_irq_manager`.
 * 
 * @param btn Ponteiro para a estrutura ButtonPi que representa o botão.
 * @param callback Função de callback que será chamada quando o botão for pressionado.
 */
void ButtonPi_attach_callback(ButtonPi *btn, void (*callback)(void)) {
    gpio_irq_manager_init(); // Garante que o gerenciador de interrupções esteja inicializado

    if (btn->pin < 30) { // Verifica se o pino é válido (pinos GPIO válidos no Raspberry Pi Pico)
        register_gpio_callback(btn->pin, callback, GPIO_IRQ_EDGE_FALL); // Configura a interrupção na borda de descida
    }
}
