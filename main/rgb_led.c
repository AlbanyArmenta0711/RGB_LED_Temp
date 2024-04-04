#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "rgb_led.h"

bool led_pwm_initialized = false;

/*
 * Function to initialize led
*/
static void rgb_led_pwm_init(){
    //1.- Timer configuration
    ledc_timer_config_t timer_config = {
        .speed_mode = SPEED_MODE,
        .timer_num = TIMER,
        .freq_hz = FREQ, 
        .duty_resolution = DUTY_RES
    };
    //set the timer
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    //2.- Channel configuration

    //Red Channel
    ledc_channel_config_t red_channel_config = {
        .gpio_num = RED_IO, 
        .speed_mode = SPEED_MODE, 
        .channel = RED_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = TIMER,
        .duty = 0,
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&red_channel_config));

    //Green Channel
    ledc_channel_config_t green_channel_config = {
        .gpio_num = GREEN_IO, 
        .speed_mode = SPEED_MODE, 
        .channel = GREEN_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = TIMER,
        .duty = 0,
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&green_channel_config));

    //Blue Channel
    ledc_channel_config_t blue_channel_config = {
        .gpio_num = BLUE_IO, 
        .speed_mode = SPEED_MODE, 
        .channel = BLUE_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = TIMER,
        .duty = 0,
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&blue_channel_config));

    led_pwm_initialized = true;
}

/*
 * Function to change RGB led color according to RGB channel values
 * @param red new red value to be assigned
 * @param green new green value to be assigned
 * @param blue new blue value to be assigned
*/
static void rgb_led_change_color(uint8_t red, uint8_t green, uint8_t blue){
    //Check if pwm channels have been initialized
    if(!led_pwm_initialized){
        rgb_led_pwm_init();
    }
    //Set and update pwm duty cycles for each PWM channel configured
    ledc_set_duty(SPEED_MODE, RED_CHANNEL, red);
    ledc_update_duty(SPEED_MODE, RED_CHANNEL);

    ledc_set_duty(SPEED_MODE, GREEN_CHANNEL, green);
    ledc_update_duty(SPEED_MODE, GREEN_CHANNEL);

    ledc_set_duty(SPEED_MODE, BLUE_CHANNEL, blue);
    ledc_update_duty(SPEED_MODE, BLUE_CHANNEL);
}

/*
 * Function to change RGB led color according to RGB channel values in str format separated by
 * commas
 * @param red new red value to be assigned
 * @param green new green value to be assigned
 * @param blue new blue value to be assigned
*/
void update_color(char * color){
    char * token; 
    char delim[2] = ",";
    uint8_t red, green, blue; 
    token = strtok(color, delim);
    red = atoi(token);
    token = strtok(NULL, delim);
    green = atoi(token);
    token = strtok(NULL, delim);
    blue = atoi(token);
    rgb_led_change_color(red, green, blue);
}

