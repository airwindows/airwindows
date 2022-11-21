/* ========================================
 *  Hype - Hype.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Hype_H
#include "Hype.h"
#endif

void Hype::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		if (cycleEnd == 4) {
			softL[8] = softL[7]; softL[7] = softL[6]; softL[6] = softL[5]; softL[5] = softL[4];
			softL[4] = softL[3]; softL[3] = softL[2]; softL[2] = softL[1]; softL[1] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[7]; softR[7] = softR[6]; softR[6] = softR[5]; softR[5] = softR[4];
			softR[4] = softR[3]; softR[3] = softR[2]; softR[2] = softR[1]; softR[1] = softR[0];
			softR[0] = inputSampleR;
		}
		if (cycleEnd == 2) {
			softL[8] = softL[6]; softL[6] = softL[4];
			softL[4] = softL[2]; softL[2] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[6]; softR[6] = softR[4];
			softR[4] = softR[2]; softR[2] = softR[0];
			softR[0] = inputSampleR;
		}
		if (cycleEnd == 1) {
			softL[8] = softL[4];
			softL[4] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[4];
			softR[4] = softR[0];
			softR[0] = inputSampleR;
		}
		softL[9] = ((softL[0]-softL[4])-(softL[4]-softL[8]));
		if (softL[9] < -1.57079633) softL[9] = -1.57079633;
		if (softL[9] > 1.57079633) softL[9] = 1.57079633;
		inputSampleL = softL[8]+(sin(softL[9])*0.61803398);
		softR[9] = ((softR[0]-softR[4])-(softR[4]-softR[8]));
		if (softR[9] < -1.57079633) softR[9] = -1.57079633;
		if (softR[9] > 1.57079633) softR[9] = 1.57079633;
		inputSampleR = softR[8]+(sin(softR[9])*0.61803398);
		//treble softening effect ended up being an aural exciter

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

void Hype::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (cycleEnd == 4) {
			softL[8] = softL[7]; softL[7] = softL[6]; softL[6] = softL[5]; softL[5] = softL[4];
			softL[4] = softL[3]; softL[3] = softL[2]; softL[2] = softL[1]; softL[1] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[7]; softR[7] = softR[6]; softR[6] = softR[5]; softR[5] = softR[4];
			softR[4] = softR[3]; softR[3] = softR[2]; softR[2] = softR[1]; softR[1] = softR[0];
			softR[0] = inputSampleR;
		}
		if (cycleEnd == 2) {
			softL[8] = softL[6]; softL[6] = softL[4];
			softL[4] = softL[2]; softL[2] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[6]; softR[6] = softR[4];
			softR[4] = softR[2]; softR[2] = softR[0];
			softR[0] = inputSampleR;
		}
		if (cycleEnd == 1) {
			softL[8] = softL[4];
			softL[4] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[4];
			softR[4] = softR[0];
			softR[0] = inputSampleR;
		}
		softL[9] = ((softL[0]-softL[4])-(softL[4]-softL[8]));
		if (softL[9] < -1.57079633) softL[9] = -1.57079633;
		if (softL[9] > 1.57079633) softL[9] = 1.57079633;
		inputSampleL = softL[8]+(sin(softL[9])*0.61803398);
		softR[9] = ((softR[0]-softR[4])-(softR[4]-softR[8]));
		if (softR[9] < -1.57079633) softR[9] = -1.57079633;
		if (softR[9] > 1.57079633) softR[9] = 1.57079633;
		inputSampleR = softR[8]+(sin(softR[9])*0.61803398);
		//treble softening effect ended up being an aural exciter
		
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
