/* ========================================
 *  VerbSixes - VerbSixes.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __VerbSixes_H
#include "VerbSixes.h"
#endif

void VerbSixes::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
#ifdef THREEBYTHREE
	double reg3n = 0.125;
	double ga3n = (reg3n+reg3n+0.0625)*0.45;
	reg3n = (reg3n+reg3n+0.0625)*0.38;
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
	double reg6n = 0.0009765625;
	double ga6n = (reg6n+reg6n+0.001953125)*0.29;
	reg6n = (reg6n+reg6n+0.001953125)*0.277;
#endif
	//adding more digits to that fraction gets closer to infinite f6. 0.333 is already long
	double derez = 1.0/overallscale;
	derez = 1.0 / ((int)(1.0/derez));
	//this hard-locks derez to exact subdivisions of 1.0
	if (derez < 0.0005) derez = 0.0005; if (derez > 1.0) derez = 1.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += ((inputSampleL+bez[bez_InL]) * derez);
		bez[bez_SampR] += ((inputSampleR+bez[bez_InR]) * derez);
		bez[bez_InL] = inputSampleL; bez[bez_InR] = inputSampleR;
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			inputSampleL = bez[bez_SampL];
			inputSampleR = bez[bez_SampR];
			
#ifdef THREEBYTHREE
			a3AL[c3AL] = inputSampleL + (f3AL * reg3n);
			a3BL[c3BL] = inputSampleL + (f3BL * reg3n);
			a3CL[c3CL] = inputSampleL + (f3CL * reg3n);
			
			a3CR[c3CR] = inputSampleR + (f3CR * reg3n);
			a3FR[c3FR] = inputSampleR + (f3FR * reg3n);
			a3IR[c3IR] = inputSampleR + (f3IR * reg3n);
			
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
			
			a3DL[c3DL] = ((o3BL + o3CL) - o3AL);
			a3EL[c3EL] = ((o3AL + o3CL) - o3BL);
			a3FL[c3FL] = ((o3AL + o3BL) - o3CL);
			a3BR[c3BR] = ((o3FR + o3IR) - o3CR);
			a3ER[c3ER] = ((o3CR + o3IR) - o3FR);
			a3HR[c3HR] = ((o3CR + o3FR) - o3IR);
			
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
			
			a3GL[c3GL] = ((o3EL + o3FL) - o3DL);
			a3HL[c3HL] = ((o3DL + o3FL) - o3EL);
			a3IL[c3IL] = ((o3DL + o3EL) - o3FL);
			a3AR[c3AR] = ((o3ER + o3HR) - o3BR);
			a3DR[c3DR] = ((o3BR + o3HR) - o3ER);
			a3GR[c3GR] = ((o3BR + o3ER) - o3HR);
			
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
			
			f3AL = (o3GR + o3HR + o3IR);				
			f3BL = (o3GR + o3HR + o3IR);
			f3CL = (o3GR + o3HR + o3IR);
			
			f3CR = (o3AL + o3DL + o3GL);
			f3FR = (o3AL + o3DL + o3GL);
			f3IR = (o3AL + o3DL + o3GL);
			
			double inputSampleL = (o3GL + o3HL + o3IL)*ga3n;
			double inputSampleR = (o3AR + o3DR + o3GR)*ga3n;
			
			f3AL = (f3AL+f3AL+f3AL+fabs(avg3L))*0.25; avg3L = f3AL;
			f3CR = (f3CR+f3CR+f3CR+fabs(avg3R))*0.25; avg3R = f3CR;
			//manipulating deep reverb tail for realism			
			
#endif
#ifdef FOURBYFOUR
			a4AL[c4AL] = inputSampleL + (f4AL * reg4n);
			a4BL[c4BL] = inputSampleL + (f4BL * reg4n);
			a4CL[c4CL] = inputSampleL + (f4CL * reg4n);
			a4DL[c4DL] = inputSampleL + (f4DL * reg4n);
			
			c4AL++; if (c4AL < 0 || c4AL > d4A) c4AL = 0;
			c4BL++; if (c4BL < 0 || c4BL > d4B) c4BL = 0;
			c4CL++; if (c4CL < 0 || c4CL > d4C) c4CL = 0;
			c4DL++; if (c4DL < 0 || c4DL > d4D) c4DL = 0;
			
			double o4AL = a4AL[c4AL-((c4AL > d4A)?d4A+1:0)];
			double o4BL = a4BL[c4BL-((c4BL > d4B)?d4B+1:0)];
			double o4CL = a4CL[c4CL-((c4CL > d4C)?d4C+1:0)];
			double o4DL = a4DL[c4DL-((c4DL > d4D)?d4D+1:0)];
			
			a4DR[c4DR] = inputSampleR + (f4DR * reg4n);
			a4HR[c4HR] = inputSampleR + (f4HR * reg4n);
			a4LR[c4LR] = inputSampleR + (f4LR * reg4n);
			a4PR[c4PR] = inputSampleR + (f4PR * reg4n);
			
			c4DR++; if (c4DR < 0 || c4DR > d4D) c4DR = 0;
			c4HR++; if (c4HR < 0 || c4HR > d4H) c4HR = 0;
			c4LR++; if (c4LR < 0 || c4LR > d4L) c4LR = 0;
			c4PR++; if (c4PR < 0 || c4PR > d4P) c4PR = 0;
			
			double o4DR = a4DR[c4DR-((c4DR > d4D)?d4D+1:0)];
			double o4HR = a4HR[c4HR-((c4HR > d4H)?d4H+1:0)];
			double o4LR = a4LR[c4LR-((c4LR > d4L)?d4L+1:0)];
			double o4PR = a4PR[c4PR-((c4PR > d4P)?d4P+1:0)];
			
			//-------- one
			
			a4EL[c4EL] = o4AL - (o4BL + o4CL + o4DL);
			a4FL[c4FL] = o4BL - (o4AL + o4CL + o4DL);
			a4GL[c4GL] = o4CL - (o4AL + o4BL + o4DL);
			a4HL[c4HL] = o4DL - (o4AL + o4BL + o4CL);
			
			c4EL++; if (c4EL < 0 || c4EL > d4E) c4EL = 0;
			c4FL++; if (c4FL < 0 || c4FL > d4F) c4FL = 0;
			c4GL++; if (c4GL < 0 || c4GL > d4G) c4GL = 0;
			c4HL++; if (c4HL < 0 || c4HL > d4H) c4HL = 0;
			
			double o4EL = a4EL[c4EL-((c4EL > d4E)?d4E+1:0)];
			double o4FL = a4FL[c4FL-((c4FL > d4F)?d4F+1:0)];
			double o4GL = a4GL[c4GL-((c4GL > d4G)?d4G+1:0)];
			double o4HL = a4HL[c4HL-((c4HL > d4H)?d4H+1:0)];
			
			a4CR[c4CR] = o4DR - (o4HR + o4LR + o4PR);
			a4GR[c4GR] = o4HR - (o4DR + o4LR + o4PR);
			a4KR[c4KR] = o4LR - (o4DR + o4HR + o4PR);
			a4OR[c4OR] = o4PR - (o4DR + o4HR + o4LR);
			
			c4CR++; if (c4CR < 0 || c4CR > d4C) c4CR = 0;
			c4GR++; if (c4GR < 0 || c4GR > d4G) c4GR = 0;
			c4KR++; if (c4KR < 0 || c4KR > d4K) c4KR = 0;
			c4OR++; if (c4OR < 0 || c4OR > d4O) c4OR = 0;
			
			double o4CR = a4CR[c4CR-((c4CR > d4C)?d4C+1:0)];
			double o4GR = a4GR[c4GR-((c4GR > d4G)?d4G+1:0)];
			double o4KR = a4KR[c4KR-((c4KR > d4K)?d4K+1:0)];
			double o4OR = a4OR[c4OR-((c4OR > d4O)?d4O+1:0)];
			
			//-------- two
			
			a4IL[c4IL] = o4EL - (o4FL + o4GL + o4HL);
			a4JL[c4JL] = o4FL - (o4EL + o4GL + o4HL);
			a4KL[c4KL] = o4GL - (o4EL + o4FL + o4HL);
			a4LL[c4LL] = o4HL - (o4EL + o4FL + o4GL);
			
			c4IL++; if (c4IL < 0 || c4IL > d4I) c4IL = 0;
			c4JL++; if (c4JL < 0 || c4JL > d4J) c4JL = 0;
			c4KL++; if (c4KL < 0 || c4KL > d4K) c4KL = 0;
			c4LL++; if (c4LL < 0 || c4LL > d4L) c4LL = 0;
			
			double o4IL = a4IL[c4IL-((c4IL > d4I)?d4I+1:0)];
			double o4JL = a4JL[c4JL-((c4JL > d4J)?d4J+1:0)];
			double o4KL = a4KL[c4KL-((c4KL > d4K)?d4K+1:0)];
			double o4LL = a4LL[c4LL-((c4LL > d4L)?d4L+1:0)];
			
			a4BR[c4BR] = o4CR - (o4GR + o4KR + o4OR);
			a4FR[c4FR] = o4GR - (o4CR + o4KR + o4OR);
			a4JR[c4JR] = o4KR - (o4CR + o4GR + o4OR);
			a4NR[c4NR] = o4OR - (o4CR + o4GR + o4KR);
			
			c4BR++; if (c4BR < 0 || c4BR > d4B) c4BR = 0;
			c4FR++; if (c4FR < 0 || c4FR > d4F) c4FR = 0;
			c4JR++; if (c4JR < 0 || c4JR > d4J) c4JR = 0;
			c4NR++; if (c4NR < 0 || c4NR > d4N) c4NR = 0;
			
			double o4BR = a4BR[c4BR-((c4BR > d4B)?d4B+1:0)];
			double o4FR = a4FR[c4FR-((c4FR > d4F)?d4F+1:0)];
			double o4JR = a4JR[c4JR-((c4JR > d4J)?d4J+1:0)];
			double o4NR = a4NR[c4NR-((c4NR > d4N)?d4N+1:0)];
			
			//-------- three
			
			a4ML[c4ML] = o4IL - (o4JL + o4KL + o4LL);
			a4NL[c4NL] = o4JL - (o4IL + o4KL + o4LL);
			a4OL[c4OL] = o4KL - (o4IL + o4JL + o4LL);
			a4PL[c4PL] = o4LL - (o4IL + o4JL + o4KL);
			
			c4ML++; if (c4ML < 0 || c4ML > d4M) c4ML = 0;
			c4NL++; if (c4NL < 0 || c4NL > d4N) c4NL = 0;
			c4OL++; if (c4OL < 0 || c4OL > d4O) c4OL = 0;
			c4PL++; if (c4PL < 0 || c4PL > d4P) c4PL = 0;
			
			double o4ML = a4ML[c4ML-((c4ML > d4M)?d4M+1:0)];
			double o4NL = a4NL[c4NL-((c4NL > d4N)?d4N+1:0)];
			double o4OL = a4OL[c4OL-((c4OL > d4O)?d4O+1:0)];
			double o4PL = a4PL[c4PL-((c4PL > d4P)?d4P+1:0)];
			
			a4AR[c4AR] = o4BR - (o4FR + o4JR + o4NR);
			a4ER[c4ER] = o4FR - (o4BR + o4JR + o4NR);
			a4IR[c4IR] = o4JR - (o4BR + o4FR + o4NR);
			a4MR[c4MR] = o4NR - (o4BR + o4FR + o4JR);
			
			c4AR++; if (c4AR < 0 || c4AR > d4A) c4AR = 0;
			c4ER++; if (c4ER < 0 || c4ER > d4E) c4ER = 0;
			c4IR++; if (c4IR < 0 || c4IR > d4I) c4IR = 0;
			c4MR++; if (c4MR < 0 || c4MR > d4M) c4MR = 0;
			
			double o4AR = a4AR[c4AR-((c4AR > d4A)?d4A+1:0)];
			double o4ER = a4ER[c4ER-((c4ER > d4E)?d4E+1:0)];
			double o4IR = a4IR[c4IR-((c4IR > d4I)?d4I+1:0)];
			double o4MR = a4MR[c4MR-((c4MR > d4M)?d4M+1:0)];
			
			//-------- four
			
			f4AL = o4AR - (o4ER + o4IR + o4MR);				
			f4BL = o4ER - (o4AR + o4IR + o4MR);
			f4CL = o4IR - (o4AR + o4ER + o4MR);
			f4DL = o4MR - (o4AR + o4ER + o4IR);
			
			f4DR = o4ML - (o4NL + o4OL + o4PL);
			f4HR = o4NL - (o4ML + o4OL + o4PL);
			f4LR = o4OL - (o4ML + o4NL + o4PL);
			f4PR = o4PL - (o4ML + o4NL + o4OL);
			
			inputSampleL = (o4ML + o4NL + o4OL + o4PL)*ga4n;
			inputSampleR = (o4AR + o4ER + o4IR + o4MR)*ga4n;
			
			f4AL = (f4AL+f4AL+f4AL+fabs(avg4L))*0.25; avg4L = f4AL;
			f4DR = (f4DR+f4DR+f4DR+fabs(avg4R))*0.25; avg4R = f4DR;
			//manipulating deep reverb tail for realism			
			
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
#ifdef SIXBYSIX
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
			
			inputSampleL = (o6ZFL + o6ZGL + o6ZHL + o6ZIL + o6ZJL + o6ZKL)*ga6n;
			inputSampleR = (o6AR + o6GR + o6MR + o6SR + o6YR + o6ZFR)*ga6n;
			
			f6AL = (f6AL+f6AL+f6AL+fabs(avg6L))*0.25; avg6L = f6AL;
			f6FR = (f6FR+f6FR+f6FR+fabs(avg6R))*0.25; avg6R = f6FR;
			//manipulating deep reverb tail for realism
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
		double CBL = (bez[bez_CL]*(1.0-bez[bez_cycle]))+(bez[bez_BL]*bez[bez_cycle]);
		double CBR = (bez[bez_CR]*(1.0-bez[bez_cycle]))+(bez[bez_BR]*bez[bez_cycle]);
		double BAL = (bez[bez_BL]*(1.0-bez[bez_cycle]))+(bez[bez_AL]*bez[bez_cycle]);
		double BAR = (bez[bez_BR]*(1.0-bez[bez_cycle]))+(bez[bez_AR]*bez[bez_cycle]);
		double CBAL = (bez[bez_BL]+(CBL*(1.0-bez[bez_cycle]))+(BAL*bez[bez_cycle]))*0.125;
		double CBAR = (bez[bez_BR]+(CBR*(1.0-bez[bez_cycle]))+(BAR*bez[bez_cycle]))*0.125;
		inputSampleL = CBAL;
		inputSampleR = CBAR;
		
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

void VerbSixes::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
#ifdef THREEBYTHREE
	double reg3n = 0.125;
	double ga3n = (reg3n+reg3n+0.0625)*0.45;
	reg3n = (reg3n+reg3n+0.0625)*0.38;
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
	double reg6n = 0.0009765625;
	double ga6n = (reg6n+reg6n+0.001953125)*0.29;
	reg6n = (reg6n+reg6n+0.001953125)*0.277;
#endif
	//adding more digits to that fraction gets closer to infinite f6. 0.333 is already long
	double derez = 1.0/overallscale;
	derez = 1.0 / ((int)(1.0/derez));
	//this hard-locks derez to exact subdivisions of 1.0
	if (derez < 0.0005) derez = 0.0005; if (derez > 1.0) derez = 1.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += ((inputSampleL+bez[bez_InL]) * derez);
		bez[bez_SampR] += ((inputSampleR+bez[bez_InR]) * derez);
		bez[bez_InL] = inputSampleL; bez[bez_InR] = inputSampleR;
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			inputSampleL = bez[bez_SampL];
			inputSampleR = bez[bez_SampR];
			
#ifdef THREEBYTHREE
			a3AL[c3AL] = inputSampleL + (f3AL * reg3n);
			a3BL[c3BL] = inputSampleL + (f3BL * reg3n);
			a3CL[c3CL] = inputSampleL + (f3CL * reg3n);
			
			a3CR[c3CR] = inputSampleR + (f3CR * reg3n);
			a3FR[c3FR] = inputSampleR + (f3FR * reg3n);
			a3IR[c3IR] = inputSampleR + (f3IR * reg3n);
			
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
			
			a3DL[c3DL] = ((o3BL + o3CL) - o3AL);
			a3EL[c3EL] = ((o3AL + o3CL) - o3BL);
			a3FL[c3FL] = ((o3AL + o3BL) - o3CL);
			a3BR[c3BR] = ((o3FR + o3IR) - o3CR);
			a3ER[c3ER] = ((o3CR + o3IR) - o3FR);
			a3HR[c3HR] = ((o3CR + o3FR) - o3IR);
			
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
			
			a3GL[c3GL] = ((o3EL + o3FL) - o3DL);
			a3HL[c3HL] = ((o3DL + o3FL) - o3EL);
			a3IL[c3IL] = ((o3DL + o3EL) - o3FL);
			a3AR[c3AR] = ((o3ER + o3HR) - o3BR);
			a3DR[c3DR] = ((o3BR + o3HR) - o3ER);
			a3GR[c3GR] = ((o3BR + o3ER) - o3HR);
			
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
			
			f3AL = (o3GR + o3HR + o3IR);				
			f3BL = (o3GR + o3HR + o3IR);
			f3CL = (o3GR + o3HR + o3IR);
			
			f3CR = (o3AL + o3DL + o3GL);
			f3FR = (o3AL + o3DL + o3GL);
			f3IR = (o3AL + o3DL + o3GL);
			
			double inputSampleL = (o3GL + o3HL + o3IL)*ga3n;
			double inputSampleR = (o3AR + o3DR + o3GR)*ga3n;
			
			f3AL = (f3AL+f3AL+f3AL+fabs(avg3L))*0.25; avg3L = f3AL;
			f3CR = (f3CR+f3CR+f3CR+fabs(avg3R))*0.25; avg3R = f3CR;
			//manipulating deep reverb tail for realism			
			
#endif
#ifdef FOURBYFOUR
			a4AL[c4AL] = inputSampleL + (f4AL * reg4n);
			a4BL[c4BL] = inputSampleL + (f4BL * reg4n);
			a4CL[c4CL] = inputSampleL + (f4CL * reg4n);
			a4DL[c4DL] = inputSampleL + (f4DL * reg4n);
			
			c4AL++; if (c4AL < 0 || c4AL > d4A) c4AL = 0;
			c4BL++; if (c4BL < 0 || c4BL > d4B) c4BL = 0;
			c4CL++; if (c4CL < 0 || c4CL > d4C) c4CL = 0;
			c4DL++; if (c4DL < 0 || c4DL > d4D) c4DL = 0;
			
			double o4AL = a4AL[c4AL-((c4AL > d4A)?d4A+1:0)];
			double o4BL = a4BL[c4BL-((c4BL > d4B)?d4B+1:0)];
			double o4CL = a4CL[c4CL-((c4CL > d4C)?d4C+1:0)];
			double o4DL = a4DL[c4DL-((c4DL > d4D)?d4D+1:0)];
			
			a4DR[c4DR] = inputSampleR + (f4DR * reg4n);
			a4HR[c4HR] = inputSampleR + (f4HR * reg4n);
			a4LR[c4LR] = inputSampleR + (f4LR * reg4n);
			a4PR[c4PR] = inputSampleR + (f4PR * reg4n);
			
			c4DR++; if (c4DR < 0 || c4DR > d4D) c4DR = 0;
			c4HR++; if (c4HR < 0 || c4HR > d4H) c4HR = 0;
			c4LR++; if (c4LR < 0 || c4LR > d4L) c4LR = 0;
			c4PR++; if (c4PR < 0 || c4PR > d4P) c4PR = 0;
			
			double o4DR = a4DR[c4DR-((c4DR > d4D)?d4D+1:0)];
			double o4HR = a4HR[c4HR-((c4HR > d4H)?d4H+1:0)];
			double o4LR = a4LR[c4LR-((c4LR > d4L)?d4L+1:0)];
			double o4PR = a4PR[c4PR-((c4PR > d4P)?d4P+1:0)];
			
			//-------- one
			
			a4EL[c4EL] = o4AL - (o4BL + o4CL + o4DL);
			a4FL[c4FL] = o4BL - (o4AL + o4CL + o4DL);
			a4GL[c4GL] = o4CL - (o4AL + o4BL + o4DL);
			a4HL[c4HL] = o4DL - (o4AL + o4BL + o4CL);
			
			c4EL++; if (c4EL < 0 || c4EL > d4E) c4EL = 0;
			c4FL++; if (c4FL < 0 || c4FL > d4F) c4FL = 0;
			c4GL++; if (c4GL < 0 || c4GL > d4G) c4GL = 0;
			c4HL++; if (c4HL < 0 || c4HL > d4H) c4HL = 0;
			
			double o4EL = a4EL[c4EL-((c4EL > d4E)?d4E+1:0)];
			double o4FL = a4FL[c4FL-((c4FL > d4F)?d4F+1:0)];
			double o4GL = a4GL[c4GL-((c4GL > d4G)?d4G+1:0)];
			double o4HL = a4HL[c4HL-((c4HL > d4H)?d4H+1:0)];
			
			a4CR[c4CR] = o4DR - (o4HR + o4LR + o4PR);
			a4GR[c4GR] = o4HR - (o4DR + o4LR + o4PR);
			a4KR[c4KR] = o4LR - (o4DR + o4HR + o4PR);
			a4OR[c4OR] = o4PR - (o4DR + o4HR + o4LR);
			
			c4CR++; if (c4CR < 0 || c4CR > d4C) c4CR = 0;
			c4GR++; if (c4GR < 0 || c4GR > d4G) c4GR = 0;
			c4KR++; if (c4KR < 0 || c4KR > d4K) c4KR = 0;
			c4OR++; if (c4OR < 0 || c4OR > d4O) c4OR = 0;
			
			double o4CR = a4CR[c4CR-((c4CR > d4C)?d4C+1:0)];
			double o4GR = a4GR[c4GR-((c4GR > d4G)?d4G+1:0)];
			double o4KR = a4KR[c4KR-((c4KR > d4K)?d4K+1:0)];
			double o4OR = a4OR[c4OR-((c4OR > d4O)?d4O+1:0)];
			
			//-------- two
			
			a4IL[c4IL] = o4EL - (o4FL + o4GL + o4HL);
			a4JL[c4JL] = o4FL - (o4EL + o4GL + o4HL);
			a4KL[c4KL] = o4GL - (o4EL + o4FL + o4HL);
			a4LL[c4LL] = o4HL - (o4EL + o4FL + o4GL);
			
			c4IL++; if (c4IL < 0 || c4IL > d4I) c4IL = 0;
			c4JL++; if (c4JL < 0 || c4JL > d4J) c4JL = 0;
			c4KL++; if (c4KL < 0 || c4KL > d4K) c4KL = 0;
			c4LL++; if (c4LL < 0 || c4LL > d4L) c4LL = 0;
			
			double o4IL = a4IL[c4IL-((c4IL > d4I)?d4I+1:0)];
			double o4JL = a4JL[c4JL-((c4JL > d4J)?d4J+1:0)];
			double o4KL = a4KL[c4KL-((c4KL > d4K)?d4K+1:0)];
			double o4LL = a4LL[c4LL-((c4LL > d4L)?d4L+1:0)];
			
			a4BR[c4BR] = o4CR - (o4GR + o4KR + o4OR);
			a4FR[c4FR] = o4GR - (o4CR + o4KR + o4OR);
			a4JR[c4JR] = o4KR - (o4CR + o4GR + o4OR);
			a4NR[c4NR] = o4OR - (o4CR + o4GR + o4KR);
			
			c4BR++; if (c4BR < 0 || c4BR > d4B) c4BR = 0;
			c4FR++; if (c4FR < 0 || c4FR > d4F) c4FR = 0;
			c4JR++; if (c4JR < 0 || c4JR > d4J) c4JR = 0;
			c4NR++; if (c4NR < 0 || c4NR > d4N) c4NR = 0;
			
			double o4BR = a4BR[c4BR-((c4BR > d4B)?d4B+1:0)];
			double o4FR = a4FR[c4FR-((c4FR > d4F)?d4F+1:0)];
			double o4JR = a4JR[c4JR-((c4JR > d4J)?d4J+1:0)];
			double o4NR = a4NR[c4NR-((c4NR > d4N)?d4N+1:0)];
			
			//-------- three
			
			a4ML[c4ML] = o4IL - (o4JL + o4KL + o4LL);
			a4NL[c4NL] = o4JL - (o4IL + o4KL + o4LL);
			a4OL[c4OL] = o4KL - (o4IL + o4JL + o4LL);
			a4PL[c4PL] = o4LL - (o4IL + o4JL + o4KL);
			
			c4ML++; if (c4ML < 0 || c4ML > d4M) c4ML = 0;
			c4NL++; if (c4NL < 0 || c4NL > d4N) c4NL = 0;
			c4OL++; if (c4OL < 0 || c4OL > d4O) c4OL = 0;
			c4PL++; if (c4PL < 0 || c4PL > d4P) c4PL = 0;
			
			double o4ML = a4ML[c4ML-((c4ML > d4M)?d4M+1:0)];
			double o4NL = a4NL[c4NL-((c4NL > d4N)?d4N+1:0)];
			double o4OL = a4OL[c4OL-((c4OL > d4O)?d4O+1:0)];
			double o4PL = a4PL[c4PL-((c4PL > d4P)?d4P+1:0)];
			
			a4AR[c4AR] = o4BR - (o4FR + o4JR + o4NR);
			a4ER[c4ER] = o4FR - (o4BR + o4JR + o4NR);
			a4IR[c4IR] = o4JR - (o4BR + o4FR + o4NR);
			a4MR[c4MR] = o4NR - (o4BR + o4FR + o4JR);
			
			c4AR++; if (c4AR < 0 || c4AR > d4A) c4AR = 0;
			c4ER++; if (c4ER < 0 || c4ER > d4E) c4ER = 0;
			c4IR++; if (c4IR < 0 || c4IR > d4I) c4IR = 0;
			c4MR++; if (c4MR < 0 || c4MR > d4M) c4MR = 0;
			
			double o4AR = a4AR[c4AR-((c4AR > d4A)?d4A+1:0)];
			double o4ER = a4ER[c4ER-((c4ER > d4E)?d4E+1:0)];
			double o4IR = a4IR[c4IR-((c4IR > d4I)?d4I+1:0)];
			double o4MR = a4MR[c4MR-((c4MR > d4M)?d4M+1:0)];
			
			//-------- four
			
			f4AL = o4AR - (o4ER + o4IR + o4MR);				
			f4BL = o4ER - (o4AR + o4IR + o4MR);
			f4CL = o4IR - (o4AR + o4ER + o4MR);
			f4DL = o4MR - (o4AR + o4ER + o4IR);
			
			f4DR = o4ML - (o4NL + o4OL + o4PL);
			f4HR = o4NL - (o4ML + o4OL + o4PL);
			f4LR = o4OL - (o4ML + o4NL + o4PL);
			f4PR = o4PL - (o4ML + o4NL + o4OL);
			
			inputSampleL = (o4ML + o4NL + o4OL + o4PL)*ga4n;
			inputSampleR = (o4AR + o4ER + o4IR + o4MR)*ga4n;
			
			f4AL = (f4AL+f4AL+f4AL+fabs(avg4L))*0.25; avg4L = f4AL;
			f4DR = (f4DR+f4DR+f4DR+fabs(avg4R))*0.25; avg4R = f4DR;
			//manipulating deep reverb tail for realism			
			
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
#ifdef SIXBYSIX
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
			
			inputSampleL = (o6ZFL + o6ZGL + o6ZHL + o6ZIL + o6ZJL + o6ZKL)*ga6n;
			inputSampleR = (o6AR + o6GR + o6MR + o6SR + o6YR + o6ZFR)*ga6n;
			
			f6AL = (f6AL+f6AL+f6AL+fabs(avg6L))*0.25; avg6L = f6AL;
			f6FR = (f6FR+f6FR+f6FR+fabs(avg6R))*0.25; avg6R = f6FR;
			//manipulating deep reverb tail for realism
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
		double CBL = (bez[bez_CL]*(1.0-bez[bez_cycle]))+(bez[bez_BL]*bez[bez_cycle]);
		double CBR = (bez[bez_CR]*(1.0-bez[bez_cycle]))+(bez[bez_BR]*bez[bez_cycle]);
		double BAL = (bez[bez_BL]*(1.0-bez[bez_cycle]))+(bez[bez_AL]*bez[bez_cycle]);
		double BAR = (bez[bez_BR]*(1.0-bez[bez_cycle]))+(bez[bez_AR]*bez[bez_cycle]);
		double CBAL = (bez[bez_BL]+(CBL*(1.0-bez[bez_cycle]))+(BAL*bez[bez_cycle]))*0.125;
		double CBAR = (bez[bez_BR]+(CBR*(1.0-bez[bez_cycle]))+(BAR*bez[bez_cycle]))*0.125;
		inputSampleL = CBAL;
		inputSampleR = CBAR;
		
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
