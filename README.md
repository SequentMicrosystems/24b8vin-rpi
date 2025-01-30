# 24b8vin-rpi

Drivers for [Eight 24-Bit Analog Inputs DAQ 8-Layer Stackable HAT for RaspberryPi](https://sequentmicrosystems.com/products/eight-24bit-analog-inputs-daq-8-layer-stackable-hat-for-raspberry-pi)

![image](https://github.com/user-attachments/assets/853b57af-b7a1-4809-9c44-6f8753f03398)

## Setup

Enable I2C communication first:
```bash
sudo raspi-config
```
A good article about I2C on Raspberry can be found [here](https://www.raspberrypi-spy.co.uk/2014/11/enabling-the-i2c-interface-on-the-raspberry-pi/).

If you use Ubuntu you need to install `raspi-config` first:
```bash
sudo apt update
sudo apt install raspi-config
```

Make sure you have all tools you need:
```bash
sudo apt update
sudo apt-get install git build-essential
```

## Usage

Install the command:
```bash
cd
git clone https://github.com/SequentMicrosystems/24b8vin-rpi.git
cd 24b8vin-rpi/
sudo make install
```

Now you can access all the functions of the [Eight 24-Bit Analog Inputs DAQ](https://sequentmicrosystems.com/products/eight-24bit-analog-inputs-daq-8-layer-stackable-hat-for-raspberry-pi) through the command "24b8vin". Use -h option for help:
```bash
24b8vin -h
```

If you clone the repository any update can be made with the following commands:
```bash
cd ~/24b8vin-rpi/  
git pull
sudo make install
```


### Gain codes

| **Gain Code** | **Voltage Range (V)** |
|---------------|-------------------------|
| 0             | ±24                     |
| 1             | ±12                     |
| 2             | ±6                      |
| 3             | ±3                      |
| 4             | ±1.5                    |
| 5             | ±0.75                   |
| 6             | ±0.37                   |
| 7             | ±0.18                   |

#### Example usage
```bash
24b8vin 0 grd 2
```
> Read gain code for input channel #2 on board #0

```bash
24b8vin 0 gwr 2 4
```
> Set gain code to 4(±1.5V) for input channel #2 on board #0
