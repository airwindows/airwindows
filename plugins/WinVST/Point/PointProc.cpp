/* ========================================
 *  Point - Point.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Point_H
#include "Point.h"
#endif

void Point::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double gaintrim = pow(10.0,((A*24.0)-12.0)/20);
	double nibDiv = 1 / pow(C+0.2,7);
	nibDiv /= overallscale;
	double nobDiv;
	if (((B*2.0)-1.0) > 0) nobDiv = nibDiv / (1.001-((B*2.0)-1.0));
	else nobDiv = nibDiv * (1.001-pow(((B*2.0)-1.0)*0.75,2));
	double nibnobFactor = 0.0; //start with the fallthrough value, why not
	double absolute;
	
	double inputSampleL;
	double inputSampleR;
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		inputSampleL *= gaintrim;
		absolute = fabs(inputSampleL);
		if (fpFlip)
		{
			nibAL = nibAL + (absolute / nibDiv);
			nibAL = nibAL / (1 + (1/nibDiv));
			nobAL = nobAL + (absolute / nobDiv);
			nobAL = nobAL / (1 + (1/nobDiv));
			if (nobAL > 0)
			{
				nibnobFactor = nibAL / nobAL;
			}
		}
		else
		{
			nibBL = nibBL + (absolute / nibDiv);
			nibBL = nibBL / (1 + (1/nibDiv));
			nobBL = nobBL + (absolute / nobDiv);
			nobBL = nobBL / (1 + (1/nobDiv));
			if (nobBL > 0)
			{
				nibnobFactor = nibBL / nobBL;
			}		
		}
		inputSampleL *= nibnobFactor;
		
		
		inputSampleR *= gaintrim;
		absolute = fabs(inputSampleR);
		if (fpFlip)
		{
			nibAR = nibAR + (absolute / nibDiv);
			nibAR = nibAR / (1 + (1/nibDiv));
			nobAR = nobAR + (absolute / nobDiv);
			nobAR = nobAR / (1 + (1/nobDiv));
			if (nobAR > 0)
			{
				nibnobFactor = nibAR / nobAR;
			}
		}
		else
		{
			nibBR = nibBR + (absolute / nibDiv);
			nibBR = nibBR / (1 + (1/nibDiv));
			nobBR = nobBR + (absolute / nobDiv);
			nobBR = nobBR / (1 + (1/nobDiv));
			if (nobBR > 0)
			{
				nibnobFactor = nibBR / nobBR;
			}		
		}
		inputSampleR *= nibnobFactor;
		fpFlip = !fpFlip;
		
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

void Point::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double gaintrim = pow(10.0,((A*24.0)-12.0)/20);
	double nibDiv = 1 / pow(C+0.2,7);
	nibDiv /= overallscale;
	double nobDiv;
	if (((B*2.0)-1.0) > 0) nobDiv = nibDiv / (1.001-((B*2.0)-1.0));
	else nobDiv = nibDiv * (1.001-pow(((B*2.0)-1.0)*0.75,2));
	double nibnobFactor = 0.0; //start with the fallthrough value, why not
	double absolute;
	
	double inputSampleL;
	double inputSampleR;

    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= gaintrim;
		absolute = fabs(inputSampleL);
		if (fpFlip)
		{
			nibAL = nibAL + (absolute / nibDiv);
			nibAL = nibAL / (1 + (1/nibDiv));
			nobAL = nobAL + (absolute / nobDiv);
			nobAL = nobAL / (1 + (1/nobDiv));
			if (nobAL > 0)
			{
				nibnobFactor = nibAL / nobAL;
			}
		}
		else
		{
			nibBL = nibBL + (absolute / nibDiv);
			nibBL = nibBL / (1 + (1/nibDiv));
			nobBL = nobBL + (absolute / nobDiv);
			nobBL = nobBL / (1 + (1/nobDiv));
			if (nobBL > 0)
			{
				nibnobFactor = nibBL / nobBL;
			}		
		}
		inputSampleL *= nibnobFactor;
		
		
		inputSampleR *= gaintrim;
		absolute = fabs(inputSampleR);
		if (fpFlip)
		{
			nibAR = nibAR + (absolute / nibDiv);
			nibAR = nibAR / (1 + (1/nibDiv));
			nobAR = nobAR + (absolute / nobDiv);
			nobAR = nobAR / (1 + (1/nobDiv));
			if (nobAR > 0)
			{
				nibnobFactor = nibAR / nobAR;
			}
		}
		else
		{
			nibBR = nibBR + (absolute / nibDiv);
			nibBR = nibBR / (1 + (1/nibDiv));
			nobBR = nobBR + (absolute / nobDiv);
			nobBR = nobBR / (1 + (1/nobDiv));
			if (nobBR > 0)
			{
				nibnobFactor = nibBR / nobBR;
			}		
		}
		inputSampleR *= nibnobFactor;
		fpFlip = !fpFlip;
		
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