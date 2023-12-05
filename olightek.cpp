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
    sendRegisterSetting( INPUT_VIDEO_TYPE               , IVT_DATA_MODE_24RGB444 | IVT_SYNC_MODE_EMBEDDED1 | IVT_SCAN_MODE_NONINTER );
    // Регистр настройки синхронизации: HS и VS низким уровнем
    sendRegisterSetting( VIDEO_RELATED_REGISTERS        , VRG_V_POL_ACTIVE_LOW | VRG_H_POL_ACTIVE_LOW );
	// Регистр включения
    sendRegisterSetting( VIDEO_DISPLAY_CONTROL_REGISTER , VDCR_DISPLAY_ON | VDCR_VSCAN_BOTTOM2TOP | VDCR_HSCAN_LEFT2RIGHT );
    //зеленый
    sendRegisterSetting( VIDEO_DISPLAY_CONTROL_REGISTER , VDCR_DISPLAY_ON | VDCR_VSCAN_TOP2BOTTOM | VDCR_HSCAN_RIGHT2LEFT );
}

/*!
    @brief  минимальная инициализация для включения и отображения шаблона
    @return none (void).
    @note
*/
void olightek :: olightek_init_pattern(void)
{
    // Регистр напряжения катода -3V - 0V (20 - FF)
    sendRegisterSetting( VCOM_LEVEL                    , vCom );
    //Регистр шаблона
    sendRegisterSetting( PATTERN						, selectedPattern );
	// Регистр включения
    sendRegisterSetting( VIDEO_DISPLAY_CONTROL_REGISTER, VDCR_DISPLAY_ON | VDCR_VSCAN_TOP2BOTTOM | VDCR_HSCAN_LEFT2RIGHT );
}

/*!
    @brief  Включить режим PAL, как в Register Settings Example
    @return none (void).
    @note
*/
void olightek :: olightek_PAL (void)
{
	sendRegisterSetting( INPUT_VIDEO_TYPE              , IVT_DATA_MODE_8YCBCR422 | IVT_SCAN_MODE_INTER );
	sendRegisterSetting( NTSC_PAL_SCALING              , NPS_V_SCALE_6_5 | NPS_H_SCALE_11_10 );
	sendRegisterSetting( DISPLAY_LEFT_MARGIN           , 0x52 );
	sendRegisterSetting( DISPLAY_RIGHT_MARGIN          , 0x52 );
	sendRegisterSetting( DISPLAY_TOP_MARGIN            , 0x52 );
	sendRegisterSetting( DISPLAY_BOTTOM_MARGIN         , 0x3E );
}

/*!
    @brief  Включить режим PAL (SQ), как в Register Settings Example
    @return none (void).
    @note
*/
void olightek :: olightek_PAL_SQ (void)
{
	sendRegisterSetting( INPUT_VIDEO_TYPE              , IVT_DATA_MODE_8YCBCR422 | IVT_SCAN_MODE_INTER );
	sendRegisterSetting( NTSC_PAL_SCALING              , NPS_V_SCALE_1_1 | NPS_H_SCALE_1_1 );
	sendRegisterSetting( DISPLAY_LEFT_MARGIN           , 0x12 );
	sendRegisterSetting( DISPLAY_RIGHT_MARGIN          , 0x12 );
	sendRegisterSetting( DISPLAY_TOP_MARGIN            , 0x14 );
	sendRegisterSetting( DISPLAY_BOTTOM_MARGIN         , 0x0E );
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
	return sendRegisterSetting( CTRL_BRIGHTNESS, brightness);
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
	return sendRegisterSetting( CTRL_CONTRAST, contrast);
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
	return sendRegisterSetting( VCOM_LEVEL, vCom);
}

/*!
    @brief  Изменить отображаемый шаблон
    @return none (void).
    @note	шаблоны меняются по кругу
*/
void olightek :: olightek_changePattern (void)
{
	// увеличиваем шаблон на 1 в диапазоне от 0b000 до 0b111
	selectedPattern = (selectedPattern + 1) % 0b1000;
	// Регистр шаблона
	sendRegisterSetting( PATTERN, selectedPattern );
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
