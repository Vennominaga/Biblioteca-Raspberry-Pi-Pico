#include "inc/ButtonPi.h"
#include "inc/gpio_irq_manager.h" 


void ButtonPi_init(ButtonPi *btn, uint pin) 
{
    btn->pin = pin;
    btn->last_state = gpio_get(pin); // Inicializa o último estado
    //btn->callback = NULL; // Inicializa o callback como NULL

    gpio_init(pin); // Inicializa o pino
    gpio_set_dir(pin, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(pin); // Habilita o resistor de pull-up interno (assumindo que o botão está conectado ao GND)
}

bool ButtonPi_read(ButtonPi *btn) 
{
    return !gpio_get(btn->pin); // Retorna o estado do botão (true se pressionado, false se liberado)
}

void ButtonPi_attach_callback(ButtonPi *btn,void (*callback)(void)) 
{
    gpio_irq_manager_init();// Garante que a Interrupção esteja Ativada Independente

    if (btn->pin < 30) 
    { // Verifica se o pino é válido
        //btn->callback = callback; // Armazena o callback na struct
        
        register_gpio_callback(btn->pin,callback, GPIO_IRQ_EDGE_FALL); // Configura a interrupção na borda de descida
    }

    //register_gpio_callback(btn->pin,callback, GPIO_IRQ_EDGE_FALL); // Configura a interrupção na borda de descida
}