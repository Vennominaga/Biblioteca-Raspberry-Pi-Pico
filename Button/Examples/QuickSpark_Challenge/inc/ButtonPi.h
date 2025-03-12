#ifndef BUTTON_PI_H
#define BUTTON_PI_H

#include "pico/stdlib.h"
#include <stdbool.h>

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

// Definição do tipo de função de callback
//typedef void (*ButtonPi_callback_t)(void);

// Estrutura para armazenar as informações do botão
typedef struct 
{
    uint pin;                   // Pino GPIO ao qual o botão está conectado
    bool last_state;            // Último estado lido do botão (para detecção de borda)
    //ButtonPi_callback_t callback; // Função de callback para o botão
} ButtonPi;

// Inicializa o botão em um pino específico
void ButtonPi_init(ButtonPi *btn, uint pin);

// Lê o estado atual do botão
bool ButtonPi_read(ButtonPi *btn);

// Registra uma função de callback para ser chamada quando o botão for pressionado
void ButtonPi_attach_callback(ButtonPi *btn, void (*callback)(void));

#endif // BUTTON_PI_H