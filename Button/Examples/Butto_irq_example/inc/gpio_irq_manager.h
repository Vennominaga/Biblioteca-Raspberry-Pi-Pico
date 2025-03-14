/*--------------DOCUMENTAÇÃO------------------*/

// gpio_irq_manager.h
#ifndef GPIO_IRQ_MANAGER_H
#define GPIO_IRQ_MANAGER_H

// Inclusão de bibliotecas necessárias 
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

#define MAX_GPIO_PINS 30  // Número máximo de pinos GPIO suportados

// Declaração do vetor de ponteiros para funções de callbacks como extern
extern void (*callbacks[MAX_GPIO_PINS])(void); // Armazena os callbacks associados a cada pino GPIO 


// Protótipos das funções

/*
* @brief Manipulador das interrupções para cada GPIO
* @param gpio Número do pino que gerou a interrupção 
* @param events Tipo de evento que ocorreu (Ex: Borda de subida/descida)
*/ 
void gpio_irq_handler(uint gpio, uint32_t events);
void register_gpio_callback(uint gpio, void (*callback)(void),uint32_t event_mask);
void remove_gpio_callback(uint gpio,uint32_t event_mask);
void gpio_irq_manager_init();

#endif // GPIO_IRQ_MANAGER_H
