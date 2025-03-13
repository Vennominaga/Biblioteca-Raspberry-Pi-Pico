#include "inc/BuzzerPi.h"
#include "hardware/clocks.h"
#include <stdio.h>
void initialize_pwm(uint pin)
{
    gpio_set_function(pin,GPIO_FUNC_PWM);

}

uint16_t calculate_wrap(uint32_t target_frequency, float clkdiv) 
{
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t wrap = (clock_freq / (target_frequency * clkdiv)) - 1;
    return (wrap > 65535) ? 65535 : wrap;

}

void play_tone(uint pin,uint32_t freq,uint duration_ms)
{
    uint slice_num = pwm_gpio_to_slice_num(pin);

    uint16_t wrap_value = calculate_wrap(freq,CLK_DIV_DEFAULT);


    pwm_set_wrap(slice_num,wrap_value);

    pwm_set_clkdiv(slice_num,CLK_DIV_DEFAULT);

    pwm_set_gpio_level(pin,wrap_value/2);

    pwm_set_enabled(slice_num,true);

    sleep_ms(duration_ms);

    pwm_set_gpio_level(pin,0);

}


void play_tone_clkdiv(uint pin, int freq, int duration_ms,float clkdiv)
{
    uint slice_num = pwm_gpio_to_slice_num(pin);

    uint16_t wrap_value = calculate_wrap(freq,clkdiv);


    pwm_set_wrap(slice_num,wrap_value);

    pwm_set_clkdiv(slice_num,clkdiv);

    pwm_set_gpio_level(pin,wrap_value/2);

    pwm_set_enabled(slice_num,true);

    sleep_ms(duration_ms);

    pwm_set_gpio_level(pin,0);

}

void play_melody(uint pin,int *melody,int *durations,float clkdiv,int length)
{

    for (int i=0;i<length;i++)
    {
       if(melody[i]!=0)
       {
        play_tone_clkdiv(pin,melody[i],durations[i],clkdiv);

       }

       else
       {
        sleep_ms(durations[i]);
       }
       

    }

}


void beep(uint pin,int freq,int duration,int repetion)
{

    for(int i=0;i<repetion;i++)
    {
        play_tone(pin,freq,duration);
        sleep_ms(500);
    }

}