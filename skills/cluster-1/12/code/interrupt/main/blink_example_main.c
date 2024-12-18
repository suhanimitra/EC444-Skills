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

// Hardware interrupt definitions
#define GPIO_INPUT_IO_1       32
int flag = 0;     // Global flag for signaling from ISR
int buttonState = 0;

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

// Define button interrupt handler -- just sets the flag on interrupt
static void IRAM_ATTR gpio_isr_handler(void* arg){
  flag = 1;
}

//From the docs/design-patterns/docs/dp-interrupts.md file, using the first design implementation
// Intialize the GPIO to detect button press as interrupt
static void button_init() {
  gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_POSEDGE; // interrupt of rising edge
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL; // bit mask of the pins, use GPIO4 here
    io_conf.mode = GPIO_MODE_INPUT;            // set as input mode
    io_conf.pull_up_en = 1;                    // enable resistor pull-up mode on pin
  gpio_config(&io_conf);                       // apply parameters
  gpio_intr_enable(GPIO_INPUT_IO_1 );          // enable interrupts on pin
  gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);   //install gpio isr service
  gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1); //hook isr handler for specific gpio pin
}

const int leds[] = {15, 5, 26, 25};
static int currLED = 0;

static void allLED(){
    int prevState = gpio_get_level(BUTTON);
    gpio_set_level(leds[currLED], 1);
    while(1) {
        buttonState = gpio_get_level(BUTTON);
        printf("Flag: %d\n", buttonState);
        if (buttonState != prevState && buttonState == 1) { //if the button is pressed
            gpio_set_level(leds[currLED], 0);
            currLED = (currLED + 1) % 4;        //increment the current LED
            gpio_set_level(leds[currLED], 1);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


// The main program (1) intializes the button interrupts and (2) starts the task
void app_main(void)
{
  // Initialize button  
  gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
  configure_led(15);
  configure_led(5);
  configure_led(26);
  configure_led(25);   

  button_init();     // Initialize button config 
  xTaskCreate(allLED, "allLED ", 1024*2, NULL, configMAX_PRIORITIES-1, NULL); // Create task
  printf("Everything initialized. Waiting for button presses...\n"); // console message that it has begun
  while(1) {                               // loop forever in this task
    vTaskDelay(100 / portTICK_PERIOD_MS);  // wait a bit
  }  
}