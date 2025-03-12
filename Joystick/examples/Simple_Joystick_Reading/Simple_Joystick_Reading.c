#include <stdio.h>
#include "pico/stdlib.h"
#include "inc/JoystickPi.h"

int main() 
{
    stdio_init_all();  

    joystickPi_init();  

    while (true) 
    {
        
        joystick_state_t state = joystickPi_read();

        
        printf("X: %u, Y: %u, Button: %d\n", state.x, state.y, state.button);

        

        sleep_ms(100);  // Aguarda 100ms antes de ler novamente
    }

    return 0;
}
