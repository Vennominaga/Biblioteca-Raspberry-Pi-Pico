#include <stdio.h>
#include "inc/JoystickPi.h"

#define MATRIX_SIZE 10


char matrix[MATRIX_SIZE][MATRIX_SIZE];


int turtle_x = 0;
int turtle_y = 0;


bool pen_down = false;


void initialize_matrix() 
{
    for (int i = 0; i < MATRIX_SIZE; i++) 
    {
        for (int j = 0; j < MATRIX_SIZE; j++) 
        {
            matrix[i][j] = ' ';
        }
    }
}


void print_matrix() 
{
   
    printf("┌");
    for (int i = 0; i < MATRIX_SIZE; i++) 
    {
        printf("───");
        if (i < MATRIX_SIZE - 1) printf("┬");
    }
    printf("┐\n");

   
    for (int i = 0; i < MATRIX_SIZE; i++) 
    {
        printf("│");
        for (int j = 0; j < MATRIX_SIZE; j++) 
        {
            printf(" %c ", matrix[i][j]);
            printf("│");
        }
        printf("\n");

       
        if (i < MATRIX_SIZE - 1) 
        {
            printf("├");
            for (int j = 0; j < MATRIX_SIZE; j++) {
                printf("───");
                if (j < MATRIX_SIZE - 1) printf("┼");
            }
            printf("┤\n");
        }
    }

    // Desenha a borda inferior da matriz
    printf("└");
    for (int i = 0; i < MATRIX_SIZE; i++) 
    {
        printf("───");
        if (i < MATRIX_SIZE - 1) printf("┴");
    }
    printf("┘\n");

    
    printf("Posição da tartaruga: [%d][%d]\n", turtle_x, turtle_y);
    printf("Estado da caneta: %s\n", pen_down ? "Para baixo (escrevendo)" : "Para cima (não escrevendo)");
    
}


void move_turtle(joystick_state_t state) 
{
   
    int16_t x_mapped = joystickPi_map_value(state.x, 0, 4095, -2, 2);
    int16_t y_mapped = joystickPi_map_value(state.y, 4095, 0, -1, 2);

   
    turtle_x += x_mapped;
    turtle_y += y_mapped;

   
    if (turtle_x < 0) turtle_x = 0;
    if (turtle_x >= MATRIX_SIZE) turtle_x = MATRIX_SIZE - 1;
    if (turtle_y < 0) turtle_y = 0;
    if (turtle_y >= MATRIX_SIZE) turtle_y = MATRIX_SIZE - 1;

    
    if (pen_down) 
    {
        matrix[turtle_y][turtle_x] = '*';
    }
}

int main() {
    stdio_init_all();
   
    joystickPi_init();

   
    initialize_matrix();

   
    while (1) 
    {
        
        joystick_state_t state = joystickPi_read();

        
        move_turtle(state);

        
        if (state.button) 
        {
            pen_down = !pen_down;
            
            while (joystickPi_read_button()) {}
        }

       
        printf("\033[H\033[J");  
        print_matrix();

        
        sleep_ms(100);
    }

    return 0;
}
