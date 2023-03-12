/* ========================================
 *  OrbitKick - OrbitKick.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __OrbitKick_H
#include "OrbitKick.h"
#endif

void OrbitKick::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double drop = 1.0+(A*(0.001/overallscale)); //more is briefer bass
	double zone = B*0.01; //the max exponentiality of the falloff
	double start = C; //higher attack
	double envelope = 9.0-((1.0-pow(1.0-D,2))*4.0); //higher is allowing more subs before gate
	envelope *= ((start*0.4)+0.6);
	double threshold = pow(E,3); // trigger threshold
	double wet = F*2.0;
	double dry = 2.0 - wet;
	if (wet > 1.0) wet = 1.0;
	if (dry > 1.0) dry = 1.0;

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if ((inputSampleL > speed*start*2.0) && (inputSampleL > threshold)) speed = inputSampleL*start;
		if ((inputSampleR > speed*start*2.0) && (inputSampleR > threshold)) speed = inputSampleR*start;
		position += (speed*start);
		speed /= (drop+(speed*zone*start));
		if (position > 31415.92653589793) position -= 31415.92653589793;		
		orbit += (cos(position)*0.001); orbit *= 0.998272;
		double applySpeed = cbrt(speed) * envelope;
		if (applySpeed < 1.0) orbit *= applySpeed;
		inputSampleL = inputSampleR = orbit * 2.0;
		
		inputSampleL = (inputSampleL * wet) + (drySampleL * dry);
		inputSampleR = (inputSampleR * wet) + (drySampleR * dry);
		
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

void OrbitKick::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double drop = 1.0+(A*(0.001/overallscale)); //more is briefer bass
	double zone = B*0.01; //the max exponentiality of the falloff
	double start = C; //higher attack
	double envelope = 9.0-((1.0-pow(1.0-D,2))*4.0); //higher is allowing more subs before gate
	envelope *= ((start*0.4)+0.6);
	double threshold = pow(E,3); // trigger threshold
	double wet = F*2.0;
	double dry = 2.0 - wet;
	if (wet > 1.0) wet = 1.0;
	if (dry > 1.0) dry = 1.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if ((inputSampleL > speed*start*2.0) && (inputSampleL > threshold)) speed = inputSampleL*start;
		if ((inputSampleR > speed*start*2.0) && (inputSampleR > threshold)) speed = inputSampleR*start;
		position += (speed*start);
		speed /= (drop+(speed*zone*start));
		if (position > 31415.92653589793) position -= 31415.92653589793;		
		orbit += (cos(position)*0.001); orbit *= 0.998272;
		double applySpeed = cbrt(speed) * envelope;
		if (applySpeed < 1.0) orbit *= applySpeed;
		inputSampleL = inputSampleR = orbit * 2.0;
		
		inputSampleL = (inputSampleL * wet) + (drySampleL * dry);
		inputSampleR = (inputSampleR * wet) + (drySampleR * dry);
		
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
