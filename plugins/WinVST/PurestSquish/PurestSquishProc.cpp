/* ========================================
 *  PurestSquish - PurestSquish.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestSquish_H
#include "PurestSquish.h"
#endif

void PurestSquish::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double highpassSampleL;
	double highpassSampleBL;
	double highpassSampleR;
	double highpassSampleBR;
	double threshold = 1.01 - (1.0-pow(1.0-(A*0.5),4));
	double iirAmount = pow(B,4)/overallscale;
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

		double muMakeupGainL = 1.0 / threshold;
		double outMakeupGainL = sqrt(muMakeupGainL);
		muMakeupGainL += outMakeupGainL;
		muMakeupGainL *= 0.5;
		outMakeupGainL *= 0.5;
		//gain settings around threshold
		double releaseL = mergedCoefficientsL * 32768.0;
		releaseL /= overallscale;
		double fastestL = sqrt(releaseL);
		//speed settings around release
		double lastCorrectionL = mergedCoefficientsL;
		// µ µ µ µ µ µ µ µ µ µ µ µ is the kitten song o/~
		
		double muMakeupGainR = 1.0 / threshold;
		double outMakeupGainR = sqrt(muMakeupGainR);
		muMakeupGainR += outMakeupGainR;
		muMakeupGainR *= 0.5;
		outMakeupGainR *= 0.5;
		//gain settings around threshold
		double releaseR = mergedCoefficientsR * 32768.0;
		releaseR /= overallscale;
		double fastestR = sqrt(releaseR);
		//speed settings around release
		double lastCorrectionR = mergedCoefficientsR;
		// µ µ µ µ µ µ µ µ µ µ µ µ is the kitten song o/~
		
		if (muMakeupGainL != 1.0) inputSampleL = inputSampleL * muMakeupGainL;
		if (muMakeupGainR != 1.0) inputSampleR = inputSampleR * muMakeupGainR;
		
		highpassSampleBL = highpassSampleL = inputSampleL;
		highpassSampleBR = highpassSampleR = inputSampleR;
		
		if (count < 1 || count > 3) count = 1;
		switch (count)
		{
			case 1:
				//begin L
				iirSampleAL = (iirSampleAL * (1 - iirAmount)) + (highpassSampleL * iirAmount);
				highpassSampleL -= iirSampleAL;
				if (fabs(highpassSampleL) > threshold)
				{
					muVaryL = threshold / fabs(highpassSampleL);
					muAttackL = sqrt(fabs(muSpeedAL));
					muCoefficientAL = muCoefficientAL * (muAttackL-1.0);
					if (muVaryL < threshold)
					{
						muCoefficientAL = muCoefficientAL + threshold;
					}
					else
					{
						muCoefficientAL = muCoefficientAL + muVaryL;
					}
					muCoefficientAL = muCoefficientAL / muAttackL;
				}
				else
				{
					muCoefficientAL = muCoefficientAL * ((muSpeedAL * muSpeedAL)-1.0);
					muCoefficientAL = muCoefficientAL + 1.0;
					muCoefficientAL = muCoefficientAL / (muSpeedAL * muSpeedAL);
				}
				muNewSpeedL = muSpeedAL * (muSpeedAL-1);
				muNewSpeedL = muNewSpeedL + fabs(highpassSampleL*releaseL)+fastestL;
				muSpeedAL = muNewSpeedL / muSpeedAL;
				lastCoefficientAL = pow(muCoefficientAL,2);
				mergedCoefficientsL = lastCoefficientBL;
				mergedCoefficientsL += lastCoefficientAL;
				lastCoefficientAL *= (1.0-lastCorrectionL);
				lastCoefficientAL += (muCoefficientAL * lastCorrectionL);
				lastCoefficientBL = lastCoefficientAL;
				//end L
				
				//begin R
				iirSampleAR = (iirSampleAR * (1 - iirAmount)) + (highpassSampleR * iirAmount);
				highpassSampleR -= iirSampleAR;
				if (fabs(highpassSampleR) > threshold)
				{
					muVaryR = threshold / fabs(highpassSampleR);
					muAttackR = sqrt(fabs(muSpeedAR));
					muCoefficientAR = muCoefficientAR * (muAttackR-1.0);
					if (muVaryR < threshold)
					{
						muCoefficientAR = muCoefficientAR + threshold;
					}
					else
					{
						muCoefficientAR = muCoefficientAR + muVaryR;
					}
					muCoefficientAR = muCoefficientAR / muAttackR;
				}
				else
				{
					muCoefficientAR = muCoefficientAR * ((muSpeedAR * muSpeedAR)-1.0);
					muCoefficientAR = muCoefficientAR + 1.0;
					muCoefficientAR = muCoefficientAR / (muSpeedAR * muSpeedAR);
				}
				muNewSpeedR = muSpeedAR * (muSpeedAR-1);
				muNewSpeedR = muNewSpeedR + fabs(highpassSampleR*releaseR)+fastestR;
				muSpeedAR = muNewSpeedR / muSpeedAR;
				lastCoefficientAR = pow(muCoefficientAR,2);
				mergedCoefficientsR = lastCoefficientBR;
				mergedCoefficientsR += lastCoefficientAR;
				lastCoefficientAR *= (1.0-lastCorrectionR);
				lastCoefficientAR += (muCoefficientAR * lastCorrectionR);
				lastCoefficientBR = lastCoefficientAR;
				//end R
				
				break;
			case 2:
				//begin L
				iirSampleBL = (iirSampleBL * (1 - iirAmount)) + (highpassSampleL * iirAmount);
				highpassSampleL -= iirSampleBL;
				if (fabs(highpassSampleL) > threshold)
				{
					muVaryL = threshold / fabs(highpassSampleL);
					muAttackL = sqrt(fabs(muSpeedBL));
					muCoefficientBL = muCoefficientBL * (muAttackL-1);
					if (muVaryL < threshold)
					{
						muCoefficientBL = muCoefficientBL + threshold;
					}
					else
					{
						muCoefficientBL = muCoefficientBL + muVaryL;
					}
					muCoefficientBL = muCoefficientBL / muAttackL;
				}
				else
				{
					muCoefficientBL = muCoefficientBL * ((muSpeedBL * muSpeedBL)-1.0);
					muCoefficientBL = muCoefficientBL + 1.0;
					muCoefficientBL = muCoefficientBL / (muSpeedBL * muSpeedBL);
				}
				muNewSpeedL = muSpeedBL * (muSpeedBL-1);
				muNewSpeedL = muNewSpeedL + fabs(highpassSampleL*releaseL)+fastestL;
				muSpeedBL = muNewSpeedL / muSpeedBL;
				lastCoefficientAL = pow(muCoefficientBL,2);
				mergedCoefficientsL = lastCoefficientBL;
				mergedCoefficientsL += lastCoefficientAL;
				lastCoefficientAL *= (1.0-lastCorrectionL);
				lastCoefficientAL += (muCoefficientBL * lastCorrectionL);
				lastCoefficientBL = lastCoefficientAL;
				//end L
				
				//begin R
				iirSampleBR = (iirSampleBR * (1 - iirAmount)) + (highpassSampleR * iirAmount);
				highpassSampleR -= iirSampleBR;
				if (fabs(highpassSampleR) > threshold)
				{
					muVaryR = threshold / fabs(highpassSampleR);
					muAttackR = sqrt(fabs(muSpeedBR));
					muCoefficientBR = muCoefficientBR * (muAttackR-1);
					if (muVaryR < threshold)
					{
						muCoefficientBR = muCoefficientBR + threshold;
					}
					else
					{
						muCoefficientBR = muCoefficientBR + muVaryR;
					}
					muCoefficientBR = muCoefficientBR / muAttackR;
				}
				else
				{
					muCoefficientBR = muCoefficientBR * ((muSpeedBR * muSpeedBR)-1.0);
					muCoefficientBR = muCoefficientBR + 1.0;
					muCoefficientBR = muCoefficientBR / (muSpeedBR * muSpeedBR);
				}
				muNewSpeedR = muSpeedBR * (muSpeedBR-1);
				muNewSpeedR = muNewSpeedR + fabs(highpassSampleR*releaseR)+fastestR;
				muSpeedBR = muNewSpeedR / muSpeedBR;
				lastCoefficientAR = pow(muCoefficientBR,2);
				mergedCoefficientsR = lastCoefficientBR;
				mergedCoefficientsR += lastCoefficientAR;
				lastCoefficientAR *= (1.0-lastCorrectionR);
				lastCoefficientAR += (muCoefficientBR * lastCorrectionR);
				lastCoefficientBR = lastCoefficientAR;
				//end R
				
				break;
			case 3:
				//begin L
				iirSampleCL = (iirSampleCL * (1 - iirAmount)) + (highpassSampleL * iirAmount);
				highpassSampleL -= iirSampleCL;
				if (fabs(highpassSampleL) > threshold)
				{
					muVaryL = threshold / fabs(highpassSampleL);
					muAttackL = sqrt(fabs(muSpeedCL));
					muCoefficientCL = muCoefficientCL * (muAttackL-1);
					if (muVaryL < threshold)
					{
						muCoefficientCL = muCoefficientCL + threshold;
					}
					else
					{
						muCoefficientCL = muCoefficientCL + muVaryL;
					}
					muCoefficientCL = muCoefficientCL / muAttackL;
				}
				else
				{
					muCoefficientCL = muCoefficientCL * ((muSpeedCL * muSpeedCL)-1.0);
					muCoefficientCL = muCoefficientCL + 1.0;
					muCoefficientCL = muCoefficientCL / (muSpeedCL * muSpeedCL);
				}
				muNewSpeedL = muSpeedCL * (muSpeedCL-1);
				muNewSpeedL = muNewSpeedL + fabs(highpassSampleL*releaseL)+fastestL;
				muSpeedCL = muNewSpeedL / muSpeedCL;
				lastCoefficientAL = pow(muCoefficientCL,2);
				mergedCoefficientsL = lastCoefficientBL;
				mergedCoefficientsL += lastCoefficientAL;
				lastCoefficientAL *= (1.0-lastCorrectionL);
				lastCoefficientAL += (muCoefficientCL * lastCorrectionL);
				lastCoefficientBL = lastCoefficientAL;
				//end L
				
				//begin R
				iirSampleCR = (iirSampleCR * (1 - iirAmount)) + (highpassSampleR * iirAmount);
				highpassSampleR -= iirSampleCR;
				if (fabs(highpassSampleR) > threshold)
				{
					muVaryR = threshold / fabs(highpassSampleR);
					muAttackR = sqrt(fabs(muSpeedCR));
					muCoefficientCR = muCoefficientCR * (muAttackR-1);
					if (muVaryR < threshold)
					{
						muCoefficientCR = muCoefficientCR + threshold;
					}
					else
					{
						muCoefficientCR = muCoefficientCR + muVaryR;
					}
					muCoefficientCR = muCoefficientCR / muAttackR;
				}
				else
				{
					muCoefficientCR = muCoefficientCR * ((muSpeedCR * muSpeedCR)-1.0);
					muCoefficientCR = muCoefficientCR + 1.0;
					muCoefficientCR = muCoefficientCR / (muSpeedCR * muSpeedCR);
				}
				muNewSpeedR = muSpeedCR * (muSpeedCR-1);
				muNewSpeedR = muNewSpeedR + fabs(highpassSampleR*releaseR)+fastestR;
				muSpeedCR = muNewSpeedR / muSpeedCR;
				lastCoefficientAR = pow(muCoefficientCR,2);
				mergedCoefficientsR = lastCoefficientBR;
				mergedCoefficientsR += lastCoefficientAR;
				lastCoefficientAR *= (1.0-lastCorrectionR);
				lastCoefficientAR += (muCoefficientCR * lastCorrectionR);
				lastCoefficientBR = lastCoefficientAR;
				//end R
				
				break;
		}		
		count++;
		
		//applied compression with vari-vari-µ-µ-µ-µ-µ-µ-is-the-kitten-song o/~
		//applied gain correction to control output level- tends to constrain sound rather than inflate it
		
		if (fpFlip) {
			//begin L
			iirSampleDL = (iirSampleDL * (1 - iirAmount)) + (highpassSampleBL * iirAmount);
			highpassSampleBL -= iirSampleDL;
			if (fabs(highpassSampleBL) > threshold)
			{
				muVaryL = threshold / fabs(highpassSampleBL);
				muAttackL = sqrt(fabs(muSpeedDL));
				muCoefficientDL = muCoefficientDL * (muAttackL-1.0);
				if (muVaryL < threshold)
				{
					muCoefficientDL = muCoefficientDL + threshold;
				}
				else
				{
					muCoefficientDL = muCoefficientDL + muVaryL;
				}
				muCoefficientDL = muCoefficientDL / muAttackL;
			}
			else
			{
				muCoefficientDL = muCoefficientDL * ((muSpeedDL * muSpeedDL)-1.0);
				muCoefficientDL = muCoefficientDL + 1.0;
				muCoefficientDL = muCoefficientDL / (muSpeedDL * muSpeedDL);
			}
			muNewSpeedL = muSpeedDL * (muSpeedDL-1);
			muNewSpeedL = muNewSpeedL + fabs(highpassSampleBL*releaseL)+fastestL;
			muSpeedDL = muNewSpeedL / muSpeedDL;
			lastCoefficientCL = pow(muCoefficientEL,2);
			mergedCoefficientsL += lastCoefficientDL;
			mergedCoefficientsL += lastCoefficientCL;
			lastCoefficientCL *= (1.0-lastCorrectionL);
			lastCoefficientCL += (muCoefficientDL * lastCorrectionL);
			lastCoefficientDL = lastCoefficientCL;
			//end L
			
			//begin R
			iirSampleDR = (iirSampleDR * (1 - iirAmount)) + (highpassSampleBR * iirAmount);
			highpassSampleBR -= iirSampleDR;
			if (fabs(highpassSampleBR) > threshold)
			{
				muVaryR = threshold / fabs(highpassSampleBR);
				muAttackR = sqrt(fabs(muSpeedDR));
				muCoefficientDR = muCoefficientDR * (muAttackR-1.0);
				if (muVaryR < threshold)
				{
					muCoefficientDR = muCoefficientDR + threshold;
				}
				else
				{
					muCoefficientDR = muCoefficientDR + muVaryR;
				}
				muCoefficientDR = muCoefficientDR / muAttackR;
			}
			else
			{
				muCoefficientDR = muCoefficientDR * ((muSpeedDR * muSpeedDR)-1.0);
				muCoefficientDR = muCoefficientDR + 1.0;
				muCoefficientDR = muCoefficientDR / (muSpeedDR * muSpeedDR);
			}
			muNewSpeedR = muSpeedDR * (muSpeedDR-1);
			muNewSpeedR = muNewSpeedR + fabs(highpassSampleBR*releaseR)+fastestR;
			muSpeedDR = muNewSpeedR / muSpeedDR;
			lastCoefficientCR = pow(muCoefficientER,2);
			mergedCoefficientsR += lastCoefficientDR;
			mergedCoefficientsR += lastCoefficientCR;
			lastCoefficientCR *= (1.0-lastCorrectionR);
			lastCoefficientCR += (muCoefficientDR * lastCorrectionR);
			lastCoefficientDR = lastCoefficientCR;
			//end R
			
		} else {
			//begin L
			iirSampleEL = (iirSampleEL * (1 - iirAmount)) + (highpassSampleBL * iirAmount);
			highpassSampleBL -= iirSampleEL;
			if (fabs(highpassSampleBL) > threshold)
			{
				muVaryL = threshold / fabs(highpassSampleBL);
				muAttackL = sqrt(fabs(muSpeedEL));
				muCoefficientEL = muCoefficientEL * (muAttackL-1.0);
				if (muVaryL < threshold)
				{
					muCoefficientEL = muCoefficientEL + threshold;
				}
				else
				{
					muCoefficientEL = muCoefficientEL + muVaryL;
				}
				muCoefficientEL = muCoefficientEL / muAttackL;
			}
			else
			{
				muCoefficientEL = muCoefficientEL * ((muSpeedEL * muSpeedEL)-1.0);
				muCoefficientEL = muCoefficientEL + 1.0;
				muCoefficientEL = muCoefficientEL / (muSpeedEL * muSpeedEL);
			}
			muNewSpeedL = muSpeedEL * (muSpeedEL-1);
			muNewSpeedL = muNewSpeedL + fabs(highpassSampleBL*releaseL)+fastestL;
			muSpeedEL = muNewSpeedL / muSpeedEL;
			lastCoefficientCL = pow(muCoefficientEL,2);
			mergedCoefficientsL += lastCoefficientDL;
			mergedCoefficientsL += lastCoefficientCL;
			lastCoefficientCL *= (1.0-lastCorrectionL);
			lastCoefficientCL += (muCoefficientEL * lastCorrectionL);
			lastCoefficientDL = lastCoefficientCL;
			//end L
			
			//begin R
			iirSampleER = (iirSampleER * (1 - iirAmount)) + (highpassSampleBR * iirAmount);
			highpassSampleBR -= iirSampleER;
			if (fabs(highpassSampleBR) > threshold)
			{
				muVaryR = threshold / fabs(highpassSampleBR);
				muAttackR = sqrt(fabs(muSpeedER));
				muCoefficientER = muCoefficientER * (muAttackR-1.0);
				if (muVaryR < threshold)
				{
					muCoefficientER = muCoefficientER + threshold;
				}
				else
				{
					muCoefficientER = muCoefficientER + muVaryR;
				}
				muCoefficientER = muCoefficientER / muAttackR;
			}
			else
			{
				muCoefficientER = muCoefficientER * ((muSpeedER * muSpeedER)-1.0);
				muCoefficientER = muCoefficientER + 1.0;
				muCoefficientER = muCoefficientER / (muSpeedER * muSpeedER);
			}
			muNewSpeedR = muSpeedER * (muSpeedER-1);
			muNewSpeedR = muNewSpeedR + fabs(highpassSampleBR*releaseR)+fastestR;
			muSpeedER = muNewSpeedR / muSpeedER;
			lastCoefficientCR = pow(muCoefficientER,2);
			mergedCoefficientsR += lastCoefficientDR;
			mergedCoefficientsR += lastCoefficientCR;
			lastCoefficientCR *= (1.0-lastCorrectionR);
			lastCoefficientCR += (muCoefficientER * lastCorrectionR);
			lastCoefficientDR = lastCoefficientCR;
			//end R
			
		}
		mergedCoefficientsL *= 0.25;
		inputSampleL *= mergedCoefficientsL;
		
		mergedCoefficientsR *= 0.25;
		inputSampleR *= mergedCoefficientsR;
		
		if (outMakeupGainL != 1.0) inputSampleL = inputSampleL * outMakeupGainL;
		if (outMakeupGainR != 1.0) inputSampleR = inputSampleR * outMakeupGainR;
		
		fpFlip = !fpFlip;
		
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		
		if (wet < 1.0) {
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

void PurestSquish::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double highpassSampleL;
	double highpassSampleBL;
	double highpassSampleR;
	double highpassSampleBR;
	double threshold = 1.01 - (1.0-pow(1.0-(A*0.5),4));
	double iirAmount = pow(B,4)/overallscale;
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
		
		double muMakeupGainL = 1.0 / threshold;
		double outMakeupGainL = sqrt(muMakeupGainL);
		muMakeupGainL += outMakeupGainL;
		muMakeupGainL *= 0.5;
		outMakeupGainL *= 0.5;
		//gain settings around threshold
		double releaseL = mergedCoefficientsL * 32768.0;
		releaseL /= overallscale;
		double fastestL = sqrt(releaseL);
		//speed settings around release
		double lastCorrectionL = mergedCoefficientsL;
		// µ µ µ µ µ µ µ µ µ µ µ µ is the kitten song o/~
		
		double muMakeupGainR = 1.0 / threshold;
		double outMakeupGainR = sqrt(muMakeupGainR);
		muMakeupGainR += outMakeupGainR;
		muMakeupGainR *= 0.5;
		outMakeupGainR *= 0.5;
		//gain settings around threshold
		double releaseR = mergedCoefficientsR * 32768.0;
		releaseR /= overallscale;
		double fastestR = sqrt(releaseR);
		//speed settings around release
		double lastCorrectionR = mergedCoefficientsR;
		// µ µ µ µ µ µ µ µ µ µ µ µ is the kitten song o/~
		
		if (muMakeupGainL != 1.0) inputSampleL = inputSampleL * muMakeupGainL;
		if (muMakeupGainR != 1.0) inputSampleR = inputSampleR * muMakeupGainR;
		
		highpassSampleBL = highpassSampleL = inputSampleL;
		highpassSampleBR = highpassSampleR = inputSampleR;
		
		if (count < 1 || count > 3) count = 1;
		switch (count)
		{
			case 1:
				//begin L
				iirSampleAL = (iirSampleAL * (1 - iirAmount)) + (highpassSampleL * iirAmount);
				highpassSampleL -= iirSampleAL;
				if (fabs(highpassSampleL) > threshold)
				{
					muVaryL = threshold / fabs(highpassSampleL);
					muAttackL = sqrt(fabs(muSpeedAL));
					muCoefficientAL = muCoefficientAL * (muAttackL-1.0);
					if (muVaryL < threshold)
					{
						muCoefficientAL = muCoefficientAL + threshold;
					}
					else
					{
						muCoefficientAL = muCoefficientAL + muVaryL;
					}
					muCoefficientAL = muCoefficientAL / muAttackL;
				}
				else
				{
					muCoefficientAL = muCoefficientAL * ((muSpeedAL * muSpeedAL)-1.0);
					muCoefficientAL = muCoefficientAL + 1.0;
					muCoefficientAL = muCoefficientAL / (muSpeedAL * muSpeedAL);
				}
				muNewSpeedL = muSpeedAL * (muSpeedAL-1);
				muNewSpeedL = muNewSpeedL + fabs(highpassSampleL*releaseL)+fastestL;
				muSpeedAL = muNewSpeedL / muSpeedAL;
				lastCoefficientAL = pow(muCoefficientAL,2);
				mergedCoefficientsL = lastCoefficientBL;
				mergedCoefficientsL += lastCoefficientAL;
				lastCoefficientAL *= (1.0-lastCorrectionL);
				lastCoefficientAL += (muCoefficientAL * lastCorrectionL);
				lastCoefficientBL = lastCoefficientAL;
				//end L
				
				//begin R
				iirSampleAR = (iirSampleAR * (1 - iirAmount)) + (highpassSampleR * iirAmount);
				highpassSampleR -= iirSampleAR;
				if (fabs(highpassSampleR) > threshold)
				{
					muVaryR = threshold / fabs(highpassSampleR);
					muAttackR = sqrt(fabs(muSpeedAR));
					muCoefficientAR = muCoefficientAR * (muAttackR-1.0);
					if (muVaryR < threshold)
					{
						muCoefficientAR = muCoefficientAR + threshold;
					}
					else
					{
						muCoefficientAR = muCoefficientAR + muVaryR;
					}
					muCoefficientAR = muCoefficientAR / muAttackR;
				}
				else
				{
					muCoefficientAR = muCoefficientAR * ((muSpeedAR * muSpeedAR)-1.0);
					muCoefficientAR = muCoefficientAR + 1.0;
					muCoefficientAR = muCoefficientAR / (muSpeedAR * muSpeedAR);
				}
				muNewSpeedR = muSpeedAR * (muSpeedAR-1);
				muNewSpeedR = muNewSpeedR + fabs(highpassSampleR*releaseR)+fastestR;
				muSpeedAR = muNewSpeedR / muSpeedAR;
				lastCoefficientAR = pow(muCoefficientAR,2);
				mergedCoefficientsR = lastCoefficientBR;
				mergedCoefficientsR += lastCoefficientAR;
				lastCoefficientAR *= (1.0-lastCorrectionR);
				lastCoefficientAR += (muCoefficientAR * lastCorrectionR);
				lastCoefficientBR = lastCoefficientAR;
				//end R
				
				break;
			case 2:
				//begin L
				iirSampleBL = (iirSampleBL * (1 - iirAmount)) + (highpassSampleL * iirAmount);
				highpassSampleL -= iirSampleBL;
				if (fabs(highpassSampleL) > threshold)
				{
					muVaryL = threshold / fabs(highpassSampleL);
					muAttackL = sqrt(fabs(muSpeedBL));
					muCoefficientBL = muCoefficientBL * (muAttackL-1);
					if (muVaryL < threshold)
					{
						muCoefficientBL = muCoefficientBL + threshold;
					}
					else
					{
						muCoefficientBL = muCoefficientBL + muVaryL;
					}
					muCoefficientBL = muCoefficientBL / muAttackL;
				}
				else
				{
					muCoefficientBL = muCoefficientBL * ((muSpeedBL * muSpeedBL)-1.0);
					muCoefficientBL = muCoefficientBL + 1.0;
					muCoefficientBL = muCoefficientBL / (muSpeedBL * muSpeedBL);
				}
				muNewSpeedL = muSpeedBL * (muSpeedBL-1);
				muNewSpeedL = muNewSpeedL + fabs(highpassSampleL*releaseL)+fastestL;
				muSpeedBL = muNewSpeedL / muSpeedBL;
				lastCoefficientAL = pow(muCoefficientBL,2);
				mergedCoefficientsL = lastCoefficientBL;
				mergedCoefficientsL += lastCoefficientAL;
				lastCoefficientAL *= (1.0-lastCorrectionL);
				lastCoefficientAL += (muCoefficientBL * lastCorrectionL);
				lastCoefficientBL = lastCoefficientAL;
				//end L
				
				//begin R
				iirSampleBR = (iirSampleBR * (1 - iirAmount)) + (highpassSampleR * iirAmount);
				highpassSampleR -= iirSampleBR;
				if (fabs(highpassSampleR) > threshold)
				{
					muVaryR = threshold / fabs(highpassSampleR);
					muAttackR = sqrt(fabs(muSpeedBR));
					muCoefficientBR = muCoefficientBR * (muAttackR-1);
					if (muVaryR < threshold)
					{
						muCoefficientBR = muCoefficientBR + threshold;
					}
					else
					{
						muCoefficientBR = muCoefficientBR + muVaryR;
					}
					muCoefficientBR = muCoefficientBR / muAttackR;
				}
				else
				{
					muCoefficientBR = muCoefficientBR * ((muSpeedBR * muSpeedBR)-1.0);
					muCoefficientBR = muCoefficientBR + 1.0;
					muCoefficientBR = muCoefficientBR / (muSpeedBR * muSpeedBR);
				}
				muNewSpeedR = muSpeedBR * (muSpeedBR-1);
				muNewSpeedR = muNewSpeedR + fabs(highpassSampleR*releaseR)+fastestR;
				muSpeedBR = muNewSpeedR / muSpeedBR;
				lastCoefficientAR = pow(muCoefficientBR,2);
				mergedCoefficientsR = lastCoefficientBR;
				mergedCoefficientsR += lastCoefficientAR;
				lastCoefficientAR *= (1.0-lastCorrectionR);
				lastCoefficientAR += (muCoefficientBR * lastCorrectionR);
				lastCoefficientBR = lastCoefficientAR;
				//end R
				
				break;
			case 3:
				//begin L
				iirSampleCL = (iirSampleCL * (1 - iirAmount)) + (highpassSampleL * iirAmount);
				highpassSampleL -= iirSampleCL;
				if (fabs(highpassSampleL) > threshold)
				{
					muVaryL = threshold / fabs(highpassSampleL);
					muAttackL = sqrt(fabs(muSpeedCL));
					muCoefficientCL = muCoefficientCL * (muAttackL-1);
					if (muVaryL < threshold)
					{
						muCoefficientCL = muCoefficientCL + threshold;
					}
					else
					{
						muCoefficientCL = muCoefficientCL + muVaryL;
					}
					muCoefficientCL = muCoefficientCL / muAttackL;
				}
				else
				{
					muCoefficientCL = muCoefficientCL * ((muSpeedCL * muSpeedCL)-1.0);
					muCoefficientCL = muCoefficientCL + 1.0;
					muCoefficientCL = muCoefficientCL / (muSpeedCL * muSpeedCL);
				}
				muNewSpeedL = muSpeedCL * (muSpeedCL-1);
				muNewSpeedL = muNewSpeedL + fabs(highpassSampleL*releaseL)+fastestL;
				muSpeedCL = muNewSpeedL / muSpeedCL;
				lastCoefficientAL = pow(muCoefficientCL,2);
				mergedCoefficientsL = lastCoefficientBL;
				mergedCoefficientsL += lastCoefficientAL;
				lastCoefficientAL *= (1.0-lastCorrectionL);
				lastCoefficientAL += (muCoefficientCL * lastCorrectionL);
				lastCoefficientBL = lastCoefficientAL;
				//end L
				
				//begin R
				iirSampleCR = (iirSampleCR * (1 - iirAmount)) + (highpassSampleR * iirAmount);
				highpassSampleR -= iirSampleCR;
				if (fabs(highpassSampleR) > threshold)
				{
					muVaryR = threshold / fabs(highpassSampleR);
					muAttackR = sqrt(fabs(muSpeedCR));
					muCoefficientCR = muCoefficientCR * (muAttackR-1);
					if (muVaryR < threshold)
					{
						muCoefficientCR = muCoefficientCR + threshold;
					}
					else
					{
						muCoefficientCR = muCoefficientCR + muVaryR;
					}
					muCoefficientCR = muCoefficientCR / muAttackR;
				}
				else
				{
					muCoefficientCR = muCoefficientCR * ((muSpeedCR * muSpeedCR)-1.0);
					muCoefficientCR = muCoefficientCR + 1.0;
					muCoefficientCR = muCoefficientCR / (muSpeedCR * muSpeedCR);
				}
				muNewSpeedR = muSpeedCR * (muSpeedCR-1);
				muNewSpeedR = muNewSpeedR + fabs(highpassSampleR*releaseR)+fastestR;
				muSpeedCR = muNewSpeedR / muSpeedCR;
				lastCoefficientAR = pow(muCoefficientCR,2);
				mergedCoefficientsR = lastCoefficientBR;
				mergedCoefficientsR += lastCoefficientAR;
				lastCoefficientAR *= (1.0-lastCorrectionR);
				lastCoefficientAR += (muCoefficientCR * lastCorrectionR);
				lastCoefficientBR = lastCoefficientAR;
				//end R
				
				break;
		}		
		count++;
		
		//applied compression with vari-vari-µ-µ-µ-µ-µ-µ-is-the-kitten-song o/~
		//applied gain correction to control output level- tends to constrain sound rather than inflate it
		
		if (fpFlip) {
			//begin L
			iirSampleDL = (iirSampleDL * (1 - iirAmount)) + (highpassSampleBL * iirAmount);
			highpassSampleBL -= iirSampleDL;
			if (fabs(highpassSampleBL) > threshold)
			{
				muVaryL = threshold / fabs(highpassSampleBL);
				muAttackL = sqrt(fabs(muSpeedDL));
				muCoefficientDL = muCoefficientDL * (muAttackL-1.0);
				if (muVaryL < threshold)
				{
					muCoefficientDL = muCoefficientDL + threshold;
				}
				else
				{
					muCoefficientDL = muCoefficientDL + muVaryL;
				}
				muCoefficientDL = muCoefficientDL / muAttackL;
			}
			else
			{
				muCoefficientDL = muCoefficientDL * ((muSpeedDL * muSpeedDL)-1.0);
				muCoefficientDL = muCoefficientDL + 1.0;
				muCoefficientDL = muCoefficientDL / (muSpeedDL * muSpeedDL);
			}
			muNewSpeedL = muSpeedDL * (muSpeedDL-1);
			muNewSpeedL = muNewSpeedL + fabs(highpassSampleBL*releaseL)+fastestL;
			muSpeedDL = muNewSpeedL / muSpeedDL;
			lastCoefficientCL = pow(muCoefficientEL,2);
			mergedCoefficientsL += lastCoefficientDL;
			mergedCoefficientsL += lastCoefficientCL;
			lastCoefficientCL *= (1.0-lastCorrectionL);
			lastCoefficientCL += (muCoefficientDL * lastCorrectionL);
			lastCoefficientDL = lastCoefficientCL;
			//end L
			
			//begin R
			iirSampleDR = (iirSampleDR * (1 - iirAmount)) + (highpassSampleBR * iirAmount);
			highpassSampleBR -= iirSampleDR;
			if (fabs(highpassSampleBR) > threshold)
			{
				muVaryR = threshold / fabs(highpassSampleBR);
				muAttackR = sqrt(fabs(muSpeedDR));
				muCoefficientDR = muCoefficientDR * (muAttackR-1.0);
				if (muVaryR < threshold)
				{
					muCoefficientDR = muCoefficientDR + threshold;
				}
				else
				{
					muCoefficientDR = muCoefficientDR + muVaryR;
				}
				muCoefficientDR = muCoefficientDR / muAttackR;
			}
			else
			{
				muCoefficientDR = muCoefficientDR * ((muSpeedDR * muSpeedDR)-1.0);
				muCoefficientDR = muCoefficientDR + 1.0;
				muCoefficientDR = muCoefficientDR / (muSpeedDR * muSpeedDR);
			}
			muNewSpeedR = muSpeedDR * (muSpeedDR-1);
			muNewSpeedR = muNewSpeedR + fabs(highpassSampleBR*releaseR)+fastestR;
			muSpeedDR = muNewSpeedR / muSpeedDR;
			lastCoefficientCR = pow(muCoefficientER,2);
			mergedCoefficientsR += lastCoefficientDR;
			mergedCoefficientsR += lastCoefficientCR;
			lastCoefficientCR *= (1.0-lastCorrectionR);
			lastCoefficientCR += (muCoefficientDR * lastCorrectionR);
			lastCoefficientDR = lastCoefficientCR;
			//end R
			
		} else {
			//begin L
			iirSampleEL = (iirSampleEL * (1 - iirAmount)) + (highpassSampleBL * iirAmount);
			highpassSampleBL -= iirSampleEL;
			if (fabs(highpassSampleBL) > threshold)
			{
				muVaryL = threshold / fabs(highpassSampleBL);
				muAttackL = sqrt(fabs(muSpeedEL));
				muCoefficientEL = muCoefficientEL * (muAttackL-1.0);
				if (muVaryL < threshold)
				{
					muCoefficientEL = muCoefficientEL + threshold;
				}
				else
				{
					muCoefficientEL = muCoefficientEL + muVaryL;
				}
				muCoefficientEL = muCoefficientEL / muAttackL;
			}
			else
			{
				muCoefficientEL = muCoefficientEL * ((muSpeedEL * muSpeedEL)-1.0);
				muCoefficientEL = muCoefficientEL + 1.0;
				muCoefficientEL = muCoefficientEL / (muSpeedEL * muSpeedEL);
			}
			muNewSpeedL = muSpeedEL * (muSpeedEL-1);
			muNewSpeedL = muNewSpeedL + fabs(highpassSampleBL*releaseL)+fastestL;
			muSpeedEL = muNewSpeedL / muSpeedEL;
			lastCoefficientCL = pow(muCoefficientEL,2);
			mergedCoefficientsL += lastCoefficientDL;
			mergedCoefficientsL += lastCoefficientCL;
			lastCoefficientCL *= (1.0-lastCorrectionL);
			lastCoefficientCL += (muCoefficientEL * lastCorrectionL);
			lastCoefficientDL = lastCoefficientCL;
			//end L
			
			//begin R
			iirSampleER = (iirSampleER * (1 - iirAmount)) + (highpassSampleBR * iirAmount);
			highpassSampleBR -= iirSampleER;
			if (fabs(highpassSampleBR) > threshold)
			{
				muVaryR = threshold / fabs(highpassSampleBR);
				muAttackR = sqrt(fabs(muSpeedER));
				muCoefficientER = muCoefficientER * (muAttackR-1.0);
				if (muVaryR < threshold)
				{
					muCoefficientER = muCoefficientER + threshold;
				}
				else
				{
					muCoefficientER = muCoefficientER + muVaryR;
				}
				muCoefficientER = muCoefficientER / muAttackR;
			}
			else
			{
				muCoefficientER = muCoefficientER * ((muSpeedER * muSpeedER)-1.0);
				muCoefficientER = muCoefficientER + 1.0;
				muCoefficientER = muCoefficientER / (muSpeedER * muSpeedER);
			}
			muNewSpeedR = muSpeedER * (muSpeedER-1);
			muNewSpeedR = muNewSpeedR + fabs(highpassSampleBR*releaseR)+fastestR;
			muSpeedER = muNewSpeedR / muSpeedER;
			lastCoefficientCR = pow(muCoefficientER,2);
			mergedCoefficientsR += lastCoefficientDR;
			mergedCoefficientsR += lastCoefficientCR;
			lastCoefficientCR *= (1.0-lastCorrectionR);
			lastCoefficientCR += (muCoefficientER * lastCorrectionR);
			lastCoefficientDR = lastCoefficientCR;
			//end R
			
		}
		mergedCoefficientsL *= 0.25;
		inputSampleL *= mergedCoefficientsL;
		
		mergedCoefficientsR *= 0.25;
		inputSampleR *= mergedCoefficientsR;
		
		if (outMakeupGainL != 1.0) inputSampleL = inputSampleL * outMakeupGainL;
		if (outMakeupGainR != 1.0) inputSampleR = inputSampleR * outMakeupGainR;
		
		fpFlip = !fpFlip;
		
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		
		if (wet < 1.0) {
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
