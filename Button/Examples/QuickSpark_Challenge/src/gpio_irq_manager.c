// gpio_irq_manager.c
#include "inc/gpio_irq_manager.h"
#define DEBOUNCE_DELAY_MS 200 
// Definição do vetor de callbacks
void (*callbacks[MAX_GPIO_PINS])(void)={NULL};

// Array para armazenar o último tempo de interrupção para cada pino
absolute_time_t last_interrupt_time[MAX_GPIO_PINS];

// Função mestra que será chamada quando uma interrupção ocorrer
void gpio_irq_handler(uint gpio, uint32_t events) 
{
    // Verifica se o pino é válido e se há um callback registrado
    if (gpio < MAX_GPIO_PINS && callbacks[gpio] != NULL) 
    {
        // Obtém o tempo atual
        absolute_time_t now = get_absolute_time();

        // Verifica se o tempo desde a última interrupção é maior que o tempo de debounce
        if (absolute_time_diff_us(last_interrupt_time[gpio], now) > DEBOUNCE_DELAY_MS * 1000) 
        {
            // Atualiza o tempo da última interrupção
            last_interrupt_time[gpio] = now;

            // Chama a função de callback correspondente ao pino
            callbacks[gpio]();
        }
    }
}


void register_gpio_callback(uint gpio, void (*callback)(void), uint32_t event_mask) 
{
    if (gpio < MAX_GPIO_PINS) 
    {
        callbacks[gpio] = callback;  // Armazena a função no vetor de callbacks
        gpio_set_irq_enabled(gpio, event_mask, true);  // Habilita a interrupção para o evento especificado
    }
    
}


// Função para remover um callback de um pino
void remove_gpio_callback(uint gpio,uint32_t event_mask) 
{
    if (gpio < MAX_GPIO_PINS) 
    {
        callbacks[gpio] = NULL;  // Remove a função do vetor de callbacks
        gpio_set_irq_enabled(gpio, event_mask, false);  // Desabilita interrupção
    }
}

// Função de inicialização da biblioteca
void gpio_irq_manager_init() 
{
    /*for (int i = 0; i < MAX_GPIO_PINS; i++) 
    {
        callbacks[i] = NULL;  // Inicializa todos os callbacks como NULL
    }*/
    gpio_set_irq_callback(gpio_irq_handler);  // Configura a função mestra como callback global
    irq_set_enabled(IO_IRQ_BANK0, true);      // Habilita interrupções no banco de GPIOs
}
