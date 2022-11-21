/* ========================================
 *  Thunder - Thunder.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Thunder_H
#include "Thunder.h"
#endif

void Thunder::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double thunder = A * 0.4;
	double threshold = 1.0 - (thunder * 2.0);
	if (threshold < 0.01) threshold = 0.01;
	double muMakeupGain = 1.0 / threshold;
	double release = pow((1.28-thunder),5)*32768.0;
	release /= overallscale;
	double fastest = sqrt(release);
	double EQ = ((0.0275 / getSampleRate())*32000.0);
	double dcblock = EQ / 300.0;
	double basstrim = (0.01/EQ)+1.0;
	//FF parameters also ride off Speed
	double outputGain = B;
	
	double coefficient;
	double inputSense;
	
	double resultL;
	double resultR;
	double resultM;
	double resultML;
	double resultMR;
	
	double inputSampleL;
	double inputSampleR;
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		inputSampleL = inputSampleL * muMakeupGain;
		inputSampleR = inputSampleR * muMakeupGain;
		
		if (gateL < fabs(inputSampleL)) gateL = inputSampleL;
		else gateL -= dcblock;
		if (gateR < fabs(inputSampleR)) gateR = inputSampleR;
		else gateR -= dcblock;
		//setting up gated DC blocking to control the tendency for rumble and offset
		
		//begin three FathomFive stages
		iirSampleAL += (inputSampleL * EQ * thunder);
		iirSampleAL -= (iirSampleAL * iirSampleAL * iirSampleAL * EQ);
		if (iirSampleAL > gateL) iirSampleAL -= dcblock;
		if (iirSampleAL < -gateL) iirSampleAL += dcblock;
		resultL = iirSampleAL*basstrim;
		iirSampleBL = (iirSampleBL * (1 - EQ)) + (resultL * EQ);
		resultL = iirSampleBL;
		
		iirSampleAR += (inputSampleR * EQ * thunder);
		iirSampleAR -= (iirSampleAR * iirSampleAR * iirSampleAR * EQ);
		if (iirSampleAR > gateR) iirSampleAR -= dcblock;
		if (iirSampleAR < -gateR) iirSampleAR += dcblock;
		resultR = iirSampleAR*basstrim;
		iirSampleBR = (iirSampleBR * (1 - EQ)) + (resultR * EQ);
		resultR = iirSampleBR;
		
		iirSampleAM += ((inputSampleL + inputSampleR) * EQ * thunder);
		iirSampleAM -= (iirSampleAM * iirSampleAM * iirSampleAM * EQ);
		resultM = iirSampleAM*basstrim;
		iirSampleBM = (iirSampleBM * (1 - EQ)) + (resultM * EQ);
		resultM = iirSampleBM;
		iirSampleCM = (iirSampleCM * (1 - EQ)) + (resultM * EQ);
		
		resultM = fabs(iirSampleCM);
		resultML = fabs(resultL);
		resultMR = fabs(resultR);
		
		if (resultM > resultML) resultML = resultM;
		if (resultM > resultMR) resultMR = resultM;
		//trying to restrict the buzziness
		
		if (resultML > 1.0) resultML = 1.0;
		if (resultMR > 1.0) resultMR = 1.0;
		//now we have result L, R and M the trigger modulator which must be 0-1
		
		//begin compressor section
		inputSampleL -= (iirSampleBL * thunder);
		inputSampleR -= (iirSampleBR * thunder);
		//highpass the comp section by sneaking out what will be the reinforcement
		
		inputSense = fabs(inputSampleL);
		if (fabs(inputSampleR) > inputSense)
			inputSense = fabs(inputSampleR);
		//we will take the greater of either channel and just use that, then apply the result
		//to both stereo channels.
		
		if (flip)
		{
			if (inputSense > threshold)
			{
				muVary = threshold / inputSense;
				muAttack = sqrt(fabs(muSpeedA));
				muCoefficientA = muCoefficientA * (muAttack-1.0);
				if (muVary < threshold)
				{
					muCoefficientA = muCoefficientA + threshold;
				}
				else
				{
					muCoefficientA = muCoefficientA + muVary;
				}
				muCoefficientA = muCoefficientA / muAttack;
			}
			else
			{
				muCoefficientA = muCoefficientA * ((muSpeedA * muSpeedA)-1.0);
				muCoefficientA = muCoefficientA + 1.0;
				muCoefficientA = muCoefficientA / (muSpeedA * muSpeedA);
			}
			muNewSpeed = muSpeedA * (muSpeedA-1);
			muNewSpeed = muNewSpeed + fabs(inputSense*release)+fastest;
			muSpeedA = muNewSpeed / muSpeedA;
		}
		else
		{
			if (inputSense > threshold)
			{
				muVary = threshold / inputSense;
				muAttack = sqrt(fabs(muSpeedB));
				muCoefficientB = muCoefficientB * (muAttack-1);
				if (muVary < threshold)
				{
					muCoefficientB = muCoefficientB + threshold;
				}
				else
				{
					muCoefficientB = muCoefficientB + muVary;
				}
				muCoefficientB = muCoefficientB / muAttack;
			}
			else
			{
				muCoefficientB = muCoefficientB * ((muSpeedB * muSpeedB)-1.0);
				muCoefficientB = muCoefficientB + 1.0;
				muCoefficientB = muCoefficientB / (muSpeedB * muSpeedB);
			}
			muNewSpeed = muSpeedB * (muSpeedB-1);
			muNewSpeed = muNewSpeed + fabs(inputSense*release)+fastest;
			muSpeedB = muNewSpeed / muSpeedB;
		}
		//got coefficients, adjusted speeds
		
		if (flip)
		{
			coefficient = pow(muCoefficientA,2);
			inputSampleL *= coefficient;
			inputSampleR *= coefficient;
		}
		else
		{
			coefficient = pow(muCoefficientB,2);
			inputSampleL *= coefficient;
			inputSampleR *= coefficient;
		}
		//applied compression with vari-vari-µ-µ-µ-µ-µ-µ-is-the-kitten-song o/~
		//applied gain correction to control output level- tends to constrain sound rather than inflate it
		
		inputSampleL += (resultL * resultM);
		inputSampleR += (resultR * resultM);
		//combine the two by adding the summed channnel of lows
		
		if (outputGain != 1.0) {
			inputSampleL *= outputGain;
			inputSampleR *= outputGain;
		}
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void Thunder::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double thunder = A * 0.4;
	double threshold = 1.0 - (thunder * 2.0);
	if (threshold < 0.01) threshold = 0.01;
	double muMakeupGain = 1.0 / threshold;
	double release = pow((1.28-thunder),5)*32768.0;
	release /= overallscale;
	double fastest = sqrt(release);
	double EQ = ((0.0275 / getSampleRate())*32000.0);
	double dcblock = EQ / 300.0;
	double basstrim = (0.01/EQ)+1.0;
	//FF parameters also ride off Speed
	double outputGain = B;
	
	double coefficient;
	double inputSense;
	
	double resultL;
	double resultR;
	double resultM;
	double resultML;
	double resultMR;
	
	double inputSampleL;
	double inputSampleR;
		
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		inputSampleL = inputSampleL * muMakeupGain;
		inputSampleR = inputSampleR * muMakeupGain;
		
		if (gateL < fabs(inputSampleL)) gateL = inputSampleL;
		else gateL -= dcblock;
		if (gateR < fabs(inputSampleR)) gateR = inputSampleR;
		else gateR -= dcblock;
		//setting up gated DC blocking to control the tendency for rumble and offset
		
		//begin three FathomFive stages
		iirSampleAL += (inputSampleL * EQ * thunder);
		iirSampleAL -= (iirSampleAL * iirSampleAL * iirSampleAL * EQ);
		if (iirSampleAL > gateL) iirSampleAL -= dcblock;
		if (iirSampleAL < -gateL) iirSampleAL += dcblock;
		resultL = iirSampleAL*basstrim;
		iirSampleBL = (iirSampleBL * (1 - EQ)) + (resultL * EQ);
		resultL = iirSampleBL;
		
		iirSampleAR += (inputSampleR * EQ * thunder);
		iirSampleAR -= (iirSampleAR * iirSampleAR * iirSampleAR * EQ);
		if (iirSampleAR > gateR) iirSampleAR -= dcblock;
		if (iirSampleAR < -gateR) iirSampleAR += dcblock;
		resultR = iirSampleAR*basstrim;
		iirSampleBR = (iirSampleBR * (1 - EQ)) + (resultR * EQ);
		resultR = iirSampleBR;
		
		iirSampleAM += ((inputSampleL + inputSampleR) * EQ * thunder);
		iirSampleAM -= (iirSampleAM * iirSampleAM * iirSampleAM * EQ);
		resultM = iirSampleAM*basstrim;
		iirSampleBM = (iirSampleBM * (1 - EQ)) + (resultM * EQ);
		resultM = iirSampleBM;
		iirSampleCM = (iirSampleCM * (1 - EQ)) + (resultM * EQ);
		
		resultM = fabs(iirSampleCM);
		resultML = fabs(resultL);
		resultMR = fabs(resultR);
		
		if (resultM > resultML) resultML = resultM;
		if (resultM > resultMR) resultMR = resultM;
		//trying to restrict the buzziness
		
		if (resultML > 1.0) resultML = 1.0;
		if (resultMR > 1.0) resultMR = 1.0;
		//now we have result L, R and M the trigger modulator which must be 0-1
		
		//begin compressor section
		inputSampleL -= (iirSampleBL * thunder);
		inputSampleR -= (iirSampleBR * thunder);
		//highpass the comp section by sneaking out what will be the reinforcement
		
		inputSense = fabs(inputSampleL);
		if (fabs(inputSampleR) > inputSense)
			inputSense = fabs(inputSampleR);
		//we will take the greater of either channel and just use that, then apply the result
		//to both stereo channels.
		
		if (flip)
		{
			if (inputSense > threshold)
			{
				muVary = threshold / inputSense;
				muAttack = sqrt(fabs(muSpeedA));
				muCoefficientA = muCoefficientA * (muAttack-1.0);
				if (muVary < threshold)
				{
					muCoefficientA = muCoefficientA + threshold;
				}
				else
				{
					muCoefficientA = muCoefficientA + muVary;
				}
				muCoefficientA = muCoefficientA / muAttack;
			}
			else
			{
				muCoefficientA = muCoefficientA * ((muSpeedA * muSpeedA)-1.0);
				muCoefficientA = muCoefficientA + 1.0;
				muCoefficientA = muCoefficientA / (muSpeedA * muSpeedA);
			}
			muNewSpeed = muSpeedA * (muSpeedA-1);
			muNewSpeed = muNewSpeed + fabs(inputSense*release)+fastest;
			muSpeedA = muNewSpeed / muSpeedA;
		}
		else
		{
			if (inputSense > threshold)
			{
				muVary = threshold / inputSense;
				muAttack = sqrt(fabs(muSpeedB));
				muCoefficientB = muCoefficientB * (muAttack-1);
				if (muVary < threshold)
				{
					muCoefficientB = muCoefficientB + threshold;
				}
				else
				{
					muCoefficientB = muCoefficientB + muVary;
				}
				muCoefficientB = muCoefficientB / muAttack;
			}
			else
			{
				muCoefficientB = muCoefficientB * ((muSpeedB * muSpeedB)-1.0);
				muCoefficientB = muCoefficientB + 1.0;
				muCoefficientB = muCoefficientB / (muSpeedB * muSpeedB);
			}
			muNewSpeed = muSpeedB * (muSpeedB-1);
			muNewSpeed = muNewSpeed + fabs(inputSense*release)+fastest;
			muSpeedB = muNewSpeed / muSpeedB;
		}
		//got coefficients, adjusted speeds
		
		if (flip)
		{
			coefficient = pow(muCoefficientA,2);
			inputSampleL *= coefficient;
			inputSampleR *= coefficient;
		}
		else
		{
			coefficient = pow(muCoefficientB,2);
			inputSampleL *= coefficient;
			inputSampleR *= coefficient;
		}
		//applied compression with vari-vari-µ-µ-µ-µ-µ-µ-is-the-kitten-song o/~
		//applied gain correction to control output level- tends to constrain sound rather than inflate it
		
		inputSampleL += (resultL * resultM);
		inputSampleR += (resultR * resultM);
		//combine the two by adding the summed channnel of lows
		
		if (outputGain != 1.0) {
			inputSampleL *= outputGain;
			inputSampleR *= outputGain;
		}
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}