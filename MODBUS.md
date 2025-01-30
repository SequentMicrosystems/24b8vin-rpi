

# Modbus

The [Eight 24-Bit Analog Inputs DAQ 8-Layer Stackable HAT for Raspberry Pi](https://sequentmicrosystems.com/products/eight-24-bit-analog-inputs-daq-8-layer-stackable-hat-for-raspberry-pi) can be accessed through Modbus RTU protocol over RS-485 port.
You can set up the RS-485 port with the **24b8vin** command.

Example:
```bash
~$ 24b8vin 0 cfg485wr 1 1 9600 1 0
```
Set modbus RTU, slave address offset = 1, baudrate 9600bps, one stop bit, parity none
```bash
~$ 24b8vin -h cfg485wr
```
display the full set of options

## Slave Address
The slave address is added with the "stack level" jumpers. For example, the jumpers configuration for stack level 1  (one jumper in position ID0) slave address offset to 1 corresponds to slave address 2.

## Modbus object types
All Modbus RTU object types with standard addresses are implemented: Coils, Discrete Inputs, Input registers, and Holding registers.

### Coils

Access level Read/Write, Size 1 bit

| Device function | Register Address | Modbus Address |
| --- | --- | --- |
| COIL_LED1 | 0001 | 0x00 |
| COIL_LED2 | 0002 | 0x01 |
| COIL_LED3 | 0003 | 0x02 |
| COIL_LED4 | 0004 | 0x03 |
| COIL_LED5 | 0005 | 0x04 |
| COIL_LED6 | 0006 | 0x05 |
| COIL_LED7 | 0007 | 0x06 |
| COIL_LED8 | 0008 | 0x07 |


### Discrete Inputs

Access level Read Only, Size 1 bit

| Device function | Register Address | Modbus Address |
| --- | --- | --- |
| | | |


### Input registers

Access level Read Only, Size 16 bits

| Device function | Register Address | Modbus Address| Description | Measurement Unit |
| --- | --- | --- | --- | --- |
| IR_VIN_H1| 30001 | 0x00 | Temperature on RTD1 sensor| degC/10 |
| IR_VIN_L1| 30002 | 0x01 | Temperature on RTD1 sensor| degC/10 |
| 0-10V_IN_1 | 30003 | 0x02 | 0-10V Input 1 | mV |
| 0-10V_IN_2 | 30004 | 0x03 | 0-10V Input 2 | mV |
| 4-20mA_IN_1 | 30005 | 0x04 | 4-20mA Input 1 | uA |
| 4-20mA_IN_2 | 30006 | 0x05 | 4-20mA Input 2 | uA |


### Holding registers

Access level Read/Write, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit | Range |
| --- | --- | --- | --- | --- |
| 0-10V_OUT_1 | 40001 | 0x00 | mV | 0..10000 |
| 0-10V_OUT_2 | 40002 | 0x01 | mV | 0..10000 |
| 4-20mA_OUT_1 | 40003 | 0x04 | uA | 4000..20000 |
| 4-20mA_OUT_2 | 40004 | 0x05 | uA | 4000..20000 |
| SERVO 1 | 40005 | 0x06 | %/10 | -1200..1200|
| SERVO 2 | 40006 | 0x07 | %/10 | -1200..1200|
| DC_MOTOR | 40007 | 0x08 | %/10 | -1000..1000|


## Function codes implemented

* Read Coils (0x01)
* Read Discrete Inputs (0x02)
* Read Holding Registers (0x03)
* Read Input Registers (0x04)
* Write Single Coil (0x05)
* Write Single Register (0x06)
* Write Multiple Coils (0x0f)
* Write Multiple registers (0x10)
