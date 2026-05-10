/* ========================================
 *  Longhand - Longhand.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Longhand_H
#include "Longhand.h"
#endif

void Longhand::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double inputGain = pow(A,2.0)*7.72;//normalized input does not wavefold too far
	double outputGain = B;		
	double wet = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		inputSampleL = fmin(fmax(inputSampleL,-1.0),1.0);
		inputSampleR = fmin(fmax(inputSampleR,-1.0),1.0);
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;

		if (inputGain != 1.0) {
			inputSampleL *= inputGain;
			inputSampleR *= inputGain;
			drySampleL *= inputGain;
			drySampleR *= inputGain;
		}
		
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
		
		if (outputGain != 1.0) {
			inputSampleL *= outputGain;
			inputSampleR *= outputGain;
		}
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		
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

void Longhand::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double inputGain = pow(A,2.0)*7.72;//normalized input does not wavefold too far
	double outputGain = B;		
	double wet = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		inputSampleL = fmin(fmax(inputSampleL,-1.0),1.0);
		inputSampleR = fmin(fmax(inputSampleR,-1.0),1.0);
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (inputGain != 1.0) {
			inputSampleL *= inputGain;
			inputSampleR *= inputGain;
			drySampleL *= inputGain;
			drySampleR *= inputGain;
		}
		
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
		
		if (outputGain != 1.0) {
			inputSampleL *= outputGain;
			inputSampleR *= outputGain;
		}
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		
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
