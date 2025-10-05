/* ========================================
 *  PearEQ - PearEQ.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PearEQ_H
#include "PearEQ.h"
#endif

void PearEQ::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double topLevl = sqrt(A+0.5);
	pearA[levl] = sqrt(B+0.5);
	pearB[levl] = sqrt(C+0.5);
	pearC[levl] = sqrt(D+0.5);
	pearD[levl] = sqrt(E+0.5);
	pearE[levl] = sqrt(F+0.5);
	
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
			
			inputSampleL *= topLevl; inputSampleR *= topLevl;
			inputSampleL += (pearA[figL]*pearA[levl]); inputSampleR += (pearA[figR]*pearA[levl]);
			inputSampleL += (pearB[figL]*pearB[levl]); inputSampleR += (pearB[figR]*pearB[levl]);
			inputSampleL += (pearC[figL]*pearC[levl]); inputSampleR += (pearC[figR]*pearC[levl]);
			inputSampleL += (pearD[figL]*pearD[levl]); inputSampleR += (pearD[figR]*pearD[levl]);
			inputSampleL += (pearE[figL]*pearE[levl]); inputSampleR += (pearE[figR]*pearE[levl]);
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void PearEQ::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double topLevl = sqrt(A+0.5);
	pearA[levl] = sqrt(B+0.5);
	pearB[levl] = sqrt(C+0.5);
	pearC[levl] = sqrt(D+0.5);
	pearD[levl] = sqrt(E+0.5);
	pearE[levl] = sqrt(F+0.5);
	
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
			
			inputSampleL *= topLevl; inputSampleR *= topLevl;
			inputSampleL += (pearA[figL]*pearA[levl]); inputSampleR += (pearA[figR]*pearA[levl]);
			inputSampleL += (pearB[figL]*pearB[levl]); inputSampleR += (pearB[figR]*pearB[levl]);
			inputSampleL += (pearC[figL]*pearC[levl]); inputSampleR += (pearC[figR]*pearC[levl]);
			inputSampleL += (pearD[figL]*pearD[levl]); inputSampleR += (pearD[figR]*pearD[levl]);
			inputSampleL += (pearE[figL]*pearE[levl]); inputSampleR += (pearE[figR]*pearE[levl]);
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
