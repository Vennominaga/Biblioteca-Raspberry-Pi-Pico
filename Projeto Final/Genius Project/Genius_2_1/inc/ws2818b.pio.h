// -------------------------------------------------- //
// Arquivo gerado automaticamente pelo pioasm - não editar! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

/**
 * @file ws2818b.pio.h
 * @brief Driver PIO para controle de LEDs WS2812B (NeoPixel)
 * 
 * Este arquivo contém o programa Assembly para a máquina de estados PIO
 * que controla a comunicação com tiras de LEDs WS2812B/NeoPixel.
 * Gerado automaticamente pelo pioasm (não modificar manualmente).
 */

// --- CONSTANTES ---
#define ws2818b_wrap_target 0  // Índice inicial do loop
#define ws2818b_wrap 3         // Índice final do loop

// --- PROGRAMA PIO ---
// Instruções em Assembly para a máquina PIO
static const uint16_t ws2818b_program_instructions[] = {
    //     .wrap_target
    0x6221, // 0: out    x, 1            side 0 [2]  // Envia 1 bit com delay
    0x1123, // 1: jmp    !x, 3           side 1 [1]  // Pula se bit = 0
    0x1400, // 2: jmp    0               side 1 [4]  // Loop principal
    0xa442, // 3: nop                    side 0 [4]  // Operação vazia
    //     .wrap
};

#if !PICO_NO_HARDWARE
// --- CONFIGURAÇÃO DO PROGRAMA PIO ---
static const struct pio_program ws2818b_program = {
    .instructions = ws2818b_program_instructions,
    .length = 4,      // Número de instruções
    .origin = -1,     // Sem origem fixa (alocação dinâmica)
};

/**
 * @brief Obtém a configuração padrão para o programa WS2812B
 * @param offset Offset do programa no PIO
 * @return Configuração inicial da máquina de estados
 */
static inline pio_sm_config ws2818b_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + ws2818b_wrap_target, offset + ws2818b_wrap);
    sm_config_set_sideset(&c, 1, false, false); // Configura pinos sideset
    return c;
}

/**
 * @brief Inicializa o programa PIO para controle de LEDs WS2812B
 * @param pio Instância PIO (0 ou 1)
 * @param sm Máquina de estados (0-3)
 * @param offset Offset do programa no PIO
 * @param pin Pino GPIO conectado aos LEDs
 * @param freq Frequência de comunicação (em Hz)
 */
static void ws2818b_program_init(PIO pio, uint sm, uint offset, uint pin, float freq) {
    // Configuração do pino GPIO
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    
    // Configuração do programa
    pio_sm_config c = ws2818b_program_get_default_config(offset);
    sm_config_set_sideset_pins(&c, pin);          // Usa pino sideset
    sm_config_set_out_shift(&c, true, true, 8);   // Shift right, 8 bits
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX); // Usa apenas FIFO TX
    
    // Calcula divisor de clock
    float prescaler = clock_get_hz(clk_sys) / (10.f * freq);
    sm_config_set_clkdiv(&c, prescaler);
    
    // Inicializa máquina de estados
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

#endif
