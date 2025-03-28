#ifndef BUTTON_PI_H
#define BUTTON_PI_H

#include "pico/stdlib.h"
#include <stdbool.h>

/******************************
 * Documentação do Arquivo
 ******************************/

/**
 * @file ButtonPi.h
 * @brief Biblioteca para gerenciamento de botões no Raspberry Pi Pico
 * 
 * Esta biblioteca fornece funcionalidades para gerenciar botões conectados ao Raspberry Pi Pico.
 * Ela permite inicializar botões, ler seu estado e registrar funções de callback que serão chamadas
 * quando o botão for pressionado.
 * 
 * Funcionalidades:
 * 1. Inicialização de botões em um pino GPIO específico.
 * 2. Leitura do estado atual do botão (pressionado ou não pressionado).
 * 3. Registro de funções de callback para tratar eventos de pressionamento.
 * 
 * A biblioteca utiliza detecção de borda para garantir que os callbacks sejam chamados apenas quando
 * o botão é pressionado, evitando múltiplas chamadas devido a ruídos ou bouncing.
 */

/******************************
 * Estruturas
 ******************************/

/**
 * @brief Estrutura que armazena as informações de um botão.
 */
typedef struct {
    uint pin;                   // Pino GPIO ao qual o botão está conectado
    bool last_state;            // Último estado lido do botão (para detecção de borda)
} ButtonPi;

/******************************
 * Funções
 ******************************/

/**
 * @brief Inicializa um botão em um pino GPIO específico.
 * 
 * @param btn Ponteiro para a estrutura ButtonPi que representa o botão.
 * @param pin Pino GPIO ao qual o botão está conectado.
 */
void ButtonPi_init(ButtonPi *btn, uint pin);

/**
 * @brief Lê o estado atual do botão.
 * 
 * @param btn Ponteiro para a estrutura ButtonPi que representa o botão.
 * @return true se o botão estiver pressionado, false caso contrário.
 */
bool ButtonPi_read(ButtonPi *btn);

/**
 * @brief Registra uma função de callback para ser chamada quando o botão for pressionado.
 * 
 * @param btn Ponteiro para a estrutura ButtonPi que representa o botão.
 * @param callback Função de callback que será chamada quando o botão for pressionado.
 */
void ButtonPi_attach_callback(ButtonPi *btn, void (*callback)(void));

#endif // BUTTON_PI_H
