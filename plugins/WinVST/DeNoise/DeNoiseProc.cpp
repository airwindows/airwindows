/* ========================================
 *  DeNoise - DeNoise.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DeNoise_H
#include "DeNoise.h"
#endif

void DeNoise::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double topThresh =   pow(A*0.25,4.0);
	double pearAThresh = pow(B*0.25,4.0);
	double pearBThresh = pow(C*0.25,4.0);
	double pearCThresh = pow(D*0.25,4.0);
	double pearDThresh = pow(E*0.25,4.0);
	double pearEThresh = pow(F*0.25,4.0);
	double spd = pow(G,3.0) / overallscale;
	double wet = H;
	
	double freqFactor = sqrt(overallscale)+(overallscale*0.5);
	pearA[freq] = pow(0.564,freqFactor+0.85);
	pearB[freq] = pow(0.564,freqFactor+1.9);
	pearC[freq] = pow(0.564,freqFactor+4.1);
	pearD[freq] = pow(0.564,freqFactor+6.2);
	pearE[freq] = pow(0.564,freqFactor+7.7);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;

		for (int x = 0; x < pear_max; x += 4) {
			//begin Pear filter stages
			pearA[figL] = inputSampleL; pearA[figR] = inputSampleR;
			pearA[slew] = ((pearA[figL]-pearA[x])+pearA[x+1])*pearA[freq]*0.5;
			pearA[x] = pearA[figL] = (pearA[freq]*pearA[figL])+((1.0-pearA[freq])*(pearA[x]+pearA[x+1]));
			pearA[x+1] = pearA[slew];
			pearA[slew] = ((pearA[figR]-pearA[x+2])+pearA[x+3])*pearA[freq]*0.5;
			pearA[x+2] = pearA[figR] = (pearA[freq]*pearA[figR])+((1.0-pearA[freq])*(pearA[x+2]+pearA[x+3]));
			pearA[x+3] = pearA[slew];
			inputSampleL -= pearA[figL]; inputSampleR -= pearA[figR];
			
			pearB[figL] = pearA[figL]; pearB[figR] = pearA[figR];
			pearB[slew] = ((pearB[figL]-pearB[x])+pearB[x+1])*pearB[freq]*0.5;
			pearB[x] = pearB[figL] = (pearB[freq]*pearA[figL])+((1.0-pearB[freq])*(pearB[x]+pearB[x+1]));
			pearB[x+1] = pearB[slew];
			pearB[slew] = ((pearB[figR]-pearB[x+2])+pearB[x+3])*pearB[freq]*0.5;
			pearB[x+2] = pearB[figR] = (pearB[freq]*pearA[figR])+((1.0-pearB[freq])*(pearB[x+2]+pearB[x+3]));
			pearB[x+3] = pearB[slew];
			pearA[figL] -= pearB[figL]; pearA[figR] -= pearB[figR];
			
			pearC[figL] = pearB[figL]; pearC[figR] = pearB[figR];
			pearC[slew] = ((pearC[figL]-pearC[x])+pearC[x+1])*pearC[freq]*0.5;
			pearC[x] = pearC[figL] = (pearC[freq]*pearB[figL])+((1.0-pearC[freq])*(pearC[x]+pearC[x+1]));
			pearC[x+1] = pearC[slew];
			pearC[slew] = ((pearC[figR]-pearC[x+2])+pearC[x+3])*pearC[freq]*0.5;
			pearC[x+2] = pearC[figR] = (pearC[freq]*pearB[figR])+((1.0-pearC[freq])*(pearC[x+2]+pearC[x+3]));
			pearC[x+3] = pearC[slew];
			pearB[figL] -= pearC[figL]; pearB[figR] -= pearC[figR];
			
			pearD[figL] = pearC[figL]; pearD[figR] = pearC[figR];
			pearD[slew] = ((pearD[figL]-pearD[x])+pearD[x+1])*pearD[freq]*0.5;
			pearD[x] = pearD[figL] = (pearD[freq]*pearC[figL])+((1.0-pearD[freq])*(pearD[x]+pearD[x+1]));
			pearD[x+1] = pearD[slew];
			pearD[slew] = ((pearD[figR]-pearD[x+2])+pearD[x+3])*pearD[freq]*0.5;
			pearD[x+2] = pearD[figR] = (pearD[freq]*pearC[figR])+((1.0-pearD[freq])*(pearD[x+2]+pearD[x+3]));
			pearD[x+3] = pearD[slew];
			pearC[figL] -= pearD[figL]; pearC[figR] -= pearD[figR];
			
			pearE[figL] = pearD[figL]; pearE[figR] = pearD[figR];
			pearE[slew] = ((pearE[figL]-pearE[x])+pearE[x+1])*pearE[freq]*0.5;
			pearE[x] = pearE[figL] = (pearE[freq]*pearD[figL])+((1.0-pearE[freq])*(pearE[x]+pearE[x+1]));
			pearE[x+1] = pearE[slew];
			pearE[slew] = ((pearE[figR]-pearE[x+2])+pearE[x+3])*pearE[freq]*0.5;
			pearE[x+2] = pearE[figR] = (pearE[freq]*pearD[figR])+((1.0-pearE[freq])*(pearE[x+2]+pearE[x+3]));
			pearE[x+3] = pearE[slew];
			pearD[figL] -= pearE[figL]; pearD[figR] -= pearE[figR];
			
			topLevlL    *= (1.0-spd);
			topLevlL    += ((fabs(inputSampleL)<topThresh)?0.0:spd*2.0);
			inputSampleL = (inputSampleL * fmin(topLevlL,1.0));
			pearA[levlL] *= (1.0-(spd*pearA[freq]));
			pearA[levlL] += ((fabs(pearA[figL])<pearAThresh)?0.0:(spd*2.0*pearA[freq]));
			inputSampleL += (pearA[figL] * fmin(pearA[levlL],1.0));
			pearB[levlL] *= (1.0-(spd*pearB[freq]));
			pearB[levlL] += ((fabs(pearB[figL])<pearBThresh)?0.0:(spd*2.0*pearB[freq]));
			inputSampleL += (pearB[figL] * fmin(pearB[levlL],1.0));
			pearC[levlL] *= (1.0-(spd*pearC[freq]));
			pearC[levlL] += ((fabs(pearC[figL])<pearCThresh)?0.0:(spd*2.0*pearC[freq]));
			inputSampleL += (pearC[figL] * fmin(pearC[levlL],1.0));
			pearD[levlL] *= (1.0-(spd*pearD[freq]));
			pearD[levlL] += ((fabs(pearD[figL])<pearDThresh)?0.0:(spd*2.0*pearD[freq]));
			inputSampleL += (pearD[figL] * fmin(pearD[levlL],1.0));
			pearE[levlL] *= (1.0-(spd*pearE[freq]));						
			pearE[levlL] += ((fabs(pearE[figL])<pearEThresh)?0.0:(spd*2.0*pearE[freq]));
			inputSampleL += (pearE[figL] * fmin(pearE[levlL],1.0));
			
			topLevlR    *= (1.0-spd);
			topLevlR    += ((fabs(inputSampleR)<topThresh)?0.0:spd*2.0);
			inputSampleR = (inputSampleR * fmin(topLevlR,1.0));
			pearA[levlR] *= (1.0-(spd*pearA[freq]));
			pearA[levlR] += ((fabs(pearA[figR])<pearAThresh)?0.0:(spd*2.0*pearA[freq]));
			inputSampleR += (pearA[figR] * fmin(pearA[levlR],1.0));
			pearB[levlR] *= (1.0-(spd*pearB[freq]));
			pearB[levlR] += ((fabs(pearB[figR])<pearBThresh)?0.0:(spd*2.0*pearB[freq]));
			inputSampleR += (pearB[figR] * fmin(pearB[levlR],1.0));
			pearC[levlR] *= (1.0-(spd*pearC[freq]));
			pearC[levlR] += ((fabs(pearC[figR])<pearCThresh)?0.0:(spd*2.0*pearC[freq]));
			inputSampleR += (pearC[figR] * fmin(pearC[levlR],1.0));
			pearD[levlR] *= (1.0-(spd*pearD[freq]));
			pearD[levlR] += ((fabs(pearD[figR])<pearDThresh)?0.0:(spd*2.0*pearD[freq]));
			inputSampleR += (pearD[figR] * fmin(pearD[levlR],1.0));
			pearE[levlR] *= (1.0-(spd*pearE[freq]));						
			pearE[levlR] += ((fabs(pearE[figR])<pearEThresh)?0.0:(spd*2.0*pearE[freq]));
			inputSampleR += (pearE[figR] * fmin(pearE[levlR],1.0));
		}
		
		if (wet == 0.0) {
			drySampleL = inputSampleL - drySampleL;
			drySampleR = inputSampleR - drySampleR;
		} //clicks only at full dry
		
		inputSampleL = (inputSampleL*wet) + (drySampleL*(1.0-wet));
		inputSampleR = (inputSampleR*wet) + (drySampleR*(1.0-wet));
		
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void DeNoise::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double topThresh =   pow(A*0.25,4.0);
	double pearAThresh = pow(B*0.25,4.0);
	double pearBThresh = pow(C*0.25,4.0);
	double pearCThresh = pow(D*0.25,4.0);
	double pearDThresh = pow(E*0.25,4.0);
	double pearEThresh = pow(F*0.25,4.0);
	double spd = pow(G,3.0) / overallscale;
	double wet = H;
	
	double freqFactor = sqrt(overallscale)+(overallscale*0.5);
	pearA[freq] = pow(0.564,freqFactor+0.85);
	pearB[freq] = pow(0.564,freqFactor+1.9);
	pearC[freq] = pow(0.564,freqFactor+4.1);
	pearD[freq] = pow(0.564,freqFactor+6.2);
	pearE[freq] = pow(0.564,freqFactor+7.7);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		for (int x = 0; x < pear_max; x += 4) {
			//begin Pear filter stages
			pearA[figL] = inputSampleL; pearA[figR] = inputSampleR;
			pearA[slew] = ((pearA[figL]-pearA[x])+pearA[x+1])*pearA[freq]*0.5;
			pearA[x] = pearA[figL] = (pearA[freq]*pearA[figL])+((1.0-pearA[freq])*(pearA[x]+pearA[x+1]));
			pearA[x+1] = pearA[slew];
			pearA[slew] = ((pearA[figR]-pearA[x+2])+pearA[x+3])*pearA[freq]*0.5;
			pearA[x+2] = pearA[figR] = (pearA[freq]*pearA[figR])+((1.0-pearA[freq])*(pearA[x+2]+pearA[x+3]));
			pearA[x+3] = pearA[slew];
			inputSampleL -= pearA[figL]; inputSampleR -= pearA[figR];
			
			pearB[figL] = pearA[figL]; pearB[figR] = pearA[figR];
			pearB[slew] = ((pearB[figL]-pearB[x])+pearB[x+1])*pearB[freq]*0.5;
			pearB[x] = pearB[figL] = (pearB[freq]*pearA[figL])+((1.0-pearB[freq])*(pearB[x]+pearB[x+1]));
			pearB[x+1] = pearB[slew];
			pearB[slew] = ((pearB[figR]-pearB[x+2])+pearB[x+3])*pearB[freq]*0.5;
			pearB[x+2] = pearB[figR] = (pearB[freq]*pearA[figR])+((1.0-pearB[freq])*(pearB[x+2]+pearB[x+3]));
			pearB[x+3] = pearB[slew];
			pearA[figL] -= pearB[figL]; pearA[figR] -= pearB[figR];
			
			pearC[figL] = pearB[figL]; pearC[figR] = pearB[figR];
			pearC[slew] = ((pearC[figL]-pearC[x])+pearC[x+1])*pearC[freq]*0.5;
			pearC[x] = pearC[figL] = (pearC[freq]*pearB[figL])+((1.0-pearC[freq])*(pearC[x]+pearC[x+1]));
			pearC[x+1] = pearC[slew];
			pearC[slew] = ((pearC[figR]-pearC[x+2])+pearC[x+3])*pearC[freq]*0.5;
			pearC[x+2] = pearC[figR] = (pearC[freq]*pearB[figR])+((1.0-pearC[freq])*(pearC[x+2]+pearC[x+3]));
			pearC[x+3] = pearC[slew];
			pearB[figL] -= pearC[figL]; pearB[figR] -= pearC[figR];
			
			pearD[figL] = pearC[figL]; pearD[figR] = pearC[figR];
			pearD[slew] = ((pearD[figL]-pearD[x])+pearD[x+1])*pearD[freq]*0.5;
			pearD[x] = pearD[figL] = (pearD[freq]*pearC[figL])+((1.0-pearD[freq])*(pearD[x]+pearD[x+1]));
			pearD[x+1] = pearD[slew];
			pearD[slew] = ((pearD[figR]-pearD[x+2])+pearD[x+3])*pearD[freq]*0.5;
			pearD[x+2] = pearD[figR] = (pearD[freq]*pearC[figR])+((1.0-pearD[freq])*(pearD[x+2]+pearD[x+3]));
			pearD[x+3] = pearD[slew];
			pearC[figL] -= pearD[figL]; pearC[figR] -= pearD[figR];
			
			pearE[figL] = pearD[figL]; pearE[figR] = pearD[figR];
			pearE[slew] = ((pearE[figL]-pearE[x])+pearE[x+1])*pearE[freq]*0.5;
			pearE[x] = pearE[figL] = (pearE[freq]*pearD[figL])+((1.0-pearE[freq])*(pearE[x]+pearE[x+1]));
			pearE[x+1] = pearE[slew];
			pearE[slew] = ((pearE[figR]-pearE[x+2])+pearE[x+3])*pearE[freq]*0.5;
			pearE[x+2] = pearE[figR] = (pearE[freq]*pearD[figR])+((1.0-pearE[freq])*(pearE[x+2]+pearE[x+3]));
			pearE[x+3] = pearE[slew];
			pearD[figL] -= pearE[figL]; pearD[figR] -= pearE[figR];
			
			topLevlL    *= (1.0-spd);
			topLevlL    += ((fabs(inputSampleL)<topThresh)?0.0:spd*2.0);
			inputSampleL = (inputSampleL * fmin(topLevlL,1.0));
			pearA[levlL] *= (1.0-(spd*pearA[freq]));
			pearA[levlL] += ((fabs(pearA[figL])<pearAThresh)?0.0:(spd*2.0*pearA[freq]));
			inputSampleL += (pearA[figL] * fmin(pearA[levlL],1.0));
			pearB[levlL] *= (1.0-(spd*pearB[freq]));
			pearB[levlL] += ((fabs(pearB[figL])<pearBThresh)?0.0:(spd*2.0*pearB[freq]));
			inputSampleL += (pearB[figL] * fmin(pearB[levlL],1.0));
			pearC[levlL] *= (1.0-(spd*pearC[freq]));
			pearC[levlL] += ((fabs(pearC[figL])<pearCThresh)?0.0:(spd*2.0*pearC[freq]));
			inputSampleL += (pearC[figL] * fmin(pearC[levlL],1.0));
			pearD[levlL] *= (1.0-(spd*pearD[freq]));
			pearD[levlL] += ((fabs(pearD[figL])<pearDThresh)?0.0:(spd*2.0*pearD[freq]));
			inputSampleL += (pearD[figL] * fmin(pearD[levlL],1.0));
			pearE[levlL] *= (1.0-(spd*pearE[freq]));						
			pearE[levlL] += ((fabs(pearE[figL])<pearEThresh)?0.0:(spd*2.0*pearE[freq]));
			inputSampleL += (pearE[figL] * fmin(pearE[levlL],1.0));
			
			topLevlR    *= (1.0-spd);
			topLevlR    += ((fabs(inputSampleR)<topThresh)?0.0:spd*2.0);
			inputSampleR = (inputSampleR * fmin(topLevlR,1.0));
			pearA[levlR] *= (1.0-(spd*pearA[freq]));
			pearA[levlR] += ((fabs(pearA[figR])<pearAThresh)?0.0:(spd*2.0*pearA[freq]));
			inputSampleR += (pearA[figR] * fmin(pearA[levlR],1.0));
			pearB[levlR] *= (1.0-(spd*pearB[freq]));
			pearB[levlR] += ((fabs(pearB[figR])<pearBThresh)?0.0:(spd*2.0*pearB[freq]));
			inputSampleR += (pearB[figR] * fmin(pearB[levlR],1.0));
			pearC[levlR] *= (1.0-(spd*pearC[freq]));
			pearC[levlR] += ((fabs(pearC[figR])<pearCThresh)?0.0:(spd*2.0*pearC[freq]));
			inputSampleR += (pearC[figR] * fmin(pearC[levlR],1.0));
			pearD[levlR] *= (1.0-(spd*pearD[freq]));
			pearD[levlR] += ((fabs(pearD[figR])<pearDThresh)?0.0:(spd*2.0*pearD[freq]));
			inputSampleR += (pearD[figR] * fmin(pearD[levlR],1.0));
			pearE[levlR] *= (1.0-(spd*pearE[freq]));						
			pearE[levlR] += ((fabs(pearE[figR])<pearEThresh)?0.0:(spd*2.0*pearE[freq]));
			inputSampleR += (pearE[figR] * fmin(pearE[levlR],1.0));
		}
		
		if (wet == 0.0) {
			drySampleL = inputSampleL - drySampleL;
			drySampleR = inputSampleR - drySampleR;
		} //clicks only at full dry
		
		inputSampleL = (inputSampleL*wet) + (drySampleL*(1.0-wet));
		inputSampleR = (inputSampleR*wet) + (drySampleR*(1.0-wet));
		
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
