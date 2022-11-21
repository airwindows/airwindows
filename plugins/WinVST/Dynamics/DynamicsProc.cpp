/* ========================================
 *  Dynamics - Dynamics.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Dynamics_H
#include "Dynamics.h"
#endif

void Dynamics::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	//begin ButterComp
	double inputgain = (pow(A,5)*35)+1.0;
	double divisor = (pow(B,4) * 0.01)+0.0005;
	divisor /= overallscale;
	double remainder = divisor;
	divisor = 1.0 - divisor;
	//end ButterComp
	
	//begin Gate
	double onthreshold = (pow(C,3)/3)+0.00018;
	double offthreshold = onthreshold * 1.1;	
	double release = 0.028331119964586;
	double absmax = 220.9;
	//speed to be compensated w.r.t sample rate
	//end Gate
	
	double wet = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin compressor
		//begin L
		inputSampleL *= inputgain;
		double inputpos = inputSampleL + 1.0;		
		if (inputpos < 0.0) inputpos = 0.0;
		double outputpos = inputpos / 2.0;
		if (outputpos > 1.0) outputpos = 1.0;		
		inputpos *= inputpos;
		targetposL *= divisor;
		targetposL += (inputpos * remainder);
		double calcpos = 1.0/targetposL;
		
		double inputneg = -inputSampleL + 1.0;		
		if (inputneg < 0.0) inputneg = 0.0;
		double outputneg = inputneg / 2.0;
		if (outputneg > 1.0) outputneg = 1.0;		
		inputneg *= inputneg;
		targetnegL *= divisor;
		targetnegL += (inputneg * remainder);
		double calcneg = 1.0/targetnegL;
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		
		if (inputSampleL > 0)
		{ //working on pos
			if (true == flip)
			{
				controlAposL *= divisor;
				controlAposL += (calcpos*remainder);
			} else {
				controlBposL *= divisor;
				controlBposL += (calcpos*remainder);
			}	
		} else { //working on neg
			if (true == flip)
			{
				controlAnegL *= divisor;
				controlAnegL += (calcneg*remainder);
			} else {
				controlBnegL *= divisor;
				controlBnegL += (calcneg*remainder);
			}
		}
		//this causes each of the four to update only when active and in the correct 'flip'
		
		double totalmultiplier;
		if (true == flip) totalmultiplier = (controlAposL * outputpos) + (controlAnegL * outputneg);
		else totalmultiplier = (controlBposL * outputpos) + (controlBnegL * outputneg);
		//this combines the sides according to flip, blending relative to the input value
		inputSampleL *= totalmultiplier;
		inputSampleL /= inputgain;
		//end L
		
		//begin R
		inputSampleR *= inputgain;
		inputpos = inputSampleR + 1.0;		
		if (inputpos < 0.0) inputpos = 0.0;
		outputpos = inputpos / 2.0;
		if (outputpos > 1.0) outputpos = 1.0;		
		inputpos *= inputpos;
		targetposR *= divisor;
		targetposR += (inputpos * remainder);
		calcpos = 1.0/targetposR;
		
		inputneg = -inputSampleR + 1.0;		
		if (inputneg < 0.0) inputneg = 0.0;
		outputneg = inputneg / 2.0;
		if (outputneg > 1.0) outputneg = 1.0;		
		inputneg *= inputneg;
		targetnegR *= divisor;
		targetnegR += (inputneg * remainder);
		calcneg = 1.0/targetnegR;
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		
		if (inputSampleR > 0)
		{ //working on pos
			if (true == flip)
			{
				controlAposR *= divisor;
				controlAposR += (calcpos*remainder);
			} else {
				controlBposR *= divisor;
				controlBposR += (calcpos*remainder);
			}	
		} else { //working on neg
			if (true == flip)
			{
				controlAnegR *= divisor;
				controlAnegR += (calcneg*remainder);
			} else {
				controlBnegR *= divisor;
				controlBnegR += (calcneg*remainder);
			}
		}
		//this causes each of the four to update only when active and in the correct 'flip'
		
		if (true == flip) totalmultiplier = (controlAposR * outputpos) + (controlAnegR * outputneg);
		else totalmultiplier = (controlBposR * outputpos) + (controlBnegR * outputneg);
		//this combines the sides according to flip, blending relative to the input value
		
		inputSampleR *= totalmultiplier;
		inputSampleR /= inputgain;
		//end R
		flip = !flip;
		//end compressor
		
		
		
		//begin Gate
		if (drySampleL > 0.0)
		{
			if (WasNegativeL == true) ZeroCrossL = absmax * 0.3;
			WasNegativeL = false;
		} else {
			ZeroCrossL += 1; WasNegativeL = true;
		}
		
		if (drySampleR > 0.0)
		{
			if (WasNegativeR == true) ZeroCrossR = absmax * 0.3;
			WasNegativeR = false;
		} else {
			ZeroCrossR += 1; WasNegativeR = true;
		}
		
		if (ZeroCrossL > absmax) ZeroCrossL = absmax;
		if (ZeroCrossR > absmax) ZeroCrossR = absmax;
		
		if (gateL == 0.0)
		{
			//if gate is totally silent
			if (fabs(drySampleL) > onthreshold)
			{
				if (gaterollerL == 0.0) gaterollerL = ZeroCrossL;
				else gaterollerL -= release;
				// trigger from total silence only- if we're active then signal must clear offthreshold
			}
			else gaterollerL -= release;
		} else {
			//gate is not silent but closing
			if (fabs(drySampleL) > offthreshold)
			{
				if (gaterollerL < ZeroCrossL) gaterollerL = ZeroCrossL;
				else gaterollerL -= release;
				//always trigger if gate is over offthreshold, otherwise close anyway
			}
			else gaterollerL -= release;
		}
		
		if (gateR == 0.0)
		{
			//if gate is totally silent
			if (fabs(drySampleR) > onthreshold)
			{
				if (gaterollerR == 0.0) gaterollerR = ZeroCrossR;
				else gaterollerR -= release;
				// trigger from total silence only- if we're active then signal must clear offthreshold
			}
			else gaterollerR -= release;
		} else {
			//gate is not silent but closing
			if (fabs(drySampleR) > offthreshold)
			{
				if (gaterollerR < ZeroCrossR) gaterollerR = ZeroCrossR;
				else gaterollerR -= release;
				//always trigger if gate is over offthreshold, otherwise close anyway
			}
			else gaterollerR -= release;
		}
		
		if (gaterollerL < 0.0) gaterollerL = 0.0;
		if (gaterollerR < 0.0) gaterollerR = 0.0;
		
		if (gaterollerL < 1.0)
		{
			gateL = gaterollerL;
			double bridgerectifier = 1-cos(fabs(inputSampleL));			
			if (inputSampleL > 0) inputSampleL = (inputSampleL*gateL)+(bridgerectifier*(1.0-gateL));
			else inputSampleL = (inputSampleL*gateL)-(bridgerectifier*(1.0-gateL));
			if (gateL == 0.0) inputSampleL = 0.0;			
		} else gateL = 1.0;
		
		if (gaterollerR < 1.0)
		{
			gateR = gaterollerR;
			double bridgerectifier = 1-cos(fabs(inputSampleR));			
			if (inputSampleR > 0) inputSampleR = (inputSampleR*gateR)+(bridgerectifier*(1.0-gateR));
			else inputSampleR = (inputSampleR*gateR)-(bridgerectifier*(1.0-gateR));
			if (gateR == 0.0) inputSampleR = 0.0;			
		} else gateR = 1.0;
		//end Gate
		
		if (wet != 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		
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

void Dynamics::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	//begin ButterComp
	double inputgain = (pow(A,5)*35)+1.0;
	double divisor = (pow(B,4) * 0.01)+0.0005;
	divisor /= overallscale;
	double remainder = divisor;
	divisor = 1.0 - divisor;
	//end ButterComp
	
	//begin Gate
	double onthreshold = (pow(C,3)/3)+0.00018;
	double offthreshold = onthreshold * 1.1;	
	double release = 0.028331119964586;
	double absmax = 220.9;
	//speed to be compensated w.r.t sample rate
	//end Gate
	
	double wet = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin compressor
		//begin L
		inputSampleL *= inputgain;
		double inputpos = inputSampleL + 1.0;		
		if (inputpos < 0.0) inputpos = 0.0;
		double outputpos = inputpos / 2.0;
		if (outputpos > 1.0) outputpos = 1.0;		
		inputpos *= inputpos;
		targetposL *= divisor;
		targetposL += (inputpos * remainder);
		double calcpos = 1.0/targetposL;
		
		double inputneg = -inputSampleL + 1.0;		
		if (inputneg < 0.0) inputneg = 0.0;
		double outputneg = inputneg / 2.0;
		if (outputneg > 1.0) outputneg = 1.0;		
		inputneg *= inputneg;
		targetnegL *= divisor;
		targetnegL += (inputneg * remainder);
		double calcneg = 1.0/targetnegL;
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		
		if (inputSampleL > 0)
		{ //working on pos
			if (true == flip)
			{
				controlAposL *= divisor;
				controlAposL += (calcpos*remainder);
			} else {
				controlBposL *= divisor;
				controlBposL += (calcpos*remainder);
			}	
		} else { //working on neg
			if (true == flip)
			{
				controlAnegL *= divisor;
				controlAnegL += (calcneg*remainder);
			} else {
				controlBnegL *= divisor;
				controlBnegL += (calcneg*remainder);
			}
		}
		//this causes each of the four to update only when active and in the correct 'flip'
		
		double totalmultiplier;
		if (true == flip) totalmultiplier = (controlAposL * outputpos) + (controlAnegL * outputneg);
		else totalmultiplier = (controlBposL * outputpos) + (controlBnegL * outputneg);
		//this combines the sides according to flip, blending relative to the input value
		inputSampleL *= totalmultiplier;
		inputSampleL /= inputgain;
		//end L
		
		//begin R
		inputSampleR *= inputgain;
		inputpos = inputSampleR + 1.0;		
		if (inputpos < 0.0) inputpos = 0.0;
		outputpos = inputpos / 2.0;
		if (outputpos > 1.0) outputpos = 1.0;		
		inputpos *= inputpos;
		targetposR *= divisor;
		targetposR += (inputpos * remainder);
		calcpos = 1.0/targetposR;
		
		inputneg = -inputSampleR + 1.0;		
		if (inputneg < 0.0) inputneg = 0.0;
		outputneg = inputneg / 2.0;
		if (outputneg > 1.0) outputneg = 1.0;		
		inputneg *= inputneg;
		targetnegR *= divisor;
		targetnegR += (inputneg * remainder);
		calcneg = 1.0/targetnegR;
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		
		if (inputSampleR > 0)
		{ //working on pos
			if (true == flip)
			{
				controlAposR *= divisor;
				controlAposR += (calcpos*remainder);
			} else {
				controlBposR *= divisor;
				controlBposR += (calcpos*remainder);
			}	
		} else { //working on neg
			if (true == flip)
			{
				controlAnegR *= divisor;
				controlAnegR += (calcneg*remainder);
			} else {
				controlBnegR *= divisor;
				controlBnegR += (calcneg*remainder);
			}
		}
		//this causes each of the four to update only when active and in the correct 'flip'
		
		if (true == flip) totalmultiplier = (controlAposR * outputpos) + (controlAnegR * outputneg);
		else totalmultiplier = (controlBposR * outputpos) + (controlBnegR * outputneg);
		//this combines the sides according to flip, blending relative to the input value
		
		inputSampleR *= totalmultiplier;
		inputSampleR /= inputgain;
		//end R
		flip = !flip;
		//end compressor
		
		
		
		//begin Gate
		if (drySampleL > 0.0)
		{
			if (WasNegativeL == true) ZeroCrossL = absmax * 0.3;
			WasNegativeL = false;
		} else {
			ZeroCrossL += 1; WasNegativeL = true;
		}
		
		if (drySampleR > 0.0)
		{
			if (WasNegativeR == true) ZeroCrossR = absmax * 0.3;
			WasNegativeR = false;
		} else {
			ZeroCrossR += 1; WasNegativeR = true;
		}
		
		if (ZeroCrossL > absmax) ZeroCrossL = absmax;
		if (ZeroCrossR > absmax) ZeroCrossR = absmax;
		
		if (gateL == 0.0)
		{
			//if gate is totally silent
			if (fabs(drySampleL) > onthreshold)
			{
				if (gaterollerL == 0.0) gaterollerL = ZeroCrossL;
				else gaterollerL -= release;
				// trigger from total silence only- if we're active then signal must clear offthreshold
			}
			else gaterollerL -= release;
		} else {
			//gate is not silent but closing
			if (fabs(drySampleL) > offthreshold)
			{
				if (gaterollerL < ZeroCrossL) gaterollerL = ZeroCrossL;
				else gaterollerL -= release;
				//always trigger if gate is over offthreshold, otherwise close anyway
			}
			else gaterollerL -= release;
		}
		
		if (gateR == 0.0)
		{
			//if gate is totally silent
			if (fabs(drySampleR) > onthreshold)
			{
				if (gaterollerR == 0.0) gaterollerR = ZeroCrossR;
				else gaterollerR -= release;
				// trigger from total silence only- if we're active then signal must clear offthreshold
			}
			else gaterollerR -= release;
		} else {
			//gate is not silent but closing
			if (fabs(drySampleR) > offthreshold)
			{
				if (gaterollerR < ZeroCrossR) gaterollerR = ZeroCrossR;
				else gaterollerR -= release;
				//always trigger if gate is over offthreshold, otherwise close anyway
			}
			else gaterollerR -= release;
		}
		
		if (gaterollerL < 0.0) gaterollerL = 0.0;
		if (gaterollerR < 0.0) gaterollerR = 0.0;
		
		if (gaterollerL < 1.0)
		{
			gateL = gaterollerL;
			double bridgerectifier = 1-cos(fabs(inputSampleL));			
			if (inputSampleL > 0) inputSampleL = (inputSampleL*gateL)+(bridgerectifier*(1.0-gateL));
			else inputSampleL = (inputSampleL*gateL)-(bridgerectifier*(1.0-gateL));
			if (gateL == 0.0) inputSampleL = 0.0;			
		} else gateL = 1.0;
		
		if (gaterollerR < 1.0)
		{
			gateR = gaterollerR;
			double bridgerectifier = 1-cos(fabs(inputSampleR));			
			if (inputSampleR > 0) inputSampleR = (inputSampleR*gateR)+(bridgerectifier*(1.0-gateR));
			else inputSampleR = (inputSampleR*gateR)-(bridgerectifier*(1.0-gateR));
			if (gateR == 0.0) inputSampleR = 0.0;			
		} else gateR = 1.0;
		//end Gate
		
		if (wet != 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		
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
