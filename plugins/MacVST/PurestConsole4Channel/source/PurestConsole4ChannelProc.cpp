/* ========================================
 *  PurestConsole4Channel - PurestConsole4Channel.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestConsole4Channel_H
#include "PurestConsole4Channel.h"
#endif

void PurestConsole4Channel::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double bezierRez = fmax(pow((1.0-SMO)*0.25,3.0)/overallscale,0.00001); 
	int stepped = 999999; if (bezierRez > 0.000001) stepped = (int)(1.0/bezierRez); bezierRez = 1.0/stepped;
	double bezierTrim = 1.0-(bezierRez*((double)stepped/(stepped+1.0)));
	//manages the overall Bezier control smoothing system plugin-wide and feed all controls
	//into bezier[] as just 0-1 values, unprocessed. do it IN the control smoothing engine
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//begin Bezier control smoothing engine
		bezier[bezier_cycle] += bezierRez;
		if (bezier[bezier_cycle] > 1.0) {bezier[bezier_cycle] = 0.0;
			bezier[bezierGainL_C] = bezier[bezierGainL_B]; bezier[bezierGainL_B] = bezier[bezierGainL_A];
			bezier[bezierGainR_C] = bezier[bezierGainR_B]; bezier[bezierGainR_B] = bezier[bezierGainR_A];
			//one of these bucket brigade lines for every smoothed control
			//begin expensive control calculations
			double gain = FAD*2.0;
			if (gain > 1.0) gain *= gain;
			if (gain < 1.0) gain = 1.0-pow(1.0-gain,2);
			gain *= 0.763932022500211;
			bezier[bezierGainL_A] = gain*sin(M_PI_2-(PAN*M_PI_2));
			bezier[bezierGainR_A] = gain*sin(PAN*M_PI_2);
			//end expensive control calculations
		} double lerp = bezier[bezier_cycle]*bezierTrim;
		bezier[bezierGainL_Out] = bezier[bezierGainL_B]+(bezier[bezierGainL_C]*(1.0-lerp)*(1.0-lerp))+(bezier[bezierGainL_B]*2.0*(1.0-lerp)*lerp)+(bezier[bezierGainL_A]*lerp*lerp);
		bezier[bezierGainR_Out] = bezier[bezierGainR_B]+(bezier[bezierGainR_C]*(1.0-lerp)*(1.0-lerp))+(bezier[bezierGainR_B]*2.0*(1.0-lerp)*lerp)+(bezier[bezierGainR_A]*lerp*lerp);
		//end Bezier control smoothing engine
		
		inputSampleL *= bezier[bezierGainL_Out];
		inputSampleR *= bezier[bezierGainR_Out];
		
		inputSampleL = fmax(fmin(inputSampleL,M_PI_2),-M_PI_2);
		inputSampleR = fmax(fmin(inputSampleR,M_PI_2),-M_PI_2);
		long double X = inputSampleL; X *= X; //long double for even
		long double temp = inputSampleL * X; //the initial multiplies
		inputSampleL -= temp*0.16666666666666666666666666666666666; temp *= X;
		inputSampleL += temp*0.00833333333333333333333333333333333; temp *= X;
		inputSampleL -= temp*0.00019841269841269841269841269841269; temp *= X;
		inputSampleL += temp*0.00000275573192239858906525573192239; temp *= X;
		inputSampleL -= temp*0.00000002505210838544171877521083854; temp *= X;
		inputSampleL += temp*0.00000000016059043836821614599392377; temp *= X;
		inputSampleL -= temp*0.00000000000076471637318198164759011; temp *= X;
		inputSampleL += temp*0.00000000000000281145725434552076319; temp *= X;
		inputSampleL -= temp*0.00000000000000000822063524662432971; temp *= X;
		inputSampleL += temp*0.00000000000000000001957294106339126;
		X = inputSampleR; X *= X; //long double for even
		temp = inputSampleR * X; //the initial multiplies
		inputSampleR -= temp*0.16666666666666666666666666666666666; temp *= X;
		inputSampleR += temp*0.00833333333333333333333333333333333; temp *= X;
		inputSampleR -= temp*0.00019841269841269841269841269841269; temp *= X;
		inputSampleR += temp*0.00000275573192239858906525573192239; temp *= X;
		inputSampleR -= temp*0.00000002505210838544171877521083854; temp *= X;
		inputSampleR += temp*0.00000000016059043836821614599392377; temp *= X;
		inputSampleR -= temp*0.00000000000076471637318198164759011; temp *= X;
		inputSampleR += temp*0.00000000000000281145725434552076319; temp *= X;
		inputSampleR -= temp*0.00000000000000000822063524662432971; temp *= X;
		inputSampleR += temp*0.00000000000000000001957294106339126;
		//retain mantissa of a long double increasing power function
		//long double probably doesn't handle more than 36 digits or so
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void PurestConsole4Channel::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double bezierRez = fmax(pow((1.0-SMO)*0.25,3.0)/overallscale,0.00001); 
	int stepped = 999999; if (bezierRez > 0.000001) stepped = (int)(1.0/bezierRez); bezierRez = 1.0/stepped;
	double bezierTrim = 1.0-(bezierRez*((double)stepped/(stepped+1.0)));
	//manages the overall Bezier control smoothing system plugin-wide and feed all controls
	//into bezier[] as just 0-1 values, unprocessed. do it IN the control smoothing engine
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//begin Bezier control smoothing engine
		bezier[bezier_cycle] += bezierRez;
		if (bezier[bezier_cycle] > 1.0) {bezier[bezier_cycle] = 0.0;
			bezier[bezierGainL_C] = bezier[bezierGainL_B]; bezier[bezierGainL_B] = bezier[bezierGainL_A];
			bezier[bezierGainR_C] = bezier[bezierGainR_B]; bezier[bezierGainR_B] = bezier[bezierGainR_A];
			//one of these bucket brigade lines for every smoothed control
			//begin expensive control calculations
			double gain = FAD*2.0;
			if (gain > 1.0) gain *= gain;
			if (gain < 1.0) gain = 1.0-pow(1.0-gain,2);
			gain *= 0.763932022500211;
			bezier[bezierGainL_A] = gain*sin(M_PI_2-(PAN*M_PI_2));
			bezier[bezierGainR_A] = gain*sin(PAN*M_PI_2);
			//end expensive control calculations
		} double lerp = bezier[bezier_cycle]*bezierTrim;
		bezier[bezierGainL_Out] = bezier[bezierGainL_B]+(bezier[bezierGainL_C]*(1.0-lerp)*(1.0-lerp))+(bezier[bezierGainL_B]*2.0*(1.0-lerp)*lerp)+(bezier[bezierGainL_A]*lerp*lerp);
		bezier[bezierGainR_Out] = bezier[bezierGainR_B]+(bezier[bezierGainR_C]*(1.0-lerp)*(1.0-lerp))+(bezier[bezierGainR_B]*2.0*(1.0-lerp)*lerp)+(bezier[bezierGainR_A]*lerp*lerp);
		//end Bezier control smoothing engine
		
		inputSampleL *= bezier[bezierGainL_Out];
		inputSampleR *= bezier[bezierGainR_Out];
		
		inputSampleL = fmax(fmin(inputSampleL,M_PI_2),-M_PI_2);
		inputSampleR = fmax(fmin(inputSampleR,M_PI_2),-M_PI_2);
		long double X = inputSampleL; X *= X; //long double for even
		long double temp = inputSampleL * X; //the initial multiplies
		inputSampleL -= temp*0.16666666666666666666666666666666666; temp *= X;
		inputSampleL += temp*0.00833333333333333333333333333333333; temp *= X;
		inputSampleL -= temp*0.00019841269841269841269841269841269; temp *= X;
		inputSampleL += temp*0.00000275573192239858906525573192239; temp *= X;
		inputSampleL -= temp*0.00000002505210838544171877521083854; temp *= X;
		inputSampleL += temp*0.00000000016059043836821614599392377; temp *= X;
		inputSampleL -= temp*0.00000000000076471637318198164759011; temp *= X;
		inputSampleL += temp*0.00000000000000281145725434552076319; temp *= X;
		inputSampleL -= temp*0.00000000000000000822063524662432971; temp *= X;
		inputSampleL += temp*0.00000000000000000001957294106339126;
		X = inputSampleR; X *= X; //long double for even
		temp = inputSampleR * X; //the initial multiplies
		inputSampleR -= temp*0.16666666666666666666666666666666666; temp *= X;
		inputSampleR += temp*0.00833333333333333333333333333333333; temp *= X;
		inputSampleR -= temp*0.00019841269841269841269841269841269; temp *= X;
		inputSampleR += temp*0.00000275573192239858906525573192239; temp *= X;
		inputSampleR -= temp*0.00000002505210838544171877521083854; temp *= X;
		inputSampleR += temp*0.00000000016059043836821614599392377; temp *= X;
		inputSampleR -= temp*0.00000000000076471637318198164759011; temp *= X;
		inputSampleR += temp*0.00000000000000281145725434552076319; temp *= X;
		inputSampleR -= temp*0.00000000000000000822063524662432971; temp *= X;
		inputSampleR += temp*0.00000000000000000001957294106339126;
		//retain mantissa of a long double increasing power function
		//long double probably doesn't handle more than 36 digits or so
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 3.553e-44l  * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
