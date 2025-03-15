#ifndef ANALOG_H
#define ANALOG_H

#include "cli.h"

extern const CliCmdType CMD_UIN_READ;

extern const CliCmdType CMD_UIN_CAL;

extern const CliCmdType CMD_GAIN_READ;
extern const CliCmdType CMD_GAIN_WRITE;
extern const CliCmdType CMD_SR_WRITE;
extern const CliCmdType CMD_SR_READ;
extern const CliCmdType CMD_SAVE_CFG_WRITE;
extern const CliCmdType CMD_SAVE_CFG_READ;
extern const CliCmdType CMD_SAVE_STATUS;
extern const CliCmdType CMD_SAVE_START;
extern const CliCmdType CMD_FIFO_LEVEL_READ;
extern const CliCmdType CMD_FIFO_READ;


int doUInRead(int argc, char *argv[]);
int doUOutRead(int argc, char *argv[]);
int doUOutWrite(int argc, char *argv[]);
int doIInRead(int argc, char *argv[]);
int doIOutRead(int argc, char *argv[]);
int doIOutWrite(int argc, char *argv[]);

int doUInCal(int argc, char *argv[]);
int doUInCalReset(int argc, char *argv[]);
int doIInCal(int argc, char *argv[]);
int doIInCalReset(int argc, char *argv[]);
int doUOutCal(int argc, char *argv[]);
int doUOutCalReset(int argc, char *argv[]);
int doIOutCal(int argc, char *argv[]);
int doIOutCalReset(int argc, char *argv[]);
int doSrWrite(int argc, char *argv[]) ;
int doSrRead(int argc, char *argv[]);
int doSaveCfgWrite(int argc, char *argv[]);
int doSaveCfgRead(int argc, char *argv[]);
int doSaveStatusRead(int argc, char *argv[]);
int doSaveStart(int argc, char *argv[]);
int doFifoLevelRead(int argc, char *argv[]);
int doFifoRead(int argc, char *argv[]);

#endif /* ANALOG_H */

// vi:fdm=marker
