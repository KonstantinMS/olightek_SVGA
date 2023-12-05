#include "display.h"
#include <iostream>
#include <iomanip>

Display::Display() {}

bool Display :: sendRegisterSetting (uint8_t reg, uint8_t value)
{
    std :: cout  << ">> " << std :: hex << std :: setw(2) << static_cast<int>(reg)  << " " << static_cast<int>(value) << std::endl;
    return true;
}