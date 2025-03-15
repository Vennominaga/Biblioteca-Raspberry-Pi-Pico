// gpio_irq_manager.h
#ifndef GPIO_IRQ_MANAGER_H
#define GPIO_IRQ_MANAGER_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

/******************************
 * Documentação do Arquivo
 ******************************/

/**
 * @file gpio_irq_manager.h
 * @brief Gerenciador de Interrupções GPIO para Raspberry Pi Pico
 * 
 * Este arquivo define a interface para um gerenciador de interrupções GPIO que permite registrar
 * e remover callbacks para eventos específicos em pinos GPIO. Ele é projetado para simplificar
 * o uso de interrupções GPIO no Raspberry Pi Pico, fornecendo uma maneira fácil de associar
 * funções de callback a eventos como bordas de subida ou descida.
 * 
 * Funcionalidades:
 * 1. Registro de callbacks para eventos GPIO.
 * 2. Remoção de callbacks registrados.
 * 3. Inicialização do gerenciador de interrupções.
 * 
 * O gerenciador suporta até 30 pinos GPIO (o número máximo de pinos GPIO no Raspberry Pi Pico).
 */

/******************************
 * Definições e Constantes
 ******************************/

/**
 * @brief Número máximo de pinos GPIO suportados pelo gerenciador.
 */
#define MAX_GPIO_PINS 30

/******************************
 * Variáveis Globais
 ******************************/

/**
 * @brief Vetor de callbacks para eventos GPIO.
 * 
 * Este vetor armazena as funções de callback registradas para cada pino GPIO.
 * Cada posição no vetor corresponde a um pino GPIO, e o valor é um ponteiro para a função de callback.
 */
extern void (*callbacks[MAX_GPIO_PINS])(void);

/******************************
 * Protótipos das Funções
 ******************************/

/**
 * @brief Função de tratamento de interrupções GPIO.
 * 
 * Esta função é chamada automaticamente pelo hardware quando ocorre uma interrupção GPIO.
 * Ela verifica qual pino GPIO gerou a interrupção e chama o callback registrado, se houver.
 * 
 * @param gpio Pino GPIO que gerou a interrupção.
 * @param events Eventos que causaram a interrupção (borda de subida, descida, etc.).
 */
void gpio_irq_handler(uint gpio, uint32_t events);

/**
 * @brief Registra uma função de callback para um pino GPIO e um evento específico.
 * 
 * @param gpio Pino GPIO para o qual o callback será registrado.
 * @param callback Função de callback a ser chamada quando o evento ocorrer.
 * @param event_mask Máscara de eventos que acionarão o callback (borda de subida, descida, etc.).
 */
void register_gpio_callback(uint gpio, void (*callback)(void), uint32_t event_mask);

/**
 * @brief Remove um callback registrado para um pino GPIO.
 * 
 * @param gpio Pino GPIO do qual o callback será removido.
 * @param event_mask Máscara de eventos para a qual o callback será removido.
 */
void remove_gpio_callback(uint gpio, uint32_t event_mask);

/**
 * @brief Inicializa o gerenciador de interrupções GPIO.
 * 
 * Configura o gerenciador de interrupções e prepara o vetor de callbacks.
 */
void gpio_irq_manager_init();

#endif // GPIO_IRQ_MANAGER_H
