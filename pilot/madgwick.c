
// Sensor fusion with Madgwick filter.		Anders Bruse 2016

#include <stdio.h>
#include <wiringPi.h>
#include <math.h>

#include <gsl/gsl_linalg.h>
#include <gsl/gsl_cblas.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>

#include"conversion.h"

//  Struct for YPR return values
struct fusionResult {
	double yaw;
	double w;
	double wdot;
};


struct fusionResult updateMadgwick(double ax, double ay, double az,
		  double gxDeg, double gyDeg, double gzDeg,
		  double mx, double my, double mz, double deltat) {
		  
	double gx = degtorad(gxDeg);
	double gy = degtorad(gyDeg);
	double gz = degtorad(gzDeg);

	struct fusionResult ret;

	static double q1 = 1;   // quaternion static to keep values between iterations   
	static double q2 = 0;
	static double q3 = 0;
	static double q4 = 0;

	static double gbiasx = 0;
	static double gbiasy = 0;
	static double gbiasz = 0;

	static double beta = 0.31;			//  Best compromise
	//static double beta = 0.62;			//  Chris Winer
	//static double beta = 0.041;		//  Madgwick report
	static double zeta = 0.0030;		//  Madgwick report

	double norm;
	double hx, hy, _2bx, _2bz;
	double s1, s2, s3, s4;
	double qDot1, qDot2, qDot3, qDot4;
    
	double gerrx, gerry, gerrz;        // gyro bias error
    
	// Auxiliary variables to avoid repeated arithmetic
	double _2q1mx;
	double _2q1my;
	double _2q1mz;
	double _2q2mx;
	double _4bx;
	double _4bz;
	double _8bx;
	double _8bz;
	double _2q1 = 2.0f * q1;
	double _2q2 = 2.0f * q2;
	double _2q3 = 2.0f * q3;
	double _2q4 = 2.0f * q4;
	double _2q1q3 = 2.0f * q1 * q3;
	double _2q3q4 = 2.0f * q3 * q4;
	double q1q1 = q1 * q1;
	double q1q2 = q1 * q2;
	double q1q3 = q1 * q3;
	double q1q4 = q1 * q4;
	double q2q2 = q2 * q2;
	double q2q3 = q2 * q3;
	double q2q4 = q2 * q4;
	double q3q3 = q3 * q3;
	double q3q4 = q3 * q4;
	double q4q4 = q4 * q4;
    
	// Normalise accelerometer measurement
	norm = sqrt(ax * ax + ay * ay + az * az);
	if (norm == 0.0f) return ret; // handle NaN
	norm = 1.0f / norm;
	ax *= norm;
	ay *= norm;
	az *= norm;
    
	// Normalise magnetometer measurement
	norm = sqrt(mx * mx + my * my + mz * mz);
	if (norm == 0.0f) return ret; // handle NaN
	norm = 1.0f / norm;
	mx *= norm;
	my *= norm;
	mz *= norm;
	
	/* Old version of algorithm usedKris Winer
	// Reference direction of Earth's magnetic field
	_2q1mx = 2.0f * q1 * mx;
	_2q1my = 2.0f * q1 * my;
	_2q1mz = 2.0f * q1 * mz;
	_2q2mx = 2.0f * q2 * mx;
	hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3 + _2q2 * mz * q4 - mx * q3q3 - mx * q4q4;
	hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2 + my * q3q3 + _2q3 * mz * q4 - my * q4q4;
	_2bx = sqrt(hx * hx + hy * hy);
	_2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2 + _2q3 * my * q4 - mz * q3q3 + mz * q4q4;
	_4bx = 2.0f * _2bx;
	_4bz = 2.0f * _2bz;

	// Gradient decent algorithm corrective step
	s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - ax) + _2q2 * (2.0f * q1q2 + _2q3q4 - ay) - _2bz * q3 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q4 + _2bz * q2) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - ax) + _2q1 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + _2bz * q4 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q4 - _4bz * q2) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - ax) + _2q4 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + (-_4bx * q3 - _2bz * q1) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q1 - _4bz * q3) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - ax) + _2q3 * (2.0f * q1q2 + _2q3q4 - ay) + (-_4bx * q4 + _2bz * q2) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q1 + _2bz * q3) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q2 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	*/
	
	//Updated algorithm
	// Reference direction of Earth's magnetic field
	_2q1mx = 2.0f * q1 * mx;
	_2q1my = 2.0f * q1 * my;
	_2q1mz = 2.0f * q1 * mz;
	_2q2mx = 2.0f * q2 * mx;
	hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3 + _2q2 * mz * q4 - mx * q3q3 - mx * q4q4;
	hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2 + my * q3q3 + _2q3 * mz * q4 - my * q4q4;
	_2bx = sqrt(hx * hx + hy * hy);
	_2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2 + _2q3 * my * q4 - mz * q3q3 + mz * q4q4;
	_4bx = 2.0f * _2bx;
	_4bz = 2.0f * _2bz;
	_8bx = 2.0f * _4bx;
	_8bz = 2.0f * _4bz;

	// Gradient decent algorithm corrective step
	s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - ax) + _2q2 * (2.0f * q1q2 + _2q3q4 - ay) - _4bz * q3 * (_4bx * (0.5f - q3q3 - q4q4) + _4bz * (q2q4 - q1q3) - mx) + (-_4bx * q4 + _4bz * q2) * (_4bx * (q2q3 - q1q4) + _4bz * (q1q2 + q3q4) - my) + _4bx * q3 * (_4bx * (q1q3 + q2q4) + _4bz * (0.5f - q2q2 - q3q3) - mz);
	s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - ax) + _2q1 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + _4bz * q4 * (_4bx * (0.5f - q3q3 - q4q4) + _4bz * (q2q4 - q1q3) - mx) + (_4bx * q3 + _4bz * q1) * (_4bx * (q2q3 - q1q4) + _4bz * (q1q2 + q3q4) - my) + (_4bx * q4 - _8bz * q2) * (_4bx * (q1q3 + q2q4) + _4bz * (0.5f - q2q2 - q3q3) - mz);
	s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - ax) + _2q4 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + (-_8bx * q3 - _4bz * q1) * (_4bx * (0.5f - q3q3 - q4q4) + _4bz * (q2q4 - q1q3) - mx) + (_4bx * q2 + _4bz * q4) * (_4bx * (q2q3 - q1q4) + _4bz * (q1q2 + q3q4) - my) + (_4bx * q1 - _8bz * q3) * (_4bx * (q1q3 + q2q4) + _4bz * (0.5f - q2q2 - q3q3) - mz);
	s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - ax) + _2q3 * (2.0f * q1q2 + _2q3q4 - ay) + (-_8bx * q4 + _4bz * q2) * (_4bx * (0.5f - q3q3 - q4q4) + _4bz * (q2q4 - q1q3) - mx) + (-_4bx * q1 + _4bz * q3) * (_4bx * (q2q3 - q1q4) + _4bz * (q1q2 + q3q4) - my) + _4bx * q2 * (_4bx * (q1q3 + q2q4) + _4bz * (0.5f - q2q2 - q3q3) - mz);
	
	norm = sqrt(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4);    // normalise step magnitude
	norm = 1.0f / norm;
	s1 *= norm;
	s2 *= norm;
	s3 *= norm;
	s4 *= norm;

	// Compute estimated gyroscope biases
	gerrx = _2q1 * s2 - _2q2 * s1 - _2q3 * s4 + _2q4 * s3;
	gerry = _2q1 * s3 + _2q2 * s4 - _2q3 * s1 - _2q4 * s2;
	gerrz = _2q1 * s4 - _2q2 * s3 + _2q3 * s2 - _2q4 * s1;

	// Compute and remove gyroscope biases
	gbiasx += gerrx * deltat * zeta;
	gbiasy += gerry * deltat * zeta;
	gbiasz += gerrz * deltat * zeta;
	gx -= gbiasx;
	gy -= gbiasy;
	gz -= gbiasz;
	
	// Compute rate of change of quaternion
	qDot1 = 0.5f * (-q2 * gx - q3 * gy - q4 * gz) - beta * s1;
	qDot2 = 0.5f * (q1 * gx + q3 * gz - q4 * gy) - beta * s2;
	qDot3 = 0.5f * (q1 * gy - q2 * gz + q4 * gx) - beta * s3;
	qDot4 = 0.5f * (q1 * gz + q2 * gy - q3 * gx) - beta * s4;

	// Integrate to yield quaternion
	q1 += qDot1 * deltat;
	q2 += qDot2 * deltat;
	q3 += qDot3 * deltat;
	q4 += qDot4 * deltat;
	norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
	norm = 1.0f / norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
	q4 = q4 * norm;

	//  Change to positive clockwise and downwards
	double yaw = deg0to360(- radtodeg( atan2(2*(q1*q4+q2*q3), 1-2*(q3*q3+q4*q4))));		//  Return value in deg (0 to 360 deg)
	double w = -gz;
	
	// Numerical differentiation to get wdot
	static double Tw = 0.25;
	static double xw = 0;
	
	xw = deltat / Tw * (-xw + w) + xw;
	double wdot = 1 / Tw * (-xw + w);
	
	// Assign return values
	ret.yaw = yaw;
	ret.w = w;
	ret.wdot = wdot;
	
	return ret;
}
