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

static void binary_count(){
    while (1) {
        if (currState == 1){
            //count forward in binary
            printf("Counting forward\n");
            countUp = 1;
            gpio_set_level(4, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);
            gpio_set_level(5, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);
            gpio_set_level(5, 0);
            gpio_set_level(26, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);
            gpio_set_level(5, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);
            gpio_set_level(26, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);
            gpio_set_level(5, 0);
            gpio_set_level(26, 0);
            gpio_set_level(25, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);
            gpio_set_level(5, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);
            gpio_set_level(5, 0);
            gpio_set_level(26, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);
            gpio_set_level(5, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);
            gpio_set_level(26, 1);
        }
        else {
            //count backwards in binary
            printf("Counting backwards\n");
            countUp = 0;
            gpio_set_level(4, 1);
            gpio_set_level(5, 1);
            gpio_set_level(26, 1);
            gpio_set_level(25, 1);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);
            
            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);
            gpio_set_level(5, 0);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);
            gpio_set_level(5, 1);
            gpio_set_level(26, 0);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);
            gpio_set_level(5, 0);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);
            gpio_set_level(5, 1);
            gpio_set_level(26, 1);
            gpio_set_level(25, 0);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);
            gpio_set_level(5, 0);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);
            gpio_set_level(5, 1);
            gpio_set_level(26, 0);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 1);
            gpio_set_level(5, 0);

            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            gpio_set_level(4, 0);
        }
    }
}

static void set_led(){
    while (1) {
        //set the LED to the current state
        if(countUp == 1){
            gpio_set_level(15, 1);  //LED on means we are counting forward
            printf("LED on\n");
        }
        else{
            gpio_set_level(15, 0);
            printf("LED off\n");
        }
        printf("Current state: %d\n", currState);
        printf("Flag: %d\n", flag);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Button task
static void button_status(){
    while(1) {
        flag = gpio_get_level(BUTTON);
        if (flag == 1 && currState == 0){
            currState = 1;
        }
        else if (flag == 1 && currState == 1){
            currState = 0;
        }
        else {
            currState = currState;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
  // Initialize button  
  gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
  configure_led(4);
  configure_led(5);
  configure_led(26);
  configure_led(25);   

  //indicator blue LED
  configure_led(15);

  xTaskCreate(binary_count, "binary count",1024*2, NULL, configMAX_PRIORITIES-2, NULL);
  xTaskCreate(button_status, "button status", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
  xTaskCreate(set_led, "set led", 1024*2, NULL, configMAX_PRIORITIES-3, NULL);
}