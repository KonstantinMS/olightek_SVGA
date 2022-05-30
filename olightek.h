// include your libs
#pragma message ("include your files here")
#include <stdint.h>
#include "../drivers/mss_i2c/mss_i2c.h"
// end user libs

#ifndef olightek_h
#define olightek_h

#define		VIDEO_DISPLAY_CONTROL_REGISTER			0x10		//	Display off & Scan directions
#define		VDCR_DISPLAY_ON							0b00000000
#define		VDCR_DISPLAY_OFF						0b00000100
#define		VDCR_VSCAN_TOP2BOTTOM					0b00000000
#define		VDCR_VSCAN_BOTTOM2TOP					0b00000010
#define		VDCR_HSCAN_LEFT2RIGHT					0b00000000
#define		VDCR_HSCAN_RIGHT2LEFT					0b00000001

// The valid range of Vcom setting is 20H ~ FFH, and the corresponding cathode voltage is about -3V ~ 0V.
// The lower cathode voltage makes the display brighter. The curve of Vcom and cathode voltage sees section 3.5 (DC / DC converter).
// Low Vcom settings will cause the display too bright, may damage the eyes of the user, and continues use may cause display overheating and damage.
#define		VCOM_LEVEL								0x19		//	Vcom Level Setting
#define		VCOM_LEVEL_MAX							0xFF		//
#define		VCOM_LEVEL_MIN							0x20		//

#define		INPUT_VIDEO_TYPE						0x01		// Input video type set
//Select input data format
#define		IVT_DATA_MODE_16YCBCR422				0b00000000
#define		IVT_DATA_MODE_24YCBCR444				0b00010000
#define		IVT_DATA_MODE_8MONO						0b00100000
#define		IVT_DATA_MODE_24RGB444					0b00110000
#define		IVT_DATA_MODE_8YCBCR422					0b01000000
//Select scan mode
#define		IVT_SCAN_MODE_NONINTER					0b00000000
#define		IVT_SCAN_MODE_INTER						0b00000001
#define		IVT_SCAN_MODE_NOTUSE					0b00000010
#define		IVT_SCAN_MODE_PSEUDO_INTER				0b00000011
//select sync mode
#define		IVT_SYNC_MODE_EMBEDDED0					0b00000000
#define		IVT_SYNC_MODE_EMBEDDED1					0b00000100	// its correct, see data sheet
#define		IVT_SYNC_MODE_EXT_WITH_DE				0b00001000
#define		IVT_SYNC_MODE_EXT_NO_DE					0b00001100

#define		VIDEO_RELATED_REGISTERS					0x02		// V sync/H sync Polarity& 3D function Setting
// 3D values not described, see datasheet
#define		VRG_V_POL_ACTIVE_LOW					0b00000010
#define		VRG_V_POL_ACTIVE_HIGH					0b00000000
#define		VRG_H_POL_ACTIVE_LOW					0b00000001
#define		VRG_H_POL_ACTIVE_HIGH					0b00000000

#define		CTRL_BRIGHTNESS							0x08		// Brightness control
#define		CTRL_BRIGHTNESS_MAX						0xFF
#define		CTRL_BRIGHTNESS_MIN						0x00

#define		CTRL_CONTRAST							0x09		// Contrast control
#define		CTRL_CONTRAST_MAX						0xFF
#define		CTRL_CONTRAST_MIN						0x00

#define		PATTERN									0x4A		// Select Test Pattern
#define		PATTERN_OFF								0b00000000
#define		PATTERN_COLOR_BAR						0b00000001
#define		PATTERN_GRAY_SCALE						0b00000010
#define		PATTERN_TILE							0b00000011
#define		PATTERN_VERTICAL_LINES					0b00000100
#define		PATTERN_HORIZONTAL_LINES				0b00000101
#define		PATTERN_VER_HOR_LINES					0b00000110
#define		PATTERN_DNU								0b00000111

#define		CPU_FREQ

class olightek
{
	/**	КОНСТРУКТОР КЛАССА **/
	public:
		olightek	(void)
		{
			displayAddr = 0x0F;
		}
		olightek	(uint8_t address)
		{
			displayAddr = address;
		}
	/**	ПОЛЬЗОВАТЕЛЬСКИЕ ФУНКЦИИ **/
		void olightek_init			(void);						// Инициализация дисплея для работы
		void olightek_init_pattern	(void);						// Инициализация дисплея и включение шаблона
		bool olightek_brightness	(uint8_t value);			// Установка яркости дисплея
		bool olightek_contrast		(uint8_t value);			// Установка контрастности дисплея
		bool olightek_vCom			(uint8_t value);			// Установка напряжения катода дисплея
		void olightek_turnOnMaxLuminance (void);				// Включить максимальную яркость дисплея (не рекомендуется включать на долго)
		void olightek_changePattern (void);						// Сменить шаблон на следующий

	private:
	/**	ВНУТРЕННИЕ ПЕРЕМЕННЫЕ **/
		uint8_t				displayAddr;						// I2C адрес дисплея
		//you can set your default value here
		uint8_t				vCom = 0x80;
		uint8_t				brightness = 0x80;
		uint8_t				contrast = 0x80;
		uint8_t				selectedPattern = PATTERN_VER_HOR_LINES;
	/**	ВНУТРЕННИЕ ФУНКЦИИ **/
		uint8_t 			_within					(uint8_t value, uint8_t min, uint8_t max); // Проверка нахождения значения в диапазоне
		bool				_sendRegisterSetting	(uint8_t reg, uint8_t value); // Объявляем  функцию отправки настройки в регистр дисплея
};
#endif
