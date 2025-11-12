🏠 Умный дом на ESP32: Управление светодиодной лентой через HomeKit
О проекте

Проект позволяет подключить обычную светодиодную ленту к системе Apple HomeKit с минимальными изменениями в существующей проводке.
Идея проекта

Вместо замены всей системы управления был найден практичный способ добавить умные функции к уже установленной светодиодной ленте с обычным выключателем.
Техническая реализация
Компоненты системы

    Микроконтроллер: ESP32-WROOM

    Управление питанием: Релейный модуль

    Выключатель: Стандартный проводной выключатель

    Датчик: DHT22 для измерения температуры и влажности

Схема подключения

Выключатель подключен между GND и пином 8. Такое подключение обеспечивает:

    Синхронизацию состояний - изменение положения выключателя переключает реле

    Двойное управление - можно использовать и приложение HomeKit, и обычный выключатель

    Надежность - система работает даже без подключения к интернету

Дополнительные функции

Датчик DHT22 добавляет возможность мониторинга:

    Температуры в комнате

    Уровня влажности

    Данные автоматически обновляются в приложении

Преимущества решения

    Экономия - не нужна полная замена проводки

    Удобство - привычное управление выключателем сохраняется

    Надежность - два независимых способа управления

    Функциональность - управление светом + мониторинг климата

    Интеграция - работа с Apple HomeKit

🏠 Smart Home on ESP32: LED Strip Control via HomeKit

Translated with AI assistance
About the Project

This project allows connecting a conventional LED strip to the Apple HomeKit system with minimal changes to existing wiring.
Project Idea

Instead of replacing the entire control system, we found a practical way to add smart features to an already installed LED strip with a conventional switch.
Technical Implementation
System Components

    Microcontroller: ESP32-WROOM

    Power Management: Relay module

    Switch: Standard wired switch

    Sensor: DHT22 for temperature and humidity measurement

Connection Diagram

The switch is connected between GND and pin 8. This connection provides:

    State synchronization - switch position change toggles the relay

    Dual control - can use both HomeKit app and conventional switch

    Reliability - system works even without internet connection

Additional Features

DHT22 sensor adds monitoring capabilities for:

    Room temperature

    Humidity level

    Data automatically updates in the app

Solution Advantages

    Cost savings - no need for complete wiring replacement

    Convenience - familiar switch control is preserved

    Reliability - two independent control methods

    Functionality - light control + climate monitoring

    Integration - works with Apple HomeKit
