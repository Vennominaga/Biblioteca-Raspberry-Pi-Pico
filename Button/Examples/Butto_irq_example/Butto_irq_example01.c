#include "pico/stdlib.h"
#include "inc/ButtonPi.h"


#include <stdio.h>


void button_5_callback(void) 
{
    gpio_put(12, 0); 
    gpio_put(13, !gpio_get(13));
    printf("Botão no pino 5 pressionado! LED vermelho aceso.\n");
}

void button_6_callback(void) 
{
    gpio_put(13, 0);
    gpio_put(12, !gpio_get(12));
    printf("Botão no pino 6 pressionado! LED azul aceso.\n");
}

int main() 
{
    stdio_init_all();  

    
    gpio_init(13);
    gpio_init(12);
    gpio_set_dir(13, GPIO_OUT);
    gpio_set_dir(12, GPIO_OUT);

    
    ButtonPi btn5;
    ButtonPi btn6;

   
    ButtonPi_init(&btn5, 5);
    ButtonPi_init(&btn6, 6);

   
    ButtonPi_attach_callback(&btn5, button_5_callback);
    ButtonPi_attach_callback(&btn6, button_6_callback);

    while (1) 
    {
        tight_loop_contents();  
    }

    return 0;
}
