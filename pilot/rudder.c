
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <linux/spi/spidev.h>
#include<ncurses.h>
#include<pthread.h>

#include "rudder.h"
#include "conversion.h"

void initRudder(void) {
	
	// Initialize ad-converter
	int adcHandle;
	
	pinMode(6, INPUT);
	pullUpDnControl(6, PUD_OFF);
	
	adcHandle = wiringPiSPISetup(0, 1000000);			// Channel 0, speed 1 MHz, get filedescriptor
	printf("Filedescriptor: %d\n", adcHandle);
	
	char mode = SPI_MODE_1;
	int ioc = ioctl(adcHandle, SPI_IOC_WR_MODE, &mode);	// Set mode = 1
	printf("Mode set return should be > -1: %d\n", ioc);
	ioc = ioctl(adcHandle, SPI_IOC_RD_MODE, &mode);			// Check mode = 1
	printf("Mode get return should be = 1: %d\n", mode);
	
	// Write contents of control registers
	unsigned char ctrl[5];
	ctrl[0] = 0x43;		// WREG cmd, write 4 register starting at 0x00
	ctrl[1] = 0x80;
	ctrl[2] = 0xC4;
	ctrl[3] = 0xC0;
	ctrl[4] = 0x00;
	ioc = wiringPiSPIDataRW(0, ctrl, 5);
	printf("Mode set return should be > -1: %d\n", ioc);

	// Send start/sync command 0x08
	unsigned char cmd[2];
	cmd[0] = 0x08;
	cmd[1] = 0x00;
	ioc = wiringPiSPIDataRW(0, cmd, 1);
	printf("Start/sync command, return should be > -1: %d\n", ioc);
	
	// Initialize PWM outputs
	pinMode(25, OUTPUT);				// EA
	pinMode(1, PWM_OUTPUT);		// PA
	pinMode(24, PWM_OUTPUT);		// PB
	pwmSetMode( PWM_MODE_MS);
	pwmSetRange(1024);
	pwmSetClock(32);
}


int pollRudder(double *angel) {

	static int fsRaw = 0x7fffff;		// Full scale digital
	static double fsU = 3.3;			// Full scale volts
	static double fu = 0;				// Filtered voltage
	static double fk = 0.1;			// Filter constant
	
	//  *******************************************************************************************
	//  Conversion to degrees, y angel in deg, x signal in volt. Straight line y=kx+m. degPerVolt = k. 
	//  uZeroDeg = voltage at 0 deg => m = - k * uZeroDeg = - degPerVolt * uZeroDeg
	//  *******************************************************************************************
	static double degPerVolt = 36.49 ;
	//static double uZeroDeg = 1.756;
	static double uZeroDeg = 1.848;
	
	// Check DRDY#
	if (digitalRead(6) == 0) {
		// Read 3 data bytes
		unsigned char rData[4];
		rData[0] = 0x10;		// RDATA cmd
		rData[1] = 0x00;
		rData[2] = 0x00;
		rData[3] = 0x00;
		wiringPiSPIDataRW(0, rData, 4);
		int res = rData[1] * 256 * 256 + rData[2] * 256 + rData[3];
		
		double u = res * fsU / fsRaw;	// Conversion to Volt
		
		fu = u * fk + (1.0 - fk) * fu; 		// Digital filter
		
		*angel = degPerVolt *fu - degPerVolt * uZeroDeg; 	// Conversion to deg

		return 1;
	}
	
	return 0;
}


void actuateRudder(double rudderSet, double rudderIs) {
	double rudderBound = 10.0;
	// Constants for Florin algorithm
	double db = 0.5;			// Dead band (deg)
	double slow = 1.5;		// Slow speed interval (deg)
	double pFast = 500;		// Max 1024
	double pSlow = 400;
	
	int out = 0;
	double dr = rudderSet - rudderIs;
	dr = deg180to180(dr);	// Ensure right interval
		
	if(dr < -slow) out = - pFast;
	if((-slow < dr) && (dr < -db)) out = - pSlow;
	if((-db < dr) && (dr < db)) out = 0;
	if((db < dr) && (dr < slow)) out = pSlow;
	if( dr > slow) out = pFast;

	if(out > 0) {
		if(rudderIs > rudderBound) {
			printf("Rudder out of bounds (positive)\r\n");
			return;
		}
		digitalWrite(25, HIGH);
		pwmWrite(24, 0);
		pwmWrite(1, out);
	}
	
	if(out < 0) {
		if(rudderIs < -rudderBound) {
			printf("Rudder out of bounds (negative)\r\n");
			return;
		}	
		digitalWrite(25, HIGH);	
		pwmWrite(1, 0);
		pwmWrite(24, -out);
	}
	
	if(out == 0) {
		digitalWrite(25, HIGH);
		pwmWrite(1, 0);
		pwmWrite(24, 0);	
	}
	
	//pthread_mutex_lock(&mutex1);
	//	fprintf(fp, "%f  %f  %d\n", rudderSet, rudderIs, out);
	//pthread_mutex_unlock(&mutex1);
	
	return;
}

