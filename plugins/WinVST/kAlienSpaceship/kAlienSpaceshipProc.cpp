/* ========================================
 *  kAlienSpaceship - kAlienSpaceship.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __kAlienSpaceship_H
#include "kAlienSpaceship.h"
#endif

void kAlienSpaceship::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double fdb6ck = (0.0009765625+0.0009765625+0.001953125)*0.3333333;
	double reg6n = (1.0-pow(1.0-A,3.0))*fdb6ck;
	//start this but pad it in the loop by volume of output?
	double derez = B/overallscale;
	derez = 1.0 / ((int)(1.0/derez));
	if (derez < 0.0005) derez = 0.0005; if (derez > 1.0) derez = 1.0;
	double derezFreq = pow(C,3.0)+0.01;
	if (derezFreq > 1.0) derezFreq = 1.0;
	double earlyLoudness = D;
	int start = (int)(E * 27.0);
	int ld3G = early[start]; 
	int ld3H = early[start+1]; 
	int ld3D = early[start+2]; 
	int ld3A = early[start+3]; 
	int ld3E = early[start+4]; 
	int ld3I = early[start+5];
	int ld3F = early[start+6];
	int ld3B = early[start+7]; 
	int ld3C = early[start+8];
	double wet = F;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += ((inputSampleL+bez[bez_InL]) * derez);
		bez[bez_SampR] += ((inputSampleR+bez[bez_InR]) * derez);
		bez[bez_InL] = inputSampleL; bez[bez_InR] = inputSampleR;
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
						
			inputSampleL = bez[bez_SampL];
			inputSampleR = bez[bez_SampR];
			
			a3AL[c3AL] = inputSampleL;// + (f3AL * reg3n);
			a3BL[c3BL] = inputSampleL;// + (f3BL * reg3n);
			a3CL[c3CL] = inputSampleL;// + (f3CL * reg3n);
			
			a3CR[c3CR] = inputSampleR;// + (f3CR * reg3n);
			a3FR[c3FR] = inputSampleR;// + (f3FR * reg3n);
			a3IR[c3IR] = inputSampleR;// + (f3IR * reg3n);
			
			c3AL++; if (c3AL < 0 || c3AL > ld3A) c3AL = 0;
			c3BL++; if (c3BL < 0 || c3BL > ld3B) c3BL = 0;
			c3CL++; if (c3CL < 0 || c3CL > ld3C) c3CL = 0;
			c3CR++; if (c3CR < 0 || c3CR > ld3C) c3CR = 0;
			c3FR++; if (c3FR < 0 || c3FR > ld3F) c3FR = 0;
			c3IR++; if (c3IR < 0 || c3IR > ld3I) c3IR = 0;
			
			double o3AL = a3AL[c3AL-((c3AL > ld3A)?c3AL+1:0)];
			double o3BL = a3BL[c3BL-((c3BL > ld3B)?c3BL+1:0)];
			double o3CL = a3CL[c3CL-((c3CL > ld3C)?c3CL+1:0)];
			double o3CR = a3CR[c3CR-((c3CR > ld3C)?c3CR+1:0)];
			double o3FR = a3FR[c3FR-((c3FR > ld3F)?c3FR+1:0)];
			double o3IR = a3IR[c3IR-((c3IR > ld3I)?c3IR+1:0)];
			
			a3DL[c3DL] = (((o3BL + o3CL) * -2.0) + o3AL);
			a3EL[c3EL] = (((o3AL + o3CL) * -2.0) + o3BL);
			a3FL[c3FL] = (((o3AL + o3BL) * -2.0) + o3CL);
			a3BR[c3BR] = (((o3FR + o3IR) * -2.0) + o3CR);
			a3ER[c3ER] = (((o3CR + o3IR) * -2.0) + o3FR);
			a3HR[c3HR] = (((o3CR + o3FR) * -2.0) + o3IR);
			
			c3DL++; if (c3DL < 0 || c3DL > ld3D) c3DL = 0;
			c3EL++; if (c3EL < 0 || c3EL > ld3E) c3EL = 0;
			c3FL++; if (c3FL < 0 || c3FL > ld3F) c3FL = 0;
			c3BR++; if (c3BR < 0 || c3BR > ld3B) c3BR = 0;
			c3ER++; if (c3ER < 0 || c3ER > ld3E) c3ER = 0;
			c3HR++; if (c3HR < 0 || c3HR > ld3H) c3HR = 0;
			
			double o3DL = a3DL[c3DL-((c3DL > ld3D)?c3DL+1:0)];
			double o3EL = a3EL[c3EL-((c3EL > ld3E)?c3EL+1:0)];
			double o3FL = a3FL[c3FL-((c3FL > ld3F)?c3FL+1:0)];
			double o3BR = a3BR[c3BR-((c3BR > ld3B)?c3BR+1:0)];
			double o3ER = a3ER[c3ER-((c3ER > ld3E)?c3ER+1:0)];
			double o3HR = a3HR[c3HR-((c3HR > ld3H)?c3HR+1:0)];
			
			a3GL[c3GL] = (((o3EL + o3FL) * -2.0) + o3DL);
			a3HL[c3HL] = (((o3DL + o3FL) * -2.0) + o3EL);
			a3IL[c3IL] = (((o3DL + o3EL) * -2.0) + o3FL);
			a3AR[c3AR] = (((o3ER + o3HR) * -2.0) + o3BR);
			a3DR[c3DR] = (((o3BR + o3HR) * -2.0) + o3ER);
			a3GR[c3GR] = (((o3BR + o3ER) * -2.0) + o3HR);
			
			c3GL++; if (c3GL < 0 || c3GL > ld3G) c3GL = 0;
			c3HL++; if (c3HL < 0 || c3HL > ld3H) c3HL = 0;
			c3IL++; if (c3IL < 0 || c3IL > ld3I) c3IL = 0;
			c3AR++; if (c3AR < 0 || c3AR > ld3A) c3AR = 0;
			c3DR++; if (c3DR < 0 || c3DR > ld3D) c3DR = 0;
			c3GR++; if (c3GR < 0 || c3GR > ld3G) c3GR = 0;
			
			double o3GL = a3GL[c3GL-((c3GL > ld3G)?c3GL+1:0)];
			double o3HL = a3HL[c3HL-((c3HL > ld3H)?c3HL+1:0)];
			double o3IL = a3IL[c3IL-((c3IL > ld3I)?c3IL+1:0)];
			double o3AR = a3AR[c3AR-((c3AR > ld3A)?c3AR+1:0)];
			double o3DR = a3DR[c3DR-((c3DR > ld3D)?c3DR+1:0)];
			double o3GR = a3GR[c3GR-((c3GR > ld3G)?c3GR+1:0)];
			
			double inputSampleL = (o3GL + o3HL + o3IL)*0.03125;
			double inputSampleR = (o3AR + o3DR + o3GR)*0.03125;
			
			bezF[bez_cycle] += derezFreq;
			bezF[bez_SampL] += ((inputSampleL+bezF[bez_InL]) * derezFreq);
			bezF[bez_SampR] += ((inputSampleL+bezF[bez_InR]) * derezFreq);
			bezF[bez_InL] = inputSampleL; bezF[bez_InR] = inputSampleR;
			if (bezF[bez_cycle] > 1.0) { //hit the end point and we do a filter sample
				bezF[bez_cycle] -= 1.0;
				bezF[bez_CL] = bezF[bez_BL];
				bezF[bez_BL] = bezF[bez_AL];
				bezF[bez_AL] = inputSampleL;
				bezF[bez_SampL] = 0.0;
				bezF[bez_CR] = bezF[bez_BR];
				bezF[bez_BR] = bezF[bez_AR];
				bezF[bez_AR] = inputSampleR;
				bezF[bez_SampR] = 0.0;
			}
			double CBLfreq = (bezF[bez_CL]*(1.0-bezF[bez_cycle]))+(bezF[bez_BL]*bezF[bez_cycle]);
			double BALfreq = (bezF[bez_BL]*(1.0-bezF[bez_cycle]))+(bezF[bez_AL]*bezF[bez_cycle]);
			inputSampleL = (bezF[bez_BL]+(CBLfreq*(1.0-bezF[bez_cycle]))+(BALfreq*bezF[bez_cycle]))*0.5;
			double CBRfreq = (bezF[bez_CR]*(1.0-bezF[bez_cycle]))+(bezF[bez_BR]*bezF[bez_cycle]);
			double BARfreq = (bezF[bez_BR]*(1.0-bezF[bez_cycle]))+(bezF[bez_AR]*bezF[bez_cycle]);
			inputSampleR = (bezF[bez_BR]+(CBRfreq*(1.0-bezF[bez_cycle]))+(BARfreq*bezF[bez_cycle]))*0.5;
			
			double earlyReflectionL = inputSampleL;
			double earlyReflectionR = inputSampleR; //for more alienness, early reflections are DeRezzed
			
			a6AL[c6AL] = inputSampleL + (f6BL * reg6n);
			a6BL[c6BL] = inputSampleL + (f6CL * reg6n);
			a6CL[c6CL] = inputSampleL + (f6DL * reg6n);
			a6DL[c6DL] = inputSampleL + (f6EL * reg6n);
			a6EL[c6EL] = inputSampleL + (f6FL * reg6n);
			a6FL[c6FL] = inputSampleL + (f6AL * reg6n);
			
			c6AL++; if (c6AL < 0 || c6AL > d6A) c6AL = 0;
			c6BL++; if (c6BL < 0 || c6BL > d6B) c6BL = 0;
			c6CL++; if (c6CL < 0 || c6CL > d6C) c6CL = 0;
			c6DL++; if (c6DL < 0 || c6DL > d6D) c6DL = 0;
			c6EL++; if (c6EL < 0 || c6EL > d6E) c6EL = 0;
			c6FL++; if (c6FL < 0 || c6FL > d6F) c6FL = 0;
			
			double o6AL = a6AL[c6AL-((c6AL > d6A)?d6A+1:0)];
			double o6BL = a6BL[c6BL-((c6BL > d6B)?d6B+1:0)];
			double o6CL = a6CL[c6CL-((c6CL > d6C)?d6C+1:0)];
			double o6DL = a6DL[c6DL-((c6DL > d6D)?d6D+1:0)];
			double o6EL = a6EL[c6EL-((c6EL > d6E)?d6E+1:0)];
			double o6FL = a6FL[c6FL-((c6FL > d6F)?d6F+1:0)];
			
			a6FR[c6FR] = inputSampleR + (f6LR * reg6n);
			a6LR[c6LR] = inputSampleR + (f6RR * reg6n);
			a6RR[c6RR] = inputSampleR + (f6XR * reg6n);
			a6XR[c6XR] = inputSampleR + (f6ZER * reg6n);
			a6ZER[c6ZER] = inputSampleR + (f6ZKR * reg6n);
			a6ZKR[c6ZKR] = inputSampleR + (f6FR * reg6n);
			
			c6FR++; if (c6FR < 0 || c6FR > d6F) c6FR = 0;
			c6LR++; if (c6LR < 0 || c6LR > d6L) c6LR = 0;
			c6RR++; if (c6RR < 0 || c6RR > d6R) c6RR = 0;
			c6XR++; if (c6XR < 0 || c6XR > d6X) c6XR = 0;
			c6ZER++; if (c6ZER < 0 || c6ZER > d6ZE) c6ZER = 0;
			c6ZKR++; if (c6ZKR < 0 || c6ZKR > d6ZK) c6ZKR = 0;
			
			double o6FR = a6FR[c6FR-((c6FR > d6F)?d6F+1:0)];
			double o6LR = a6LR[c6LR-((c6LR > d6L)?d6L+1:0)];
			double o6RR = a6RR[c6RR-((c6RR > d6R)?d6R+1:0)];
			double o6XR = a6XR[c6XR-((c6XR > d6X)?d6X+1:0)];
			double o6ZER = a6ZER[c6ZER-((c6ZER > d6ZE)?d6ZE+1:0)];
			double o6ZKR = a6ZKR[c6ZKR-((c6ZKR > d6ZK)?d6ZK+1:0)];
			
			//-------- one
			
			a6GL[c6GL] = ((o6AL*2.0) - (o6BL + o6CL + o6DL + o6EL + o6FL));
			a6HL[c6HL] = ((o6BL*2.0) - (o6AL + o6CL + o6DL + o6EL + o6FL));
			a6IL[c6IL] = ((o6CL*2.0) - (o6AL + o6BL + o6DL + o6EL + o6FL));
			a6JL[c6JL] = ((o6DL*2.0) - (o6AL + o6BL + o6CL + o6EL + o6FL));
			a6KL[c6KL] = ((o6EL*2.0) - (o6AL + o6BL + o6CL + o6DL + o6FL));
			a6LL[c6LL] = ((o6FL*2.0) - (o6AL + o6BL + o6CL + o6DL + o6EL));
			
			c6GL++; if (c6GL < 0 || c6GL > d6G) c6GL = 0;
			c6HL++; if (c6HL < 0 || c6HL > d6H) c6HL = 0;
			c6IL++; if (c6IL < 0 || c6IL > d6I) c6IL = 0;
			c6JL++; if (c6JL < 0 || c6JL > d6J) c6JL = 0;
			c6KL++; if (c6KL < 0 || c6KL > d6K) c6KL = 0;
			c6LL++; if (c6LL < 0 || c6LL > d6L) c6LL = 0;
			
			double o6GL = a6GL[c6GL-((c6GL > d6G)?d6G+1:0)];
			double o6HL = a6HL[c6HL-((c6HL > d6H)?d6H+1:0)];
			double o6IL = a6IL[c6IL-((c6IL > d6I)?d6I+1:0)];
			double o6JL = a6JL[c6JL-((c6JL > d6J)?d6J+1:0)];
			double o6KL = a6KL[c6KL-((c6KL > d6K)?d6K+1:0)];
			double o6LL = a6LL[c6LL-((c6LL > d6L)?d6L+1:0)];
			
			a6ER[c6ER] = ((o6FR*2.0) - (o6LR + o6RR + o6XR + o6ZER + o6ZKR));
			a6KR[c6KR] = ((o6LR*2.0) - (o6FR + o6RR + o6XR + o6ZER + o6ZKR));
			a6QR[c6QR] = ((o6RR*2.0) - (o6FR + o6LR + o6XR + o6ZER + o6ZKR));
			a6WR[c6WR] = ((o6XR*2.0) - (o6FR + o6LR + o6RR + o6ZER + o6ZKR));
			a6ZDR[c6ZDR] = ((o6ZER*2.0) - (o6FR + o6LR + o6RR + o6XR + o6ZKR));			
			a6ZJR[c6ZJR] = ((o6ZKR*2.0) - (o6FR + o6LR + o6RR + o6XR + o6ZER));			
			
			c6ER++; if (c6ER < 0 || c6ER > d6E) c6ER = 0;
			c6KR++; if (c6KR < 0 || c6KR > d6K) c6KR = 0;
			c6QR++; if (c6QR < 0 || c6QR > d6Q) c6QR = 0;
			c6WR++; if (c6WR < 0 || c6WR > d6W) c6WR = 0;
			c6ZDR++; if (c6ZDR < 0 || c6ZDR > d6ZD) c6ZDR = 0;
			c6ZJR++; if (c6ZJR < 0 || c6ZJR > d6ZJ) c6ZJR = 0;
			
			double o6ER = a6ER[c6ER-((c6ER > d6E)?d6E+1:0)];
			double o6KR = a6KR[c6KR-((c6KR > d6K)?d6K+1:0)];
			double o6QR = a6QR[c6QR-((c6QR > d6Q)?d6Q+1:0)];
			double o6WR = a6WR[c6WR-((c6WR > d6W)?d6W+1:0)];
			double o6ZDR = a6ZDR[c6ZDR-((c6ZDR > d6ZD)?d6ZD+1:0)];
			double o6ZJR = a6ZJR[c6ZJR-((c6ZJR > d6ZJ)?d6ZJ+1:0)];
			
			//-------- two
			
			a6ML[c6ML] = ((o6GL*2.0) - (o6HL + o6IL + o6JL + o6KL + o6LL));
			a6NL[c6NL] = ((o6HL*2.0) - (o6GL + o6IL + o6JL + o6KL + o6LL));
			a6OL[c6OL] = ((o6IL*2.0) - (o6GL + o6HL + o6JL + o6KL + o6LL));
			a6PL[c6PL] = ((o6JL*2.0) - (o6GL + o6HL + o6IL + o6KL + o6LL));
			a6QL[c6QL] = ((o6KL*2.0) - (o6GL + o6HL + o6IL + o6JL + o6LL));
			a6RL[c6RL] = ((o6LL*2.0) - (o6GL + o6HL + o6IL + o6JL + o6KL));
			
			c6ML++; if (c6ML < 0 || c6ML > d6M) c6ML = 0;
			c6NL++; if (c6NL < 0 || c6NL > d6N) c6NL = 0;
			c6OL++; if (c6OL < 0 || c6OL > d6O) c6OL = 0;
			c6PL++; if (c6PL < 0 || c6PL > d6P) c6PL = 0;
			c6QL++; if (c6QL < 0 || c6QL > d6Q) c6QL = 0;
			c6RL++; if (c6RL < 0 || c6RL > d6R) c6RL = 0;
			
			double o6ML = a6ML[c6ML-((c6ML > d6M)?d6M+1:0)];
			double o6NL = a6NL[c6NL-((c6NL > d6N)?d6N+1:0)];
			double o6OL = a6OL[c6OL-((c6OL > d6O)?d6O+1:0)];
			double o6PL = a6PL[c6PL-((c6PL > d6P)?d6P+1:0)];
			double o6QL = a6QL[c6QL-((c6QL > d6Q)?d6Q+1:0)];
			double o6RL = a6RL[c6RL-((c6RL > d6R)?d6R+1:0)];
			
			a6DR[c6DR] = ((o6ER*2.0) - (o6KR + o6QR + o6WR + o6ZDR + o6ZJR));
			a6JR[c6JR] = ((o6KR*2.0) - (o6ER + o6QR + o6WR + o6ZDR + o6ZJR));
			a6PR[c6PR] = ((o6QR*2.0) - (o6ER + o6KR + o6WR + o6ZDR + o6ZJR));
			a6VR[c6VR] = ((o6WR*2.0) - (o6ER + o6KR + o6QR + o6ZDR + o6ZJR));
			a6ZCR[c6ZCR] = ((o6ZDR*2.0) - (o6ER + o6KR + o6QR + o6WR + o6ZJR));
			a6ZIR[c6ZIR] = ((o6ZJR*2.0) - (o6ER + o6KR + o6QR + o6WR + o6ZDR));
			
			c6DR++; if (c6DR < 0 || c6DR > d6D) c6DR = 0;
			c6JR++; if (c6JR < 0 || c6JR > d6J) c6JR = 0;
			c6PR++; if (c6PR < 0 || c6PR > d6P) c6PR = 0;
			c6VR++; if (c6VR < 0 || c6VR > d6V) c6VR = 0;
			c6ZCR++; if (c6ZCR < 0 || c6ZCR > d6ZC) c6ZCR = 0;
			c6ZIR++; if (c6ZIR < 0 || c6ZIR > d6ZI) c6ZIR = 0;
			
			double o6DR = a6DR[c6DR-((c6DR > d6D)?d6D+1:0)];
			double o6JR = a6JR[c6JR-((c6JR > d6J)?d6J+1:0)];
			double o6PR = a6PR[c6PR-((c6PR > d6P)?d6P+1:0)];
			double o6VR = a6VR[c6VR-((c6VR > d6V)?d6V+1:0)];
			double o6ZCR = a6ZCR[c6ZCR-((c6ZCR > d6ZC)?d6ZC+1:0)];
			double o6ZIR = a6ZIR[c6ZIR-((c6ZIR > d6ZI)?d6ZI+1:0)];
			
			//-------- three
			
			a6SL[c6SL] = ((o6ML*2.0) - (o6NL + o6OL + o6PL + o6QL + o6RL));
			a6TL[c6TL] = ((o6NL*2.0) - (o6ML + o6OL + o6PL + o6QL + o6RL));
			a6UL[c6UL] = ((o6OL*2.0) - (o6ML + o6NL + o6PL + o6QL + o6RL));
			a6VL[c6VL] = ((o6PL*2.0) - (o6ML + o6NL + o6OL + o6QL + o6RL));
			a6WL[c6WL] = ((o6QL*2.0) - (o6ML + o6NL + o6OL + o6PL + o6RL));
			a6XL[c6XL] = ((o6RL*2.0) - (o6ML + o6NL + o6OL + o6PL + o6QL));
			
			c6SL++; if (c6SL < 0 || c6SL > d6S) c6SL = 0;
			c6TL++; if (c6TL < 0 || c6TL > d6T) c6TL = 0;
			c6UL++; if (c6UL < 0 || c6UL > d6U) c6UL = 0;
			c6VL++; if (c6VL < 0 || c6VL > d6V) c6VL = 0;
			c6WL++; if (c6WL < 0 || c6WL > d6W) c6WL = 0;
			c6XL++; if (c6XL < 0 || c6XL > d6X) c6XL = 0;
			
			double o6SL = a6SL[c6SL-((c6SL > d6S)?d6S+1:0)];
			double o6TL = a6TL[c6TL-((c6TL > d6T)?d6T+1:0)];
			double o6UL = a6UL[c6UL-((c6UL > d6U)?d6U+1:0)];
			double o6VL = a6VL[c6VL-((c6VL > d6V)?d6V+1:0)];
			double o6WL = a6WL[c6WL-((c6WL > d6W)?d6W+1:0)];
			double o6XL = a6XL[c6XL-((c6XL > d6X)?d6X+1:0)];
			
			a6CR[c6CR] = ((o6DR*2.0) - (o6JR + o6PR + o6VR + o6ZCR + o6ZIR));
			a6IR[c6IR] = ((o6JR*2.0) - (o6DR + o6PR + o6VR + o6ZCR + o6ZIR));
			a6OR[c6OR] = ((o6PR*2.0) - (o6DR + o6JR + o6VR + o6ZCR + o6ZIR));
			a6UR[c6UR] = ((o6VR*2.0) - (o6DR + o6JR + o6PR + o6ZCR + o6ZIR));
			a6ZBR[c6ZBR] = ((o6ZCR*2.0) - (o6DR + o6JR + o6PR + o6VR + o6ZIR));
			a6ZHR[c6ZHR] = ((o6ZIR*2.0) - (o6DR + o6JR + o6PR + o6VR + o6ZCR));
			
			c6CR++; if (c6CR < 0 || c6CR > d6C) c6CR = 0;
			c6IR++; if (c6IR < 0 || c6IR > d6I) c6IR = 0;
			c6OR++; if (c6OR < 0 || c6OR > d6O) c6OR = 0;
			c6UR++; if (c6UR < 0 || c6UR > d6U) c6UR = 0;
			c6ZBR++; if (c6ZBR < 0 || c6ZBR > d6ZB) c6ZBR = 0;
			c6ZHR++; if (c6ZHR < 0 || c6ZHR > d6ZH) c6ZHR = 0;
			
			double o6CR = a6CR[c6CR-((c6CR > d6C)?d6C+1:0)];
			double o6IR = a6IR[c6IR-((c6IR > d6I)?d6I+1:0)];
			double o6OR = a6OR[c6OR-((c6OR > d6O)?d6O+1:0)];
			double o6UR = a6UR[c6UR-((c6UR > d6U)?d6U+1:0)];
			double o6ZBR = a6ZBR[c6ZBR-((c6ZBR > d6ZB)?d6ZB+1:0)];
			double o6ZHR = a6ZHR[c6ZHR-((c6ZHR > d6ZH)?d6ZH+1:0)];
			
			//-------- four
			
			a6YL[c6YL] = ((o6SL*2.0) - (o6TL + o6UL + o6VL + o6WL + o6XL));
			a6ZAL[c6ZAL] = ((o6TL*2.0) - (o6SL + o6UL + o6VL + o6WL + o6XL));
			a6ZBL[c6ZBL] = ((o6UL*2.0) - (o6SL + o6TL + o6VL + o6WL + o6XL));
			a6ZCL[c6ZCL] = ((o6VL*2.0) - (o6SL + o6TL + o6UL + o6WL + o6XL));
			a6ZDL[c6ZDL] = ((o6WL*2.0) - (o6SL + o6TL + o6UL + o6VL + o6XL));
			a6ZEL[c6ZEL] = ((o6XL*2.0) - (o6SL + o6TL + o6UL + o6VL + o6WL));
			
			c6YL++; if (c6YL < 0 || c6YL > d6Y) c6YL = 0;
			c6ZAL++; if (c6ZAL < 0 || c6ZAL > d6ZA) c6ZAL = 0;
			c6ZBL++; if (c6ZBL < 0 || c6ZBL > d6ZB) c6ZBL = 0;
			c6ZCL++; if (c6ZCL < 0 || c6ZCL > d6ZC) c6ZCL = 0;
			c6ZDL++; if (c6ZDL < 0 || c6ZDL > d6ZD) c6ZDL = 0;
			c6ZEL++; if (c6ZEL < 0 || c6ZEL > d6ZE) c6ZEL = 0;
			
			double o6YL = a6YL[c6YL-((c6YL > d6Y)?d6Y+1:0)];
			double o6ZAL = a6ZAL[c6ZAL-((c6ZAL > d6ZA)?d6ZA+1:0)];
			double o6ZBL = a6ZBL[c6ZBL-((c6ZBL > d6ZB)?d6ZB+1:0)];
			double o6ZCL = a6ZCL[c6ZCL-((c6ZCL > d6ZC)?d6ZC+1:0)];
			double o6ZDL = a6ZDL[c6ZDL-((c6ZDL > d6ZD)?d6ZD+1:0)];
			double o6ZEL = a6ZEL[c6ZEL-((c6ZEL > d6ZE)?d6ZE+1:0)];
			
			a6BR[c6BR] = ((o6CR*2.0) - (o6IR + o6OR + o6UR + o6ZBR + o6ZHR));
			a6HR[c6HR] = ((o6IR*2.0) - (o6CR + o6OR + o6UR + o6ZBR + o6ZHR));
			a6NR[c6NR] = ((o6OR*2.0) - (o6CR + o6IR + o6UR + o6ZBR + o6ZHR));
			a6TR[c6TR] = ((o6UR*2.0) - (o6CR + o6IR + o6OR + o6ZBR + o6ZHR));
			a6ZAR[c6ZAR] = ((o6ZBR*2.0) - (o6CR + o6IR + o6OR + o6UR + o6ZHR));
			a6ZGR[c6ZGR] = ((o6ZHR*2.0) - (o6CR + o6IR + o6OR + o6UR + o6ZBR));
			
			c6BR++; if (c6BR < 0 || c6BR > d6B) c6BR = 0;
			c6HR++; if (c6HR < 0 || c6HR > d6H) c6HR = 0;
			c6NR++; if (c6NR < 0 || c6NR > d6N) c6NR = 0;
			c6TR++; if (c6TR < 0 || c6TR > d6T) c6TR = 0;
			c6ZBR++; if (c6ZBR < 0 || c6ZBR > d6ZB) c6ZBR = 0;
			c6ZGR++; if (c6ZGR < 0 || c6ZGR > d6ZG) c6ZGR = 0;
			
			double o6BR = a6BR[c6BR-((c6BR > d6B)?d6B+1:0)];
			double o6HR = a6HR[c6HR-((c6HR > d6H)?d6H+1:0)];
			double o6NR = a6NR[c6NR-((c6NR > d6N)?d6N+1:0)];
			double o6TR = a6TR[c6TR-((c6TR > d6T)?d6T+1:0)];
			double o6ZAR = a6ZAR[c6ZAR-((c6ZAR > d6ZA)?d6ZA+1:0)];
			double o6ZGR = a6ZGR[c6ZGR-((c6ZGR > d6ZG)?d6ZG+1:0)];
			
			//-------- five
			
			a6ZFL[c6ZFL] = ((o6YL*2.0) - (o6ZAL + o6ZBL + o6ZCL + o6ZDL + o6ZEL));
			a6ZGL[c6ZGL] = ((o6ZAL*2.0) - (o6YL + o6ZBL + o6ZCL + o6ZDL + o6ZEL));
			a6ZHL[c6ZHL] = ((o6ZBL*2.0) - (o6YL + o6ZAL + o6ZCL + o6ZDL + o6ZEL));
			a6ZIL[c6ZIL] = ((o6ZCL*2.0) - (o6YL + o6ZAL + o6ZBL + o6ZDL + o6ZEL));
			a6ZJL[c6ZJL] = ((o6ZDL*2.0) - (o6YL + o6ZAL + o6ZBL + o6ZCL + o6ZEL));
			a6ZKL[c6ZKL] = ((o6ZEL*2.0) - (o6YL + o6ZAL + o6ZBL + o6ZCL + o6ZDL));
			
			c6ZFL++; if (c6ZFL < 0 || c6ZFL > d6ZF) c6ZFL = 0;
			c6ZGL++; if (c6ZGL < 0 || c6ZGL > d6ZG) c6ZGL = 0;
			c6ZHL++; if (c6ZHL < 0 || c6ZHL > d6ZH) c6ZHL = 0;
			c6ZIL++; if (c6ZIL < 0 || c6ZIL > d6ZI) c6ZIL = 0;
			c6ZJL++; if (c6ZJL < 0 || c6ZJL > d6ZJ) c6ZJL = 0;
			c6ZKL++; if (c6ZKL < 0 || c6ZKL > d6ZK) c6ZKL = 0;
			
			double o6ZFL = a6ZFL[c6ZFL-((c6ZFL > d6ZF)?d6ZF+1:0)];
			double o6ZGL = a6ZGL[c6ZGL-((c6ZGL > d6ZG)?d6ZG+1:0)];
			double o6ZHL = a6ZHL[c6ZHL-((c6ZHL > d6ZH)?d6ZH+1:0)];
			double o6ZIL = a6ZIL[c6ZIL-((c6ZIL > d6ZI)?d6ZI+1:0)];
			double o6ZJL = a6ZJL[c6ZJL-((c6ZJL > d6ZJ)?d6ZJ+1:0)];
			double o6ZKL = a6ZKL[c6ZKL-((c6ZKL > d6ZK)?d6ZK+1:0)];
			
			a6AR[c6AR] = ((o6BR*2.0) - (o6HR + o6NR + o6TR + o6ZAR + o6ZGR));
			a6GR[c6GR] = ((o6HR*2.0) - (o6BR + o6NR + o6TR + o6ZAR + o6ZGR));
			a6MR[c6MR] = ((o6NR*2.0) - (o6BR + o6HR + o6TR + o6ZAR + o6ZGR));
			a6SR[c6SR] = ((o6TR*2.0) - (o6BR + o6HR + o6NR + o6ZAR + o6ZGR));
			a6YR[c6YR] = ((o6ZAR*2.0) - (o6BR + o6HR + o6NR + o6TR + o6ZGR));
			a6ZFR[c6ZFR] = ((o6ZGR*2.0) - (o6BR + o6HR + o6NR + o6TR + o6ZAR));
			
			c6AR++; if (c6AR < 0 || c6AR > d6A) c6AR = 0;
			c6GR++; if (c6GR < 0 || c6GR > d6G) c6GR = 0;
			c6MR++; if (c6MR < 0 || c6MR > d6M) c6MR = 0;
			c6SR++; if (c6SR < 0 || c6SR > d6S) c6SR = 0;
			c6YR++; if (c6YR < 0 || c6YR > d6Y) c6YR = 0;
			c6ZFR++; if (c6ZFR < 0 || c6ZFR > d6ZF) c6ZFR = 0;
			
			double o6AR = a6AR[c6AR-((c6AR > d6A)?d6A+1:0)];
			double o6GR = a6GR[c6GR-((c6GR > d6G)?d6G+1:0)];
			double o6MR = a6MR[c6MR-((c6MR > d6M)?d6M+1:0)];
			double o6SR = a6SR[c6SR-((c6SR > d6S)?d6S+1:0)];
			double o6YR = a6YR[c6YR-((c6YR > d6Y)?d6Y+1:0)];
			double o6ZFR = a6ZFR[c6ZFR-((c6ZFR > d6ZF)?d6ZF+1:0)];
			
			//-------- six
			
			f6AL = ((o6AR*2.0) - (o6GR + o6MR + o6SR + o6YR + o6ZFR));
			f6BL = ((o6GR*2.0) - (o6AR + o6MR + o6SR + o6YR + o6ZFR));
			f6CL = ((o6MR*2.0) - (o6AR + o6GR + o6SR + o6YR + o6ZFR));
			f6DL = ((o6SR*2.0) - (o6AR + o6GR + o6MR + o6YR + o6ZFR));
			f6EL = ((o6YR*2.0) - (o6AR + o6GR + o6MR + o6SR + o6ZFR));
			f6FL = ((o6ZFR*2.0) - (o6AR + o6GR + o6MR + o6SR + o6YR));
			
			f6FR = ((o6ZFL*2.0) - (o6ZGL + o6ZHL + o6ZIL + o6ZJL + o6ZKL));
			f6LR = ((o6ZGL*2.0) - (o6ZFL + o6ZHL + o6ZIL + o6ZJL + o6ZKL));
			f6RR = ((o6ZHL*2.0) - (o6ZFL + o6ZGL + o6ZIL + o6ZJL + o6ZKL));
			f6XR = ((o6ZIL*2.0) - (o6ZFL + o6ZGL + o6ZHL + o6ZJL + o6ZKL));
			f6ZER = ((o6ZJL*2.0) - (o6ZFL + o6ZGL + o6ZHL + o6ZIL + o6ZKL));
			f6ZKR = ((o6ZKL*2.0) - (o6ZFL + o6ZGL + o6ZHL + o6ZIL + o6ZJL));
			
			inputSampleL = (o6ZFL + o6ZGL + o6ZHL + o6ZIL + o6ZJL + o6ZKL)*0.001953125;
			inputSampleR = (o6AR + o6GR + o6MR + o6SR + o6YR + o6ZFR)*0.001953125;
			
			f6AL = (f6AL+f6AL+f6AL+fabs(avg6L))*0.25; avg6L = f6AL;
			f6FR = (f6FR+f6FR+f6FR+fabs(avg6R))*0.25; avg6R = f6FR;
			//manipulating deep reverb tail for realism
			
			inputSampleL += (earlyReflectionL * earlyLoudness);
			inputSampleR += (earlyReflectionR * earlyLoudness);
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		double CBL = (bez[bez_CL]*(1.0-bez[bez_cycle]))+(bez[bez_BL]*bez[bez_cycle]);
		double CBR = (bez[bez_CR]*(1.0-bez[bez_cycle]))+(bez[bez_BR]*bez[bez_cycle]);
		double BAL = (bez[bez_BL]*(1.0-bez[bez_cycle]))+(bez[bez_AL]*bez[bez_cycle]);
		double BAR = (bez[bez_BR]*(1.0-bez[bez_cycle]))+(bez[bez_AR]*bez[bez_cycle]);
		double CBAL = (bez[bez_BL]+(CBL*(1.0-bez[bez_cycle]))+(BAL*bez[bez_cycle]))*-0.125;
		double CBAR = (bez[bez_BR]+(CBR*(1.0-bez[bez_cycle]))+(BAR*bez[bez_cycle]))*-0.125;
		inputSampleL = CBAL;
		inputSampleR = CBAR;
		
		inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0-wet));
		inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0-wet));
		
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

void kAlienSpaceship::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double fdb6ck = (0.0009765625+0.0009765625+0.001953125)*0.3333333;
	double reg6n = (1.0-pow(1.0-A,3.0))*fdb6ck;
	//start this but pad it in the loop by volume of output?
	double derez = B/overallscale;
	derez = 1.0 / ((int)(1.0/derez));
	if (derez < 0.0005) derez = 0.0005; if (derez > 1.0) derez = 1.0;
	double derezFreq = pow(C,3.0)+0.01;
	if (derezFreq > 1.0) derezFreq = 1.0;
	double earlyLoudness = D;
	int start = (int)(E * 27.0);
	int ld3G = early[start]; 
	int ld3H = early[start+1]; 
	int ld3D = early[start+2]; 
	int ld3A = early[start+3]; 
	int ld3E = early[start+4]; 
	int ld3I = early[start+5];
	int ld3F = early[start+6];
	int ld3B = early[start+7]; 
	int ld3C = early[start+8];
	double wet = F;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += ((inputSampleL+bez[bez_InL]) * derez);
		bez[bez_SampR] += ((inputSampleR+bez[bez_InR]) * derez);
		bez[bez_InL] = inputSampleL; bez[bez_InR] = inputSampleR;
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
						
			inputSampleL = bez[bez_SampL];
			inputSampleR = bez[bez_SampR];
			
			a3AL[c3AL] = inputSampleL;// + (f3AL * reg3n);
			a3BL[c3BL] = inputSampleL;// + (f3BL * reg3n);
			a3CL[c3CL] = inputSampleL;// + (f3CL * reg3n);
			
			a3CR[c3CR] = inputSampleR;// + (f3CR * reg3n);
			a3FR[c3FR] = inputSampleR;// + (f3FR * reg3n);
			a3IR[c3IR] = inputSampleR;// + (f3IR * reg3n);
			
			c3AL++; if (c3AL < 0 || c3AL > ld3A) c3AL = 0;
			c3BL++; if (c3BL < 0 || c3BL > ld3B) c3BL = 0;
			c3CL++; if (c3CL < 0 || c3CL > ld3C) c3CL = 0;
			c3CR++; if (c3CR < 0 || c3CR > ld3C) c3CR = 0;
			c3FR++; if (c3FR < 0 || c3FR > ld3F) c3FR = 0;
			c3IR++; if (c3IR < 0 || c3IR > ld3I) c3IR = 0;
			
			double o3AL = a3AL[c3AL-((c3AL > ld3A)?c3AL+1:0)];
			double o3BL = a3BL[c3BL-((c3BL > ld3B)?c3BL+1:0)];
			double o3CL = a3CL[c3CL-((c3CL > ld3C)?c3CL+1:0)];
			double o3CR = a3CR[c3CR-((c3CR > ld3C)?c3CR+1:0)];
			double o3FR = a3FR[c3FR-((c3FR > ld3F)?c3FR+1:0)];
			double o3IR = a3IR[c3IR-((c3IR > ld3I)?c3IR+1:0)];
			
			a3DL[c3DL] = (((o3BL + o3CL) * -2.0) + o3AL);
			a3EL[c3EL] = (((o3AL + o3CL) * -2.0) + o3BL);
			a3FL[c3FL] = (((o3AL + o3BL) * -2.0) + o3CL);
			a3BR[c3BR] = (((o3FR + o3IR) * -2.0) + o3CR);
			a3ER[c3ER] = (((o3CR + o3IR) * -2.0) + o3FR);
			a3HR[c3HR] = (((o3CR + o3FR) * -2.0) + o3IR);
			
			c3DL++; if (c3DL < 0 || c3DL > ld3D) c3DL = 0;
			c3EL++; if (c3EL < 0 || c3EL > ld3E) c3EL = 0;
			c3FL++; if (c3FL < 0 || c3FL > ld3F) c3FL = 0;
			c3BR++; if (c3BR < 0 || c3BR > ld3B) c3BR = 0;
			c3ER++; if (c3ER < 0 || c3ER > ld3E) c3ER = 0;
			c3HR++; if (c3HR < 0 || c3HR > ld3H) c3HR = 0;
			
			double o3DL = a3DL[c3DL-((c3DL > ld3D)?c3DL+1:0)];
			double o3EL = a3EL[c3EL-((c3EL > ld3E)?c3EL+1:0)];
			double o3FL = a3FL[c3FL-((c3FL > ld3F)?c3FL+1:0)];
			double o3BR = a3BR[c3BR-((c3BR > ld3B)?c3BR+1:0)];
			double o3ER = a3ER[c3ER-((c3ER > ld3E)?c3ER+1:0)];
			double o3HR = a3HR[c3HR-((c3HR > ld3H)?c3HR+1:0)];
			
			a3GL[c3GL] = (((o3EL + o3FL) * -2.0) + o3DL);
			a3HL[c3HL] = (((o3DL + o3FL) * -2.0) + o3EL);
			a3IL[c3IL] = (((o3DL + o3EL) * -2.0) + o3FL);
			a3AR[c3AR] = (((o3ER + o3HR) * -2.0) + o3BR);
			a3DR[c3DR] = (((o3BR + o3HR) * -2.0) + o3ER);
			a3GR[c3GR] = (((o3BR + o3ER) * -2.0) + o3HR);
			
			c3GL++; if (c3GL < 0 || c3GL > ld3G) c3GL = 0;
			c3HL++; if (c3HL < 0 || c3HL > ld3H) c3HL = 0;
			c3IL++; if (c3IL < 0 || c3IL > ld3I) c3IL = 0;
			c3AR++; if (c3AR < 0 || c3AR > ld3A) c3AR = 0;
			c3DR++; if (c3DR < 0 || c3DR > ld3D) c3DR = 0;
			c3GR++; if (c3GR < 0 || c3GR > ld3G) c3GR = 0;
			
			double o3GL = a3GL[c3GL-((c3GL > ld3G)?c3GL+1:0)];
			double o3HL = a3HL[c3HL-((c3HL > ld3H)?c3HL+1:0)];
			double o3IL = a3IL[c3IL-((c3IL > ld3I)?c3IL+1:0)];
			double o3AR = a3AR[c3AR-((c3AR > ld3A)?c3AR+1:0)];
			double o3DR = a3DR[c3DR-((c3DR > ld3D)?c3DR+1:0)];
			double o3GR = a3GR[c3GR-((c3GR > ld3G)?c3GR+1:0)];
			
			double inputSampleL = (o3GL + o3HL + o3IL)*0.03125;
			double inputSampleR = (o3AR + o3DR + o3GR)*0.03125;
			
			bezF[bez_cycle] += derezFreq;
			bezF[bez_SampL] += ((inputSampleL+bezF[bez_InL]) * derezFreq);
			bezF[bez_SampR] += ((inputSampleL+bezF[bez_InR]) * derezFreq);
			bezF[bez_InL] = inputSampleL; bezF[bez_InR] = inputSampleR;
			if (bezF[bez_cycle] > 1.0) { //hit the end point and we do a filter sample
				bezF[bez_cycle] -= 1.0;
				bezF[bez_CL] = bezF[bez_BL];
				bezF[bez_BL] = bezF[bez_AL];
				bezF[bez_AL] = inputSampleL;
				bezF[bez_SampL] = 0.0;
				bezF[bez_CR] = bezF[bez_BR];
				bezF[bez_BR] = bezF[bez_AR];
				bezF[bez_AR] = inputSampleR;
				bezF[bez_SampR] = 0.0;
			}
			double CBLfreq = (bezF[bez_CL]*(1.0-bezF[bez_cycle]))+(bezF[bez_BL]*bezF[bez_cycle]);
			double BALfreq = (bezF[bez_BL]*(1.0-bezF[bez_cycle]))+(bezF[bez_AL]*bezF[bez_cycle]);
			inputSampleL = (bezF[bez_BL]+(CBLfreq*(1.0-bezF[bez_cycle]))+(BALfreq*bezF[bez_cycle]))*0.5;
			double CBRfreq = (bezF[bez_CR]*(1.0-bezF[bez_cycle]))+(bezF[bez_BR]*bezF[bez_cycle]);
			double BARfreq = (bezF[bez_BR]*(1.0-bezF[bez_cycle]))+(bezF[bez_AR]*bezF[bez_cycle]);
			inputSampleR = (bezF[bez_BR]+(CBRfreq*(1.0-bezF[bez_cycle]))+(BARfreq*bezF[bez_cycle]))*0.5;
			
			double earlyReflectionL = inputSampleL;
			double earlyReflectionR = inputSampleR; //for more alienness, early reflections are DeRezzed
			
			a6AL[c6AL] = inputSampleL + (f6BL * reg6n);
			a6BL[c6BL] = inputSampleL + (f6CL * reg6n);
			a6CL[c6CL] = inputSampleL + (f6DL * reg6n);
			a6DL[c6DL] = inputSampleL + (f6EL * reg6n);
			a6EL[c6EL] = inputSampleL + (f6FL * reg6n);
			a6FL[c6FL] = inputSampleL + (f6AL * reg6n);
			
			c6AL++; if (c6AL < 0 || c6AL > d6A) c6AL = 0;
			c6BL++; if (c6BL < 0 || c6BL > d6B) c6BL = 0;
			c6CL++; if (c6CL < 0 || c6CL > d6C) c6CL = 0;
			c6DL++; if (c6DL < 0 || c6DL > d6D) c6DL = 0;
			c6EL++; if (c6EL < 0 || c6EL > d6E) c6EL = 0;
			c6FL++; if (c6FL < 0 || c6FL > d6F) c6FL = 0;
			
			double o6AL = a6AL[c6AL-((c6AL > d6A)?d6A+1:0)];
			double o6BL = a6BL[c6BL-((c6BL > d6B)?d6B+1:0)];
			double o6CL = a6CL[c6CL-((c6CL > d6C)?d6C+1:0)];
			double o6DL = a6DL[c6DL-((c6DL > d6D)?d6D+1:0)];
			double o6EL = a6EL[c6EL-((c6EL > d6E)?d6E+1:0)];
			double o6FL = a6FL[c6FL-((c6FL > d6F)?d6F+1:0)];
			
			a6FR[c6FR] = inputSampleR + (f6LR * reg6n);
			a6LR[c6LR] = inputSampleR + (f6RR * reg6n);
			a6RR[c6RR] = inputSampleR + (f6XR * reg6n);
			a6XR[c6XR] = inputSampleR + (f6ZER * reg6n);
			a6ZER[c6ZER] = inputSampleR + (f6ZKR * reg6n);
			a6ZKR[c6ZKR] = inputSampleR + (f6FR * reg6n);
			
			c6FR++; if (c6FR < 0 || c6FR > d6F) c6FR = 0;
			c6LR++; if (c6LR < 0 || c6LR > d6L) c6LR = 0;
			c6RR++; if (c6RR < 0 || c6RR > d6R) c6RR = 0;
			c6XR++; if (c6XR < 0 || c6XR > d6X) c6XR = 0;
			c6ZER++; if (c6ZER < 0 || c6ZER > d6ZE) c6ZER = 0;
			c6ZKR++; if (c6ZKR < 0 || c6ZKR > d6ZK) c6ZKR = 0;
			
			double o6FR = a6FR[c6FR-((c6FR > d6F)?d6F+1:0)];
			double o6LR = a6LR[c6LR-((c6LR > d6L)?d6L+1:0)];
			double o6RR = a6RR[c6RR-((c6RR > d6R)?d6R+1:0)];
			double o6XR = a6XR[c6XR-((c6XR > d6X)?d6X+1:0)];
			double o6ZER = a6ZER[c6ZER-((c6ZER > d6ZE)?d6ZE+1:0)];
			double o6ZKR = a6ZKR[c6ZKR-((c6ZKR > d6ZK)?d6ZK+1:0)];
			
			//-------- one
			
			a6GL[c6GL] = ((o6AL*2.0) - (o6BL + o6CL + o6DL + o6EL + o6FL));
			a6HL[c6HL] = ((o6BL*2.0) - (o6AL + o6CL + o6DL + o6EL + o6FL));
			a6IL[c6IL] = ((o6CL*2.0) - (o6AL + o6BL + o6DL + o6EL + o6FL));
			a6JL[c6JL] = ((o6DL*2.0) - (o6AL + o6BL + o6CL + o6EL + o6FL));
			a6KL[c6KL] = ((o6EL*2.0) - (o6AL + o6BL + o6CL + o6DL + o6FL));
			a6LL[c6LL] = ((o6FL*2.0) - (o6AL + o6BL + o6CL + o6DL + o6EL));
			
			c6GL++; if (c6GL < 0 || c6GL > d6G) c6GL = 0;
			c6HL++; if (c6HL < 0 || c6HL > d6H) c6HL = 0;
			c6IL++; if (c6IL < 0 || c6IL > d6I) c6IL = 0;
			c6JL++; if (c6JL < 0 || c6JL > d6J) c6JL = 0;
			c6KL++; if (c6KL < 0 || c6KL > d6K) c6KL = 0;
			c6LL++; if (c6LL < 0 || c6LL > d6L) c6LL = 0;
			
			double o6GL = a6GL[c6GL-((c6GL > d6G)?d6G+1:0)];
			double o6HL = a6HL[c6HL-((c6HL > d6H)?d6H+1:0)];
			double o6IL = a6IL[c6IL-((c6IL > d6I)?d6I+1:0)];
			double o6JL = a6JL[c6JL-((c6JL > d6J)?d6J+1:0)];
			double o6KL = a6KL[c6KL-((c6KL > d6K)?d6K+1:0)];
			double o6LL = a6LL[c6LL-((c6LL > d6L)?d6L+1:0)];
			
			a6ER[c6ER] = ((o6FR*2.0) - (o6LR + o6RR + o6XR + o6ZER + o6ZKR));
			a6KR[c6KR] = ((o6LR*2.0) - (o6FR + o6RR + o6XR + o6ZER + o6ZKR));
			a6QR[c6QR] = ((o6RR*2.0) - (o6FR + o6LR + o6XR + o6ZER + o6ZKR));
			a6WR[c6WR] = ((o6XR*2.0) - (o6FR + o6LR + o6RR + o6ZER + o6ZKR));
			a6ZDR[c6ZDR] = ((o6ZER*2.0) - (o6FR + o6LR + o6RR + o6XR + o6ZKR));			
			a6ZJR[c6ZJR] = ((o6ZKR*2.0) - (o6FR + o6LR + o6RR + o6XR + o6ZER));			
			
			c6ER++; if (c6ER < 0 || c6ER > d6E) c6ER = 0;
			c6KR++; if (c6KR < 0 || c6KR > d6K) c6KR = 0;
			c6QR++; if (c6QR < 0 || c6QR > d6Q) c6QR = 0;
			c6WR++; if (c6WR < 0 || c6WR > d6W) c6WR = 0;
			c6ZDR++; if (c6ZDR < 0 || c6ZDR > d6ZD) c6ZDR = 0;
			c6ZJR++; if (c6ZJR < 0 || c6ZJR > d6ZJ) c6ZJR = 0;
			
			double o6ER = a6ER[c6ER-((c6ER > d6E)?d6E+1:0)];
			double o6KR = a6KR[c6KR-((c6KR > d6K)?d6K+1:0)];
			double o6QR = a6QR[c6QR-((c6QR > d6Q)?d6Q+1:0)];
			double o6WR = a6WR[c6WR-((c6WR > d6W)?d6W+1:0)];
			double o6ZDR = a6ZDR[c6ZDR-((c6ZDR > d6ZD)?d6ZD+1:0)];
			double o6ZJR = a6ZJR[c6ZJR-((c6ZJR > d6ZJ)?d6ZJ+1:0)];
			
			//-------- two
			
			a6ML[c6ML] = ((o6GL*2.0) - (o6HL + o6IL + o6JL + o6KL + o6LL));
			a6NL[c6NL] = ((o6HL*2.0) - (o6GL + o6IL + o6JL + o6KL + o6LL));
			a6OL[c6OL] = ((o6IL*2.0) - (o6GL + o6HL + o6JL + o6KL + o6LL));
			a6PL[c6PL] = ((o6JL*2.0) - (o6GL + o6HL + o6IL + o6KL + o6LL));
			a6QL[c6QL] = ((o6KL*2.0) - (o6GL + o6HL + o6IL + o6JL + o6LL));
			a6RL[c6RL] = ((o6LL*2.0) - (o6GL + o6HL + o6IL + o6JL + o6KL));
			
			c6ML++; if (c6ML < 0 || c6ML > d6M) c6ML = 0;
			c6NL++; if (c6NL < 0 || c6NL > d6N) c6NL = 0;
			c6OL++; if (c6OL < 0 || c6OL > d6O) c6OL = 0;
			c6PL++; if (c6PL < 0 || c6PL > d6P) c6PL = 0;
			c6QL++; if (c6QL < 0 || c6QL > d6Q) c6QL = 0;
			c6RL++; if (c6RL < 0 || c6RL > d6R) c6RL = 0;
			
			double o6ML = a6ML[c6ML-((c6ML > d6M)?d6M+1:0)];
			double o6NL = a6NL[c6NL-((c6NL > d6N)?d6N+1:0)];
			double o6OL = a6OL[c6OL-((c6OL > d6O)?d6O+1:0)];
			double o6PL = a6PL[c6PL-((c6PL > d6P)?d6P+1:0)];
			double o6QL = a6QL[c6QL-((c6QL > d6Q)?d6Q+1:0)];
			double o6RL = a6RL[c6RL-((c6RL > d6R)?d6R+1:0)];
			
			a6DR[c6DR] = ((o6ER*2.0) - (o6KR + o6QR + o6WR + o6ZDR + o6ZJR));
			a6JR[c6JR] = ((o6KR*2.0) - (o6ER + o6QR + o6WR + o6ZDR + o6ZJR));
			a6PR[c6PR] = ((o6QR*2.0) - (o6ER + o6KR + o6WR + o6ZDR + o6ZJR));
			a6VR[c6VR] = ((o6WR*2.0) - (o6ER + o6KR + o6QR + o6ZDR + o6ZJR));
			a6ZCR[c6ZCR] = ((o6ZDR*2.0) - (o6ER + o6KR + o6QR + o6WR + o6ZJR));
			a6ZIR[c6ZIR] = ((o6ZJR*2.0) - (o6ER + o6KR + o6QR + o6WR + o6ZDR));
			
			c6DR++; if (c6DR < 0 || c6DR > d6D) c6DR = 0;
			c6JR++; if (c6JR < 0 || c6JR > d6J) c6JR = 0;
			c6PR++; if (c6PR < 0 || c6PR > d6P) c6PR = 0;
			c6VR++; if (c6VR < 0 || c6VR > d6V) c6VR = 0;
			c6ZCR++; if (c6ZCR < 0 || c6ZCR > d6ZC) c6ZCR = 0;
			c6ZIR++; if (c6ZIR < 0 || c6ZIR > d6ZI) c6ZIR = 0;
			
			double o6DR = a6DR[c6DR-((c6DR > d6D)?d6D+1:0)];
			double o6JR = a6JR[c6JR-((c6JR > d6J)?d6J+1:0)];
			double o6PR = a6PR[c6PR-((c6PR > d6P)?d6P+1:0)];
			double o6VR = a6VR[c6VR-((c6VR > d6V)?d6V+1:0)];
			double o6ZCR = a6ZCR[c6ZCR-((c6ZCR > d6ZC)?d6ZC+1:0)];
			double o6ZIR = a6ZIR[c6ZIR-((c6ZIR > d6ZI)?d6ZI+1:0)];
			
			//-------- three
			
			a6SL[c6SL] = ((o6ML*2.0) - (o6NL + o6OL + o6PL + o6QL + o6RL));
			a6TL[c6TL] = ((o6NL*2.0) - (o6ML + o6OL + o6PL + o6QL + o6RL));
			a6UL[c6UL] = ((o6OL*2.0) - (o6ML + o6NL + o6PL + o6QL + o6RL));
			a6VL[c6VL] = ((o6PL*2.0) - (o6ML + o6NL + o6OL + o6QL + o6RL));
			a6WL[c6WL] = ((o6QL*2.0) - (o6ML + o6NL + o6OL + o6PL + o6RL));
			a6XL[c6XL] = ((o6RL*2.0) - (o6ML + o6NL + o6OL + o6PL + o6QL));
			
			c6SL++; if (c6SL < 0 || c6SL > d6S) c6SL = 0;
			c6TL++; if (c6TL < 0 || c6TL > d6T) c6TL = 0;
			c6UL++; if (c6UL < 0 || c6UL > d6U) c6UL = 0;
			c6VL++; if (c6VL < 0 || c6VL > d6V) c6VL = 0;
			c6WL++; if (c6WL < 0 || c6WL > d6W) c6WL = 0;
			c6XL++; if (c6XL < 0 || c6XL > d6X) c6XL = 0;
			
			double o6SL = a6SL[c6SL-((c6SL > d6S)?d6S+1:0)];
			double o6TL = a6TL[c6TL-((c6TL > d6T)?d6T+1:0)];
			double o6UL = a6UL[c6UL-((c6UL > d6U)?d6U+1:0)];
			double o6VL = a6VL[c6VL-((c6VL > d6V)?d6V+1:0)];
			double o6WL = a6WL[c6WL-((c6WL > d6W)?d6W+1:0)];
			double o6XL = a6XL[c6XL-((c6XL > d6X)?d6X+1:0)];
			
			a6CR[c6CR] = ((o6DR*2.0) - (o6JR + o6PR + o6VR + o6ZCR + o6ZIR));
			a6IR[c6IR] = ((o6JR*2.0) - (o6DR + o6PR + o6VR + o6ZCR + o6ZIR));
			a6OR[c6OR] = ((o6PR*2.0) - (o6DR + o6JR + o6VR + o6ZCR + o6ZIR));
			a6UR[c6UR] = ((o6VR*2.0) - (o6DR + o6JR + o6PR + o6ZCR + o6ZIR));
			a6ZBR[c6ZBR] = ((o6ZCR*2.0) - (o6DR + o6JR + o6PR + o6VR + o6ZIR));
			a6ZHR[c6ZHR] = ((o6ZIR*2.0) - (o6DR + o6JR + o6PR + o6VR + o6ZCR));
			
			c6CR++; if (c6CR < 0 || c6CR > d6C) c6CR = 0;
			c6IR++; if (c6IR < 0 || c6IR > d6I) c6IR = 0;
			c6OR++; if (c6OR < 0 || c6OR > d6O) c6OR = 0;
			c6UR++; if (c6UR < 0 || c6UR > d6U) c6UR = 0;
			c6ZBR++; if (c6ZBR < 0 || c6ZBR > d6ZB) c6ZBR = 0;
			c6ZHR++; if (c6ZHR < 0 || c6ZHR > d6ZH) c6ZHR = 0;
			
			double o6CR = a6CR[c6CR-((c6CR > d6C)?d6C+1:0)];
			double o6IR = a6IR[c6IR-((c6IR > d6I)?d6I+1:0)];
			double o6OR = a6OR[c6OR-((c6OR > d6O)?d6O+1:0)];
			double o6UR = a6UR[c6UR-((c6UR > d6U)?d6U+1:0)];
			double o6ZBR = a6ZBR[c6ZBR-((c6ZBR > d6ZB)?d6ZB+1:0)];
			double o6ZHR = a6ZHR[c6ZHR-((c6ZHR > d6ZH)?d6ZH+1:0)];
			
			//-------- four
			
			a6YL[c6YL] = ((o6SL*2.0) - (o6TL + o6UL + o6VL + o6WL + o6XL));
			a6ZAL[c6ZAL] = ((o6TL*2.0) - (o6SL + o6UL + o6VL + o6WL + o6XL));
			a6ZBL[c6ZBL] = ((o6UL*2.0) - (o6SL + o6TL + o6VL + o6WL + o6XL));
			a6ZCL[c6ZCL] = ((o6VL*2.0) - (o6SL + o6TL + o6UL + o6WL + o6XL));
			a6ZDL[c6ZDL] = ((o6WL*2.0) - (o6SL + o6TL + o6UL + o6VL + o6XL));
			a6ZEL[c6ZEL] = ((o6XL*2.0) - (o6SL + o6TL + o6UL + o6VL + o6WL));
			
			c6YL++; if (c6YL < 0 || c6YL > d6Y) c6YL = 0;
			c6ZAL++; if (c6ZAL < 0 || c6ZAL > d6ZA) c6ZAL = 0;
			c6ZBL++; if (c6ZBL < 0 || c6ZBL > d6ZB) c6ZBL = 0;
			c6ZCL++; if (c6ZCL < 0 || c6ZCL > d6ZC) c6ZCL = 0;
			c6ZDL++; if (c6ZDL < 0 || c6ZDL > d6ZD) c6ZDL = 0;
			c6ZEL++; if (c6ZEL < 0 || c6ZEL > d6ZE) c6ZEL = 0;
			
			double o6YL = a6YL[c6YL-((c6YL > d6Y)?d6Y+1:0)];
			double o6ZAL = a6ZAL[c6ZAL-((c6ZAL > d6ZA)?d6ZA+1:0)];
			double o6ZBL = a6ZBL[c6ZBL-((c6ZBL > d6ZB)?d6ZB+1:0)];
			double o6ZCL = a6ZCL[c6ZCL-((c6ZCL > d6ZC)?d6ZC+1:0)];
			double o6ZDL = a6ZDL[c6ZDL-((c6ZDL > d6ZD)?d6ZD+1:0)];
			double o6ZEL = a6ZEL[c6ZEL-((c6ZEL > d6ZE)?d6ZE+1:0)];
			
			a6BR[c6BR] = ((o6CR*2.0) - (o6IR + o6OR + o6UR + o6ZBR + o6ZHR));
			a6HR[c6HR] = ((o6IR*2.0) - (o6CR + o6OR + o6UR + o6ZBR + o6ZHR));
			a6NR[c6NR] = ((o6OR*2.0) - (o6CR + o6IR + o6UR + o6ZBR + o6ZHR));
			a6TR[c6TR] = ((o6UR*2.0) - (o6CR + o6IR + o6OR + o6ZBR + o6ZHR));
			a6ZAR[c6ZAR] = ((o6ZBR*2.0) - (o6CR + o6IR + o6OR + o6UR + o6ZHR));
			a6ZGR[c6ZGR] = ((o6ZHR*2.0) - (o6CR + o6IR + o6OR + o6UR + o6ZBR));
			
			c6BR++; if (c6BR < 0 || c6BR > d6B) c6BR = 0;
			c6HR++; if (c6HR < 0 || c6HR > d6H) c6HR = 0;
			c6NR++; if (c6NR < 0 || c6NR > d6N) c6NR = 0;
			c6TR++; if (c6TR < 0 || c6TR > d6T) c6TR = 0;
			c6ZBR++; if (c6ZBR < 0 || c6ZBR > d6ZB) c6ZBR = 0;
			c6ZGR++; if (c6ZGR < 0 || c6ZGR > d6ZG) c6ZGR = 0;
			
			double o6BR = a6BR[c6BR-((c6BR > d6B)?d6B+1:0)];
			double o6HR = a6HR[c6HR-((c6HR > d6H)?d6H+1:0)];
			double o6NR = a6NR[c6NR-((c6NR > d6N)?d6N+1:0)];
			double o6TR = a6TR[c6TR-((c6TR > d6T)?d6T+1:0)];
			double o6ZAR = a6ZAR[c6ZAR-((c6ZAR > d6ZA)?d6ZA+1:0)];
			double o6ZGR = a6ZGR[c6ZGR-((c6ZGR > d6ZG)?d6ZG+1:0)];
			
			//-------- five
			
			a6ZFL[c6ZFL] = ((o6YL*2.0) - (o6ZAL + o6ZBL + o6ZCL + o6ZDL + o6ZEL));
			a6ZGL[c6ZGL] = ((o6ZAL*2.0) - (o6YL + o6ZBL + o6ZCL + o6ZDL + o6ZEL));
			a6ZHL[c6ZHL] = ((o6ZBL*2.0) - (o6YL + o6ZAL + o6ZCL + o6ZDL + o6ZEL));
			a6ZIL[c6ZIL] = ((o6ZCL*2.0) - (o6YL + o6ZAL + o6ZBL + o6ZDL + o6ZEL));
			a6ZJL[c6ZJL] = ((o6ZDL*2.0) - (o6YL + o6ZAL + o6ZBL + o6ZCL + o6ZEL));
			a6ZKL[c6ZKL] = ((o6ZEL*2.0) - (o6YL + o6ZAL + o6ZBL + o6ZCL + o6ZDL));
			
			c6ZFL++; if (c6ZFL < 0 || c6ZFL > d6ZF) c6ZFL = 0;
			c6ZGL++; if (c6ZGL < 0 || c6ZGL > d6ZG) c6ZGL = 0;
			c6ZHL++; if (c6ZHL < 0 || c6ZHL > d6ZH) c6ZHL = 0;
			c6ZIL++; if (c6ZIL < 0 || c6ZIL > d6ZI) c6ZIL = 0;
			c6ZJL++; if (c6ZJL < 0 || c6ZJL > d6ZJ) c6ZJL = 0;
			c6ZKL++; if (c6ZKL < 0 || c6ZKL > d6ZK) c6ZKL = 0;
			
			double o6ZFL = a6ZFL[c6ZFL-((c6ZFL > d6ZF)?d6ZF+1:0)];
			double o6ZGL = a6ZGL[c6ZGL-((c6ZGL > d6ZG)?d6ZG+1:0)];
			double o6ZHL = a6ZHL[c6ZHL-((c6ZHL > d6ZH)?d6ZH+1:0)];
			double o6ZIL = a6ZIL[c6ZIL-((c6ZIL > d6ZI)?d6ZI+1:0)];
			double o6ZJL = a6ZJL[c6ZJL-((c6ZJL > d6ZJ)?d6ZJ+1:0)];
			double o6ZKL = a6ZKL[c6ZKL-((c6ZKL > d6ZK)?d6ZK+1:0)];
			
			a6AR[c6AR] = ((o6BR*2.0) - (o6HR + o6NR + o6TR + o6ZAR + o6ZGR));
			a6GR[c6GR] = ((o6HR*2.0) - (o6BR + o6NR + o6TR + o6ZAR + o6ZGR));
			a6MR[c6MR] = ((o6NR*2.0) - (o6BR + o6HR + o6TR + o6ZAR + o6ZGR));
			a6SR[c6SR] = ((o6TR*2.0) - (o6BR + o6HR + o6NR + o6ZAR + o6ZGR));
			a6YR[c6YR] = ((o6ZAR*2.0) - (o6BR + o6HR + o6NR + o6TR + o6ZGR));
			a6ZFR[c6ZFR] = ((o6ZGR*2.0) - (o6BR + o6HR + o6NR + o6TR + o6ZAR));
			
			c6AR++; if (c6AR < 0 || c6AR > d6A) c6AR = 0;
			c6GR++; if (c6GR < 0 || c6GR > d6G) c6GR = 0;
			c6MR++; if (c6MR < 0 || c6MR > d6M) c6MR = 0;
			c6SR++; if (c6SR < 0 || c6SR > d6S) c6SR = 0;
			c6YR++; if (c6YR < 0 || c6YR > d6Y) c6YR = 0;
			c6ZFR++; if (c6ZFR < 0 || c6ZFR > d6ZF) c6ZFR = 0;
			
			double o6AR = a6AR[c6AR-((c6AR > d6A)?d6A+1:0)];
			double o6GR = a6GR[c6GR-((c6GR > d6G)?d6G+1:0)];
			double o6MR = a6MR[c6MR-((c6MR > d6M)?d6M+1:0)];
			double o6SR = a6SR[c6SR-((c6SR > d6S)?d6S+1:0)];
			double o6YR = a6YR[c6YR-((c6YR > d6Y)?d6Y+1:0)];
			double o6ZFR = a6ZFR[c6ZFR-((c6ZFR > d6ZF)?d6ZF+1:0)];
			
			//-------- six
			
			f6AL = ((o6AR*2.0) - (o6GR + o6MR + o6SR + o6YR + o6ZFR));
			f6BL = ((o6GR*2.0) - (o6AR + o6MR + o6SR + o6YR + o6ZFR));
			f6CL = ((o6MR*2.0) - (o6AR + o6GR + o6SR + o6YR + o6ZFR));
			f6DL = ((o6SR*2.0) - (o6AR + o6GR + o6MR + o6YR + o6ZFR));
			f6EL = ((o6YR*2.0) - (o6AR + o6GR + o6MR + o6SR + o6ZFR));
			f6FL = ((o6ZFR*2.0) - (o6AR + o6GR + o6MR + o6SR + o6YR));
			
			f6FR = ((o6ZFL*2.0) - (o6ZGL + o6ZHL + o6ZIL + o6ZJL + o6ZKL));
			f6LR = ((o6ZGL*2.0) - (o6ZFL + o6ZHL + o6ZIL + o6ZJL + o6ZKL));
			f6RR = ((o6ZHL*2.0) - (o6ZFL + o6ZGL + o6ZIL + o6ZJL + o6ZKL));
			f6XR = ((o6ZIL*2.0) - (o6ZFL + o6ZGL + o6ZHL + o6ZJL + o6ZKL));
			f6ZER = ((o6ZJL*2.0) - (o6ZFL + o6ZGL + o6ZHL + o6ZIL + o6ZKL));
			f6ZKR = ((o6ZKL*2.0) - (o6ZFL + o6ZGL + o6ZHL + o6ZIL + o6ZJL));
			
			inputSampleL = (o6ZFL + o6ZGL + o6ZHL + o6ZIL + o6ZJL + o6ZKL)*0.001953125;
			inputSampleR = (o6AR + o6GR + o6MR + o6SR + o6YR + o6ZFR)*0.001953125;
			
			f6AL = (f6AL+f6AL+f6AL+fabs(avg6L))*0.25; avg6L = f6AL;
			f6FR = (f6FR+f6FR+f6FR+fabs(avg6R))*0.25; avg6R = f6FR;
			//manipulating deep reverb tail for realism
			
			inputSampleL += (earlyReflectionL * earlyLoudness);
			inputSampleR += (earlyReflectionR * earlyLoudness);
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		double CBL = (bez[bez_CL]*(1.0-bez[bez_cycle]))+(bez[bez_BL]*bez[bez_cycle]);
		double CBR = (bez[bez_CR]*(1.0-bez[bez_cycle]))+(bez[bez_BR]*bez[bez_cycle]);
		double BAL = (bez[bez_BL]*(1.0-bez[bez_cycle]))+(bez[bez_AL]*bez[bez_cycle]);
		double BAR = (bez[bez_BR]*(1.0-bez[bez_cycle]))+(bez[bez_AR]*bez[bez_cycle]);
		double CBAL = (bez[bez_BL]+(CBL*(1.0-bez[bez_cycle]))+(BAL*bez[bez_cycle]))*-0.125;
		double CBAR = (bez[bez_BR]+(CBR*(1.0-bez[bez_cycle]))+(BAR*bez[bez_cycle]))*-0.125;
		inputSampleL = CBAL;
		inputSampleR = CBAR;
		
		inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0-wet));
		inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0-wet));
		
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
