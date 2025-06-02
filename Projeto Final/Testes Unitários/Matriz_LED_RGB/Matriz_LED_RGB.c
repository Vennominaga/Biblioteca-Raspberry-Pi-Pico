#include <stdio.h>              // Biblioteca padrão de entrada/saída
#include "pico/stdlib.h"        // Biblioteca específica do Raspberry Pi Pico
#include "inc/MatrizRGBPI.h"    // Biblioteca personalizada para controle da matriz RGB

int main()
{
    // Inicializa a comunicação serial (para possível debug)
    stdio_init_all();
    
    // Inicializa a matriz de LEDs RGB no pino definido por LED_PIN
    MatrizRGBPI_Init(LED_PIN);

    // Loop principal infinito
    while (true) {
        // Exibe a palavra "Led" com scroll:
        // - Velocidade: 250ms entre movimentos
        // - Cor: RGB (20,20,20) - branco fraco
        MatrizRGBPI_displayStringWithScroll("Led ", 250, 20, 20, 20);
        
        // Exibe "Matriz " com cor verde-água (R:20, G:10, B:0)
        MatrizRGBPI_displayStringWithScroll("Matriz ", 250, 20, 10, 0);
        
        // Exibe letras individuais com cores primárias:
        // "R" em vermelho puro (R:18)
        MatrizRGBPI_displayStringWithScroll("R", 250, 18, 0, 0);
        // "G" em verde puro (G:18)
        MatrizRGBPI_displayStringWithScroll("G", 250, 0, 18, 0);
        // "B" em azul puro (B:18)
        MatrizRGBPI_displayStringWithScroll("B", 250, 0, 0, 18);
        
        // Exibe o símbolo "<" em roxo (R:18, B:18)
        MatrizRGBPI_displayStringWithScroll("<", 250, 18, 0, 18);
        
        // O loop continua indefinidamente, repetindo a sequência de mensagens
    }
}
