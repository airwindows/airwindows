/* ========================================
 *  TexturizeMS - TexturizeMS.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TexturizeMS_H
#include "TexturizeMS.h"
#endif

void TexturizeMS::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double slewAmountM = ((pow(A,2.0)*4.0)+0.71)/overallscale;
	double dynAmountM = pow(B,2.0);
	double wetM = pow(C,5);
	
	double slewAmountS = ((pow(D,2.0)*4.0)+0.71)/overallscale;
	double dynAmountS = pow(E,2.0);
	double wetS = pow(F,5);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double mid = inputSampleL + inputSampleR;
		double side = inputSampleL - inputSampleR;
		//assign mid and side.Between these sections, you can do mid/side processing
		double drySampleM = mid;
		double drySampleS = side;
		
		//begin mid
		if (mid < 0) {
			if (polarityM == true) {
				if (noiseAM < 0) flipM = true;
				else flipM = false;
			}
			polarityM = false;
		} else polarityM = true;		
		
		if (flipM) noiseAM += (double(fpdL)/UINT32_MAX);
		else noiseAM -= (double(fpdL)/UINT32_MAX);
		//here's the guts of the random walk		
		flipM = !flipM;
		
		if (mid > 1.0) mid = 1.0; if (mid < -1.0) mid = -1.0;
		if (dynAmountM < 0.4999) mid = (mid*dynAmountM*2.0) + (sin(mid)*(1.0-(dynAmountM*2.0)));		
		if (dynAmountM > 0.5001) mid = (asin(mid)*((dynAmountM*2.0)-1.0)) + (mid*(1.0-((dynAmountM*2.0)-1.0)));
		//doing this in two steps means I get to not run an extra sin/asin function per sample
		
		noiseBM = sin(noiseAM*(0.2-(dynAmountM*0.125))*fabs(mid));
		
		double slew = fabs(mid-lastSampleM)*slewAmountM;
		lastSampleM = mid*(0.86-(dynAmountM*0.125));
		
		if (slew > 1.0) slew = 1.0;
		double iirIntensity = slew;
		iirIntensity *= 2.472;
		iirIntensity *= iirIntensity;
		if (iirIntensity > 1.0) iirIntensity = 1.0;
		
		iirSampleM = (iirSampleM * (1.0 - iirIntensity)) + (noiseBM * iirIntensity);
		noiseBM = iirSampleM;
		noiseBM = (noiseBM * slew) + (noiseCM * (1.0-slew));
		noiseCM = noiseBM;
		
		mid = (noiseCM * wetM) + (drySampleM * (1.0-wetM));
		//end mid
		
		//begin side
		if (side < 0) {
			if (polarityS == true) {
				if (noiseAS < 0) flipS = true;
				else flipS = false;
			}
			polarityS = false;
		} else polarityS = true;		
		
		if (flipS) noiseAS += (double(fpdR)/UINT32_MAX);
		else noiseAS -= (double(fpdR)/UINT32_MAX);
		//here's the guts of the random walk		
		flipS = !flipS;
		
		if (side > 1.0) side = 1.0; if (side < -1.0) side = -1.0;
		if (dynAmountS < 0.4999) side = (side*dynAmountS*2.0) + (sin(side)*(1.0-(dynAmountS*2.0)));		
		if (dynAmountS > 0.5001) side = (asin(side)*((dynAmountS*2.0)-1.0)) + (side*(1.0-((dynAmountS*2.0)-1.0)));
		//doing this in two steps means I get to not run an extra sin/asin function per sample
		
		noiseBS = sin(noiseAS*(0.2-(dynAmountS*0.125))*fabs(side));
		
		slew = fabs(side-lastSampleS)*slewAmountS;
		lastSampleS = side*(0.86-(dynAmountS*0.125));
		
		if (slew > 1.0) slew = 1.0;
		iirIntensity = slew;
		iirIntensity *= 2.472;
		iirIntensity *= iirIntensity;
		if (iirIntensity > 1.0) iirIntensity = 1.0;
		
		iirSampleS = (iirSampleS * (1.0 - iirIntensity)) + (noiseBS * iirIntensity);
		noiseBS = iirSampleS;
		noiseBS = (noiseBS * slew) + (noiseCS * (1.0-slew));
		noiseCS = noiseBS;
		
		side = (noiseCS * wetS) + (drySampleS * (1.0-wetS));
		//end side
		
		inputSampleL = (mid+side)/2.0;
		inputSampleR = (mid-side)/2.0;
		//unassign mid and side
		
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

void TexturizeMS::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double slewAmountM = ((pow(A,2.0)*4.0)+0.71)/overallscale;
	double dynAmountM = pow(B,2.0);
	double wetM = pow(C,5);
	
	double slewAmountS = ((pow(D,2.0)*4.0)+0.71)/overallscale;
	double dynAmountS = pow(E,2.0);
	double wetS = pow(F,5);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double mid = inputSampleL + inputSampleR;
		double side = inputSampleL - inputSampleR;
		//assign mid and side.Between these sections, you can do mid/side processing
		double drySampleM = mid;
		double drySampleS = side;
		
		//begin mid
		if (mid < 0) {
			if (polarityM == true) {
				if (noiseAM < 0) flipM = true;
				else flipM = false;
			}
			polarityM = false;
		} else polarityM = true;		
		
		if (flipM) noiseAM += (double(fpdL)/UINT32_MAX);
		else noiseAM -= (double(fpdL)/UINT32_MAX);
		//here's the guts of the random walk		
		flipM = !flipM;
		
		if (mid > 1.0) mid = 1.0; if (mid < -1.0) mid = -1.0;
		if (dynAmountM < 0.4999) mid = (mid*dynAmountM*2.0) + (sin(mid)*(1.0-(dynAmountM*2.0)));		
		if (dynAmountM > 0.5001) mid = (asin(mid)*((dynAmountM*2.0)-1.0)) + (mid*(1.0-((dynAmountM*2.0)-1.0)));
		//doing this in two steps means I get to not run an extra sin/asin function per sample
		
		noiseBM = sin(noiseAM*(0.2-(dynAmountM*0.125))*fabs(mid));
		
		double slew = fabs(mid-lastSampleM)*slewAmountM;
		lastSampleM = mid*(0.86-(dynAmountM*0.125));
		
		if (slew > 1.0) slew = 1.0;
		double iirIntensity = slew;
		iirIntensity *= 2.472;
		iirIntensity *= iirIntensity;
		if (iirIntensity > 1.0) iirIntensity = 1.0;
		
		iirSampleM = (iirSampleM * (1.0 - iirIntensity)) + (noiseBM * iirIntensity);
		noiseBM = iirSampleM;
		noiseBM = (noiseBM * slew) + (noiseCM * (1.0-slew));
		noiseCM = noiseBM;
		
		mid = (noiseCM * wetM) + (drySampleM * (1.0-wetM));
		//end mid
		
		//begin side
		if (side < 0) {
			if (polarityS == true) {
				if (noiseAS < 0) flipS = true;
				else flipS = false;
			}
			polarityS = false;
		} else polarityS = true;		
		
		if (flipS) noiseAS += (double(fpdR)/UINT32_MAX);
		else noiseAS -= (double(fpdR)/UINT32_MAX);
		//here's the guts of the random walk		
		flipS = !flipS;
		
		if (side > 1.0) side = 1.0; if (side < -1.0) side = -1.0;
		if (dynAmountS < 0.4999) side = (side*dynAmountS*2.0) + (sin(side)*(1.0-(dynAmountS*2.0)));		
		if (dynAmountS > 0.5001) side = (asin(side)*((dynAmountS*2.0)-1.0)) + (side*(1.0-((dynAmountS*2.0)-1.0)));
		//doing this in two steps means I get to not run an extra sin/asin function per sample
		
		noiseBS = sin(noiseAS*(0.2-(dynAmountS*0.125))*fabs(side));
		
		slew = fabs(side-lastSampleS)*slewAmountS;
		lastSampleS = side*(0.86-(dynAmountS*0.125));
		
		if (slew > 1.0) slew = 1.0;
		iirIntensity = slew;
		iirIntensity *= 2.472;
		iirIntensity *= iirIntensity;
		if (iirIntensity > 1.0) iirIntensity = 1.0;
		
		iirSampleS = (iirSampleS * (1.0 - iirIntensity)) + (noiseBS * iirIntensity);
		noiseBS = iirSampleS;
		noiseBS = (noiseBS * slew) + (noiseCS * (1.0-slew));
		noiseCS = noiseBS;
		
		side = (noiseCS * wetS) + (drySampleS * (1.0-wetS));
		//end side
		
		inputSampleL = (mid+side)/2.0;
		inputSampleR = (mid-side)/2.0;
		//unassign mid and side
		
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
