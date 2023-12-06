≡ Библиотека для настройки по I2C следующих дисплеев фирмы Olightek:  
* **SVGA050**  
* **SVGA060**  
* **SVGA097**  

Файлы: olightek.cpp, olightek.h Остальное для примера и тестов. 

Для применения необходимо:
* добавить файлы в проект;
* создать свой класс, унаследовать от него экземпляр класса Olightek. См. пример в Display.cpp
```cpp
Display display;
```
* Произвести минимальную инициализацию дисплея
```cpp
display.olightek_init();
```
* Установить желаемую яркость с помощью функций  
```cpp
bool olightek_brightness    (uint8_t value);			// Установка яркости дисплея
bool olightek_contrast      (uint8_t value);			// Установка контрастности дисплея
bool olightek_vCom          (uint8_t value);			// Установка напряжения катода дисплея
```