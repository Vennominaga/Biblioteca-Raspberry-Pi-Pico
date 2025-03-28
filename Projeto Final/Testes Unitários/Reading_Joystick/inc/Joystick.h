#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>
#include<stdbool.h>
#include"pico/stdlib.h"
#include "hardware/adc.h"


/******************************
 * Definições e Constantes
 ******************************/

/**
 * @brief Pino ADC para leitura do eixo X do joystick.
 */
#define JOYSTICK_X_PIN 26 // Pino ADC0 (GP26)

/**
 * @brief Pino ADC para leitura do eixo Y do joystick.
 */
#define JOYSTICK_Y_PIN 27 // Pino ADC1 (GP27)

/**
 * @brief Pino GPIO para leitura do botão do joystick.
 */
#define JOYSTICK_BUTTON_PIN 22 // Pino GPIO para o botão



/******************************
 * Estruturas
 ******************************/

/**
 * @brief Estrutura para armazenar os valores do joystick.
 * 
 * Contém os valores dos eixos X e Y (lidos do ADC) e o estado do botão.
 */
typedef struct {
    uint16_t x;      // Valor do eixo X (0-4095)
    uint16_t y;      // Valor do eixo Y (0-4095)
    bool button;     // Estado do botão (true = pressionado, false = não pressionado)
} joystick_state_t;

typedef enum {
    CENTRALIZADO,
    CIMA,
    BAIXO,
    ESQUERDA,
    DIREITA,
    DIREITA_CIMA,
    DIREITA_BAIXO,
    ESQUERDA_CIMA,
    ESQUERDA_BAIXO
} Direction;


/*
******************************
 * Funções
 ******************************/

/**
 * @brief Inicializa o joystick.
 * 
 * Configura os pinos ADC para leitura dos eixos X e Y e o pino GPIO para leitura do botão.
 */
void joystick_init();

/**
 * @brief Lê os valores atuais do joystick.
 * 
 * @return Estrutura `joystick_state_t` contendo os valores dos eixos X e Y e o estado do botão.
 */
joystick_state_t joystick_read();

/**
 * @brief Lê o valor do eixo X do joystick.
 * 
 * @return Valor do eixo X (0-4095).
 */
uint16_t joystick_read_x();

/**
 * @brief Lê o valor do eixo Y do joystick.
 * 
 * @return Valor do eixo Y (0-4095).
 */
uint16_t joystick_read_y();

/**
 * @brief Lê o estado do botão do joystick.
 * 
 * @return true se o botão estiver pressionado, false caso contrário.
 */
bool joystick_read_button();

/**
 * @brief Identifica a direção do joystick com base nos valores dos eixos X e Y.
 * 
 * @param state Estrutura `joystick_state_t` contendo os valores dos eixos X e Y.
 * @return Direção do joystick (enum Direction).
 */
Direction joystick_get_direction(joystick_state_t state);

/**
 * @brief Mapeia um valor de uma faixa de entrada para uma faixa de saída.
 * 
 * Útil para normalizar os valores do ADC para uma faixa desejada (ex: -100 a 100).
 * 
 * @param value Valor a ser mapeado.
 * @param min_input Valor mínimo da faixa de entrada.
 * @param max_input Valor máximo da faixa de entrada.
 * @param min_output Valor mínimo da faixa de saída.
 * @param max_output Valor máximo da faixa de saída.
 * @return Valor mapeado para a faixa de saída.
 */
int16_t joystickPi_map_value(uint16_t value, uint16_t min_input, uint16_t max_input, int16_t min_output, int16_t max_output);

#endif