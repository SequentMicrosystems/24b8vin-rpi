/*
 * data.h
 *
 *  Created on: Jun 15, 2020
 *      Author: Alex
 */
#ifndef DATA_H_
#define DATA_H_

// TODO: Change with full card name
#define CARD_NAME "Eight 24-Bit Analog Inputs"
#define PROGRAM_NAME "24b8vin"

#define SLAVE_OWN_ADDRESS_BASE 0x31

//#define SLAVE_BUFF_SIZE 128
#define HW_MAJOR	1
#define HW_MINOR	0
#define FW_MAJOR	1
#define FW_MINOR	1
#define VERSION "1.0"
#define U_IN_CH_NO 8
#define ANALOG_VAL_SIZE 4
#define MODBUS_SETTINGS_SIZE_B 5

#define MIN_CH_NO 1
#define LED_CH_NO 8

#define VOLT_TO_MILIVOLT 1000

#define CALIBRATION_KEY 0xaa
#define RESET_CALIBRATION_KEY	0x55

#define WDT_RESET_SIGNATURE     0xca
#define WDT_RESET_COUNT_SIGNATURE    0xbe

#define CAL_0_10V_OUT_START_ID 6
#define CAL_0_10V_OUT_STOP_ID 7
#define CAL_4_20mA_OUT_START_ID 8
#define CAL_4_20mA_OUT_STOP_ID 9
enum
{
	CALIB_IN_PROGRESS = 0,
	CALIB_DONE,
	CALIB_ERROR,
};

enum
{
	CALIB_CH_NONE = 0,
	CALIB_U_IN_CH1,

};

enum //save samples command
{
	SS_CMD_NONE = 0,
	SS_CMD_START_SAVE,
	SS_CMD_START_LOOK_FOR_TH,
};


enum
{

	I2C_MEM_LEDS, // 8X leds
	I2C_MEM_LED_SET,
	I2C_MEM_LED_CLR,

	I2C_U_IN_VAL1_ADD, //8x floating point values expressed in Volts

	I2C_GAIN_CH1 = I2C_U_IN_VAL1_ADD + U_IN_CH_NO * ANALOG_VAL_SIZE,// 8x gain codes [0..7] setting full scale as follows:
	// 0 => +/-24V
	// 1 => +/-12V
	// 2 => +/-6V
	// 3 => +/-3V
	// 4 => +/-1.5V
	// 5 => +/-0.75V
	// 6 => +/-0.37V
	// 7 => +/-0.18V

	I2C_MEM_SR_SEL = I2C_GAIN_CH1 + U_IN_CH_NO, // 0 = 250sps, 1 = 500sps, 2 = 1Ksps; 3 = 2Ksps; 4 = 4Ksps; 5 = 8Ksps

	I2C_MEM_DIAG_TEMPERATURE_ADD,
	I2C_MEM_DIAG_RASP_V_ADD,
	I2C_MEM_DIAG_RASP_V,
	I2C_MEM_DIAG_RASP_V1,

	I2C_RTC_YEAR_ADD,
	I2C_RTC_MONTH_ADD,
	I2C_RTC_DAY_ADD,
	I2C_RTC_HOUR_ADD,
	I2C_RTC_MINUTE_ADD,
	I2C_RTC_SECOND_ADD,
	I2C_RTC_SET_YEAR_ADD,
	I2C_RTC_SET_MONTH_ADD,
	I2C_RTC_SET_DAY_ADD,
	I2C_RTC_SET_HOUR_ADD,
	I2C_RTC_SET_MINUTE_ADD,
	I2C_RTC_SET_SECOND_ADD,
	I2C_RTC_CMD_ADD,

	I2C_MEM_WDT_RESET_ADD,
	I2C_MEM_WDT_INTERVAL_SET_ADD,
	I2C_MEM_WDT_INTERVAL_GET_ADD = I2C_MEM_WDT_INTERVAL_SET_ADD + 2,
	I2C_MEM_WDT_INIT_INTERVAL_SET_ADD = I2C_MEM_WDT_INTERVAL_GET_ADD + 2,
	I2C_MEM_WDT_INIT_INTERVAL_GET_ADD = I2C_MEM_WDT_INIT_INTERVAL_SET_ADD + 2,
	I2C_MEM_WDT_RESET_COUNT_ADD = I2C_MEM_WDT_INIT_INTERVAL_GET_ADD + 2,
	I2C_MEM_WDT_CLEAR_RESET_COUNT_ADD = I2C_MEM_WDT_RESET_COUNT_ADD + 2,
	I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD,
	I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD = I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD + 4,
	I2C_MEM_BUTTON = I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD + 4,

	I2C_MODBUS_SETINGS_ADD ,

	I2C_MEM_CALIB_VALUE = I2C_MODBUS_SETINGS_ADD + MODBUS_SETTINGS_SIZE_B,
	I2C_MEM_CALIB_CHANNEL = I2C_MEM_CALIB_VALUE + 4,
	I2C_MEM_CALIB_KEY, //set calib point -> 0xaa; reset calibration on the channel -> 0x55
	I2C_MEM_CALIB_STATUS,
	I2C_MEM_CLOCK_SET,// 0 -> internal clock usage, no cloock output, 1 -> internal clock usage, clock output, 2 -> external clock usage
	I2C_MEM_SYNC_SOURCE, //0 -> internal sync no sync out, 1 ->internal sync, sync out to connector, 2 - external sync usage
	I2C_MEM_SYNC_CMD,// 0xaa sync signal is sent accordingly with

	I2C_MEM_SAVE_CH,
	I2C_MEM_SAVE_N_SMAPLES,
	I2C_MEM_SAVE_THRESHOLD =I2C_MEM_SAVE_N_SMAPLES + 2,
	I2C_MEM_SAVE_STATUS = I2C_MEM_SAVE_THRESHOLD + 4,
	I2C_MEM_SAVE_CMD,


	I2C_MEM_SAVE_FIFO ,
	I2C_MEM_SAVE_FIFO_LEVEL = I2C_MEM_SAVE_FIFO +4,
	I2C_MEM_SAVE_FIFO_STATUS = I2C_MEM_SAVE_FIFO_LEVEL + 2,


	I2C_MEM_UPDATE_ADD = 0xaa,

	I2C_MEM_REVISION_HW_MAJOR_ADD = 251,
	I2C_MEM_REVISION_HW_MINOR_ADD,
	I2C_MEM_REVISION_MAJOR_ADD,
	I2C_MEM_REVISION_MINOR_ADD,
	SLAVE_BUFF_SIZE = 255,
};

void dataInit(void);

void dataRecUpd(void);

#define ERROR -1
#define OK 0
#define ARG_CNT_ERR -2
#define ARG_RANGE_ERROR -3
#define IO_ERROR -4

#define STR_(x) #x
#define STR(x) STR_(x)
#define MASK_1 1
#define MASK_2 3
#define MASK_3 7
#define MASK_4 15
#define MASK_5 31
#define MASK_6 63
#define MASK_7 127
#define MASK_(x) MASK_##x
#define MASK(x) MASK_(x)

typedef enum {
	OFF,
	ON,
	STATE_COUNT,
} State;
#endif /* DATA_H_ */
