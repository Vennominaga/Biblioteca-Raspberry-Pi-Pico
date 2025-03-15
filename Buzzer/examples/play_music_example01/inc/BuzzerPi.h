#ifndef BUZZER_PI_H
#define BUZZER_PI_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"

/******************************
 * Documentação do Arquivo
 ******************************/

/**
 * @file BuzzerPi.h
 * @brief Biblioteca para controle de um buzzer no Raspberry Pi Pico
 * 
 * Esta biblioteca fornece funcionalidades para controlar um buzzer usando PWM (Modulação por Largura de Pulso)
 * no Raspberry Pi Pico. Ela permite tocar tons, melodias e beeps com controle de frequência, duração e
 * divisão de clock.
 * 
 * Funcionalidades:
 * 1. Inicialização do PWM para controle do buzzer.
 * 2. Cálculo do valor de "wrap" para gerar frequências específicas.
 * 3. Reprodução de tons únicos com controle de frequência e duração.
 * 4. Reprodução de melodias a partir de arrays de frequências e durações.
 * 5. Reprodução de beeps repetidos.
 */

/******************************
 * Definições e Constantes
 ******************************/

/**
 * @brief Divisor de clock padrão para o PWM.
 */
#define CLK_DIV_DEFAULT 125.0f

/**
 * @brief Pino GPIO padrão para o buzzer.
 */
#define BUZZER_PIN 21

/******************************
 * Funções
 ******************************/

/**
 * @brief Inicializa o PWM no pino especificado.
 * 
 * Configura o pino para funcionar como saída PWM e define o divisor de clock padrão.
 * 
 * @param pin Pino GPIO onde o buzzer está conectado.
 */
void initialize_pwm(uint pin);

/**
 * @brief Calcula o valor de "wrap" para gerar uma frequência específica.
 * 
 * O valor de "wrap" é usado pelo hardware PWM para definir a frequência do sinal.
 * 
 * @param target_frequency Frequência desejada em Hz.
 * @param clkdiv Divisor de clock usado para o PWM.
 * @return Valor de "wrap" calculado.
 */
uint16_t calculate_wrap(uint32_t target_frequency, float clkdiv);

/**
 * @brief Toca um tom no buzzer com a frequência e duração especificadas.
 * 
 * @param pin Pino GPIO onde o buzzer está conectado.
 * @param freq Frequência do tom em Hz.
 * @param duration_ms Duração do tom em milissegundos.
 */
void play_tone(uint pin, uint32_t freq, uint duration_ms);

/**
 * @brief Toca um tom no buzzer com a frequência, duração e divisor de clock especificados.
 * 
 * @param pin Pino GPIO onde o buzzer está conectado.
 * @param freq Frequência do tom em Hz.
 * @param duration_ms Duração do tom em milissegundos.
 * @param clkdiv Divisor de clock usado para o PWM.
 */
void play_tone_clkdiv(uint pin, int freq, int duration_ms, float clkdiv);

/**
 * @brief Toca uma melodia a partir de arrays de frequências e durações.
 * 
 * @param pin Pino GPIO onde o buzzer está conectado.
 * @param melody Array de frequências que compõem a melodia.
 * @param durations Array de durações correspondentes a cada frequência.
 * @param clkdiv Divisor de clock usado para o PWM.
 * @param length Número de notas na melodia.
 */
void play_melody(uint pin, int *melody, int *durations, float clkdiv, int length);

/**
 * @brief Reproduz um beep repetido no buzzer.
 * 
 * @param pin Pino GPIO onde o buzzer está conectado.
 * @param freq Frequência do beep em Hz.
 * @param duration Duração de cada beep em milissegundos.
 * @param repetition Número de vezes que o beep será repetido.
 */
void beep(uint pin, int freq, int duration, int repetition);

#endif // BUZZER_PI_H
