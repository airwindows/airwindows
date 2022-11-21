/* ========================================
 *  Air - Air.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Air_H
#include "Air.h"
#endif

void Air::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double hiIntensity = -pow(((A*2.0)-1.0),3)*2;
	double tripletintensity = -pow(((B*2.0)-1.0),3);
	double airIntensity = -pow(((C*2.0)-1.0),3)/2;
	double filterQ = 2.1-D;
	double output = E;
	double wet = F;
	double dry = 1.0-wet;
	

	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	double correctionL;
	double correctionR;
    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		correctionL = 0.0;
		correctionR = 0.0;  //from here on down, please add L and R to the code
		
		if (count < 1 || count > 3) count = 1;
		tripletFactorL = tripletPrevL - inputSampleL;
		tripletFactorR = tripletPrevR - inputSampleR;
		switch (count)
		{
			case 1:
				tripletAL += tripletFactorL;
				tripletCL -= tripletFactorL;
				tripletFactorL = tripletAL * tripletintensity;
				tripletPrevL = tripletMidL;
				tripletMidL = inputSampleL;
				
				tripletAR += tripletFactorR;
				tripletCR -= tripletFactorR;
				tripletFactorR = tripletAR * tripletintensity;
				tripletPrevR = tripletMidR;
				tripletMidR = inputSampleR;
				break;
			case 2:
				tripletBL += tripletFactorL;
				tripletAL -= tripletFactorL;
				tripletFactorL = tripletBL * tripletintensity;
				tripletPrevL = tripletMidL;
				tripletMidL = inputSampleL;
				
				tripletBR += tripletFactorR;
				tripletAR -= tripletFactorR;
				tripletFactorR = tripletBR * tripletintensity;
				tripletPrevR = tripletMidR;
				tripletMidR = inputSampleR;
				break;
			case 3:
				tripletCL += tripletFactorL;
				tripletBL -= tripletFactorL;
				tripletFactorL = tripletCL * tripletintensity;
				tripletPrevL = tripletMidL;
				tripletMidL = inputSampleL;
				
				tripletCR += tripletFactorR;
				tripletBR -= tripletFactorR;
				tripletFactorR = tripletCR * tripletintensity;
				tripletPrevR = tripletMidR;
				tripletMidR = inputSampleR;
				break;
		}
		tripletAL /= filterQ;
		tripletBL /= filterQ;
		tripletCL /= filterQ;
		correctionL = correctionL + tripletFactorL;

		tripletAR /= filterQ;
		tripletBR /= filterQ;
		tripletCR /= filterQ;
		correctionR = correctionR + tripletFactorR;
		
		count++;
		//finished Triplet section- 15K
		
		if (flop)
		{
			airFactorAL = airPrevAL - inputSampleL;
			airFactorAR = airPrevAR - inputSampleR;
			if (flipA)
			{
				airEvenAL += airFactorAL;
				airOddAL -= airFactorAL;
				airFactorAL = airEvenAL * airIntensity;

				airEvenAR += airFactorAR;
				airOddAR -= airFactorAR;
				airFactorAR = airEvenAR * airIntensity;
			}
			else
			{
				airOddAL += airFactorAL;
				airEvenAL -= airFactorAL;
				airFactorAL = airOddAL * airIntensity;

				airOddAR += airFactorAR;
				airEvenAR -= airFactorAR;
				airFactorAR = airOddAR * airIntensity;
			}
			airOddAL = (airOddAL - ((airOddAL - airEvenAL)/256.0)) / filterQ;
			airEvenAL = (airEvenAL - ((airEvenAL - airOddAL)/256.0)) / filterQ;
			airPrevAL = inputSampleL;
			correctionL = correctionL + airFactorAL;

			airOddAR = (airOddAR - ((airOddAR - airEvenAR)/256.0)) / filterQ;
			airEvenAR = (airEvenAR - ((airEvenAR - airOddAR)/256.0)) / filterQ;
			airPrevAR = inputSampleR;
			correctionR = correctionR + airFactorAR;

			flipA = !flipA;
		}
		else
		{
			airFactorBL = airPrevBL - inputSampleL;
			airFactorBR = airPrevBR - inputSampleR;
			if (flipB)
			{
				airEvenBL += airFactorBL;
				airOddBL -= airFactorBL;
				airFactorBL = airEvenBL * airIntensity;

				airEvenBR += airFactorBR;
				airOddBR -= airFactorBR;
				airFactorBR = airEvenBR * airIntensity;
			}
			else
			{
				airOddBL += airFactorBL;
				airEvenBL -= airFactorBL;
				airFactorBL = airOddBL * airIntensity;

				airOddBR += airFactorBR;
				airEvenBR -= airFactorBR;
				airFactorBR = airOddBR * airIntensity;
			}
			airOddBL = (airOddBL - ((airOddBL - airEvenBL)/256.0)) / filterQ;
			airEvenBL = (airEvenBL - ((airEvenBL - airOddBL)/256.0)) / filterQ;
			airPrevBL = inputSampleL;
			correctionL = correctionL + airFactorBL;

			airOddBR = (airOddBR - ((airOddBR - airEvenBR)/256.0)) / filterQ;
			airEvenBR = (airEvenBR - ((airEvenBR - airOddBR)/256.0)) / filterQ;
			airPrevBR = inputSampleR;
			correctionR = correctionR + airFactorBR;

			flipB = !flipB;
		}
		//11K one
		airFactorCL = airPrevCL - inputSampleL;
		airFactorCR = airPrevCR - inputSampleR;
		if (flop)
		{
			airEvenCL += airFactorCL;
			airOddCL -= airFactorCL;
			airFactorCL = airEvenCL * hiIntensity;

			airEvenCR += airFactorCR;
			airOddCR -= airFactorCR;
			airFactorCR = airEvenCR * hiIntensity;
		}
		else
		{
			airOddCL += airFactorCL;
			airEvenCL -= airFactorCL;
			airFactorCL = airOddCL * hiIntensity;

			airOddCR += airFactorCR;
			airEvenCR -= airFactorCR;
			airFactorCR = airOddCR * hiIntensity;
		}
		airOddCL = (airOddCL - ((airOddCL - airEvenCL)/256.0)) / filterQ;
		airEvenCL = (airEvenCL - ((airEvenCL - airOddCL)/256.0)) / filterQ;
		airPrevCL = inputSampleL;
		correctionL = correctionL + airFactorCL;

		airOddCR = (airOddCR - ((airOddCR - airEvenCR)/256.0)) / filterQ;
		airEvenCR = (airEvenCR - ((airEvenCR - airOddCR)/256.0)) / filterQ;
		airPrevCR = inputSampleR;
		correctionR = correctionR + airFactorCR;

		flop = !flop;
		
		inputSampleL += correctionL;
		inputSampleR += correctionR;
		
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		if (wet < 1.0) {
			inputSampleL = (drySampleL*dry)+(inputSampleL*wet);
			inputSampleR = (drySampleR*dry)+(inputSampleR*wet);
		}
		//nice little output stage template: if we have another scale of floating point
		//number, we really don't want to meaninglessly multiply that by 1.0.
		
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

void Air::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double hiIntensity = -pow(((A*2.0)-1.0),3)*2;
	double tripletintensity = -pow(((B*2.0)-1.0),3);
	double airIntensity = -pow(((C*2.0)-1.0),3)/2;
	double filterQ = 2.1-D;
	double output = E;
	double wet = F;
	double dry = 1.0-wet;
	

	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	double correctionL;
	double correctionR;

    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		correctionL = 0.0;
		correctionR = 0.0;  //from here on down, please add L and R to the code
		
		if (count < 1 || count > 3) count = 1;
		tripletFactorL = tripletPrevL - inputSampleL;
		tripletFactorR = tripletPrevR - inputSampleR;
		switch (count)
		{
			case 1:
				tripletAL += tripletFactorL;
				tripletCL -= tripletFactorL;
				tripletFactorL = tripletAL * tripletintensity;
				tripletPrevL = tripletMidL;
				tripletMidL = inputSampleL;
				
				tripletAR += tripletFactorR;
				tripletCR -= tripletFactorR;
				tripletFactorR = tripletAR * tripletintensity;
				tripletPrevR = tripletMidR;
				tripletMidR = inputSampleR;
				break;
			case 2:
				tripletBL += tripletFactorL;
				tripletAL -= tripletFactorL;
				tripletFactorL = tripletBL * tripletintensity;
				tripletPrevL = tripletMidL;
				tripletMidL = inputSampleL;
				
				tripletBR += tripletFactorR;
				tripletAR -= tripletFactorR;
				tripletFactorR = tripletBR * tripletintensity;
				tripletPrevR = tripletMidR;
				tripletMidR = inputSampleR;
				break;
			case 3:
				tripletCL += tripletFactorL;
				tripletBL -= tripletFactorL;
				tripletFactorL = tripletCL * tripletintensity;
				tripletPrevL = tripletMidL;
				tripletMidL = inputSampleL;
				
				tripletCR += tripletFactorR;
				tripletBR -= tripletFactorR;
				tripletFactorR = tripletCR * tripletintensity;
				tripletPrevR = tripletMidR;
				tripletMidR = inputSampleR;
				break;
		}
		tripletAL /= filterQ;
		tripletBL /= filterQ;
		tripletCL /= filterQ;
		correctionL = correctionL + tripletFactorL;
		
		tripletAR /= filterQ;
		tripletBR /= filterQ;
		tripletCR /= filterQ;
		correctionR = correctionR + tripletFactorR;
		
		count++;
		//finished Triplet section- 15K
		
		if (flop)
		{
			airFactorAL = airPrevAL - inputSampleL;
			airFactorAR = airPrevAR - inputSampleR;
			if (flipA)
			{
				airEvenAL += airFactorAL;
				airOddAL -= airFactorAL;
				airFactorAL = airEvenAL * airIntensity;
				
				airEvenAR += airFactorAR;
				airOddAR -= airFactorAR;
				airFactorAR = airEvenAR * airIntensity;
			}
			else
			{
				airOddAL += airFactorAL;
				airEvenAL -= airFactorAL;
				airFactorAL = airOddAL * airIntensity;
				
				airOddAR += airFactorAR;
				airEvenAR -= airFactorAR;
				airFactorAR = airOddAR * airIntensity;
			}
			airOddAL = (airOddAL - ((airOddAL - airEvenAL)/256.0)) / filterQ;
			airEvenAL = (airEvenAL - ((airEvenAL - airOddAL)/256.0)) / filterQ;
			airPrevAL = inputSampleL;
			correctionL = correctionL + airFactorAL;
			
			airOddAR = (airOddAR - ((airOddAR - airEvenAR)/256.0)) / filterQ;
			airEvenAR = (airEvenAR - ((airEvenAR - airOddAR)/256.0)) / filterQ;
			airPrevAR = inputSampleR;
			correctionR = correctionR + airFactorAR;
			
			flipA = !flipA;
		}
		else
		{
			airFactorBL = airPrevBL - inputSampleL;
			airFactorBR = airPrevBR - inputSampleR;
			if (flipB)
			{
				airEvenBL += airFactorBL;
				airOddBL -= airFactorBL;
				airFactorBL = airEvenBL * airIntensity;
				
				airEvenBR += airFactorBR;
				airOddBR -= airFactorBR;
				airFactorBR = airEvenBR * airIntensity;
			}
			else
			{
				airOddBL += airFactorBL;
				airEvenBL -= airFactorBL;
				airFactorBL = airOddBL * airIntensity;
				
				airOddBR += airFactorBR;
				airEvenBR -= airFactorBR;
				airFactorBR = airOddBR * airIntensity;
			}
			airOddBL = (airOddBL - ((airOddBL - airEvenBL)/256.0)) / filterQ;
			airEvenBL = (airEvenBL - ((airEvenBL - airOddBL)/256.0)) / filterQ;
			airPrevBL = inputSampleL;
			correctionL = correctionL + airFactorBL;
			
			airOddBR = (airOddBR - ((airOddBR - airEvenBR)/256.0)) / filterQ;
			airEvenBR = (airEvenBR - ((airEvenBR - airOddBR)/256.0)) / filterQ;
			airPrevBR = inputSampleR;
			correctionR = correctionR + airFactorBR;
			
			flipB = !flipB;
		}
		//11K one
		airFactorCL = airPrevCL - inputSampleL;
		airFactorCR = airPrevCR - inputSampleR;
		if (flop)
		{
			airEvenCL += airFactorCL;
			airOddCL -= airFactorCL;
			airFactorCL = airEvenCL * hiIntensity;
			
			airEvenCR += airFactorCR;
			airOddCR -= airFactorCR;
			airFactorCR = airEvenCR * hiIntensity;
		}
		else
		{
			airOddCL += airFactorCL;
			airEvenCL -= airFactorCL;
			airFactorCL = airOddCL * hiIntensity;
			
			airOddCR += airFactorCR;
			airEvenCR -= airFactorCR;
			airFactorCR = airOddCR * hiIntensity;
		}
		airOddCL = (airOddCL - ((airOddCL - airEvenCL)/256.0)) / filterQ;
		airEvenCL = (airEvenCL - ((airEvenCL - airOddCL)/256.0)) / filterQ;
		airPrevCL = inputSampleL;
		correctionL = correctionL + airFactorCL;
		
		airOddCR = (airOddCR - ((airOddCR - airEvenCR)/256.0)) / filterQ;
		airEvenCR = (airEvenCR - ((airEvenCR - airOddCR)/256.0)) / filterQ;
		airPrevCR = inputSampleR;
		correctionR = correctionR + airFactorCR;
		
		flop = !flop;
		
		inputSampleL += correctionL;
		inputSampleR += correctionR;
		
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		if (wet < 1.0) {
			inputSampleL = (drySampleL*dry)+(inputSampleL*wet);
			inputSampleR = (drySampleR*dry)+(inputSampleR*wet);
		}
		//nice little output stage template: if we have another scale of floating point
		//number, we really don't want to meaninglessly multiply that by 1.0.

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