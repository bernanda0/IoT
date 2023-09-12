#include <stdlib.h>

#define LED_TASK_PRIORITY 2
#define INPUT_TASK_PRIORITY 3
#define PRINT_TASK_PRIORITY 1

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static int blinkInterval = 500;
static TaskHandle_t xLedTaskHandle, xInputTaskHandle, xPrintTaskHandle;

// Fungsi tugas LED
void vLedTask(void *pvParameters) {
  pinMode(2, OUTPUT);
  bool ledState = LOW;

  while (true) {
    digitalWrite(2, ledState);
    ledState = !ledState;
    vTaskDelay(pdMS_TO_TICKS(blinkInterval)); // Menunda tugas selama 500 ms
  }
}

void vInputTask(void *pvParameters) {
  while (true) {
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      updateBlinkInterval(input.c_str());
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);  // Delay to avoid busy-waiting
  }
}

void vPrintTask(void *pvParameters) {
  while (true) {
    Serial.println(blinkInterval);
    vTaskDelay(pdMS_TO_TICKS(200)); 
  }
}

void updateBlinkInterval(const char* input) {
  if (input[0] == '+') {
    int value = atoi(&input[1]);
    blinkInterval += value;
  } else if (input[0] == '-') {
    int value = atoi(&input[1]);
    blinkInterval -= value;
  } else if (input[0] == '*') {
    int value = atoi(&input[1]);
    blinkInterval *= value;
  } else if (input[0] == '/') {
    int value = atoi(&input[1]);
    if (value != 0) { //division by 0 handling
      blinkInterval /= value;
    }
  }

  // Ensure that blinkInterval is greater than 0
  if (blinkInterval <= 0) {
    blinkInterval = 1;
  }
}

void setup() {
  Serial.begin(9600);
 
  xTaskCreatePinnedToCore(
    vLedTask,
    "LedTask",
    1000,
    NULL,
    1,
    &xLedTaskHandle,
    0
  );

  xTaskCreatePinnedToCore(
    vInputTask,
    "InputTask",
    1000,
    NULL,
    INPUT_TASK_PRIORITY,
    &xInputTaskHandle,
    0
  );

  xTaskCreatePinnedToCore(
    vPrintTask,
    "PrintTask",
    1000,
    NULL,
    PRINT_TASK_PRIORITY,
    &xPrintTaskHandle,
    0
  );

  // Menghapus task setup dan loop
  vTaskDelete(NULL);
}

void loop() {
}
