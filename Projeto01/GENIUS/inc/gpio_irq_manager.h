// gpio_irq_manager.h
#ifndef GPIO_IRQ_MANAGER_H
#define GPIO_IRQ_MANAGER_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

#define MAX_GPIO_PINS 30  // Número máximo de pinos GPIO suportados

// Declaração do vetor de callbacks como extern
extern void (*callbacks[MAX_GPIO_PINS])(void);


// Protótipos das funções
void gpio_irq_handler(uint gpio, uint32_t events);
void register_gpio_callback(uint gpio, void (*callback)(void),uint32_t event_mask);
void remove_gpio_callback(uint gpio,uint32_t event_mask);
void gpio_irq_manager_init();

#endif // GPIO_IRQ_MANAGER_H
