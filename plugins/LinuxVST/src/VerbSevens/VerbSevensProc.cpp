/* ========================================
 *  VerbSevens - VerbSevens.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __VerbSevens_H
#include "VerbSevens.h"
#endif

void VerbSevens::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 48000.0;
	overallscale *= getSampleRate();
	
#ifdef THREEBYTHREE
	double reg3n = 0.03125;
	double ga3n = (reg3n+reg3n+0.015625)*0.45;
	reg3n = (reg3n+reg3n+0.015625)*0.38;
#endif
#ifdef FOURBYFOUR
	double reg4n = 0.0625;
	double ga4n = (reg4n+reg4n+0.03125)*0.22;
	reg4n = (reg4n+reg4n+0.03125)*0.345;
#endif
#ifdef FIVEBYFIVE
	double reg5n = 0.000244140625;
	double ga5n = (reg5n+reg5n+0.00048828125)*0.39;
	reg5n = (reg5n+reg5n+0.00048828125)*0.28;
#endif
#ifdef SIXBYSIX
	double reg6n = 0.0078125;
	double ga6n = (reg6n+reg6n+0.00390625)*0.32;
	reg6n = (reg6n+reg6n+0.00390625)*0.34;
#endif
#ifdef SIXBYSIXMONO
	double reg6n = 0.0078125;
	double ga6n = (reg6n+reg6n+0.00390625)*0.32;
	reg6n = (reg6n+reg6n+0.00390625)*0.34;
#endif
	//adding more digits to that fraction gets closer to infinite f6. 0.333 is already long
	
	double downRez = (overallscale/(overallscale+0.99999))/overallscale;
	downRez = fmin(fmax(downRez,0.0005),1.0/overallscale);
	
	int bezFraction = (int)(1.0/downRez);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	downRez = 0.99999999 / bezFraction;
	bezTrim = 1.0-(downRez*bezTrim);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		bez[bez_cycle] += downRez;
		bez[bez_SampL] += (inputSampleL * downRez);
		bez[bez_SampR] += (inputSampleR * downRez);
		if (bez[bez_cycle] > bezTrim) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			
			inputSampleL = bez[bez_SampL];
			inputSampleR = bez[bez_SampR];
			
#ifdef THREEBYTHREE
			a3AL[c3AL] = inputSampleL;// + (f3AL * reg3n);
			a3BL[c3BL] = inputSampleL;// + (f3BL * reg3n);
			a3CL[c3CL] = inputSampleL;// + (f3CL * reg3n);
			
			a3CR[c3CR] = inputSampleR;// + (f3CR * reg3n);
			a3FR[c3FR] = inputSampleR;// + (f3FR * reg3n);
			a3IR[c3IR] = inputSampleR;// + (f3IR * reg3n);
			
			c3AL++; if (c3AL < 0 || c3AL > d3A) c3AL = 0;
			c3BL++; if (c3BL < 0 || c3BL > d3B) c3BL = 0;
			c3CL++; if (c3CL < 0 || c3CL > d3C) c3CL = 0;
			c3CR++; if (c3CR < 0 || c3CR > d3C) c3CR = 0;
			c3FR++; if (c3FR < 0 || c3FR > d3F) c3FR = 0;
			c3IR++; if (c3IR < 0 || c3IR > d3I) c3IR = 0;
			
			double o3AL = a3AL[c3AL-((c3AL > d3A)?c3AL+1:0)];
			double o3BL = a3BL[c3BL-((c3BL > d3B)?c3BL+1:0)];
			double o3CL = a3CL[c3CL-((c3CL > d3C)?c3CL+1:0)];
			double o3CR = a3CR[c3CR-((c3CR > d3C)?c3CR+1:0)];
			double o3FR = a3FR[c3FR-((c3FR > d3F)?c3FR+1:0)];
			double o3IR = a3IR[c3IR-((c3IR > d3I)?c3IR+1:0)];
			
			a3DL[c3DL] = (((o3BL + o3CL) * -2.0) + o3AL);
			a3EL[c3EL] = (((o3AL + o3CL) * -2.0) + o3BL);
			a3FL[c3FL] = (((o3AL + o3BL) * -2.0) + o3CL);
			a3BR[c3BR] = (((o3FR + o3IR) * -2.0) + o3CR);
			a3ER[c3ER] = (((o3CR + o3IR) * -2.0) + o3FR);
			a3HR[c3HR] = (((o3CR + o3FR) * -2.0) + o3IR);
			
			c3DL++; if (c3DL < 0 || c3DL > d3D) c3DL = 0;
			c3EL++; if (c3EL < 0 || c3EL > d3E) c3EL = 0;
			c3FL++; if (c3FL < 0 || c3FL > d3F) c3FL = 0;
			c3BR++; if (c3BR < 0 || c3BR > d3B) c3BR = 0;
			c3ER++; if (c3ER < 0 || c3ER > d3E) c3ER = 0;
			c3HR++; if (c3HR < 0 || c3HR > d3H) c3HR = 0;
			
			double o3DL = a3DL[c3DL-((c3DL > d3D)?c3DL+1:0)];
			double o3EL = a3EL[c3EL-((c3EL > d3E)?c3EL+1:0)];
			double o3FL = a3FL[c3FL-((c3FL > d3F)?c3FL+1:0)];
			double o3BR = a3BR[c3BR-((c3BR > d3B)?c3BR+1:0)];
			double o3ER = a3ER[c3ER-((c3ER > d3E)?c3ER+1:0)];
			double o3HR = a3HR[c3HR-((c3HR > d3H)?c3HR+1:0)];
			
			a3GL[c3GL] = (((o3EL + o3FL) * -2.0) + o3DL);
			a3HL[c3HL] = (((o3DL + o3FL) * -2.0) + o3EL);
			a3IL[c3IL] = (((o3DL + o3EL) * -2.0) + o3FL);
			a3AR[c3AR] = (((o3ER + o3HR) * -2.0) + o3BR);
			a3DR[c3DR] = (((o3BR + o3HR) * -2.0) + o3ER);
			a3GR[c3GR] = (((o3BR + o3ER) * -2.0) + o3HR);
			
			c3GL++; if (c3GL < 0 || c3GL > d3G) c3GL = 0;
			c3HL++; if (c3HL < 0 || c3HL > d3H) c3HL = 0;
			c3IL++; if (c3IL < 0 || c3IL > d3I) c3IL = 0;
			c3AR++; if (c3AR < 0 || c3AR > d3A) c3AR = 0;
			c3DR++; if (c3DR < 0 || c3DR > d3D) c3DR = 0;
			c3GR++; if (c3GR < 0 || c3GR > d3G) c3GR = 0;
			
			double o3GL = a3GL[c3GL-((c3GL > d3G)?c3GL+1:0)];
			double o3HL = a3HL[c3HL-((c3HL > d3H)?c3HL+1:0)];
			double o3IL = a3IL[c3IL-((c3IL > d3I)?c3IL+1:0)];
			double o3AR = a3AR[c3AR-((c3AR > d3A)?c3AR+1:0)];
			double o3DR = a3DR[c3DR-((c3DR > d3D)?c3DR+1:0)];
			double o3GR = a3GR[c3GR-((c3GR > d3G)?c3GR+1:0)];
			
			f3AL = (((o3GR + o3HR) * -2.0) + o3IR);				
			f3BL = (((o3GR + o3HR) * -2.0) + o3IR);
			f3CL = (((o3GR + o3HR) * -2.0) + o3IR);
			
			f3CR = (((o3AL + o3DL) * -2.0) + o3GL);
			f3FR = (((o3AL + o3DL) * -2.0) + o3GL);
			f3IR = (((o3AL + o3DL) * -2.0) + o3GL);
			
			double inputSampleL = (o3GL + o3HL + o3IL)*ga3n;
			double inputSampleR = (o3AR + o3DR + o3GR)*ga3n;
			
			f3AL = (f3AL+f3AL+f3AL+fabs(avg3L))*0.25; avg3L = f3AL;
			f3CR = (f3CR+f3CR+f3CR+fabs(avg3R))*0.25; avg3R = f3CR;
			//manipulating deep reverb tail for realism			
			
#endif
#ifdef FOURBYFOUR
			a4AL[c4AL] = fma(f4AL,reg4n,inputSampleL);
			a4BL[c4BL] = fma(f4BL,reg4n,inputSampleL);
			a4CL[c4CL] = fma(f4CL,reg4n,inputSampleL);
			a4DL[c4DL] = fma(f4DL,reg4n,inputSampleL);
			a4DR[c4DR] = fma(f4DR,reg4n,inputSampleR);
			a4HR[c4HR] = fma(f4HR,reg4n,inputSampleR);
			a4LR[c4LR] = fma(f4LR,reg4n,inputSampleR);
			a4PR[c4PR] = fma(f4PR,reg4n,inputSampleR);
			c4AL++; c4BL++; c4CL++; c4DL++;
			c4DR++; c4HR++; c4LR++; c4PR++;
			if (c4AL > d4A) c4AL = 0;
			if (c4BL > d4B) c4BL = 0;
			if (c4CL > d4C) c4CL = 0;
			if (c4DL > d4D) c4DL = 0;
			if (c4DR > d4D) c4DR = 0;
			if (c4HR > d4H) c4HR = 0;
			if (c4LR > d4L) c4LR = 0;
			if (c4PR > d4P) c4PR = 0;
			hAL = a4AL[c4AL]; hBL = a4BL[c4BL]; hCL = a4CL[c4CL]; hDL = a4DL[c4DL];
			hAR = a4DR[c4DR]; hBR = a4HR[c4HR]; hCR = a4LR[c4LR]; hDR = a4PR[c4PR];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			a4EL[c4EL] = fma(hAL,2.0f,hEL);
			a4FL[c4FL] = fma(hBL,2.0f,hEL);
			a4GL[c4GL] = fma(hCL,2.0f,hEL);
			a4HL[c4HL] = fma(hDL,2.0f,hEL);
			a4CR[c4CR] = fma(hAR,2.0f,hER);
			a4GR[c4GR] = fma(hBR,2.0f,hER);
			a4KR[c4KR] = fma(hCR,2.0f,hER);
			a4OR[c4OR] = fma(hDR,2.0f,hER);
			c4EL++; c4FL++; c4GL++; c4HL++;
			c4CR++; c4GR++; c4KR++; c4OR++;
			if (c4EL > d4E) c4EL = 0;
			if (c4FL > d4F) c4FL = 0;
			if (c4GL > d4G) c4GL = 0;
			if (c4HL > d4H) c4HL = 0;
			if (c4CR > d4C) c4CR = 0;
			if (c4GR > d4G) c4GR = 0;
			if (c4KR > d4K) c4KR = 0;
			if (c4OR > d4O) c4OR = 0;
			hAL = a4EL[c4EL]; hBL = a4FL[c4FL]; hCL = a4GL[c4GL]; hDL = a4HL[c4HL];
			hAR = a4CR[c4CR]; hBR = a4GR[c4GR]; hCR = a4KR[c4KR]; hDR = a4OR[c4OR];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			a4IL[c4IL] = fma(hAL,2.0f,hEL);
			a4JL[c4JL] = fma(hBL,2.0f,hEL);
			a4KL[c4KL] = fma(hCL,2.0f,hEL);
			a4LL[c4LL] = fma(hDL,2.0f,hEL);
			a4BR[c4BR] = fma(hAR,2.0f,hER);
			a4FR[c4FR] = fma(hBR,2.0f,hER);
			a4JR[c4JR] = fma(hCR,2.0f,hER);
			a4NR[c4NR] = fma(hDR,2.0f,hER);
			c4IL++; c4JL++; c4KL++; c4LL++;
			c4BR++; c4FR++; c4JR++; c4NR++;
			if (c4IL > d4I) c4IL = 0;
			if (c4JL > d4J) c4JL = 0;
			if (c4KL > d4K) c4KL = 0;
			if (c4LL > d4L) c4LL = 0;
			if (c4BR > d4B) c4BR = 0;
			if (c4FR > d4F) c4FR = 0;
			if (c4JR > d4J) c4JR = 0;
			if (c4NR > d4N) c4NR = 0;
			hAL = a4IL[c4IL]; hBL = a4JL[c4JL]; hCL = a4KL[c4KL]; hDL = a4LL[c4LL];
			hAR = a4BR[c4BR]; hBR = a4FR[c4FR]; hCR = a4JR[c4JR]; hDR = a4NR[c4NR];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			a4ML[c4ML] = fma(hAL,2.0f,hEL);
			a4NL[c4NL] = fma(hBL,2.0f,hEL);
			a4OL[c4OL] = fma(hCL,2.0f,hEL);
			a4PL[c4PL] = fma(hDL,2.0f,hEL);
			a4AR[c4AR] = fma(hAR,2.0f,hER);
			a4ER[c4ER] = fma(hBR,2.0f,hER);
			a4IR[c4IR] = fma(hCR,2.0f,hER);
			a4MR[c4MR] = fma(hDR,2.0f,hER);
			c4ML++; c4NL++; c4OL++; c4PL++;
			c4AR++; c4ER++; c4IR++; c4MR++;
			if (c4ML > d4M) c4ML = 0;
			if (c4NL > d4N) c4NL = 0;
			if (c4OL > d4O) c4OL = 0;
			if (c4PL > d4P) c4PL = 0;
			if (c4AR > d4A) c4AR = 0;
			if (c4ER > d4E) c4ER = 0;
			if (c4IR > d4I) c4IR = 0;
			if (c4MR > d4M) c4MR = 0;
			hAL = a4ML[c4ML]; hBL = a4NL[c4NL]; hCL = a4OL[c4OL]; hDL = a4PL[c4PL];
			hAR = a4AR[c4AR]; hBR = a4ER[c4ER]; hCR = a4IR[c4IR]; hDR = a4MR[c4MR];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			f4AL = fma(hAR,2.0f,hER);				
			f4BL = fma(hBR,2.0f,hER);
			f4CL = fma(hCR,2.0f,hER);
			f4DL = fma(hDR,2.0f,hER);
			f4DR = fma(hAL,2.0f,hEL);				
			f4HR = fma(hBL,2.0f,hEL);
			f4LR = fma(hCL,2.0f,hEL);
			f4PR = fma(hDL,2.0f,hEL);
			inputSampleL = (hAL + hBL + hCL + hDL)*ga4n;
			inputSampleR = (hAR + hBR + hCR + hDR)*ga4n;
#endif
#ifdef FIVEBYFIVE
			a5AL[c5AL] = inputSampleL + (f5AL * reg5n);
			a5BL[c5BL] = inputSampleL + (f5BL * reg5n);
			a5CL[c5CL] = inputSampleL + (f5CL * reg5n);
			a5DL[c5DL] = inputSampleL + (f5DL * reg5n);
			a5EL[c5EL] = inputSampleL + (f5EL * reg5n);
			
			c5AL++; if (c5AL < 0 || c5AL > d5A) c5AL = 0;
			c5BL++; if (c5BL < 0 || c5BL > d5B) c5BL = 0;
			c5CL++; if (c5CL < 0 || c5CL > d5C) c5CL = 0;
			c5DL++; if (c5DL < 0 || c5DL > d5D) c5DL = 0;
			c5EL++; if (c5EL < 0 || c5EL > d5E) c5EL = 0;
			
			double o5AL = a5AL[c5AL-((c5AL > d5A)?d5A+1:0)];
			double o5BL = a5BL[c5BL-((c5BL > d5B)?d5B+1:0)];
			double o5CL = a5CL[c5CL-((c5CL > d5C)?d5C+1:0)];
			double o5DL = a5DL[c5DL-((c5DL > d5D)?d5D+1:0)];
			double o5EL = a5EL[c5EL-((c5EL > d5E)?d5E+1:0)];
			
			a5ER[c5ER] = inputSampleR + (f5ER * reg5n);
			a5JR[c5JR] = inputSampleR + (f5JR * reg5n);
			a5OR[c5OR] = inputSampleR + (f5OR * reg5n);
			a5TR[c5TR] = inputSampleR + (f5TR * reg5n);
			a5YR[c5YR] = inputSampleR + (f5YR * reg5n);
			
			c5ER++; if (c5ER < 0 || c5ER > d5E) c5ER = 0;
			c5JR++; if (c5JR < 0 || c5JR > d5J) c5JR = 0;
			c5OR++; if (c5OR < 0 || c5OR > d5O) c5OR = 0;
			c5TR++; if (c5TR < 0 || c5TR > d5T) c5TR = 0;
			c5YR++; if (c5YR < 0 || c5YR > d5Y) c5YR = 0;
			
			double o5ER = a5ER[c5ER-((c5ER > d5E)?d5E+1:0)];
			double o5JR = a5JR[c5JR-((c5JR > d5J)?d5J+1:0)];
			double o5OR = a5OR[c5OR-((c5OR > d5O)?d5O+1:0)];
			double o5TR = a5TR[c5TR-((c5TR > d5T)?d5T+1:0)];
			double o5YR = a5YR[c5YR-((c5YR > d5Y)?d5Y+1:0)];
			
			//-------- one
			
			a5FL[c5FL] = ((o5AL*3.0) - ((o5BL + o5CL + o5DL + o5EL)*2.0));
			a5GL[c5GL] = ((o5BL*3.0) - ((o5AL + o5CL + o5DL + o5EL)*2.0));
			a5HL[c5HL] = ((o5CL*3.0) - ((o5AL + o5BL + o5DL + o5EL)*2.0));
			a5IL[c5IL] = ((o5DL*3.0) - ((o5AL + o5BL + o5CL + o5EL)*2.0));
			a5JL[c5JL] = ((o5EL*3.0) - ((o5AL + o5BL + o5CL + o5DL)*2.0));
			
			c5FL++; if (c5FL < 0 || c5FL > d5F) c5FL = 0;
			c5GL++; if (c5GL < 0 || c5GL > d5G) c5GL = 0;
			c5HL++; if (c5HL < 0 || c5HL > d5H) c5HL = 0;
			c5IL++; if (c5IL < 0 || c5IL > d5I) c5IL = 0;
			c5JL++; if (c5JL < 0 || c5JL > d5J) c5JL = 0;
			
			double o5FL = a5FL[c5FL-((c5FL > d5F)?d5F+1:0)];
			double o5GL = a5GL[c5GL-((c5GL > d5G)?d5G+1:0)];
			double o5HL = a5HL[c5HL-((c5HL > d5H)?d5H+1:0)];
			double o5IL = a5IL[c5IL-((c5IL > d5I)?d5I+1:0)];
			double o5JL = a5JL[c5JL-((c5JL > d5J)?d5J+1:0)];
			
			a5DR[c5DR] = ((o5ER*3.0) - ((o5JR + o5OR + o5TR + o5YR)*2.0));
			a5IR[c5IR] = ((o5JR*3.0) - ((o5ER + o5OR + o5TR + o5YR)*2.0));
			a5NR[c5NR] = ((o5OR*3.0) - ((o5ER + o5JR + o5TR + o5YR)*2.0));
			a5SR[c5SR] = ((o5TR*3.0) - ((o5ER + o5JR + o5OR + o5YR)*2.0));
			a5XR[c5XR] = ((o5YR*3.0) - ((o5ER + o5JR + o5OR + o5TR)*2.0));
			
			c5DR++; if (c5DR < 0 || c5DR > d5D) c5DR = 0;
			c5IR++; if (c5IR < 0 || c5IR > d5I) c5IR = 0;
			c5NR++; if (c5NR < 0 || c5NR > d5N) c5NR = 0;
			c5SR++; if (c5SR < 0 || c5SR > d5S) c5SR = 0;
			c5XR++; if (c5XR < 0 || c5XR > d5X) c5XR = 0;
			
			double o5DR = a5DR[c5DR-((c5DR > d5D)?d5D+1:0)];
			double o5IR = a5IR[c5IR-((c5IR > d5I)?d5I+1:0)];
			double o5NR = a5NR[c5NR-((c5NR > d5N)?d5N+1:0)];
			double o5SR = a5SR[c5SR-((c5SR > d5S)?d5S+1:0)];
			double o5XR = a5XR[c5XR-((c5XR > d5X)?d5X+1:0)];
			
			//-------- two
			
			a5KL[c5KL] = ((o5FL*3.0) - ((o5GL + o5HL + o5IL + o5JL)*2.0));
			a5LL[c5LL] = ((o5GL*3.0) - ((o5FL + o5HL + o5IL + o5JL)*2.0));
			a5ML[c5ML] = ((o5HL*3.0) - ((o5FL + o5GL + o5IL + o5JL)*2.0));
			a5NL[c5NL] = ((o5IL*3.0) - ((o5FL + o5GL + o5HL + o5JL)*2.0));
			a5OL[c5OL] = ((o5JL*3.0) - ((o5FL + o5GL + o5HL + o5IL)*2.0));
			
			c5KL++; if (c5KL < 0 || c5KL > d5K) c5KL = 0;
			c5LL++; if (c5LL < 0 || c5LL > d5L) c5LL = 0;
			c5ML++; if (c5ML < 0 || c5ML > d5M) c5ML = 0;
			c5NL++; if (c5NL < 0 || c5NL > d5N) c5NL = 0;
			c5OL++; if (c5OL < 0 || c5OL > d5O) c5OL = 0;
			
			double o5KL = a5KL[c5KL-((c5KL > d5K)?d5K+1:0)];
			double o5LL = a5LL[c5LL-((c5LL > d5L)?d5L+1:0)];
			double o5ML = a5ML[c5ML-((c5ML > d5M)?d5M+1:0)];
			double o5NL = a5NL[c5NL-((c5NL > d5N)?d5N+1:0)];
			double o5OL = a5OL[c5OL-((c5OL > d5O)?d5O+1:0)];
			
			a5CR[c5CR] = ((o5DR*3.0) - ((o5IR + o5NR + o5SR + o5XR)*2.0));
			a5HR[c5HR] = ((o5IR*3.0) - ((o5DR + o5NR + o5SR + o5XR)*2.0));
			a5MR[c5MR] = ((o5NR*3.0) - ((o5DR + o5IR + o5SR + o5XR)*2.0));
			a5RR[c5RR] = ((o5SR*3.0) - ((o5DR + o5IR + o5NR + o5XR)*2.0));
			a5WR[c5WR] = ((o5XR*3.0) - ((o5DR + o5IR + o5NR + o5SR)*2.0));
			
			c5CR++; if (c5CR < 0 || c5CR > d5C) c5CR = 0;
			c5HR++; if (c5HR < 0 || c5HR > d5H) c5HR = 0;
			c5MR++; if (c5MR < 0 || c5MR > d5M) c5MR = 0;
			c5RR++; if (c5RR < 0 || c5RR > d5R) c5RR = 0;
			c5WR++; if (c5WR < 0 || c5WR > d5W) c5WR = 0;
			
			double o5CR = a5CR[c5CR-((c5CR > d5C)?d5C+1:0)];
			double o5HR = a5HR[c5HR-((c5HR > d5H)?d5H+1:0)];
			double o5MR = a5MR[c5MR-((c5MR > d5M)?d5M+1:0)];
			double o5RR = a5RR[c5RR-((c5RR > d5R)?d5R+1:0)];
			double o5WR = a5WR[c5WR-((c5WR > d5W)?d5W+1:0)];
			
			//-------- three
			
			a5PL[c5PL] = ((o5KL*3.0) - ((o5LL + o5ML + o5NL + o5OL)*2.0));
			a5QL[c5QL] = ((o5LL*3.0) - ((o5KL + o5ML + o5NL + o5OL)*2.0));
			a5RL[c5RL] = ((o5ML*3.0) - ((o5KL + o5LL + o5NL + o5OL)*2.0));
			a5SL[c5SL] = ((o5NL*3.0) - ((o5KL + o5LL + o5ML + o5OL)*2.0));
			a5TL[c5TL] = ((o5OL*3.0) - ((o5KL + o5LL + o5ML + o5NL)*2.0));
			
			c5PL++; if (c5PL < 0 || c5PL > d5P) c5PL = 0;
			c5QL++; if (c5QL < 0 || c5QL > d5Q) c5QL = 0;
			c5RL++; if (c5RL < 0 || c5RL > d5R) c5RL = 0;
			c5SL++; if (c5SL < 0 || c5SL > d5S) c5SL = 0;
			c5TL++; if (c5TL < 0 || c5TL > d5T) c5TL = 0;
			
			double o5PL = a5PL[c5PL-((c5PL > d5P)?d5P+1:0)];
			double o5QL = a5QL[c5QL-((c5QL > d5Q)?d5Q+1:0)];
			double o5RL = a5RL[c5RL-((c5RL > d5R)?d5R+1:0)];
			double o5SL = a5SL[c5SL-((c5SL > d5S)?d5S+1:0)];
			double o5TL = a5TL[c5TL-((c5TL > d5T)?d5T+1:0)];
			
			a5BR[c5BR] = ((o5CR*3.0) - ((o5HR + o5MR + o5RR + o5WR)*2.0));
			a5GR[c5GR] = ((o5HR*3.0) - ((o5CR + o5MR + o5RR + o5WR)*2.0));
			a5LR[c5LR] = ((o5MR*3.0) - ((o5CR + o5HR + o5RR + o5WR)*2.0));
			a5QR[c5QR] = ((o5RR*3.0) - ((o5CR + o5HR + o5MR + o5WR)*2.0));
			a5VR[c5VR] = ((o5WR*3.0) - ((o5CR + o5HR + o5MR + o5RR)*2.0));
			
			c5BR++; if (c5BR < 0 || c5BR > d5B) c5BR = 0;
			c5GR++; if (c5GR < 0 || c5GR > d5G) c5GR = 0;
			c5LR++; if (c5LR < 0 || c5LR > d5L) c5LR = 0;
			c5QR++; if (c5QR < 0 || c5QR > d5Q) c5QR = 0;
			c5VR++; if (c5VR < 0 || c5VR > d5V) c5VR = 0;
			
			double o5BR = a5BR[c5BR-((c5BR > d5B)?d5B+1:0)];
			double o5GR = a5GR[c5GR-((c5GR > d5G)?d5G+1:0)];
			double o5LR = a5LR[c5LR-((c5LR > d5L)?d5L+1:0)];
			double o5QR = a5QR[c5QR-((c5QR > d5Q)?d5Q+1:0)];
			double o5VR = a5VR[c5VR-((c5VR > d5V)?d5V+1:0)];
			
			//-------- four
			
			a5UL[c5UL] = ((o5PL*3.0) - ((o5QL + o5RL + o5SL + o5TL)*2.0));
			a5VL[c5VL] = ((o5QL*3.0) - ((o5PL + o5RL + o5SL + o5TL)*2.0));
			a5WL[c5WL] = ((o5RL*3.0) - ((o5PL + o5QL + o5SL + o5TL)*2.0));
			a5XL[c5XL] = ((o5SL*3.0) - ((o5PL + o5QL + o5RL + o5TL)*2.0));
			a5YL[c5YL] = ((o5TL*3.0) - ((o5PL + o5QL + o5RL + o5SL)*2.0));
			
			c5UL++; if (c5UL < 0 || c5UL > d5U) c5UL = 0;
			c5VL++; if (c5VL < 0 || c5VL > d5V) c5VL = 0;
			c5WL++; if (c5WL < 0 || c5WL > d5W) c5WL = 0;
			c5XL++; if (c5XL < 0 || c5XL > d5X) c5XL = 0;
			c5YL++; if (c5YL < 0 || c5YL > d5Y) c5YL = 0;
			
			double o5UL = a5UL[c5UL-((c5UL > d5U)?d5U+1:0)];
			double o5VL = a5VL[c5VL-((c5VL > d5V)?d5V+1:0)];
			double o5WL = a5WL[c5WL-((c5WL > d5W)?d5W+1:0)];
			double o5XL = a5XL[c5XL-((c5XL > d5X)?d5X+1:0)];
			double o5YL = a5YL[c5YL-((c5YL > d5Y)?d5Y+1:0)];
			
			a5AR[c5AR] = ((o5BR*3.0) - ((o5GR + o5LR + o5QR + o5VR)*2.0));
			a5FR[c5FR] = ((o5GR*3.0) - ((o5BR + o5LR + o5QR + o5VR)*2.0));
			a5KR[c5KR] = ((o5LR*3.0) - ((o5BR + o5GR + o5QR + o5VR)*2.0));
			a5PR[c5PR] = ((o5QR*3.0) - ((o5BR + o5GR + o5LR + o5VR)*2.0));
			a5UR[c5UR] = ((o5VR*3.0) - ((o5BR + o5GR + o5LR + o5QR)*2.0));
			
			c5AR++; if (c5AR < 0 || c5AR > d5A) c5AR = 0;
			c5FR++; if (c5FR < 0 || c5FR > d5F) c5FR = 0;
			c5KR++; if (c5KR < 0 || c5KR > d5K) c5KR = 0;
			c5PR++; if (c5PR < 0 || c5PR > d5P) c5PR = 0;
			c5UR++; if (c5UR < 0 || c5UR > d5U) c5UR = 0;
			
			double o5AR = a5AR[c5AR-((c5AR > d5A)?d5A+1:0)];
			double o5FR = a5FR[c5FR-((c5FR > d5F)?d5F+1:0)];
			double o5KR = a5KR[c5KR-((c5KR > d5K)?d5K+1:0)];
			double o5PR = a5PR[c5PR-((c5PR > d5P)?d5P+1:0)];
			double o5UR = a5UR[c5UR-((c5UR > d5U)?d5U+1:0)];
			
			//-------- five
			
			f5AL = ((o5AR*3.0) - ((o5FR + o5KR + o5PR + o5UR)*2.0));
			f5BL = ((o5FR*3.0) - ((o5AR + o5KR + o5PR + o5UR)*2.0));
			f5CL = ((o5KR*3.0) - ((o5AR + o5FR + o5PR + o5UR)*2.0));
			f5DL = ((o5PR*3.0) - ((o5AR + o5FR + o5KR + o5UR)*2.0));
			f5EL = ((o5UR*3.0) - ((o5AR + o5FR + o5KR + o5PR)*2.0));
			
			f5ER = ((o5UL*3.0) - ((o5VL + o5WL + o5XL + o5YL)*2.0));
			f5JR = ((o5VL*3.0) - ((o5UL + o5WL + o5XL + o5YL)*2.0));
			f5OR = ((o5WL*3.0) - ((o5UL + o5VL + o5XL + o5YL)*2.0));
			f5TR = ((o5XL*3.0) - ((o5UL + o5VL + o5WL + o5YL)*2.0));
			f5YR = ((o5YL*3.0) - ((o5UL + o5VL + o5WL + o5XL)*2.0));
			
			inputSampleL = (o5UL + o5VL + o5WL + o5XL + o5YL)*ga5n;
			inputSampleR = (o5AR + o5FR + o5KR + o5PR + o5UR)*ga5n;
			
			f5AL = (f5AL+f5AL+f5AL+fabs(avg5L))*0.25; avg5L = f5AL;
			f5ER = (f5ER+f5ER+f5ER+fabs(avg5R))*0.25; avg5R = f5ER;
			//manipulating deep reverb tail for realism
#endif			
#ifdef SIXBYSIX //instead of roomNoiseR, use reg6n
			
			a6AL[c6AL] = fma(f6BL,reg6n,inputSampleL);
			a6BL[c6BL] = fma(f6CL,reg6n,inputSampleL);
			a6CL[c6CL] = fma(f6DL,reg6n,inputSampleL);
			a6DL[c6DL] = fma(f6EL,reg6n,inputSampleL);
			a6EL[c6EL] = fma(f6FL,reg6n,inputSampleL);
			a6FL[c6FL] = fma(f6AL,reg6n,inputSampleL);
			
			a6FR[c6FR] = fma(f6LR,reg6n,inputSampleR);
			a6LR[c6LR] = fma(f6RR,reg6n,inputSampleR);
			a6RR[c6RR] = fma(f6XR,reg6n,inputSampleR);
			a6XR[c6XR] = fma(f6ZER,reg6n,inputSampleR);
			a6ZER[c6ZER] = fma(f6ZKR,reg6n,inputSampleR);
			a6ZKR[c6ZKR] = fma(f6FR,reg6n,inputSampleR);
			
			//left verb
			
			c6AL++; c6BL++; c6CL++; c6DL++; c6EL++; c6FL++; 
			
			if (c6AL > d6A) c6AL = 0;
			if (c6BL > d6B) c6BL = 0;
			if (c6CL > d6C) c6CL = 0;
			if (c6DL > d6D) c6DL = 0;
			if (c6EL > d6E) c6EL = 0;
			if (c6FL > d6F) c6FL = 0;
			
			hA = a6AL[c6AL-((c6AL > d6A)?d6A+1:0)];
			hB = a6BL[c6BL-((c6BL > d6B)?d6B+1:0)];
			hC = a6CL[c6CL-((c6CL > d6C)?d6C+1:0)];
			hD = a6DL[c6DL-((c6DL > d6D)?d6D+1:0)];
			hE = a6EL[c6EL-((c6EL > d6E)?d6E+1:0)];
			hF = a6FL[c6FL-((c6FL > d6F)?d6F+1:0)];
			
			a6GL[c6GL] = hB + hC + hD + hE + hF;
			a6HL[c6HL] = (hA + hC + hF) - (hD + hE);
			a6IL[c6IL] = (hA + hB + hD) - (hE + hF);
			a6JL[c6JL] = (hA + hC + hE) - (hB + hF);
			a6KL[c6KL] = (hA + hD + hF) - (hB + hC);
			a6LL[c6LL] = (hA + hB + hE) - (hC + hD);
			
			c6GL++; c6HL++; c6IL++; c6JL++; c6KL++; c6LL++; 
			
			if (c6GL > d6G) c6GL = 0;
			if (c6HL > d6H) c6HL = 0;
			if (c6IL > d6I) c6IL = 0;
			if (c6JL > d6J) c6JL = 0;
			if (c6KL > d6K) c6KL = 0;
			if (c6LL > d6L) c6LL = 0;
			
			hA = a6GL[c6GL-((c6GL > d6G)?d6G+1:0)];
			hB = a6HL[c6HL-((c6HL > d6H)?d6H+1:0)];
			hC = a6IL[c6IL-((c6IL > d6I)?d6I+1:0)];
			hD = a6JL[c6JL-((c6JL > d6J)?d6J+1:0)];
			hE = a6KL[c6KL-((c6KL > d6K)?d6K+1:0)];
			hF = a6LL[c6LL-((c6LL > d6L)?d6L+1:0)];
			
			a6ML[c6ML] = hB + hC + hD + hE + hF;
			a6NL[c6NL] = (hA + hC + hF) - (hD + hE);
			a6OL[c6OL] = (hA + hB + hD) - (hE + hF);
			a6PL[c6PL] = (hA + hC + hE) - (hB + hF);
			a6QL[c6QL] = (hA + hD + hF) - (hB + hC);
			a6RL[c6RL] = (hA + hB + hE) - (hC + hD);
			
			
			c6ML++; c6NL++; c6OL++; c6PL++; c6QL++; c6RL++; 
			
			if (c6ML > d6M) c6ML = 0;
			if (c6NL > d6N) c6NL = 0;
			if (c6OL > d6O) c6OL = 0;
			if (c6PL > d6P) c6PL = 0;
			if (c6QL > d6Q) c6QL = 0;
			if (c6RL > d6R) c6RL = 0;
			
			hA = a6ML[c6ML-((c6ML > d6M)?d6M+1:0)];
			hB = a6NL[c6NL-((c6NL > d6N)?d6N+1:0)];
			hC = a6OL[c6OL-((c6OL > d6O)?d6O+1:0)];
			hD = a6PL[c6PL-((c6PL > d6P)?d6P+1:0)];
			hE = a6QL[c6QL-((c6QL > d6Q)?d6Q+1:0)];
			hF = a6RL[c6RL-((c6RL > d6R)?d6R+1:0)];
			
			a6SL[c6SL] = hB + hC + hD + hE + hF;
			a6TL[c6TL] = (hA + hC + hF) - (hD + hE);
			a6UL[c6UL] = (hA + hB + hD) - (hE + hF);
			a6VL[c6VL] = (hA + hC + hE) - (hB + hF);
			a6WL[c6WL] = (hA + hD + hF) - (hB + hC);
			a6XL[c6XL] = (hA + hB + hE) - (hC + hD);
			
			
			c6SL++; c6TL++; c6UL++; c6VL++; c6WL++; c6XL++; 
			
			if (c6SL > d6S) c6SL = 0;
			if (c6TL > d6T) c6TL = 0;
			if (c6UL > d6U) c6UL = 0;
			if (c6VL > d6V) c6VL = 0;
			if (c6WL > d6W) c6WL = 0;
			if (c6XL > d6X) c6XL = 0;
			
			hA = a6SL[c6SL-((c6SL > d6S)?d6S+1:0)];
			hB = a6TL[c6TL-((c6TL > d6T)?d6T+1:0)];
			hC = a6UL[c6UL-((c6UL > d6U)?d6U+1:0)];
			hD = a6VL[c6VL-((c6VL > d6V)?d6V+1:0)];
			hE = a6WL[c6WL-((c6WL > d6W)?d6W+1:0)];
			hF = a6XL[c6XL-((c6XL > d6X)?d6X+1:0)];
			
			a6YL[c6YL] = hB + hC + hD + hE + hF;
			a6ZAL[c6ZAL] = (hA + hC + hF) - (hD + hE);
			a6ZBL[c6ZBL] = (hA + hB + hD) - (hE + hF);
			a6ZCL[c6ZCL] = (hA + hC + hE) - (hB + hF);
			a6ZDL[c6ZDL] = (hA + hD + hF) - (hB + hC);
			a6ZEL[c6ZEL] = (hA + hB + hE) - (hC + hD);
			
			c6YL++; c6ZAL++; c6ZBL++; c6ZCL++; c6ZDL++; c6ZEL++; 
			
			if (c6YL > d6Y) c6YL = 0;
			if (c6ZAL > d6ZA) c6ZAL = 0;
			if (c6ZBL > d6ZB) c6ZBL = 0;
			if (c6ZCL > d6ZC) c6ZCL = 0;
			if (c6ZDL > d6ZD) c6ZDL = 0;
			if (c6ZEL > d6ZE) c6ZEL = 0;
			
			hA = a6YL[c6YL-((c6YL > d6Y)?d6Y+1:0)];
			hB = a6ZAL[c6ZAL-((c6ZAL > d6ZA)?d6ZA+1:0)];
			hC = a6ZBL[c6ZBL-((c6ZBL > d6ZB)?d6ZB+1:0)];
			hD = a6ZCL[c6ZCL-((c6ZCL > d6ZC)?d6ZC+1:0)];
			hE = a6ZDL[c6ZDL-((c6ZDL > d6ZD)?d6ZD+1:0)];
			hF = a6ZEL[c6ZEL-((c6ZEL > d6ZE)?d6ZE+1:0)];
			
			a6ZFL[c6ZFL] = hB + hC + hD + hE + hF;
			a6ZGL[c6ZGL] = (hA + hC + hF) - (hD + hE);
			a6ZHL[c6ZHL] = (hA + hB + hD) - (hE + hF);
			a6ZIL[c6ZIL] = (hA + hC + hE) - (hB + hF);
			a6ZJL[c6ZJL] = (hA + hD + hF) - (hB + hC);
			a6ZKL[c6ZKL] = (hA + hB + hE) - (hC + hD);
			
			c6ZFL++; c6ZGL++; c6ZHL++; c6ZIL++; c6ZJL++; c6ZKL++; 
			
			if (c6ZFL > d6ZF) c6ZFL = 0;
			if (c6ZGL > d6ZG) c6ZGL = 0;
			if (c6ZHL > d6ZH) c6ZHL = 0;
			if (c6ZIL > d6ZI) c6ZIL = 0;
			if (c6ZJL > d6ZJ) c6ZJL = 0;
			if (c6ZKL > d6ZK) c6ZKL = 0;
			
			hA = a6ZFL[c6ZFL-((c6ZFL > d6ZF)?d6ZF+1:0)];
			hB = a6ZGL[c6ZGL-((c6ZGL > d6ZG)?d6ZG+1:0)];
			hC = a6ZHL[c6ZHL-((c6ZHL > d6ZH)?d6ZH+1:0)];
			hD = a6ZIL[c6ZIL-((c6ZIL > d6ZI)?d6ZI+1:0)];
			hE = a6ZJL[c6ZJL-((c6ZJL > d6ZJ)?d6ZJ+1:0)];
			hF = a6ZKL[c6ZKL-((c6ZKL > d6ZK)?d6ZK+1:0)];
			
			f6FR = hB + hC + hD + hE + hF;
			f6LR = (hA + hC + hF) - (hD + hE);
			f6RR = (hA + hB + hD) - (hE + hF);
			f6XR = (hA + hC + hE) - (hB + hF);
			f6ZER = (hA + hD + hF) - (hB + hC);
			f6ZKR = (hA + hB + hE) - (hC + hD);
			
			inputSampleL = (hB + hC + hD + hE + hF)*ga6n;
			
			//right verb
			
			c6FR++; c6LR++; c6RR++; c6XR++; c6ZER++; c6ZKR++; 
			
			if (c6FR > d6F) c6FR = 0;
			if (c6LR > d6L) c6LR = 0;
			if (c6RR > d6R) c6RR = 0;
			if (c6XR > d6X) c6XR = 0;
			if (c6ZER > d6ZE) c6ZER = 0;
			if (c6ZKR > d6ZK) c6ZKR = 0;
			
			hA = a6FR[c6FR-((c6FR > d6F)?d6F+1:0)];
			hB = a6LR[c6LR-((c6LR > d6L)?d6L+1:0)];
			hC = a6RR[c6RR-((c6RR > d6R)?d6R+1:0)];
			hD = a6XR[c6XR-((c6XR > d6X)?d6X+1:0)];
			hE = a6ZER[c6ZER-((c6ZER > d6ZE)?d6ZE+1:0)];
			hF = a6ZKR[c6ZKR-((c6ZKR > d6ZK)?d6ZK+1:0)];
			
			a6ER[c6ER] = hB + hC + hD + hE + hF;
			a6KR[c6KR] = (hA + hC + hF) - (hD + hE);
			a6QR[c6QR] = (hA + hB + hD) - (hE + hF);
			a6WR[c6WR] = (hA + hC + hE) - (hB + hF);
			a6ZDR[c6ZDR] = (hA + hD + hF) - (hB + hC);
			a6ZJR[c6ZJR] = (hA + hB + hE) - (hC + hD);
			
			c6ER++; c6KR++; c6QR++; c6WR++; c6ZDR++; c6ZJR++; 
			
			if (c6ER > d6E) c6ER = 0;
			if (c6KR > d6K) c6KR = 0;
			if (c6QR > d6Q) c6QR = 0;
			if (c6WR > d6W) c6WR = 0;
			if (c6ZDR > d6ZD) c6ZDR = 0;
			if (c6ZJR > d6ZJ) c6ZJR = 0;
			
			hA = a6ER[c6ER-((c6ER > d6E)?d6E+1:0)];
			hB = a6KR[c6KR-((c6KR > d6K)?d6K+1:0)];
			hC = a6QR[c6QR-((c6QR > d6Q)?d6Q+1:0)];
			hD = a6WR[c6WR-((c6WR > d6W)?d6W+1:0)];
			hE = a6ZDR[c6ZDR-((c6ZDR > d6ZD)?d6ZD+1:0)];
			hF = a6ZJR[c6ZJR-((c6ZJR > d6ZJ)?d6ZJ+1:0)];
			
			a6DR[c6DR] = hB + hC + hD + hE + hF;
			a6JR[c6JR] = (hA + hC + hF) - (hD + hE);
			a6PR[c6PR] = (hA + hB + hD) - (hE + hF);
			a6VR[c6VR] = (hA + hC + hE) - (hB + hF);
			a6ZCR[c6ZCR] = (hA + hD + hF) - (hB + hC);
			a6ZIR[c6ZIR] = (hA + hB + hE) - (hC + hD);
			
			c6DR++; c6JR++; c6PR++; c6VR++; c6ZCR++; c6ZIR++; 
			
			if (c6DR > d6D) c6DR = 0;
			if (c6JR > d6J) c6JR = 0;
			if (c6PR > d6P) c6PR = 0;
			if (c6VR > d6V) c6VR = 0;
			if (c6ZCR > d6ZC) c6ZCR = 0;
			if (c6ZIR > d6ZI) c6ZIR = 0;
			
			hA = a6DR[c6DR-((c6DR > d6D)?d6D+1:0)];
			hB = a6JR[c6JR-((c6JR > d6J)?d6J+1:0)];
			hC = a6PR[c6PR-((c6PR > d6P)?d6P+1:0)];
			hD = a6VR[c6VR-((c6VR > d6V)?d6V+1:0)];
			hE = a6ZCR[c6ZCR-((c6ZCR > d6ZC)?d6ZC+1:0)];
			hF = a6ZIR[c6ZIR-((c6ZIR > d6ZI)?d6ZI+1:0)];
			
			a6CR[c6CR] = hB + hC + hD + hE + hF;
			a6IR[c6IR] = (hA + hC + hF) - (hD + hE);
			a6OR[c6OR] = (hA + hB + hD) - (hE + hF);
			a6UR[c6UR] = (hA + hC + hE) - (hB + hF);
			a6ZBR[c6ZBR] = (hA + hD + hF) - (hB + hC);
			a6ZHR[c6ZHR] = (hA + hB + hE) - (hC + hD);
			
			c6CR++; c6IR++; c6OR++; c6UR++; c6ZBR++; c6ZHR++; 
			
			if (c6CR > d6C) c6CR = 0;
			if (c6IR > d6I) c6IR = 0;
			if (c6OR > d6O) c6OR = 0;
			if (c6UR > d6U) c6UR = 0;
			if (c6ZBR > d6ZB) c6ZBR = 0;
			if (c6ZHR > d6ZH) c6ZHR = 0;
			
			hA = a6CR[c6CR-((c6CR > d6C)?d6C+1:0)];
			hB = a6IR[c6IR-((c6IR > d6I)?d6I+1:0)];
			hC = a6OR[c6OR-((c6OR > d6O)?d6O+1:0)];
			hD = a6UR[c6UR-((c6UR > d6U)?d6U+1:0)];
			hE = a6ZBR[c6ZBR-((c6ZBR > d6ZB)?d6ZB+1:0)];
			hF = a6ZHR[c6ZHR-((c6ZHR > d6ZH)?d6ZH+1:0)];
			
			a6BR[c6BR] = hB + hC + hD + hE + hF;
			a6HR[c6HR] = (hA + hC + hF) - (hD + hE);
			a6NR[c6NR] = (hA + hB + hD) - (hE + hF);
			a6TR[c6TR] = (hA + hC + hE) - (hB + hF);
			a6ZAR[c6ZAR] = (hA + hD + hF) - (hB + hC);
			a6ZGR[c6ZGR] = (hA + hB + hE) - (hC + hD);
			
			c6BR++; c6HR++; c6NR++; c6TR++; c6ZBR++; c6ZGR++; 
			
			if (c6BR > d6B) c6BR = 0;
			if (c6HR > d6H) c6HR = 0;
			if (c6NR > d6N) c6NR = 0;
			if (c6TR > d6T) c6TR = 0;
			if (c6ZBR > d6ZB) c6ZBR = 0;
			if (c6ZGR > d6ZG) c6ZGR = 0;
			
			hA = a6BR[c6BR-((c6BR > d6B)?d6B+1:0)];
			hB = a6HR[c6HR-((c6HR > d6H)?d6H+1:0)];
			hC = a6NR[c6NR-((c6NR > d6N)?d6N+1:0)];
			hD = a6TR[c6TR-((c6TR > d6T)?d6T+1:0)];
			hE = a6ZAR[c6ZAR-((c6ZAR > d6ZA)?d6ZA+1:0)];
			hF = a6ZGR[c6ZGR-((c6ZGR > d6ZG)?d6ZG+1:0)];
			
			a6AR[c6AR] = hB + hC + hD + hE + hF;
			a6GR[c6GR] = (hA + hC + hF) - (hD + hE);
			a6MR[c6MR] = (hA + hB + hD) - (hE + hF);
			a6SR[c6SR] = (hA + hC + hE) - (hB + hF);
			a6YR[c6YR] = (hA + hD + hF) - (hB + hC);
			a6ZFR[c6ZFR] = (hA + hB + hE) - (hC + hD);
			
			c6AR++; c6GR++; c6MR++; c6SR++; c6YR++; c6ZFR++; 
			
			if (c6AR > d6A) c6AR = 0;
			if (c6GR > d6G) c6GR = 0;
			if (c6MR > d6M) c6MR = 0;
			if (c6SR > d6S) c6SR = 0;
			if (c6YR > d6Y) c6YR = 0;
			if (c6ZFR > d6ZF) c6ZFR = 0;
			
			hA = a6AR[c6AR-((c6AR > d6A)?d6A+1:0)];
			hB = a6GR[c6GR-((c6GR > d6G)?d6G+1:0)];
			hC = a6MR[c6MR-((c6MR > d6M)?d6M+1:0)];
			hD = a6SR[c6SR-((c6SR > d6S)?d6S+1:0)];
			hE = a6YR[c6YR-((c6YR > d6Y)?d6Y+1:0)];
			hF = a6ZFR[c6ZFR-((c6ZFR > d6ZF)?d6ZF+1:0)];
			
			f6AL = hB + hC + hD + hE + hF;
			f6BL = (hA + hC + hF) - (hD + hE);
			f6CL = (hA + hB + hD) - (hE + hF);
			f6DL = (hA + hC + hE) - (hB + hF);
			f6EL = (hA + hD + hF) - (hB + hC);
			f6FL = (hA + hB + hE) - (hC + hD);
			
			inputSampleR = (hB + hC + hD + hE + hF)*ga6n;
#endif
#ifdef SIXBYSIXMONO //instead of roomNoiseR, use reg6n
			
			a6AL[c6AL] = fma(f6BL,reg6n,inputSampleL);
			a6BL[c6BL] = fma(f6CL,reg6n,inputSampleL);
			a6CL[c6CL] = fma(f6DL,reg6n,inputSampleL);
			a6DL[c6DL] = fma(f6EL,reg6n,inputSampleL);
			a6EL[c6EL] = fma(f6FL,reg6n,inputSampleL);
			a6FL[c6FL] = fma(f6AL,reg6n,inputSampleL);
			
			a6AR[c6AL] = fma(f6BR,reg6n,inputSampleR);
			a6BR[c6BL] = fma(f6CR,reg6n,inputSampleR);
			a6CR[c6CL] = fma(f6DR,reg6n,inputSampleR);
			a6DR[c6DL] = fma(f6ER,reg6n,inputSampleR);
			a6ER[c6EL] = fma(f6FR,reg6n,inputSampleR);
			a6FR[c6FL] = fma(f6AR,reg6n,inputSampleR);
			//left verb
			
			c6AL++; c6BL++; c6CL++; c6DL++; c6EL++; c6FL++; 
			
			if (c6AL > d6A) c6AL = 0;
			if (c6BL > d6B) c6BL = 0;
			if (c6CL > d6C) c6CL = 0;
			if (c6DL > d6D) c6DL = 0;
			if (c6EL > d6E) c6EL = 0;
			if (c6FL > d6F) c6FL = 0;
			
			hA = a6AL[c6AL-((c6AL > d6A)?d6A+1:0)];
			hB = a6BL[c6BL-((c6BL > d6B)?d6B+1:0)];
			hC = a6CL[c6CL-((c6CL > d6C)?d6C+1:0)];
			hD = a6DL[c6DL-((c6DL > d6D)?d6D+1:0)];
			hE = a6EL[c6EL-((c6EL > d6E)?d6E+1:0)];
			hF = a6FL[c6FL-((c6FL > d6F)?d6F+1:0)];
			
			a6GL[c6GL] = hB + hC + hD + hE + hF;
			a6HL[c6HL] = (hA + hC + hF) - (hD + hE);
			a6IL[c6IL] = (hA + hB + hD) - (hE + hF);
			a6JL[c6JL] = (hA + hC + hE) - (hB + hF);
			a6KL[c6KL] = (hA + hD + hF) - (hB + hC);
			a6LL[c6LL] = (hA + hB + hE) - (hC + hD);
			
			hA = a6AR[c6AL-((c6AL > d6A)?d6A+1:0)];
			hB = a6BR[c6BL-((c6BL > d6B)?d6B+1:0)];
			hC = a6CR[c6CL-((c6CL > d6C)?d6C+1:0)];
			hD = a6DR[c6DL-((c6DL > d6D)?d6D+1:0)];
			hE = a6ER[c6EL-((c6EL > d6E)?d6E+1:0)];
			hF = a6FR[c6FL-((c6FL > d6F)?d6F+1:0)];
			
			a6GR[c6GL] = hB + hC + hD + hE + hF;
			a6HR[c6HL] = (hA + hC + hF) - (hD + hE);
			a6IR[c6IL] = (hA + hB + hD) - (hE + hF);
			a6JR[c6JL] = (hA + hC + hE) - (hB + hF);
			a6KR[c6KL] = (hA + hD + hF) - (hB + hC);
			a6LR[c6LL] = (hA + hB + hE) - (hC + hD);
			
			c6GL++; c6HL++; c6IL++; c6JL++; c6KL++; c6LL++; 
			
			if (c6GL > d6G) c6GL = 0;
			if (c6HL > d6H) c6HL = 0;
			if (c6IL > d6I) c6IL = 0;
			if (c6JL > d6J) c6JL = 0;
			if (c6KL > d6K) c6KL = 0;
			if (c6LL > d6L) c6LL = 0;
			
			hA = a6GL[c6GL-((c6GL > d6G)?d6G+1:0)];
			hB = a6HL[c6HL-((c6HL > d6H)?d6H+1:0)];
			hC = a6IL[c6IL-((c6IL > d6I)?d6I+1:0)];
			hD = a6JL[c6JL-((c6JL > d6J)?d6J+1:0)];
			hE = a6KL[c6KL-((c6KL > d6K)?d6K+1:0)];
			hF = a6LL[c6LL-((c6LL > d6L)?d6L+1:0)];
			
			a6ML[c6ML] = hB + hC + hD + hE + hF;
			a6NL[c6NL] = (hA + hC + hF) - (hD + hE);
			a6OL[c6OL] = (hA + hB + hD) - (hE + hF);
			a6PL[c6PL] = (hA + hC + hE) - (hB + hF);
			a6QL[c6QL] = (hA + hD + hF) - (hB + hC);
			a6RL[c6RL] = (hA + hB + hE) - (hC + hD);
			
			hA = a6GR[c6GL-((c6GL > d6G)?d6G+1:0)];
			hB = a6HR[c6HL-((c6HL > d6H)?d6H+1:0)];
			hC = a6IR[c6IL-((c6IL > d6I)?d6I+1:0)];
			hD = a6JR[c6JL-((c6JL > d6J)?d6J+1:0)];
			hE = a6KR[c6KL-((c6KL > d6K)?d6K+1:0)];
			hF = a6LR[c6LL-((c6LL > d6L)?d6L+1:0)];
			
			a6MR[c6ML] = hB + hC + hD + hE + hF;
			a6NR[c6NL] = (hA + hC + hF) - (hD + hE);
			a6OR[c6OL] = (hA + hB + hD) - (hE + hF);
			a6PR[c6PL] = (hA + hC + hE) - (hB + hF);
			a6QR[c6QL] = (hA + hD + hF) - (hB + hC);
			a6RR[c6RL] = (hA + hB + hE) - (hC + hD);
			
			
			c6ML++; c6NL++; c6OL++; c6PL++; c6QL++; c6RL++; 
			
			if (c6ML > d6M) c6ML = 0;
			if (c6NL > d6N) c6NL = 0;
			if (c6OL > d6O) c6OL = 0;
			if (c6PL > d6P) c6PL = 0;
			if (c6QL > d6Q) c6QL = 0;
			if (c6RL > d6R) c6RL = 0;
			
			hA = a6ML[c6ML-((c6ML > d6M)?d6M+1:0)];
			hB = a6NL[c6NL-((c6NL > d6N)?d6N+1:0)];
			hC = a6OL[c6OL-((c6OL > d6O)?d6O+1:0)];
			hD = a6PL[c6PL-((c6PL > d6P)?d6P+1:0)];
			hE = a6QL[c6QL-((c6QL > d6Q)?d6Q+1:0)];
			hF = a6RL[c6RL-((c6RL > d6R)?d6R+1:0)];
			
			a6SL[c6SL] = hB + hC + hD + hE + hF;
			a6TL[c6TL] = (hA + hC + hF) - (hD + hE);
			a6UL[c6UL] = (hA + hB + hD) - (hE + hF);
			a6VL[c6VL] = (hA + hC + hE) - (hB + hF);
			a6WL[c6WL] = (hA + hD + hF) - (hB + hC);
			a6XL[c6XL] = (hA + hB + hE) - (hC + hD);
			
			hA = a6MR[c6ML-((c6ML > d6M)?d6M+1:0)];
			hB = a6NR[c6NL-((c6NL > d6N)?d6N+1:0)];
			hC = a6OR[c6OL-((c6OL > d6O)?d6O+1:0)];
			hD = a6PR[c6PL-((c6PL > d6P)?d6P+1:0)];
			hE = a6QR[c6QL-((c6QL > d6Q)?d6Q+1:0)];
			hF = a6RR[c6RL-((c6RL > d6R)?d6R+1:0)];
			
			a6SR[c6SL] = hB + hC + hD + hE + hF;
			a6TR[c6TL] = (hA + hC + hF) - (hD + hE);
			a6UR[c6UL] = (hA + hB + hD) - (hE + hF);
			a6VR[c6VL] = (hA + hC + hE) - (hB + hF);
			a6WR[c6WL] = (hA + hD + hF) - (hB + hC);
			a6XR[c6XL] = (hA + hB + hE) - (hC + hD);
			
			
			c6SL++; c6TL++; c6UL++; c6VL++; c6WL++; c6XL++; 
			
			if (c6SL > d6S) c6SL = 0;
			if (c6TL > d6T) c6TL = 0;
			if (c6UL > d6U) c6UL = 0;
			if (c6VL > d6V) c6VL = 0;
			if (c6WL > d6W) c6WL = 0;
			if (c6XL > d6X) c6XL = 0;
			
			hA = a6SL[c6SL-((c6SL > d6S)?d6S+1:0)];
			hB = a6TL[c6TL-((c6TL > d6T)?d6T+1:0)];
			hC = a6UL[c6UL-((c6UL > d6U)?d6U+1:0)];
			hD = a6VL[c6VL-((c6VL > d6V)?d6V+1:0)];
			hE = a6WL[c6WL-((c6WL > d6W)?d6W+1:0)];
			hF = a6XL[c6XL-((c6XL > d6X)?d6X+1:0)];
			
			a6YL[c6YL] = hB + hC + hD + hE + hF;
			a6ZAL[c6ZAL] = (hA + hC + hF) - (hD + hE);
			a6ZBL[c6ZBL] = (hA + hB + hD) - (hE + hF);
			a6ZCL[c6ZCL] = (hA + hC + hE) - (hB + hF);
			a6ZDL[c6ZDL] = (hA + hD + hF) - (hB + hC);
			a6ZEL[c6ZEL] = (hA + hB + hE) - (hC + hD);
			
			hA = a6SR[c6SL-((c6SL > d6S)?d6S+1:0)];
			hB = a6TR[c6TL-((c6TL > d6T)?d6T+1:0)];
			hC = a6UR[c6UL-((c6UL > d6U)?d6U+1:0)];
			hD = a6VR[c6VL-((c6VL > d6V)?d6V+1:0)];
			hE = a6WR[c6WL-((c6WL > d6W)?d6W+1:0)];
			hF = a6XR[c6XL-((c6XL > d6X)?d6X+1:0)];
			
			a6YR[c6YL] = hB + hC + hD + hE + hF;
			a6ZAR[c6ZAL] = (hA + hC + hF) - (hD + hE);
			a6ZBR[c6ZBL] = (hA + hB + hD) - (hE + hF);
			a6ZCR[c6ZCL] = (hA + hC + hE) - (hB + hF);
			a6ZDR[c6ZDL] = (hA + hD + hF) - (hB + hC);
			a6ZER[c6ZEL] = (hA + hB + hE) - (hC + hD);
			
			c6YL++; c6ZAL++; c6ZBL++; c6ZCL++; c6ZDL++; c6ZEL++; 
			
			if (c6YL > d6Y) c6YL = 0;
			if (c6ZAL > d6ZA) c6ZAL = 0;
			if (c6ZBL > d6ZB) c6ZBL = 0;
			if (c6ZCL > d6ZC) c6ZCL = 0;
			if (c6ZDL > d6ZD) c6ZDL = 0;
			if (c6ZEL > d6ZE) c6ZEL = 0;
			
			hA = a6YL[c6YL-((c6YL > d6Y)?d6Y+1:0)];
			hB = a6ZAL[c6ZAL-((c6ZAL > d6ZA)?d6ZA+1:0)];
			hC = a6ZBL[c6ZBL-((c6ZBL > d6ZB)?d6ZB+1:0)];
			hD = a6ZCL[c6ZCL-((c6ZCL > d6ZC)?d6ZC+1:0)];
			hE = a6ZDL[c6ZDL-((c6ZDL > d6ZD)?d6ZD+1:0)];
			hF = a6ZEL[c6ZEL-((c6ZEL > d6ZE)?d6ZE+1:0)];
			
			a6ZFL[c6ZFL] = hB + hC + hD + hE + hF;
			a6ZGL[c6ZGL] = (hA + hC + hF) - (hD + hE);
			a6ZHL[c6ZHL] = (hA + hB + hD) - (hE + hF);
			a6ZIL[c6ZIL] = (hA + hC + hE) - (hB + hF);
			a6ZJL[c6ZJL] = (hA + hD + hF) - (hB + hC);
			a6ZKL[c6ZKL] = (hA + hB + hE) - (hC + hD);
			
			hA = a6YR[c6YL-((c6YL > d6Y)?d6Y+1:0)];
			hB = a6ZAR[c6ZAL-((c6ZAL > d6ZA)?d6ZA+1:0)];
			hC = a6ZBR[c6ZBL-((c6ZBL > d6ZB)?d6ZB+1:0)];
			hD = a6ZCR[c6ZCL-((c6ZCL > d6ZC)?d6ZC+1:0)];
			hE = a6ZDR[c6ZDL-((c6ZDL > d6ZD)?d6ZD+1:0)];
			hF = a6ZER[c6ZEL-((c6ZEL > d6ZE)?d6ZE+1:0)];
			
			a6ZFR[c6ZFL] = hB + hC + hD + hE + hF;
			a6ZGR[c6ZGL] = (hA + hC + hF) - (hD + hE);
			a6ZHR[c6ZHL] = (hA + hB + hD) - (hE + hF);
			a6ZIR[c6ZIL] = (hA + hC + hE) - (hB + hF);
			a6ZJR[c6ZJL] = (hA + hD + hF) - (hB + hC);
			a6ZKR[c6ZKL] = (hA + hB + hE) - (hC + hD);
			
			c6ZFL++; c6ZGL++; c6ZHL++; c6ZIL++; c6ZJL++; c6ZKL++; 
			
			if (c6ZFL > d6ZF) c6ZFL = 0;
			if (c6ZGL > d6ZG) c6ZGL = 0;
			if (c6ZHL > d6ZH) c6ZHL = 0;
			if (c6ZIL > d6ZI) c6ZIL = 0;
			if (c6ZJL > d6ZJ) c6ZJL = 0;
			if (c6ZKL > d6ZK) c6ZKL = 0;
			
			hA = a6ZFL[c6ZFL-((c6ZFL > d6ZF)?d6ZF+1:0)];
			hB = a6ZGL[c6ZGL-((c6ZGL > d6ZG)?d6ZG+1:0)];
			hC = a6ZHL[c6ZHL-((c6ZHL > d6ZH)?d6ZH+1:0)];
			hD = a6ZIL[c6ZIL-((c6ZIL > d6ZI)?d6ZI+1:0)];
			hE = a6ZJL[c6ZJL-((c6ZJL > d6ZJ)?d6ZJ+1:0)];
			hF = a6ZKL[c6ZKL-((c6ZKL > d6ZK)?d6ZK+1:0)];
			
			f6AL = hB + hC + hD + hE + hF;
			f6BL = (hA + hC + hF) - (hD + hE);
			f6CL = (hA + hB + hD) - (hE + hF);
			f6DL = (hA + hC + hE) - (hB + hF);
			f6EL = (hA + hD + hF) - (hB + hC);
			f6FL = (hA + hB + hE) - (hC + hD);
			
			inputSampleL = (hB + hC + hD + hE + hF)*ga6n;
			
			hA = a6ZFR[c6ZFL-((c6ZFL > d6ZF)?d6ZF+1:0)];
			hB = a6ZGR[c6ZGL-((c6ZGL > d6ZG)?d6ZG+1:0)];
			hC = a6ZHR[c6ZHL-((c6ZHL > d6ZH)?d6ZH+1:0)];
			hD = a6ZIR[c6ZIL-((c6ZIL > d6ZI)?d6ZI+1:0)];
			hE = a6ZJR[c6ZJL-((c6ZJL > d6ZJ)?d6ZJ+1:0)];
			hF = a6ZKR[c6ZKL-((c6ZKL > d6ZK)?d6ZK+1:0)];
			
			f6AR = hB + hC + hD + hE + hF;
			f6BR = (hA + hC + hF) - (hD + hE);
			f6CR = (hA + hB + hD) - (hE + hF);
			f6DR = (hA + hC + hE) - (hB + hF);
			f6ER = (hA + hD + hF) - (hB + hC);
			f6FR = (hA + hB + hE) - (hC + hD);
			
			inputSampleR = (hB + hC + hD + hE + hF)*ga6n;
#endif
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		double X = bez[bez_cycle];
		inputSampleL = (bez[bez_BL]+(bez[bez_CL]*(1.0-X)*(1.0-X))+(bez[bez_BL]*2.0*(1.0-X)*X)+(bez[bez_AL]*X*X))*-0.375;
		inputSampleR = (bez[bez_BR]+(bez[bez_CR]*(1.0-X)*(1.0-X))+(bez[bez_BR]*2.0*(1.0-X)*X)+(bez[bez_AR]*X*X))*-0.375;
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void VerbSevens::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 48000.0;
	overallscale *= getSampleRate();
	
#ifdef THREEBYTHREE
	double reg3n = 0.03125;
	double ga3n = (reg3n+reg3n+0.015625)*0.45;
	reg3n = (reg3n+reg3n+0.015625)*0.38;
#endif
#ifdef FOURBYFOUR
	double reg4n = 0.0625;
	double ga4n = (reg4n+reg4n+0.03125)*0.22;
	reg4n = (reg4n+reg4n+0.03125)*0.345;
#endif
#ifdef FIVEBYFIVE
	double reg5n = 0.000244140625;
	double ga5n = (reg5n+reg5n+0.00048828125)*0.39;
	reg5n = (reg5n+reg5n+0.00048828125)*0.28;
#endif
#ifdef SIXBYSIX
	double reg6n = 0.0078125;
	double ga6n = (reg6n+reg6n+0.00390625)*0.32;
	reg6n = (reg6n+reg6n+0.00390625)*0.34;
#endif
#ifdef SIXBYSIXMONO
	double reg6n = 0.0078125;
	double ga6n = (reg6n+reg6n+0.00390625)*0.32;
	reg6n = (reg6n+reg6n+0.00390625)*0.34;
#endif
	//adding more digits to that fraction gets closer to infinite f6. 0.333 is already long
	
	double downRez = (overallscale/(overallscale+0.99999))/overallscale;
	downRez = fmin(fmax(downRez,0.0005),1.0/overallscale);
	
	int bezFraction = (int)(1.0/downRez);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	downRez = 0.99999999 / bezFraction;
	bezTrim = 1.0-(downRez*bezTrim);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		bez[bez_cycle] += downRez;
		bez[bez_SampL] += (inputSampleL * downRez);
		bez[bez_SampR] += (inputSampleR * downRez);
		if (bez[bez_cycle] > bezTrim) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			
			inputSampleL = bez[bez_SampL];
			inputSampleR = bez[bez_SampR];
			
#ifdef THREEBYTHREE
			a3AL[c3AL] = inputSampleL;// + (f3AL * reg3n);
			a3BL[c3BL] = inputSampleL;// + (f3BL * reg3n);
			a3CL[c3CL] = inputSampleL;// + (f3CL * reg3n);
			
			a3CR[c3CR] = inputSampleR;// + (f3CR * reg3n);
			a3FR[c3FR] = inputSampleR;// + (f3FR * reg3n);
			a3IR[c3IR] = inputSampleR;// + (f3IR * reg3n);
			
			c3AL++; if (c3AL < 0 || c3AL > d3A) c3AL = 0;
			c3BL++; if (c3BL < 0 || c3BL > d3B) c3BL = 0;
			c3CL++; if (c3CL < 0 || c3CL > d3C) c3CL = 0;
			c3CR++; if (c3CR < 0 || c3CR > d3C) c3CR = 0;
			c3FR++; if (c3FR < 0 || c3FR > d3F) c3FR = 0;
			c3IR++; if (c3IR < 0 || c3IR > d3I) c3IR = 0;
			
			double o3AL = a3AL[c3AL-((c3AL > d3A)?c3AL+1:0)];
			double o3BL = a3BL[c3BL-((c3BL > d3B)?c3BL+1:0)];
			double o3CL = a3CL[c3CL-((c3CL > d3C)?c3CL+1:0)];
			double o3CR = a3CR[c3CR-((c3CR > d3C)?c3CR+1:0)];
			double o3FR = a3FR[c3FR-((c3FR > d3F)?c3FR+1:0)];
			double o3IR = a3IR[c3IR-((c3IR > d3I)?c3IR+1:0)];
			
			a3DL[c3DL] = (((o3BL + o3CL) * -2.0) + o3AL);
			a3EL[c3EL] = (((o3AL + o3CL) * -2.0) + o3BL);
			a3FL[c3FL] = (((o3AL + o3BL) * -2.0) + o3CL);
			a3BR[c3BR] = (((o3FR + o3IR) * -2.0) + o3CR);
			a3ER[c3ER] = (((o3CR + o3IR) * -2.0) + o3FR);
			a3HR[c3HR] = (((o3CR + o3FR) * -2.0) + o3IR);
			
			c3DL++; if (c3DL < 0 || c3DL > d3D) c3DL = 0;
			c3EL++; if (c3EL < 0 || c3EL > d3E) c3EL = 0;
			c3FL++; if (c3FL < 0 || c3FL > d3F) c3FL = 0;
			c3BR++; if (c3BR < 0 || c3BR > d3B) c3BR = 0;
			c3ER++; if (c3ER < 0 || c3ER > d3E) c3ER = 0;
			c3HR++; if (c3HR < 0 || c3HR > d3H) c3HR = 0;
			
			double o3DL = a3DL[c3DL-((c3DL > d3D)?c3DL+1:0)];
			double o3EL = a3EL[c3EL-((c3EL > d3E)?c3EL+1:0)];
			double o3FL = a3FL[c3FL-((c3FL > d3F)?c3FL+1:0)];
			double o3BR = a3BR[c3BR-((c3BR > d3B)?c3BR+1:0)];
			double o3ER = a3ER[c3ER-((c3ER > d3E)?c3ER+1:0)];
			double o3HR = a3HR[c3HR-((c3HR > d3H)?c3HR+1:0)];
			
			a3GL[c3GL] = (((o3EL + o3FL) * -2.0) + o3DL);
			a3HL[c3HL] = (((o3DL + o3FL) * -2.0) + o3EL);
			a3IL[c3IL] = (((o3DL + o3EL) * -2.0) + o3FL);
			a3AR[c3AR] = (((o3ER + o3HR) * -2.0) + o3BR);
			a3DR[c3DR] = (((o3BR + o3HR) * -2.0) + o3ER);
			a3GR[c3GR] = (((o3BR + o3ER) * -2.0) + o3HR);
			
			c3GL++; if (c3GL < 0 || c3GL > d3G) c3GL = 0;
			c3HL++; if (c3HL < 0 || c3HL > d3H) c3HL = 0;
			c3IL++; if (c3IL < 0 || c3IL > d3I) c3IL = 0;
			c3AR++; if (c3AR < 0 || c3AR > d3A) c3AR = 0;
			c3DR++; if (c3DR < 0 || c3DR > d3D) c3DR = 0;
			c3GR++; if (c3GR < 0 || c3GR > d3G) c3GR = 0;
			
			double o3GL = a3GL[c3GL-((c3GL > d3G)?c3GL+1:0)];
			double o3HL = a3HL[c3HL-((c3HL > d3H)?c3HL+1:0)];
			double o3IL = a3IL[c3IL-((c3IL > d3I)?c3IL+1:0)];
			double o3AR = a3AR[c3AR-((c3AR > d3A)?c3AR+1:0)];
			double o3DR = a3DR[c3DR-((c3DR > d3D)?c3DR+1:0)];
			double o3GR = a3GR[c3GR-((c3GR > d3G)?c3GR+1:0)];
			
			f3AL = (((o3GR + o3HR) * -2.0) + o3IR);				
			f3BL = (((o3GR + o3HR) * -2.0) + o3IR);
			f3CL = (((o3GR + o3HR) * -2.0) + o3IR);
			
			f3CR = (((o3AL + o3DL) * -2.0) + o3GL);
			f3FR = (((o3AL + o3DL) * -2.0) + o3GL);
			f3IR = (((o3AL + o3DL) * -2.0) + o3GL);
			
			double inputSampleL = (o3GL + o3HL + o3IL)*ga3n;
			double inputSampleR = (o3AR + o3DR + o3GR)*ga3n;
			
			f3AL = (f3AL+f3AL+f3AL+fabs(avg3L))*0.25; avg3L = f3AL;
			f3CR = (f3CR+f3CR+f3CR+fabs(avg3R))*0.25; avg3R = f3CR;
			//manipulating deep reverb tail for realism			
			
#endif
#ifdef FOURBYFOUR
			a4AL[c4AL] = fma(f4AL,reg4n,inputSampleL);
			a4BL[c4BL] = fma(f4BL,reg4n,inputSampleL);
			a4CL[c4CL] = fma(f4CL,reg4n,inputSampleL);
			a4DL[c4DL] = fma(f4DL,reg4n,inputSampleL);
			a4DR[c4DR] = fma(f4DR,reg4n,inputSampleR);
			a4HR[c4HR] = fma(f4HR,reg4n,inputSampleR);
			a4LR[c4LR] = fma(f4LR,reg4n,inputSampleR);
			a4PR[c4PR] = fma(f4PR,reg4n,inputSampleR);
			c4AL++; c4BL++; c4CL++; c4DL++;
			c4DR++; c4HR++; c4LR++; c4PR++;
			if (c4AL > d4A) c4AL = 0;
			if (c4BL > d4B) c4BL = 0;
			if (c4CL > d4C) c4CL = 0;
			if (c4DL > d4D) c4DL = 0;
			if (c4DR > d4D) c4DR = 0;
			if (c4HR > d4H) c4HR = 0;
			if (c4LR > d4L) c4LR = 0;
			if (c4PR > d4P) c4PR = 0;
			hAL = a4AL[c4AL]; hBL = a4BL[c4BL]; hCL = a4CL[c4CL]; hDL = a4DL[c4DL];
			hAR = a4DR[c4DR]; hBR = a4HR[c4HR]; hCR = a4LR[c4LR]; hDR = a4PR[c4PR];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			a4EL[c4EL] = fma(hAL,2.0f,hEL);
			a4FL[c4FL] = fma(hBL,2.0f,hEL);
			a4GL[c4GL] = fma(hCL,2.0f,hEL);
			a4HL[c4HL] = fma(hDL,2.0f,hEL);
			a4CR[c4CR] = fma(hAR,2.0f,hER);
			a4GR[c4GR] = fma(hBR,2.0f,hER);
			a4KR[c4KR] = fma(hCR,2.0f,hER);
			a4OR[c4OR] = fma(hDR,2.0f,hER);
			c4EL++; c4FL++; c4GL++; c4HL++;
			c4CR++; c4GR++; c4KR++; c4OR++;
			if (c4EL > d4E) c4EL = 0;
			if (c4FL > d4F) c4FL = 0;
			if (c4GL > d4G) c4GL = 0;
			if (c4HL > d4H) c4HL = 0;
			if (c4CR > d4C) c4CR = 0;
			if (c4GR > d4G) c4GR = 0;
			if (c4KR > d4K) c4KR = 0;
			if (c4OR > d4O) c4OR = 0;
			hAL = a4EL[c4EL]; hBL = a4FL[c4FL]; hCL = a4GL[c4GL]; hDL = a4HL[c4HL];
			hAR = a4CR[c4CR]; hBR = a4GR[c4GR]; hCR = a4KR[c4KR]; hDR = a4OR[c4OR];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			a4IL[c4IL] = fma(hAL,2.0f,hEL);
			a4JL[c4JL] = fma(hBL,2.0f,hEL);
			a4KL[c4KL] = fma(hCL,2.0f,hEL);
			a4LL[c4LL] = fma(hDL,2.0f,hEL);
			a4BR[c4BR] = fma(hAR,2.0f,hER);
			a4FR[c4FR] = fma(hBR,2.0f,hER);
			a4JR[c4JR] = fma(hCR,2.0f,hER);
			a4NR[c4NR] = fma(hDR,2.0f,hER);
			c4IL++; c4JL++; c4KL++; c4LL++;
			c4BR++; c4FR++; c4JR++; c4NR++;
			if (c4IL > d4I) c4IL = 0;
			if (c4JL > d4J) c4JL = 0;
			if (c4KL > d4K) c4KL = 0;
			if (c4LL > d4L) c4LL = 0;
			if (c4BR > d4B) c4BR = 0;
			if (c4FR > d4F) c4FR = 0;
			if (c4JR > d4J) c4JR = 0;
			if (c4NR > d4N) c4NR = 0;
			hAL = a4IL[c4IL]; hBL = a4JL[c4JL]; hCL = a4KL[c4KL]; hDL = a4LL[c4LL];
			hAR = a4BR[c4BR]; hBR = a4FR[c4FR]; hCR = a4JR[c4JR]; hDR = a4NR[c4NR];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			a4ML[c4ML] = fma(hAL,2.0f,hEL);
			a4NL[c4NL] = fma(hBL,2.0f,hEL);
			a4OL[c4OL] = fma(hCL,2.0f,hEL);
			a4PL[c4PL] = fma(hDL,2.0f,hEL);
			a4AR[c4AR] = fma(hAR,2.0f,hER);
			a4ER[c4ER] = fma(hBR,2.0f,hER);
			a4IR[c4IR] = fma(hCR,2.0f,hER);
			a4MR[c4MR] = fma(hDR,2.0f,hER);
			c4ML++; c4NL++; c4OL++; c4PL++;
			c4AR++; c4ER++; c4IR++; c4MR++;
			if (c4ML > d4M) c4ML = 0;
			if (c4NL > d4N) c4NL = 0;
			if (c4OL > d4O) c4OL = 0;
			if (c4PL > d4P) c4PL = 0;
			if (c4AR > d4A) c4AR = 0;
			if (c4ER > d4E) c4ER = 0;
			if (c4IR > d4I) c4IR = 0;
			if (c4MR > d4M) c4MR = 0;
			hAL = a4ML[c4ML]; hBL = a4NL[c4NL]; hCL = a4OL[c4OL]; hDL = a4PL[c4PL];
			hAR = a4AR[c4AR]; hBR = a4ER[c4ER]; hCR = a4IR[c4IR]; hDR = a4MR[c4MR];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			f4AL = fma(hAR,2.0f,hER);				
			f4BL = fma(hBR,2.0f,hER);
			f4CL = fma(hCR,2.0f,hER);
			f4DL = fma(hDR,2.0f,hER);
			f4DR = fma(hAL,2.0f,hEL);				
			f4HR = fma(hBL,2.0f,hEL);
			f4LR = fma(hCL,2.0f,hEL);
			f4PR = fma(hDL,2.0f,hEL);
			inputSampleL = (hAL + hBL + hCL + hDL)*ga4n;
			inputSampleR = (hAR + hBR + hCR + hDR)*ga4n;
#endif
#ifdef FIVEBYFIVE
			a5AL[c5AL] = inputSampleL + (f5AL * reg5n);
			a5BL[c5BL] = inputSampleL + (f5BL * reg5n);
			a5CL[c5CL] = inputSampleL + (f5CL * reg5n);
			a5DL[c5DL] = inputSampleL + (f5DL * reg5n);
			a5EL[c5EL] = inputSampleL + (f5EL * reg5n);
			
			c5AL++; if (c5AL < 0 || c5AL > d5A) c5AL = 0;
			c5BL++; if (c5BL < 0 || c5BL > d5B) c5BL = 0;
			c5CL++; if (c5CL < 0 || c5CL > d5C) c5CL = 0;
			c5DL++; if (c5DL < 0 || c5DL > d5D) c5DL = 0;
			c5EL++; if (c5EL < 0 || c5EL > d5E) c5EL = 0;
			
			double o5AL = a5AL[c5AL-((c5AL > d5A)?d5A+1:0)];
			double o5BL = a5BL[c5BL-((c5BL > d5B)?d5B+1:0)];
			double o5CL = a5CL[c5CL-((c5CL > d5C)?d5C+1:0)];
			double o5DL = a5DL[c5DL-((c5DL > d5D)?d5D+1:0)];
			double o5EL = a5EL[c5EL-((c5EL > d5E)?d5E+1:0)];
			
			a5ER[c5ER] = inputSampleR + (f5ER * reg5n);
			a5JR[c5JR] = inputSampleR + (f5JR * reg5n);
			a5OR[c5OR] = inputSampleR + (f5OR * reg5n);
			a5TR[c5TR] = inputSampleR + (f5TR * reg5n);
			a5YR[c5YR] = inputSampleR + (f5YR * reg5n);
			
			c5ER++; if (c5ER < 0 || c5ER > d5E) c5ER = 0;
			c5JR++; if (c5JR < 0 || c5JR > d5J) c5JR = 0;
			c5OR++; if (c5OR < 0 || c5OR > d5O) c5OR = 0;
			c5TR++; if (c5TR < 0 || c5TR > d5T) c5TR = 0;
			c5YR++; if (c5YR < 0 || c5YR > d5Y) c5YR = 0;
			
			double o5ER = a5ER[c5ER-((c5ER > d5E)?d5E+1:0)];
			double o5JR = a5JR[c5JR-((c5JR > d5J)?d5J+1:0)];
			double o5OR = a5OR[c5OR-((c5OR > d5O)?d5O+1:0)];
			double o5TR = a5TR[c5TR-((c5TR > d5T)?d5T+1:0)];
			double o5YR = a5YR[c5YR-((c5YR > d5Y)?d5Y+1:0)];
			
			//-------- one
			
			a5FL[c5FL] = ((o5AL*3.0) - ((o5BL + o5CL + o5DL + o5EL)*2.0));
			a5GL[c5GL] = ((o5BL*3.0) - ((o5AL + o5CL + o5DL + o5EL)*2.0));
			a5HL[c5HL] = ((o5CL*3.0) - ((o5AL + o5BL + o5DL + o5EL)*2.0));
			a5IL[c5IL] = ((o5DL*3.0) - ((o5AL + o5BL + o5CL + o5EL)*2.0));
			a5JL[c5JL] = ((o5EL*3.0) - ((o5AL + o5BL + o5CL + o5DL)*2.0));
			
			c5FL++; if (c5FL < 0 || c5FL > d5F) c5FL = 0;
			c5GL++; if (c5GL < 0 || c5GL > d5G) c5GL = 0;
			c5HL++; if (c5HL < 0 || c5HL > d5H) c5HL = 0;
			c5IL++; if (c5IL < 0 || c5IL > d5I) c5IL = 0;
			c5JL++; if (c5JL < 0 || c5JL > d5J) c5JL = 0;
			
			double o5FL = a5FL[c5FL-((c5FL > d5F)?d5F+1:0)];
			double o5GL = a5GL[c5GL-((c5GL > d5G)?d5G+1:0)];
			double o5HL = a5HL[c5HL-((c5HL > d5H)?d5H+1:0)];
			double o5IL = a5IL[c5IL-((c5IL > d5I)?d5I+1:0)];
			double o5JL = a5JL[c5JL-((c5JL > d5J)?d5J+1:0)];
			
			a5DR[c5DR] = ((o5ER*3.0) - ((o5JR + o5OR + o5TR + o5YR)*2.0));
			a5IR[c5IR] = ((o5JR*3.0) - ((o5ER + o5OR + o5TR + o5YR)*2.0));
			a5NR[c5NR] = ((o5OR*3.0) - ((o5ER + o5JR + o5TR + o5YR)*2.0));
			a5SR[c5SR] = ((o5TR*3.0) - ((o5ER + o5JR + o5OR + o5YR)*2.0));
			a5XR[c5XR] = ((o5YR*3.0) - ((o5ER + o5JR + o5OR + o5TR)*2.0));
			
			c5DR++; if (c5DR < 0 || c5DR > d5D) c5DR = 0;
			c5IR++; if (c5IR < 0 || c5IR > d5I) c5IR = 0;
			c5NR++; if (c5NR < 0 || c5NR > d5N) c5NR = 0;
			c5SR++; if (c5SR < 0 || c5SR > d5S) c5SR = 0;
			c5XR++; if (c5XR < 0 || c5XR > d5X) c5XR = 0;
			
			double o5DR = a5DR[c5DR-((c5DR > d5D)?d5D+1:0)];
			double o5IR = a5IR[c5IR-((c5IR > d5I)?d5I+1:0)];
			double o5NR = a5NR[c5NR-((c5NR > d5N)?d5N+1:0)];
			double o5SR = a5SR[c5SR-((c5SR > d5S)?d5S+1:0)];
			double o5XR = a5XR[c5XR-((c5XR > d5X)?d5X+1:0)];
			
			//-------- two
			
			a5KL[c5KL] = ((o5FL*3.0) - ((o5GL + o5HL + o5IL + o5JL)*2.0));
			a5LL[c5LL] = ((o5GL*3.0) - ((o5FL + o5HL + o5IL + o5JL)*2.0));
			a5ML[c5ML] = ((o5HL*3.0) - ((o5FL + o5GL + o5IL + o5JL)*2.0));
			a5NL[c5NL] = ((o5IL*3.0) - ((o5FL + o5GL + o5HL + o5JL)*2.0));
			a5OL[c5OL] = ((o5JL*3.0) - ((o5FL + o5GL + o5HL + o5IL)*2.0));
			
			c5KL++; if (c5KL < 0 || c5KL > d5K) c5KL = 0;
			c5LL++; if (c5LL < 0 || c5LL > d5L) c5LL = 0;
			c5ML++; if (c5ML < 0 || c5ML > d5M) c5ML = 0;
			c5NL++; if (c5NL < 0 || c5NL > d5N) c5NL = 0;
			c5OL++; if (c5OL < 0 || c5OL > d5O) c5OL = 0;
			
			double o5KL = a5KL[c5KL-((c5KL > d5K)?d5K+1:0)];
			double o5LL = a5LL[c5LL-((c5LL > d5L)?d5L+1:0)];
			double o5ML = a5ML[c5ML-((c5ML > d5M)?d5M+1:0)];
			double o5NL = a5NL[c5NL-((c5NL > d5N)?d5N+1:0)];
			double o5OL = a5OL[c5OL-((c5OL > d5O)?d5O+1:0)];
			
			a5CR[c5CR] = ((o5DR*3.0) - ((o5IR + o5NR + o5SR + o5XR)*2.0));
			a5HR[c5HR] = ((o5IR*3.0) - ((o5DR + o5NR + o5SR + o5XR)*2.0));
			a5MR[c5MR] = ((o5NR*3.0) - ((o5DR + o5IR + o5SR + o5XR)*2.0));
			a5RR[c5RR] = ((o5SR*3.0) - ((o5DR + o5IR + o5NR + o5XR)*2.0));
			a5WR[c5WR] = ((o5XR*3.0) - ((o5DR + o5IR + o5NR + o5SR)*2.0));
			
			c5CR++; if (c5CR < 0 || c5CR > d5C) c5CR = 0;
			c5HR++; if (c5HR < 0 || c5HR > d5H) c5HR = 0;
			c5MR++; if (c5MR < 0 || c5MR > d5M) c5MR = 0;
			c5RR++; if (c5RR < 0 || c5RR > d5R) c5RR = 0;
			c5WR++; if (c5WR < 0 || c5WR > d5W) c5WR = 0;
			
			double o5CR = a5CR[c5CR-((c5CR > d5C)?d5C+1:0)];
			double o5HR = a5HR[c5HR-((c5HR > d5H)?d5H+1:0)];
			double o5MR = a5MR[c5MR-((c5MR > d5M)?d5M+1:0)];
			double o5RR = a5RR[c5RR-((c5RR > d5R)?d5R+1:0)];
			double o5WR = a5WR[c5WR-((c5WR > d5W)?d5W+1:0)];
			
			//-------- three
			
			a5PL[c5PL] = ((o5KL*3.0) - ((o5LL + o5ML + o5NL + o5OL)*2.0));
			a5QL[c5QL] = ((o5LL*3.0) - ((o5KL + o5ML + o5NL + o5OL)*2.0));
			a5RL[c5RL] = ((o5ML*3.0) - ((o5KL + o5LL + o5NL + o5OL)*2.0));
			a5SL[c5SL] = ((o5NL*3.0) - ((o5KL + o5LL + o5ML + o5OL)*2.0));
			a5TL[c5TL] = ((o5OL*3.0) - ((o5KL + o5LL + o5ML + o5NL)*2.0));
			
			c5PL++; if (c5PL < 0 || c5PL > d5P) c5PL = 0;
			c5QL++; if (c5QL < 0 || c5QL > d5Q) c5QL = 0;
			c5RL++; if (c5RL < 0 || c5RL > d5R) c5RL = 0;
			c5SL++; if (c5SL < 0 || c5SL > d5S) c5SL = 0;
			c5TL++; if (c5TL < 0 || c5TL > d5T) c5TL = 0;
			
			double o5PL = a5PL[c5PL-((c5PL > d5P)?d5P+1:0)];
			double o5QL = a5QL[c5QL-((c5QL > d5Q)?d5Q+1:0)];
			double o5RL = a5RL[c5RL-((c5RL > d5R)?d5R+1:0)];
			double o5SL = a5SL[c5SL-((c5SL > d5S)?d5S+1:0)];
			double o5TL = a5TL[c5TL-((c5TL > d5T)?d5T+1:0)];
			
			a5BR[c5BR] = ((o5CR*3.0) - ((o5HR + o5MR + o5RR + o5WR)*2.0));
			a5GR[c5GR] = ((o5HR*3.0) - ((o5CR + o5MR + o5RR + o5WR)*2.0));
			a5LR[c5LR] = ((o5MR*3.0) - ((o5CR + o5HR + o5RR + o5WR)*2.0));
			a5QR[c5QR] = ((o5RR*3.0) - ((o5CR + o5HR + o5MR + o5WR)*2.0));
			a5VR[c5VR] = ((o5WR*3.0) - ((o5CR + o5HR + o5MR + o5RR)*2.0));
			
			c5BR++; if (c5BR < 0 || c5BR > d5B) c5BR = 0;
			c5GR++; if (c5GR < 0 || c5GR > d5G) c5GR = 0;
			c5LR++; if (c5LR < 0 || c5LR > d5L) c5LR = 0;
			c5QR++; if (c5QR < 0 || c5QR > d5Q) c5QR = 0;
			c5VR++; if (c5VR < 0 || c5VR > d5V) c5VR = 0;
			
			double o5BR = a5BR[c5BR-((c5BR > d5B)?d5B+1:0)];
			double o5GR = a5GR[c5GR-((c5GR > d5G)?d5G+1:0)];
			double o5LR = a5LR[c5LR-((c5LR > d5L)?d5L+1:0)];
			double o5QR = a5QR[c5QR-((c5QR > d5Q)?d5Q+1:0)];
			double o5VR = a5VR[c5VR-((c5VR > d5V)?d5V+1:0)];
			
			//-------- four
			
			a5UL[c5UL] = ((o5PL*3.0) - ((o5QL + o5RL + o5SL + o5TL)*2.0));
			a5VL[c5VL] = ((o5QL*3.0) - ((o5PL + o5RL + o5SL + o5TL)*2.0));
			a5WL[c5WL] = ((o5RL*3.0) - ((o5PL + o5QL + o5SL + o5TL)*2.0));
			a5XL[c5XL] = ((o5SL*3.0) - ((o5PL + o5QL + o5RL + o5TL)*2.0));
			a5YL[c5YL] = ((o5TL*3.0) - ((o5PL + o5QL + o5RL + o5SL)*2.0));
			
			c5UL++; if (c5UL < 0 || c5UL > d5U) c5UL = 0;
			c5VL++; if (c5VL < 0 || c5VL > d5V) c5VL = 0;
			c5WL++; if (c5WL < 0 || c5WL > d5W) c5WL = 0;
			c5XL++; if (c5XL < 0 || c5XL > d5X) c5XL = 0;
			c5YL++; if (c5YL < 0 || c5YL > d5Y) c5YL = 0;
			
			double o5UL = a5UL[c5UL-((c5UL > d5U)?d5U+1:0)];
			double o5VL = a5VL[c5VL-((c5VL > d5V)?d5V+1:0)];
			double o5WL = a5WL[c5WL-((c5WL > d5W)?d5W+1:0)];
			double o5XL = a5XL[c5XL-((c5XL > d5X)?d5X+1:0)];
			double o5YL = a5YL[c5YL-((c5YL > d5Y)?d5Y+1:0)];
			
			a5AR[c5AR] = ((o5BR*3.0) - ((o5GR + o5LR + o5QR + o5VR)*2.0));
			a5FR[c5FR] = ((o5GR*3.0) - ((o5BR + o5LR + o5QR + o5VR)*2.0));
			a5KR[c5KR] = ((o5LR*3.0) - ((o5BR + o5GR + o5QR + o5VR)*2.0));
			a5PR[c5PR] = ((o5QR*3.0) - ((o5BR + o5GR + o5LR + o5VR)*2.0));
			a5UR[c5UR] = ((o5VR*3.0) - ((o5BR + o5GR + o5LR + o5QR)*2.0));
			
			c5AR++; if (c5AR < 0 || c5AR > d5A) c5AR = 0;
			c5FR++; if (c5FR < 0 || c5FR > d5F) c5FR = 0;
			c5KR++; if (c5KR < 0 || c5KR > d5K) c5KR = 0;
			c5PR++; if (c5PR < 0 || c5PR > d5P) c5PR = 0;
			c5UR++; if (c5UR < 0 || c5UR > d5U) c5UR = 0;
			
			double o5AR = a5AR[c5AR-((c5AR > d5A)?d5A+1:0)];
			double o5FR = a5FR[c5FR-((c5FR > d5F)?d5F+1:0)];
			double o5KR = a5KR[c5KR-((c5KR > d5K)?d5K+1:0)];
			double o5PR = a5PR[c5PR-((c5PR > d5P)?d5P+1:0)];
			double o5UR = a5UR[c5UR-((c5UR > d5U)?d5U+1:0)];
			
			//-------- five
			
			f5AL = ((o5AR*3.0) - ((o5FR + o5KR + o5PR + o5UR)*2.0));
			f5BL = ((o5FR*3.0) - ((o5AR + o5KR + o5PR + o5UR)*2.0));
			f5CL = ((o5KR*3.0) - ((o5AR + o5FR + o5PR + o5UR)*2.0));
			f5DL = ((o5PR*3.0) - ((o5AR + o5FR + o5KR + o5UR)*2.0));
			f5EL = ((o5UR*3.0) - ((o5AR + o5FR + o5KR + o5PR)*2.0));
			
			f5ER = ((o5UL*3.0) - ((o5VL + o5WL + o5XL + o5YL)*2.0));
			f5JR = ((o5VL*3.0) - ((o5UL + o5WL + o5XL + o5YL)*2.0));
			f5OR = ((o5WL*3.0) - ((o5UL + o5VL + o5XL + o5YL)*2.0));
			f5TR = ((o5XL*3.0) - ((o5UL + o5VL + o5WL + o5YL)*2.0));
			f5YR = ((o5YL*3.0) - ((o5UL + o5VL + o5WL + o5XL)*2.0));
			
			inputSampleL = (o5UL + o5VL + o5WL + o5XL + o5YL)*ga5n;
			inputSampleR = (o5AR + o5FR + o5KR + o5PR + o5UR)*ga5n;
			
			f5AL = (f5AL+f5AL+f5AL+fabs(avg5L))*0.25; avg5L = f5AL;
			f5ER = (f5ER+f5ER+f5ER+fabs(avg5R))*0.25; avg5R = f5ER;
			//manipulating deep reverb tail for realism
#endif			
#ifdef SIXBYSIX //instead of roomNoiseR, use reg6n
			
			a6AL[c6AL] = fma(f6BL,reg6n,inputSampleL);
			a6BL[c6BL] = fma(f6CL,reg6n,inputSampleL);
			a6CL[c6CL] = fma(f6DL,reg6n,inputSampleL);
			a6DL[c6DL] = fma(f6EL,reg6n,inputSampleL);
			a6EL[c6EL] = fma(f6FL,reg6n,inputSampleL);
			a6FL[c6FL] = fma(f6AL,reg6n,inputSampleL);
			
			a6FR[c6FR] = fma(f6LR,reg6n,inputSampleR);
			a6LR[c6LR] = fma(f6RR,reg6n,inputSampleR);
			a6RR[c6RR] = fma(f6XR,reg6n,inputSampleR);
			a6XR[c6XR] = fma(f6ZER,reg6n,inputSampleR);
			a6ZER[c6ZER] = fma(f6ZKR,reg6n,inputSampleR);
			a6ZKR[c6ZKR] = fma(f6FR,reg6n,inputSampleR);
			
			//left verb
			
			c6AL++; c6BL++; c6CL++; c6DL++; c6EL++; c6FL++; 
			
			if (c6AL > d6A) c6AL = 0;
			if (c6BL > d6B) c6BL = 0;
			if (c6CL > d6C) c6CL = 0;
			if (c6DL > d6D) c6DL = 0;
			if (c6EL > d6E) c6EL = 0;
			if (c6FL > d6F) c6FL = 0;
			
			hA = a6AL[c6AL-((c6AL > d6A)?d6A+1:0)];
			hB = a6BL[c6BL-((c6BL > d6B)?d6B+1:0)];
			hC = a6CL[c6CL-((c6CL > d6C)?d6C+1:0)];
			hD = a6DL[c6DL-((c6DL > d6D)?d6D+1:0)];
			hE = a6EL[c6EL-((c6EL > d6E)?d6E+1:0)];
			hF = a6FL[c6FL-((c6FL > d6F)?d6F+1:0)];
			
			a6GL[c6GL] = hB + hC + hD + hE + hF;
			a6HL[c6HL] = (hA + hC + hF) - (hD + hE);
			a6IL[c6IL] = (hA + hB + hD) - (hE + hF);
			a6JL[c6JL] = (hA + hC + hE) - (hB + hF);
			a6KL[c6KL] = (hA + hD + hF) - (hB + hC);
			a6LL[c6LL] = (hA + hB + hE) - (hC + hD);
			
			c6GL++; c6HL++; c6IL++; c6JL++; c6KL++; c6LL++; 
			
			if (c6GL > d6G) c6GL = 0;
			if (c6HL > d6H) c6HL = 0;
			if (c6IL > d6I) c6IL = 0;
			if (c6JL > d6J) c6JL = 0;
			if (c6KL > d6K) c6KL = 0;
			if (c6LL > d6L) c6LL = 0;
			
			hA = a6GL[c6GL-((c6GL > d6G)?d6G+1:0)];
			hB = a6HL[c6HL-((c6HL > d6H)?d6H+1:0)];
			hC = a6IL[c6IL-((c6IL > d6I)?d6I+1:0)];
			hD = a6JL[c6JL-((c6JL > d6J)?d6J+1:0)];
			hE = a6KL[c6KL-((c6KL > d6K)?d6K+1:0)];
			hF = a6LL[c6LL-((c6LL > d6L)?d6L+1:0)];
			
			a6ML[c6ML] = hB + hC + hD + hE + hF;
			a6NL[c6NL] = (hA + hC + hF) - (hD + hE);
			a6OL[c6OL] = (hA + hB + hD) - (hE + hF);
			a6PL[c6PL] = (hA + hC + hE) - (hB + hF);
			a6QL[c6QL] = (hA + hD + hF) - (hB + hC);
			a6RL[c6RL] = (hA + hB + hE) - (hC + hD);
			
			
			c6ML++; c6NL++; c6OL++; c6PL++; c6QL++; c6RL++; 
			
			if (c6ML > d6M) c6ML = 0;
			if (c6NL > d6N) c6NL = 0;
			if (c6OL > d6O) c6OL = 0;
			if (c6PL > d6P) c6PL = 0;
			if (c6QL > d6Q) c6QL = 0;
			if (c6RL > d6R) c6RL = 0;
			
			hA = a6ML[c6ML-((c6ML > d6M)?d6M+1:0)];
			hB = a6NL[c6NL-((c6NL > d6N)?d6N+1:0)];
			hC = a6OL[c6OL-((c6OL > d6O)?d6O+1:0)];
			hD = a6PL[c6PL-((c6PL > d6P)?d6P+1:0)];
			hE = a6QL[c6QL-((c6QL > d6Q)?d6Q+1:0)];
			hF = a6RL[c6RL-((c6RL > d6R)?d6R+1:0)];
			
			a6SL[c6SL] = hB + hC + hD + hE + hF;
			a6TL[c6TL] = (hA + hC + hF) - (hD + hE);
			a6UL[c6UL] = (hA + hB + hD) - (hE + hF);
			a6VL[c6VL] = (hA + hC + hE) - (hB + hF);
			a6WL[c6WL] = (hA + hD + hF) - (hB + hC);
			a6XL[c6XL] = (hA + hB + hE) - (hC + hD);
			
			
			c6SL++; c6TL++; c6UL++; c6VL++; c6WL++; c6XL++; 
			
			if (c6SL > d6S) c6SL = 0;
			if (c6TL > d6T) c6TL = 0;
			if (c6UL > d6U) c6UL = 0;
			if (c6VL > d6V) c6VL = 0;
			if (c6WL > d6W) c6WL = 0;
			if (c6XL > d6X) c6XL = 0;
			
			hA = a6SL[c6SL-((c6SL > d6S)?d6S+1:0)];
			hB = a6TL[c6TL-((c6TL > d6T)?d6T+1:0)];
			hC = a6UL[c6UL-((c6UL > d6U)?d6U+1:0)];
			hD = a6VL[c6VL-((c6VL > d6V)?d6V+1:0)];
			hE = a6WL[c6WL-((c6WL > d6W)?d6W+1:0)];
			hF = a6XL[c6XL-((c6XL > d6X)?d6X+1:0)];
			
			a6YL[c6YL] = hB + hC + hD + hE + hF;
			a6ZAL[c6ZAL] = (hA + hC + hF) - (hD + hE);
			a6ZBL[c6ZBL] = (hA + hB + hD) - (hE + hF);
			a6ZCL[c6ZCL] = (hA + hC + hE) - (hB + hF);
			a6ZDL[c6ZDL] = (hA + hD + hF) - (hB + hC);
			a6ZEL[c6ZEL] = (hA + hB + hE) - (hC + hD);
			
			c6YL++; c6ZAL++; c6ZBL++; c6ZCL++; c6ZDL++; c6ZEL++; 
			
			if (c6YL > d6Y) c6YL = 0;
			if (c6ZAL > d6ZA) c6ZAL = 0;
			if (c6ZBL > d6ZB) c6ZBL = 0;
			if (c6ZCL > d6ZC) c6ZCL = 0;
			if (c6ZDL > d6ZD) c6ZDL = 0;
			if (c6ZEL > d6ZE) c6ZEL = 0;
			
			hA = a6YL[c6YL-((c6YL > d6Y)?d6Y+1:0)];
			hB = a6ZAL[c6ZAL-((c6ZAL > d6ZA)?d6ZA+1:0)];
			hC = a6ZBL[c6ZBL-((c6ZBL > d6ZB)?d6ZB+1:0)];
			hD = a6ZCL[c6ZCL-((c6ZCL > d6ZC)?d6ZC+1:0)];
			hE = a6ZDL[c6ZDL-((c6ZDL > d6ZD)?d6ZD+1:0)];
			hF = a6ZEL[c6ZEL-((c6ZEL > d6ZE)?d6ZE+1:0)];
			
			a6ZFL[c6ZFL] = hB + hC + hD + hE + hF;
			a6ZGL[c6ZGL] = (hA + hC + hF) - (hD + hE);
			a6ZHL[c6ZHL] = (hA + hB + hD) - (hE + hF);
			a6ZIL[c6ZIL] = (hA + hC + hE) - (hB + hF);
			a6ZJL[c6ZJL] = (hA + hD + hF) - (hB + hC);
			a6ZKL[c6ZKL] = (hA + hB + hE) - (hC + hD);
			
			c6ZFL++; c6ZGL++; c6ZHL++; c6ZIL++; c6ZJL++; c6ZKL++; 
			
			if (c6ZFL > d6ZF) c6ZFL = 0;
			if (c6ZGL > d6ZG) c6ZGL = 0;
			if (c6ZHL > d6ZH) c6ZHL = 0;
			if (c6ZIL > d6ZI) c6ZIL = 0;
			if (c6ZJL > d6ZJ) c6ZJL = 0;
			if (c6ZKL > d6ZK) c6ZKL = 0;
			
			hA = a6ZFL[c6ZFL-((c6ZFL > d6ZF)?d6ZF+1:0)];
			hB = a6ZGL[c6ZGL-((c6ZGL > d6ZG)?d6ZG+1:0)];
			hC = a6ZHL[c6ZHL-((c6ZHL > d6ZH)?d6ZH+1:0)];
			hD = a6ZIL[c6ZIL-((c6ZIL > d6ZI)?d6ZI+1:0)];
			hE = a6ZJL[c6ZJL-((c6ZJL > d6ZJ)?d6ZJ+1:0)];
			hF = a6ZKL[c6ZKL-((c6ZKL > d6ZK)?d6ZK+1:0)];
			
			f6FR = hB + hC + hD + hE + hF;
			f6LR = (hA + hC + hF) - (hD + hE);
			f6RR = (hA + hB + hD) - (hE + hF);
			f6XR = (hA + hC + hE) - (hB + hF);
			f6ZER = (hA + hD + hF) - (hB + hC);
			f6ZKR = (hA + hB + hE) - (hC + hD);
			
			inputSampleL = (hB + hC + hD + hE + hF)*ga6n;
			
			//right verb
			
			c6FR++; c6LR++; c6RR++; c6XR++; c6ZER++; c6ZKR++; 
			
			if (c6FR > d6F) c6FR = 0;
			if (c6LR > d6L) c6LR = 0;
			if (c6RR > d6R) c6RR = 0;
			if (c6XR > d6X) c6XR = 0;
			if (c6ZER > d6ZE) c6ZER = 0;
			if (c6ZKR > d6ZK) c6ZKR = 0;
			
			hA = a6FR[c6FR-((c6FR > d6F)?d6F+1:0)];
			hB = a6LR[c6LR-((c6LR > d6L)?d6L+1:0)];
			hC = a6RR[c6RR-((c6RR > d6R)?d6R+1:0)];
			hD = a6XR[c6XR-((c6XR > d6X)?d6X+1:0)];
			hE = a6ZER[c6ZER-((c6ZER > d6ZE)?d6ZE+1:0)];
			hF = a6ZKR[c6ZKR-((c6ZKR > d6ZK)?d6ZK+1:0)];
			
			a6ER[c6ER] = hB + hC + hD + hE + hF;
			a6KR[c6KR] = (hA + hC + hF) - (hD + hE);
			a6QR[c6QR] = (hA + hB + hD) - (hE + hF);
			a6WR[c6WR] = (hA + hC + hE) - (hB + hF);
			a6ZDR[c6ZDR] = (hA + hD + hF) - (hB + hC);
			a6ZJR[c6ZJR] = (hA + hB + hE) - (hC + hD);
			
			c6ER++; c6KR++; c6QR++; c6WR++; c6ZDR++; c6ZJR++; 
			
			if (c6ER > d6E) c6ER = 0;
			if (c6KR > d6K) c6KR = 0;
			if (c6QR > d6Q) c6QR = 0;
			if (c6WR > d6W) c6WR = 0;
			if (c6ZDR > d6ZD) c6ZDR = 0;
			if (c6ZJR > d6ZJ) c6ZJR = 0;
			
			hA = a6ER[c6ER-((c6ER > d6E)?d6E+1:0)];
			hB = a6KR[c6KR-((c6KR > d6K)?d6K+1:0)];
			hC = a6QR[c6QR-((c6QR > d6Q)?d6Q+1:0)];
			hD = a6WR[c6WR-((c6WR > d6W)?d6W+1:0)];
			hE = a6ZDR[c6ZDR-((c6ZDR > d6ZD)?d6ZD+1:0)];
			hF = a6ZJR[c6ZJR-((c6ZJR > d6ZJ)?d6ZJ+1:0)];
			
			a6DR[c6DR] = hB + hC + hD + hE + hF;
			a6JR[c6JR] = (hA + hC + hF) - (hD + hE);
			a6PR[c6PR] = (hA + hB + hD) - (hE + hF);
			a6VR[c6VR] = (hA + hC + hE) - (hB + hF);
			a6ZCR[c6ZCR] = (hA + hD + hF) - (hB + hC);
			a6ZIR[c6ZIR] = (hA + hB + hE) - (hC + hD);
			
			c6DR++; c6JR++; c6PR++; c6VR++; c6ZCR++; c6ZIR++; 
			
			if (c6DR > d6D) c6DR = 0;
			if (c6JR > d6J) c6JR = 0;
			if (c6PR > d6P) c6PR = 0;
			if (c6VR > d6V) c6VR = 0;
			if (c6ZCR > d6ZC) c6ZCR = 0;
			if (c6ZIR > d6ZI) c6ZIR = 0;
			
			hA = a6DR[c6DR-((c6DR > d6D)?d6D+1:0)];
			hB = a6JR[c6JR-((c6JR > d6J)?d6J+1:0)];
			hC = a6PR[c6PR-((c6PR > d6P)?d6P+1:0)];
			hD = a6VR[c6VR-((c6VR > d6V)?d6V+1:0)];
			hE = a6ZCR[c6ZCR-((c6ZCR > d6ZC)?d6ZC+1:0)];
			hF = a6ZIR[c6ZIR-((c6ZIR > d6ZI)?d6ZI+1:0)];
			
			a6CR[c6CR] = hB + hC + hD + hE + hF;
			a6IR[c6IR] = (hA + hC + hF) - (hD + hE);
			a6OR[c6OR] = (hA + hB + hD) - (hE + hF);
			a6UR[c6UR] = (hA + hC + hE) - (hB + hF);
			a6ZBR[c6ZBR] = (hA + hD + hF) - (hB + hC);
			a6ZHR[c6ZHR] = (hA + hB + hE) - (hC + hD);
			
			c6CR++; c6IR++; c6OR++; c6UR++; c6ZBR++; c6ZHR++; 
			
			if (c6CR > d6C) c6CR = 0;
			if (c6IR > d6I) c6IR = 0;
			if (c6OR > d6O) c6OR = 0;
			if (c6UR > d6U) c6UR = 0;
			if (c6ZBR > d6ZB) c6ZBR = 0;
			if (c6ZHR > d6ZH) c6ZHR = 0;
			
			hA = a6CR[c6CR-((c6CR > d6C)?d6C+1:0)];
			hB = a6IR[c6IR-((c6IR > d6I)?d6I+1:0)];
			hC = a6OR[c6OR-((c6OR > d6O)?d6O+1:0)];
			hD = a6UR[c6UR-((c6UR > d6U)?d6U+1:0)];
			hE = a6ZBR[c6ZBR-((c6ZBR > d6ZB)?d6ZB+1:0)];
			hF = a6ZHR[c6ZHR-((c6ZHR > d6ZH)?d6ZH+1:0)];
			
			a6BR[c6BR] = hB + hC + hD + hE + hF;
			a6HR[c6HR] = (hA + hC + hF) - (hD + hE);
			a6NR[c6NR] = (hA + hB + hD) - (hE + hF);
			a6TR[c6TR] = (hA + hC + hE) - (hB + hF);
			a6ZAR[c6ZAR] = (hA + hD + hF) - (hB + hC);
			a6ZGR[c6ZGR] = (hA + hB + hE) - (hC + hD);
			
			c6BR++; c6HR++; c6NR++; c6TR++; c6ZBR++; c6ZGR++; 
			
			if (c6BR > d6B) c6BR = 0;
			if (c6HR > d6H) c6HR = 0;
			if (c6NR > d6N) c6NR = 0;
			if (c6TR > d6T) c6TR = 0;
			if (c6ZBR > d6ZB) c6ZBR = 0;
			if (c6ZGR > d6ZG) c6ZGR = 0;
			
			hA = a6BR[c6BR-((c6BR > d6B)?d6B+1:0)];
			hB = a6HR[c6HR-((c6HR > d6H)?d6H+1:0)];
			hC = a6NR[c6NR-((c6NR > d6N)?d6N+1:0)];
			hD = a6TR[c6TR-((c6TR > d6T)?d6T+1:0)];
			hE = a6ZAR[c6ZAR-((c6ZAR > d6ZA)?d6ZA+1:0)];
			hF = a6ZGR[c6ZGR-((c6ZGR > d6ZG)?d6ZG+1:0)];
			
			a6AR[c6AR] = hB + hC + hD + hE + hF;
			a6GR[c6GR] = (hA + hC + hF) - (hD + hE);
			a6MR[c6MR] = (hA + hB + hD) - (hE + hF);
			a6SR[c6SR] = (hA + hC + hE) - (hB + hF);
			a6YR[c6YR] = (hA + hD + hF) - (hB + hC);
			a6ZFR[c6ZFR] = (hA + hB + hE) - (hC + hD);
			
			c6AR++; c6GR++; c6MR++; c6SR++; c6YR++; c6ZFR++; 
			
			if (c6AR > d6A) c6AR = 0;
			if (c6GR > d6G) c6GR = 0;
			if (c6MR > d6M) c6MR = 0;
			if (c6SR > d6S) c6SR = 0;
			if (c6YR > d6Y) c6YR = 0;
			if (c6ZFR > d6ZF) c6ZFR = 0;
			
			hA = a6AR[c6AR-((c6AR > d6A)?d6A+1:0)];
			hB = a6GR[c6GR-((c6GR > d6G)?d6G+1:0)];
			hC = a6MR[c6MR-((c6MR > d6M)?d6M+1:0)];
			hD = a6SR[c6SR-((c6SR > d6S)?d6S+1:0)];
			hE = a6YR[c6YR-((c6YR > d6Y)?d6Y+1:0)];
			hF = a6ZFR[c6ZFR-((c6ZFR > d6ZF)?d6ZF+1:0)];
			
			f6AL = hB + hC + hD + hE + hF;
			f6BL = (hA + hC + hF) - (hD + hE);
			f6CL = (hA + hB + hD) - (hE + hF);
			f6DL = (hA + hC + hE) - (hB + hF);
			f6EL = (hA + hD + hF) - (hB + hC);
			f6FL = (hA + hB + hE) - (hC + hD);
			
			inputSampleR = (hB + hC + hD + hE + hF)*ga6n;
#endif
#ifdef SIXBYSIXMONO //instead of roomNoiseR, use reg6n
			
			a6AL[c6AL] = fma(f6BL,reg6n,inputSampleL);
			a6BL[c6BL] = fma(f6CL,reg6n,inputSampleL);
			a6CL[c6CL] = fma(f6DL,reg6n,inputSampleL);
			a6DL[c6DL] = fma(f6EL,reg6n,inputSampleL);
			a6EL[c6EL] = fma(f6FL,reg6n,inputSampleL);
			a6FL[c6FL] = fma(f6AL,reg6n,inputSampleL);
			
			a6AR[c6AL] = fma(f6BR,reg6n,inputSampleR);
			a6BR[c6BL] = fma(f6CR,reg6n,inputSampleR);
			a6CR[c6CL] = fma(f6DR,reg6n,inputSampleR);
			a6DR[c6DL] = fma(f6ER,reg6n,inputSampleR);
			a6ER[c6EL] = fma(f6FR,reg6n,inputSampleR);
			a6FR[c6FL] = fma(f6AR,reg6n,inputSampleR);
			//left verb
			
			c6AL++; c6BL++; c6CL++; c6DL++; c6EL++; c6FL++; 
			
			if (c6AL > d6A) c6AL = 0;
			if (c6BL > d6B) c6BL = 0;
			if (c6CL > d6C) c6CL = 0;
			if (c6DL > d6D) c6DL = 0;
			if (c6EL > d6E) c6EL = 0;
			if (c6FL > d6F) c6FL = 0;
			
			hA = a6AL[c6AL-((c6AL > d6A)?d6A+1:0)];
			hB = a6BL[c6BL-((c6BL > d6B)?d6B+1:0)];
			hC = a6CL[c6CL-((c6CL > d6C)?d6C+1:0)];
			hD = a6DL[c6DL-((c6DL > d6D)?d6D+1:0)];
			hE = a6EL[c6EL-((c6EL > d6E)?d6E+1:0)];
			hF = a6FL[c6FL-((c6FL > d6F)?d6F+1:0)];
			
			a6GL[c6GL] = hB + hC + hD + hE + hF;
			a6HL[c6HL] = (hA + hC + hF) - (hD + hE);
			a6IL[c6IL] = (hA + hB + hD) - (hE + hF);
			a6JL[c6JL] = (hA + hC + hE) - (hB + hF);
			a6KL[c6KL] = (hA + hD + hF) - (hB + hC);
			a6LL[c6LL] = (hA + hB + hE) - (hC + hD);
			
			hA = a6AR[c6AL-((c6AL > d6A)?d6A+1:0)];
			hB = a6BR[c6BL-((c6BL > d6B)?d6B+1:0)];
			hC = a6CR[c6CL-((c6CL > d6C)?d6C+1:0)];
			hD = a6DR[c6DL-((c6DL > d6D)?d6D+1:0)];
			hE = a6ER[c6EL-((c6EL > d6E)?d6E+1:0)];
			hF = a6FR[c6FL-((c6FL > d6F)?d6F+1:0)];
			
			a6GR[c6GL] = hB + hC + hD + hE + hF;
			a6HR[c6HL] = (hA + hC + hF) - (hD + hE);
			a6IR[c6IL] = (hA + hB + hD) - (hE + hF);
			a6JR[c6JL] = (hA + hC + hE) - (hB + hF);
			a6KR[c6KL] = (hA + hD + hF) - (hB + hC);
			a6LR[c6LL] = (hA + hB + hE) - (hC + hD);
			
			c6GL++; c6HL++; c6IL++; c6JL++; c6KL++; c6LL++; 
			
			if (c6GL > d6G) c6GL = 0;
			if (c6HL > d6H) c6HL = 0;
			if (c6IL > d6I) c6IL = 0;
			if (c6JL > d6J) c6JL = 0;
			if (c6KL > d6K) c6KL = 0;
			if (c6LL > d6L) c6LL = 0;
			
			hA = a6GL[c6GL-((c6GL > d6G)?d6G+1:0)];
			hB = a6HL[c6HL-((c6HL > d6H)?d6H+1:0)];
			hC = a6IL[c6IL-((c6IL > d6I)?d6I+1:0)];
			hD = a6JL[c6JL-((c6JL > d6J)?d6J+1:0)];
			hE = a6KL[c6KL-((c6KL > d6K)?d6K+1:0)];
			hF = a6LL[c6LL-((c6LL > d6L)?d6L+1:0)];
			
			a6ML[c6ML] = hB + hC + hD + hE + hF;
			a6NL[c6NL] = (hA + hC + hF) - (hD + hE);
			a6OL[c6OL] = (hA + hB + hD) - (hE + hF);
			a6PL[c6PL] = (hA + hC + hE) - (hB + hF);
			a6QL[c6QL] = (hA + hD + hF) - (hB + hC);
			a6RL[c6RL] = (hA + hB + hE) - (hC + hD);
			
			hA = a6GR[c6GL-((c6GL > d6G)?d6G+1:0)];
			hB = a6HR[c6HL-((c6HL > d6H)?d6H+1:0)];
			hC = a6IR[c6IL-((c6IL > d6I)?d6I+1:0)];
			hD = a6JR[c6JL-((c6JL > d6J)?d6J+1:0)];
			hE = a6KR[c6KL-((c6KL > d6K)?d6K+1:0)];
			hF = a6LR[c6LL-((c6LL > d6L)?d6L+1:0)];
			
			a6MR[c6ML] = hB + hC + hD + hE + hF;
			a6NR[c6NL] = (hA + hC + hF) - (hD + hE);
			a6OR[c6OL] = (hA + hB + hD) - (hE + hF);
			a6PR[c6PL] = (hA + hC + hE) - (hB + hF);
			a6QR[c6QL] = (hA + hD + hF) - (hB + hC);
			a6RR[c6RL] = (hA + hB + hE) - (hC + hD);
			
			
			c6ML++; c6NL++; c6OL++; c6PL++; c6QL++; c6RL++; 
			
			if (c6ML > d6M) c6ML = 0;
			if (c6NL > d6N) c6NL = 0;
			if (c6OL > d6O) c6OL = 0;
			if (c6PL > d6P) c6PL = 0;
			if (c6QL > d6Q) c6QL = 0;
			if (c6RL > d6R) c6RL = 0;
			
			hA = a6ML[c6ML-((c6ML > d6M)?d6M+1:0)];
			hB = a6NL[c6NL-((c6NL > d6N)?d6N+1:0)];
			hC = a6OL[c6OL-((c6OL > d6O)?d6O+1:0)];
			hD = a6PL[c6PL-((c6PL > d6P)?d6P+1:0)];
			hE = a6QL[c6QL-((c6QL > d6Q)?d6Q+1:0)];
			hF = a6RL[c6RL-((c6RL > d6R)?d6R+1:0)];
			
			a6SL[c6SL] = hB + hC + hD + hE + hF;
			a6TL[c6TL] = (hA + hC + hF) - (hD + hE);
			a6UL[c6UL] = (hA + hB + hD) - (hE + hF);
			a6VL[c6VL] = (hA + hC + hE) - (hB + hF);
			a6WL[c6WL] = (hA + hD + hF) - (hB + hC);
			a6XL[c6XL] = (hA + hB + hE) - (hC + hD);
			
			hA = a6MR[c6ML-((c6ML > d6M)?d6M+1:0)];
			hB = a6NR[c6NL-((c6NL > d6N)?d6N+1:0)];
			hC = a6OR[c6OL-((c6OL > d6O)?d6O+1:0)];
			hD = a6PR[c6PL-((c6PL > d6P)?d6P+1:0)];
			hE = a6QR[c6QL-((c6QL > d6Q)?d6Q+1:0)];
			hF = a6RR[c6RL-((c6RL > d6R)?d6R+1:0)];
			
			a6SR[c6SL] = hB + hC + hD + hE + hF;
			a6TR[c6TL] = (hA + hC + hF) - (hD + hE);
			a6UR[c6UL] = (hA + hB + hD) - (hE + hF);
			a6VR[c6VL] = (hA + hC + hE) - (hB + hF);
			a6WR[c6WL] = (hA + hD + hF) - (hB + hC);
			a6XR[c6XL] = (hA + hB + hE) - (hC + hD);
			
			
			c6SL++; c6TL++; c6UL++; c6VL++; c6WL++; c6XL++; 
			
			if (c6SL > d6S) c6SL = 0;
			if (c6TL > d6T) c6TL = 0;
			if (c6UL > d6U) c6UL = 0;
			if (c6VL > d6V) c6VL = 0;
			if (c6WL > d6W) c6WL = 0;
			if (c6XL > d6X) c6XL = 0;
			
			hA = a6SL[c6SL-((c6SL > d6S)?d6S+1:0)];
			hB = a6TL[c6TL-((c6TL > d6T)?d6T+1:0)];
			hC = a6UL[c6UL-((c6UL > d6U)?d6U+1:0)];
			hD = a6VL[c6VL-((c6VL > d6V)?d6V+1:0)];
			hE = a6WL[c6WL-((c6WL > d6W)?d6W+1:0)];
			hF = a6XL[c6XL-((c6XL > d6X)?d6X+1:0)];
			
			a6YL[c6YL] = hB + hC + hD + hE + hF;
			a6ZAL[c6ZAL] = (hA + hC + hF) - (hD + hE);
			a6ZBL[c6ZBL] = (hA + hB + hD) - (hE + hF);
			a6ZCL[c6ZCL] = (hA + hC + hE) - (hB + hF);
			a6ZDL[c6ZDL] = (hA + hD + hF) - (hB + hC);
			a6ZEL[c6ZEL] = (hA + hB + hE) - (hC + hD);
			
			hA = a6SR[c6SL-((c6SL > d6S)?d6S+1:0)];
			hB = a6TR[c6TL-((c6TL > d6T)?d6T+1:0)];
			hC = a6UR[c6UL-((c6UL > d6U)?d6U+1:0)];
			hD = a6VR[c6VL-((c6VL > d6V)?d6V+1:0)];
			hE = a6WR[c6WL-((c6WL > d6W)?d6W+1:0)];
			hF = a6XR[c6XL-((c6XL > d6X)?d6X+1:0)];
			
			a6YR[c6YL] = hB + hC + hD + hE + hF;
			a6ZAR[c6ZAL] = (hA + hC + hF) - (hD + hE);
			a6ZBR[c6ZBL] = (hA + hB + hD) - (hE + hF);
			a6ZCR[c6ZCL] = (hA + hC + hE) - (hB + hF);
			a6ZDR[c6ZDL] = (hA + hD + hF) - (hB + hC);
			a6ZER[c6ZEL] = (hA + hB + hE) - (hC + hD);
			
			c6YL++; c6ZAL++; c6ZBL++; c6ZCL++; c6ZDL++; c6ZEL++; 
			
			if (c6YL > d6Y) c6YL = 0;
			if (c6ZAL > d6ZA) c6ZAL = 0;
			if (c6ZBL > d6ZB) c6ZBL = 0;
			if (c6ZCL > d6ZC) c6ZCL = 0;
			if (c6ZDL > d6ZD) c6ZDL = 0;
			if (c6ZEL > d6ZE) c6ZEL = 0;
			
			hA = a6YL[c6YL-((c6YL > d6Y)?d6Y+1:0)];
			hB = a6ZAL[c6ZAL-((c6ZAL > d6ZA)?d6ZA+1:0)];
			hC = a6ZBL[c6ZBL-((c6ZBL > d6ZB)?d6ZB+1:0)];
			hD = a6ZCL[c6ZCL-((c6ZCL > d6ZC)?d6ZC+1:0)];
			hE = a6ZDL[c6ZDL-((c6ZDL > d6ZD)?d6ZD+1:0)];
			hF = a6ZEL[c6ZEL-((c6ZEL > d6ZE)?d6ZE+1:0)];
			
			a6ZFL[c6ZFL] = hB + hC + hD + hE + hF;
			a6ZGL[c6ZGL] = (hA + hC + hF) - (hD + hE);
			a6ZHL[c6ZHL] = (hA + hB + hD) - (hE + hF);
			a6ZIL[c6ZIL] = (hA + hC + hE) - (hB + hF);
			a6ZJL[c6ZJL] = (hA + hD + hF) - (hB + hC);
			a6ZKL[c6ZKL] = (hA + hB + hE) - (hC + hD);
			
			hA = a6YR[c6YL-((c6YL > d6Y)?d6Y+1:0)];
			hB = a6ZAR[c6ZAL-((c6ZAL > d6ZA)?d6ZA+1:0)];
			hC = a6ZBR[c6ZBL-((c6ZBL > d6ZB)?d6ZB+1:0)];
			hD = a6ZCR[c6ZCL-((c6ZCL > d6ZC)?d6ZC+1:0)];
			hE = a6ZDR[c6ZDL-((c6ZDL > d6ZD)?d6ZD+1:0)];
			hF = a6ZER[c6ZEL-((c6ZEL > d6ZE)?d6ZE+1:0)];
			
			a6ZFR[c6ZFL] = hB + hC + hD + hE + hF;
			a6ZGR[c6ZGL] = (hA + hC + hF) - (hD + hE);
			a6ZHR[c6ZHL] = (hA + hB + hD) - (hE + hF);
			a6ZIR[c6ZIL] = (hA + hC + hE) - (hB + hF);
			a6ZJR[c6ZJL] = (hA + hD + hF) - (hB + hC);
			a6ZKR[c6ZKL] = (hA + hB + hE) - (hC + hD);
			
			c6ZFL++; c6ZGL++; c6ZHL++; c6ZIL++; c6ZJL++; c6ZKL++; 
			
			if (c6ZFL > d6ZF) c6ZFL = 0;
			if (c6ZGL > d6ZG) c6ZGL = 0;
			if (c6ZHL > d6ZH) c6ZHL = 0;
			if (c6ZIL > d6ZI) c6ZIL = 0;
			if (c6ZJL > d6ZJ) c6ZJL = 0;
			if (c6ZKL > d6ZK) c6ZKL = 0;
			
			hA = a6ZFL[c6ZFL-((c6ZFL > d6ZF)?d6ZF+1:0)];
			hB = a6ZGL[c6ZGL-((c6ZGL > d6ZG)?d6ZG+1:0)];
			hC = a6ZHL[c6ZHL-((c6ZHL > d6ZH)?d6ZH+1:0)];
			hD = a6ZIL[c6ZIL-((c6ZIL > d6ZI)?d6ZI+1:0)];
			hE = a6ZJL[c6ZJL-((c6ZJL > d6ZJ)?d6ZJ+1:0)];
			hF = a6ZKL[c6ZKL-((c6ZKL > d6ZK)?d6ZK+1:0)];
			
			f6AL = hB + hC + hD + hE + hF;
			f6BL = (hA + hC + hF) - (hD + hE);
			f6CL = (hA + hB + hD) - (hE + hF);
			f6DL = (hA + hC + hE) - (hB + hF);
			f6EL = (hA + hD + hF) - (hB + hC);
			f6FL = (hA + hB + hE) - (hC + hD);
			
			inputSampleL = (hB + hC + hD + hE + hF)*ga6n;
			
			hA = a6ZFR[c6ZFL-((c6ZFL > d6ZF)?d6ZF+1:0)];
			hB = a6ZGR[c6ZGL-((c6ZGL > d6ZG)?d6ZG+1:0)];
			hC = a6ZHR[c6ZHL-((c6ZHL > d6ZH)?d6ZH+1:0)];
			hD = a6ZIR[c6ZIL-((c6ZIL > d6ZI)?d6ZI+1:0)];
			hE = a6ZJR[c6ZJL-((c6ZJL > d6ZJ)?d6ZJ+1:0)];
			hF = a6ZKR[c6ZKL-((c6ZKL > d6ZK)?d6ZK+1:0)];
			
			f6AR = hB + hC + hD + hE + hF;
			f6BR = (hA + hC + hF) - (hD + hE);
			f6CR = (hA + hB + hD) - (hE + hF);
			f6DR = (hA + hC + hE) - (hB + hF);
			f6ER = (hA + hD + hF) - (hB + hC);
			f6FR = (hA + hB + hE) - (hC + hD);
			
			inputSampleR = (hB + hC + hD + hE + hF)*ga6n;
#endif
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		double X = bez[bez_cycle];
		inputSampleL = (bez[bez_BL]+(bez[bez_CL]*(1.0-X)*(1.0-X))+(bez[bez_BL]*2.0*(1.0-X)*X)+(bez[bez_AL]*X*X))*-0.375;
		inputSampleR = (bez[bez_BR]+(bez[bez_CR]*(1.0-X)*(1.0-X))+(bez[bez_BR]*2.0*(1.0-X)*X)+(bez[bez_AR]*X*X))*-0.375;
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 3.553e-44l  * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
