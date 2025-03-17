# update

This is the Eight 24-Bit Analog Inputs DAQ 8-Layer Stackable HAT for RaspberryPi firmware update tool.

## Usage

```bash 
~$ git clone https://github.com/SequentMicrosystems/24b8vin-rpi.git
~$ cd 24b8vin-rpi/update/ 
~/24b8vin-rpi/update$ ./update 0 
```

For the 64-bit OS, replace the last line with:
```bash
~/24b8vin-rpi/update$ ./update64 0 
```

If you already cloned the repository, skip the first step.
The command will download the newest firmware version from our server and write it to the board. 
The stack level of the board must be provided as a parameter.
Make sure there is no script or program that acces the I2C port during the update process.
