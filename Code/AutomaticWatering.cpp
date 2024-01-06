#include <Arduino.h>
#include <Wifi.h>
#include <ArduinoMqttClient.h>

//Настройка пинов
#define PUMB_PIN    D0                      //Пин к которому подключен MOSFET для управления насосом
#define MOISTURE    A0                      //Пин к которому подключен датчик влажности почвы
#define WATERLVL    D1                      //Пин к которому подключен датчик уровня жидкости

//Настройка сети
#define SSID        "SSID_NAME"             // Название сети
#define PASS        "SSID_PASS"             // Пароль для сети

//Настройка MQTT
#define USERNAME    "username"              // username указанный в dealgate
#define PASSWORD    "password"              // password указанный в dealgate

#define BROCKER     "mqtt.dealgate.ru"      //MQTT брокер
#define PORT        1883                    //Порт для MQTT-соединения
#define KEEP_ALIVE  50000                   //вемя допустимое для отсутствия соединения в милисекундах

//Настройка параметров времени
#define INTERVAL    5000                    //Интервал отправки данных в MQTT в миллисекундах

unsigned long previousMillis = 0;           //Время предыдущей отправки данных в MQTT в миллисек       

void onMqttMessage(int messageSize);        //Прототип функции обработки сообщений MQTT

byte mac[6];                                // MAC-адрес, он же, в шеснадцатиричном представлении, идентификатор устройства
char macStr[12];                            // Текстовое представление MAC-адреса


WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);          // Объект MQTT-соединения


//Топики
String      PumpTopic;                      //Топик для получения данных о включении или выключении насоса
String      MoistureTopic;                  //Топик для отправки данных о влажности почвы
String      LowWaterTopic;                  //Топик для оповещения о низком уровне воды


void setup() 
{
    //Активируем наш отладочный светодиод
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    pinMode(PUMB_PIN, OUTPUT);
    pinMode(MOISTURE, INPUT);

    //Открываем порт и начинаем писать даные, чтобы их можно было залогировать
    //Сочетание символов "#t#" будем использовать в качестве метки времени при записи логов, заменяя "#t#" на время получния сообщения
    Serial.begin(9600);
    Serial.println("#t# - Board starded working");

    //Открываем соединение с сетью
    Serial.println("#t# - Connecting to the network...");
    while (WiFi.begin(SSID, PASS) != WL_CONNECTED) 
    {
        // failed, retry
        Serial.print(".");
        delay(500);
    }
    Serial.println("#t# - You are connected to the network");
    
    //Получаем MAC-адрес и генерируем его текстовое представление
    Serial.print("#t# - Yor MAC-address is: ");
    WiFi.macAddress(mac);
    for (int i = 0; i < 12; i+=2) 
    {
        char buf[2];
        sprintf(buf, "%02x", mac[i/2]);
        macStr[i] = buf[0];
        macStr[i+1] = buf[1];
    }
    Serial.println(macStr);

    //Генерируем топики для MQTT
    //Будем использовать формат топика: "MAC/Topic"
    PumpTopic = macStr;
    MoistureTopic = macStr;
    LowWaterTopic = macStr;
    PumpTopic += "/Pump";
    MoistureTopic += "/Humidity";
    LowWaterTopic += "LowWater";

    //Открываем соединение с MQTT-соединением
    Serial.print("#t# - Attempting to connect to the MQTT broker: ");
    Serial.println(BROCKER);
    mqttClient.setKeepAliveInterval(KEEP_ALIVE);
    mqttClient.setId(macStr);
    mqttClient.setUsernamePassword(USERNAME, PASSWORD);
    if (!mqttClient.connect(BROCKER, PORT)) 
    {
        Serial.print("#t# - MQTT connection failed! Error code = ");
        Serial.println(mqttClient.connectError());
        rp2040.reboot();
    }
    Serial.println("#t# - MQTT connection successful");

    //Подписываемся на топик для получения данных о включении или выключении насоса
    mqttClient.onMessage(onMqttMessage);//Передаём функцию обработки сообщений MQTT в обработчик
    Serial.print("#t# - Subscribing to topic: ");
    Serial.println(PumpTopic);
    mqttClient.subscribe(PumpTopic);

    //Завершение инициализации
    Serial.println("#t# - Controller initialized");
    Serial.println();
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() 
{
    mqttClient.poll();  //Опрашиваем MQTT

    //Блок проверяющий соединение с брокером и WiFi, при отсутствии соединения перезагружаемся
    if (!wifiClient.connected() or !mqttClient.connected())
    {
        rp2040.reboot();
    }

    if (millis() - previousMillis >= INTERVAL)
    {
        digitalWrite(LED_BUILTIN, HIGH);            //Активируем светодиод для индикации отправки данных в MQTT

        uint MoistureRaw = analogRead(MOISTURE);    //Чтиаем показатели датчика влажности почвы
        //Так как ёмкостный датчик почвы выдаёт обратные знатчения, то преобразования так-же будут идти в обратную сторону,
        //чем меньше значения с датчика, тем больше процентов влажности почвы. 
        //Если датчик выдаёт прямопропорциональные значения, то надо поменять местами последние 2 значения
        //!!ВОЗМОЖЗНО НАДО БУДЕТ ДОМНОЖИТЬ ЗНАЧЕНИЯ НА КАКОЙ-ЛИБО КОЭФИЦИЕНТ ДЛЯ КАЛИБРОВКИ!!
        uint Moisture = map(MoistureRaw, 0, 4095, 100, 0);  //Преобразуем показатели датчика влажности почвы в проценты
        
        //Читаем состояние датчика уровня жидкости, если сигнал есть, то жидкости достаточно.
        isWaterLow = !digitalRead(WATERLVL);

        //Выводим данные в порт
        Serial.print("#t# - MoistureRaw: ");
        Serial.println(MoistureRaw);
        Serial.print("#t# - Moisture: ");
        Serial.print(Moisture);
        Serial.println(" %");
        Serial.print("#t# - Is water low?: ");
        Serial.prinln(isWaterLow);
        Serial.println();

        //Отправляем данные о влажности почвы в топик MQTT
        mqttClient.beginMessage(MoistureTopic);
        mqttClient.print(Moisture);
        mqttClient.endMessage();

        Serial.print("#t# - Data published to topic: ");
        Serial.println(MoistureTopic);
        
        //Отправляем данные об уровне воды в топик MQTT
        mqttClient.beginMessage(LowWaterTopic);
        mqttClient.print(LowWater);
        mqttClient.endMessage();

        Serial.print("#t# - Data published to topic: ");
        Serial.println(LowWaterTopic);

        //Заканчиваем отправку
        Serial.println();
        previousMillis = millis();      //Обновляем таймер
        digitalWrite(LED_BUILTIN, LOW); //Гасим светодиод по окончании отправки данных в MQTT
    } 
}


void onMqttMessage(int messageSize)
{
    digitalWrite(LED_BUILTIN, HIGH); //Снова включаем наш волшебный светодиод

    //Вывод данных о сообщении в порт
    Serial.print("#t# - Message received from topic: ");
    String topic = mqttClient.messageTopic();
    Serial.print(topic);
    Serial.print(", length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    //Обрабатываем сообщение
    String message = "";
    for (int i = 0; i < messageSize; i++) 
    {
        message += (char)mqttClient.read();
    }
    Serial.print("#t# - Message: ");
    Serial.println(message);

    //Проверяем топик
    if (topic == PumpTopic)
    //Просто включаем или выключаем помпу
    {
        if (message == "ON")
        {
            Serial.println("#t# - Pump ON");
            digitalWrite(PUMB_PIN, HIGH);
        }
        else if (message == "OFF")
        {
            Serial.println("#t# - Pump OFF");
            digitalWrite(PUMB_PIN, LOW);
        }
    }
    Serial.println();
    digitalWrite(LED_BUILTIN, LOW); //Гасим лапочку
}