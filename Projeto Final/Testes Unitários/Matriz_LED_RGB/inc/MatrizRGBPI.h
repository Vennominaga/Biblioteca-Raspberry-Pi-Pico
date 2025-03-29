#ifndef MATRIZ_RGB_PI_H
#define MATRIZ_RGB_PI_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"


// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7

// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t MatrizRGBPI_t; // Mudança de nome de "struct pixel_t" para "MatrizRGBPI_t" por clareza.

// Dictionary of letters (A-Z)
typedef struct {
  char character;
  int matrix[5][5][3];
} Letter;



// Declaração do buffer de pixels que formam a matriz.
extern MatrizRGBPI_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
extern PIO matriz_pio;
extern uint sm;

//extern const Letter alphabet[];

// Protótipos das funções
void MatrizRGBPI_Init(uint pin);
void MatrizRGBPI_SetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void MatrizRGBPI_Clear();
void MatrizRGBPI_Write();
int  MatrizRGBPI_getIndex(int x, int y);
//void MatrizRGBPI_displayLetter(const int letter[5][5][3]);
void MatrizRGBPI_displayLetter(const int letter[5][5][3], uint8_t r, uint8_t g, uint8_t b);
//void MatrizRGBPI_displayString(const char *str);
void MatrizRGBPI_displayString(const char *str, uint8_t r, uint8_t g, uint8_t b);
void MatrizRGBPI_displayFrame(const int frame[5][5][3]);
//void MatrizRGBPI_scrollLetters(const Letter *currentLetter, const Letter *nextLetter, int delay_ms);
void scrollLetters(const Letter *currentLetter, const Letter *nextLetter, int delay_ms, uint8_t r, uint8_t g, uint8_t b);
//void MatrizRGBPI_displayStringWithScroll(const char *str, int delay_ms);
void MatrizRGBPI_displayStringWithScroll(const char *str, int delay_ms, uint8_t r, uint8_t g, uint8_t b);

#endif // MATRIZ_RGB_PI_H

