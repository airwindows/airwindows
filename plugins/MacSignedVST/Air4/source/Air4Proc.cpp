/* ========================================
 *  Air4 - Air4.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Air4_H
#include "Air4.h"
#endif

void Air4::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double airGain = A*2.0;
	if (airGain > 1.0) airGain = pow(airGain,3.0+sqrt(overallscale));
	double gndGain = B*2.0;
	double threshSinew = pow(C,2)/overallscale;
	double depthSinew = D;

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		air[pvSL4] = air[pvAL4] - air[pvAL3];
		air[pvSL3] = air[pvAL3] - air[pvAL2];
		air[pvSL2] = air[pvAL2] - air[pvAL1];
		air[pvSL1] = air[pvAL1] - inputSampleL;
		
		air[accSL3] = air[pvSL4] - air[pvSL3];
		air[accSL2] = air[pvSL3] - air[pvSL2];
		air[accSL1] = air[pvSL2] - air[pvSL1];
		
		air[acc2SL2] = air[accSL3] - air[accSL2];
		air[acc2SL1] = air[accSL2] - air[accSL1];		
		
		air[outAL] = -(air[pvAL1] + air[pvSL3] + air[acc2SL2] - ((air[acc2SL2] + air[acc2SL1])*0.5));
		
		air[gainAL] *= 0.5; 
		air[gainAL] += fabs(drySampleL-air[outAL])*0.5;
		if (air[gainAL] > 0.3*sqrt(overallscale)) air[gainAL] = 0.3*sqrt(overallscale);
		air[pvAL4] = air[pvAL3];
		air[pvAL3] = air[pvAL2];
		air[pvAL2] = air[pvAL1];		
		air[pvAL1] = (air[gainAL] * air[outAL]) + drySampleL;
		
		double gnd = drySampleL - ((air[outAL]*0.5)+(drySampleL*(0.457-(0.017*overallscale))));
		double temp = (gnd + air[gndavgL])*0.5; air[gndavgL] = gnd; gnd = temp;
		inputSampleL = ((drySampleL-gnd)*airGain)+(gnd*gndGain);
		
		air[pvSR4] = air[pvAR4] - air[pvAR3];
		air[pvSR3] = air[pvAR3] - air[pvAR2];
		air[pvSR2] = air[pvAR2] - air[pvAR1];
		air[pvSR1] = air[pvAR1] - inputSampleR;
		
		air[accSR3] = air[pvSR4] - air[pvSR3];
		air[accSR2] = air[pvSR3] - air[pvSR2];
		air[accSR1] = air[pvSR2] - air[pvSR1];
		
		air[acc2SR2] = air[accSR3] - air[accSR2];
		air[acc2SR1] = air[accSR2] - air[accSR1];		
		
		air[outAR] = -(air[pvAR1] + air[pvSR3] + air[acc2SR2] - ((air[acc2SR2] + air[acc2SR1])*0.5));
		
		air[gainAR] *= 0.5; 
		air[gainAR] += fabs(drySampleR-air[outAR])*0.5;
		if (air[gainAR] > 0.3*sqrt(overallscale)) air[gainAR] = 0.3*sqrt(overallscale);
		air[pvAR4] = air[pvAR3];
		air[pvAR3] = air[pvAR2];
		air[pvAR2] = air[pvAR1];		
		air[pvAR1] = (air[gainAR] * air[outAR]) + drySampleR;
		
		gnd = drySampleR - ((air[outAR]*0.5)+(drySampleR*(0.457-(0.017*overallscale))));
		temp = (gnd + air[gndavgR])*0.5; air[gndavgR] = gnd; gnd = temp;
		inputSampleR = ((drySampleR-gnd)*airGain)+(gnd*gndGain);
		
		temp = inputSampleL; if (temp > 1.0) temp = 1.0; if (temp < -1.0) temp = -1.0;
		double sinew = threshSinew * cos(air[lastSL]*air[lastSL]);
		if (temp - air[lastSL] > sinew) temp = air[lastSL] + sinew;
		if (-(temp - air[lastSL]) > sinew) temp = air[lastSL] - sinew;
		air[lastSL] = temp;
		if (air[lastSL] > 1.0) air[lastSL] = 1.0; 
		if (air[lastSL] < -1.0) air[lastSL] = -1.0; 
		inputSampleL = (inputSampleL * (1.0-depthSinew))+(air[lastSL]*depthSinew);		
		temp = inputSampleR; if (temp > 1.0) temp = 1.0; if (temp < -1.0) temp = -1.0;
		sinew = threshSinew * cos(air[lastSR]*air[lastSR]);
		if (temp - air[lastSR] > sinew) temp = air[lastSR] + sinew;
		if (-(temp - air[lastSR]) > sinew) temp = air[lastSR] - sinew;
		air[lastSR] = temp;
		if (air[lastSR] > 1.0) air[lastSR] = 1.0; 
		if (air[lastSR] < -1.0) air[lastSR] = -1.0; 
		inputSampleR = (inputSampleR * (1.0-depthSinew))+(air[lastSR]*depthSinew);
		//run Sinew to stop excess slews, but run a dry/wet to allow a range of brights		
		
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

void Air4::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double airGain = A*2.0;
	if (airGain > 1.0) airGain = pow(airGain,3.0+sqrt(overallscale));
	double gndGain = B*2.0;
	double threshSinew = pow(C,2)/overallscale;
	double depthSinew = D;

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		air[pvSL4] = air[pvAL4] - air[pvAL3];
		air[pvSL3] = air[pvAL3] - air[pvAL2];
		air[pvSL2] = air[pvAL2] - air[pvAL1];
		air[pvSL1] = air[pvAL1] - inputSampleL;
		
		air[accSL3] = air[pvSL4] - air[pvSL3];
		air[accSL2] = air[pvSL3] - air[pvSL2];
		air[accSL1] = air[pvSL2] - air[pvSL1];
		
		air[acc2SL2] = air[accSL3] - air[accSL2];
		air[acc2SL1] = air[accSL2] - air[accSL1];		
		
		air[outAL] = -(air[pvAL1] + air[pvSL3] + air[acc2SL2] - ((air[acc2SL2] + air[acc2SL1])*0.5));
		
		air[gainAL] *= 0.5; 
		air[gainAL] += fabs(drySampleL-air[outAL])*0.5;
		if (air[gainAL] > 0.3*sqrt(overallscale)) air[gainAL] = 0.3*sqrt(overallscale);
		air[pvAL4] = air[pvAL3];
		air[pvAL3] = air[pvAL2];
		air[pvAL2] = air[pvAL1];		
		air[pvAL1] = (air[gainAL] * air[outAL]) + drySampleL;
		
		double gnd = drySampleL - ((air[outAL]*0.5)+(drySampleL*(0.457-(0.017*overallscale))));
		double temp = (gnd + air[gndavgL])*0.5; air[gndavgL] = gnd; gnd = temp;
		inputSampleL = ((drySampleL-gnd)*airGain)+(gnd*gndGain);
		
		air[pvSR4] = air[pvAR4] - air[pvAR3];
		air[pvSR3] = air[pvAR3] - air[pvAR2];
		air[pvSR2] = air[pvAR2] - air[pvAR1];
		air[pvSR1] = air[pvAR1] - inputSampleR;
		
		air[accSR3] = air[pvSR4] - air[pvSR3];
		air[accSR2] = air[pvSR3] - air[pvSR2];
		air[accSR1] = air[pvSR2] - air[pvSR1];
		
		air[acc2SR2] = air[accSR3] - air[accSR2];
		air[acc2SR1] = air[accSR2] - air[accSR1];		
		
		air[outAR] = -(air[pvAR1] + air[pvSR3] + air[acc2SR2] - ((air[acc2SR2] + air[acc2SR1])*0.5));
		
		air[gainAR] *= 0.5; 
		air[gainAR] += fabs(drySampleR-air[outAR])*0.5;
		if (air[gainAR] > 0.3*sqrt(overallscale)) air[gainAR] = 0.3*sqrt(overallscale);
		air[pvAR4] = air[pvAR3];
		air[pvAR3] = air[pvAR2];
		air[pvAR2] = air[pvAR1];		
		air[pvAR1] = (air[gainAR] * air[outAR]) + drySampleR;
		
		gnd = drySampleR - ((air[outAR]*0.5)+(drySampleR*(0.457-(0.017*overallscale))));
		temp = (gnd + air[gndavgR])*0.5; air[gndavgR] = gnd; gnd = temp;
		inputSampleR = ((drySampleR-gnd)*airGain)+(gnd*gndGain);
		
		temp = inputSampleL; if (temp > 1.0) temp = 1.0; if (temp < -1.0) temp = -1.0;
		double sinew = threshSinew * cos(air[lastSL]*air[lastSL]);
		if (temp - air[lastSL] > sinew) temp = air[lastSL] + sinew;
		if (-(temp - air[lastSL]) > sinew) temp = air[lastSL] - sinew;
		air[lastSL] = temp;
		if (air[lastSL] > 1.0) air[lastSL] = 1.0; 
		if (air[lastSL] < -1.0) air[lastSL] = -1.0; 
		inputSampleL = (inputSampleL * (1.0-depthSinew))+(air[lastSL]*depthSinew);		
		temp = inputSampleR; if (temp > 1.0) temp = 1.0; if (temp < -1.0) temp = -1.0;
		sinew = threshSinew * cos(air[lastSR]*air[lastSR]);
		if (temp - air[lastSR] > sinew) temp = air[lastSR] + sinew;
		if (-(temp - air[lastSR]) > sinew) temp = air[lastSR] - sinew;
		air[lastSR] = temp;
		if (air[lastSR] > 1.0) air[lastSR] = 1.0; 
		if (air[lastSR] < -1.0) air[lastSR] = -1.0; 
		inputSampleR = (inputSampleR * (1.0-depthSinew))+(air[lastSR]*depthSinew);
		//run Sinew to stop excess slews, but run a dry/wet to allow a range of brights		
		
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
