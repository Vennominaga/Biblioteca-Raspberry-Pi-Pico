#include "inc/MatrizRGBPI.h"
#include <ctype.h>
#include <string.h>
#include "inc/alphabet.h"

// Definição do buffer de pixels que formam a matriz.
MatrizRGBPI_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO matriz_pio;
uint sm;


/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void MatrizRGBPI_Init(uint pin) {
  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  matriz_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(matriz_pio, false);
  if (sm < 0) {
    matriz_pio = pio1;
    sm = pio_claim_unused_sm(matriz_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(matriz_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

// Mapeamento de coordenadas para a matriz de LEDs
int getIndex(int x, int y) {
  if (y % 2 == 0) {
      return 24 - (y * 5 + x); // Linha par (esquerda para direita)
  } else {
      return 24 - (y * 5 + (4 - x)); // Linha ímpar (direita para esquerda)
  }
}

/**
 * Atribui uma cor RGB a um LED.
 */
void MatrizRGBPI_SetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void MatrizRGBPI_Clear() {
  for (uint i = 0; i < LED_COUNT; ++i)
    MatrizRGBPI_SetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void MatrizRGBPI_Write() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(matriz_pio, sm, leds[i].G);
    pio_sm_put_blocking(matriz_pio, sm, leds[i].R);
    pio_sm_put_blocking(matriz_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

/**
 * Exibe uma letra na matriz de LEDs.
 * @param letra Matriz de bits representando a letra.
 * @param corR Componente vermelho da cor.
 * @param corG Componente verde da cor.
 * @param corB Componente azul da cor.
 */


/*
 void MatrizRGBPI_displayLetter(const int letter[5][5][3]) {
      for (int row = 0; row < 5; row++) {
          for (int col = 0; col < 5; col++) {
              int position = getIndex(row, col);
              MatrizRGBPI_SetLED(position, letter[col][row][0],  // Red
                                letter[col][row][1],  // Green
                                letter[col][row][2]); // Blue
          }
      }
      MatrizRGBPI_Write();
      sleep_ms(1000); // Display time for each letter (1 second)
    }
*/
void MatrizRGBPI_displayLetter(const int letter[5][5][3], uint8_t r, uint8_t g, uint8_t b) {
      for (int row = 0; row < 5; row++) {
          for (int col = 0; col < 5; col++) {
              int position = getIndex(row, col);
              // Use the letter matrix as a mask (0 = off, non-zero = use provided color)
              if (letter[col][row][0] || letter[col][row][1] || letter[col][row][2]) {
                  MatrizRGBPI_SetLED(position, r, g, b);
              } else {
                  MatrizRGBPI_SetLED(position, 0, 0, 0);
              }
          }
      }
      MatrizRGBPI_Write();
      sleep_ms(1000); // Display time for each letter (1 second)
  }

/*
// Função para escrever uma string na matriz (com suporte a minúsculas)
void MatrizRGBPI_displayString(const char *str) {
  for (int i = 0; i < strlen(str); i++) {
      char c = toupper(str[i]); // Usa a função toupper() do <ctype.h>
      // Busca a letra no dicionário
      for (int j = 0; j < ALPHABET_COUNT; j++) {
          if (alphabet[j].character == c) {
              MatrizRGBPI_displayLetter(alphabet[j].matrix);
              break;
          }
      }
  }
}
*/

void MatrizRGBPI_displayString(const char *str, uint8_t r, uint8_t g, uint8_t b) {
      for (int i = 0; i < strlen(str); i++) {
          char c = toupper(str[i]);
          // Busca a letra no dicionário
          for (int j = 0; j < ALPHABET_COUNT; j++) {
              if (alphabet[j].character == c) {
                  MatrizRGBPI_displayLetter(alphabet[j].matrix, r, g, b);
                  break;
              }
          }
      }
  }

// Função para exibir um frame específico (5x5) na matriz
void MatrizRGBPI_displayFrame(const int frame[5][5][3]) {
      for (int y = 0; y < 5; y++) {
          for (int x = 0; x < 5; x++) {
              int posicao = getIndex(x, y);
              MatrizRGBPI_SetLED(posicao, frame[y][x][0], frame[y][x][1], frame[y][x][2]);
          }
      }
      MatrizRGBPI_Write();
  }
/*
// Function to generate scroll between two letters
void scrollLetters(const Letter *currentLetter, const Letter *nextLetter, int delay_ms) {
      int frame[5][5][3]; // Buffer for the current frame
    
      for (int offset = 0; offset <= 5; offset++) {
          // Fill the frame with columns from current and next letter
          for (int y = 0; y < 5; y++) {
              for (int x = 0; x < 5; x++) {
                  if (x < 5 - offset) {
                      // Get from current letter
                      frame[y][x][0] = currentLetter->matrix[y][x + offset][0];
                      frame[y][x][1] = currentLetter->matrix[y][x + offset][1];
                      frame[y][x][2] = currentLetter->matrix[y][x + offset][2];
                  } else {
                      // Get from next letter
                      frame[y][x][0] = nextLetter->matrix[y][x - (5 - offset)][0];
                      frame[y][x][1] = nextLetter->matrix[y][x - (5 - offset)][1];
                      frame[y][x][2] = nextLetter->matrix[y][x - (5 - offset)][2];
                  }
              }
          }
          MatrizRGBPI_displayFrame(frame);
          sleep_ms(delay_ms);
      }
  }

*/
void scrollLetters(const Letter *currentLetter, const Letter *nextLetter, int delay_ms, uint8_t r, uint8_t g, uint8_t b) {
      int frame[5][5][3]; // Buffer for the current frame
      
      for (int offset = 0; offset <= 5; offset++) {
          // Fill the frame with columns from current and next letter
          for (int y = 0; y < 5; y++) {
              for (int x = 0; x < 5; x++) {
                  if (x < 5 - offset) {
                      // Get from current letter - use specified color if pixel is "on"
                      if (currentLetter->matrix[y][x + offset][0] || 
                          currentLetter->matrix[y][x + offset][1] || 
                          currentLetter->matrix[y][x + offset][2]) {
                          frame[y][x][0] = r;
                          frame[y][x][1] = g;
                          frame[y][x][2] = b;
                      } else {
                          frame[y][x][0] = 0;
                          frame[y][x][1] = 0;
                          frame[y][x][2] = 0;
                      }
                  } else {
                      // Get from next letter - use specified color if pixel is "on"
                      if (nextLetter->matrix[y][x - (5 - offset)][0] || 
                          nextLetter->matrix[y][x - (5 - offset)][1] || 
                          nextLetter->matrix[y][x - (5 - offset)][2]) {
                          frame[y][x][0] = r;
                          frame[y][x][1] = g;
                          frame[y][x][2] = b;
                      } else {
                          frame[y][x][0] = 0;
                          frame[y][x][1] = 0;
                          frame[y][x][2] = 0;
                      }
                  }
              }
          }
          MatrizRGBPI_displayFrame(frame);
          sleep_ms(delay_ms);
      }
  }
  
/*
// Function to write a string with scroll effect
void MatrizRGBPI_displayStringWithScroll(const char *str, int delay_ms) {
      const Letter *currentLetter = NULL;
      const Letter *nextLetter = NULL;
  
      for (int i = 0; i < strlen(str); i++) {
          char c = toupper(str[i]);
          
          // Find the next letter in the dictionary
          for (int j = 0; j < ALPHABET_COUNT; j++) {
              if (alphabet[j].character == c) {
                  nextLetter = &alphabet[j];
                  break;
              }
          }
  
          if (currentLetter != NULL && nextLetter != NULL) {
              scrollLetters(currentLetter, nextLetter, delay_ms);
          } else if (nextLetter != NULL) {
              // Display the first letter without scrolling
              MatrizRGBPI_displayFrame(nextLetter->matrix);
              sleep_ms(delay_ms * 3);
          }
  
          currentLetter = nextLetter;
      }
  }
      */
// Function to write a string with scroll effect and custom color
void MatrizRGBPI_displayStringWithScroll(const char *str, int delay_ms, uint8_t r, uint8_t g, uint8_t b) {
      const Letter *currentLetter = NULL;
      const Letter *nextLetter = NULL;
  
      for (int i = 0; i < strlen(str); i++) {
          char c = toupper(str[i]);
          
          // Find the next letter in the dictionary
          for (int j = 0; j < ALPHABET_COUNT; j++) {
              if (alphabet[j].character == c) {
                  nextLetter = &alphabet[j];
                  break;
              }
          }
  
          if (currentLetter != NULL && nextLetter != NULL) {
              scrollLetters(currentLetter, nextLetter, delay_ms, r, g, b);
          } else if (nextLetter != NULL) {
              // Display the first letter without scrolling (with specified color)
              int frame[5][5][3];
              for (int y = 0; y < 5; y++) {
                  for (int x = 0; x < 5; x++) {
                      // If the pixel is "on" in the letter matrix (any non-zero value), use the specified color
                      if (nextLetter->matrix[y][x][0] || nextLetter->matrix[y][x][1] || nextLetter->matrix[y][x][2]) {
                          frame[y][x][0] = r;
                          frame[y][x][1] = g;
                          frame[y][x][2] = b;
                      } else {
                          frame[y][x][0] = 0;
                          frame[y][x][1] = 0;
                          frame[y][x][2] = 0;
                      }
                  }
              }
              MatrizRGBPI_displayFrame(frame);
              sleep_ms(delay_ms * 3);
          }
  
          currentLetter = nextLetter;
      }
  }