#include "board.h"
#include "cli.h"
#include "help.h"
#include "led.h"
#include "rtc.h"
#include "wdt.h"
#include "rs485.h"
#include "analog.h"

const CliCmdType *gCmdArray[] = {
	&CMD_HELP,
	&CMD_VERSION,
	&CMD_BOARD,
	&CMD_UIN_READ,
	&CMD_UIN_CAL,
	&CMD_RTC_GET,
	&CMD_RTC_SET,
	&CMD_WDT_RELOAD,
	&CMD_WDT_GET_PERIOD,
	&CMD_WDT_SET_PERIOD,
	&CMD_WDT_GET_INIT_PERIOD,
	&CMD_WDT_SET_INIT_PERIOD,
	&CMD_WDT_GET_OFF_PERIOD,
	&CMD_WDT_SET_OFF_PERIOD,
	&CMD_WDT_GET_RESET_COUNT,
	&CMD_WDT_CLR_RESET_COUNT,
	&CMD_LED_READ,
	&CMD_LED_WRITE,
	&CMD_CFG485_READ,
	&CMD_CFG485_WRITE,
	0
};
