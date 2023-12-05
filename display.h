#pragma once
#include "olightek.h"

class Display : public olightek
{
public:
    Display	();
    virtual bool sendRegisterSetting (uint8_t reg, uint8_t value) override; // Реализация функции отправки настройки в регистр дисплея
};