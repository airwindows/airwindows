/* ========================================
 *  Doublelay - Doublelay.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Doublelay_H
#include "Doublelay.h"
#endif

void Doublelay::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check
	double delayTrim = (getSampleRate()/cycleEnd)/48001.0; //this gives us a time adjustment
	if (delayTrim > 0.99999) delayTrim = 0.99999; //sanity check so we don't smash our delay buffer
	if (delayTrim < 0.0) delayTrim = 0.0; //sanity check so we don't smash our delay buffer
	
	double trim = (A*2.0)-1.0;
	trim *= fabs(trim);
	trim /= 40;
	double speedL = trim+1.0;
	double speedR = (-trim)+1.0;
	if (speedL < 0.0) speedL = 0.0;
	if (speedR < 0.0) speedR = 0.0;
	
	int delayL = (B*(int)(48000.0*delayTrim));
	int delayR = (C*(int)(48000.0*delayTrim));
	//this now adjusts to give exactly one second max delay at all times up to 48k
	//or multipliers of it using undersampling
	
	double adjust = 1100;
	int width = 2300;
	if (prevwidth != width)
	{
		positionL[0] = 0;
		positionL[1] = (int)(width/3);
		positionL[2] = (int)((width/3)*2);
		positionL[3] = (int)(width/5);
		positionL[4] = (int)((width/5)*2);
		positionL[5] = (int)((width/5)*3);
		positionL[6] = (int)((width/5)*4);
		positionL[7] = (int)(width/2);
		positionR[0] = 0;
		positionR[1] = (int)(width/3);
		positionR[2] = (int)((width/3)*2);
		positionR[3] = (int)(width/5);
		positionR[4] = (int)((width/5)*2);
		positionR[5] = (int)((width/5)*3);
		positionR[6] = (int)((width/5)*4);
		positionR[7] = (int)(width/2);
		prevwidth = width;
	}
	double feedbackDirect = D * 0.618033988749894848204586;
	double feedbackCross = D * (1.0-0.618033988749894848204586);
	double wet = E;
	int gplusL;
	int lastplusL;
	int gplusR;
	int lastplusR;
	double posplusL;
	double lastposplusL;
	double posplusR;
	double lastposplusR;
	double depth;
	double crossfade;
	int count;
	int bcountL;
	int bcountR;
	int base;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a doubler sample
			double drySampleL = inputSampleL;
			double drySampleR = inputSampleR;
			
			//assign working variables
			airFactorL = airPrevL - inputSampleL;
			if (flip) {airEvenL += airFactorL; airOddL -= airFactorL; airFactorL = airEvenL;}
			else {airOddL += airFactorL; airEvenL -= airFactorL; airFactorL = airOddL;}
			airOddL = (airOddL - ((airOddL - airEvenL)/256.0)) / 1.0001;
			airEvenL = (airEvenL - ((airEvenL - airOddL)/256.0)) / 1.0001;
			airPrevL = inputSampleL;
			inputSampleL += airFactorL;
			//left
			airFactorR = airPrevR - inputSampleR;
			if (flip) {airEvenR += airFactorR; airOddR -= airFactorR; airFactorR = airEvenR;}
			else {airOddR += airFactorR; airEvenR -= airFactorR; airFactorR = airOddR;}
			airOddR = (airOddR - ((airOddR - airEvenR)/256.0)) / 1.0001;
			airEvenR = (airEvenR - ((airEvenR - airOddR)/256.0)) / 1.0001;
			airPrevR = inputSampleR;
			inputSampleR += airFactorR;
			//right
			flip = !flip;
			//air, compensates for loss of highs in flanger's interpolation
			
			inputSampleL += feedbackL*feedbackDirect;
			inputSampleR += feedbackR*feedbackDirect;
			inputSampleL += feedbackR*feedbackCross;
			inputSampleR += feedbackL*feedbackCross;
			
			if (dcount < 1 || dcount > 48005) dcount = 48005;
			count = dcount;
			
			dL[count] = inputSampleL;
			dR[count] = inputSampleR;
			//double buffer
			
			inputSampleL = dL[count+delayL-((count+delayL>48005)?48005:0)];
			inputSampleR = dR[count+delayR-((count+delayR>48005)?48005:0)];
			//assign delays
			
			dcount--;
			gcountL++;
			gcountR++;
			for(count = 0; count < 8; count++) {positionL[count] += speedL; positionR[count] += speedR;}
			
			gplusL = gcountL+(int)adjust;
			lastplusL = lastcountL+(int)adjust;
			if (gplusL > width) {gplusL -= width;}
			if (lastplusL > width) {lastplusL -= width;}
			
			gplusR = gcountR+(int)adjust;
			lastplusR = lastcountR+(int)adjust;
			if (gplusR > width) {gplusR -= width;}
			if (lastplusR > width) {lastplusR -= width;}
			
			if (trackingL[activeL] == 0.0)
			{
				posplusL = positionL[activeL]+adjust;
				lastposplusL = lastpositionL[activeL]+adjust;
				if (posplusL > width) {posplusL -= width;}
				if (lastposplusL > width) {lastposplusL -= width;}
				if ((gplusL > positionL[activeL]) && (lastplusL < lastpositionL[activeL])) {trackingL[activeL] = 1.0;}
				if ((posplusL > gcountL) && (lastposplusL < lastcountL)) {trackingL[activeL] = 1.0;}
				//fire splice based on whether somebody moved past somebody else just now
			}
			
			if (trackingR[activeR] == 0.0)
			{
				posplusR = positionR[activeR]+adjust;
				lastposplusR = lastpositionR[activeR]+adjust;
				if (posplusR > width) {posplusR -= width;}
				if (lastposplusR > width) {lastposplusR -= width;}
				if ((gplusR > positionR[activeR]) && (lastplusR < lastpositionR[activeR])) {trackingR[activeR] = 1.0;}
				if ((posplusR > gcountR) && (lastposplusR < lastcountR)) {trackingR[activeR] = 1.0;}
				//fire splice based on whether somebody moved past somebody else just now
			}
			
			for(count = 0; count < 8; count++) 
			{
				if (positionL[count] > width) {positionL[count] -= width;}
				if (positionR[count] > width) {positionR[count] -= width;}
				lastpositionL[count] = positionL[count];
				lastpositionR[count] = positionR[count];
			}
			if (gcountL < 0 || gcountL > width) {gcountL -= width;}
			lastcountL = bcountL = gcountL;
			if (gcountR < 0 || gcountR > width) {gcountR -= width;}
			lastcountR = bcountR = gcountR;
			
			pL[bcountL+width] = pL[bcountL] = inputSampleL;
			pR[bcountR+width] = pR[bcountR] = inputSampleR;
			
			
			for(count = 0; count < 8; count++)
			{
				base = (int)floor(positionL[count]);
				tempL[count] = (pL[base] * (1-(positionL[count]-base))); //less as value moves away from .0
				tempL[count] += pL[base+1]; //we can assume always using this in one way or another?
				tempL[count] += (pL[base+2] * (positionL[count]-base)); //greater as value moves away from .0
				tempL[count] -= (((pL[base]-pL[base+1])-(pL[base+1]-pL[base+2]))/50); //interpolation hacks 'r us		
				tempL[count] /= 2;
				
				base = (int)floor(positionR[count]);
				tempR[count] = (pR[base] * (1-(positionR[count]-base))); //less as value moves away from .0
				tempR[count] += pR[base+1]; //we can assume always using this in one way or another?
				tempR[count] += (pR[base+2] * (positionR[count]-base)); //greater as value moves away from .0
				tempR[count] -= (((pR[base]-pR[base+1])-(pR[base+1]-pR[base+2]))/50); //interpolation hacks 'r us		
				tempR[count] /= 2;
			}
			
			
			if (trackingL[activeL] > 0.0)
			{
				crossfade = sin(trackingL[bestspliceL]*1.57);
				inputSampleL = (tempL[activeL]*crossfade)+(tempL[bestspliceL]*(1.0-crossfade));
				
				for(count = 0; count < 8; count++)
				{
					depth = (0.5-fabs(tempL[activeL]-tempL[count]));
					if ((depth > 0) && (count != activeL))
					{trackingL[count] -= (depth/adjust);
						bestspliceL = count;}
					//take down the splicings but skip the current one
				}
				bestyetL = 1.0;
				for(count = 0; count < 8; count++)
				{
					if ((trackingL[count] < bestyetL)&&(count != activeL))
					{bestspliceL = count; bestyetL = trackingL[count];}
				}
				
				if (trackingL[bestspliceL] < 0.0)
				{
					for(count = 0; count < 8; count++)
					{trackingL[count] = 1.0;}
					activeL = bestspliceL;
					trackingL[activeL] = 0.0;
				}
			}
			else inputSampleL = tempL[activeL];
			
			if (trackingR[activeR] > 0.0)
			{
				crossfade = sin(trackingR[bestspliceR]*1.57);
				inputSampleR = (tempR[activeR]*crossfade)+(tempR[bestspliceR]*(1.0-crossfade));
				
				for(count = 0; count < 8; count++)
				{
					depth = (0.5-fabs(tempR[activeR]-tempR[count]));
					if ((depth > 0) && (count != activeR))
					{trackingR[count] -= (depth/adjust); bestspliceR = count;}
					//take down the splicings but skip the current one
				}
				bestyetR = 1.0;
				for(count = 0; count < 8; count++)
				{
					if ((trackingR[count] < bestyetR)&&(count != activeR))
					{bestspliceR = count; bestyetR = trackingR[count];}
				}
				
				if (trackingR[bestspliceR] < 0.0)
				{
					for(count = 0; count < 8; count++)
					{trackingR[count] = 1.0;}
					activeR = bestspliceR;
					trackingR[activeR] = 0.0;
				}					
			}
			else inputSampleR = tempR[activeR];
			
			feedbackL = inputSampleL;
			feedbackR = inputSampleR;
			//feedback section
			
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
			
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
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
		
		in1++;
		in2++;
		out1++;
		out2++;
	}
}

void Doublelay::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check
	double delayTrim = (getSampleRate()/cycleEnd)/48001.0; //this gives us a time adjustment
	if (delayTrim > 0.99999) delayTrim = 0.99999; //sanity check so we don't smash our delay buffer
	if (delayTrim < 0.0) delayTrim = 0.0; //sanity check so we don't smash our delay buffer
	
	double trim = (A*2.0)-1.0;
	trim *= fabs(trim);
	trim /= 40;
	double speedL = trim+1.0;
	double speedR = (-trim)+1.0;
	if (speedL < 0.0) speedL = 0.0;
	if (speedR < 0.0) speedR = 0.0;
	
	int delayL = (B*(int)(48000.0*delayTrim));
	int delayR = (C*(int)(48000.0*delayTrim));
	//this now adjusts to give exactly one second max delay at all times up to 48k
	//or multipliers of it using undersampling
	
	double adjust = 1100;
	int width = 2300;
	if (prevwidth != width)
	{
		positionL[0] = 0;
		positionL[1] = (int)(width/3);
		positionL[2] = (int)((width/3)*2);
		positionL[3] = (int)(width/5);
		positionL[4] = (int)((width/5)*2);
		positionL[5] = (int)((width/5)*3);
		positionL[6] = (int)((width/5)*4);
		positionL[7] = (int)(width/2);
		positionR[0] = 0;
		positionR[1] = (int)(width/3);
		positionR[2] = (int)((width/3)*2);
		positionR[3] = (int)(width/5);
		positionR[4] = (int)((width/5)*2);
		positionR[5] = (int)((width/5)*3);
		positionR[6] = (int)((width/5)*4);
		positionR[7] = (int)(width/2);
		prevwidth = width;
	}
	double feedbackDirect = D * 0.618033988749894848204586;
	double feedbackCross = D * (1.0-0.618033988749894848204586);
	double wet = E;
	int gplusL;
	int lastplusL;
	int gplusR;
	int lastplusR;
	double posplusL;
	double lastposplusL;
	double posplusR;
	double lastposplusR;
	double depth;
	double crossfade;
	int count;
	int bcountL;
	int bcountR;
	int base;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a doubler sample
			double drySampleL = inputSampleL;
			double drySampleR = inputSampleR;
			
			//assign working variables
			airFactorL = airPrevL - inputSampleL;
			if (flip) {airEvenL += airFactorL; airOddL -= airFactorL; airFactorL = airEvenL;}
			else {airOddL += airFactorL; airEvenL -= airFactorL; airFactorL = airOddL;}
			airOddL = (airOddL - ((airOddL - airEvenL)/256.0)) / 1.0001;
			airEvenL = (airEvenL - ((airEvenL - airOddL)/256.0)) / 1.0001;
			airPrevL = inputSampleL;
			inputSampleL += airFactorL;
			//left
			airFactorR = airPrevR - inputSampleR;
			if (flip) {airEvenR += airFactorR; airOddR -= airFactorR; airFactorR = airEvenR;}
			else {airOddR += airFactorR; airEvenR -= airFactorR; airFactorR = airOddR;}
			airOddR = (airOddR - ((airOddR - airEvenR)/256.0)) / 1.0001;
			airEvenR = (airEvenR - ((airEvenR - airOddR)/256.0)) / 1.0001;
			airPrevR = inputSampleR;
			inputSampleR += airFactorR;
			//right
			flip = !flip;
			//air, compensates for loss of highs in flanger's interpolation
			
			inputSampleL += feedbackL*feedbackDirect;
			inputSampleR += feedbackR*feedbackDirect;
			inputSampleL += feedbackR*feedbackCross;
			inputSampleR += feedbackL*feedbackCross;
			
			if (dcount < 1 || dcount > 48005) dcount = 48005;
			count = dcount;
			
			dL[count] = inputSampleL;
			dR[count] = inputSampleR;
			//double buffer
			
			inputSampleL = dL[count+delayL-((count+delayL>48005)?48005:0)];
			inputSampleR = dR[count+delayR-((count+delayR>48005)?48005:0)];
			//assign delays
			
			dcount--;
			gcountL++;
			gcountR++;
			for(count = 0; count < 8; count++) {positionL[count] += speedL; positionR[count] += speedR;}
			
			gplusL = gcountL+(int)adjust;
			lastplusL = lastcountL+(int)adjust;
			if (gplusL > width) {gplusL -= width;}
			if (lastplusL > width) {lastplusL -= width;}
			
			gplusR = gcountR+(int)adjust;
			lastplusR = lastcountR+(int)adjust;
			if (gplusR > width) {gplusR -= width;}
			if (lastplusR > width) {lastplusR -= width;}
			
			if (trackingL[activeL] == 0.0)
			{
				posplusL = positionL[activeL]+adjust;
				lastposplusL = lastpositionL[activeL]+adjust;
				if (posplusL > width) {posplusL -= width;}
				if (lastposplusL > width) {lastposplusL -= width;}
				if ((gplusL > positionL[activeL]) && (lastplusL < lastpositionL[activeL])) {trackingL[activeL] = 1.0;}
				if ((posplusL > gcountL) && (lastposplusL < lastcountL)) {trackingL[activeL] = 1.0;}
				//fire splice based on whether somebody moved past somebody else just now
			}
			
			if (trackingR[activeR] == 0.0)
			{
				posplusR = positionR[activeR]+adjust;
				lastposplusR = lastpositionR[activeR]+adjust;
				if (posplusR > width) {posplusR -= width;}
				if (lastposplusR > width) {lastposplusR -= width;}
				if ((gplusR > positionR[activeR]) && (lastplusR < lastpositionR[activeR])) {trackingR[activeR] = 1.0;}
				if ((posplusR > gcountR) && (lastposplusR < lastcountR)) {trackingR[activeR] = 1.0;}
				//fire splice based on whether somebody moved past somebody else just now
			}
			
			for(count = 0; count < 8; count++) 
			{
				if (positionL[count] > width) {positionL[count] -= width;}
				if (positionR[count] > width) {positionR[count] -= width;}
				lastpositionL[count] = positionL[count];
				lastpositionR[count] = positionR[count];
			}
			if (gcountL < 0 || gcountL > width) {gcountL -= width;}
			lastcountL = bcountL = gcountL;
			if (gcountR < 0 || gcountR > width) {gcountR -= width;}
			lastcountR = bcountR = gcountR;
			
			pL[bcountL+width] = pL[bcountL] = inputSampleL;
			pR[bcountR+width] = pR[bcountR] = inputSampleR;
			
			
			for(count = 0; count < 8; count++)
			{
				base = (int)floor(positionL[count]);
				tempL[count] = (pL[base] * (1-(positionL[count]-base))); //less as value moves away from .0
				tempL[count] += pL[base+1]; //we can assume always using this in one way or another?
				tempL[count] += (pL[base+2] * (positionL[count]-base)); //greater as value moves away from .0
				tempL[count] -= (((pL[base]-pL[base+1])-(pL[base+1]-pL[base+2]))/50); //interpolation hacks 'r us		
				tempL[count] /= 2;
				
				base = (int)floor(positionR[count]);
				tempR[count] = (pR[base] * (1-(positionR[count]-base))); //less as value moves away from .0
				tempR[count] += pR[base+1]; //we can assume always using this in one way or another?
				tempR[count] += (pR[base+2] * (positionR[count]-base)); //greater as value moves away from .0
				tempR[count] -= (((pR[base]-pR[base+1])-(pR[base+1]-pR[base+2]))/50); //interpolation hacks 'r us		
				tempR[count] /= 2;
			}
			
			
			if (trackingL[activeL] > 0.0)
			{
				crossfade = sin(trackingL[bestspliceL]*1.57);
				inputSampleL = (tempL[activeL]*crossfade)+(tempL[bestspliceL]*(1.0-crossfade));
				
				for(count = 0; count < 8; count++)
				{
					depth = (0.5-fabs(tempL[activeL]-tempL[count]));
					if ((depth > 0) && (count != activeL))
					{trackingL[count] -= (depth/adjust);
						bestspliceL = count;}
					//take down the splicings but skip the current one
				}
				bestyetL = 1.0;
				for(count = 0; count < 8; count++)
				{
					if ((trackingL[count] < bestyetL)&&(count != activeL))
					{bestspliceL = count; bestyetL = trackingL[count];}
				}
				
				if (trackingL[bestspliceL] < 0.0)
				{
					for(count = 0; count < 8; count++)
					{trackingL[count] = 1.0;}
					activeL = bestspliceL;
					trackingL[activeL] = 0.0;
				}
			}
			else inputSampleL = tempL[activeL];
			
			if (trackingR[activeR] > 0.0)
			{
				crossfade = sin(trackingR[bestspliceR]*1.57);
				inputSampleR = (tempR[activeR]*crossfade)+(tempR[bestspliceR]*(1.0-crossfade));
				
				for(count = 0; count < 8; count++)
				{
					depth = (0.5-fabs(tempR[activeR]-tempR[count]));
					if ((depth > 0) && (count != activeR))
					{trackingR[count] -= (depth/adjust); bestspliceR = count;}
					//take down the splicings but skip the current one
				}
				bestyetR = 1.0;
				for(count = 0; count < 8; count++)
				{
					if ((trackingR[count] < bestyetR)&&(count != activeR))
					{bestspliceR = count; bestyetR = trackingR[count];}
				}
				
				if (trackingR[bestspliceR] < 0.0)
				{
					for(count = 0; count < 8; count++)
					{trackingR[count] = 1.0;}
					activeR = bestspliceR;
					trackingR[activeR] = 0.0;
				}					
			}
			else inputSampleR = tempR[activeR];
			
			feedbackL = inputSampleL;
			feedbackR = inputSampleR;
			//feedback section
			
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
			
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
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
		
		in1++;
		in2++;
		out1++;
		out2++;
	}
}
