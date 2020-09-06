#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#define NUM_INPUTS 5

#define GPIO_INPUT_IO_0    27
#define GPIO_INPUT_IO_1    32
#define GPIO_INPUT_IO_2    33
#define GPIO_INPUT_IO_3    25
#define GPIO_INPUT_IO_4    26

#define GPIO_INPUT_PIN_SEL  (1<<GPIO_INPUT_IO_0 | 1ULL<<GPIO_INPUT_IO_1 | 1ULL<<GPIO_INPUT_IO_2 | 1<<GPIO_INPUT_IO_3 | 1<<GPIO_INPUT_IO_4)

#define UPDATE_RATE           (20)
#define LONG_PRESS_THRESHOLD  (500 / UPDATE_RATE)
#define IDLE_TIME             (200 / UPDATE_RATE)

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef enum input_fsm_state_t {
  INPUT_INIT,
  INPUT_IDLE,
  INPUT_PRESSED,
  INPUT_MAX
} input_fsm_state_t;

typedef struct input_context_t {
  input_fsm_state_t state;
  uint8_t  io;
  uint8_t  id;
  uint8_t  input_count;
} input_context_t;

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
uint8_t inputIO[NUM_INPUTS] = {GPIO_INPUT_IO_0, GPIO_INPUT_IO_1, GPIO_INPUT_IO_2, GPIO_INPUT_IO_3, GPIO_INPUT_IO_4};
bool currentState[NUM_INPUTS] = {0};

/******************************************************************************
* Function Definitions
*******************************************************************************/

/***
 *
 * @param ctx
 */
static void InputStateInit(input_context_t *ctx) {
  gpio_config_t io_conf;
  //disable interrupt
  io_conf.intr_type = GPIO_INTR_DISABLE;
  //set as output mode
  io_conf.mode = GPIO_MODE_INPUT;
  //bit mask of the pins that you want to set,e.g.GPIO18/19
  io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
  //disable pull-down mode
  io_conf.pull_down_en = 0;
  //disable pull-up mode
  io_conf.pull_up_en = 1;
  //configure GPIO with the given settings
  gpio_config(&io_conf);

  ctx->state = INPUT_IDLE;
}

/***
 *
 * @param ctx
 */
static void InputStateIdle(input_context_t *ctx) {
  if (gpio_get_level(ctx->io) == 0) {
    ctx->state = INPUT_PRESSED;
    currentState[ctx->id] = true;
  }
}

/***
 *
 * @param ctx
 */
static void InputStatePressed(input_context_t *ctx) {
  if (gpio_get_level(ctx->io) == 1) {
    ctx->state = INPUT_IDLE;
    currentState[ctx->id] = false;
    return;
  }
}

/***
 *
 * @param pvParameters
 */
static void InputTask(void *pvParameters) {
  input_context_t ctx[NUM_INPUTS] = {0};
  bool exit = false;

  void (*input_fsm[INPUT_MAX])(input_context_t *) =  {
      InputStateInit,
      InputStateIdle,
      InputStatePressed,
  };

  for (size_t i = 0; i < NUM_INPUTS; i++) {
    ctx[i].io = inputIO[i];
    ctx[i].id = (uint8_t) i;
    ctx[i].state = INPUT_INIT;
  }

  while (!exit) {
    for (size_t i = 0; i < NUM_INPUTS; i++) {
      input_fsm[ctx[i].state](&ctx[i]);
    }

    vTaskDelay(UPDATE_RATE / portTICK_PERIOD_MS);
  }

  vTaskDelete(NULL);
}

/***
 *
 */
void InputTaskInit(void) {
  xTaskCreate(&InputTask, "Input Task", 2048, NULL, 5, NULL);
}

bool InputTaskGetState(uint8_t inputNum) {
  if (inputNum < NUM_INPUTS)
    return currentState[inputNum];

  return false;
}