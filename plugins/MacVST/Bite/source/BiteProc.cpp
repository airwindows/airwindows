/* ========================================
 *  Bite - Bite.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Bite_H
#include "Bite.h"
#endif

void Bite::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.3;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double gain = ((A*2.0)-1.0)*overallscale;
	double outputgain = B;
	double midA;
	double midB;
	double midC;
	double midD;
	double trigger;
	double inputSampleL;
	double inputSampleR;	
    
    while (--sampleFrames >= 0)
    {
		sampleIL = sampleHL;
		sampleHL = sampleGL;
		sampleGL = sampleFL;
		sampleFL = sampleEL;
		sampleEL = sampleDL;
		sampleDL = sampleCL;
		sampleCL = sampleBL;
		sampleBL = sampleAL;
		sampleAL = *in1;
		
		sampleIR = sampleHR;
		sampleHR = sampleGR;
		sampleGR = sampleFR;
		sampleFR = sampleER;
		sampleER = sampleDR;
		sampleDR = sampleCR;
		sampleCR = sampleBR;
		sampleBR = sampleAR;
		sampleAR = *in2;
		//rotate the buffer in primitive fashion
		if (fabs(sampleAL)<1.18e-23) sampleAL = fpdL * 1.18e-17;
		if (fabs(sampleAR)<1.18e-23) sampleAR = fpdR * 1.18e-17;

		midA = sampleAL - sampleEL;
		midB = sampleIL - sampleEL;
		midC = sampleCL - sampleEL;
		midD = sampleGL - sampleEL;
		midA *= ((((sampleBL + sampleCL + sampleDL)/3) - ((sampleAL + sampleEL)/2.0))*gain);
		midB *= ((((sampleFL + sampleGL + sampleHL)/3) - ((sampleEL + sampleIL)/2.0))*gain);		
		midC *= ((sampleDL - ((sampleCL + sampleEL)/2.0))*gain);
		midD *= ((sampleFL - ((sampleEL + sampleGL)/2.0))*gain);
		trigger = sin(midA + midB + midC + midD);		
		inputSampleL = sampleEL + (trigger*8.0);
		
		midA = sampleAR - sampleER;
		midB = sampleIR - sampleER;
		midC = sampleCR - sampleER;
		midD = sampleGR - sampleER;
		midA *= ((((sampleBR + sampleCR + sampleDR)/3) - ((sampleAR + sampleER)/2.0))*gain);
		midB *= ((((sampleFR + sampleGR + sampleHR)/3) - ((sampleER + sampleIR)/2.0))*gain);		
		midC *= ((sampleDR - ((sampleCR + sampleER)/2.0))*gain);
		midD *= ((sampleFR - ((sampleER + sampleGR)/2.0))*gain);
		trigger = sin(midA + midB + midC + midD);		
		inputSampleR = sampleER + (trigger*8.0);
		
		if (outputgain != 1.0) {
			inputSampleL *= outputgain;
			inputSampleR *= outputgain;
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

void Bite::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.3;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double gain = ((A*2.0)-1.0)*overallscale;
	double outputgain = B;
	double midA;
	double midB;
	double midC;
	double midD;
	double trigger;
	double inputSampleL;
	double inputSampleR;	

    while (--sampleFrames >= 0)
    {		
		sampleIL = sampleHL;
		sampleHL = sampleGL;
		sampleGL = sampleFL;
		sampleFL = sampleEL;
		sampleEL = sampleDL;
		sampleDL = sampleCL;
		sampleCL = sampleBL;
		sampleBL = sampleAL;
		sampleAL = *in1;

		sampleIR = sampleHR;
		sampleHR = sampleGR;
		sampleGR = sampleFR;
		sampleFR = sampleER;
		sampleER = sampleDR;
		sampleDR = sampleCR;
		sampleCR = sampleBR;
		sampleBR = sampleAR;
		sampleAR = *in2;
		//rotate the buffer in primitive fashion
		if (fabs(sampleAL)<1.18e-23) sampleAL = fpdL * 1.18e-17;
		if (fabs(sampleAR)<1.18e-23) sampleAR = fpdR * 1.18e-17;
		
		midA = sampleAL - sampleEL;
		midB = sampleIL - sampleEL;
		midC = sampleCL - sampleEL;
		midD = sampleGL - sampleEL;
		midA *= ((((sampleBL + sampleCL + sampleDL)/3) - ((sampleAL + sampleEL)/2.0))*gain);
		midB *= ((((sampleFL + sampleGL + sampleHL)/3) - ((sampleEL + sampleIL)/2.0))*gain);		
		midC *= ((sampleDL - ((sampleCL + sampleEL)/2.0))*gain);
		midD *= ((sampleFL - ((sampleEL + sampleGL)/2.0))*gain);
		trigger = sin(midA + midB + midC + midD);		
		inputSampleL = sampleEL + (trigger*8.0);
		
		midA = sampleAR - sampleER;
		midB = sampleIR - sampleER;
		midC = sampleCR - sampleER;
		midD = sampleGR - sampleER;
		midA *= ((((sampleBR + sampleCR + sampleDR)/3) - ((sampleAR + sampleER)/2.0))*gain);
		midB *= ((((sampleFR + sampleGR + sampleHR)/3) - ((sampleER + sampleIR)/2.0))*gain);		
		midC *= ((sampleDR - ((sampleCR + sampleER)/2.0))*gain);
		midD *= ((sampleFR - ((sampleER + sampleGR)/2.0))*gain);
		trigger = sin(midA + midB + midC + midD);		
		inputSampleR = sampleER + (trigger*8.0);
		
		if (outputgain != 1.0) {
			inputSampleL *= outputgain;
			inputSampleR *= outputgain;
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