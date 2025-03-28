#include "inc/MatrizRGBPI.h"
#include <ctype.h>
#include <string.h>
#include "inc/alphabet.h"

// Buffer de pixels que formam a matriz de LEDs. Cada LED é representado por uma estrutura MatrizRGBPI_t.
MatrizRGBPI_t leds[LED_COUNT];

// Variáveis para controle da máquina PIO (Programmable I/O) do Raspberry Pi Pico.
PIO matriz_pio;  // Instância do PIO (PIO0 ou PIO1)
uint sm;         // Máquina de estado (state machine) do PIO

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 * @param pin Pino GPIO conectado à matriz de LEDs.
 */
void MatrizRGBPI_Init(uint pin) {
    // Adiciona o programa PIO para controle dos LEDs WS2812B (NeoPixel) ao PIO0.
    uint offset = pio_add_program(pio0, &ws2818b_program);
    matriz_pio = pio0;

    // Tenta obter uma máquina de estado livre no PIO0.
    sm = pio_claim_unused_sm(matriz_pio, false);
    if (sm < 0) {
        // Se não houver máquinas livres no PIO0, tenta no PIO1.
        matriz_pio = pio1;
        sm = pio_claim_unused_sm(matriz_pio, true); // Força a obtenção de uma máquina.
    }

    // Inicializa o programa WS2812B na máquina de estado obtida.
    ws2818b_program_init(matriz_pio, sm, offset, pin, 800000.f);

    // Limpa o buffer de pixels, definindo todas as cores como 0 (apagado).
    for (uint i = 0; i < LED_COUNT; ++i) {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }
}

/**
 * Mapeia coordenadas (x, y) para o índice linear do buffer de LEDs.
 * @param x Coordenada horizontal (0 a 4).
 * @param y Coordenada vertical (0 a 4).
 * @return Índice linear correspondente no buffer.
 */
int getIndex(int x, int y) {
    if (y % 2 == 0) {
        return 24 - (y * 5 + x); // Linhas pares: ordem esquerda para direita.
    } else {
        return 24 - (y * 5 + (4 - x)); // Linhas ímpares: ordem direita para esquerda.
    }
}

/**
 * Define a cor de um LED específico no buffer.
 * @param index Índice do LED no buffer.
 * @param r Componente vermelho (0 a 255).
 * @param g Componente verde (0 a 255).
 * @param b Componente azul (0 a 255).
 */
void MatrizRGBPI_SetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
}

/**
 * Limpa o buffer de pixels, apagando todos os LEDs.
 */
void MatrizRGBPI_Clear() {
    for (uint i = 0; i < LED_COUNT; ++i)
        MatrizRGBPI_SetLED(i, 0, 0, 0);
}

/**
 * Envia os dados do buffer para a matriz de LEDs.
 */
void MatrizRGBPI_Write() {
    // Envia cada componente de cor (G, R, B) para a máquina PIO.
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(matriz_pio, sm, leds[i].G);
        pio_sm_put_blocking(matriz_pio, sm, leds[i].R);
        pio_sm_put_blocking(matriz_pio, sm, leds[i].B);
    }
    sleep_us(100); // Aguarda 100us (sinal de RESET conforme o datasheet do WS2812B).
}

/**
 * Exibe uma letra na matriz de LEDs com a cor especificada.
 * @param letter Matriz 5x5x3 representando a letra (cada pixel pode estar ligado ou desligado).
 * @param r Componente vermelho da cor.
 * @param g Componente verde da cor.
 * @param b Componente azul da cor.
 */
void MatrizRGBPI_displayLetter(const int letter[5][5][3], uint8_t r, uint8_t g, uint8_t b) {
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            int position = getIndex(row, col);
            // Se o pixel na matriz da letra estiver "ligado", usa a cor especificada.
            if (letter[col][row][0] || letter[col][row][1] || letter[col][row][2]) {
                MatrizRGBPI_SetLED(position, r, g, b);
            } else {
                MatrizRGBPI_SetLED(position, 0, 0, 0);
            }
        }
    }
    MatrizRGBPI_Write();
    sleep_ms(1000); // Exibe a letra por 1 segundo.
}

/**
 * Exibe uma string na matriz de LEDs, caractere por caractere, com a cor especificada.
 * @param str String a ser exibida.
 * @param r Componente vermelho da cor.
 * @param g Componente verde da cor.
 * @param b Componente azul da cor.
 */
void MatrizRGBPI_displayString(const char *str, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < strlen(str); i++) {
        char c = toupper(str[i]); // Converte para maiúscula.
        // Busca a letra no dicionário (alphabet.h) e a exibe.
        for (int j = 0; j < ALPHABET_COUNT; j++) {
            if (alphabet[j].character == c) {
                MatrizRGBPI_displayLetter(alphabet[j].matrix, r, g, b);
                break;
            }
        }
    }
}

/**
 * Exibe um frame (imagem 5x5) na matriz de LEDs.
 * @param frame Matriz 5x5x3 contendo as cores de cada pixel.
 */
void MatrizRGBPI_displayFrame(const int frame[5][5][3]) {
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            int posicao = getIndex(x, y);
            MatrizRGBPI_SetLED(posicao, frame[y][x][0], frame[y][x][1], frame[y][x][2]);
        }
    }
    MatrizRGBPI_Write();
}

/**
 * Gera uma animação de scroll entre duas letras com a cor especificada.
 * @param currentLetter Letra atual.
 * @param nextLetter Próxima letra.
 * @param delay_ms Tempo de delay entre os frames da animação.
 * @param r Componente vermelho da cor.
 * @param g Componente verde da cor.
 * @param b Componente azul da cor.
 */
void scrollLetters(const Letter *currentLetter, const Letter *nextLetter, int delay_ms, uint8_t r, uint8_t g, uint8_t b) {
    int frame[5][5][3]; // Buffer para o frame atual.
    for (int offset = 0; offset <= 5; offset++) {
        // Preenche o frame com colunas da letra atual e da próxima letra.
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                if (x < 5 - offset) {
                    // Pega da letra atual (usa a cor especificada se o pixel estiver ligado).
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
                    // Pega da próxima letra (usa a cor especificada se o pixel estiver ligado).
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

/**
 * Exibe uma string com efeito de scroll e cor personalizada.
 * @param str String a ser exibida.
 * @param delay_ms Tempo de delay entre os frames da animação.
 * @param r Componente vermelho da cor.
 * @param g Componente verde da cor.
 * @param b Componente azul da cor.
 */
void MatrizRGBPI_displayStringWithScroll(const char *str, int delay_ms, uint8_t r, uint8_t g, uint8_t b) {
    const Letter *currentLetter = NULL;
    const Letter *nextLetter = NULL;
    for (int i = 0; i < strlen(str); i++) {
        char c = toupper(str[i]);
        // Busca a próxima letra no dicionário.
        for (int j = 0; j < ALPHABET_COUNT; j++) {
            if (alphabet[j].character == c) {
                nextLetter = &alphabet[j];
                break;
            }
        }
        // Se houver letra atual e próxima, gera o scroll.
        if (currentLetter != NULL && nextLetter != NULL) {
            scrollLetters(currentLetter, nextLetter, delay_ms, r, g, b);
        } else if (nextLetter != NULL) {
            // Exibe a primeira letra sem scroll.
            int frame[5][5][3];
            for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
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
            sleep_ms(delay_ms * 3); // Exibe por um tempo maior.
        }
        currentLetter = nextLetter;
    }
}
