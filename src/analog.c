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
	if(OK != valFloatGet(dev, I2C_U_IN_VAL1_ADD, ch, &val)) {
		return ERROR;
	}
	printf("%0.6f\n", val);
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
	"  grd              Read gain code for scaling [0..7]->+/-[24,12,6,3,1.5,0.75,0.37,0.18]V\n",
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
	"  gwr              Write gain code for scaling [0..7]->+/-[24,12,6,3,1.5,0.75,0.37,0.18]V\n",
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


int doSrWrite(int argc, char *argv[]);
const CliCmdType CMD_SR_WRITE = {
	"srwr",
	2,
	&doSrWrite,
	"  srwr              Write ADC sample rate code  [0..5]-> 0 = 250Hz, 1 = 500Hz, 2 = 1 kHz; 3 = 2 kHz; 4 = 4 kHz; 5 = 8 kHz\n",
	"  Usage:           "PROGRAM_NAME" <id> srwr <value[0..5]>\n",
	"  Example:         "PROGRAM_NAME" 0 srwr 2 #Set sample rate code to 2(1 kHz) for board #0\n",
};
int doSrWrite(int argc, char *argv[]) {
	if(argc != 4) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}

	int val = atoi(argv[3]);
	if(!(0 <= val && val <= 5)) {
		printf("Sample rate code out of range. Must be in [0,5]\n");
		return ARG_RANGE_ERROR;
	}
	uint8_t bval = val;
	if(OK != i2cMem8Write(dev, I2C_MEM_SR_SEL, &bval, 1)) {
		printf("Fail to write\n");
		return ERROR;
	}
	return OK;
}


int doSrRead(int argc, char *argv[]);
const CliCmdType CMD_SR_READ = {
	"srrd",
	2,
	&doSrRead,
	"  srrd              Return the ADC sample rate in Hz\n",
	"  Usage:           "PROGRAM_NAME" <id> srrd\n",
	"  Example:         "PROGRAM_NAME" 0 srrd #Return the sample rate for board #0\n",
};
const int SR_VAL[6] ={250, 500, 1000, 2000, 4000, 8000};
int doSrRead(int argc, char *argv[]) {
	if(argc != 3) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}

	uint8_t bval = 0;
	if(OK != i2cMem8Read(dev, I2C_MEM_SR_SEL, &bval, 1)) {
		printf("Fail to read\n");
		return ERROR;
	}
	if(bval > 5) 
	{
		bval = 5;
	}
	printf("%d\n",SR_VAL[bval] );

	return OK;
}


int doSaveCfgWrite(int argc, char *argv[]);
const CliCmdType CMD_SAVE_CFG_WRITE = {
	"scfgwr",
	2,
	&doSaveCfgWrite,
	"  scfgwr           Write the save mecahanism configuration: Channel, number of samples, and trigger threshold\n",
	"  Usage:           "PROGRAM_NAME" <id> scfgwr <ch[1..8]> <Nsample[100..6000]> <threshold> \n",
	"  Example:         "PROGRAM_NAME" 0 scfgwr 2 5000 1.235 #Set save algorithm on channel 2, for 5000 samples to trigger when input exceed 1.235V for board #0\n",
};
int doSaveCfgWrite(int argc, char *argv[]) {
	if(argc != 6) {
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
	int samples = atoi(argv[4]);
	if(!(100 <= samples && samples <= 6000)) {
		printf("Number of samples to be saved out of range. Must be in [100,6000]\n");
		return ARG_RANGE_ERROR;
	}
	float threshold = atof(argv[5]);
	uint8_t buff[8];
	buff[0] = ch;
	uint16_t s = 0xffff & samples;
	memcpy(&buff[1], &s, 2);
	memcpy(&buff[3], &threshold, 4);

	if(OK != i2cMem8Write(dev, I2C_MEM_SAVE_CH, buff, 7)) {
		printf("Fail to write\n");
		return ERROR;
	}
	return OK;
}

int doSaveCfgRead(int argc, char *argv[]);
const CliCmdType CMD_SAVE_CFG_READ = {
	"scfgrd",
	2,
	&doSaveCfgRead,
	"  scfgrd           Read the save mecahanism configuration: Channel, number of samples, and trigger threshold\n",
	"  Usage:           "PROGRAM_NAME" <id> scfgrd\n",
	"  Example:         "PROGRAM_NAME" 0 scfgwr #Displays the save algorithm parameters for board #0\n",
};
int doSaveCfgRead(int argc, char *argv[]) {
	if(argc != 3) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}
	uint8_t buff[8];
	if(OK != i2cMem8Read(dev, I2C_MEM_SAVE_CH, buff, 7)) {
		printf("Fail to write\n");
		return ERROR;
	}
	int ch = buff[0];

	int samples = 0;
	memcpy(&samples, &buff[1], 2);

	float threshold = 0;
	memcpy (&threshold, &buff[3], 4);
	
	printf("Save %d sample from channel %d, when input reach %0.3fV\n", samples, ch, threshold);

	return OK;
}

int doSaveStatusRead(int argc, char *argv[]);
const CliCmdType CMD_SAVE_STATUS = {
	"sstat",
	2,
	&doSaveStatusRead,
	"  sstat           Read the save mecahanism status (IDLE(0)/CHECKING_THRESHOLD(1)/SAVING(2)\n",
	"  Usage:           "PROGRAM_NAME" <id> sstat\n",
	"  Example:         "PROGRAM_NAME" 0 sstat #Displays the save algorithm status for board #0\n",
};
int doSaveStatusRead(int argc, char *argv[]) {
	if(argc != 3) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}
	uint8_t status;
	if(OK != i2cMem8Read(dev, I2C_MEM_SAVE_STATUS, &status, 1)) {
		printf("Fail to write\n");
		return ERROR;
	}
	
	printf("%d\n", (int)status);

	return OK;
}


int doSaveStart(int argc, char *argv[]);
const CliCmdType CMD_SAVE_START = {
	"start",
	2,
	&doSaveStart,
	"  start           Start saving data imediatly if the argumnt is 1 or after the threshold condition is meet if argument is 0\n",
	"  Usage:           "PROGRAM_NAME" <id> start <0/1>\n",
	"  Example:         "PROGRAM_NAME" 0 start 1 #Start saving samples for board #0\n",
};
int doSaveStart(int argc, char *argv[]) {
	if(argc != 4) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}
	uint8_t cmd = SS_CMD_START_LOOK_FOR_TH;
	if( atoi(argv[3]) != 0)
	{
		cmd = SS_CMD_START_SAVE;
	}

	if(OK != i2cMem8Write(dev, I2C_MEM_SAVE_CMD, &cmd, 1)) {
		printf("Fail to write\n");
		return ERROR;
	}

	return OK;
}
// fifo 

int doFifoLevelRead(int argc, char *argv[]);
const CliCmdType CMD_FIFO_LEVEL_READ = {
	"fifolvl",
	2,
	&doFifoLevelRead,
	"  fifolvl           Read the fifo level ( number of samples saved and not yet readed)\n",
	"  Usage:           "PROGRAM_NAME" <id> fifolvl\n",
	"  Example:         "PROGRAM_NAME" 0 fifolvl #Displays the number of sampleds available in the fifo for board #0\n",
};
int doFifoLevelRead(int argc, char *argv[]) {
	if(argc != 3) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}
	uint16_t level;
	if(OK != i2cMem8Read(dev, I2C_MEM_SAVE_FIFO_LEVEL, (uint8_t*)&level, 2)) {
		printf("Fail to read\n");
		return ERROR;
	}
	
	printf("%d\n", (int)level);

	return OK;
}


int doFifoRead(int argc, char *argv[]);
const CliCmdType CMD_FIFO_READ = {
	"fiford",
	2,
	&doFifoRead,
	"  fiford           Read the oldest sample from the fifo and advance the pointer to the next one \n",
	"  Usage:           "PROGRAM_NAME" <id> fiford\n",
	"  Example:         "PROGRAM_NAME" 0 fiford #Displays the oldest sample in the fifo for board #0\n",
};
int doFifoRead(int argc, char *argv[]) {
	if(argc != 3) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}
	uint8_t buff[4];
	if(OK != i2cMem8Read(dev, I2C_MEM_SAVE_FIFO, buff, 4)) {
		printf("Fail to read\n");
		return ERROR;
	}
	float val = 0;
	memcpy(&val, buff, 4);
	printf("%0.6f\n", val);

	return OK;
}

// vi:fdm=marker
