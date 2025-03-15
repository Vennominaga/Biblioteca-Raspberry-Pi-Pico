// gpio_irq_manager.c
#include "inc/gpio_irq_manager.h"

/******************************
 * Documentação do Arquivo
 ******************************/

/**
 * @file gpio_irq_manager.c
 * @brief Implementação do Gerenciador de Interrupções GPIO para Raspberry Pi Pico
 * 
 * Este arquivo implementa as funcionalidades declaradas em `gpio_irq_manager.h` para gerenciar
 * interrupções GPIO no Raspberry Pi Pico. Ele permite registrar e remover callbacks para eventos
 * específicos em pinos GPIO, além de tratar o debounce (eliminação de ruídos) para garantir que
 * as interrupções sejam acionadas de forma confiável.
 * 
 * Funcionalidades:
 * 1. Registro de callbacks para eventos GPIO.
 * 2. Remoção de callbacks registrados.
 * 3. Tratamento de debounce para evitar múltiplas interrupções causadas por ruídos.
 * 4. Inicialização do gerenciador de interrupções.
 */

/******************************
 * Definições e Constantes
 ******************************/

/**
 * @brief Tempo de debounce em milissegundos.
 * 
 * Define o intervalo mínimo entre duas interrupções consecutivas para evitar ruídos.
 */
#define DEBOUNCE_DELAY_MS 200

/******************************
 * Variáveis Globais
 ******************************/

/**
 * @brief Vetor de callbacks para eventos GPIO.
 * 
 * Este vetor armazena as funções de callback registradas para cada pino GPIO.
 * Cada posição no vetor corresponde a um pino GPIO, e o valor é um ponteiro para a função de callback.
 */
void (*callbacks[MAX_GPIO_PINS])(void) = {NULL};

/**
 * @brief Array para armazenar o último tempo de interrupção para cada pino.
 * 
 * Usado para implementar o debounce, garantindo que interrupções consecutivas sejam ignoradas
 * se ocorrerem dentro do intervalo de tempo definido por `DEBOUNCE_DELAY_MS`.
 */
absolute_time_t last_interrupt_time[MAX_GPIO_PINS];

/******************************
 * Funções
 ******************************/

/**
 * @brief Função de tratamento de interrupções GPIO.
 * 
 * Esta função é chamada automaticamente pelo hardware quando ocorre uma interrupção GPIO.
 * Ela verifica se o pino GPIO é válido, se há um callback registrado e se o tempo desde a última
 * interrupção é maior que o tempo de debounce. Se todas as condições forem atendidas, o callback
 * correspondente é chamado.
 * 
 * @param gpio Pino GPIO que gerou a interrupção.
 * @param events Eventos que causaram a interrupção (borda de subida, descida, etc.).
 */
void gpio_irq_handler(uint gpio, uint32_t events) {
    // Verifica se o pino é válido e se há um callback registrado
    if (gpio < MAX_GPIO_PINS && callbacks[gpio] != NULL) {
        // Obtém o tempo atual
        absolute_time_t now = get_absolute_time();

        // Verifica se o tempo desde a última interrupção é maior que o tempo de debounce
        if (absolute_time_diff_us(last_interrupt_time[gpio], now) > DEBOUNCE_DELAY_MS * 1000) {
            // Atualiza o tempo da última interrupção
            last_interrupt_time[gpio] = now;

            // Chama a função de callback correspondente ao pino
            callbacks[gpio]();
        }
    }
}

/**
 * @brief Registra uma função de callback para um pino GPIO e um evento específico.
 * 
 * @param gpio Pino GPIO para o qual o callback será registrado.
 * @param callback Função de callback a ser chamada quando o evento ocorrer.
 * @param event_mask Máscara de eventos que acionarão o callback (borda de subida, descida, etc.).
 */
void register_gpio_callback(uint gpio, void (*callback)(void), uint32_t event_mask) {
    if (gpio < MAX_GPIO_PINS) {
        callbacks[gpio] = callback; // Armazena a função no vetor de callbacks
        gpio_set_irq_enabled(gpio, event_mask, true); // Habilita a interrupção para o evento especificado
    }
}

/**
 * @brief Remove um callback registrado para um pino GPIO.
 * 
 * @param gpio Pino GPIO do qual o callback será removido.
 * @param event_mask Máscara de eventos para a qual o callback será removido.
 */
void remove_gpio_callback(uint gpio, uint32_t event_mask) {
    if (gpio < MAX_GPIO_PINS) {
        callbacks[gpio] = NULL; // Remove a função do vetor de callbacks
        gpio_set_irq_enabled(gpio, event_mask, false); // Desabilita interrupção
    }
}

/**
 * @brief Inicializa o gerenciador de interrupções GPIO.
 * 
 * Configura a função de tratamento de interrupções global e habilita interrupções no banco de GPIOs.
 */
void gpio_irq_manager_init() {
    gpio_set_irq_callback(gpio_irq_handler); // Configura a função mestra como callback global
    irq_set_enabled(IO_IRQ_BANK0, true); // Habilita interrupções no banco de GPIOs
}
