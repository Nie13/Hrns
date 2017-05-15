# Hrns
for code used on STM32 board
the andoird cell phone code, check out at https://github.com/Nie13/Hrns_android.git

#
#
#Load the Embedded.ino file into STM32 board from Embedded folder;
#
#There are 3 function timers used in program in total;
#Timer 0 used for read and calculate data from accelemeter, keep updating data read from 3 axis. Before the program start, a read accelerate is scheduled to measure what normal accelerate is. It is recorded and kept as a standard through the entire program. User should not move and stand still during the first 0.1 second after pressing the reset button (this is the calibrate void). Any data measured, as we set at accx, accy, and accz, need to go through sqrt(pow(accx-avgx, 2)+pow(accy-avgy,2)+pow(accz-avgz,2)), to get the total scaler sum of the accelerate vector. Thus, in order to prevent huge change in data measured, the final outcome is actually the average of current scaler sum and previous scaler sum (this part in getPedo int). Also, this data is saved in totave. Once totave goes over threshold and flag is down, flag up, we consider this as one single step. Once totave goes lower than threshold and flag is up, the flag down. Else nothing would happen. The step threshold get updated when energy is calculated, while energy is the last 15 totave avergy, indecating what 
#
#Timer 1 is used to send data to the android application. All numbers are calculated on the board and application is only used for presenting data.
#
#Timer 2 is used for calculate heartrate. Basically we record the time lasted between two heart beats. After adding it all together, we can get the period that how long it takes to beat how many times. So, by using beat divide by time and multiplied by 60 we can get the heart beat per minute (these functions are executed in readHeartrate and calcBPM, also we set the threshold of the heartrate to be 4000; as the ADC level in the STM32 is 0 to 4095.
#
#Bluetooth is sent through Serial2.
