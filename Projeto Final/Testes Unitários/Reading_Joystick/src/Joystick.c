#include"inc/Joystick.h"
#include<stdlib.h>
#include<math.h>


/******************************
 * Funções
 ******************************/

/**
 * @brief Inicializa o joystick.
 * 
 * Configura os pinos ADC para leitura dos eixos X e Y e o pino GPIO para leitura do botão.
 * Habilita o resistor de pull-up no pino do botão.
 */
void joystick_init() {
    // Inicializa o ADC
    adc_init();

    // Configura os pinos do joystick como entradas analógicas
    adc_gpio_init(JOYSTICK_X_PIN); // Configura o pino do eixo X (GP26)
    adc_gpio_init(JOYSTICK_Y_PIN); // Configura o pino do eixo Y (GP27)

    // Configura o pino do botão como entrada digital
    gpio_init(JOYSTICK_BUTTON_PIN);
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON_PIN); // Habilita o resistor de pull-up
}

/**
 * @brief Lê o estado atual do joystick (eixos X e Y e botão).
 * 
 * @return Estrutura `joystick_state_t` contendo os valores dos eixos X e Y e o estado do botão.
 */
joystick_state_t joystick_read() {
    joystick_state_t state;

    // Lê o valor do eixo X
    adc_select_input(1); // Seleciona o canal ADC0 (GP26)
    state.x = adc_read(); // Lê o valor do ADC

    // Lê o valor do eixo Y
    adc_select_input(0); // Seleciona o canal ADC1 (GP27)
    state.y = adc_read(); // Lê o valor do ADC

    // Lê o estado do botão
    state.button = !gpio_get(JOYSTICK_BUTTON_PIN); // Inverte o valor porque o botão está em pull-up

    return state;
}

/**
 * @brief Lê apenas o valor do eixo X do joystick.
 * 
 * @return Valor do eixo X (0-4095).
 */
uint16_t joystick_read_x() {
    adc_select_input(1); // Seleciona o canal ADC0 (GP26)
    return adc_read(); // Lê o valor do ADC
}

/**
 * @brief Lê apenas o valor do eixo Y do joystick.
 * 
 * @return Valor do eixo Y (0-4095).
 */
uint16_t joystickPi_read_y() {
    adc_select_input(0); // Seleciona o canal ADC1 (GP27)
    return adc_read(); // Lê o valor do ADC
}

/**
 * @brief Lê apenas o estado do botão do joystick.
 * 
 * @return true se o botão estiver pressionado, false caso contrário.
 */
bool joystickPi_read_button() {
    return !gpio_get(JOYSTICK_BUTTON_PIN); // Inverte o valor porque o botão está em pull-up
}

/**
 * @brief Identifica a direção do joystick com base nos valores dos eixos X e Y.
 * 
 * @param state Estrutura `joystick_state_t` contendo os valores dos eixos X e Y.
 * @return Direção do joystick (enum Direction).
 */
Direction joystick_get_direction(joystick_state_t state) {
    // Define os limites para considerar o joystick centralizado
    const uint16_t center_threshold = 2048; // Valor central do ADC (0-4095)
    const uint16_t deadzone = 500; // Zona morta para evitar flutuações

    // Calcula a diferença em relação ao centro
    int16_t x_diff = state.x - center_threshold;
    int16_t y_diff = state.y - center_threshold;

    // Verifica se o joystick está centralizado
    if (abs(x_diff) < deadzone && abs(y_diff) < deadzone) {
        return CENTRALIZADO;
    }

    // Verifica as direções principais e diagonais
    if (x_diff > deadzone && abs(y_diff) < deadzone) {
        return DIREITA;
    } else if (x_diff < -deadzone && abs(y_diff) < deadzone) {
        return ESQUERDA;
    } else if (y_diff > deadzone && abs(x_diff) < deadzone) {
        return CIMA;
    } else if (y_diff < -deadzone && abs(x_diff) < deadzone) {
        return BAIXO;
    } else if (x_diff > deadzone && y_diff > deadzone) {
        return DIREITA_CIMA;
    } else if (x_diff > deadzone && y_diff < -deadzone) {
        return DIREITA_BAIXO;
    } else if (x_diff < -deadzone && y_diff > deadzone) {
        return ESQUERDA_CIMA;
    } else if (x_diff < -deadzone && y_diff < -deadzone) {
        return ESQUERDA_BAIXO;
    }

    // Caso padrão (centralizado)
    return CENTRALIZADO;
}

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

int16_t joystickPi_map_value(uint16_t value, uint16_t min_input, uint16_t max_input, int16_t min_output, int16_t max_output) {
    return (int16_t)((value - min_input) * (max_output - min_output) / (max_input - min_input) + min_output);
}

