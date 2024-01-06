# IIS
Проект автоматизированного полива растений с помощью микроконтроллера ESP32 с использованием брокера сообщений [Dealgate](https://dealgate.ru/) и [Алисы](https://yandex.ru/alice).

## Необходимое ПО

Необходимы следующие комплектующие:

- Микроконтролер [Raspberry Pi Pico WiFi](https://aliexpress.ru/item/1005002312427284.html?sku_id=12000029980957099&spm=.search_results.0.1ba7654dxgWAPP)
-  Помпа
<p align="center">
 <img width="400px" src="/Image/pompa.jpg" alt="qr"/>
</p>

-  [Arduino IDE](https://www.arduino.cc/en/software)

## Схема проекта

Подключение микроконтроллера Raspberry Pi Pico WiFi и помпы представлено на схеме:

<p align="center">
 <img width="400px" src="/Image/Scheme.png"/>
</p>


## Настройка Dealgate
Заходим на Dealgate с помощью Яндекса с аккаунта, с которого будем управлять.
После входа на сайт создаем новое устройство.

<p align="center">
 <img width="800px" src="/Image/create.jpg"/>
</p>

Далее нужно заполнить все обязательные поля.

<p align="center">
 <img width="600px" src="/Image/settings.jpg"/>
</p>

После настройки устройства настраиваем профиль для подключения к Dealgate. 

<p align="center">
 <img width="600px" src="/Image/profile.jpg"/>
</p>




## Загрузка прошивки

Для загрузки прошивки открываем Arduino IDE и в панели выбираем "File", затем нажимаем "Preferences" и вставляем ссылку
...
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
...

## Настройка умного дома с Алисой


