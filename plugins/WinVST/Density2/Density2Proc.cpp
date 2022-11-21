/* ========================================
 *  Density2 - Density2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Density2_H
#include "Density2.h"
#endif

void Density2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double density = (A*5.0)-1.0;
	double out = fabs(density);
	while (out > 1.0) out = out - 1.0;
	density = density * fabs(density);
	double iirAmount = pow(B,3)/overallscale;
	double output = C;
	double wet = D;
	    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double halfwaySampleL = (inputSampleL + last1SampleL + ((-last2SampleL + last3SampleL) * 0.0414213562373095048801688)) / 2.0;
		double halfDrySampleL = halfwaySampleL;
		
		double halfwaySampleR = (inputSampleR + last1SampleR + ((-last2SampleR + last3SampleR) * 0.0414213562373095048801688)) / 2.0;
		double halfDrySampleR = halfwaySampleR;
		
		last3SampleL = last2SampleL; last2SampleL = last1SampleL; last1SampleL = inputSampleL;
		last3SampleR = last2SampleR; last2SampleR = last1SampleR; last1SampleR = inputSampleR;
		
		iirSampleBL = (iirSampleBL * (1.0 - iirAmount)) + (halfwaySampleL * iirAmount); halfwaySampleL -= iirSampleBL; //highpass section
		iirSampleBR = (iirSampleBR * (1.0 - iirAmount)) + (halfwaySampleR * iirAmount); halfwaySampleR -= iirSampleBR; //highpass section
		
		double bridgerectifier;

		double count = density;
		while (count > 1.0) {
			bridgerectifier = fabs(halfwaySampleL)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (halfwaySampleL > 0.0) halfwaySampleL = bridgerectifier;
			else halfwaySampleL = -bridgerectifier;
			count = count - 1.0;
		}
		bridgerectifier = fabs(halfwaySampleL)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		if (density > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier); //produce either boosted or starved version
		if (halfwaySampleL > 0) halfwaySampleL = (halfwaySampleL*(1.0-out))+(bridgerectifier*out);
		else halfwaySampleL = (halfwaySampleL*(1.0-out))-(bridgerectifier*out); //blend according to density control

		count = density;
		while (count > 1.0) {
			bridgerectifier = fabs(halfwaySampleR)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (halfwaySampleR > 0.0) halfwaySampleR = bridgerectifier;
			else halfwaySampleR = -bridgerectifier;
			count = count - 1.0;
		}
		bridgerectifier = fabs(halfwaySampleR)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		if (density > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier); //produce either boosted or starved version
		if (halfwaySampleR > 0) halfwaySampleR = (halfwaySampleR*(1.0-out))+(bridgerectifier*out);
		else halfwaySampleR = (halfwaySampleR*(1.0-out))-(bridgerectifier*out); //blend according to density control
		
		ataCL = halfwaySampleL - halfDrySampleL;
		ataAL *= 0.915965594177219015; ataBL *= 0.915965594177219015;
		ataBL += ataCL; ataAL -= ataCL; ataCL = ataBL;
		double halfDiffSampleL = ataCL * 0.915965594177219015;
		
		ataCR = halfwaySampleR - halfDrySampleR;
		ataAR *= 0.915965594177219015; ataBR *= 0.915965594177219015;
		ataBR += ataCR; ataAR -= ataCR; ataCR = ataBR;
		double halfDiffSampleR = ataCR * 0.915965594177219015;
		
		iirSampleAL = (iirSampleAL * (1.0 - iirAmount)) + (inputSampleL * iirAmount); inputSampleL -= iirSampleAL; //highpass section
		iirSampleAR = (iirSampleAR * (1.0 - iirAmount)) + (inputSampleR * iirAmount); inputSampleR -= iirSampleAR; //highpass section
		
		count = density;
		while (count > 1.0) {
			bridgerectifier = fabs(inputSampleL)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
			else inputSampleL = -bridgerectifier;
			count = count - 1.0;
		}
		bridgerectifier = fabs(inputSampleL)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		if (density > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier); //produce either boosted or starved version
		if (inputSampleL > 0) inputSampleL = (inputSampleL*(1-out))+(bridgerectifier*out);
		else inputSampleL = (inputSampleL*(1-out))-(bridgerectifier*out); //blend according to density control
		
		count = density;
		while (count > 1.0) {
			bridgerectifier = fabs(inputSampleR)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
			else inputSampleR = -bridgerectifier;
			count = count - 1.0;
		}
		bridgerectifier = fabs(inputSampleR)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		if (density > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier); //produce either boosted or starved version
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1-out))+(bridgerectifier*out);
		else inputSampleR = (inputSampleR*(1-out))-(bridgerectifier*out); //blend according to density control
		
		ataCL = inputSampleL - drySampleL;
		ataAL *= 0.915965594177219015; ataBL *= 0.915965594177219015;
		ataAL += ataCL; ataBL -= ataCL; ataCL = ataAL;
		double diffSampleL = ataCL * 0.915965594177219015; 
		
		ataCR = inputSampleR - drySampleR;
		ataAR *= 0.915965594177219015; ataBR *= 0.915965594177219015;
		ataAR += ataCR; ataBR -= ataCR; ataCR = ataAR;
		double diffSampleR = ataCR * 0.915965594177219015; 
		
		inputSampleL = drySampleL + ((diffSampleL + halfDiffSampleL + lastDiffSampleL) / 1.187);
		lastDiffSampleL = diffSampleL / 2.0;
		inputSampleL *= output;
		inputSampleL = (drySampleL*(1.0-wet))+(inputSampleL*wet);
		
		inputSampleR = drySampleR + ((diffSampleR + halfDiffSampleR + lastDiffSampleR) / 1.187);
		lastDiffSampleR = diffSampleR / 2.0;
		inputSampleR *= output;
		inputSampleR = (drySampleR*(1.0-wet))+(inputSampleR*wet);
		
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

void Density2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double density = (A*5.0)-1.0;
	double out = fabs(density);
	while (out > 1.0) out = out - 1.0;
	density = density * fabs(density);
	double iirAmount = pow(B,3)/overallscale;
	double output = C;
	double wet = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double halfwaySampleL = (inputSampleL + last1SampleL + ((-last2SampleL + last3SampleL) * 0.0414213562373095048801688)) / 2.0;
		double halfDrySampleL = halfwaySampleL;
		
		double halfwaySampleR = (inputSampleR + last1SampleR + ((-last2SampleR + last3SampleR) * 0.0414213562373095048801688)) / 2.0;
		double halfDrySampleR = halfwaySampleR;
		
		last3SampleL = last2SampleL; last2SampleL = last1SampleL; last1SampleL = inputSampleL;
		last3SampleR = last2SampleR; last2SampleR = last1SampleR; last1SampleR = inputSampleR;
		
		iirSampleBL = (iirSampleBL * (1.0 - iirAmount)) + (halfwaySampleL * iirAmount); halfwaySampleL -= iirSampleBL; //highpass section
		iirSampleBR = (iirSampleBR * (1.0 - iirAmount)) + (halfwaySampleR * iirAmount); halfwaySampleR -= iirSampleBR; //highpass section
		
		double bridgerectifier;
		
		double count = density;
		while (count > 1.0) {
			bridgerectifier = fabs(halfwaySampleL)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (halfwaySampleL > 0.0) halfwaySampleL = bridgerectifier;
			else halfwaySampleL = -bridgerectifier;
			count = count - 1.0;
		}
		bridgerectifier = fabs(halfwaySampleL)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		if (density > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier); //produce either boosted or starved version
		if (halfwaySampleL > 0) halfwaySampleL = (halfwaySampleL*(1.0-out))+(bridgerectifier*out);
		else halfwaySampleL = (halfwaySampleL*(1.0-out))-(bridgerectifier*out); //blend according to density control
		
		count = density;
		while (count > 1.0) {
			bridgerectifier = fabs(halfwaySampleR)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (halfwaySampleR > 0.0) halfwaySampleR = bridgerectifier;
			else halfwaySampleR = -bridgerectifier;
			count = count - 1.0;
		}
		bridgerectifier = fabs(halfwaySampleR)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		if (density > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier); //produce either boosted or starved version
		if (halfwaySampleR > 0) halfwaySampleR = (halfwaySampleR*(1.0-out))+(bridgerectifier*out);
		else halfwaySampleR = (halfwaySampleR*(1.0-out))-(bridgerectifier*out); //blend according to density control
		
		ataCL = halfwaySampleL - halfDrySampleL;
		ataAL *= 0.915965594177219015; ataBL *= 0.915965594177219015;
		ataBL += ataCL; ataAL -= ataCL; ataCL = ataBL;
		double halfDiffSampleL = ataCL * 0.915965594177219015;
		
		ataCR = halfwaySampleR - halfDrySampleR;
		ataAR *= 0.915965594177219015; ataBR *= 0.915965594177219015;
		ataBR += ataCR; ataAR -= ataCR; ataCR = ataBR;
		double halfDiffSampleR = ataCR * 0.915965594177219015;
		
		iirSampleAL = (iirSampleAL * (1.0 - iirAmount)) + (inputSampleL * iirAmount); inputSampleL -= iirSampleAL; //highpass section
		iirSampleAR = (iirSampleAR * (1.0 - iirAmount)) + (inputSampleR * iirAmount); inputSampleR -= iirSampleAR; //highpass section
		
		count = density;
		while (count > 1.0) {
			bridgerectifier = fabs(inputSampleL)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
			else inputSampleL = -bridgerectifier;
			count = count - 1.0;
		}
		bridgerectifier = fabs(inputSampleL)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		if (density > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier); //produce either boosted or starved version
		if (inputSampleL > 0) inputSampleL = (inputSampleL*(1-out))+(bridgerectifier*out);
		else inputSampleL = (inputSampleL*(1-out))-(bridgerectifier*out); //blend according to density control
		
		count = density;
		while (count > 1.0) {
			bridgerectifier = fabs(inputSampleR)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
			else inputSampleR = -bridgerectifier;
			count = count - 1.0;
		}
		bridgerectifier = fabs(inputSampleR)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		if (density > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier); //produce either boosted or starved version
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1-out))+(bridgerectifier*out);
		else inputSampleR = (inputSampleR*(1-out))-(bridgerectifier*out); //blend according to density control
		
		ataCL = inputSampleL - drySampleL;
		ataAL *= 0.915965594177219015; ataBL *= 0.915965594177219015;
		ataAL += ataCL; ataBL -= ataCL; ataCL = ataAL;
		double diffSampleL = ataCL * 0.915965594177219015; 
		
		ataCR = inputSampleR - drySampleR;
		ataAR *= 0.915965594177219015; ataBR *= 0.915965594177219015;
		ataAR += ataCR; ataBR -= ataCR; ataCR = ataAR;
		double diffSampleR = ataCR * 0.915965594177219015; 
		
		inputSampleL = drySampleL + ((diffSampleL + halfDiffSampleL + lastDiffSampleL) / 1.187);
		lastDiffSampleL = diffSampleL / 2.0;
		inputSampleL *= output;
		inputSampleL = (drySampleL*(1.0-wet))+(inputSampleL*wet);
		
		inputSampleR = drySampleR + ((diffSampleR + halfDiffSampleR + lastDiffSampleR) / 1.187);
		lastDiffSampleR = diffSampleR / 2.0;
		inputSampleR *= output;
		inputSampleR = (drySampleR*(1.0-wet))+(inputSampleR*wet);
		
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
