#include <stdio.h>
#include "pico/stdlib.h"
#include "inc/BuzzerPi.h"
#include "inc/melody.h"


int main()
{
    stdio_init_all();

    initialize_pwm(BUZZER_PIN);


    while (true) 
    {
    	play_melody(BUZZER_PIN,PiratesCaribeanMelody,PiratesCaribeanDurations,125.0f,sizeof(PiratesCaribeanMelody)/sizeof(PiratesCaribeanMelody[0]));
    	sleep_ms(1000);
        play_melody( BUZZER_PIN,MarchImperialMelody,MarchImperialDurations,125.0f,sizeof(MarchImperialMelody)/sizeof(MarchImperialMelody[0]));
        sleep_ms(1000);
        play_melody( BUZZER_PIN,ForEliseMelody,ForEliseDurations,125.0f,sizeof(ForEliseMelody)/sizeof(ForEliseMelody[0]));
        sleep_ms(1000);
    }

}
