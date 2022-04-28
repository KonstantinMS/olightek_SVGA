#include "olightek.h" 

/*!
    @brief  минимальная инициализация для включения
    @return none (void).
    @note   
*/
void olightek :: olightek_init(void)
{
    // Регистр напряжения катода, указанное в классе по умолчанию
    olightek_vCom(vCom);
    // Формат видеопотока
    _sendRegisterSetting( INPUT_VIDEO_TYPE               , IVT_DATA_MODE_24RGB444 | IVT_SYNC_MODE_EMBEDDED1 | IVT_SCAN_MODE_NONINTER );
    // Регистр настройки синхронизации: HS и VS низким уровнем
    _sendRegisterSetting( VIDEO_RELATED_REGISTERS        , VRG_V_POL_ACTIVE_LOW | VRG_H_POL_ACTIVE_LOW );
	// Регистр включения
    _sendRegisterSetting( VIDEO_DISPLAY_CONTROL_REGISTER , VDCR_DISPLAY_ON | VDCR_VSCAN_BOTTOM2TOP | VDCR_HSCAN_LEFT2RIGHT );
    //зеленый
    _sendRegisterSetting( VIDEO_DISPLAY_CONTROL_REGISTER , VDCR_DISPLAY_ON | VDCR_VSCAN_TOP2BOTTOM | VDCR_HSCAN_RIGHT2LEFT );
}

/*!
    @brief  минимальная инициализация для включения и отображения шаблона
    @return none (void).
    @note
*/
void olightek :: olightek_init_pattern(void)
{
    // Регистр напряжения катода -3V - 0V (20 - FF)
    _sendRegisterSetting( VCOM_LEVEL                    , vCom );
    //Регистр шаблона
    _sendRegisterSetting( PATTERN						, PATTERN_COLOR_BAR );
	// Регистр включения
    _sendRegisterSetting( VIDEO_DISPLAY_CONTROL_REGISTER, VDCR_DISPLAY_ON | VDCR_VSCAN_TOP2BOTTOM | VDCR_HSCAN_LEFT2RIGHT );
}

/*!
    @brief  Включить все настройки светимости на максимум
    @return none (void).
    @note	не включать на длительное время, можно повредить дисплей
*/
void olightek :: olightek_turnOnMaxLuminance (void)
{
	olightek_vCom (0x50);
	olightek_brightness (0x80);
	olightek_contrast (CTRL_CONTRAST_MAX);
}

/*!
    @brief  Изменить яркость дисплея
    @param  value
            передаваемое значение
            0x00 - Darkest setting
            0x80 - Normal
            0xFF - Brightest setting
    @return верно, если удалось изменить яркость
    @note	запоминаем яркость в brightness класса
*/
bool olightek :: olightek_brightness (uint8_t value)
{
	brightness = value;
	return _sendRegisterSetting( CTRL_BRIGHTNESS, brightness);
}

/*!
    @brief  Изменить контрастность дисплея
    @param  value
            передаваемое значение
            0x00 - (Gain = 0) Black Screen
            0x80 - (Gain = 1) Normal
            0xFF - (Gain = 2) Contrast Double
    @return верно, если удалось изменить контрастность
    @note	запоминаем яркость в contrast класса
*/
bool olightek :: olightek_contrast (uint8_t value)
{
	contrast = value;
	return _sendRegisterSetting( CTRL_CONTRAST, contrast);
}

/*!
    @brief  Изменить напряжение катода дисплея
    @param  value
            передаваемое значение
            0x20 - min (-3 V)
            0xFF - max (0 V)
    @return верно, если удалось изменить напряжение катода
    @note	запоминаем напряжения в Vcom класса
*/
bool olightek :: olightek_vCom (uint8_t value)
{
	vCom = _within(value, VCOM_LEVEL_MIN, VCOM_LEVEL_MAX);
	return _sendRegisterSetting( VCOM_LEVEL, vCom);
}

/*!
    @brief  Отправка байта регистр дисплея
    @param  register
            адрес регистра
    @param  value
            передаваемое значение 
    @return верно, если команда успешно передана
    @note   
*/
bool olightek :: _sendRegisterSetting(uint8_t reg, uint8_t value)
{
    uint8_t data[2] = {reg, value};
	/// user code
    #pragma message ("use your i2c function here")
    /** Далее используйте свою функцию I2C для передачи значений регистров в дисплей **/
    MSS_I2C_write(&g_mss_i2c0, olightek::displayAddr, data, 2, MSS_I2C_RELEASE_BUS);
    if (MSS_I2C_wait_complete(&g_mss_i2c0, 0x3000) != MSS_I2C_SUCCESS)
    {
        return false;
    }
    /// end user code
    return true;
}

/*!
    @brief  Проверка нахождения значения в диапазоне
    @param  value
            проверяемое значение
    @param  min
            минимальное значение разрешенного диапазона
	@param  max
            максимальное значение разрешенного диапазона
    @return если в диапазоне, возвращает входное значение, иначе крайнее значение диапазона
    @note
*/
uint8_t olightek :: _within(uint8_t value, uint8_t min, uint8_t max)
{
	if (value < min)
		value = min;
	if (value > max)
		value = max;
	return value;
}
