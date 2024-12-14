#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analog.h"
#include "calib.h"
#include "comm.h"
#include "data.h"

const float GAIN_SCALE[] = {24, 12, 6, 3, 1.5, 0.75, 0.37, 0.18};


int valFloatGet(int dev, int baseAddr, int ch, float* val) {
	int addr = baseAddr + ANALOG_VAL_SIZE * (ch - 1);
	uint8_t buf[ANALOG_VAL_SIZE];
	if(OK != i2cMem8Read(dev, addr, buf, ANALOG_VAL_SIZE)) {
		return ERROR;
	}
	memcpy(val, buf, ANALOG_VAL_SIZE);
	return OK;
}
/*
int valFloatSet(int dev, int baseAddr, int ch, float val) {
	int addr = baseAddr + ANALOG_VAL_SIZE * (ch - 1);
	uint8_t buf[ANALOG_VAL_SIZE];
	memcpy(buf, &val, ANALOG_VAL_SIZE);
	if(OK != i2cMem8Write(dev, addr, buf, ANALOG_VAL_SIZE)) {
		return ERROR;
	}
	return OK;
}
*/

/* bad channel check functoin {{{ */
bool badUInCh(int ch) {
	if(!(MIN_CH_NO <= ch && ch <= U_IN_CH_NO)) {
		printf("Input channel out of range![%d..%d]\n", MIN_CH_NO, U_IN_CH_NO);
		return true;
	}
	return false;
}
/* }}} */

const CliCmdType CMD_UIN_READ = {/*{{{*/
	"rd",
	2,
	&doUInRead,
	"  rd               Read input voltage value(V)\n",
	"  Usage:           "PROGRAM_NAME" <id> rd <channel>\n",
	"  Example:         "PROGRAM_NAME" 0 rd 2 #Read voltage on input channel #2 on board #0\n",
};
int doUInRead(int argc, char *argv[]) {
	if(argc != 4) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}
	int ch = atoi(argv[3]);
	if(badUInCh(ch)) {
		return ARG_RANGE_ERROR;
	}
	float val = 0;
	if(OK != valFloatGet(dev, I2C_MEM_U_IN, ch, &val)) {
		return ERROR;
	}
	printf("%0.3f\n", val);
	return OK;
}/*}}}*/
const CliCmdType CMD_UIN_CAL = {/*{{{*/
	"cal",
	2,
	&doUInCal,
	"  cal              Calibrate input channel, the calibration must be done in 2 points.\n",
	"  Usage 1:         "PROGRAM_NAME" <id> cal <channel> <value(V)>\n"
	"  Usage 2:         "PROGRAM_NAME" <id> cal <channel> reset\n",
	"  Example:         "PROGRAM_NAME" 0 cal 1 0.5; Calibrate the input channel #1 on board #0 at 0.5V\n"
};
int doUInCal(int argc, char *argv[]) {
	if(argc != 5) {
		return ARG_CNT_ERR;
	}
	int ch = atoi(argv[3]);
	if(badUInCh(ch)) {
		return ARG_RANGE_ERROR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if(dev < 0) {
		return ERROR;
	}
	if(strcasecmp(argv[4], "reset") == 0) {
		if(OK != calibReset(dev, CALIB_U_IN_CH1 + (ch - 1))) {
			return ERROR;
		}
		return OK;
	}
	float value = atof(argv[4]);
	if(OK != calibSet(dev, CALIB_U_IN_CH1 + (ch - 1), value)) {
		return ERROR;
	}
	return OK;
}/*}}}*/


int doGainRead(int argc, char *argv[]);
const CliCmdType CMD_GAIN_READ = {/*{{{*/
	"grd",
	2,
	&doGainRead,
	"  grd             Read gain code for scaling [0..7]->+/-[24,12,6,3,1.5,0.75,0.37,0.18]V\n",
	"  Usage:           "PROGRAM_NAME" <id> grd <channel>\n",
	"  Example:         "PROGRAM_NAME" 0 grd 2 #Read gain code for input channel #2 on board #0\n",
};
int doGainRead(int argc, char *argv[]) {
	if(argc != 4) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}
	int ch = atoi(argv[3]);
	if(badUInCh(ch)) {
		return ARG_RANGE_ERROR;
	}
	uint8_t val;
	if(OK != i2cMem8Read(dev, I2C_GAIN_CH1 + (ch - 1), &val, 1)) {
		return ERROR;
	}
	printf("%hhu\n", val);
	return OK;
}/*}}}*/

int doGainWrite(int argc, char *argv[]);
const CliCmdType CMD_GAIN_WRITE = {/*{{{*/
	"gwr",
	2,
	&doGainWrite,
	"  gwr             Write gain code for scaling [0..7]->+/-[24,12,6,3,1.5,0.75,0.37,0.18]V\n",
	"  Usage:           "PROGRAM_NAME" <id> gwr <channel> <value[0..7]>\n",
	"  Example:         "PROGRAM_NAME" 0 gwr 2 4#Set gain code to 4(+/-1.5V) for input channel #2 on board #0\n",
};
int doGainWrite(int argc, char *argv[]) {
	if(argc != 5) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}
	int ch = atoi(argv[3]);
	if(badUInCh(ch)) {
		return ARG_RANGE_ERROR;
	}
	int val = atoi(argv[4]);
	if(!(0 <= val && val <= 7)) {
		printf("Gain code out of range. Must be in [0,7]\n");
		return ARG_RANGE_ERROR;
	}
	uint8_t bval = val;
	if(OK != i2cMem8Write(dev, I2C_GAIN_CH1 + (ch - 1), &bval, 1)) {
		printf("Fail to write\n");
		return ERROR;
	}
	return OK;
}/*}}}*/


// vi:fdm=marker
