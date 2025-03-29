#include <stdio.h>
#include "pico/stdlib.h"
#include "inc/MatrizRGBPI.h"


int main()
{
    stdio_init_all();
    MatrizRGBPI_Init(LED_PIN);

    //MatrizRGBPI_Init(LED_PIN);

    while (true) {

      
        MatrizRGBPI_displayStringWithScroll("Led ",250,20,20,20);
        MatrizRGBPI_displayStringWithScroll("Matriz ",250,20,10, 0);
        MatrizRGBPI_displayStringWithScroll("R",250,18,0,0);
        MatrizRGBPI_displayStringWithScroll("G",250,0,18,0);
        MatrizRGBPI_displayStringWithScroll("B",250,0,0,18);
        
	MatrizRGBPI_displayStringWithScroll("<",250,18,0,18);





       
    }
}
