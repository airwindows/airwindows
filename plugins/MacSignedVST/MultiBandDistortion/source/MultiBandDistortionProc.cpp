/* ========================================
 *  MultiBandDistortion - MultiBandDistortion.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __MultiBandDistortion_H
#include "MultiBandDistortion.h"
#endif

void MultiBandDistortion::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double iirAmount = pow(A,3)/overallscale;
	double gainH = pow(10.0,(B*48.0)/20);
	
	double thresholdH = D;
	double hardnessH;
	if (thresholdH < 1.0) hardnessH = 1.0 / (1.0-thresholdH);
	else hardnessH = 999999999999999999999.0;
	
	double gainL = pow(10.0,(C*48.0)/20);
	double thresholdL = E;
	double hardnessL;
	if (thresholdL < 1.0) hardnessL = 1.0 / (1.0-thresholdL);
	else hardnessL = 999999999999999999999.0;
	
	double breakup = 1.5707963267949;
	double bridgerectifier;
	
	double outputH = F;
	double outputL = G;
	double outputD = H*0.597;
	double outtrim = outputH + outputL + outputD;
	outputH *= outtrim;
	outputL *= outtrim;
	outputD *= outtrim;
	double outputGlobal = pow(10.0,((I-1.0)*48.0)/20);
	
	double tempHL;
	double tempLL;
	double tempHR;
	double tempLR;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		ataDrySampleL = inputSampleL;
		ataHalfDrySampleL = ataHalfwaySampleL = (inputSampleL + ataLast1SampleL + ((-ataLast2SampleL + ataLast3SampleL) * ataUpsampleHighTweakL)) / 2.0;
		ataLast3SampleL = ataLast2SampleL; ataLast2SampleL = ataLast1SampleL; ataLast1SampleL = inputSampleL;
		//setting up oversampled special antialiasing
		ataDrySampleR = inputSampleR;
		ataHalfDrySampleR = ataHalfwaySampleR = (inputSampleR + ataLast1SampleR + ((-ataLast2SampleR + ataLast3SampleR) * ataUpsampleHighTweakR)) / 2.0;
		ataLast3SampleR = ataLast2SampleR; ataLast2SampleR = ataLast1SampleR; ataLast1SampleR = inputSampleR;
		//setting up oversampled special antialiasing
		
		//pre-center code on inputSample and halfwaySample in parallel
		//begin interpolated sample- change inputSample -> ataHalfwaySample, ataDrySample -> ataHalfDrySample
		tempLL = iirSampleAL = (iirSampleAL * (1 - iirAmount)) + (ataHalfwaySampleL * iirAmount);
		tempHL = ataHalfwaySampleL - iirSampleAL;
		//highpass section
		tempLR = iirSampleAR = (iirSampleAR * (1 - iirAmount)) + (ataHalfwaySampleR * iirAmount);
		tempHR = ataHalfwaySampleR - iirSampleAR;
		//highpass section
		
		
		tempHL *= gainH;
		if (fabs(tempHL) > thresholdH)
		{
			bridgerectifier = (fabs(tempHL)-thresholdH)*hardnessH;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessH;
			//do the sine factor, scale back to proper amount
			if (tempHL > 0) tempHL = bridgerectifier+thresholdH;
			else tempHL = -(bridgerectifier+thresholdH);
		}
		tempHR *= gainH;
		if (fabs(tempHR) > thresholdH)
		{
			bridgerectifier = (fabs(tempHR)-thresholdH)*hardnessH;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessH;
			//do the sine factor, scale back to proper amount
			if (tempHR > 0) tempHR = bridgerectifier+thresholdH;
			else tempHR = -(bridgerectifier+thresholdH);
		}
		
		tempLL *= gainL;
		if (fabs(tempLL) > thresholdL)
		{
			bridgerectifier = (fabs(tempLL)-thresholdL)*hardnessL;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessL;
			//do the sine factor, scale back to proper amount
			if (tempLL > 0) tempLL = bridgerectifier+thresholdL;
			else tempLL = -(bridgerectifier+thresholdL);
		}
		tempLR *= gainL;
		if (fabs(tempLR) > thresholdL)
		{
			bridgerectifier = (fabs(tempLR)-thresholdL)*hardnessL;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessL;
			//do the sine factor, scale back to proper amount
			if (tempLR > 0) tempLR = bridgerectifier+thresholdL;
			else tempLR = -(bridgerectifier+thresholdL);
		}

		ataHalfwaySampleL = (tempLL * outputL) + (tempHL * outputH);
		ataHalfwaySampleR = (tempLR * outputL) + (tempHR * outputH);
		//end interpolated sample
		
		//begin raw sample- inputSample and ataDrySample handled separately here
		tempLL = iirSampleBL = (iirSampleBL * (1 - iirAmount)) + (inputSampleL * iirAmount);
		tempHL = inputSampleL - iirSampleBL;
		//highpass section
		tempLR = iirSampleBR = (iirSampleBR * (1 - iirAmount)) + (inputSampleR * iirAmount);
		tempHR = inputSampleR - iirSampleBR;
		//highpass section
		
		tempHL *= gainH;
		if (fabs(tempHL) > thresholdH)
		{
			bridgerectifier = (fabs(tempHL)-thresholdH)*hardnessH;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessH;
			//do the sine factor, scale back to proper amount
			if (tempHL > 0) tempHL = bridgerectifier+thresholdH;
			else tempHL = -(bridgerectifier+thresholdH);
		}
		tempHR *= gainH;
		if (fabs(tempHR) > thresholdH)
		{
			bridgerectifier = (fabs(tempHR)-thresholdH)*hardnessH;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessH;
			//do the sine factor, scale back to proper amount
			if (tempHR > 0) tempHR = bridgerectifier+thresholdH;
			else tempHR = -(bridgerectifier+thresholdH);
		}
		
		tempLL *= gainL;
		if (fabs(tempLL) > thresholdL)
		{
			bridgerectifier = (fabs(tempLL)-thresholdL)*hardnessL;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessL;
			//do the sine factor, scale back to proper amount
			if (tempLL > 0) tempLL = bridgerectifier+thresholdL;
			else tempLL = -(bridgerectifier+thresholdL);
		}
		tempLR *= gainL;
		if (fabs(tempLR) > thresholdL)
		{
			bridgerectifier = (fabs(tempLR)-thresholdL)*hardnessL;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessL;
			//do the sine factor, scale back to proper amount
			if (tempLR > 0) tempLR = bridgerectifier+thresholdL;
			else tempLR = -(bridgerectifier+thresholdL);
		}
		
		
		inputSampleL = (tempLL * outputL) + (tempHL * outputH);
		inputSampleR = (tempLR * outputL) + (tempHR * outputH);
		//end raw sample
		
		//begin antialiasing section for halfway sample
		ataCL = ataHalfwaySampleL - ataHalfDrySampleL;
		if (ataFlip) {ataAL *= ataDecayL; ataBL *= ataDecayL; ataAL += ataCL; ataBL -= ataCL; ataCL = ataAL;}
		else {ataBL *= ataDecayL; ataAL *= ataDecayL; ataBL += ataCL; ataAL -= ataCL; ataCL = ataBL;}
		ataHalfDiffSampleL = (ataCL * ataDecayL);
		//end antialiasing section for halfway sample L
		ataCR = ataHalfwaySampleR - ataHalfDrySampleR;
		if (ataFlip) {ataAR *= ataDecayR; ataBR *= ataDecayR; ataAR += ataCR; ataBR -= ataCR; ataCR = ataAR;}
		else {ataBR *= ataDecayR; ataAR *= ataDecayR; ataBR += ataCR; ataAR -= ataCR; ataCR = ataBR;}
		ataHalfDiffSampleR = (ataCR * ataDecayR);
		//end antialiasing section for halfway sample R
		
		//begin antialiasing section for raw sample
		ataCL = inputSampleL - ataDrySampleL;
		if (ataFlip) {ataAL *= ataDecayL; ataBL *= ataDecayL; ataAL += ataCL; ataBL -= ataCL; ataCL = ataAL;}
		else {ataBL *= ataDecayL; ataAL *= ataDecayL; ataBL += ataCL; ataAL -= ataCL; ataCL = ataBL;}
		ataDiffSampleL = (ataCL * ataDecayL);
		//end antialiasing section for input sample L
		ataCR = inputSampleR - ataDrySampleR;
		if (ataFlip) {ataAR *= ataDecayR; ataBR *= ataDecayR; ataAR += ataCR; ataBR -= ataCR; ataCR = ataAR;}
		else {ataBR *= ataDecayR; ataAR *= ataDecayR; ataBR += ataCR; ataAR -= ataCR; ataCR = ataBR;}
		ataDiffSampleR = (ataCR * ataDecayR);
		//end antialiasing section for input sample R
		
		ataFlip = !ataFlip;
		
		inputSampleL = ataDrySampleL*outputD; inputSampleL += (ataDiffSampleL + ataHalfDiffSampleL);
		inputSampleR = ataDrySampleR*outputD; inputSampleR += (ataDiffSampleR + ataHalfDiffSampleR);
		//apply processing as difference to non-oversampled raw input
		
		inputSampleL *= outputGlobal;
		inputSampleR *= outputGlobal;
				
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

void MultiBandDistortion::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double iirAmount = pow(A,3)/overallscale;
	double gainH = pow(10.0,(B*48.0)/20);
	
	double thresholdH = D;
	double hardnessH;
	if (thresholdH < 1.0) hardnessH = 1.0 / (1.0-thresholdH);
	else hardnessH = 999999999999999999999.0;
	
	double gainL = pow(10.0,(C*48.0)/20);
	double thresholdL = E;
	double hardnessL;
	if (thresholdL < 1.0) hardnessL = 1.0 / (1.0-thresholdL);
	else hardnessL = 999999999999999999999.0;
	
	double breakup = 1.5707963267949;
	double bridgerectifier;
	
	double outputH = F;
	double outputL = G;
	double outputD = H*0.597;
	double outtrim = outputH + outputL + outputD;
	outputH *= outtrim;
	outputL *= outtrim;
	outputD *= outtrim;
	double outputGlobal = pow(10.0,((I-1.0)*48.0)/20);
	
	double tempHL;
	double tempLL;
	double tempHR;
	double tempLR;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		ataDrySampleL = inputSampleL;
		ataHalfDrySampleL = ataHalfwaySampleL = (inputSampleL + ataLast1SampleL + ((-ataLast2SampleL + ataLast3SampleL) * ataUpsampleHighTweakL)) / 2.0;
		ataLast3SampleL = ataLast2SampleL; ataLast2SampleL = ataLast1SampleL; ataLast1SampleL = inputSampleL;
		//setting up oversampled special antialiasing
		ataDrySampleR = inputSampleR;
		ataHalfDrySampleR = ataHalfwaySampleR = (inputSampleR + ataLast1SampleR + ((-ataLast2SampleR + ataLast3SampleR) * ataUpsampleHighTweakR)) / 2.0;
		ataLast3SampleR = ataLast2SampleR; ataLast2SampleR = ataLast1SampleR; ataLast1SampleR = inputSampleR;
		//setting up oversampled special antialiasing
		
		//pre-center code on inputSample and halfwaySample in parallel
		//begin interpolated sample- change inputSample -> ataHalfwaySample, ataDrySample -> ataHalfDrySample
		tempLL = iirSampleAL = (iirSampleAL * (1 - iirAmount)) + (ataHalfwaySampleL * iirAmount);
		tempHL = ataHalfwaySampleL - iirSampleAL;
		//highpass section
		tempLR = iirSampleAR = (iirSampleAR * (1 - iirAmount)) + (ataHalfwaySampleR * iirAmount);
		tempHR = ataHalfwaySampleR - iirSampleAR;
		//highpass section
		
		
		tempHL *= gainH;
		if (fabs(tempHL) > thresholdH)
		{
			bridgerectifier = (fabs(tempHL)-thresholdH)*hardnessH;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessH;
			//do the sine factor, scale back to proper amount
			if (tempHL > 0) tempHL = bridgerectifier+thresholdH;
			else tempHL = -(bridgerectifier+thresholdH);
		}
		tempHR *= gainH;
		if (fabs(tempHR) > thresholdH)
		{
			bridgerectifier = (fabs(tempHR)-thresholdH)*hardnessH;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessH;
			//do the sine factor, scale back to proper amount
			if (tempHR > 0) tempHR = bridgerectifier+thresholdH;
			else tempHR = -(bridgerectifier+thresholdH);
		}
		
		tempLL *= gainL;
		if (fabs(tempLL) > thresholdL)
		{
			bridgerectifier = (fabs(tempLL)-thresholdL)*hardnessL;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessL;
			//do the sine factor, scale back to proper amount
			if (tempLL > 0) tempLL = bridgerectifier+thresholdL;
			else tempLL = -(bridgerectifier+thresholdL);
		}
		tempLR *= gainL;
		if (fabs(tempLR) > thresholdL)
		{
			bridgerectifier = (fabs(tempLR)-thresholdL)*hardnessL;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessL;
			//do the sine factor, scale back to proper amount
			if (tempLR > 0) tempLR = bridgerectifier+thresholdL;
			else tempLR = -(bridgerectifier+thresholdL);
		}
		
		ataHalfwaySampleL = (tempLL * outputL) + (tempHL * outputH);
		ataHalfwaySampleR = (tempLR * outputL) + (tempHR * outputH);
		//end interpolated sample
		
		//begin raw sample- inputSample and ataDrySample handled separately here
		tempLL = iirSampleBL = (iirSampleBL * (1 - iirAmount)) + (inputSampleL * iirAmount);
		tempHL = inputSampleL - iirSampleBL;
		//highpass section
		tempLR = iirSampleBR = (iirSampleBR * (1 - iirAmount)) + (inputSampleR * iirAmount);
		tempHR = inputSampleR - iirSampleBR;
		//highpass section
		
		tempHL *= gainH;
		if (fabs(tempHL) > thresholdH)
		{
			bridgerectifier = (fabs(tempHL)-thresholdH)*hardnessH;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessH;
			//do the sine factor, scale back to proper amount
			if (tempHL > 0) tempHL = bridgerectifier+thresholdH;
			else tempHL = -(bridgerectifier+thresholdH);
		}
		tempHR *= gainH;
		if (fabs(tempHR) > thresholdH)
		{
			bridgerectifier = (fabs(tempHR)-thresholdH)*hardnessH;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessH;
			//do the sine factor, scale back to proper amount
			if (tempHR > 0) tempHR = bridgerectifier+thresholdH;
			else tempHR = -(bridgerectifier+thresholdH);
		}
		
		tempLL *= gainL;
		if (fabs(tempLL) > thresholdL)
		{
			bridgerectifier = (fabs(tempLL)-thresholdL)*hardnessL;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessL;
			//do the sine factor, scale back to proper amount
			if (tempLL > 0) tempLL = bridgerectifier+thresholdL;
			else tempLL = -(bridgerectifier+thresholdL);
		}
		tempLR *= gainL;
		if (fabs(tempLR) > thresholdL)
		{
			bridgerectifier = (fabs(tempLR)-thresholdL)*hardnessL;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardnessL;
			//do the sine factor, scale back to proper amount
			if (tempLR > 0) tempLR = bridgerectifier+thresholdL;
			else tempLR = -(bridgerectifier+thresholdL);
		}
		
		
		inputSampleL = (tempLL * outputL) + (tempHL * outputH);
		inputSampleR = (tempLR * outputL) + (tempHR * outputH);
		//end raw sample
		
		//begin antialiasing section for halfway sample
		ataCL = ataHalfwaySampleL - ataHalfDrySampleL;
		if (ataFlip) {ataAL *= ataDecayL; ataBL *= ataDecayL; ataAL += ataCL; ataBL -= ataCL; ataCL = ataAL;}
		else {ataBL *= ataDecayL; ataAL *= ataDecayL; ataBL += ataCL; ataAL -= ataCL; ataCL = ataBL;}
		ataHalfDiffSampleL = (ataCL * ataDecayL);
		//end antialiasing section for halfway sample L
		ataCR = ataHalfwaySampleR - ataHalfDrySampleR;
		if (ataFlip) {ataAR *= ataDecayR; ataBR *= ataDecayR; ataAR += ataCR; ataBR -= ataCR; ataCR = ataAR;}
		else {ataBR *= ataDecayR; ataAR *= ataDecayR; ataBR += ataCR; ataAR -= ataCR; ataCR = ataBR;}
		ataHalfDiffSampleR = (ataCR * ataDecayR);
		//end antialiasing section for halfway sample R
		
		//begin antialiasing section for raw sample
		ataCL = inputSampleL - ataDrySampleL;
		if (ataFlip) {ataAL *= ataDecayL; ataBL *= ataDecayL; ataAL += ataCL; ataBL -= ataCL; ataCL = ataAL;}
		else {ataBL *= ataDecayL; ataAL *= ataDecayL; ataBL += ataCL; ataAL -= ataCL; ataCL = ataBL;}
		ataDiffSampleL = (ataCL * ataDecayL);
		//end antialiasing section for input sample L
		ataCR = inputSampleR - ataDrySampleR;
		if (ataFlip) {ataAR *= ataDecayR; ataBR *= ataDecayR; ataAR += ataCR; ataBR -= ataCR; ataCR = ataAR;}
		else {ataBR *= ataDecayR; ataAR *= ataDecayR; ataBR += ataCR; ataAR -= ataCR; ataCR = ataBR;}
		ataDiffSampleR = (ataCR * ataDecayR);
		//end antialiasing section for input sample R
		
		ataFlip = !ataFlip;
		
		inputSampleL = ataDrySampleL*outputD; inputSampleL += (ataDiffSampleL + ataHalfDiffSampleL);
		inputSampleR = ataDrySampleR*outputD; inputSampleR += (ataDiffSampleR + ataHalfDiffSampleR);
		//apply processing as difference to non-oversampled raw input
		
		inputSampleL *= outputGlobal;
		inputSampleR *= outputGlobal;
		
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
