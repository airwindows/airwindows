/* ========================================
 *  PunchyGuitar - PunchyGuitar.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PunchyGuitar_H
#include "PunchyGuitar.h"
#endif

void PunchyGuitar::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double drive = A+0.618033988749894;
	angG[0] = sqrt(B*2.0);
	angG[2] = sqrt(C*2.0);
	angG[4] = sqrt(D*2.0);
	angG[6] = sqrt(E*2.0);
	angG[8] = sqrt(F*2.0);
	angG[1] = (angG[0]+angG[2])*0.5;
	angG[3] = (angG[2]+angG[4])*0.5;
	angG[5] = (angG[4]+angG[6])*0.5;
	angG[7] = (angG[6]+angG[8])*0.5;
	angG[9] = angG[8];
	int poles = (int)(drive*10.0);
	double hFreq = pow(G,overallscale);
	double lFreq = pow(H,overallscale+3.0);
	//begin Gate
	double onthreshold = (pow(I,3)/3)+0.00018;
	double offthreshold = onthreshold * 1.1;	
	double release = 0.028331119964586;
	double absmax = 220.9;
	//end Gate
	double output = J;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//begin Gate
		if (inputSampleL > 0.0)
		{
			if (WasNegativeL == true) ZeroCrossL = absmax * 0.3;
			WasNegativeL = false;
		} else {
			ZeroCrossL += 1; WasNegativeL = true;
		}
		if (ZeroCrossL > absmax) ZeroCrossL = absmax;
		if (gateL == 0.0)
		{
			//if gateL is totally silent
			if (fabs(inputSampleL) > onthreshold)
			{
				if (gaterollerL == 0.0) gaterollerL = ZeroCrossL;
				else gaterollerL -= release;
				// trigger from total silence only- if we're active then signal must clear offthreshold
			}
			else gaterollerL -= release;
		} else {
			//gateL is not silent but closing
			if (fabs(inputSampleL) > offthreshold)
			{
				if (gaterollerL < ZeroCrossL) gaterollerL = ZeroCrossL;
				else gaterollerL -= release;
				//always trigger if gateL is over offthreshold, otherwise close anyway
			}
			else gaterollerL -= release;
		}
		if (gaterollerL < 0.0) gaterollerL = 0.0;
		
		for (int x = 0; x < poles; x++) {
			double fr = 0.9/overallscale;
			double band = inputSampleL; inputSampleL = 0.0;
			for (int y = 0; y < 9; y++) {
				angAL[x][y] = (angAL[x][y]*(1.0-fr)) + ((band-angSL[x][y])*fr);
				double temp = band; band = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSL[x][y] = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleL += ((temp-band)*angG[y]);
				fr *= 0.618033988749894;
			}
			inputSampleL += (band*angG[9]);
			inputSampleL *= drive;
			inputSampleL = sin(fmin(fmax(inputSampleL,-M_PI),M_PI));
		}
		
		if (gaterollerL < 1.0)
		{
			gateL = gaterollerL;
			double bridgerectifier = 1-cos(fabs(inputSampleL));			
			if (inputSampleL > 0) inputSampleL = (inputSampleL*gateL)+(bridgerectifier*(1.0-gateL));
			else inputSampleL = (inputSampleL*gateL)-(bridgerectifier*(1.0-gateL));
			if (gateL == 0.0) inputSampleL = 0.0;			
		} else gateL = 1.0;
		//end Gate		
		
		double lowSample = inputSampleL;
		for(int count = 0; count < (3.0+(lFreq*32.0)); count++) {
			iirBAngleL[count] = (iirBAngleL[count]*(1.0-lFreq))+((lowSample-iirBPositionL[count])*lFreq);
			lowSample = ((iirBPositionL[count]+(iirBAngleL[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			iirBPositionL[count] = ((iirBPositionL[count]+(iirBAngleL[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			inputSampleL -= (lowSample * (1.0/(3.0+(lFreq*32.0))) );
		}
		
		for(int count = 0; count < (3.0+(hFreq*32.0)); count++) {
			iirHAngleL[count] = (iirHAngleL[count]*(1.0-hFreq))+((inputSampleL-iirHPositionL[count])*hFreq);
			inputSampleL = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(inputSampleL*hFreq);
			iirHPositionL[count] = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(inputSampleL*hFreq);
		} //the lowpass	
		inputSampleL *= output;
		
		//begin Gate
		if (inputSampleR > 0.0)
		{
			if (WasNegativeR == true) ZeroCrossR = absmax * 0.3;
			WasNegativeR = false;
		} else {
			ZeroCrossR += 1; WasNegativeR = true;
		}
		if (ZeroCrossR > absmax) ZeroCrossR = absmax;
		if (gateR == 0.0)
		{
			//if gateR is totally silent
			if (fabs(inputSampleR) > onthreshold)
			{
				if (gaterollerR == 0.0) gaterollerR = ZeroCrossR;
				else gaterollerR -= release;
				// trigger from total silence only- if we're active then signal must clear offthreshold
			}
			else gaterollerR -= release;
		} else {
			//gateR is not silent but closing
			if (fabs(inputSampleR) > offthreshold)
			{
				if (gaterollerR < ZeroCrossR) gaterollerR = ZeroCrossR;
				else gaterollerR -= release;
				//always trigger if gateR is over offthreshold, otherwise close anyway
			}
			else gaterollerR -= release;
		}
		if (gaterollerR < 0.0) gaterollerR = 0.0;
		
		for (int x = 0; x < poles; x++) {
			double fr = 0.9/overallscale;
			double band = inputSampleR; inputSampleR = 0.0;
			for (int y = 0; y < 9; y++) {
				angAR[x][y] = (angAR[x][y]*(1.0-fr)) + ((band-angSR[x][y])*fr);
				double temp = band; band = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSR[x][y] = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleR += ((temp-band)*angG[y]);
				fr *= 0.618033988749894;
			}
			inputSampleR += (band*angG[9]);
			inputSampleR *= drive;
			inputSampleR = sin(fmin(fmax(inputSampleR,-M_PI),M_PI));
		}
		
		if (gaterollerR < 1.0)
		{
			gateR = gaterollerR;
			double bridgerectifier = 1-cos(fabs(inputSampleR));			
			if (inputSampleR > 0) inputSampleR = (inputSampleR*gateR)+(bridgerectifier*(1.0-gateR));
			else inputSampleR = (inputSampleR*gateR)-(bridgerectifier*(1.0-gateR));
			if (gateR == 0.0) inputSampleR = 0.0;			
		} else gateR = 1.0;
		//end Gate		
		
		lowSample = inputSampleR;
		for(int count = 0; count < (3.0+(lFreq*32.0)); count++) {
			iirBAngleR[count] = (iirBAngleR[count]*(1.0-lFreq))+((lowSample-iirBPositionR[count])*lFreq);
			lowSample = ((iirBPositionR[count]+(iirBAngleR[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			iirBPositionR[count] = ((iirBPositionR[count]+(iirBAngleR[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			inputSampleR -= (lowSample * (1.0/(3.0+(lFreq*32.0))) );
		}
		
		for(int count = 0; count < (3.0+(hFreq*32.0)); count++) {
			iirHAngleR[count] = (iirHAngleR[count]*(1.0-hFreq))+((inputSampleR-iirHPositionR[count])*hFreq);
			inputSampleR = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(inputSampleR*hFreq);
			iirHPositionR[count] = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(inputSampleR*hFreq);
		} //the lowpass	
		inputSampleR *= output;
		
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

void PunchyGuitar::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double drive = A+0.618033988749894;
	angG[0] = sqrt(B*2.0);
	angG[2] = sqrt(C*2.0);
	angG[4] = sqrt(D*2.0);
	angG[6] = sqrt(E*2.0);
	angG[8] = sqrt(F*2.0);
	angG[1] = (angG[0]+angG[2])*0.5;
	angG[3] = (angG[2]+angG[4])*0.5;
	angG[5] = (angG[4]+angG[6])*0.5;
	angG[7] = (angG[6]+angG[8])*0.5;
	angG[9] = angG[8];
	int poles = (int)(drive*10.0);
	double hFreq = pow(G,overallscale);
	double lFreq = pow(H,overallscale+3.0);
	//begin Gate
	double onthreshold = (pow(I,3)/3)+0.00018;
	double offthreshold = onthreshold * 1.1;	
	double release = 0.028331119964586;
	double absmax = 220.9;
	//end Gate
	double output = J;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//begin Gate
		if (inputSampleL > 0.0)
		{
			if (WasNegativeL == true) ZeroCrossL = absmax * 0.3;
			WasNegativeL = false;
		} else {
			ZeroCrossL += 1; WasNegativeL = true;
		}
		if (ZeroCrossL > absmax) ZeroCrossL = absmax;
		if (gateL == 0.0)
		{
			//if gateL is totally silent
			if (fabs(inputSampleL) > onthreshold)
			{
				if (gaterollerL == 0.0) gaterollerL = ZeroCrossL;
				else gaterollerL -= release;
				// trigger from total silence only- if we're active then signal must clear offthreshold
			}
			else gaterollerL -= release;
		} else {
			//gateL is not silent but closing
			if (fabs(inputSampleL) > offthreshold)
			{
				if (gaterollerL < ZeroCrossL) gaterollerL = ZeroCrossL;
				else gaterollerL -= release;
				//always trigger if gateL is over offthreshold, otherwise close anyway
			}
			else gaterollerL -= release;
		}
		if (gaterollerL < 0.0) gaterollerL = 0.0;
		
		for (int x = 0; x < poles; x++) {
			double fr = 0.9/overallscale;
			double band = inputSampleL; inputSampleL = 0.0;
			for (int y = 0; y < 9; y++) {
				angAL[x][y] = (angAL[x][y]*(1.0-fr)) + ((band-angSL[x][y])*fr);
				double temp = band; band = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSL[x][y] = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleL += ((temp-band)*angG[y]);
				fr *= 0.618033988749894;
			}
			inputSampleL += (band*angG[9]);
			inputSampleL *= drive;
			inputSampleL = sin(fmin(fmax(inputSampleL,-M_PI),M_PI));
		}
		
		if (gaterollerL < 1.0)
		{
			gateL = gaterollerL;
			double bridgerectifier = 1-cos(fabs(inputSampleL));			
			if (inputSampleL > 0) inputSampleL = (inputSampleL*gateL)+(bridgerectifier*(1.0-gateL));
			else inputSampleL = (inputSampleL*gateL)-(bridgerectifier*(1.0-gateL));
			if (gateL == 0.0) inputSampleL = 0.0;			
		} else gateL = 1.0;
		//end Gate		
		
		double lowSample = inputSampleL;
		for(int count = 0; count < (3.0+(lFreq*32.0)); count++) {
			iirBAngleL[count] = (iirBAngleL[count]*(1.0-lFreq))+((lowSample-iirBPositionL[count])*lFreq);
			lowSample = ((iirBPositionL[count]+(iirBAngleL[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			iirBPositionL[count] = ((iirBPositionL[count]+(iirBAngleL[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			inputSampleL -= (lowSample * (1.0/(3.0+(lFreq*32.0))) );
		}
		
		for(int count = 0; count < (3.0+(hFreq*32.0)); count++) {
			iirHAngleL[count] = (iirHAngleL[count]*(1.0-hFreq))+((inputSampleL-iirHPositionL[count])*hFreq);
			inputSampleL = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(inputSampleL*hFreq);
			iirHPositionL[count] = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(inputSampleL*hFreq);
		} //the lowpass	
		inputSampleL *= output;
		
		//begin Gate
		if (inputSampleR > 0.0)
		{
			if (WasNegativeR == true) ZeroCrossR = absmax * 0.3;
			WasNegativeR = false;
		} else {
			ZeroCrossR += 1; WasNegativeR = true;
		}
		if (ZeroCrossR > absmax) ZeroCrossR = absmax;
		if (gateR == 0.0)
		{
			//if gateR is totally silent
			if (fabs(inputSampleR) > onthreshold)
			{
				if (gaterollerR == 0.0) gaterollerR = ZeroCrossR;
				else gaterollerR -= release;
				// trigger from total silence only- if we're active then signal must clear offthreshold
			}
			else gaterollerR -= release;
		} else {
			//gateR is not silent but closing
			if (fabs(inputSampleR) > offthreshold)
			{
				if (gaterollerR < ZeroCrossR) gaterollerR = ZeroCrossR;
				else gaterollerR -= release;
				//always trigger if gateR is over offthreshold, otherwise close anyway
			}
			else gaterollerR -= release;
		}
		if (gaterollerR < 0.0) gaterollerR = 0.0;
		
		for (int x = 0; x < poles; x++) {
			double fr = 0.9/overallscale;
			double band = inputSampleR; inputSampleR = 0.0;
			for (int y = 0; y < 9; y++) {
				angAR[x][y] = (angAR[x][y]*(1.0-fr)) + ((band-angSR[x][y])*fr);
				double temp = band; band = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSR[x][y] = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleR += ((temp-band)*angG[y]);
				fr *= 0.618033988749894;
			}
			inputSampleR += (band*angG[9]);
			inputSampleR *= drive;
			inputSampleR = sin(fmin(fmax(inputSampleR,-M_PI),M_PI));
		}
		
		if (gaterollerR < 1.0)
		{
			gateR = gaterollerR;
			double bridgerectifier = 1-cos(fabs(inputSampleR));			
			if (inputSampleR > 0) inputSampleR = (inputSampleR*gateR)+(bridgerectifier*(1.0-gateR));
			else inputSampleR = (inputSampleR*gateR)-(bridgerectifier*(1.0-gateR));
			if (gateR == 0.0) inputSampleR = 0.0;			
		} else gateR = 1.0;
		//end Gate		
		
		lowSample = inputSampleR;
		for(int count = 0; count < (3.0+(lFreq*32.0)); count++) {
			iirBAngleR[count] = (iirBAngleR[count]*(1.0-lFreq))+((lowSample-iirBPositionR[count])*lFreq);
			lowSample = ((iirBPositionR[count]+(iirBAngleR[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			iirBPositionR[count] = ((iirBPositionR[count]+(iirBAngleR[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			inputSampleR -= (lowSample * (1.0/(3.0+(lFreq*32.0))) );
		}
		
		for(int count = 0; count < (3.0+(hFreq*32.0)); count++) {
			iirHAngleR[count] = (iirHAngleR[count]*(1.0-hFreq))+((inputSampleR-iirHPositionR[count])*hFreq);
			inputSampleR = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(inputSampleR*hFreq);
			iirHPositionR[count] = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(inputSampleR*hFreq);
		} //the lowpass	
		inputSampleR *= output;
		
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
