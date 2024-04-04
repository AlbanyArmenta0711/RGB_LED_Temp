
#include "driver/ledc.h"

#define SPEED_MODE LEDC_HIGH_SPEED_MODE
#define TIMER  LEDC_TIMER_0
#define DUTY_RES  LEDC_TIMER_8_BIT
#define FREQ  100
#define RED_IO  21
#define RED_CHANNEL LEDC_CHANNEL_0
#define GREEN_IO 22
#define GREEN_CHANNEL LEDC_CHANNEL_1
#define BLUE_IO 23
#define BLUE_CHANNEL LEDC_CHANNEL_2

void update_color(char * color);