/* ========================================
 *  CStrip2 - CStrip2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __CStrip2_H
#include "CStrip2.h"
#endif

void CStrip2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	double compscale = overallscale;
	overallscale = getSampleRate();
	compscale = compscale * overallscale;
	//compscale is the one that's 1 or something like 2.2 for 96K rates
	double fpOld = 0.618033988749894848204586; //golden ratio!
	double fpNew = 1.0 - fpOld;
	
	double highSampleL = 0.0;
	double midSampleL = 0.0;
	double bassSampleL = 0.0;
	
	double highSampleR = 0.0;
	double midSampleR = 0.0;
	double bassSampleR = 0.0;
	
	double densityA = (A*12.0)-6.0;
	double densityB = (B*12.0)-6.0;
	double densityC = (C*12.0)-6.0;
	bool engageEQ = true;
	if ( (0.0 == densityA) && (0.0 == densityB) && (0.0 == densityC) ) engageEQ = false;
	
	densityA = pow(10.0,densityA/20.0)-1.0;
	densityB = pow(10.0,densityB/20.0)-1.0;
	densityC = pow(10.0,densityC/20.0)-1.0;
	//convert to 0 to X multiplier with 1.0 being O db
	//minus one gives nearly -1 to ? (should top out at 1)
	//calibrate so that X db roughly equals X db with maximum topping out at 1 internally
	double tripletIntensity = -densityA;
	double iirAmountA = (((D*D*15.0)+1.0)*1000)/overallscale;
	double iirAmountB = (((E*E*1570.0)+30.0)*10)/overallscale;
	double bridgerectifier;
	double outA = fabs(densityA);
	double outB = fabs(densityB);
	double outC = fabs(densityC);
	//end EQ
	
	double lowpassAmount = pow(F,2);
	double highpassAmount = pow(G,2);
	bool engageCapacitor = false;
	if ((lowpassAmount < 1.0) || (highpassAmount > 0.0)) engageCapacitor = true;
	//end Capacitor
	
	//begin ButterComp
	double inputpos;
	double inputneg;
	double calcpos;
	double calcneg;
	double outputpos;
	double outputneg;
	double totalmultiplier;
	double inputgain = (pow(H,4)*35)+1.0;
	double compoutgain = inputgain;
	compoutgain -= 1.0;
	compoutgain /= 1.2;
	compoutgain += 1.0;
	double divisor = (0.008 * pow(I,2))+0.0004;
	//originally 0.012
	divisor /= compscale;
	double remainder = divisor;
	divisor = 1.0 - divisor;
	bool engageComp = false;
	if (inputgain > 1.0) engageComp = true;
	//end ButterComp

	double outputgain = J*3.0; //0-2
	double density = outputgain-1.0; //allow for output 0-1 to be clean, 1-3 all boosts
	if (density < 0.0) density = 0.0;
	double phattity = density - 1.0;
	if (density > 1.0) density = 1.0; //max out at full wet for Spiral aspect
	if (phattity < 0.0) phattity = 0.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (engageCapacitor)
		{
			count++; if (count > 5) count = 0;
			switch (count)
			{
				case 0:
					iirHighpassAL = (iirHighpassAL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
					iirLowpassAL = (iirLowpassAL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
					iirHighpassBL = (iirHighpassBL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassBL;
					iirLowpassBL = (iirLowpassBL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassBL;
					iirHighpassDL = (iirHighpassDL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassDL;
					iirLowpassDL = (iirLowpassDL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassDL;
					iirHighpassAR = (iirHighpassAR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
					iirLowpassAR = (iirLowpassAR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
					iirHighpassBR = (iirHighpassBR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassBR;
					iirLowpassBR = (iirLowpassBR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassBR;
					iirHighpassDR = (iirHighpassDR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassDR;
					iirLowpassDR = (iirLowpassDR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassDR;
					break;
				case 1:
					iirHighpassAL = (iirHighpassAL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
					iirLowpassAL = (iirLowpassAL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
					iirHighpassCL = (iirHighpassCL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassCL;
					iirLowpassCL = (iirLowpassCL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassCL;
					iirHighpassEL = (iirHighpassEL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassEL;
					iirLowpassEL = (iirLowpassEL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassEL;
					iirHighpassAR = (iirHighpassAR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
					iirLowpassAR = (iirLowpassAR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
					iirHighpassCR = (iirHighpassCR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassCR;
					iirLowpassCR = (iirLowpassCR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassCR;
					iirHighpassER = (iirHighpassER * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassER;
					iirLowpassER = (iirLowpassER * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassER;
					break;
				case 2:
					iirHighpassAL = (iirHighpassAL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
					iirLowpassAL = (iirLowpassAL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
					iirHighpassBL = (iirHighpassBL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassBL;
					iirLowpassBL = (iirLowpassBL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassBL;
					iirHighpassFL = (iirHighpassFL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassFL;
					iirLowpassFL = (iirLowpassFL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassFL;
					iirHighpassAR = (iirHighpassAR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
					iirLowpassAR = (iirLowpassAR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
					iirHighpassBR = (iirHighpassBR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassBR;
					iirLowpassBR = (iirLowpassBR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassBR;
					iirHighpassFR = (iirHighpassFR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassFR;
					iirLowpassFR = (iirLowpassFR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassFR;
					break;
				case 3:
					iirHighpassAL = (iirHighpassAL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
					iirLowpassAL = (iirLowpassAL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
					iirHighpassCL = (iirHighpassCL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassCL;
					iirLowpassCL = (iirLowpassCL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassCL;
					iirHighpassDL = (iirHighpassDL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassDL;
					iirLowpassDL = (iirLowpassDL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassDL;
					iirHighpassAR = (iirHighpassAR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
					iirLowpassAR = (iirLowpassAR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
					iirHighpassCR = (iirHighpassCR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassCR;
					iirLowpassCR = (iirLowpassCR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassCR;
					iirHighpassDR = (iirHighpassDR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassDR;
					iirLowpassDR = (iirLowpassDR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassDR;
					break;
				case 4:
					iirHighpassAL = (iirHighpassAL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
					iirLowpassAL = (iirLowpassAL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
					iirHighpassBL = (iirHighpassBL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassBL;
					iirLowpassBL = (iirLowpassBL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassBL;
					iirHighpassEL = (iirHighpassEL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassEL;
					iirLowpassEL = (iirLowpassEL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassEL;
					iirHighpassAR = (iirHighpassAR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
					iirLowpassAR = (iirLowpassAR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
					iirHighpassBR = (iirHighpassBR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassBR;
					iirLowpassBR = (iirLowpassBR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassBR;
					iirHighpassER = (iirHighpassER * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassER;
					iirLowpassER = (iirLowpassER * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassER;
					break;
				case 5:
					iirHighpassAL = (iirHighpassAL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
					iirLowpassAL = (iirLowpassAL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
					iirHighpassCL = (iirHighpassCL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassCL;
					iirLowpassCL = (iirLowpassCL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassCL;
					iirHighpassFL = (iirHighpassFL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassFL;
					iirLowpassFL = (iirLowpassFL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassFL;
					iirHighpassAR = (iirHighpassAR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
					iirLowpassAR = (iirLowpassAR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
					iirHighpassCR = (iirHighpassCR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassCR;
					iirLowpassCR = (iirLowpassCR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassCR;
					iirHighpassFR = (iirHighpassFR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassFR;
					iirLowpassFR = (iirLowpassFR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassFR;
					break;
			}
			//Highpass Filter chunk. This is three poles of IIR highpass, with a 'gearbox' that progressively
			//steepens the filter after minimizing artifacts.
		}

		
		//begin compressor
		if (engageComp)
		{
			flip = !flip;
			//begin L
			inputSampleL *= inputgain;
			
			inputpos = (inputSampleL * fpOld) + (avgLA * fpNew) + 1.0;
			avgLA = inputSampleL;
			
			if (inputpos < 0.0) inputpos = 0.0;
			outputpos = inputpos / 2.0;
			if (outputpos > 1.0) outputpos = 1.0;		
			inputpos *= inputpos;
			targetposL *= divisor;
			targetposL += (inputpos * remainder);
			calcpos = pow((1.0/targetposL),2);
			
			inputneg = (-inputSampleL * fpOld) + (nvgLA * fpNew) + 1.0;
			nvgLA = -inputSampleL;
			
			if (inputneg < 0.0) inputneg = 0.0;
			outputneg = inputneg / 2.0;
			if (outputneg > 1.0) outputneg = 1.0;		
			inputneg *= inputneg;
			targetnegL *= divisor;
			targetnegL += (inputneg * remainder);
			calcneg = pow((1.0/targetnegL),2);
			//now we have mirrored targets for comp
			//outputpos and outputneg go from 0 to 1
			
			if (inputSampleL > 0)
			{ //working on pos
				if (true == flip)
				{
					controlAposL *= divisor;
					controlAposL += (calcpos*remainder);
					
				}
				else
				{
					controlBposL *= divisor;
					controlBposL += (calcpos*remainder);
				}	
			}
			else
			{ //working on neg
				if (true == flip)
				{
					controlAnegL *= divisor;
					controlAnegL += (calcneg*remainder);
				}
				else
				{
					controlBnegL *= divisor;
					controlBnegL += (calcneg*remainder);
				}
			}
			//this causes each of the four to update only when active and in the correct 'flip'
			
			if (true == flip)
			{totalmultiplier = (controlAposL * outputpos) + (controlAnegL * outputneg);}
			else
			{totalmultiplier = (controlBposL * outputpos) + (controlBnegL * outputneg);}
			//this combines the sides according to flip, blending relative to the input value
			
			inputSampleL *= totalmultiplier;
			inputSampleL /= compoutgain;
			//end L
			
			//begin R
			inputSampleR *= inputgain;
			
			inputpos = (inputSampleR * fpOld) + (avgRA * fpNew) + 1.0;
			avgRA = inputSampleR;
			
			if (inputpos < 0.0) inputpos = 0.0;
			outputpos = inputpos / 2.0;
			if (outputpos > 1.0) outputpos = 1.0;		
			inputpos *= inputpos;
			targetposR *= divisor;
			targetposR += (inputpos * remainder);
			calcpos = pow((1.0/targetposR),2);
			
			inputneg = (-inputSampleR * fpOld) + (nvgRA * fpNew) + 1.0;
			nvgRA = -inputSampleR;
			
			if (inputneg < 0.0) inputneg = 0.0;
			outputneg = inputneg / 2.0;
			if (outputneg > 1.0) outputneg = 1.0;		
			inputneg *= inputneg;
			targetnegR *= divisor;
			targetnegR += (inputneg * remainder);
			calcneg = pow((1.0/targetnegR),2);
			//now we have mirrored targets for comp
			//outputpos and outputneg go from 0 to 1
			
			if (inputSampleR > 0)
			{ //working on pos
				if (true == flip)
				{
					controlAposR *= divisor;
					controlAposR += (calcpos*remainder);
					
				}
				else
				{
					controlBposR *= divisor;
					controlBposR += (calcpos*remainder);
				}	
			}
			else
			{ //working on neg
				if (true == flip)
				{
					controlAnegR *= divisor;
					controlAnegR += (calcneg*remainder);
				}
				else
				{
					controlBnegR *= divisor;
					controlBnegR += (calcneg*remainder);
				}
			}
			//this causes each of the four to update only when active and in the correct 'flip'
			
			if (true == flip)
			{totalmultiplier = (controlAposR * outputpos) + (controlAnegR * outputneg);}
			else
			{totalmultiplier = (controlBposR * outputpos) + (controlBnegR * outputneg);}
			//this combines the sides according to flip, blending relative to the input value
			
			inputSampleR *= totalmultiplier;
			inputSampleR /= compoutgain;
			//end R
		}
		//end compressor
		
		//begin EQ
		if (engageEQ)
		{
			last2SampleL = lastSampleL;
			lastSampleL = inputSampleL;
			last2SampleR = lastSampleR;
			lastSampleR = inputSampleR;
			flipthree++;
			if (flipthree < 1 || flipthree > 3) flipthree = 1;
			switch (flipthree)
			{
				case 1:
					tripletFactorL = last2SampleL - inputSampleL;
					tripletLA += tripletFactorL;
					tripletLC -= tripletFactorL;
					tripletFactorL = tripletLA * tripletIntensity;
					iirHighSampleLC = (iirHighSampleLC * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
					highSampleL = inputSampleL - iirHighSampleLC;
					iirLowSampleLC = (iirLowSampleLC * (1.0 - iirAmountB)) + (inputSampleL * iirAmountB);
					bassSampleL = iirLowSampleLC;
					
					tripletFactorR = last2SampleR - inputSampleR;
					tripletRA += tripletFactorR;
					tripletRC -= tripletFactorR;
					tripletFactorR = tripletRA * tripletIntensity;
					iirHighSampleRC = (iirHighSampleRC * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
					highSampleR = inputSampleR - iirHighSampleRC;
					iirLowSampleRC = (iirLowSampleRC * (1.0 - iirAmountB)) + (inputSampleR * iirAmountB);
					bassSampleR = iirLowSampleRC;
					break;
				case 2:
					tripletFactorL = last2SampleL - inputSampleL;
					tripletLB += tripletFactorL;
					tripletLA -= tripletFactorL;
					tripletFactorL = tripletLB * tripletIntensity;
					iirHighSampleLD = (iirHighSampleLD * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
					highSampleL = inputSampleL - iirHighSampleLD;
					iirLowSampleLD = (iirLowSampleLD * (1.0 - iirAmountB)) + (inputSampleL * iirAmountB);
					bassSampleL = iirLowSampleLD;
					
					tripletFactorR = last2SampleR - inputSampleR;
					tripletRB += tripletFactorR;
					tripletRA -= tripletFactorR;
					tripletFactorR = tripletRB * tripletIntensity;
					iirHighSampleRD = (iirHighSampleRD * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
					highSampleR = inputSampleR - iirHighSampleRD;
					iirLowSampleRD = (iirLowSampleRD * (1.0 - iirAmountB)) + (inputSampleR * iirAmountB);
					bassSampleR = iirLowSampleRD;
					break;
				case 3:
					tripletFactorL = last2SampleL - inputSampleL;
					tripletLC += tripletFactorL;
					tripletLB -= tripletFactorL;
					tripletFactorL = tripletLC * tripletIntensity;
					iirHighSampleLE = (iirHighSampleLE * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
					highSampleL = inputSampleL - iirHighSampleLE;
					iirLowSampleLE = (iirLowSampleLE * (1.0 - iirAmountB)) + (inputSampleL * iirAmountB);
					bassSampleL = iirLowSampleLE;
					
					tripletFactorR = last2SampleR - inputSampleR;
					tripletRC += tripletFactorR;
					tripletRB -= tripletFactorR;
					tripletFactorR = tripletRC * tripletIntensity;
					iirHighSampleRE = (iirHighSampleRE * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
					highSampleR = inputSampleR - iirHighSampleRE;
					iirLowSampleRE = (iirLowSampleRE * (1.0 - iirAmountB)) + (inputSampleR * iirAmountB);
					bassSampleR = iirLowSampleRE;
					break;
			}
			tripletLA /= 2.0;
			tripletLB /= 2.0;
			tripletLC /= 2.0;
			highSampleL = highSampleL + tripletFactorL;
			
			tripletRA /= 2.0;
			tripletRB /= 2.0;
			tripletRC /= 2.0;
			highSampleR = highSampleR + tripletFactorR;
			
			if (flip)
			{
				iirHighSampleLA = (iirHighSampleLA * (1.0 - iirAmountA)) + (highSampleL * iirAmountA);
				highSampleL -= iirHighSampleLA;
				iirLowSampleLA = (iirLowSampleLA * (1.0 - iirAmountB)) + (bassSampleL * iirAmountB);
				bassSampleL = iirLowSampleLA;
				
				iirHighSampleRA = (iirHighSampleRA * (1.0 - iirAmountA)) + (highSampleR * iirAmountA);
				highSampleR -= iirHighSampleRA;
				iirLowSampleRA = (iirLowSampleRA * (1.0 - iirAmountB)) + (bassSampleR * iirAmountB);
				bassSampleR = iirLowSampleRA;
			}
			else
			{
				iirHighSampleLB = (iirHighSampleLB * (1.0 - iirAmountA)) + (highSampleL * iirAmountA);
				highSampleL -= iirHighSampleLB;
				iirLowSampleLB = (iirLowSampleLB * (1.0 - iirAmountB)) + (bassSampleL * iirAmountB);
				bassSampleL = iirLowSampleLB;
				
				iirHighSampleRB = (iirHighSampleRB * (1.0 - iirAmountA)) + (highSampleR * iirAmountA);
				highSampleR -= iirHighSampleRB;
				iirLowSampleRB = (iirLowSampleRB * (1.0 - iirAmountB)) + (bassSampleR * iirAmountB);
				bassSampleR = iirLowSampleRB;
			}
			
			iirHighSampleL = (iirHighSampleL * (1.0 - iirAmountA)) + (highSampleL * iirAmountA);
			highSampleL -= iirHighSampleL;
			iirLowSampleL = (iirLowSampleL * (1.0 - iirAmountB)) + (bassSampleL * iirAmountB);
			bassSampleL = iirLowSampleL;
			
			iirHighSampleR = (iirHighSampleR * (1.0 - iirAmountA)) + (highSampleR * iirAmountA);
			highSampleR -= iirHighSampleR;
			iirLowSampleR = (iirLowSampleR * (1.0 - iirAmountB)) + (bassSampleR * iirAmountB);
			bassSampleR = iirLowSampleR;
			
			midSampleL = (inputSampleL-bassSampleL)-highSampleL;
			midSampleR = (inputSampleR-bassSampleR)-highSampleR;
			
			//drive section
			highSampleL *= (densityA+1.0);
			bridgerectifier = fabs(highSampleL)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			if (densityA > 0) bridgerectifier = sin(bridgerectifier);
			else bridgerectifier = 1-cos(bridgerectifier);
			//produce either boosted or starved version
			if (highSampleL > 0) highSampleL = (highSampleL*(1-outA))+(bridgerectifier*outA);
			else highSampleL = (highSampleL*(1-outA))-(bridgerectifier*outA);
			//blend according to densityA control
			
			highSampleR *= (densityA+1.0);
			bridgerectifier = fabs(highSampleR)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			if (densityA > 0) bridgerectifier = sin(bridgerectifier);
			else bridgerectifier = 1-cos(bridgerectifier);
			//produce either boosted or starved version
			if (highSampleR > 0) highSampleR = (highSampleR*(1-outA))+(bridgerectifier*outA);
			else highSampleR = (highSampleR*(1-outA))-(bridgerectifier*outA);
			//blend according to densityA control
			
			midSampleL *= (densityB+1.0);
			bridgerectifier = fabs(midSampleL)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			if (densityB > 0) bridgerectifier = sin(bridgerectifier);
			else bridgerectifier = 1-cos(bridgerectifier);
			//produce either boosted or starved version
			if (midSampleL > 0) midSampleL = (midSampleL*(1-outB))+(bridgerectifier*outB);
			else midSampleL = (midSampleL*(1-outB))-(bridgerectifier*outB);
			//blend according to densityB control
			
			midSampleR *= (densityB+1.0);
			bridgerectifier = fabs(midSampleR)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			if (densityB > 0) bridgerectifier = sin(bridgerectifier);
			else bridgerectifier = 1-cos(bridgerectifier);
			//produce either boosted or starved version
			if (midSampleR > 0) midSampleR = (midSampleR*(1-outB))+(bridgerectifier*outB);
			else midSampleR = (midSampleR*(1-outB))-(bridgerectifier*outB);
			//blend according to densityB control
			
			bassSampleL *= (densityC+1.0);
			bridgerectifier = fabs(bassSampleL)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			if (densityC > 0) bridgerectifier = sin(bridgerectifier);
			else bridgerectifier = 1-cos(bridgerectifier);
			//produce either boosted or starved version
			if (bassSampleL > 0) bassSampleL = (bassSampleL*(1-outC))+(bridgerectifier*outC);
			else bassSampleL = (bassSampleL*(1-outC))-(bridgerectifier*outC);
			//blend according to densityC control
			
			bassSampleR *= (densityC+1.0);
			bridgerectifier = fabs(bassSampleR)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			if (densityC > 0) bridgerectifier = sin(bridgerectifier);
			else bridgerectifier = 1-cos(bridgerectifier);
			//produce either boosted or starved version
			if (bassSampleR > 0) bassSampleR = (bassSampleR*(1-outC))+(bridgerectifier*outC);
			else bassSampleR = (bassSampleR*(1-outC))-(bridgerectifier*outC);
			//blend according to densityC control
			
			inputSampleL = midSampleL;
			inputSampleL += highSampleL;
			inputSampleL += bassSampleL;
			
			inputSampleR = midSampleR;
			inputSampleR += highSampleR;
			inputSampleR += bassSampleR;
		}
		//end EQ
		
		//built in output trim and dry/wet if desired
		if (outputgain != 1.0) {
			inputSampleL *= outputgain;
			inputSampleR *= outputgain;
			double rawSampleL = inputSampleL; //Spiral crossfades from unity gain to boosted
			double rawSampleR = inputSampleR; //Spiral crossfades from unity gain to boosted
			if (inputSampleL > 1.0) inputSampleL = 1.0;
			if (inputSampleL < -1.0) inputSampleL = -1.0; //all boosts clipped to 0dB
			if (inputSampleR > 1.0) inputSampleR = 1.0;
			if (inputSampleR < -1.0) inputSampleR = -1.0; //all boosts clipped to 0dB
			double phatSampleL = sin(inputSampleL * 1.57079633);
			double phatSampleR = sin(inputSampleR * 1.57079633);
			inputSampleL *= 1.2533141373155;
			inputSampleR *= 1.2533141373155;
			//clip to 1.2533141373155 to reach maximum output, or 1.57079633 for pure sine 'phat' version
			double distSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1.0:fabs(inputSampleL));
			double distSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1.0:fabs(inputSampleR));
			inputSampleL = distSampleL; //purest form is full Spiral
			inputSampleR = distSampleR; //purest form is full Spiral
			if (density < 1.0) {
				inputSampleL = (rawSampleL*(1.0-density))+(distSampleL*density);
				inputSampleR = (rawSampleR*(1.0-density))+(distSampleR*density);
			} //fade Spiral aspect
			if (phattity > 0.0) {
				inputSampleL = (inputSampleL*(1.0-phattity))+(phatSampleL*phattity);
				inputSampleR = (inputSampleR*(1.0-phattity))+(phatSampleR*phattity);
			} //apply original Density on top
			//output section is a pad from 0 to 1, Spiral from 1 to 2, Density from 2 to 3
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

void CStrip2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	double compscale = overallscale;
	overallscale = getSampleRate();
	compscale = compscale * overallscale;
	//compscale is the one that's 1 or something like 2.2 for 96K rates
	double fpOld = 0.618033988749894848204586; //golden ratio!
	double fpNew = 1.0 - fpOld;
	
	double highSampleL = 0.0;
	double midSampleL = 0.0;
	double bassSampleL = 0.0;
	
	double highSampleR = 0.0;
	double midSampleR = 0.0;
	double bassSampleR = 0.0;
	
	double densityA = (A*12.0)-6.0;
	double densityB = (B*12.0)-6.0;
	double densityC = (C*12.0)-6.0;
	bool engageEQ = true;
	if ( (0.0 == densityA) && (0.0 == densityB) && (0.0 == densityC) ) engageEQ = false;
	
	densityA = pow(10.0,densityA/20.0)-1.0;
	densityB = pow(10.0,densityB/20.0)-1.0;
	densityC = pow(10.0,densityC/20.0)-1.0;
	//convert to 0 to X multiplier with 1.0 being O db
	//minus one gives nearly -1 to ? (should top out at 1)
	//calibrate so that X db roughly equals X db with maximum topping out at 1 internally
	double tripletIntensity = -densityA;
	double iirAmountA = (((D*D*15.0)+1.0)*1000)/overallscale;
	double iirAmountB = (((E*E*1570.0)+30.0)*10)/overallscale;
	double bridgerectifier;
	double outA = fabs(densityA);
	double outB = fabs(densityB);
	double outC = fabs(densityC);
	//end EQ
	
	double lowpassAmount = pow(F,2);
	double highpassAmount = pow(G,2);
	bool engageCapacitor = false;
	if ((lowpassAmount < 1.0) || (highpassAmount > 0.0)) engageCapacitor = true;
	//end Capacitor
	
	//begin ButterComp
	double inputpos;
	double inputneg;
	double calcpos;
	double calcneg;
	double outputpos;
	double outputneg;
	double totalmultiplier;
	double inputgain = (pow(H,4)*35)+1.0;
	double compoutgain = inputgain;
	compoutgain -= 1.0;
	compoutgain /= 1.2;
	compoutgain += 1.0;
	double divisor = (0.008 * pow(I,2))+0.0004;
	//originally 0.012
	divisor /= compscale;
	double remainder = divisor;
	divisor = 1.0 - divisor;
	bool engageComp = false;
	if (inputgain > 1.0) engageComp = true;
	//end ButterComp
	
	double outputgain = J*3.0; //0-2
	double density = outputgain-1.0; //allow for output 0-1 to be clean, 1-3 all boosts
	if (density < 0.0) density = 0.0;
	double phattity = density - 1.0;
	if (density > 1.0) density = 1.0; //max out at full wet for Spiral aspect
	if (phattity < 0.0) phattity = 0.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (engageCapacitor)
		{
			count++; if (count > 5) count = 0;
			switch (count)
			{
				case 0:
					iirHighpassAL = (iirHighpassAL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
					iirLowpassAL = (iirLowpassAL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
					iirHighpassBL = (iirHighpassBL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassBL;
					iirLowpassBL = (iirLowpassBL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassBL;
					iirHighpassDL = (iirHighpassDL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassDL;
					iirLowpassDL = (iirLowpassDL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassDL;
					iirHighpassAR = (iirHighpassAR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
					iirLowpassAR = (iirLowpassAR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
					iirHighpassBR = (iirHighpassBR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassBR;
					iirLowpassBR = (iirLowpassBR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassBR;
					iirHighpassDR = (iirHighpassDR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassDR;
					iirLowpassDR = (iirLowpassDR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassDR;
					break;
				case 1:
					iirHighpassAL = (iirHighpassAL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
					iirLowpassAL = (iirLowpassAL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
					iirHighpassCL = (iirHighpassCL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassCL;
					iirLowpassCL = (iirLowpassCL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassCL;
					iirHighpassEL = (iirHighpassEL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassEL;
					iirLowpassEL = (iirLowpassEL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassEL;
					iirHighpassAR = (iirHighpassAR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
					iirLowpassAR = (iirLowpassAR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
					iirHighpassCR = (iirHighpassCR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassCR;
					iirLowpassCR = (iirLowpassCR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassCR;
					iirHighpassER = (iirHighpassER * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassER;
					iirLowpassER = (iirLowpassER * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassER;
					break;
				case 2:
					iirHighpassAL = (iirHighpassAL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
					iirLowpassAL = (iirLowpassAL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
					iirHighpassBL = (iirHighpassBL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassBL;
					iirLowpassBL = (iirLowpassBL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassBL;
					iirHighpassFL = (iirHighpassFL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassFL;
					iirLowpassFL = (iirLowpassFL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassFL;
					iirHighpassAR = (iirHighpassAR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
					iirLowpassAR = (iirLowpassAR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
					iirHighpassBR = (iirHighpassBR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassBR;
					iirLowpassBR = (iirLowpassBR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassBR;
					iirHighpassFR = (iirHighpassFR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassFR;
					iirLowpassFR = (iirLowpassFR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassFR;
					break;
				case 3:
					iirHighpassAL = (iirHighpassAL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
					iirLowpassAL = (iirLowpassAL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
					iirHighpassCL = (iirHighpassCL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassCL;
					iirLowpassCL = (iirLowpassCL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassCL;
					iirHighpassDL = (iirHighpassDL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassDL;
					iirLowpassDL = (iirLowpassDL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassDL;
					iirHighpassAR = (iirHighpassAR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
					iirLowpassAR = (iirLowpassAR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
					iirHighpassCR = (iirHighpassCR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassCR;
					iirLowpassCR = (iirLowpassCR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassCR;
					iirHighpassDR = (iirHighpassDR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassDR;
					iirLowpassDR = (iirLowpassDR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassDR;
					break;
				case 4:
					iirHighpassAL = (iirHighpassAL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
					iirLowpassAL = (iirLowpassAL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
					iirHighpassBL = (iirHighpassBL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassBL;
					iirLowpassBL = (iirLowpassBL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassBL;
					iirHighpassEL = (iirHighpassEL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassEL;
					iirLowpassEL = (iirLowpassEL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassEL;
					iirHighpassAR = (iirHighpassAR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
					iirLowpassAR = (iirLowpassAR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
					iirHighpassBR = (iirHighpassBR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassBR;
					iirLowpassBR = (iirLowpassBR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassBR;
					iirHighpassER = (iirHighpassER * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassER;
					iirLowpassER = (iirLowpassER * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassER;
					break;
				case 5:
					iirHighpassAL = (iirHighpassAL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
					iirLowpassAL = (iirLowpassAL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
					iirHighpassCL = (iirHighpassCL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassCL;
					iirLowpassCL = (iirLowpassCL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassCL;
					iirHighpassFL = (iirHighpassFL * (1.0-highpassAmount)) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassFL;
					iirLowpassFL = (iirLowpassFL * (1.0-lowpassAmount)) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassFL;
					iirHighpassAR = (iirHighpassAR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
					iirLowpassAR = (iirLowpassAR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
					iirHighpassCR = (iirHighpassCR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassCR;
					iirLowpassCR = (iirLowpassCR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassCR;
					iirHighpassFR = (iirHighpassFR * (1.0-highpassAmount)) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassFR;
					iirLowpassFR = (iirLowpassFR * (1.0-lowpassAmount)) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassFR;
					break;
			}
			//Highpass Filter chunk. This is three poles of IIR highpass, with a 'gearbox' that progressively
			//steepens the filter after minimizing artifacts.
		}
		
		
		//begin compressor
		if (engageComp)
		{
			flip = !flip;
			//begin L
			inputSampleL *= inputgain;
			
			inputpos = (inputSampleL * fpOld) + (avgLA * fpNew) + 1.0;
			avgLA = inputSampleL;
			
			if (inputpos < 0.0) inputpos = 0.0;
			outputpos = inputpos / 2.0;
			if (outputpos > 1.0) outputpos = 1.0;		
			inputpos *= inputpos;
			targetposL *= divisor;
			targetposL += (inputpos * remainder);
			calcpos = pow((1.0/targetposL),2);
			
			inputneg = (-inputSampleL * fpOld) + (nvgLA * fpNew) + 1.0;
			nvgLA = -inputSampleL;
			
			if (inputneg < 0.0) inputneg = 0.0;
			outputneg = inputneg / 2.0;
			if (outputneg > 1.0) outputneg = 1.0;		
			inputneg *= inputneg;
			targetnegL *= divisor;
			targetnegL += (inputneg * remainder);
			calcneg = pow((1.0/targetnegL),2);
			//now we have mirrored targets for comp
			//outputpos and outputneg go from 0 to 1
			
			if (inputSampleL > 0)
			{ //working on pos
				if (true == flip)
				{
					controlAposL *= divisor;
					controlAposL += (calcpos*remainder);
					
				}
				else
				{
					controlBposL *= divisor;
					controlBposL += (calcpos*remainder);
				}	
			}
			else
			{ //working on neg
				if (true == flip)
				{
					controlAnegL *= divisor;
					controlAnegL += (calcneg*remainder);
				}
				else
				{
					controlBnegL *= divisor;
					controlBnegL += (calcneg*remainder);
				}
			}
			//this causes each of the four to update only when active and in the correct 'flip'
			
			if (true == flip)
			{totalmultiplier = (controlAposL * outputpos) + (controlAnegL * outputneg);}
			else
			{totalmultiplier = (controlBposL * outputpos) + (controlBnegL * outputneg);}
			//this combines the sides according to flip, blending relative to the input value
			
			inputSampleL *= totalmultiplier;
			inputSampleL /= compoutgain;
			//end L
			
			//begin R
			inputSampleR *= inputgain;
			
			inputpos = (inputSampleR * fpOld) + (avgRA * fpNew) + 1.0;
			avgRA = inputSampleR;
			
			if (inputpos < 0.0) inputpos = 0.0;
			outputpos = inputpos / 2.0;
			if (outputpos > 1.0) outputpos = 1.0;		
			inputpos *= inputpos;
			targetposR *= divisor;
			targetposR += (inputpos * remainder);
			calcpos = pow((1.0/targetposR),2);
			
			inputneg = (-inputSampleR * fpOld) + (nvgRA * fpNew) + 1.0;
			nvgRA = -inputSampleR;
			
			if (inputneg < 0.0) inputneg = 0.0;
			outputneg = inputneg / 2.0;
			if (outputneg > 1.0) outputneg = 1.0;		
			inputneg *= inputneg;
			targetnegR *= divisor;
			targetnegR += (inputneg * remainder);
			calcneg = pow((1.0/targetnegR),2);
			//now we have mirrored targets for comp
			//outputpos and outputneg go from 0 to 1
			
			if (inputSampleR > 0)
			{ //working on pos
				if (true == flip)
				{
					controlAposR *= divisor;
					controlAposR += (calcpos*remainder);
					
				}
				else
				{
					controlBposR *= divisor;
					controlBposR += (calcpos*remainder);
				}	
			}
			else
			{ //working on neg
				if (true == flip)
				{
					controlAnegR *= divisor;
					controlAnegR += (calcneg*remainder);
				}
				else
				{
					controlBnegR *= divisor;
					controlBnegR += (calcneg*remainder);
				}
			}
			//this causes each of the four to update only when active and in the correct 'flip'
			
			if (true == flip)
			{totalmultiplier = (controlAposR * outputpos) + (controlAnegR * outputneg);}
			else
			{totalmultiplier = (controlBposR * outputpos) + (controlBnegR * outputneg);}
			//this combines the sides according to flip, blending relative to the input value
			
			inputSampleR *= totalmultiplier;
			inputSampleR /= compoutgain;
			//end R
		}
		//end compressor
		
		//begin EQ
		if (engageEQ)
		{
			last2SampleL = lastSampleL;
			lastSampleL = inputSampleL;
			last2SampleR = lastSampleR;
			lastSampleR = inputSampleR;
			flipthree++;
			if (flipthree < 1 || flipthree > 3) flipthree = 1;
			switch (flipthree)
			{
				case 1:
					tripletFactorL = last2SampleL - inputSampleL;
					tripletLA += tripletFactorL;
					tripletLC -= tripletFactorL;
					tripletFactorL = tripletLA * tripletIntensity;
					iirHighSampleLC = (iirHighSampleLC * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
					highSampleL = inputSampleL - iirHighSampleLC;
					iirLowSampleLC = (iirLowSampleLC * (1.0 - iirAmountB)) + (inputSampleL * iirAmountB);
					bassSampleL = iirLowSampleLC;
					
					tripletFactorR = last2SampleR - inputSampleR;
					tripletRA += tripletFactorR;
					tripletRC -= tripletFactorR;
					tripletFactorR = tripletRA * tripletIntensity;
					iirHighSampleRC = (iirHighSampleRC * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
					highSampleR = inputSampleR - iirHighSampleRC;
					iirLowSampleRC = (iirLowSampleRC * (1.0 - iirAmountB)) + (inputSampleR * iirAmountB);
					bassSampleR = iirLowSampleRC;
					break;
				case 2:
					tripletFactorL = last2SampleL - inputSampleL;
					tripletLB += tripletFactorL;
					tripletLA -= tripletFactorL;
					tripletFactorL = tripletLB * tripletIntensity;
					iirHighSampleLD = (iirHighSampleLD * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
					highSampleL = inputSampleL - iirHighSampleLD;
					iirLowSampleLD = (iirLowSampleLD * (1.0 - iirAmountB)) + (inputSampleL * iirAmountB);
					bassSampleL = iirLowSampleLD;
					
					tripletFactorR = last2SampleR - inputSampleR;
					tripletRB += tripletFactorR;
					tripletRA -= tripletFactorR;
					tripletFactorR = tripletRB * tripletIntensity;
					iirHighSampleRD = (iirHighSampleRD * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
					highSampleR = inputSampleR - iirHighSampleRD;
					iirLowSampleRD = (iirLowSampleRD * (1.0 - iirAmountB)) + (inputSampleR * iirAmountB);
					bassSampleR = iirLowSampleRD;
					break;
				case 3:
					tripletFactorL = last2SampleL - inputSampleL;
					tripletLC += tripletFactorL;
					tripletLB -= tripletFactorL;
					tripletFactorL = tripletLC * tripletIntensity;
					iirHighSampleLE = (iirHighSampleLE * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
					highSampleL = inputSampleL - iirHighSampleLE;
					iirLowSampleLE = (iirLowSampleLE * (1.0 - iirAmountB)) + (inputSampleL * iirAmountB);
					bassSampleL = iirLowSampleLE;
					
					tripletFactorR = last2SampleR - inputSampleR;
					tripletRC += tripletFactorR;
					tripletRB -= tripletFactorR;
					tripletFactorR = tripletRC * tripletIntensity;
					iirHighSampleRE = (iirHighSampleRE * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
					highSampleR = inputSampleR - iirHighSampleRE;
					iirLowSampleRE = (iirLowSampleRE * (1.0 - iirAmountB)) + (inputSampleR * iirAmountB);
					bassSampleR = iirLowSampleRE;
					break;
			}
			tripletLA /= 2.0;
			tripletLB /= 2.0;
			tripletLC /= 2.0;
			highSampleL = highSampleL + tripletFactorL;
			
			tripletRA /= 2.0;
			tripletRB /= 2.0;
			tripletRC /= 2.0;
			highSampleR = highSampleR + tripletFactorR;
			
			if (flip)
			{
				iirHighSampleLA = (iirHighSampleLA * (1.0 - iirAmountA)) + (highSampleL * iirAmountA);
				highSampleL -= iirHighSampleLA;
				iirLowSampleLA = (iirLowSampleLA * (1.0 - iirAmountB)) + (bassSampleL * iirAmountB);
				bassSampleL = iirLowSampleLA;
				
				iirHighSampleRA = (iirHighSampleRA * (1.0 - iirAmountA)) + (highSampleR * iirAmountA);
				highSampleR -= iirHighSampleRA;
				iirLowSampleRA = (iirLowSampleRA * (1.0 - iirAmountB)) + (bassSampleR * iirAmountB);
				bassSampleR = iirLowSampleRA;
			}
			else
			{
				iirHighSampleLB = (iirHighSampleLB * (1.0 - iirAmountA)) + (highSampleL * iirAmountA);
				highSampleL -= iirHighSampleLB;
				iirLowSampleLB = (iirLowSampleLB * (1.0 - iirAmountB)) + (bassSampleL * iirAmountB);
				bassSampleL = iirLowSampleLB;
				
				iirHighSampleRB = (iirHighSampleRB * (1.0 - iirAmountA)) + (highSampleR * iirAmountA);
				highSampleR -= iirHighSampleRB;
				iirLowSampleRB = (iirLowSampleRB * (1.0 - iirAmountB)) + (bassSampleR * iirAmountB);
				bassSampleR = iirLowSampleRB;
			}
			
			iirHighSampleL = (iirHighSampleL * (1.0 - iirAmountA)) + (highSampleL * iirAmountA);
			highSampleL -= iirHighSampleL;
			iirLowSampleL = (iirLowSampleL * (1.0 - iirAmountB)) + (bassSampleL * iirAmountB);
			bassSampleL = iirLowSampleL;
			
			iirHighSampleR = (iirHighSampleR * (1.0 - iirAmountA)) + (highSampleR * iirAmountA);
			highSampleR -= iirHighSampleR;
			iirLowSampleR = (iirLowSampleR * (1.0 - iirAmountB)) + (bassSampleR * iirAmountB);
			bassSampleR = iirLowSampleR;
			
			midSampleL = (inputSampleL-bassSampleL)-highSampleL;
			midSampleR = (inputSampleR-bassSampleR)-highSampleR;
			
			//drive section
			highSampleL *= (densityA+1.0);
			bridgerectifier = fabs(highSampleL)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			if (densityA > 0) bridgerectifier = sin(bridgerectifier);
			else bridgerectifier = 1-cos(bridgerectifier);
			//produce either boosted or starved version
			if (highSampleL > 0) highSampleL = (highSampleL*(1-outA))+(bridgerectifier*outA);
			else highSampleL = (highSampleL*(1-outA))-(bridgerectifier*outA);
			//blend according to densityA control
			
			highSampleR *= (densityA+1.0);
			bridgerectifier = fabs(highSampleR)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			if (densityA > 0) bridgerectifier = sin(bridgerectifier);
			else bridgerectifier = 1-cos(bridgerectifier);
			//produce either boosted or starved version
			if (highSampleR > 0) highSampleR = (highSampleR*(1-outA))+(bridgerectifier*outA);
			else highSampleR = (highSampleR*(1-outA))-(bridgerectifier*outA);
			//blend according to densityA control
			
			midSampleL *= (densityB+1.0);
			bridgerectifier = fabs(midSampleL)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			if (densityB > 0) bridgerectifier = sin(bridgerectifier);
			else bridgerectifier = 1-cos(bridgerectifier);
			//produce either boosted or starved version
			if (midSampleL > 0) midSampleL = (midSampleL*(1-outB))+(bridgerectifier*outB);
			else midSampleL = (midSampleL*(1-outB))-(bridgerectifier*outB);
			//blend according to densityB control
			
			midSampleR *= (densityB+1.0);
			bridgerectifier = fabs(midSampleR)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			if (densityB > 0) bridgerectifier = sin(bridgerectifier);
			else bridgerectifier = 1-cos(bridgerectifier);
			//produce either boosted or starved version
			if (midSampleR > 0) midSampleR = (midSampleR*(1-outB))+(bridgerectifier*outB);
			else midSampleR = (midSampleR*(1-outB))-(bridgerectifier*outB);
			//blend according to densityB control
			
			bassSampleL *= (densityC+1.0);
			bridgerectifier = fabs(bassSampleL)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			if (densityC > 0) bridgerectifier = sin(bridgerectifier);
			else bridgerectifier = 1-cos(bridgerectifier);
			//produce either boosted or starved version
			if (bassSampleL > 0) bassSampleL = (bassSampleL*(1-outC))+(bridgerectifier*outC);
			else bassSampleL = (bassSampleL*(1-outC))-(bridgerectifier*outC);
			//blend according to densityC control
			
			bassSampleR *= (densityC+1.0);
			bridgerectifier = fabs(bassSampleR)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			if (densityC > 0) bridgerectifier = sin(bridgerectifier);
			else bridgerectifier = 1-cos(bridgerectifier);
			//produce either boosted or starved version
			if (bassSampleR > 0) bassSampleR = (bassSampleR*(1-outC))+(bridgerectifier*outC);
			else bassSampleR = (bassSampleR*(1-outC))-(bridgerectifier*outC);
			//blend according to densityC control
			
			inputSampleL = midSampleL;
			inputSampleL += highSampleL;
			inputSampleL += bassSampleL;
			
			inputSampleR = midSampleR;
			inputSampleR += highSampleR;
			inputSampleR += bassSampleR;
		}
		//end EQ
		
		//built in output trim and dry/wet if desired
		if (outputgain != 1.0) {
			inputSampleL *= outputgain;
			inputSampleR *= outputgain;
			double rawSampleL = inputSampleL; //Spiral crossfades from unity gain to boosted
			double rawSampleR = inputSampleR; //Spiral crossfades from unity gain to boosted
			if (inputSampleL > 1.0) inputSampleL = 1.0;
			if (inputSampleL < -1.0) inputSampleL = -1.0; //all boosts clipped to 0dB
			if (inputSampleR > 1.0) inputSampleR = 1.0;
			if (inputSampleR < -1.0) inputSampleR = -1.0; //all boosts clipped to 0dB
			double phatSampleL = sin(inputSampleL * 1.57079633);
			double phatSampleR = sin(inputSampleR * 1.57079633);
			inputSampleL *= 1.2533141373155;
			inputSampleR *= 1.2533141373155;
			//clip to 1.2533141373155 to reach maximum output, or 1.57079633 for pure sine 'phat' version
			double distSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1.0:fabs(inputSampleL));
			double distSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1.0:fabs(inputSampleR));
			inputSampleL = distSampleL; //purest form is full Spiral
			inputSampleR = distSampleR; //purest form is full Spiral
			if (density < 1.0) {
				inputSampleL = (rawSampleL*(1.0-density))+(distSampleL*density);
				inputSampleR = (rawSampleR*(1.0-density))+(distSampleR*density);
			} //fade Spiral aspect
			if (phattity > 0.0) {
				inputSampleL = (inputSampleL*(1.0-phattity))+(phatSampleL*phattity);
				inputSampleR = (inputSampleR*(1.0-phattity))+(phatSampleR*phattity);
			} //apply original Density on top
			//output section is a pad from 0 to 1, Spiral from 1 to 2, Density from 2 to 3
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
