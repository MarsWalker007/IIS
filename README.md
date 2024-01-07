# IIS
Проект автоматизированного полива растений с помощью микроконтроллера  Raspberry Pi Pico использованием брокера сообщений [Dealgate](https://dealgate.ru/) и [Алисы](https://yandex.ru/alice).

## Необходимое оборудование и ПО

Необходимы следующие комплектующие:

- Микроконтролер [Raspberry Pi Pico WiFi](https://aliexpress.ru/item/1005002312427284.html?sku_id=12000029980957099&spm=.search_results.0.1ba7654dxgWAPP)
-  Помпа
<p align="center">
 <img width="400px" src="/Image/pompa.jpg" alt="qr"/>
</p>

-  Датчик влажности почвы
<p align="center">
 <img width="400px" src="/Image/earth.jpg" alt="qr"/>
</p>

-  Датчик уровня воды
<p align="center">
 <img width="400px" src="/Image/water.jpg" alt="qr"/>
</p>

Необходимы следующие ПО:

-  [Arduino IDE](https://www.arduino.cc/en/software)

## Схема проекта

Подключение микроконтроллера Raspberry Pi Pico WiFi и комплектующих представлено на схеме:

<p align="center">
 <img width="400px" src="/Image/Scheme.png"/>
</p>


## Настройка Dealgate
Заходим на Dealgate с помощью Яндекса с аккаунта, с которого будем управлять.
После входа на сайт создаем новое устройство.

<p align="center">
 <img width="800px" src="/Image/create.jpg"/>
</p>

Далее нужно заполнить все обязательные поля и настроить умения.

<p align="center">
 <img width="600px" src="/Image/settings.jpg"/>
</p>

После настройки устройства настраиваем профиль для подключения к Dealgate. 





## Загрузка прошивки

Для загрузки прошивки открываем Arduino IDE и в панели выбираем "File", затем нажимаем "Preferences", вставляем ссылку и нажимаем OK:

```
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
```

<p align="center">
 <img width="600px" src="/Image/url.jpg"/>
</p>

После этого вставляем код в Arduino IDE из папки Code/AutomaticWatering.cpp.Дальше нужно выбрать плату в "Tools", затем "Board" и найти Raspberry Pi Pico W

<p align="center">
 <img width="600px" src="/Image/PicoW.jpg"/>
</p>

Далее нажимаем "Upload" и загружаем код на плату.

## Настройка умного дома с Алисой

Открываем "Умный дом"

<p align="center">
 <img width="300px" src="/Image/Home.jpg"/>
</p>

Нажимаем на плюс и выбираем Dealgate.

<p align="center">
 <img width="300px" src="/Image/DealgateHome.jpg"/>
</p>

С помощью инструкции добавляем аккаунт Dealgate.

<p align="center">
 <img width="300px" src="/Image/DealgateAdd.jpg"/>
</p>

Теперь нам показываются устройства, нужно только выставить настройки.

<p align="center">
 <img width="600px" src="/Image/DealgateSettings.jpg"/>
</p>


