/* ========================================
 *  Acceleration - Acceleration.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Acceleration_H
#include "Acceleration.h"
#endif

void Acceleration::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double intensity = pow(A,3)*(32/overallscale);
	double wet = B;
	//removed extra dry variable
	
	double senseL;
	double smoothL;
	double senseR;
	double smoothR;
	double accumulatorSample;
	double drySampleL;
	double drySampleR;
	double inputSampleL;
	double inputSampleR;
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		s3L = s2L;
		s2L = s1L;
		s1L = inputSampleL;
		smoothL = (s3L + s2L + s1L) / 3.0;
		m1L = (s1L-s2L)*((s1L-s2L)/1.3);
		m2L = (s2L-s3L)*((s1L-s2L)/1.3);
		senseL = fabs(m1L-m2L);
		senseL = (intensity*intensity*senseL);
		o3L = o2L;
		o2L = o1L;
		o1L = senseL;
		if (o2L > senseL) senseL = o2L;
		if (o3L > senseL) senseL = o3L;
		//sense on the most intense
		
		s3R = s2R;
		s2R = s1R;
		s1R = inputSampleR;
		smoothR = (s3R + s2R + s1R) / 3.0;
		m1R = (s1R-s2R)*((s1R-s2R)/1.3);
		m2R = (s2R-s3R)*((s1R-s2R)/1.3);
		senseR = fabs(m1R-m2R);
		senseR = (intensity*intensity*senseR);
		o3R = o2R;
		o2R = o1R;
		o1R = senseR;
		if (o2R > senseR) senseR = o2R;
		if (o3R > senseR) senseR = o3R;
		//sense on the most intense
		
		if (senseL > 1.0) senseL = 1.0;
		if (senseR > 1.0) senseR = 1.0;
		
		inputSampleL *= (1.0-senseL);
		inputSampleR *= (1.0-senseR);
		
		inputSampleL += (smoothL*senseL);
		inputSampleR += (smoothR*senseR);
		
		senseL /= 2.0;
		senseR /= 2.0;
		
		accumulatorSample = (ataLastOutL*senseL)+(inputSampleL*(1.0-senseL));
		ataLastOutL = inputSampleL;
		inputSampleL = accumulatorSample;
		
		accumulatorSample = (ataLastOutR*senseR)+(inputSampleR*(1.0-senseR));
		ataLastOutR = inputSampleR;
		inputSampleR = accumulatorSample;		
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
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

void Acceleration::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double intensity = pow(A,3)*(32/overallscale);
	double wet = B;
	//removed extra dry variable
	
	double senseL;
	double smoothL;
	double senseR;
	double smoothR;
	double accumulatorSample;
	double drySampleL;
	double drySampleR;
	double inputSampleL;
	double inputSampleR;
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		s3L = s2L;
		s2L = s1L;
		s1L = inputSampleL;
		smoothL = (s3L + s2L + s1L) / 3.0;
		m1L = (s1L-s2L)*((s1L-s2L)/1.3);
		m2L = (s2L-s3L)*((s1L-s2L)/1.3);
		senseL = fabs(m1L-m2L);
		senseL = (intensity*intensity*senseL);
		o3L = o2L;
		o2L = o1L;
		o1L = senseL;
		if (o2L > senseL) senseL = o2L;
		if (o3L > senseL) senseL = o3L;
		//sense on the most intense
		
		s3R = s2R;
		s2R = s1R;
		s1R = inputSampleR;
		smoothR = (s3R + s2R + s1R) / 3.0;
		m1R = (s1R-s2R)*((s1R-s2R)/1.3);
		m2R = (s2R-s3R)*((s1R-s2R)/1.3);
		senseR = fabs(m1R-m2R);
		senseR = (intensity*intensity*senseR);
		o3R = o2R;
		o2R = o1R;
		o1R = senseR;
		if (o2R > senseR) senseR = o2R;
		if (o3R > senseR) senseR = o3R;
		//sense on the most intense
		
		if (senseL > 1.0) senseL = 1.0;
		if (senseR > 1.0) senseR = 1.0;
		
		inputSampleL *= (1.0-senseL);
		inputSampleR *= (1.0-senseR);
		
		inputSampleL += (smoothL*senseL);
		inputSampleR += (smoothR*senseR);
		
		senseL /= 2.0;
		senseR /= 2.0;
		
		accumulatorSample = (ataLastOutL*senseL)+(inputSampleL*(1.0-senseL));
		ataLastOutL = inputSampleL;
		inputSampleL = accumulatorSample;
		
		accumulatorSample = (ataLastOutR*senseR)+(inputSampleR*(1.0-senseR));
		ataLastOutR = inputSampleR;
		inputSampleR = accumulatorSample;		
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
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