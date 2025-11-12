#include "HomeSpan.h"           // Библиотека HomeSpan для HomeKit интеграции / HomeSpan library for HomeKit integration
#include <Adafruit_Sensor.h>    // Библиотека датчиков Adafruit / Adafruit sensor library
#include <DHT.h>                // Библиотека DHT датчиков / DHT sensor library
#include <DHT_U.h>              // Расширенная библиотека DHT / Extended DHT library

#define DHTPIN 4                // Пин к которому подключен датчик DHT / Pin connected to DHT sensor
#define DHTTYPE DHT22           // Тип датчика - DHT22 / Sensor type - DHT22

DHT_Unified dht(DHTPIN, DHTTYPE);  // Создание объекта датчика / Create sensor object
uint32_t delayMS;               // Переменная для хранения задержки между измерениями / Variable for storing delay between measurements

// Настройка пинов / Pin configuration
int relayPin = 13;              // Пин подключения реле / Relay connection pin
int buttonPin = 8;              // Пин подключения кнопки / Button connection pin

// Переменные состояния / State variables
int lastButtonState = HIGH;     // Предыдущее состояние кнопки / Previous button state
SpanCharacteristic *lightPower; // Указатель на характеристику мощности света / Pointer to light power characteristic
bool lastLightState = false;    // Предыдущее состояние света / Previous light state

// Указатели на характеристики датчиков / Pointers to sensor characteristics
SpanCharacteristic *tempCharacteristic;     // Характеристика температуры / Temperature characteristic
SpanCharacteristic *humidityCharacteristic; // Характеристика влажности / Humidity characteristic

// Переменные для хранения показаний датчика / Variables for storing sensor readings
float currentTemperature = 0;   // Текущая температура / Current temperature
float currentHumidity = 0;      // Текущая влажность / Current humidity
unsigned long lastSensorRead = 0; // Время последнего чтения датчика / Last sensor reading time

void setup() {
  Serial.begin(115200);         // Инициализация последовательного порта / Initialize serial port

  // Настройка пинов / Pin configuration
  pinMode(relayPin, OUTPUT);    // Настройка пина реле как выход / Set relay pin as output
  pinMode(buttonPin, INPUT_PULLUP); // Настройка пина кнопки как вход с подтяжкой / Set button pin as input with pullup

  // Начальное состояние реле (выключено) / Initial relay state (off)
  digitalWrite(relayPin, HIGH); // Выключение реле (активно по LOW) / Turn off relay (active LOW)
  
  // Инициализация датчика DHT22 / Initialize DHT22 sensor
  dht.begin();                  // Запуск датчика / Start sensor
  sensor_t sensor;              // Структура для информации о датчике / Structure for sensor information
  
  // Получаем параметры датчика температуры / Get temperature sensor parameters
  dht.temperature().getSensor(&sensor);
  
  // Закомментированная отладочная информация / Commented debug information
  /*Serial.println("=== ДАТЧИК ТЕМПЕРАТУРЫ ===");
  Serial.print("Датчик:          "); Serial.println(sensor.name);
  Serial.print("Макс. значение:  "); Serial.print(sensor.max_value); Serial.println(" °C");
  Serial.print("Мин. значение:   "); Serial.print(sensor.min_value); Serial.println(" °C");
  Serial.print("Разрешение:      "); Serial.print(sensor.resolution); Serial.println(" °C");
  Serial.print("Задержка:        "); Serial.print(sensor.min_delay / 1000); Serial.println(" мс");*/
  
  // Получаем параметры датчика влажности / Get humidity sensor parameters
  dht.humidity().getSensor(&sensor);
  /* Serial.println("=== ДАТЧИК ВЛАЖНОСТИ ===");
  Serial.print("Датчик:          "); Serial.println(sensor.name);
  Serial.print("Макс. значение:  "); Serial.print(sensor.max_value); Serial.println(" %");
  Serial.print("Мин. значение:   "); Serial.print(sensor.min_value); Serial.println(" %");
  Serial.print("Разрешение:      "); Serial.print(sensor.resolution); Serial.println(" %");
  Serial.print("Задержка:        "); Serial.print(sensor.min_delay / 1000); Serial.println(" мс*/

  delayMS = sensor.min_delay / 1000; // Установка задержки между измерениями / Set delay between measurements

  // Инициализация HomeSpan / HomeSpan initialization
  homeSpan.begin(Category::Bridges, "Умный дом ESP32"); // Запуск HomeSpan как моста / Start HomeSpan as bridge
  homeSpan.setPairingCode("11122333"); // Установка кода сопряжения / Set pairing code

  // Аксессуар 1: Реле (лампочка) / Accessory 1: Relay (light bulb)
  new SpanAccessory();          // Создание нового аксессуара / Create new accessory
    new Service::AccessoryInformation(); // Сервис информации об аксессуаре / Accessory information service
      new Characteristic::Name("Умная лампа"); // Название аксессуара / Accessory name
      new Characteristic::Manufacturer("HomeSpan"); // Производитель / Manufacturer
      new Characteristic::SerialNumber("123-456"); // Серийный номер / Serial number
      new Characteristic::Model("ESP32-C3 Relay"); // Модель / Model
      new Characteristic::FirmwareRevision("1.0"); // Версия прошивки / Firmware version
      new Characteristic::Identify(); // Характеристика идентификации / Identify characteristic
    
    new Service::LightBulb();   // Сервис лампочки / Light bulb service
      lightPower = new Characteristic::On(false); // Характеристика включения/выключения / On/Off characteristic

  // Аксессуар 2: Датчик температуры / Accessory 2: Temperature sensor
  new SpanAccessory();          // Создание нового аксессуара / Create new accessory
    new Service::AccessoryInformation(); // Сервис информации об аксессуаре / Accessory information service
      new Characteristic::Name("Датчик температуры"); // Название аксессуара / Accessory name
      new Characteristic::Manufacturer("DHT22"); // Производитель / Manufacturer
      new Characteristic::SerialNumber("123-457"); // Серийный номер / Serial number
      new Characteristic::Model("DHT22"); // Модель / Model
      new Characteristic::FirmwareRevision("1.0"); // Версия прошивки / Firmware version
      new Characteristic::Identify(); // Характеристика идентификации / Identify characteristic
    
    new Service::TemperatureSensor(); // Сервис датчика температуры / Temperature sensor service
      tempCharacteristic = new Characteristic::CurrentTemperature(0); // Характеристика текущей температуры / Current temperature characteristic

  // Аксессуар 3: Датчик влажности / Accessory 3: Humidity sensor
  new SpanAccessory();          // Создание нового аксессуара / Create new accessory
    new Service::AccessoryInformation(); // Сервис информации об аксессуаре / Accessory information service
      new Characteristic::Name("Датчик влажности"); // Название аксессуара / Accessory name
      new Characteristic::Manufacturer("DHT22"); // Производитель / Manufacturer
      new Characteristic::SerialNumber("123-458"); // Серийный номер / Serial number
      new Characteristic::Model("DHT22"); // Модель / Model
      new Characteristic::FirmwareRevision("1.0"); // Версия прошивки / Firmware version
      new Characteristic::Identify(); // Характеристика идентификации / Identify characteristic
    
    new Service::HumiditySensor(); // Сервис датчика влажности / Humidity sensor service
      humidityCharacteristic = new Characteristic::CurrentRelativeHumidity(0); // Характеристика текущей влажности / Current humidity characteristic
}

void readDHTSensor() {
  // Проверяем, прошло ли достаточно времени с последнего чтения / Check if enough time has passed since last reading
  if (millis() - lastSensorRead < delayMS) {
    return; // Выход если не прошло достаточно времени / Exit if not enough time passed
  }
  
  // Чтение температуры / Temperature reading
  sensors_event_t event;        // Структура для хранения данных события / Structure for event data storage
  dht.temperature().getEvent(&event); // Получение данных температуры / Get temperature data
  if (!isnan(event.temperature)) { // Проверка на валидность значения / Check for valid value
    currentTemperature = event.temperature; // Сохранение температуры / Save temperature
    Serial.print("Температура: "); // Вывод в serial / Serial output
    Serial.print(currentTemperature);
    Serial.println(" °C");
    
    // Обновляем характеристику температуры / Update temperature characteristic
    tempCharacteristic->setVal(currentTemperature);
  } else {
    Serial.println("Ошибка чтения температуры!"); // Ошибка чтения / Reading error
  }

  // Чтение влажности / Humidity reading
  dht.humidity().getEvent(&event); // Получение данных влажности / Get humidity data
  if (!isnan(event.relative_humidity)) { // Проверка на валидность значения / Check for valid value
    currentHumidity = event.relative_humidity; // Сохранение влажности / Save humidity
    Serial.print("Влажность: "); // Вывод в serial / Serial output
    Serial.print(currentHumidity);
    Serial.println(" %");
    
    // Обновляем характеристику влажности / Update humidity characteristic
    humidityCharacteristic->setVal(currentHumidity);
  } else {
    Serial.println("Ошибка чтения влажности!"); // Ошибка чтения / Reading error
  }
  
  lastSensorRead = millis(); // Обновление времени последнего чтения / Update last reading time
}

void loop() {
  homeSpan.poll();              // Обработка HomeSpan событий / Process HomeSpan events
  
  // Управление реле в соответствии с состоянием из HomeKit / Relay control according to HomeKit state
  bool currentLightState = lightPower->getVal(); // Получение текущего состояния света / Get current light state
  if (currentLightState != lastLightState) { // Если состояние изменилось / If state changed
    digitalWrite(relayPin, !currentLightState); // Управление реле (инвертировано) / Control relay (inverted)
    lastLightState = currentLightState; // Сохранение состояния / Save state
    Serial.print("Состояние лампочки изменено: "); // Вывод статуса / Status output
    Serial.println(currentLightState ? "ВКЛ" : "ВЫКЛ");
  }
  
  // Обработка кнопки / Button handling
  int currentButtonState = digitalRead(buttonPin); // Чтение состояния кнопки / Read button state
  if (currentButtonState != lastButtonState) { // Если состояние изменилось / If state changed
    delay(1000); // Задержка для антидребезга / Debounce delay
    currentButtonState = digitalRead(buttonPin); // Повторное чтение / Read again
    if (currentButtonState != lastButtonState) { // Подтверждение изменения / Confirm change
      bool newState = !lightPower->getVal(); // Инвертирование состояния / Invert state
      lightPower->setVal(newState); // Установка нового состояния / Set new state
      digitalWrite(relayPin, !newState); // Управление реле / Control relay
      
      Serial.print("Переключатель изменен. Состояние: "); // Вывод статуса / Status output
      Serial.println(newState ? "ВКЛ" : "ВЫКЛ");
      
      lastButtonState = currentButtonState; // Сохранение состояния кнопки / Save button state
      delay(250); // Дополнительная задержка / Additional delay
    }
  }
  
  // Чтение данных с датчика температуры/влажности / Read temperature/humidity sensor data
  readDHTSensor();
  
  delay(10); // Короткая задержка для стабильности / Short delay for stability
}
