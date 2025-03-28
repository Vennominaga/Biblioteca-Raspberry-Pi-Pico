#ifndef MATRIZ_RGB_PI_H
#define MATRIZ_RGB_PI_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"

/**
 * @file MatrizRGBPI.h
 * @brief Cabeçalho para controle de uma matriz de LEDs RGB 5x5 usando Raspberry Pi Pico.
 * 
 * Este arquivo define estruturas, constantes e funções para manipular uma matriz de LEDs WS2812B (NeoPixel).
 * Inclui suporte para exibição de letras, strings, frames personalizados e efeitos de scroll.
 */

// --- DEFINIÇÕES DE HARDWARE ---
#define LED_COUNT 25   // Número total de LEDs (5x5)
#define LED_PIN 7      // Pino GPIO conectado à matriz

// --- ESTRUTURAS DE DADOS ---
/**
 * @brief Estrutura de um pixel RGB (formato GRB para WS2812B).
 */
typedef struct {
    uint8_t G;  // Componente verde (0-255)
    uint8_t R;  // Componente vermelho (0-255)
    uint8_t B;  // Componente azul (0-255)
} pixel_t;

typedef pixel_t MatrizRGBPI_t;  // Sinônimo para clareza no contexto da matriz.

/**
 * @brief Estrutura de uma letra para exibição.
 * @param character Caractere (A-Z ou espaço).
 * @param matrix Matriz 5x5x3 representando o desenho da letra (valores RGB).
 */
typedef struct {
    char character;
    int matrix[5][5][3];
} Letter;

// --- VARIÁVEIS GLOBAIS ---
extern MatrizRGBPI_t leds[LED_COUNT];  // Buffer de pixels da matriz
extern PIO matriz_pio;                // Instância do PIO (PIO0 ou PIO1)
extern uint sm;                       // Máquina de estado (state machine) do PIO

// --- PROTÓTIPOS DE FUNÇÕES ---

/**
 * @brief Inicializa a matriz de LEDs.
 * @param pin Pino GPIO conectado aos LEDs.
 */
void MatrizRGBPI_Init(uint pin);

/**
 * @brief Define a cor de um LED específico.
 * @param index Índice do LED (0 a LED_COUNT-1).
 * @param r Componente vermelho (0-255).
 * @param g Componente verde (0-255).
 * @param b Componente azul (0-255).
 */
void MatrizRGBPI_SetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);

/**
 * @brief Limpa todos os LEDs (define cores como 0).
 */
void MatrizRGBPI_Clear();

/**
 * @brief Envia os dados do buffer para a matriz de LEDs.
 */
void MatrizRGBPI_Write();

/**
 * @brief Converte coordenadas (x, y) para índice linear.
 * @param x Coordenada horizontal (0-4).
 * @param y Coordenada vertical (0-4).
 * @return Índice linear correspondente (0-24).
 */
int getIndex(int x, int y);

/**
 * @brief Exibe uma letra na matriz com cor personalizada.
 * @param letter Matriz 5x5x3 representando a letra.
 * @param r Componente vermelho (0-255).
 * @param g Componente verde (0-255).
 * @param b Componente azul (0-255).
 */
void MatrizRGBPI_displayLetter(const int letter[5][5][3], uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Exibe uma string na matriz, caractere por caractere.
 * @param str String a ser exibida.
 * @param r Componente vermelho (0-255).
 * @param g Componente verde (0-255).
 * @param b Componente azul (0-255).
 */
void MatrizRGBPI_displayString(const char *str, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Exibe um frame personalizado (5x5) na matriz.
 * @param frame Matriz 5x5x3 contendo as cores dos pixels.
 */
void MatrizRGBPI_displayFrame(const int frame[5][5][3]);

/**
 * @brief Gera animação de scroll entre duas letras.
 * @param currentLetter Letra atual.
 * @param nextLetter Próxima letra.
 * @param delay_ms Tempo entre frames (ms).
 * @param r Componente vermelho (0-255).
 * @param g Componente verde (0-255).
 * @param b Componente azul (0-255).
 */
void scrollLetters(const Letter *currentLetter, const Letter *nextLetter, int delay_ms, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Exibe uma string com efeito de scroll.
 * @param str String a ser exibida.
 * @param delay_ms Tempo entre frames (ms).
 * @param r Componente vermelho (0-255).
 * @param g Componente verde (0-255).
 * @param b Componente azul (0-255).
 */
void MatrizRGBPI_displayStringWithScroll(const char *str, int delay_ms, uint8_t r, uint8_t g, uint8_t b);

#endif // MATRIZ_RGB_PI_H
