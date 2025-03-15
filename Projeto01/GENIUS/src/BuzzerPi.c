#include "inc/BuzzerPi.h"
#include "hardware/clocks.h"
#include <stdio.h>

/******************************
 * Documentação do Arquivo
 ******************************/

/**
 * @file BuzzerPi.c
 * @brief Implementação da biblioteca BuzzerPi para controle de um buzzer no Raspberry Pi Pico
 * 
 * Este arquivo implementa as funcionalidades declaradas em `BuzzerPi.h` para controlar um buzzer
 * usando PWM (Modulação por Largura de Pulso) no Raspberry Pi Pico. Ele permite tocar tons, melodias
 * e beeps com controle de frequência, duração e divisão de clock.
 * 
 * Funcionalidades:
 * 1. Inicialização do PWM para controle do buzzer.
 * 2. Cálculo do valor de "wrap" para gerar frequências específicas.
 * 3. Reprodução de tons únicos com controle de frequência e duração.
 * 4. Reprodução de melodias a partir de arrays de frequências e durações.
 * 5. Reprodução de beeps repetidos.
 */

/******************************
 * Funções
 ******************************/

/**
 * @brief Inicializa o PWM no pino especificado.
 * 
 * Configura o pino para funcionar como saída PWM.
 * 
 * @param pin Pino GPIO onde o buzzer está conectado.
 */
void initialize_pwm(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM); // Configura o pino como saída PWM
}

/**
 * @brief Calcula o valor de "wrap" para gerar uma frequência específica.
 * 
 * O valor de "wrap" é usado pelo hardware PWM para definir a frequência do sinal.
 * 
 * @param target_frequency Frequência desejada em Hz.
 * @param clkdiv Divisor de clock usado para o PWM.
 * @return Valor de "wrap" calculado. Se o valor exceder 65535, retorna 65535.
 */
uint16_t calculate_wrap(uint32_t target_frequency, float clkdiv) {
    uint32_t clock_freq = clock_get_hz(clk_sys); // Obtém a frequência do clock do sistema
    uint32_t wrap = (clock_freq / (target_frequency * clkdiv)) - 1; // Calcula o valor de wrap
    return (wrap > 65535) ? 65535 : wrap; // Limita o valor de wrap a 65535 (máximo suportado)
}

/**
 * @brief Toca um tom no buzzer com a frequência e duração especificadas.
 * 
 * Usa o divisor de clock padrão definido em `CLK_DIV_DEFAULT`.
 * 
 * @param pin Pino GPIO onde o buzzer está conectado.
 * @param freq Frequência do tom em Hz.
 * @param duration_ms Duração do tom em milissegundos.
 */
void play_tone(uint pin, uint32_t freq, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin); // Obtém o número do slice PWM associado ao pino

    uint16_t wrap_value = calculate_wrap(freq, CLK_DIV_DEFAULT); // Calcula o valor de wrap

    pwm_set_wrap(slice_num, wrap_value); // Configura o valor de wrap no slice PWM
    pwm_set_clkdiv(slice_num, CLK_DIV_DEFAULT); // Configura o divisor de clock
    pwm_set_gpio_level(pin, wrap_value / 2); // Define o nível do PWM para 50% (duty cycle)
    pwm_set_enabled(slice_num, true); // Habilita o PWM

    sleep_ms(duration_ms); // Mantém o tom ativo pelo tempo especificado

    pwm_set_gpio_level(pin, 0); // Desliga o PWM
}

/**
 * @brief Toca um tom no buzzer com a frequência, duração e divisor de clock especificados.
 * 
 * Permite maior controle sobre a geração do tom, especificando um divisor de clock personalizado.
 * 
 * @param pin Pino GPIO onde o buzzer está conectado.
 * @param freq Frequência do tom em Hz.
 * @param duration_ms Duração do tom em milissegundos.
 * @param clkdiv Divisor de clock usado para o PWM.
 */
void play_tone_clkdiv(uint pin, int freq, int duration_ms, float clkdiv) {
    uint slice_num = pwm_gpio_to_slice_num(pin); // Obtém o número do slice PWM associado ao pino

    uint16_t wrap_value = calculate_wrap(freq, clkdiv); // Calcula o valor de wrap

    pwm_set_wrap(slice_num, wrap_value); // Configura o valor de wrap no slice PWM
    pwm_set_clkdiv(slice_num, clkdiv); // Configura o divisor de clock
    pwm_set_gpio_level(pin, wrap_value / 2); // Define o nível do PWM para 50% (duty cycle)
    pwm_set_enabled(slice_num, true); // Habilita o PWM

    sleep_ms(duration_ms); // Mantém o tom ativo pelo tempo especificado

    pwm_set_gpio_level(pin, 0); // Desliga o PWM
}

/**
 * @brief Toca uma melodia a partir de arrays de frequências e durações.
 * 
 * Reproduz uma sequência de tons definidos pelos arrays `melody` e `durations`.
 * Se a frequência for 0, o buzzer permanece em silêncio pelo tempo especificado.
 * 
 * @param pin Pino GPIO onde o buzzer está conectado.
 * @param melody Array de frequências que compõem a melodia.
 * @param durations Array de durações correspondentes a cada frequência.
 * @param clkdiv Divisor de clock usado para o PWM.
 * @param length Número de notas na melodia.
 */
void play_melody(uint pin, int *melody, int *durations, float clkdiv, int length) {
    for (int i = 0; i < length; i++) {
        if (melody[i] != 0) {
            play_tone_clkdiv(pin, melody[i], durations[i], clkdiv); // Toca a nota
        } else {
            sleep_ms(durations[i]); // Pausa (nota silenciosa)
        }
    }
}

/**
 * @brief Reproduz um beep repetido no buzzer.
 * 
 * Reproduz um beep com a frequência e duração especificadas, repetindo-o pelo número de vezes indicado.
 * 
 * @param pin Pino GPIO onde o buzzer está conectado.
 * @param freq Frequência do beep em Hz.
 * @param duration Duração de cada beep em milissegundos.
 * @param repetition Número de vezes que o beep será repetido.
 */
void beep(uint pin, int freq, int duration, int repetition) {
    for (int i = 0; i < repetition; i++) {
        play_tone(pin, freq, duration); // Toca o beep
        sleep_ms(500); // Intervalo entre os beeps
    }
}
