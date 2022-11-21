/* ========================================
 *  Texturize - Texturize.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Gain_H
#include "Texturize.h"
#endif

void Texturize::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double slewAmount = ((pow(A,2.0)*4.0)+0.71)/overallscale;
	double dynAmount = pow(B,2.0);
	double wet = pow(C,5);
	    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (inputSampleL < 0) {
			if (polarityL == true) {
				if (noiseAL < 0) flipL = true;
				else flipL = false;
			}
			polarityL = false;
		} else polarityL = true;		
		
		if (inputSampleR < 0) {
			if (polarityR == true) {
				if (noiseAR < 0) flipR = true;
				else flipR = false;
			}
			polarityR = false;
		} else polarityR = true;		
		
		if (flipL) noiseAL += (double(fpdL)/UINT32_MAX);
		else noiseAL -= (double(fpdL)/UINT32_MAX);
		//here's the guts of the random walk		
		flipL = !flipL;

		if (flipR) noiseAR += (double(fpdR)/UINT32_MAX);
		else noiseAR -= (double(fpdR)/UINT32_MAX);
		//here's the guts of the random walk		
		flipR = !flipR;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;

		if (dynAmount < 0.4999) {
			inputSampleL = (inputSampleL*dynAmount*2.0) + (sin(inputSampleL)*(1.0-(dynAmount*2.0)));		
			inputSampleR = (inputSampleR*dynAmount*2.0) + (sin(inputSampleR)*(1.0-(dynAmount*2.0)));		
		}
		if (dynAmount > 0.5001) {
			inputSampleL = (asin(inputSampleL)*((dynAmount*2.0)-1.0)) + (inputSampleL*(1.0-((dynAmount*2.0)-1.0)));
			inputSampleR = (asin(inputSampleR)*((dynAmount*2.0)-1.0)) + (inputSampleR*(1.0-((dynAmount*2.0)-1.0)));
		}
		//doing this in two steps means I get to not run an extra sin/asin function per sample
		
		noiseBL = sin(noiseAL*(0.2-(dynAmount*0.125))*fabs(inputSampleL));
		noiseBR = sin(noiseAR*(0.2-(dynAmount*0.125))*fabs(inputSampleR));
		
		double slewL = fabs(inputSampleL-lastSampleL)*slewAmount;
		lastSampleL = inputSampleL*(0.86-(dynAmount*0.125));
		if (slewL > 1.0) slewL = 1.0;
		double iirIntensityL = slewL;
		iirIntensityL *= 2.472;
		iirIntensityL *= iirIntensityL;
		if (iirIntensityL > 1.0) iirIntensityL = 1.0;
		
		double slewR = fabs(inputSampleR-lastSampleR)*slewAmount;
		lastSampleR = inputSampleR*(0.86-(dynAmount*0.125));
		if (slewR > 1.0) slewR = 1.0;
		double iirIntensityR = slewR;
		iirIntensityR *= 2.472;
		iirIntensityR *= iirIntensityR;
		if (iirIntensityR > 1.0) iirIntensityR = 1.0;
		
		iirSampleL = (iirSampleL * (1.0 - iirIntensityL)) + (noiseBL * iirIntensityL);
		noiseBL = iirSampleL;
		noiseBL = (noiseBL * slewL) + (noiseCL * (1.0-slewL));
		noiseCL = noiseBL;
		
		iirSampleR = (iirSampleR * (1.0 - iirIntensityR)) + (noiseBR * iirIntensityR);
		noiseBR = iirSampleR;
		noiseBR = (noiseBR * slewR) + (noiseCR * (1.0-slewR));
		noiseCR = noiseBR;
		
		inputSampleL = (noiseCL * wet) + (drySampleL * (1.0-wet));
		inputSampleR = (noiseCR * wet) + (drySampleR * (1.0-wet));
		
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

void Texturize::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double slewAmount = ((pow(A,2.0)*4.0)+0.71)/overallscale;
	double dynAmount = pow(B,2.0);
	double wet = pow(C,5);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (inputSampleL < 0) {
			if (polarityL == true) {
				if (noiseAL < 0) flipL = true;
				else flipL = false;
			}
			polarityL = false;
		} else polarityL = true;		
		
		if (inputSampleR < 0) {
			if (polarityR == true) {
				if (noiseAR < 0) flipR = true;
				else flipR = false;
			}
			polarityR = false;
		} else polarityR = true;		
		
		if (flipL) noiseAL += (double(fpdL)/UINT32_MAX);
		else noiseAL -= (double(fpdL)/UINT32_MAX);
		//here's the guts of the random walk		
		flipL = !flipL;
		
		if (flipR) noiseAR += (double(fpdR)/UINT32_MAX);
		else noiseAR -= (double(fpdR)/UINT32_MAX);
		//here's the guts of the random walk		
		flipR = !flipR;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		
		if (dynAmount < 0.4999) {
			inputSampleL = (inputSampleL*dynAmount*2.0) + (sin(inputSampleL)*(1.0-(dynAmount*2.0)));		
			inputSampleR = (inputSampleR*dynAmount*2.0) + (sin(inputSampleR)*(1.0-(dynAmount*2.0)));		
		}
		if (dynAmount > 0.5001) {
			inputSampleL = (asin(inputSampleL)*((dynAmount*2.0)-1.0)) + (inputSampleL*(1.0-((dynAmount*2.0)-1.0)));
			inputSampleR = (asin(inputSampleR)*((dynAmount*2.0)-1.0)) + (inputSampleR*(1.0-((dynAmount*2.0)-1.0)));
		}
		//doing this in two steps means I get to not run an extra sin/asin function per sample
		
		noiseBL = sin(noiseAL*(0.2-(dynAmount*0.125))*fabs(inputSampleL));
		noiseBR = sin(noiseAR*(0.2-(dynAmount*0.125))*fabs(inputSampleR));
		
		double slewL = fabs(inputSampleL-lastSampleL)*slewAmount;
		lastSampleL = inputSampleL*(0.86-(dynAmount*0.125));
		if (slewL > 1.0) slewL = 1.0;
		double iirIntensityL = slewL;
		iirIntensityL *= 2.472;
		iirIntensityL *= iirIntensityL;
		if (iirIntensityL > 1.0) iirIntensityL = 1.0;
		
		double slewR = fabs(inputSampleR-lastSampleR)*slewAmount;
		lastSampleR = inputSampleR*(0.86-(dynAmount*0.125));
		if (slewR > 1.0) slewR = 1.0;
		double iirIntensityR = slewR;
		iirIntensityR *= 2.472;
		iirIntensityR *= iirIntensityR;
		if (iirIntensityR > 1.0) iirIntensityR = 1.0;
		
		iirSampleL = (iirSampleL * (1.0 - iirIntensityL)) + (noiseBL * iirIntensityL);
		noiseBL = iirSampleL;
		noiseBL = (noiseBL * slewL) + (noiseCL * (1.0-slewL));
		noiseCL = noiseBL;
		
		iirSampleR = (iirSampleR * (1.0 - iirIntensityR)) + (noiseBR * iirIntensityR);
		noiseBR = iirSampleR;
		noiseBR = (noiseBR * slewR) + (noiseCR * (1.0-slewR));
		noiseCR = noiseBR;
		
		inputSampleL = (noiseCL * wet) + (drySampleL * (1.0-wet));
		inputSampleR = (noiseCR * wet) + (drySampleR * (1.0-wet));
		
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
