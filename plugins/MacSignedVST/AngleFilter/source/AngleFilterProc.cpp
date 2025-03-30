/* ========================================
 *  AngleFilter - AngleFilter.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __AngleFilter_H
#include "AngleFilter.h"
#endif

void AngleFilter::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	primaryA = primaryB; primaryB = pow(A,overallscale+2.0);
	secondaryA = secondaryB; secondaryB = B;
	tertiaryA = tertiaryB; tertiaryB = fmin(primaryB+C,1.0);
	double poles = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double primary = (primaryA*temp)+(primaryB*(1.0-temp));
		double secondary = (secondaryA*temp)+(secondaryB*(1.0-temp));
		double tertiary = (tertiaryA*temp)+(tertiaryB*(1.0-temp));
		
		for(int count = 0; count < poles*32; count++) {
			double FT = fmax(primary+(fabs(inputSampleL)*(secondary-0.5)),0.0);
			double FR = (FT*(1.0-tertiary))+tertiary;
			iirAngleL[count] = (iirAngleL[count]*(1.0-FT))+((inputSampleL-iirPositionL[count])*FT);
			inputSampleL = ((iirPositionL[count]+(iirAngleL[count]*FT*FR))*(1.0-FT))+(inputSampleL*FT);
			iirPositionL[count] = ((iirPositionL[count]+(iirAngleL[count]*FT*FR))*(1.0-FT))+(inputSampleL*FT);
			
			FT = fmax(primary+(fabs(inputSampleR)*(secondary-0.5)),0.0);
			FR = (FT*(1.0-tertiary))+tertiary;
			iirAngleR[count] = (iirAngleR[count]*(1.0-FT))+((inputSampleR-iirPositionR[count])*FT);
			inputSampleR = ((iirPositionR[count]+(iirAngleR[count]*FT*FR))*(1.0-FT))+(inputSampleR*FT);
			iirPositionR[count] = ((iirPositionR[count]+(iirAngleR[count]*FT*FR))*(1.0-FT))+(inputSampleR*FT);
		}
		inputSampleL = sin(inputSampleL);
		inputSampleR = sin(inputSampleR); //straight up just wavefold it
		
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

void AngleFilter::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	primaryA = primaryB; primaryB = pow(A,overallscale+2.0);
	secondaryA = secondaryB; secondaryB = B;
	tertiaryA = tertiaryB; tertiaryB = fmin(primaryB+C,1.0);
	double poles = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double primary = (primaryA*temp)+(primaryB*(1.0-temp));
		double secondary = (secondaryA*temp)+(secondaryB*(1.0-temp));
		double tertiary = (tertiaryA*temp)+(tertiaryB*(1.0-temp));
		
		for(int count = 0; count < poles*32; count++) {
			double FT = fmax(primary+(fabs(inputSampleL)*(secondary-0.5)),0.0);
			double FR = (FT*(1.0-tertiary))+tertiary;
			iirAngleL[count] = (iirAngleL[count]*(1.0-FT))+((inputSampleL-iirPositionL[count])*FT);
			inputSampleL = ((iirPositionL[count]+(iirAngleL[count]*FT*FR))*(1.0-FT))+(inputSampleL*FT);
			iirPositionL[count] = ((iirPositionL[count]+(iirAngleL[count]*FT*FR))*(1.0-FT))+(inputSampleL*FT);
			
			FT = fmax(primary+(fabs(inputSampleR)*(secondary-0.5)),0.0);
			FR = (FT*(1.0-tertiary))+tertiary;
			iirAngleR[count] = (iirAngleR[count]*(1.0-FT))+((inputSampleR-iirPositionR[count])*FT);
			inputSampleR = ((iirPositionR[count]+(iirAngleR[count]*FT*FR))*(1.0-FT))+(inputSampleR*FT);
			iirPositionR[count] = ((iirPositionR[count]+(iirAngleR[count]*FT*FR))*(1.0-FT))+(inputSampleR*FT);
		}
		inputSampleL = sin(inputSampleL);
		inputSampleR = sin(inputSampleR); //straight up just wavefold it
		
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
