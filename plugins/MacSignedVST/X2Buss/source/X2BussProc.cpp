/* ========================================
 *  X2Buss - X2Buss.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __X2Buss_H
#include "X2Buss.h"
#endif

void X2Buss::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int spacing = floor(overallscale*2.0);
	if (spacing < 2) spacing = 2; if (spacing > 32) spacing = 32;
	
	double trebleGain = (A-0.5)*2.0;
	trebleGain = 1.0+(trebleGain*fabs(trebleGain)*fabs(trebleGain));
	double highmidGain = (B-0.5)*2.0;
	highmidGain = 1.0+(highmidGain*fabs(highmidGain)*fabs(highmidGain));
	double lowmidGain = (C-0.5)*2.0;
	lowmidGain = 1.0+(lowmidGain*fabs(lowmidGain)*fabs(lowmidGain));
	double bassGain = (D-0.5)*2.0;
	bassGain = 1.0+(bassGain*fabs(bassGain)*fabs(bassGain));
	
	double trebleRef = E-0.5;
	double highmidRef = F-0.5;
	double lowmidRef = G-0.5;
	double bassRef = H-0.5;
	double highF = 0.75 + ((trebleRef+trebleRef+trebleRef+highmidRef)*0.125);
	double bassF = 0.25 + ((lowmidRef+bassRef+bassRef+bassRef)*0.125);
	double midF = (highF*0.5) + (bassF*0.5) + ((highmidRef+lowmidRef)*0.125);
	
	double highQ = fmax(fmin(1.0+(highmidRef-trebleRef),4.0),0.125);
	double midQ = fmax(fmin(1.0+(lowmidRef-highmidRef),4.0),0.125);
	double lowQ = fmax(fmin(1.0+(bassRef-lowmidRef),4.0),0.125);
	
	highA[biq_freq] = ((pow(highF,3)*20000.0)/getSampleRate());
	highC[biq_freq] = highB[biq_freq] = highA[biq_freq] = fmax(fmin(highA[biq_freq],0.4999),0.00025);
	double highFreq = pow(highF,3)*20000.0;
	double omega = 2.0*M_PI*(highFreq/getSampleRate());
	double biqK = 2.0-cos(omega);
	double highCoef = -sqrt((biqK*biqK)-1.0)+biqK;
	highA[biq_reso] = 2.24697960 * highQ;
	highB[biq_reso] = 0.80193774 * highQ;
	highC[biq_reso] = 0.55495813 * highQ;
	
	midA[biq_freq] = ((pow(midF,3)*20000.0)/getSampleRate());
	midC[biq_freq] = midB[biq_freq] = midA[biq_freq] = fmax(fmin(midA[biq_freq],0.4999),0.00025);	
	double midFreq = pow(midF,3)*20000.0;
	omega = 2.0*M_PI*(midFreq/getSampleRate());
	biqK = 2.0-cos(omega);
	double midCoef = -sqrt((biqK*biqK)-1.0)+biqK;
	midA[biq_reso] = 2.24697960 * midQ;
	midB[biq_reso] = 0.80193774 * midQ;
	midC[biq_reso] = 0.55495813 * midQ;
	
	lowA[biq_freq] = ((pow(bassF,3)*20000.0)/getSampleRate());
	lowC[biq_freq] = lowB[biq_freq] = lowA[biq_freq] = fmax(fmin(lowA[biq_freq],0.4999),0.00025);
	double lowFreq = pow(bassF,3)*20000.0;
	omega = 2.0*M_PI*(lowFreq/getSampleRate());
	biqK = 2.0-cos(omega);
	double lowCoef = -sqrt((biqK*biqK)-1.0)+biqK;
	lowA[biq_reso] = 2.24697960 * lowQ;
	lowB[biq_reso] = 0.80193774 * lowQ;
	lowC[biq_reso] = 0.55495813 * lowQ;
	
	biqK = tan(M_PI * highA[biq_freq]);
	double norm = 1.0 / (1.0 + biqK / highA[biq_reso] + biqK * biqK);
	highA[biq_a0] = biqK * biqK * norm;
	highA[biq_a1] = 2.0 * highA[biq_a0];
	highA[biq_a2] = highA[biq_a0];
	highA[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highA[biq_b2] = (1.0 - biqK / highA[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * highB[biq_freq]);
	norm = 1.0 / (1.0 + biqK / highB[biq_reso] + biqK * biqK);
	highB[biq_a0] = biqK * biqK * norm;
	highB[biq_a1] = 2.0 * highB[biq_a0];
	highB[biq_a2] = highB[biq_a0];
	highB[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highB[biq_b2] = (1.0 - biqK / highB[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * highC[biq_freq]);
	norm = 1.0 / (1.0 + biqK / highC[biq_reso] + biqK * biqK);
	highC[biq_a0] = biqK * biqK * norm;
	highC[biq_a1] = 2.0 * highC[biq_a0];
	highC[biq_a2] = highC[biq_a0];
	highC[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highC[biq_b2] = (1.0 - biqK / highC[biq_reso] + biqK * biqK) * norm;
	
	biqK = tan(M_PI * midA[biq_freq]);
	norm = 1.0 / (1.0 + biqK / midA[biq_reso] + biqK * biqK);
	midA[biq_a0] = biqK * biqK * norm;
	midA[biq_a1] = 2.0 * midA[biq_a0];
	midA[biq_a2] = midA[biq_a0];
	midA[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	midA[biq_b2] = (1.0 - biqK / midA[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * midB[biq_freq]);
	norm = 1.0 / (1.0 + biqK / midB[biq_reso] + biqK * biqK);
	midB[biq_a0] = biqK * biqK * norm;
	midB[biq_a1] = 2.0 * midB[biq_a0];
	midB[biq_a2] = midB[biq_a0];
	midB[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	midB[biq_b2] = (1.0 - biqK / midB[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * midC[biq_freq]);
	norm = 1.0 / (1.0 + biqK / midC[biq_reso] + biqK * biqK);
	midC[biq_a0] = biqK * biqK * norm;
	midC[biq_a1] = 2.0 * midC[biq_a0];
	midC[biq_a2] = midC[biq_a0];
	midC[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	midC[biq_b2] = (1.0 - biqK / midC[biq_reso] + biqK * biqK) * norm;
	
	biqK = tan(M_PI * lowA[biq_freq]);
	norm = 1.0 / (1.0 + biqK / lowA[biq_reso] + biqK * biqK);
	lowA[biq_a0] = biqK * biqK * norm;
	lowA[biq_a1] = 2.0 * lowA[biq_a0];
	lowA[biq_a2] = lowA[biq_a0];
	lowA[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowA[biq_b2] = (1.0 - biqK / lowA[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * lowB[biq_freq]);
	norm = 1.0 / (1.0 + biqK / lowB[biq_reso] + biqK * biqK);
	lowB[biq_a0] = biqK * biqK * norm;
	lowB[biq_a1] = 2.0 * lowB[biq_a0];
	lowB[biq_a2] = lowB[biq_a0];
	lowB[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowB[biq_b2] = (1.0 - biqK / lowB[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * lowC[biq_freq]);
	norm = 1.0 / (1.0 + biqK / lowC[biq_reso] + biqK * biqK);
	lowC[biq_a0] = biqK * biqK * norm;
	lowC[biq_a1] = 2.0 * lowC[biq_a0];
	lowC[biq_a2] = lowC[biq_a0];
	lowC[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowC[biq_b2] = (1.0 - biqK / lowC[biq_reso] + biqK * biqK) * norm;
	//SmoothEQ2
	
	double bezCThresh = pow(1.0-I, 6.0) * 8.0;
	double bezRez = pow(1.0-I, 12.360679774997898) / overallscale;
	bezRez = fmin(fmax(bezRez,0.00001),1.0);
	int stepped = 999999; if (bezRez > 0.000001) stepped = (int)(1.0/bezRez);
	bezRez = 0.99999999 / stepped;
	double bezTrim = 1.0-(bezRez*((double)stepped/(stepped+1.0)));
	double sloRez = pow(1.0-I,10.0) / overallscale;
	sloRez = fmin(fmax(sloRez,0.00001),1.0);
	stepped = 999999; if (sloRez > 0.000001) stepped = (int)(1.0/sloRez);
	sloRez = 0.99999999 / stepped;
	double sloTrim = 1.0-(sloRez*((double)stepped/(stepped+1.0)));
	//Dynamics2
	
	inTrimA = inTrimB; inTrimB = J*2.0;
	//Console
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
				
		double trebleL = inputSampleL;		
		double outSample = (trebleL * highA[biq_a0]) + highA[biq_sL1];
		highA[biq_sL1] = (trebleL * highA[biq_a1]) - (outSample * highA[biq_b1]) + highA[biq_sL2];
		highA[biq_sL2] = (trebleL * highA[biq_a2]) - (outSample * highA[biq_b2]);
		double highmidL = outSample; trebleL -= highmidL;
		
		outSample = (highmidL * midA[biq_a0]) + midA[biq_sL1];
		midA[biq_sL1] = (highmidL * midA[biq_a1]) - (outSample * midA[biq_b1]) + midA[biq_sL2];
		midA[biq_sL2] = (highmidL * midA[biq_a2]) - (outSample * midA[biq_b2]);
		double lowmidL = outSample; highmidL -= lowmidL;
		
		outSample = (lowmidL * lowA[biq_a0]) + lowA[biq_sL1];
		lowA[biq_sL1] = (lowmidL * lowA[biq_a1]) - (outSample * lowA[biq_b1]) + lowA[biq_sL2];
		lowA[biq_sL2] = (lowmidL * lowA[biq_a2]) - (outSample * lowA[biq_b2]);
		double bassL = outSample; lowmidL -= bassL;
		
		trebleL = (bassL*bassGain) + (lowmidL*lowmidGain) + (highmidL*highmidGain) + (trebleL*trebleGain);
		//first stage of three crossovers
		
		outSample = (trebleL * highB[biq_a0]) + highB[biq_sL1];
		highB[biq_sL1] = (trebleL * highB[biq_a1]) - (outSample * highB[biq_b1]) + highB[biq_sL2];
		highB[biq_sL2] = (trebleL * highB[biq_a2]) - (outSample * highB[biq_b2]);
		highmidL = outSample; trebleL -= highmidL;
		
		outSample = (highmidL * midB[biq_a0]) + midB[biq_sL1];
		midB[biq_sL1] = (highmidL * midB[biq_a1]) - (outSample * midB[biq_b1]) + midB[biq_sL2];
		midB[biq_sL2] = (highmidL * midB[biq_a2]) - (outSample * midB[biq_b2]);
		lowmidL = outSample; highmidL -= lowmidL;
		
		outSample = (lowmidL * lowB[biq_a0]) + lowB[biq_sL1];
		lowB[biq_sL1] = (lowmidL * lowB[biq_a1]) - (outSample * lowB[biq_b1]) + lowB[biq_sL2];
		lowB[biq_sL2] = (lowmidL * lowB[biq_a2]) - (outSample * lowB[biq_b2]);
		bassL = outSample; lowmidL -= bassL;
		
		trebleL = (bassL*bassGain) + (lowmidL*lowmidGain) + (highmidL*highmidGain) + (trebleL*trebleGain);
		//second stage of three crossovers
		
		outSample = (trebleL * highC[biq_a0]) + highC[biq_sL1];
		highC[biq_sL1] = (trebleL * highC[biq_a1]) - (outSample * highC[biq_b1]) + highC[biq_sL2];
		highC[biq_sL2] = (trebleL * highC[biq_a2]) - (outSample * highC[biq_b2]);
		highmidL = outSample; trebleL -= highmidL;
		
		outSample = (highmidL * midC[biq_a0]) + midC[biq_sL1];
		midC[biq_sL1] = (highmidL * midC[biq_a1]) - (outSample * midC[biq_b1]) + midC[biq_sL2];
		midC[biq_sL2] = (highmidL * midC[biq_a2]) - (outSample * midC[biq_b2]);
		lowmidL = outSample; highmidL -= lowmidL;
		
		outSample = (lowmidL * lowC[biq_a0]) + lowC[biq_sL1];
		lowC[biq_sL1] = (lowmidL * lowC[biq_a1]) - (outSample * lowC[biq_b1]) + lowC[biq_sL2];
		lowC[biq_sL2] = (lowmidL * lowC[biq_a2]) - (outSample * lowC[biq_b2]);
		bassL = outSample; lowmidL -= bassL;
		
		trebleL = (bassL*bassGain) + (lowmidL*lowmidGain) + (highmidL*highmidGain) + (trebleL*trebleGain);
		//third stage of three crossovers
		
		highLIIR = (highLIIR*highCoef) + (trebleL*(1.0-highCoef));
		highmidL = highLIIR; trebleL -= highmidL;
		
		midLIIR = (midLIIR*midCoef) + (highmidL*(1.0-midCoef));
		lowmidL = midLIIR; highmidL -= lowmidL;
		
		lowLIIR = (lowLIIR*lowCoef) + (lowmidL*(1.0-lowCoef));
		bassL = lowLIIR; lowmidL -= bassL;
		
		inputSampleL = (bassL*bassGain) + (lowmidL*lowmidGain) + (highmidL*highmidGain) + (trebleL*trebleGain);		
		//fourth stage of three crossovers is the exponential filters
		
		
		double trebleR = inputSampleR;		
		outSample = (trebleR * highA[biq_a0]) + highA[biq_sR1];
		highA[biq_sR1] = (trebleR * highA[biq_a1]) - (outSample * highA[biq_b1]) + highA[biq_sR2];
		highA[biq_sR2] = (trebleR * highA[biq_a2]) - (outSample * highA[biq_b2]);
		double highmidR = outSample; trebleR -= highmidR;
		
		outSample = (highmidR * midA[biq_a0]) + midA[biq_sR1];
		midA[biq_sR1] = (highmidR * midA[biq_a1]) - (outSample * midA[biq_b1]) + midA[biq_sR2];
		midA[biq_sR2] = (highmidR * midA[biq_a2]) - (outSample * midA[biq_b2]);
		double lowmidR = outSample; highmidR -= lowmidR;
		
		outSample = (lowmidR * lowA[biq_a0]) + lowA[biq_sR1];
		lowA[biq_sR1] = (lowmidR * lowA[biq_a1]) - (outSample * lowA[biq_b1]) + lowA[biq_sR2];
		lowA[biq_sR2] = (lowmidR * lowA[biq_a2]) - (outSample * lowA[biq_b2]);
		double bassR = outSample; lowmidR -= bassR;
		
		trebleR = (bassR*bassGain) + (lowmidR*lowmidGain) + (highmidR*highmidGain) + (trebleR*trebleGain);
		//first stage of three crossovers
		
		outSample = (trebleR * highB[biq_a0]) + highB[biq_sR1];
		highB[biq_sR1] = (trebleR * highB[biq_a1]) - (outSample * highB[biq_b1]) + highB[biq_sR2];
		highB[biq_sR2] = (trebleR * highB[biq_a2]) - (outSample * highB[biq_b2]);
		highmidR = outSample; trebleR -= highmidR;
		
		outSample = (highmidR * midB[biq_a0]) + midB[biq_sR1];
		midB[biq_sR1] = (highmidR * midB[biq_a1]) - (outSample * midB[biq_b1]) + midB[biq_sR2];
		midB[biq_sR2] = (highmidR * midB[biq_a2]) - (outSample * midB[biq_b2]);
		lowmidR = outSample; highmidR -= lowmidR;
		
		outSample = (lowmidR * lowB[biq_a0]) + lowB[biq_sR1];
		lowB[biq_sR1] = (lowmidR * lowB[biq_a1]) - (outSample * lowB[biq_b1]) + lowB[biq_sR2];
		lowB[biq_sR2] = (lowmidR * lowB[biq_a2]) - (outSample * lowB[biq_b2]);
		bassR = outSample; lowmidR -= bassR;
		
		trebleR = (bassR*bassGain) + (lowmidR*lowmidGain) + (highmidR*highmidGain) + (trebleR*trebleGain);
		//second stage of three crossovers
		
		outSample = (trebleR * highC[biq_a0]) + highC[biq_sR1];
		highC[biq_sR1] = (trebleR * highC[biq_a1]) - (outSample * highC[biq_b1]) + highC[biq_sR2];
		highC[biq_sR2] = (trebleR * highC[biq_a2]) - (outSample * highC[biq_b2]);
		highmidR = outSample; trebleR -= highmidR;
		
		outSample = (highmidR * midC[biq_a0]) + midC[biq_sR1];
		midC[biq_sR1] = (highmidR * midC[biq_a1]) - (outSample * midC[biq_b1]) + midC[biq_sR2];
		midC[biq_sR2] = (highmidR * midC[biq_a2]) - (outSample * midC[biq_b2]);
		lowmidR = outSample; highmidR -= lowmidR;
		
		outSample = (lowmidR * lowC[biq_a0]) + lowC[biq_sR1];
		lowC[biq_sR1] = (lowmidR * lowC[biq_a1]) - (outSample * lowC[biq_b1]) + lowC[biq_sR2];
		lowC[biq_sR2] = (lowmidR * lowC[biq_a2]) - (outSample * lowC[biq_b2]);
		bassR = outSample; lowmidR -= bassR;
		
		trebleR = (bassR*bassGain) + (lowmidR*lowmidGain) + (highmidR*highmidGain) + (trebleR*trebleGain);
		//third stage of three crossovers
		
		highRIIR = (highRIIR*highCoef) + (trebleR*(1.0-highCoef));
		highmidR = highRIIR; trebleR -= highmidR;
		
		midRIIR = (midRIIR*midCoef) + (highmidR*(1.0-midCoef));
		lowmidR = midRIIR; highmidR -= lowmidR;
		
		lowRIIR = (lowRIIR*lowCoef) + (lowmidR*(1.0-lowCoef));
		bassR = lowRIIR; lowmidR -= bassR;
		
		inputSampleR = (bassR*bassGain) + (lowmidR*lowmidGain) + (highmidR*highmidGain) + (trebleR*trebleGain);		
		//fourth stage of three crossovers is the exponential filters
		
		//SmoothEQ2
		if (bezCThresh > 0.0) {
			inputSampleL *= ((bezCThresh*0.5)+1.0);
			inputSampleR *= ((bezCThresh*0.5)+1.0);
		}
		bezCompF[bez_cycle] += bezRez;
		bezCompF[bez_SampL] += (fabs(inputSampleL) * bezRez);
		bezCompF[bez_SampR] += (fabs(inputSampleR) * bezRez);
		bezMaxF = fmax(bezMaxF,fmax(fabs(inputSampleL),fabs(inputSampleR)));
		if (bezCompF[bez_cycle] > bezTrim) {
			bezCompF[bez_cycle] = 0.0;
			bezCompF[bez_CL] = bezCompF[bez_BL];
			bezCompF[bez_BL] = bezCompF[bez_AL];
			bezCompF[bez_AL] = bezCompF[bez_SampL];
			bezCompF[bez_SampL] = 0.0;
			bezCompF[bez_CR] = bezCompF[bez_BR];
			bezCompF[bez_BR] = bezCompF[bez_AR];
			bezCompF[bez_AR] = bezCompF[bez_SampR];
			bezCompF[bez_SampR] = 0.0;
			bezMaxF = 0.0;
		}
		bezCompS[bez_cycle] += sloRez;
		bezCompS[bez_SampL] += (fabs(inputSampleL) * sloRez); //note: SampL is a control voltage
		bezCompS[bez_SampR] += (fabs(inputSampleR) * sloRez); //note: SampR is a control voltage
		if (bezCompS[bez_cycle] > sloTrim) {
			bezCompS[bez_cycle] = 0.0;
			bezCompS[bez_CL] = bezCompS[bez_BL];
			bezCompS[bez_BL] = bezCompS[bez_AL];
			bezCompS[bez_AL] = bezCompS[bez_SampL];
			bezCompS[bez_SampL] = 0.0;
			bezCompS[bez_CR] = bezCompS[bez_BR];
			bezCompS[bez_BR] = bezCompS[bez_AR];
			bezCompS[bez_AR] = bezCompS[bez_SampR];
			bezCompS[bez_SampR] = 0.0;
		}
		double X = bezCompF[bez_cycle];
		double CBAFL = (bezCompF[bez_BL]+(bezCompF[bez_CL]*(1.0-X)*(1.0-X))+(bezCompF[bez_BL]*2.0*(1.0-X)*X)+(bezCompF[bez_AL]*X*X))*0.5;
		double CBAFR = (bezCompF[bez_BR]+(bezCompF[bez_CR]*(1.0-X)*(1.0-X))+(bezCompF[bez_BR]*2.0*(1.0-X)*X)+(bezCompF[bez_AR]*X*X))*0.5;
		X = bezCompS[bez_cycle];
		double CBASL = (bezCompS[bez_BL]+(bezCompS[bez_CL]*(1.0-X)*(1.0-X))+(bezCompS[bez_BL]*2.0*(1.0-X)*X)+(bezCompS[bez_AL]*X*X))*0.5;
		double CBASR = (bezCompS[bez_BR]+(bezCompS[bez_CR]*(1.0-X)*(1.0-X))+(bezCompS[bez_BR]*2.0*(1.0-X)*X)+(bezCompS[bez_AR]*X*X))*0.5;
		double CBAMax = fmax(CBASL,CBAFL); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		double CBAFade = ((CBASL*-CBAMax)+(CBAFL*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleL *= 1.0-(fmin(((CBASL*(1.0-CBAFade))+(CBAFL*CBAFade))*bezCThresh,1.0));
		CBAMax = fmax(CBASR,CBAFR); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		CBAFade = ((CBASR*-CBAMax)+(CBAFR*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleR *= 1.0-(fmin(((CBASR*(1.0-CBAFade))+(CBAFR*CBAFade))*bezCThresh,1.0));
		//Dynamics2
		
		const double temp = (double)sampleFrames/inFramesToProcess;
		double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
		inputSampleL = inputSampleL * gain;
		inputSampleR = inputSampleR * gain;
		//applies pan section, and smoothed fader gain
		
		//begin ClipOnly3 as a little, compressed chunk that can be dropped into code
		double noise = 1.0-((double(fpdL)/UINT32_MAX)*0.076);
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL<lastSampleL) lastSampleL=(0.9085097*noise)+(inputSampleL*(1.0-noise));
			else lastSampleL = 0.94; //~-0.2dB to nearly match ClipOnly and ClipOnly2
		} wasPosClipL = false;
		if (inputSampleL>0.9085097) {wasPosClipL=true;inputSampleL=(0.9085097*noise)+(lastSampleL*(1.0-noise));}
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) lastSampleL=(-0.9085097*noise)+(inputSampleL*(1.0-noise));
			else lastSampleL = -0.94;
		} wasNegClipL = false;
		if (inputSampleL<-0.9085097) {wasNegClipL=true;inputSampleL=(-0.9085097*noise)+(lastSampleL*(1.0-noise));}
		slewL[spacing*2] = fabs(lastSampleL-inputSampleL);
		for (int x = spacing*2; x > 0; x--) slewL[x-1] = slewL[x];
		intermediateL[spacing] = inputSampleL; inputSampleL = lastSampleL;
		//latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) {intermediateL[x-1] = intermediateL[x];} lastSampleL = intermediateL[0];
		if (wasPosClipL || wasNegClipL) {
			for (int x = spacing; x > 0; x--) lastSampleL += intermediateL[x];
			lastSampleL /= spacing;
		} double finalSlew = 0.0;
		for (int x = spacing*2; x >= 0; x--) if (finalSlew < slewL[x]) finalSlew = slewL[x];
		double postclip = 0.94 / (1.0+(finalSlew*1.3986013));
		if (inputSampleL > postclip) inputSampleL = postclip; if (inputSampleL < -postclip) inputSampleL = -postclip;
		
		noise = 1.0-((double(fpdR)/UINT32_MAX)*0.076);
		if (wasPosClipR == true) { //current will be over
			if (inputSampleR<lastSampleR) lastSampleR=(0.9085097*noise)+(inputSampleR*(1.0-noise));
			else lastSampleR = 0.94; //~-0.2dB to nearly match ClipOnly and ClipOnly2
		} wasPosClipR = false;
		if (inputSampleR>0.9085097) {wasPosClipR=true;inputSampleR=(0.9085097*noise)+(lastSampleR*(1.0-noise));}
		if (wasNegClipR == true) { //current will be -over
			if (inputSampleR > lastSampleR) lastSampleR=(-0.9085097*noise)+(inputSampleR*(1.0-noise));
			else lastSampleR = -0.94;
		} wasNegClipR = false;
		if (inputSampleR<-0.9085097) {wasNegClipR=true;inputSampleR=(-0.9085097*noise)+(lastSampleR*(1.0-noise));}
		slewR[spacing*2] = fabs(lastSampleR-inputSampleR);
		for (int x = spacing*2; x > 0; x--) slewR[x-1] = slewR[x];
		intermediateR[spacing] = inputSampleR; inputSampleR = lastSampleR;
		//latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) {intermediateR[x-1] = intermediateR[x];} lastSampleR = intermediateR[0];
		if (wasPosClipR || wasNegClipR) {
			for (int x = spacing; x > 0; x--) lastSampleR += intermediateR[x];
			lastSampleR /= spacing;
		} finalSlew = 0.0;
		for (int x = spacing*2; x >= 0; x--) if (finalSlew < slewR[x]) finalSlew = slewR[x];
		postclip = 0.94 / (1.0+(finalSlew*1.3986013));
		if (inputSampleR > postclip) inputSampleR = postclip; if (inputSampleR < -postclip) inputSampleR = -postclip;
		//end ClipOnly3 as a little, compressed chunk that can be dropped into code
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void X2Buss::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int spacing = floor(overallscale*2.0);
	if (spacing < 2) spacing = 2; if (spacing > 32) spacing = 32;
	
	double trebleGain = (A-0.5)*2.0;
	trebleGain = 1.0+(trebleGain*fabs(trebleGain)*fabs(trebleGain));
	double highmidGain = (B-0.5)*2.0;
	highmidGain = 1.0+(highmidGain*fabs(highmidGain)*fabs(highmidGain));
	double lowmidGain = (C-0.5)*2.0;
	lowmidGain = 1.0+(lowmidGain*fabs(lowmidGain)*fabs(lowmidGain));
	double bassGain = (D-0.5)*2.0;
	bassGain = 1.0+(bassGain*fabs(bassGain)*fabs(bassGain));
	
	double trebleRef = E-0.5;
	double highmidRef = F-0.5;
	double lowmidRef = G-0.5;
	double bassRef = H-0.5;
	double highF = 0.75 + ((trebleRef+trebleRef+trebleRef+highmidRef)*0.125);
	double bassF = 0.25 + ((lowmidRef+bassRef+bassRef+bassRef)*0.125);
	double midF = (highF*0.5) + (bassF*0.5) + ((highmidRef+lowmidRef)*0.125);
	
	double highQ = fmax(fmin(1.0+(highmidRef-trebleRef),4.0),0.125);
	double midQ = fmax(fmin(1.0+(lowmidRef-highmidRef),4.0),0.125);
	double lowQ = fmax(fmin(1.0+(bassRef-lowmidRef),4.0),0.125);
	
	highA[biq_freq] = ((pow(highF,3)*20000.0)/getSampleRate());
	highC[biq_freq] = highB[biq_freq] = highA[biq_freq] = fmax(fmin(highA[biq_freq],0.4999),0.00025);
	double highFreq = pow(highF,3)*20000.0;
	double omega = 2.0*M_PI*(highFreq/getSampleRate());
	double biqK = 2.0-cos(omega);
	double highCoef = -sqrt((biqK*biqK)-1.0)+biqK;
	highA[biq_reso] = 2.24697960 * highQ;
	highB[biq_reso] = 0.80193774 * highQ;
	highC[biq_reso] = 0.55495813 * highQ;
	
	midA[biq_freq] = ((pow(midF,3)*20000.0)/getSampleRate());
	midC[biq_freq] = midB[biq_freq] = midA[biq_freq] = fmax(fmin(midA[biq_freq],0.4999),0.00025);	
	double midFreq = pow(midF,3)*20000.0;
	omega = 2.0*M_PI*(midFreq/getSampleRate());
	biqK = 2.0-cos(omega);
	double midCoef = -sqrt((biqK*biqK)-1.0)+biqK;
	midA[biq_reso] = 2.24697960 * midQ;
	midB[biq_reso] = 0.80193774 * midQ;
	midC[biq_reso] = 0.55495813 * midQ;
	
	lowA[biq_freq] = ((pow(bassF,3)*20000.0)/getSampleRate());
	lowC[biq_freq] = lowB[biq_freq] = lowA[biq_freq] = fmax(fmin(lowA[biq_freq],0.4999),0.00025);
	double lowFreq = pow(bassF,3)*20000.0;
	omega = 2.0*M_PI*(lowFreq/getSampleRate());
	biqK = 2.0-cos(omega);
	double lowCoef = -sqrt((biqK*biqK)-1.0)+biqK;
	lowA[biq_reso] = 2.24697960 * lowQ;
	lowB[biq_reso] = 0.80193774 * lowQ;
	lowC[biq_reso] = 0.55495813 * lowQ;
	
	biqK = tan(M_PI * highA[biq_freq]);
	double norm = 1.0 / (1.0 + biqK / highA[biq_reso] + biqK * biqK);
	highA[biq_a0] = biqK * biqK * norm;
	highA[biq_a1] = 2.0 * highA[biq_a0];
	highA[biq_a2] = highA[biq_a0];
	highA[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highA[biq_b2] = (1.0 - biqK / highA[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * highB[biq_freq]);
	norm = 1.0 / (1.0 + biqK / highB[biq_reso] + biqK * biqK);
	highB[biq_a0] = biqK * biqK * norm;
	highB[biq_a1] = 2.0 * highB[biq_a0];
	highB[biq_a2] = highB[biq_a0];
	highB[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highB[biq_b2] = (1.0 - biqK / highB[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * highC[biq_freq]);
	norm = 1.0 / (1.0 + biqK / highC[biq_reso] + biqK * biqK);
	highC[biq_a0] = biqK * biqK * norm;
	highC[biq_a1] = 2.0 * highC[biq_a0];
	highC[biq_a2] = highC[biq_a0];
	highC[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highC[biq_b2] = (1.0 - biqK / highC[biq_reso] + biqK * biqK) * norm;
	
	biqK = tan(M_PI * midA[biq_freq]);
	norm = 1.0 / (1.0 + biqK / midA[biq_reso] + biqK * biqK);
	midA[biq_a0] = biqK * biqK * norm;
	midA[biq_a1] = 2.0 * midA[biq_a0];
	midA[biq_a2] = midA[biq_a0];
	midA[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	midA[biq_b2] = (1.0 - biqK / midA[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * midB[biq_freq]);
	norm = 1.0 / (1.0 + biqK / midB[biq_reso] + biqK * biqK);
	midB[biq_a0] = biqK * biqK * norm;
	midB[biq_a1] = 2.0 * midB[biq_a0];
	midB[biq_a2] = midB[biq_a0];
	midB[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	midB[biq_b2] = (1.0 - biqK / midB[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * midC[biq_freq]);
	norm = 1.0 / (1.0 + biqK / midC[biq_reso] + biqK * biqK);
	midC[biq_a0] = biqK * biqK * norm;
	midC[biq_a1] = 2.0 * midC[biq_a0];
	midC[biq_a2] = midC[biq_a0];
	midC[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	midC[biq_b2] = (1.0 - biqK / midC[biq_reso] + biqK * biqK) * norm;
	
	biqK = tan(M_PI * lowA[biq_freq]);
	norm = 1.0 / (1.0 + biqK / lowA[biq_reso] + biqK * biqK);
	lowA[biq_a0] = biqK * biqK * norm;
	lowA[biq_a1] = 2.0 * lowA[biq_a0];
	lowA[biq_a2] = lowA[biq_a0];
	lowA[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowA[biq_b2] = (1.0 - biqK / lowA[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * lowB[biq_freq]);
	norm = 1.0 / (1.0 + biqK / lowB[biq_reso] + biqK * biqK);
	lowB[biq_a0] = biqK * biqK * norm;
	lowB[biq_a1] = 2.0 * lowB[biq_a0];
	lowB[biq_a2] = lowB[biq_a0];
	lowB[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowB[biq_b2] = (1.0 - biqK / lowB[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * lowC[biq_freq]);
	norm = 1.0 / (1.0 + biqK / lowC[biq_reso] + biqK * biqK);
	lowC[biq_a0] = biqK * biqK * norm;
	lowC[biq_a1] = 2.0 * lowC[biq_a0];
	lowC[biq_a2] = lowC[biq_a0];
	lowC[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowC[biq_b2] = (1.0 - biqK / lowC[biq_reso] + biqK * biqK) * norm;
	//SmoothEQ2
	
	double bezCThresh = pow(1.0-I, 6.0) * 8.0;
	double bezRez = pow(1.0-I, 12.360679774997898) / overallscale;
	bezRez = fmin(fmax(bezRez,0.00001),1.0);
	int stepped = 999999; if (bezRez > 0.000001) stepped = (int)(1.0/bezRez);
	bezRez = 0.99999999 / stepped;
	double bezTrim = 1.0-(bezRez*((double)stepped/(stepped+1.0)));
	double sloRez = pow(1.0-I,10.0) / overallscale;
	sloRez = fmin(fmax(sloRez,0.00001),1.0);
	stepped = 999999; if (sloRez > 0.000001) stepped = (int)(1.0/sloRez);
	sloRez = 0.99999999 / stepped;
	double sloTrim = 1.0-(sloRez*((double)stepped/(stepped+1.0)));
	//Dynamics2
	
	inTrimA = inTrimB; inTrimB = J*2.0;
	//Console
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		else if (inputSampleL > 0.0) inputSampleL = -expm1((log1p(-inputSampleL) * 0.6180339887498949));
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		else if (inputSampleL < 0.0) inputSampleL = expm1((log1p(inputSampleL) * 0.6180339887498949));
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		else if (inputSampleR > 0.0) inputSampleR = -expm1((log1p(-inputSampleR) * 0.6180339887498949));
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		else if (inputSampleR < 0.0) inputSampleR = expm1((log1p(inputSampleR) * 0.6180339887498949));
		
		double trebleL = inputSampleL;		
		double outSample = (trebleL * highA[biq_a0]) + highA[biq_sL1];
		highA[biq_sL1] = (trebleL * highA[biq_a1]) - (outSample * highA[biq_b1]) + highA[biq_sL2];
		highA[biq_sL2] = (trebleL * highA[biq_a2]) - (outSample * highA[biq_b2]);
		double highmidL = outSample; trebleL -= highmidL;
		
		outSample = (highmidL * midA[biq_a0]) + midA[biq_sL1];
		midA[biq_sL1] = (highmidL * midA[biq_a1]) - (outSample * midA[biq_b1]) + midA[biq_sL2];
		midA[biq_sL2] = (highmidL * midA[biq_a2]) - (outSample * midA[biq_b2]);
		double lowmidL = outSample; highmidL -= lowmidL;
		
		outSample = (lowmidL * lowA[biq_a0]) + lowA[biq_sL1];
		lowA[biq_sL1] = (lowmidL * lowA[biq_a1]) - (outSample * lowA[biq_b1]) + lowA[biq_sL2];
		lowA[biq_sL2] = (lowmidL * lowA[biq_a2]) - (outSample * lowA[biq_b2]);
		double bassL = outSample; lowmidL -= bassL;
		
		trebleL = (bassL*bassGain) + (lowmidL*lowmidGain) + (highmidL*highmidGain) + (trebleL*trebleGain);
		//first stage of three crossovers
		
		outSample = (trebleL * highB[biq_a0]) + highB[biq_sL1];
		highB[biq_sL1] = (trebleL * highB[biq_a1]) - (outSample * highB[biq_b1]) + highB[biq_sL2];
		highB[biq_sL2] = (trebleL * highB[biq_a2]) - (outSample * highB[biq_b2]);
		highmidL = outSample; trebleL -= highmidL;
		
		outSample = (highmidL * midB[biq_a0]) + midB[biq_sL1];
		midB[biq_sL1] = (highmidL * midB[biq_a1]) - (outSample * midB[biq_b1]) + midB[biq_sL2];
		midB[biq_sL2] = (highmidL * midB[biq_a2]) - (outSample * midB[biq_b2]);
		lowmidL = outSample; highmidL -= lowmidL;
		
		outSample = (lowmidL * lowB[biq_a0]) + lowB[biq_sL1];
		lowB[biq_sL1] = (lowmidL * lowB[biq_a1]) - (outSample * lowB[biq_b1]) + lowB[biq_sL2];
		lowB[biq_sL2] = (lowmidL * lowB[biq_a2]) - (outSample * lowB[biq_b2]);
		bassL = outSample; lowmidL -= bassL;
		
		trebleL = (bassL*bassGain) + (lowmidL*lowmidGain) + (highmidL*highmidGain) + (trebleL*trebleGain);
		//second stage of three crossovers
		
		outSample = (trebleL * highC[biq_a0]) + highC[biq_sL1];
		highC[biq_sL1] = (trebleL * highC[biq_a1]) - (outSample * highC[biq_b1]) + highC[biq_sL2];
		highC[biq_sL2] = (trebleL * highC[biq_a2]) - (outSample * highC[biq_b2]);
		highmidL = outSample; trebleL -= highmidL;
		
		outSample = (highmidL * midC[biq_a0]) + midC[biq_sL1];
		midC[biq_sL1] = (highmidL * midC[biq_a1]) - (outSample * midC[biq_b1]) + midC[biq_sL2];
		midC[biq_sL2] = (highmidL * midC[biq_a2]) - (outSample * midC[biq_b2]);
		lowmidL = outSample; highmidL -= lowmidL;
		
		outSample = (lowmidL * lowC[biq_a0]) + lowC[biq_sL1];
		lowC[biq_sL1] = (lowmidL * lowC[biq_a1]) - (outSample * lowC[biq_b1]) + lowC[biq_sL2];
		lowC[biq_sL2] = (lowmidL * lowC[biq_a2]) - (outSample * lowC[biq_b2]);
		bassL = outSample; lowmidL -= bassL;
		
		trebleL = (bassL*bassGain) + (lowmidL*lowmidGain) + (highmidL*highmidGain) + (trebleL*trebleGain);
		//third stage of three crossovers
		
		highLIIR = (highLIIR*highCoef) + (trebleL*(1.0-highCoef));
		highmidL = highLIIR; trebleL -= highmidL;
		
		midLIIR = (midLIIR*midCoef) + (highmidL*(1.0-midCoef));
		lowmidL = midLIIR; highmidL -= lowmidL;
		
		lowLIIR = (lowLIIR*lowCoef) + (lowmidL*(1.0-lowCoef));
		bassL = lowLIIR; lowmidL -= bassL;
		
		inputSampleL = (bassL*bassGain) + (lowmidL*lowmidGain) + (highmidL*highmidGain) + (trebleL*trebleGain);		
		//fourth stage of three crossovers is the exponential filters
		
		
		double trebleR = inputSampleR;		
		outSample = (trebleR * highA[biq_a0]) + highA[biq_sR1];
		highA[biq_sR1] = (trebleR * highA[biq_a1]) - (outSample * highA[biq_b1]) + highA[biq_sR2];
		highA[biq_sR2] = (trebleR * highA[biq_a2]) - (outSample * highA[biq_b2]);
		double highmidR = outSample; trebleR -= highmidR;
		
		outSample = (highmidR * midA[biq_a0]) + midA[biq_sR1];
		midA[biq_sR1] = (highmidR * midA[biq_a1]) - (outSample * midA[biq_b1]) + midA[biq_sR2];
		midA[biq_sR2] = (highmidR * midA[biq_a2]) - (outSample * midA[biq_b2]);
		double lowmidR = outSample; highmidR -= lowmidR;
		
		outSample = (lowmidR * lowA[biq_a0]) + lowA[biq_sR1];
		lowA[biq_sR1] = (lowmidR * lowA[biq_a1]) - (outSample * lowA[biq_b1]) + lowA[biq_sR2];
		lowA[biq_sR2] = (lowmidR * lowA[biq_a2]) - (outSample * lowA[biq_b2]);
		double bassR = outSample; lowmidR -= bassR;
		
		trebleR = (bassR*bassGain) + (lowmidR*lowmidGain) + (highmidR*highmidGain) + (trebleR*trebleGain);
		//first stage of three crossovers
		
		outSample = (trebleR * highB[biq_a0]) + highB[biq_sR1];
		highB[biq_sR1] = (trebleR * highB[biq_a1]) - (outSample * highB[biq_b1]) + highB[biq_sR2];
		highB[biq_sR2] = (trebleR * highB[biq_a2]) - (outSample * highB[biq_b2]);
		highmidR = outSample; trebleR -= highmidR;
		
		outSample = (highmidR * midB[biq_a0]) + midB[biq_sR1];
		midB[biq_sR1] = (highmidR * midB[biq_a1]) - (outSample * midB[biq_b1]) + midB[biq_sR2];
		midB[biq_sR2] = (highmidR * midB[biq_a2]) - (outSample * midB[biq_b2]);
		lowmidR = outSample; highmidR -= lowmidR;
		
		outSample = (lowmidR * lowB[biq_a0]) + lowB[biq_sR1];
		lowB[biq_sR1] = (lowmidR * lowB[biq_a1]) - (outSample * lowB[biq_b1]) + lowB[biq_sR2];
		lowB[biq_sR2] = (lowmidR * lowB[biq_a2]) - (outSample * lowB[biq_b2]);
		bassR = outSample; lowmidR -= bassR;
		
		trebleR = (bassR*bassGain) + (lowmidR*lowmidGain) + (highmidR*highmidGain) + (trebleR*trebleGain);
		//second stage of three crossovers
		
		outSample = (trebleR * highC[biq_a0]) + highC[biq_sR1];
		highC[biq_sR1] = (trebleR * highC[biq_a1]) - (outSample * highC[biq_b1]) + highC[biq_sR2];
		highC[biq_sR2] = (trebleR * highC[biq_a2]) - (outSample * highC[biq_b2]);
		highmidR = outSample; trebleR -= highmidR;
		
		outSample = (highmidR * midC[biq_a0]) + midC[biq_sR1];
		midC[biq_sR1] = (highmidR * midC[biq_a1]) - (outSample * midC[biq_b1]) + midC[biq_sR2];
		midC[biq_sR2] = (highmidR * midC[biq_a2]) - (outSample * midC[biq_b2]);
		lowmidR = outSample; highmidR -= lowmidR;
		
		outSample = (lowmidR * lowC[biq_a0]) + lowC[biq_sR1];
		lowC[biq_sR1] = (lowmidR * lowC[biq_a1]) - (outSample * lowC[biq_b1]) + lowC[biq_sR2];
		lowC[biq_sR2] = (lowmidR * lowC[biq_a2]) - (outSample * lowC[biq_b2]);
		bassR = outSample; lowmidR -= bassR;
		
		trebleR = (bassR*bassGain) + (lowmidR*lowmidGain) + (highmidR*highmidGain) + (trebleR*trebleGain);
		//third stage of three crossovers
		
		highRIIR = (highRIIR*highCoef) + (trebleR*(1.0-highCoef));
		highmidR = highRIIR; trebleR -= highmidR;
		
		midRIIR = (midRIIR*midCoef) + (highmidR*(1.0-midCoef));
		lowmidR = midRIIR; highmidR -= lowmidR;
		
		lowRIIR = (lowRIIR*lowCoef) + (lowmidR*(1.0-lowCoef));
		bassR = lowRIIR; lowmidR -= bassR;
		
		inputSampleR = (bassR*bassGain) + (lowmidR*lowmidGain) + (highmidR*highmidGain) + (trebleR*trebleGain);		
		//fourth stage of three crossovers is the exponential filters
		
		//SmoothEQ2
		if (bezCThresh > 0.0) {
			inputSampleL *= ((bezCThresh*0.5)+1.0);
			inputSampleR *= ((bezCThresh*0.5)+1.0);
		}
		bezCompF[bez_cycle] += bezRez;
		bezCompF[bez_SampL] += (fabs(inputSampleL) * bezRez);
		bezCompF[bez_SampR] += (fabs(inputSampleR) * bezRez);
		bezMaxF = fmax(bezMaxF,fmax(fabs(inputSampleL),fabs(inputSampleR)));
		if (bezCompF[bez_cycle] > bezTrim) {
			bezCompF[bez_cycle] = 0.0;
			bezCompF[bez_CL] = bezCompF[bez_BL];
			bezCompF[bez_BL] = bezCompF[bez_AL];
			bezCompF[bez_AL] = bezCompF[bez_SampL];
			bezCompF[bez_SampL] = 0.0;
			bezCompF[bez_CR] = bezCompF[bez_BR];
			bezCompF[bez_BR] = bezCompF[bez_AR];
			bezCompF[bez_AR] = bezCompF[bez_SampR];
			bezCompF[bez_SampR] = 0.0;
			bezMaxF = 0.0;
		}
		bezCompS[bez_cycle] += sloRez;
		bezCompS[bez_SampL] += (fabs(inputSampleL) * sloRez); //note: SampL is a control voltage
		bezCompS[bez_SampR] += (fabs(inputSampleR) * sloRez); //note: SampR is a control voltage
		if (bezCompS[bez_cycle] > sloTrim) {
			bezCompS[bez_cycle] = 0.0;
			bezCompS[bez_CL] = bezCompS[bez_BL];
			bezCompS[bez_BL] = bezCompS[bez_AL];
			bezCompS[bez_AL] = bezCompS[bez_SampL];
			bezCompS[bez_SampL] = 0.0;
			bezCompS[bez_CR] = bezCompS[bez_BR];
			bezCompS[bez_BR] = bezCompS[bez_AR];
			bezCompS[bez_AR] = bezCompS[bez_SampR];
			bezCompS[bez_SampR] = 0.0;
		}
		double X = bezCompF[bez_cycle];
		double CBAFL = (bezCompF[bez_BL]+(bezCompF[bez_CL]*(1.0-X)*(1.0-X))+(bezCompF[bez_BL]*2.0*(1.0-X)*X)+(bezCompF[bez_AL]*X*X))*0.5;
		double CBAFR = (bezCompF[bez_BR]+(bezCompF[bez_CR]*(1.0-X)*(1.0-X))+(bezCompF[bez_BR]*2.0*(1.0-X)*X)+(bezCompF[bez_AR]*X*X))*0.5;
		X = bezCompS[bez_cycle];
		double CBASL = (bezCompS[bez_BL]+(bezCompS[bez_CL]*(1.0-X)*(1.0-X))+(bezCompS[bez_BL]*2.0*(1.0-X)*X)+(bezCompS[bez_AL]*X*X))*0.5;
		double CBASR = (bezCompS[bez_BR]+(bezCompS[bez_CR]*(1.0-X)*(1.0-X))+(bezCompS[bez_BR]*2.0*(1.0-X)*X)+(bezCompS[bez_AR]*X*X))*0.5;
		double CBAMax = fmax(CBASL,CBAFL); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		double CBAFade = ((CBASL*-CBAMax)+(CBAFL*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleL *= 1.0-(fmin(((CBASL*(1.0-CBAFade))+(CBAFL*CBAFade))*bezCThresh,1.0));
		CBAMax = fmax(CBASR,CBAFR); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		CBAFade = ((CBASR*-CBAMax)+(CBAFR*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleR *= 1.0-(fmin(((CBASR*(1.0-CBAFade))+(CBAFR*CBAFade))*bezCThresh,1.0));
		//Dynamics2
		
		const double temp = (double)sampleFrames/inFramesToProcess;
		double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
		inputSampleL = inputSampleL * gain;
		inputSampleR = inputSampleR * gain;
		//applies pan section, and smoothed fader gain
		
		//begin ClipOnly3 as a little, compressed chunk that can be dropped into code
		double noise = 1.0-((double(fpdL)/UINT32_MAX)*0.076);
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL<lastSampleL) lastSampleL=(0.9085097*noise)+(inputSampleL*(1.0-noise));
			else lastSampleL = 0.94; //~-0.2dB to nearly match ClipOnly and ClipOnly2
		} wasPosClipL = false;
		if (inputSampleL>0.9085097) {wasPosClipL=true;inputSampleL=(0.9085097*noise)+(lastSampleL*(1.0-noise));}
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) lastSampleL=(-0.9085097*noise)+(inputSampleL*(1.0-noise));
			else lastSampleL = -0.94;
		} wasNegClipL = false;
		if (inputSampleL<-0.9085097) {wasNegClipL=true;inputSampleL=(-0.9085097*noise)+(lastSampleL*(1.0-noise));}
		slewL[spacing*2] = fabs(lastSampleL-inputSampleL);
		for (int x = spacing*2; x > 0; x--) slewL[x-1] = slewL[x];
		intermediateL[spacing] = inputSampleL; inputSampleL = lastSampleL;
		//latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) {intermediateL[x-1] = intermediateL[x];} lastSampleL = intermediateL[0];
		if (wasPosClipL || wasNegClipL) {
			for (int x = spacing; x > 0; x--) lastSampleL += intermediateL[x];
			lastSampleL /= spacing;
		} double finalSlew = 0.0;
		for (int x = spacing*2; x >= 0; x--) if (finalSlew < slewL[x]) finalSlew = slewL[x];
		double postclip = 0.94 / (1.0+(finalSlew*1.3986013));
		if (inputSampleL > postclip) inputSampleL = postclip; if (inputSampleL < -postclip) inputSampleL = -postclip;
		
		noise = 1.0-((double(fpdR)/UINT32_MAX)*0.076);
		if (wasPosClipR == true) { //current will be over
			if (inputSampleR<lastSampleR) lastSampleR=(0.9085097*noise)+(inputSampleR*(1.0-noise));
			else lastSampleR = 0.94; //~-0.2dB to nearly match ClipOnly and ClipOnly2
		} wasPosClipR = false;
		if (inputSampleR>0.9085097) {wasPosClipR=true;inputSampleR=(0.9085097*noise)+(lastSampleR*(1.0-noise));}
		if (wasNegClipR == true) { //current will be -over
			if (inputSampleR > lastSampleR) lastSampleR=(-0.9085097*noise)+(inputSampleR*(1.0-noise));
			else lastSampleR = -0.94;
		} wasNegClipR = false;
		if (inputSampleR<-0.9085097) {wasNegClipR=true;inputSampleR=(-0.9085097*noise)+(lastSampleR*(1.0-noise));}
		slewR[spacing*2] = fabs(lastSampleR-inputSampleR);
		for (int x = spacing*2; x > 0; x--) slewR[x-1] = slewR[x];
		intermediateR[spacing] = inputSampleR; inputSampleR = lastSampleR;
		//latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) {intermediateR[x-1] = intermediateR[x];} lastSampleR = intermediateR[0];
		if (wasPosClipR || wasNegClipR) {
			for (int x = spacing; x > 0; x--) lastSampleR += intermediateR[x];
			lastSampleR /= spacing;
		} finalSlew = 0.0;
		for (int x = spacing*2; x >= 0; x--) if (finalSlew < slewR[x]) finalSlew = slewR[x];
		postclip = 0.94 / (1.0+(finalSlew*1.3986013));
		if (inputSampleR > postclip) inputSampleR = postclip; if (inputSampleR < -postclip) inputSampleR = -postclip;
		//end ClipOnly3 as a little, compressed chunk that can be dropped into code
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
