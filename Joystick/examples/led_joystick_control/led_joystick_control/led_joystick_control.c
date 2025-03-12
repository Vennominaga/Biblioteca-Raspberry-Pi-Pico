#include <stdio.h>
#include "pico/stdlib.h"
#include "inc/JoystickPi.h"


#define LED_PIN 12  

int main() {
    stdio_init_all();  

    // Inicializa o joystick e o LED
    joystickPi_init();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    bool led_state = false;  

    while (true) 
    {
      
        joystick_state_t state = joystickPi_read();

        
        int16_t x_mapped = joystickPi_map_value(state.x, 0, 4095, -100, 100);

      
        if (x_mapped > 50) 
        {
            gpio_put(LED_PIN, 1); 
            led_state = true;
        } else if (x_mapped < -50) 
        {
            gpio_put(LED_PIN, 0);  
            led_state = false;
        }

        
        if (state.button) 
        {
            led_state = !led_state;  
            gpio_put(LED_PIN, led_state);
            sleep_ms(300);  
        }

       
        printf("X: %d, Button: %d, LED: %d\n", x_mapped, state.button, led_state);

        sleep_ms(100);  // Aguarda 100ms antes de ler novamente
    }

    return 0;
}
