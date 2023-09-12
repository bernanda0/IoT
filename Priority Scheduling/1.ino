#define LED_TASK_PRIORITY 1
#define BUTTON_TASK_PRIORITY 2

TaskHandle_t xLedTaskHandle, xButtonTaskHandle;

static const int btn_pin = 15;

void vLedTask(void *parameter){
  pinMode(LED_BUILTIN, OUTPUT);
  while(1){
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void vButtonTask(void *parameter){
  pinMode(btn_pin, INPUT_PULLUP);
  while (1) {
    int buttonState = digitalRead(btn_pin);
    
    if (buttonState == LOW) {
      vTaskSuspend(xLedTaskHandle); // Menangguhkan tugas LED
      Serial.println("Suspend bro");
    } else {
      vTaskResume(xLedTaskHandle); // Melanjutkan tugas LED
      Serial.println("Resume bro");
    }

    vTaskDelay(pdMS_TO_TICKS(200)); 
  }
}
void setup() {
  Serial.begin(9600);  
  xTaskCreatePinnedToCore(
    vLedTask,
    "LedTask",
    1024,
    NULL,
    LED_TASK_PRIORITY,
    &xLedTaskHandle,
    0
  );

  xTaskCreatePinnedToCore(
    vButtonTask,
    "ButtonTask",
    1024,
    NULL,
    BUTTON_TASK_PRIORITY,
    &xButtonTaskHandle,
    0
  );

  vTaskDelete(NULL);
}

void loop() {

}

