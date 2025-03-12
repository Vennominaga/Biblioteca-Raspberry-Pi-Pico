#ifndef JOYSTICK_PI_H
#define JOYSTICK_PI_H

#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Definições para os pinos ADC
#define JOYSTICK_X_PIN 26                   // Pino ADC0 (GP26)
#define JOYSTICK_Y_PIN 27                   // Pino ADC1 (GP27)
#define JOYSTICK_BUTTON_PIN 22              // Pino GPIO para o botão

// Estrutura para armazenar os valores do joystick
typedef struct 
{
    uint16_t x;
    uint16_t y;
    bool button;
} joystick_state_t;


// Protótipos das funções
void joystickPi_init();
joystick_state_t joystickPi_read();
uint16_t joystickPi_read_x();
uint16_t joystickPi_read_y();
bool joystickPi_read_button();
int16_t joystickPi_map_value(uint16_t value, uint16_t min_input, uint16_t max_input, int16_t min_output, int16_t max_output);

#endif // JOYSTICK_PI_H