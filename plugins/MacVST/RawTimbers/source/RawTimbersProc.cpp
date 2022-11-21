/* ========================================
 *  RawTimbers - RawTimbers.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __RawTimbers_H
#include "RawTimbers.h"
#endif

void RawTimbers::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	int processing = (VstInt32)( A * 1.999 );
	bool highres = false;
	if (processing == 1) highres = true;
	float scaleFactor;
	if (highres) scaleFactor = 8388608.0;
	else scaleFactor = 32768.0;
	float derez = B;
	if (derez > 0.0) scaleFactor *= pow(1.0-derez,6);
	if (scaleFactor < 0.0001) scaleFactor = 0.0001;
	float outScale = scaleFactor;
	if (outScale < 8.0) outScale = 8.0;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		
		
		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		double outputSampleL;
		double outputSampleR;
		
		inputSampleL += 0.381966011250105;
		inputSampleR += 0.381966011250105;

		if ((lastSampleL+lastSampleL) >= (inputSampleL+lastSample2L)) outputSampleL = floor(lastSampleL);
		else outputSampleL = floor(lastSampleL+1.0); //round down or up based on whether it softens treble angles
		
		if ((lastSampleR+lastSampleR) >= (inputSampleR+lastSample2R)) outputSampleR = floor(lastSampleR);
		else outputSampleR = floor(lastSampleR+1.0); //round down or up based on whether it softens treble angles
		
		lastSample2L = lastSampleL;
		lastSampleL = inputSampleL; //we retain three samples in a row
		
		lastSample2R = lastSampleR;
		lastSampleR = inputSampleR; //we retain three samples in a row
				
		*out1 = outputSampleL / outScale;
		*out2 = outputSampleR / outScale;
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void RawTimbers::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	int processing = (VstInt32)( A * 1.999 );
	bool highres = false;
	if (processing == 1) highres = true;
	float scaleFactor;
	if (highres) scaleFactor = 8388608.0;
	else scaleFactor = 32768.0;
	float derez = B;
	if (derez > 0.0) scaleFactor *= pow(1.0-derez,6);
	if (scaleFactor < 0.0001) scaleFactor = 0.0001;
	float outScale = scaleFactor;
	if (outScale < 8.0) outScale = 8.0;
    
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;

		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		double outputSampleL;
		double outputSampleR;
		
		inputSampleL += 0.381966011250105;
		inputSampleR += 0.381966011250105;
		
		if ((lastSampleL+lastSampleL) >= (inputSampleL+lastSample2L)) outputSampleL = floor(lastSampleL);
		else outputSampleL = floor(lastSampleL+1.0); //round down or up based on whether it softens treble angles
		
		if ((lastSampleR+lastSampleR) >= (inputSampleR+lastSample2R)) outputSampleR = floor(lastSampleR);
		else outputSampleR = floor(lastSampleR+1.0); //round down or up based on whether it softens treble angles
		
		lastSample2L = lastSampleL;
		lastSampleL = inputSampleL; //we retain three samples in a row
		
		lastSample2R = lastSampleR;
		lastSampleR = inputSampleR; //we retain three samples in a row
		
		*out1 = outputSampleL / outScale;
		*out2 = outputSampleR / outScale;
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }	
}
