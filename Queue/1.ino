struct Data
{
  int id;
  int value;
};

QueueHandle_t xQueue;

void TaskSending(void *pvParameters)
{
  Data *d;
  int id = 0;
  while (true)
  {
    d = (Data *)pvPortMalloc(sizeof(Data));
    d->id = id++;
    d->value = random(0, 100);
    if (xQueueSend(xQueue, &d, portMAX_DELAY) != pdPASS)
    {
      Serial.println("Failed to post to queue");
      vPortFree(d); // Free memory if message could not be sent to the queue
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void TaskReceiving(void *pvParameters)
{
  Data *d;
  while (true)
  {
    if (xQueueReceive(xQueue, &d, portMAX_DELAY))
    {
      Serial.printf("{\"id\":%d,\"value\":%d}\n",d->id, d->value);
      vPortFree(d);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  xQueue = xQueueCreate(10, sizeof(int *));
  if (xQueue == NULL)
  {
    Serial.println("Error creating the queue");
  }
  xTaskCreatePinnedToCore(TaskSending, "TaskSending", 10000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskReceiving, "TaskReceiving", 10000, NULL, 1, NULL, 1);
  vTaskDelete(NULL);
}

void loop()
{
}