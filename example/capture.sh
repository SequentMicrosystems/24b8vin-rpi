
#set sample rate to 2000 samples per second, this parameter must be set only once because it is saved to the card flash memory
24b8vin 0 srwr 3 

#setup the capture parameters for channel 1 of 4000  samples when the input voltage exceeds 1.23V
24b8vin 0 scfgwr 1 4000 1.23 

#begin monitoring the input. If you wish to start the capture immediately, run "24b8vin 0 start 1"
24b8vin 0 start 0 

a=0
while [ $a -lt 4000 ] # wait for the card to finish the capture
do
    sleep 0.25
    a=$(24b8vin 0 fifolvl)
done
echo "Done capture, start to read samples"
#Save all the samples in a .csv file 
for i in {0..4000} ; do 24b8vin 0 fiford ; done >> /home/pi/fifo-"`date +"%H:%M_%d-%m-%Y"`".csv

echo "Done reading the samples"

