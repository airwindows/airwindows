/* ========================================
 *  Ensemble - Ensemble.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Ensemble_H
#include "Ensemble.h"
#endif

void Ensemble::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double spd = pow(0.4+(B/12),10);
	spd *= overallscale;
	double depth = 0.002 / spd;
	double tupi = 3.141592653589793238 * 2.0;
	double taps = floor((A*46.0)+2.9);
	double brighten = C;
	double wet = D;
	//removed extra dry variable
	double hapi = 3.141592653589793238 / taps;
	double offset;
	double floffset;
	double start[49];
	double sinoffset[49];	
	double speed[49];
	int count;
	int ensemble;
	double tempL;
	double tempR;
	

	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	//now we'll precalculate some stuff that needn't be in every sample
	
	for(count = 1; count <= taps; count++)
	{
		start[count] = depth * count;
		sinoffset[count] = hapi * (count-1);
		speed[count] = spd / (1 + (count/taps));
	}
	//that's for speeding up things in the sample-processing area
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;

		airFactorL = airPrevL - inputSampleL;
		if (fpFlip) {airEvenL += airFactorL; airOddL -= airFactorL; airFactorL = airEvenL;}
		else {airOddL += airFactorL; airEvenL -= airFactorL; airFactorL = airOddL;}
		airOddL = (airOddL - ((airOddL - airEvenL)/256.0)) / 1.0001;
		airEvenL = (airEvenL - ((airEvenL - airOddL)/256.0)) / 1.0001;
		airPrevL = inputSampleL;
		inputSampleL += (airFactorL*brighten);
		//air, compensates for loss of highs in flanger's interpolation

		airFactorR = airPrevR - inputSampleR;
		if (fpFlip) {airEvenR += airFactorR; airOddR -= airFactorR; airFactorR = airEvenR;}
		else {airOddR += airFactorR; airEvenR -= airFactorR; airFactorR = airOddR;}
		airOddR = (airOddR - ((airOddR - airEvenR)/256.0)) / 1.0001;
		airEvenR = (airEvenR - ((airEvenR - airOddR)/256.0)) / 1.0001;
		airPrevR = inputSampleR;
		inputSampleR += (airFactorR*brighten);
		//air, compensates for loss of highs in flanger's interpolation
		fpFlip = !fpFlip;
	
		if (gcount < 1 || gcount > 32767) {gcount = 32767;}
		count = gcount;
		dL[count+32767] = dL[count] = tempL = inputSampleL;
		dR[count+32767] = dR[count] = tempR = inputSampleR;
		//double buffer
		
		for(ensemble = 1; ensemble <= taps; ensemble++)
		{
			offset = start[ensemble] + (depth * sin(sweep[ensemble]+sinoffset[ensemble]));
			floffset = offset-floor(offset);
			count = gcount + (int)floor(offset);
			
			tempL += dL[count] * (1-floffset); //less as value moves away from .0
			tempL += dL[count+1]; //we can assume always using this in one way or another?
			tempL += dL[count+2] * floffset; //greater as value moves away from .0
			tempL -= ((dL[count]-dL[count+1])-(dL[count+1]-dL[count+2]))/50; //interpolation hacks 'r us
			
			tempR += dR[count] * (1-floffset); //less as value moves away from .0
			tempR += dR[count+1]; //we can assume always using this in one way or another?
			tempR += dR[count+2] * floffset; //greater as value moves away from .0
			tempR -= ((dR[count]-dR[count+1])-(dR[count+1]-dR[count+2]))/50; //interpolation hacks 'r us
			
			sweep[ensemble] += speed[ensemble];
			if (sweep[ensemble] > tupi){sweep[ensemble] -= tupi;}
		}
		gcount--;
		//still scrolling through the samples, remember
				
		inputSampleL = tempL/(4.0*sqrt(taps));
		inputSampleR = tempR/(4.0*sqrt(taps));
		

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

void Ensemble::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double spd = pow(0.4+(B/12),10);
	spd *= overallscale;
	double depth = 0.002 / spd;
	double tupi = 3.141592653589793238 * 2.0;
	double taps = floor((A*46.0)+2.9);
	double brighten = C;
	double wet = D;
	//removed extra dry variable
	double hapi = 3.141592653589793238 / taps;
	double offset;
	double floffset;
	double start[49];
	double sinoffset[49];	
	double speed[49];
	int count;
	int ensemble;
	double tempL;
	double tempR;
	

	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	//now we'll precalculate some stuff that needn't be in every sample
	
	for(count = 1; count <= taps; count++)
	{
		start[count] = depth * count;
		sinoffset[count] = hapi * (count-1);
		speed[count] = spd / (1 + (count/taps));
	}
	//that's for speeding up things in the sample-processing area
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		airFactorL = airPrevL - inputSampleL;
		if (fpFlip) {airEvenL += airFactorL; airOddL -= airFactorL; airFactorL = airEvenL;}
		else {airOddL += airFactorL; airEvenL -= airFactorL; airFactorL = airOddL;}
		airOddL = (airOddL - ((airOddL - airEvenL)/256.0)) / 1.0001;
		airEvenL = (airEvenL - ((airEvenL - airOddL)/256.0)) / 1.0001;
		airPrevL = inputSampleL;
		inputSampleL += (airFactorL*brighten);
		//air, compensates for loss of highs in flanger's interpolation
		
		airFactorR = airPrevR - inputSampleR;
		if (fpFlip) {airEvenR += airFactorR; airOddR -= airFactorR; airFactorR = airEvenR;}
		else {airOddR += airFactorR; airEvenR -= airFactorR; airFactorR = airOddR;}
		airOddR = (airOddR - ((airOddR - airEvenR)/256.0)) / 1.0001;
		airEvenR = (airEvenR - ((airEvenR - airOddR)/256.0)) / 1.0001;
		airPrevR = inputSampleR;
		inputSampleR += (airFactorR*brighten);
		//air, compensates for loss of highs in flanger's interpolation
		fpFlip = !fpFlip;
		
		if (gcount < 1 || gcount > 32767) {gcount = 32767;}
		count = gcount;
		dL[count+32767] = dL[count] = tempL = inputSampleL;
		dR[count+32767] = dR[count] = tempR = inputSampleR;
		//double buffer
		
		for(ensemble = 1; ensemble <= taps; ensemble++)
		{
			offset = start[ensemble] + (depth * sin(sweep[ensemble]+sinoffset[ensemble]));
			floffset = offset-floor(offset);
			count = gcount + (int)floor(offset);
			
			tempL += dL[count] * (1-floffset); //less as value moves away from .0
			tempL += dL[count+1]; //we can assume always using this in one way or another?
			tempL += dL[count+2] * floffset; //greater as value moves away from .0
			tempL -= ((dL[count]-dL[count+1])-(dL[count+1]-dL[count+2]))/50; //interpolation hacks 'r us
			
			tempR += dR[count] * (1-floffset); //less as value moves away from .0
			tempR += dR[count+1]; //we can assume always using this in one way or another?
			tempR += dR[count+2] * floffset; //greater as value moves away from .0
			tempR -= ((dR[count]-dR[count+1])-(dR[count+1]-dR[count+2]))/50; //interpolation hacks 'r us
			
			sweep[ensemble] += speed[ensemble];
			if (sweep[ensemble] > tupi){sweep[ensemble] -= tupi;}
		}
		gcount--;
		//still scrolling through the samples, remember
		
		inputSampleL = tempL/(4.0*sqrt(taps));
		inputSampleR = tempR/(4.0*sqrt(taps));
		
		
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