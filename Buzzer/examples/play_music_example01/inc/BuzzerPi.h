#ifndef BUZZER_PI_H
#define BUZZER_PI_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define CLK_DIV_DEFAULT 125.0f
#define BUZZER_PIN 21

void initialize_pwm(uint pin);
uint16_t calculate_wrap(uint32_t target_frequency, float clkdiv);

void play_tone(uint pin, uint32_t freq, uint duration_ms);
void play_tone_clkdiv(uint pin, int freq, int duration_ms,float clkdiv);

void play_melody(uint pin,int *melody,int *durations,float clkdiv,int length);

void beep(uint pin,int freq,int duration,int repetion);

#endif
