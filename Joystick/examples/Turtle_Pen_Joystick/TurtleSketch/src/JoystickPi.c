#include "inc/JoystickPi.h"

// Inicializa o joystick
void joystickPi_init() 
{
    // Inicializa o ADC
    adc_init();

    // Configura os pinos do joystick como entradas analógicas
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    // Configura o pino do botão como entrada digital
    gpio_init(JOYSTICK_BUTTON_PIN);
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);
    // Habilita o resistor de pull-up
    gpio_pull_up(JOYSTICK_BUTTON_PIN);  
}

// Lê o estado atual do joystick (X, Y e botão)
joystick_state_t joystickPi_read() 
{
    joystick_state_t state;

    // Lê o valor do eixo X
    adc_select_input(1);  // Seleciona o canal ADC0 (GP26)
    state.x = adc_read();

    // Lê o valor do eixo Y
    adc_select_input(0);  // Seleciona o canal ADC1 (GP27)
    state.y = adc_read();

    // Lê o estado do botão
    state.button = !gpio_get(JOYSTICK_BUTTON_PIN);  // Inverte o valor porque o botão está em pull-up


    return state;
}

// Lê apenas o valor do eixo X
uint16_t joystickPi_read_x() 
{
    adc_select_input(0);
    return adc_read();
}

// Lê apenas o valor do eixo Y
uint16_t joystickPi_read_y() 
{
    adc_select_input(1);
    return adc_read();
}

// Lê apenas o estado do botão
bool joystickPi_read_button() 
{
    return !gpio_get(JOYSTICK_BUTTON_PIN);  // Inverte o valor porque o botão está em pull-up
}

// Mapeia um valor de um intervalo para outro
int16_t joystickPi_map_value(uint16_t value, uint16_t min_input, uint16_t max_input, int16_t min_output, int16_t max_output) 
{
    return (int16_t)((value - min_input) * (max_output - min_output) / (max_input - min_input) + min_output);
}