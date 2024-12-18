/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "example";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO
#ifdef CONFIG_BLINK_LED_STRIP

static led_strip_handle_t led_strip;

#elif CONFIG_BLINK_LED_GPIO

#define ESP_INTR_FLAG_DEFAULT 0
#define GPIO_INPUT_PIN_SEL    1ULL<<GPIO_INPUT_IO_1
#define BUTTON 32

int flag = 0;
int currState = 0;      //variable so we can also keep account of the previous state
int countUp = 0;        //variable to aid with LED toggling

static void configure_led(int gpin)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(gpin);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(gpin, GPIO_MODE_OUTPUT);
}

#else
#error "unsupported LED type" 
#endif

// Button task

const int leds[] = {15, 5, 26, 25};
static int currLED = 0;

static void allLED(){
    int prevState = gpio_get_level(BUTTON);
    gpio_set_level(leds[currLED], 1);
    while(1) {
        flag = gpio_get_level(BUTTON);
        printf("Flag: %d\n", flag);
        if (flag != prevState && flag == 1) { //if the button is pressed
            gpio_set_level(leds[currLED], 0);   //turn off the current LED
            currLED = (currLED + 1) % 4;        //increment the current LED
            gpio_set_level(leds[currLED], 1);   //turn on the next LED
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
  // Initialize button  
  gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
  configure_led(15);
  configure_led(5);
  configure_led(26);
  configure_led(25);   

  xTaskCreate(allLED, "allLED ", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
}