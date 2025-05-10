/* ========================================
 *  kCosmos - kCosmos.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __kCosmos_H
#include "kCosmos.h"
#endif

void kCosmos::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double regenMax = (1.0-pow(1.0-A,3.0))*0.000321;
	//start this but pad it in the loop by volume of output?
	double feedbackSum = 0.0; 
	bool applyCream = (B > 0.9999);
	double remainder = (overallscale-1.0)*0.0375;
	derezA = derezB; derezB = B/overallscale;
	if (applyCream) derezB = 1.0 / ((int)(1.0/derezB));
	else derezB /= (2.0/pow(overallscale,0.5-remainder));
	//this hard-locks derez to exact subdivisions of 1.0
	if (derezB < 0.0005) derezB = 0.0005; if (derezB > 1.0) derezB = 1.0;
	double freq = C*M_PI_2; if (freq < 0.5) freq = 0.5;
	bool applyAvg = (C < 1.0);
	double earlyLoudness = D;
	int adjPredelay = predelay*E*derezB;	
	double wet = F;
	
	double fir[74]; fir[36] = 1.0;
	for(int fip = 0; fip < 36; fip++) {
		fir[fip] = (fip-36)*freq;
		fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
	}
	for(int fip = 37; fip < 72; fip++) {
		fir[fip] = (fip-36)*freq;
		fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
	} //setting up the filter which will run inside DeRez
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;

		double temp = (double)sampleFrames/inFramesToProcess;
		double derez = (derezA*temp)+(derezB*(1.0-temp));
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += ((inputSampleL+bez[bez_InL]) * derez);
		bez[bez_SampR] += ((inputSampleR+bez[bez_InR]) * derez);
		bez[bez_InL] = inputSampleL; bez[bez_InR] = inputSampleR;
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			if (applyCream) bez[bez_cycle] = 0.0;
			else bez[bez_cycle] -= 1.0;
			
			//predelay
			aZL[countZ] = bez[bez_SampL];
			aZR[countZ] = bez[bez_SampR];
			countZ++; if (countZ < 0 || countZ > adjPredelay) countZ = 0;
			bez[bez_SampL] = aZL[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			bez[bez_SampR] = aZR[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			//end predelay
			
			eAL[countAL] = bez[bez_SampL];
			eBL[countBL] = bez[bez_SampL];
			eCL[countCL] = bez[bez_SampL];
			eDL[countDL] = bez[bez_SampL];
			eEL[countEL] = bez[bez_SampL];
			
			eER[countER] = bez[bez_SampR];
			eJR[countJR] = bez[bez_SampR];
			eOR[countOR] = bez[bez_SampR];
			eTR[countTR] = bez[bez_SampR];
			eYR[countYR] = bez[bez_SampR];
			
			
			
			if (firPosition < 0 || firPosition > 32767) firPosition = 32767; int firp = firPosition;		
			firBufferL[firp] = earlyReflectionL; earlyReflectionL = 0.0;
			firBufferR[firp] = earlyReflectionR; earlyReflectionR = 0.0;
			if (firp + 72 < 32767) {
				for(int fip = 1; fip < 72; fip++) {
					earlyReflectionL += firBufferL[firp+fip] * fir[fip];
					earlyReflectionR += firBufferR[firp+fip] * fir[fip];
				}
			} else {
				for(int fip = 1; fip < 72; fip++) {
					earlyReflectionL += firBufferL[firp+fip - ((firp+fip > 32767)?32768:0)] * fir[fip];
					earlyReflectionR += firBufferR[firp+fip - ((firp+fip > 32767)?32768:0)] * fir[fip];
				}
			}
			earlyReflectionL *= 0.25; earlyReflectionR *= 0.25;
			earlyReflectionL *= sqrt(freq); earlyReflectionR *= sqrt(freq);
			firPosition--;//here's the brickwall FIR filter, running in front of the Householder matrix
			
			feedbackSum *= 0.00001;
			feedbackSum += fabs(earlyReflectionL);
			feedbackSum += fabs(earlyReflectionR);
			feedbackSum *= 0.00003;
			
			double regen = fmax(regenMax - feedbackSum, 0.0);
			if (applyAvg) regen *= 0.5;
			else {
				prevAL = prevBL = prevCL = prevDL = prevEL = 0.0;
				prevER = prevJR = prevOR = prevTR = prevYR = 0.0;				
			}
			aAL[countAL] = earlyReflectionL + ((feedbackAL+prevAL) * regen);
			aBL[countBL] = earlyReflectionL + ((feedbackBL+prevBL) * regen);
			aCL[countCL] = earlyReflectionL + ((feedbackCL+prevCL) * regen);
			aDL[countDL] = earlyReflectionL + ((feedbackDL+prevDL) * regen);
			aEL[countEL] = earlyReflectionL + ((feedbackEL+prevEL) * regen);
			
			aER[countER] = earlyReflectionR + ((feedbackER+prevER) * regen);
			aJR[countJR] = earlyReflectionR + ((feedbackJR+prevJR) * regen);
			aOR[countOR] = earlyReflectionR + ((feedbackOR+prevOR) * regen);
			aTR[countTR] = earlyReflectionR + ((feedbackTR+prevTR) * regen);
			aYR[countYR] = earlyReflectionR + ((feedbackYR+prevYR) * regen);
			
			prevAL = feedbackAL;
			prevBL = feedbackBL;
			prevCL = feedbackCL;
			prevDL = feedbackDL;
			prevEL = feedbackEL;
			
			prevER = feedbackER;
			prevJR = feedbackJR;
			prevOR = feedbackOR;
			prevTR = feedbackTR;
			prevYR = feedbackYR;
			
			countAL++; if (countAL < 0 || countAL > delayA) countAL = 0;
			countBL++; if (countBL < 0 || countBL > delayB) countBL = 0;
			countCL++; if (countCL < 0 || countCL > delayC) countCL = 0;
			countDL++; if (countDL < 0 || countDL > delayD) countDL = 0;
			countEL++; if (countEL < 0 || countEL > delayE) countEL = 0;
			
			countER++; if (countER < 0 || countER > delayE) countER = 0;
			countJR++; if (countJR < 0 || countJR > delayJ) countJR = 0;
			countOR++; if (countOR < 0 || countOR > delayO) countOR = 0;
			countTR++; if (countTR < 0 || countTR > delayT) countTR = 0;
			countYR++; if (countYR < 0 || countYR > delayY) countYR = 0;
			
			double earlyAL = eAL[countAL-((countAL > delayA)?delayA+1:0)];
			double earlyBL = eBL[countBL-((countBL > delayB)?delayB+1:0)];
			double earlyCL = eCL[countCL-((countCL > delayC)?delayC+1:0)];
			double earlyDL = eDL[countDL-((countDL > delayD)?delayD+1:0)];
			double earlyEL = eEL[countEL-((countEL > delayE)?delayE+1:0)];
			
			double earlyER = eER[countER-((countER > delayE)?delayE+1:0)];
			double earlyJR = eJR[countJR-((countJR > delayJ)?delayJ+1:0)];
			double earlyOR = eOR[countOR-((countOR > delayO)?delayO+1:0)];
			double earlyTR = eTR[countTR-((countTR > delayT)?delayT+1:0)];
			double earlyYR = eYR[countYR-((countYR > delayY)?delayY+1:0)];
			
			double outAL = aAL[countAL-((countAL > delayA)?delayA+1:0)];
			double outBL = aBL[countBL-((countBL > delayB)?delayB+1:0)];
			double outCL = aCL[countCL-((countCL > delayC)?delayC+1:0)];
			double outDL = aDL[countDL-((countDL > delayD)?delayD+1:0)];
			double outEL = aEL[countEL-((countEL > delayE)?delayE+1:0)];
			
			double outER = aER[countER-((countER > delayE)?delayE+1:0)];
			double outJR = aJR[countJR-((countJR > delayJ)?delayJ+1:0)];
			double outOR = aOR[countOR-((countOR > delayO)?delayO+1:0)];
			double outTR = aTR[countTR-((countTR > delayT)?delayT+1:0)];
			double outYR = aYR[countYR-((countYR > delayY)?delayY+1:0)];
			
			//-------- one
			
			eFL[countFL] = ((earlyAL*3.0) - ((earlyBL + earlyCL + earlyDL + earlyEL)*2.0));
			eGL[countGL] = ((earlyBL*3.0) - ((earlyAL + earlyCL + earlyDL + earlyEL)*2.0));
			eHL[countHL] = ((earlyCL*3.0) - ((earlyAL + earlyBL + earlyDL + earlyEL)*2.0));
			eIL[countIL] = ((earlyDL*3.0) - ((earlyAL + earlyBL + earlyCL + earlyEL)*2.0));
			eJL[countJL] = ((earlyEL*3.0) - ((earlyAL + earlyBL + earlyCL + earlyDL)*2.0));
			
			eDR[countDR] = ((earlyER*3.0) - ((earlyJR + earlyOR + earlyTR + earlyYR)*2.0));
			eIR[countIR] = ((earlyJR*3.0) - ((earlyER + earlyOR + earlyTR + earlyYR)*2.0));
			eNR[countNR] = ((earlyOR*3.0) - ((earlyER + earlyJR + earlyTR + earlyYR)*2.0));
			eSR[countSR] = ((earlyTR*3.0) - ((earlyER + earlyJR + earlyOR + earlyYR)*2.0));
			eXR[countXR] = ((earlyYR*3.0) - ((earlyER + earlyJR + earlyOR + earlyTR)*2.0));
			
			aFL[countFL] = ((outAL*3.0) - ((outBL + outCL + outDL + outEL)*2.0));
			aGL[countGL] = ((outBL*3.0) - ((outAL + outCL + outDL + outEL)*2.0));
			aHL[countHL] = ((outCL*3.0) - ((outAL + outBL + outDL + outEL)*2.0));
			aIL[countIL] = ((outDL*3.0) - ((outAL + outBL + outCL + outEL)*2.0));
			aJL[countJL] = ((outEL*3.0) - ((outAL + outBL + outCL + outDL)*2.0));
			
			aDR[countDR] = ((outER*3.0) - ((outJR + outOR + outTR + outYR)*2.0));
			aIR[countIR] = ((outJR*3.0) - ((outER + outOR + outTR + outYR)*2.0));
			aNR[countNR] = ((outOR*3.0) - ((outER + outJR + outTR + outYR)*2.0));
			aSR[countSR] = ((outTR*3.0) - ((outER + outJR + outOR + outYR)*2.0));
			aXR[countXR] = ((outYR*3.0) - ((outER + outJR + outOR + outTR)*2.0));			
			
			countFL++; if (countFL < 0 || countFL > delayF) countFL = 0;
			countGL++; if (countGL < 0 || countGL > delayG) countGL = 0;
			countHL++; if (countHL < 0 || countHL > delayH) countHL = 0;
			countIL++; if (countIL < 0 || countIL > delayI) countIL = 0;
			countJL++; if (countJL < 0 || countJL > delayJ) countJL = 0;
			
			countDR++; if (countDR < 0 || countDR > delayD) countDR = 0;
			countIR++; if (countIR < 0 || countIR > delayI) countIR = 0;
			countNR++; if (countNR < 0 || countNR > delayN) countNR = 0;
			countSR++; if (countSR < 0 || countSR > delayS) countSR = 0;
			countXR++; if (countXR < 0 || countXR > delayX) countXR = 0;
			
			double earlyFL = eFL[countFL-((countFL > delayF)?delayF+1:0)];
			double earlyGL = eGL[countGL-((countGL > delayG)?delayG+1:0)];
			double earlyHL = eHL[countHL-((countHL > delayH)?delayH+1:0)];
			double earlyIL = eIL[countIL-((countIL > delayI)?delayI+1:0)];
			double earlyJL = eJL[countJL-((countJL > delayJ)?delayJ+1:0)];
			
			double earlyDR = eDR[countDR-((countDR > delayD)?delayD+1:0)];
			double earlyIR = eIR[countIR-((countIR > delayI)?delayI+1:0)];
			double earlyNR = eNR[countNR-((countNR > delayN)?delayN+1:0)];
			double earlySR = eSR[countSR-((countSR > delayS)?delayS+1:0)];
			double earlyXR = eXR[countXR-((countXR > delayX)?delayX+1:0)];
			
			double outFL = aFL[countFL-((countFL > delayF)?delayF+1:0)];
			double outGL = aGL[countGL-((countGL > delayG)?delayG+1:0)];
			double outHL = aHL[countHL-((countHL > delayH)?delayH+1:0)];
			double outIL = aIL[countIL-((countIL > delayI)?delayI+1:0)];
			double outJL = aJL[countJL-((countJL > delayJ)?delayJ+1:0)];
			
			double outDR = aDR[countDR-((countDR > delayD)?delayD+1:0)];
			double outIR = aIR[countIR-((countIR > delayI)?delayI+1:0)];
			double outNR = aNR[countNR-((countNR > delayN)?delayN+1:0)];
			double outSR = aSR[countSR-((countSR > delayS)?delayS+1:0)];
			double outXR = aXR[countXR-((countXR > delayX)?delayX+1:0)];
			
			//-------- two
			
			eKL[countKL] = ((earlyFL*3.0) - ((earlyGL + earlyHL + earlyIL + earlyJL)*2.0));
			eLL[countLL] = ((earlyGL*3.0) - ((earlyFL + earlyHL + earlyIL + earlyJL)*2.0));
			eML[countML] = ((earlyHL*3.0) - ((earlyFL + earlyGL + earlyIL + earlyJL)*2.0));
			eNL[countNL] = ((earlyIL*3.0) - ((earlyFL + earlyGL + earlyHL + earlyJL)*2.0));
			eOL[countOL] = ((earlyJL*3.0) - ((earlyFL + earlyGL + earlyHL + earlyIL)*2.0));
			
			eCR[countCR] = ((earlyDR*3.0) - ((earlyIR + earlyNR + earlySR + earlyXR)*2.0));
			eHR[countHR] = ((earlyIR*3.0) - ((earlyDR + earlyNR + earlySR + earlyXR)*2.0));
			eMR[countMR] = ((earlyNR*3.0) - ((earlyDR + earlyIR + earlySR + earlyXR)*2.0));
			eRR[countRR] = ((earlySR*3.0) - ((earlyDR + earlyIR + earlyNR + earlyXR)*2.0));
			eWR[countWR] = ((earlyXR*3.0) - ((earlyDR + earlyIR + earlyNR + earlySR)*2.0));
			
			aKL[countKL] = ((outFL*3.0) - ((outGL + outHL + outIL + outJL)*2.0));
			aLL[countLL] = ((outGL*3.0) - ((outFL + outHL + outIL + outJL)*2.0));
			aML[countML] = ((outHL*3.0) - ((outFL + outGL + outIL + outJL)*2.0));
			aNL[countNL] = ((outIL*3.0) - ((outFL + outGL + outHL + outJL)*2.0));
			aOL[countOL] = ((outJL*3.0) - ((outFL + outGL + outHL + outIL)*2.0));
			
			aCR[countCR] = ((outDR*3.0) - ((outIR + outNR + outSR + outXR)*2.0));
			aHR[countHR] = ((outIR*3.0) - ((outDR + outNR + outSR + outXR)*2.0));
			aMR[countMR] = ((outNR*3.0) - ((outDR + outIR + outSR + outXR)*2.0));
			aRR[countRR] = ((outSR*3.0) - ((outDR + outIR + outNR + outXR)*2.0));
			aWR[countWR] = ((outXR*3.0) - ((outDR + outIR + outNR + outSR)*2.0));
			
			countKL++; if (countKL < 0 || countKL > delayK) countKL = 0;
			countLL++; if (countLL < 0 || countLL > delayL) countLL = 0;
			countML++; if (countML < 0 || countML > delayM) countML = 0;
			countNL++; if (countNL < 0 || countNL > delayN) countNL = 0;
			countOL++; if (countOL < 0 || countOL > delayO) countOL = 0;
			
			countCR++; if (countCR < 0 || countCR > delayC) countCR = 0;
			countHR++; if (countHR < 0 || countHR > delayH) countHR = 0;
			countMR++; if (countMR < 0 || countMR > delayM) countMR = 0;
			countRR++; if (countRR < 0 || countRR > delayR) countRR = 0;
			countWR++; if (countWR < 0 || countWR > delayW) countWR = 0;
			
			double earlyKL = eKL[countKL-((countKL > delayK)?delayK+1:0)];
			double earlyLL = eLL[countLL-((countLL > delayL)?delayL+1:0)];
			double earlyML = eML[countML-((countML > delayM)?delayM+1:0)];
			double earlyNL = eNL[countNL-((countNL > delayN)?delayN+1:0)];
			double earlyOL = eOL[countOL-((countOL > delayO)?delayO+1:0)];
			
			double earlyCR = eCR[countCR-((countCR > delayC)?delayC+1:0)];
			double earlyHR = eHR[countHR-((countHR > delayH)?delayH+1:0)];
			double earlyMR = eMR[countMR-((countMR > delayM)?delayM+1:0)];
			double earlyRR = eRR[countRR-((countRR > delayR)?delayR+1:0)];
			double earlyWR = eWR[countWR-((countWR > delayW)?delayW+1:0)];
			
			double outKL = aKL[countKL-((countKL > delayK)?delayK+1:0)];
			double outLL = aLL[countLL-((countLL > delayL)?delayL+1:0)];
			double outML = aML[countML-((countML > delayM)?delayM+1:0)];
			double outNL = aNL[countNL-((countNL > delayN)?delayN+1:0)];
			double outOL = aOL[countOL-((countOL > delayO)?delayO+1:0)];
			
			double outCR = aCR[countCR-((countCR > delayC)?delayC+1:0)];
			double outHR = aHR[countHR-((countHR > delayH)?delayH+1:0)];
			double outMR = aMR[countMR-((countMR > delayM)?delayM+1:0)];
			double outRR = aRR[countRR-((countRR > delayR)?delayR+1:0)];
			double outWR = aWR[countWR-((countWR > delayW)?delayW+1:0)];
			
			//-------- three
			
			ePL[countPL] = ((earlyKL*3.0) - ((earlyLL + earlyML + earlyNL + earlyOL)*2.0));
			eQL[countQL] = ((earlyLL*3.0) - ((earlyKL + earlyML + earlyNL + earlyOL)*2.0));
			eRL[countRL] = ((earlyML*3.0) - ((earlyKL + earlyLL + earlyNL + earlyOL)*2.0));
			eSL[countSL] = ((earlyNL*3.0) - ((earlyKL + earlyLL + earlyML + earlyOL)*2.0));
			eTL[countTL] = ((earlyOL*3.0) - ((earlyKL + earlyLL + earlyML + earlyNL)*2.0));
			
			eBR[countBR] = ((earlyCR*3.0) - ((earlyHR + earlyMR + earlyRR + earlyWR)*2.0));
			eGR[countGR] = ((earlyHR*3.0) - ((earlyCR + earlyMR + earlyRR + earlyWR)*2.0));
			eLR[countLR] = ((earlyMR*3.0) - ((earlyCR + earlyHR + earlyRR + earlyWR)*2.0));
			eQR[countQR] = ((earlyRR*3.0) - ((earlyCR + earlyHR + earlyMR + earlyWR)*2.0));
			eVR[countVR] = ((earlyWR*3.0) - ((earlyCR + earlyHR + earlyMR + earlyRR)*2.0));
			
			aPL[countPL] = ((outKL*3.0) - ((outLL + outML + outNL + outOL)*2.0));
			aQL[countQL] = ((outLL*3.0) - ((outKL + outML + outNL + outOL)*2.0));
			aRL[countRL] = ((outML*3.0) - ((outKL + outLL + outNL + outOL)*2.0));
			aSL[countSL] = ((outNL*3.0) - ((outKL + outLL + outML + outOL)*2.0));
			aTL[countTL] = ((outOL*3.0) - ((outKL + outLL + outML + outNL)*2.0));
			
			aBR[countBR] = ((outCR*3.0) - ((outHR + outMR + outRR + outWR)*2.0));
			aGR[countGR] = ((outHR*3.0) - ((outCR + outMR + outRR + outWR)*2.0));
			aLR[countLR] = ((outMR*3.0) - ((outCR + outHR + outRR + outWR)*2.0));
			aQR[countQR] = ((outRR*3.0) - ((outCR + outHR + outMR + outWR)*2.0));
			aVR[countVR] = ((outWR*3.0) - ((outCR + outHR + outMR + outRR)*2.0));
			
			countPL++; if (countPL < 0 || countPL > delayP) countPL = 0;
			countQL++; if (countQL < 0 || countQL > delayQ) countQL = 0;
			countRL++; if (countRL < 0 || countRL > delayR) countRL = 0;
			countSL++; if (countSL < 0 || countSL > delayS) countSL = 0;
			countTL++; if (countTL < 0 || countTL > delayT) countTL = 0;
			
			countBR++; if (countBR < 0 || countBR > delayB) countBR = 0;
			countGR++; if (countGR < 0 || countGR > delayG) countGR = 0;
			countLR++; if (countLR < 0 || countLR > delayL) countLR = 0;
			countQR++; if (countQR < 0 || countQR > delayQ) countQR = 0;
			countVR++; if (countVR < 0 || countVR > delayV) countVR = 0;
			
			double earlyPL = ePL[countPL-((countPL > delayP)?delayP+1:0)];
			double earlyQL = eQL[countQL-((countQL > delayQ)?delayQ+1:0)];
			double earlyRL = eRL[countRL-((countRL > delayR)?delayR+1:0)];
			double earlySL = eSL[countSL-((countSL > delayS)?delayS+1:0)];
			double earlyTL = eTL[countTL-((countTL > delayT)?delayT+1:0)];
			
			double earlyBR = eBR[countBR-((countBR > delayB)?delayB+1:0)];
			double earlyGR = eGR[countGR-((countGR > delayG)?delayG+1:0)];
			double earlyLR = eLR[countLR-((countLR > delayL)?delayL+1:0)];
			double earlyQR = eQR[countQR-((countQR > delayQ)?delayQ+1:0)];
			double earlyVR = eVR[countVR-((countVR > delayV)?delayV+1:0)];
			
			double outPL = aPL[countPL-((countPL > delayP)?delayP+1:0)];
			double outQL = aQL[countQL-((countQL > delayQ)?delayQ+1:0)];
			double outRL = aRL[countRL-((countRL > delayR)?delayR+1:0)];
			double outSL = aSL[countSL-((countSL > delayS)?delayS+1:0)];
			double outTL = aTL[countTL-((countTL > delayT)?delayT+1:0)];
			
			double outBR = aBR[countBR-((countBR > delayB)?delayB+1:0)];
			double outGR = aGR[countGR-((countGR > delayG)?delayG+1:0)];
			double outLR = aLR[countLR-((countLR > delayL)?delayL+1:0)];
			double outQR = aQR[countQR-((countQR > delayQ)?delayQ+1:0)];
			double outVR = aVR[countVR-((countVR > delayV)?delayV+1:0)];
			
			//-------- four
			
			eUL[countUL] = ((earlyPL*3.0) - ((earlyQL + earlyRL + earlySL + earlyTL)*2.0));
			eVL[countVL] = ((earlyQL*3.0) - ((earlyPL + earlyRL + earlySL + earlyTL)*2.0));
			eWL[countWL] = ((earlyRL*3.0) - ((earlyPL + earlyQL + earlySL + earlyTL)*2.0));
			eXL[countXL] = ((earlySL*3.0) - ((earlyPL + earlyQL + earlyRL + earlyTL)*2.0));
			eYL[countYL] = ((earlyTL*3.0) - ((earlyPL + earlyQL + earlyRL + earlySL)*2.0));
			
			eAR[countAR] = ((earlyBR*3.0) - ((earlyGR + earlyLR + earlyQR + earlyVR)*2.0));
			eFR[countFR] = ((earlyGR*3.0) - ((earlyBR + earlyLR + earlyQR + earlyVR)*2.0));
			eKR[countKR] = ((earlyLR*3.0) - ((earlyBR + earlyGR + earlyQR + earlyVR)*2.0));
			ePR[countPR] = ((earlyQR*3.0) - ((earlyBR + earlyGR + earlyLR + earlyVR)*2.0));
			eUR[countUR] = ((earlyVR*3.0) - ((earlyBR + earlyGR + earlyLR + earlyQR)*2.0));
			
			aUL[countUL] = ((outPL*3.0) - ((outQL + outRL + outSL + outTL)*2.0));
			aVL[countVL] = ((outQL*3.0) - ((outPL + outRL + outSL + outTL)*2.0));
			aWL[countWL] = ((outRL*3.0) - ((outPL + outQL + outSL + outTL)*2.0));
			aXL[countXL] = ((outSL*3.0) - ((outPL + outQL + outRL + outTL)*2.0));
			aYL[countYL] = ((outTL*3.0) - ((outPL + outQL + outRL + outSL)*2.0));
			
			aAR[countAR] = ((outBR*3.0) - ((outGR + outLR + outQR + outVR)*2.0));
			aFR[countFR] = ((outGR*3.0) - ((outBR + outLR + outQR + outVR)*2.0));
			aKR[countKR] = ((outLR*3.0) - ((outBR + outGR + outQR + outVR)*2.0));
			aPR[countPR] = ((outQR*3.0) - ((outBR + outGR + outLR + outVR)*2.0));
			aUR[countUR] = ((outVR*3.0) - ((outBR + outGR + outLR + outQR)*2.0));
			
			countUL++; if (countUL < 0 || countUL > delayU) countUL = 0;
			countVL++; if (countVL < 0 || countVL > delayV) countVL = 0;
			countWL++; if (countWL < 0 || countWL > delayW) countWL = 0;
			countXL++; if (countXL < 0 || countXL > delayX) countXL = 0;
			countYL++; if (countYL < 0 || countYL > delayY) countYL = 0;
			
			countAR++; if (countAR < 0 || countAR > delayA) countAR = 0;
			countFR++; if (countFR < 0 || countFR > delayF) countFR = 0;
			countKR++; if (countKR < 0 || countKR > delayK) countKR = 0;
			countPR++; if (countPR < 0 || countPR > delayP) countPR = 0;
			countUR++; if (countUR < 0 || countUR > delayU) countUR = 0;
			
			double earlyUL = eUL[countUL-((countUL > delayU)?delayU+1:0)];
			double earlyVL = eVL[countVL-((countVL > delayV)?delayV+1:0)];
			double earlyWL = eWL[countWL-((countWL > delayW)?delayW+1:0)];
			double earlyXL = eXL[countXL-((countXL > delayX)?delayX+1:0)];
			double earlyYL = eYL[countYL-((countYL > delayY)?delayY+1:0)];
			
			double earlyAR = eAR[countAR-((countAR > delayA)?delayA+1:0)];
			double earlyFR = eFR[countFR-((countFR > delayF)?delayF+1:0)];
			double earlyKR = eKR[countKR-((countKR > delayK)?delayK+1:0)];
			double earlyPR = ePR[countPR-((countPR > delayP)?delayP+1:0)];
			double earlyUR = eUR[countUR-((countUR > delayU)?delayU+1:0)];
			
			double outUL = aUL[countUL-((countUL > delayU)?delayU+1:0)];
			double outVL = aVL[countVL-((countVL > delayV)?delayV+1:0)];
			double outWL = aWL[countWL-((countWL > delayW)?delayW+1:0)];
			double outXL = aXL[countXL-((countXL > delayX)?delayX+1:0)];
			double outYL = aYL[countYL-((countYL > delayY)?delayY+1:0)];
			
			double outAR = aAR[countAR-((countAR > delayA)?delayA+1:0)];
			double outFR = aFR[countFR-((countFR > delayF)?delayF+1:0)];
			double outKR = aKR[countKR-((countKR > delayK)?delayK+1:0)];
			double outPR = aPR[countPR-((countPR > delayP)?delayP+1:0)];
			double outUR = aUR[countUR-((countUR > delayU)?delayU+1:0)];
			
			//-------- five
			
			earlyReflectionL = (earlyUL + earlyVL + earlyWL + earlyXL + earlyYL)*0.0008;
			earlyReflectionR = (earlyAR + earlyFR + earlyKR + earlyPR + earlyUR)*0.0008;
			//and take the final combined sum of outputs, corrected for Householder gain
			
			feedbackAL = ((outAR*3.0) - ((outFR + outKR + outPR + outUR)*2.0)); feedbackSum = fabs(feedbackAL);
			feedbackER = ((outUL*3.0) - ((outVL + outWL + outXL + outYL)*2.0)); feedbackSum += fabs(feedbackER);
			feedbackBL = ((outVL*3.0) - ((outUL + outWL + outXL + outYL)*2.0)); feedbackSum += fabs(feedbackBL);
			feedbackJR = ((outFR*3.0) - ((outAR + outKR + outPR + outUR)*2.0)); feedbackSum += fabs(feedbackJR);
			feedbackCL = ((outWL*3.0) - ((outUL + outVL + outXL + outYL)*2.0)); feedbackSum += fabs(feedbackCL);
			feedbackOR = ((outKR*3.0) - ((outAR + outFR + outPR + outUR)*2.0)); feedbackSum += fabs(feedbackOR);
			feedbackDL = ((outXL*3.0) - ((outUL + outVL + outWL + outYL)*2.0)); feedbackSum += fabs(feedbackDL);
			feedbackTR = ((outPR*3.0) - ((outAR + outFR + outKR + outUR)*2.0)); feedbackSum += fabs(feedbackTR);
			feedbackEL = ((outYL*3.0) - ((outUL + outVL + outWL + outXL)*2.0)); feedbackSum += fabs(feedbackEL);
			feedbackYR = ((outUR*3.0) - ((outAR + outFR + outKR + outPR)*2.0)); feedbackSum += fabs(feedbackYR);
			
			inputSampleL = (outUL + outVL + outWL + outXL + outYL)*0.0008;
			inputSampleR = (outAR + outFR + outKR + outPR + outUR)*0.0008;
			//and take the final combined sum of outputs, corrected for Householder gain			
			
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
		double CBAL = (bez[bez_BL]+(CBL*(1.0-bez[bez_cycle]))+(BAL*bez[bez_cycle]))*0.125;
		double CBAR = (bez[bez_BR]+(CBR*(1.0-bez[bez_cycle]))+(BAR*bez[bez_cycle]))*0.125;
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

void kCosmos::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double regenMax = (1.0-pow(1.0-A,3.0))*0.000321;
	//start this but pad it in the loop by volume of output?
	double feedbackSum = 0.0; 
	bool applyCream = (B > 0.9999);
	double remainder = (overallscale-1.0)*0.0375;
	derezA = derezB; derezB = B/overallscale;
	if (applyCream) derezB = 1.0 / ((int)(1.0/derezB));
	else derezB /= (2.0/pow(overallscale,0.5-remainder));
	//this hard-locks derez to exact subdivisions of 1.0
	if (derezB < 0.0005) derezB = 0.0005; if (derezB > 1.0) derezB = 1.0;
	double freq = C*M_PI_2; if (freq < 0.5) freq = 0.5;
	bool applyAvg = (C < 1.0);
	double earlyLoudness = D;
	int adjPredelay = predelay*E*derezB;	
	double wet = F;
	
	double fir[74]; fir[36] = 1.0;
	for(int fip = 0; fip < 36; fip++) {
		fir[fip] = (fip-36)*freq;
		fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
	}
	for(int fip = 37; fip < 72; fip++) {
		fir[fip] = (fip-36)*freq;
		fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
	} //setting up the filter which will run inside DeRez
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;

		double temp = (double)sampleFrames/inFramesToProcess;
		double derez = (derezA*temp)+(derezB*(1.0-temp));
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += ((inputSampleL+bez[bez_InL]) * derez);
		bez[bez_SampR] += ((inputSampleR+bez[bez_InR]) * derez);
		bez[bez_InL] = inputSampleL; bez[bez_InR] = inputSampleR;
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			if (applyCream) bez[bez_cycle] = 0.0;
			else bez[bez_cycle] -= 1.0;
			
			//predelay
			aZL[countZ] = bez[bez_SampL];
			aZR[countZ] = bez[bez_SampR];
			countZ++; if (countZ < 0 || countZ > adjPredelay) countZ = 0;
			bez[bez_SampL] = aZL[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			bez[bez_SampR] = aZR[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			//end predelay
			
			eAL[countAL] = bez[bez_SampL];
			eBL[countBL] = bez[bez_SampL];
			eCL[countCL] = bez[bez_SampL];
			eDL[countDL] = bez[bez_SampL];
			eEL[countEL] = bez[bez_SampL];
			
			eER[countER] = bez[bez_SampR];
			eJR[countJR] = bez[bez_SampR];
			eOR[countOR] = bez[bez_SampR];
			eTR[countTR] = bez[bez_SampR];
			eYR[countYR] = bez[bez_SampR];
			
			
			
			if (firPosition < 0 || firPosition > 32767) firPosition = 32767; int firp = firPosition;		
			firBufferL[firp] = earlyReflectionL; earlyReflectionL = 0.0;
			firBufferR[firp] = earlyReflectionR; earlyReflectionR = 0.0;
			if (firp + 72 < 32767) {
				for(int fip = 1; fip < 72; fip++) {
					earlyReflectionL += firBufferL[firp+fip] * fir[fip];
					earlyReflectionR += firBufferR[firp+fip] * fir[fip];
				}
			} else {
				for(int fip = 1; fip < 72; fip++) {
					earlyReflectionL += firBufferL[firp+fip - ((firp+fip > 32767)?32768:0)] * fir[fip];
					earlyReflectionR += firBufferR[firp+fip - ((firp+fip > 32767)?32768:0)] * fir[fip];
				}
			}
			earlyReflectionL *= 0.25; earlyReflectionR *= 0.25;
			earlyReflectionL *= sqrt(freq); earlyReflectionR *= sqrt(freq);
			firPosition--;//here's the brickwall FIR filter, running in front of the Householder matrix
			
			feedbackSum *= 0.00001;
			feedbackSum += fabs(earlyReflectionL);
			feedbackSum += fabs(earlyReflectionR);
			feedbackSum *= 0.00003;
			
			double regen = fmax(regenMax - feedbackSum, 0.0);
			if (applyAvg) regen *= 0.5;
			else {
				prevAL = prevBL = prevCL = prevDL = prevEL = 0.0;
				prevER = prevJR = prevOR = prevTR = prevYR = 0.0;				
			}
			aAL[countAL] = earlyReflectionL + ((feedbackAL+prevAL) * regen);
			aBL[countBL] = earlyReflectionL + ((feedbackBL+prevBL) * regen);
			aCL[countCL] = earlyReflectionL + ((feedbackCL+prevCL) * regen);
			aDL[countDL] = earlyReflectionL + ((feedbackDL+prevDL) * regen);
			aEL[countEL] = earlyReflectionL + ((feedbackEL+prevEL) * regen);
			
			aER[countER] = earlyReflectionR + ((feedbackER+prevER) * regen);
			aJR[countJR] = earlyReflectionR + ((feedbackJR+prevJR) * regen);
			aOR[countOR] = earlyReflectionR + ((feedbackOR+prevOR) * regen);
			aTR[countTR] = earlyReflectionR + ((feedbackTR+prevTR) * regen);
			aYR[countYR] = earlyReflectionR + ((feedbackYR+prevYR) * regen);
			
			prevAL = feedbackAL;
			prevBL = feedbackBL;
			prevCL = feedbackCL;
			prevDL = feedbackDL;
			prevEL = feedbackEL;
			
			prevER = feedbackER;
			prevJR = feedbackJR;
			prevOR = feedbackOR;
			prevTR = feedbackTR;
			prevYR = feedbackYR;
			
			countAL++; if (countAL < 0 || countAL > delayA) countAL = 0;
			countBL++; if (countBL < 0 || countBL > delayB) countBL = 0;
			countCL++; if (countCL < 0 || countCL > delayC) countCL = 0;
			countDL++; if (countDL < 0 || countDL > delayD) countDL = 0;
			countEL++; if (countEL < 0 || countEL > delayE) countEL = 0;
			
			countER++; if (countER < 0 || countER > delayE) countER = 0;
			countJR++; if (countJR < 0 || countJR > delayJ) countJR = 0;
			countOR++; if (countOR < 0 || countOR > delayO) countOR = 0;
			countTR++; if (countTR < 0 || countTR > delayT) countTR = 0;
			countYR++; if (countYR < 0 || countYR > delayY) countYR = 0;
			
			double earlyAL = eAL[countAL-((countAL > delayA)?delayA+1:0)];
			double earlyBL = eBL[countBL-((countBL > delayB)?delayB+1:0)];
			double earlyCL = eCL[countCL-((countCL > delayC)?delayC+1:0)];
			double earlyDL = eDL[countDL-((countDL > delayD)?delayD+1:0)];
			double earlyEL = eEL[countEL-((countEL > delayE)?delayE+1:0)];
			
			double earlyER = eER[countER-((countER > delayE)?delayE+1:0)];
			double earlyJR = eJR[countJR-((countJR > delayJ)?delayJ+1:0)];
			double earlyOR = eOR[countOR-((countOR > delayO)?delayO+1:0)];
			double earlyTR = eTR[countTR-((countTR > delayT)?delayT+1:0)];
			double earlyYR = eYR[countYR-((countYR > delayY)?delayY+1:0)];
			
			double outAL = aAL[countAL-((countAL > delayA)?delayA+1:0)];
			double outBL = aBL[countBL-((countBL > delayB)?delayB+1:0)];
			double outCL = aCL[countCL-((countCL > delayC)?delayC+1:0)];
			double outDL = aDL[countDL-((countDL > delayD)?delayD+1:0)];
			double outEL = aEL[countEL-((countEL > delayE)?delayE+1:0)];
			
			double outER = aER[countER-((countER > delayE)?delayE+1:0)];
			double outJR = aJR[countJR-((countJR > delayJ)?delayJ+1:0)];
			double outOR = aOR[countOR-((countOR > delayO)?delayO+1:0)];
			double outTR = aTR[countTR-((countTR > delayT)?delayT+1:0)];
			double outYR = aYR[countYR-((countYR > delayY)?delayY+1:0)];
			
			//-------- one
			
			eFL[countFL] = ((earlyAL*3.0) - ((earlyBL + earlyCL + earlyDL + earlyEL)*2.0));
			eGL[countGL] = ((earlyBL*3.0) - ((earlyAL + earlyCL + earlyDL + earlyEL)*2.0));
			eHL[countHL] = ((earlyCL*3.0) - ((earlyAL + earlyBL + earlyDL + earlyEL)*2.0));
			eIL[countIL] = ((earlyDL*3.0) - ((earlyAL + earlyBL + earlyCL + earlyEL)*2.0));
			eJL[countJL] = ((earlyEL*3.0) - ((earlyAL + earlyBL + earlyCL + earlyDL)*2.0));
			
			eDR[countDR] = ((earlyER*3.0) - ((earlyJR + earlyOR + earlyTR + earlyYR)*2.0));
			eIR[countIR] = ((earlyJR*3.0) - ((earlyER + earlyOR + earlyTR + earlyYR)*2.0));
			eNR[countNR] = ((earlyOR*3.0) - ((earlyER + earlyJR + earlyTR + earlyYR)*2.0));
			eSR[countSR] = ((earlyTR*3.0) - ((earlyER + earlyJR + earlyOR + earlyYR)*2.0));
			eXR[countXR] = ((earlyYR*3.0) - ((earlyER + earlyJR + earlyOR + earlyTR)*2.0));
			
			aFL[countFL] = ((outAL*3.0) - ((outBL + outCL + outDL + outEL)*2.0));
			aGL[countGL] = ((outBL*3.0) - ((outAL + outCL + outDL + outEL)*2.0));
			aHL[countHL] = ((outCL*3.0) - ((outAL + outBL + outDL + outEL)*2.0));
			aIL[countIL] = ((outDL*3.0) - ((outAL + outBL + outCL + outEL)*2.0));
			aJL[countJL] = ((outEL*3.0) - ((outAL + outBL + outCL + outDL)*2.0));
			
			aDR[countDR] = ((outER*3.0) - ((outJR + outOR + outTR + outYR)*2.0));
			aIR[countIR] = ((outJR*3.0) - ((outER + outOR + outTR + outYR)*2.0));
			aNR[countNR] = ((outOR*3.0) - ((outER + outJR + outTR + outYR)*2.0));
			aSR[countSR] = ((outTR*3.0) - ((outER + outJR + outOR + outYR)*2.0));
			aXR[countXR] = ((outYR*3.0) - ((outER + outJR + outOR + outTR)*2.0));			
			
			countFL++; if (countFL < 0 || countFL > delayF) countFL = 0;
			countGL++; if (countGL < 0 || countGL > delayG) countGL = 0;
			countHL++; if (countHL < 0 || countHL > delayH) countHL = 0;
			countIL++; if (countIL < 0 || countIL > delayI) countIL = 0;
			countJL++; if (countJL < 0 || countJL > delayJ) countJL = 0;
			
			countDR++; if (countDR < 0 || countDR > delayD) countDR = 0;
			countIR++; if (countIR < 0 || countIR > delayI) countIR = 0;
			countNR++; if (countNR < 0 || countNR > delayN) countNR = 0;
			countSR++; if (countSR < 0 || countSR > delayS) countSR = 0;
			countXR++; if (countXR < 0 || countXR > delayX) countXR = 0;
			
			double earlyFL = eFL[countFL-((countFL > delayF)?delayF+1:0)];
			double earlyGL = eGL[countGL-((countGL > delayG)?delayG+1:0)];
			double earlyHL = eHL[countHL-((countHL > delayH)?delayH+1:0)];
			double earlyIL = eIL[countIL-((countIL > delayI)?delayI+1:0)];
			double earlyJL = eJL[countJL-((countJL > delayJ)?delayJ+1:0)];
			
			double earlyDR = eDR[countDR-((countDR > delayD)?delayD+1:0)];
			double earlyIR = eIR[countIR-((countIR > delayI)?delayI+1:0)];
			double earlyNR = eNR[countNR-((countNR > delayN)?delayN+1:0)];
			double earlySR = eSR[countSR-((countSR > delayS)?delayS+1:0)];
			double earlyXR = eXR[countXR-((countXR > delayX)?delayX+1:0)];
			
			double outFL = aFL[countFL-((countFL > delayF)?delayF+1:0)];
			double outGL = aGL[countGL-((countGL > delayG)?delayG+1:0)];
			double outHL = aHL[countHL-((countHL > delayH)?delayH+1:0)];
			double outIL = aIL[countIL-((countIL > delayI)?delayI+1:0)];
			double outJL = aJL[countJL-((countJL > delayJ)?delayJ+1:0)];
			
			double outDR = aDR[countDR-((countDR > delayD)?delayD+1:0)];
			double outIR = aIR[countIR-((countIR > delayI)?delayI+1:0)];
			double outNR = aNR[countNR-((countNR > delayN)?delayN+1:0)];
			double outSR = aSR[countSR-((countSR > delayS)?delayS+1:0)];
			double outXR = aXR[countXR-((countXR > delayX)?delayX+1:0)];
			
			//-------- two
			
			eKL[countKL] = ((earlyFL*3.0) - ((earlyGL + earlyHL + earlyIL + earlyJL)*2.0));
			eLL[countLL] = ((earlyGL*3.0) - ((earlyFL + earlyHL + earlyIL + earlyJL)*2.0));
			eML[countML] = ((earlyHL*3.0) - ((earlyFL + earlyGL + earlyIL + earlyJL)*2.0));
			eNL[countNL] = ((earlyIL*3.0) - ((earlyFL + earlyGL + earlyHL + earlyJL)*2.0));
			eOL[countOL] = ((earlyJL*3.0) - ((earlyFL + earlyGL + earlyHL + earlyIL)*2.0));
			
			eCR[countCR] = ((earlyDR*3.0) - ((earlyIR + earlyNR + earlySR + earlyXR)*2.0));
			eHR[countHR] = ((earlyIR*3.0) - ((earlyDR + earlyNR + earlySR + earlyXR)*2.0));
			eMR[countMR] = ((earlyNR*3.0) - ((earlyDR + earlyIR + earlySR + earlyXR)*2.0));
			eRR[countRR] = ((earlySR*3.0) - ((earlyDR + earlyIR + earlyNR + earlyXR)*2.0));
			eWR[countWR] = ((earlyXR*3.0) - ((earlyDR + earlyIR + earlyNR + earlySR)*2.0));
			
			aKL[countKL] = ((outFL*3.0) - ((outGL + outHL + outIL + outJL)*2.0));
			aLL[countLL] = ((outGL*3.0) - ((outFL + outHL + outIL + outJL)*2.0));
			aML[countML] = ((outHL*3.0) - ((outFL + outGL + outIL + outJL)*2.0));
			aNL[countNL] = ((outIL*3.0) - ((outFL + outGL + outHL + outJL)*2.0));
			aOL[countOL] = ((outJL*3.0) - ((outFL + outGL + outHL + outIL)*2.0));
			
			aCR[countCR] = ((outDR*3.0) - ((outIR + outNR + outSR + outXR)*2.0));
			aHR[countHR] = ((outIR*3.0) - ((outDR + outNR + outSR + outXR)*2.0));
			aMR[countMR] = ((outNR*3.0) - ((outDR + outIR + outSR + outXR)*2.0));
			aRR[countRR] = ((outSR*3.0) - ((outDR + outIR + outNR + outXR)*2.0));
			aWR[countWR] = ((outXR*3.0) - ((outDR + outIR + outNR + outSR)*2.0));
			
			countKL++; if (countKL < 0 || countKL > delayK) countKL = 0;
			countLL++; if (countLL < 0 || countLL > delayL) countLL = 0;
			countML++; if (countML < 0 || countML > delayM) countML = 0;
			countNL++; if (countNL < 0 || countNL > delayN) countNL = 0;
			countOL++; if (countOL < 0 || countOL > delayO) countOL = 0;
			
			countCR++; if (countCR < 0 || countCR > delayC) countCR = 0;
			countHR++; if (countHR < 0 || countHR > delayH) countHR = 0;
			countMR++; if (countMR < 0 || countMR > delayM) countMR = 0;
			countRR++; if (countRR < 0 || countRR > delayR) countRR = 0;
			countWR++; if (countWR < 0 || countWR > delayW) countWR = 0;
			
			double earlyKL = eKL[countKL-((countKL > delayK)?delayK+1:0)];
			double earlyLL = eLL[countLL-((countLL > delayL)?delayL+1:0)];
			double earlyML = eML[countML-((countML > delayM)?delayM+1:0)];
			double earlyNL = eNL[countNL-((countNL > delayN)?delayN+1:0)];
			double earlyOL = eOL[countOL-((countOL > delayO)?delayO+1:0)];
			
			double earlyCR = eCR[countCR-((countCR > delayC)?delayC+1:0)];
			double earlyHR = eHR[countHR-((countHR > delayH)?delayH+1:0)];
			double earlyMR = eMR[countMR-((countMR > delayM)?delayM+1:0)];
			double earlyRR = eRR[countRR-((countRR > delayR)?delayR+1:0)];
			double earlyWR = eWR[countWR-((countWR > delayW)?delayW+1:0)];
			
			double outKL = aKL[countKL-((countKL > delayK)?delayK+1:0)];
			double outLL = aLL[countLL-((countLL > delayL)?delayL+1:0)];
			double outML = aML[countML-((countML > delayM)?delayM+1:0)];
			double outNL = aNL[countNL-((countNL > delayN)?delayN+1:0)];
			double outOL = aOL[countOL-((countOL > delayO)?delayO+1:0)];
			
			double outCR = aCR[countCR-((countCR > delayC)?delayC+1:0)];
			double outHR = aHR[countHR-((countHR > delayH)?delayH+1:0)];
			double outMR = aMR[countMR-((countMR > delayM)?delayM+1:0)];
			double outRR = aRR[countRR-((countRR > delayR)?delayR+1:0)];
			double outWR = aWR[countWR-((countWR > delayW)?delayW+1:0)];
			
			//-------- three
			
			ePL[countPL] = ((earlyKL*3.0) - ((earlyLL + earlyML + earlyNL + earlyOL)*2.0));
			eQL[countQL] = ((earlyLL*3.0) - ((earlyKL + earlyML + earlyNL + earlyOL)*2.0));
			eRL[countRL] = ((earlyML*3.0) - ((earlyKL + earlyLL + earlyNL + earlyOL)*2.0));
			eSL[countSL] = ((earlyNL*3.0) - ((earlyKL + earlyLL + earlyML + earlyOL)*2.0));
			eTL[countTL] = ((earlyOL*3.0) - ((earlyKL + earlyLL + earlyML + earlyNL)*2.0));
			
			eBR[countBR] = ((earlyCR*3.0) - ((earlyHR + earlyMR + earlyRR + earlyWR)*2.0));
			eGR[countGR] = ((earlyHR*3.0) - ((earlyCR + earlyMR + earlyRR + earlyWR)*2.0));
			eLR[countLR] = ((earlyMR*3.0) - ((earlyCR + earlyHR + earlyRR + earlyWR)*2.0));
			eQR[countQR] = ((earlyRR*3.0) - ((earlyCR + earlyHR + earlyMR + earlyWR)*2.0));
			eVR[countVR] = ((earlyWR*3.0) - ((earlyCR + earlyHR + earlyMR + earlyRR)*2.0));
			
			aPL[countPL] = ((outKL*3.0) - ((outLL + outML + outNL + outOL)*2.0));
			aQL[countQL] = ((outLL*3.0) - ((outKL + outML + outNL + outOL)*2.0));
			aRL[countRL] = ((outML*3.0) - ((outKL + outLL + outNL + outOL)*2.0));
			aSL[countSL] = ((outNL*3.0) - ((outKL + outLL + outML + outOL)*2.0));
			aTL[countTL] = ((outOL*3.0) - ((outKL + outLL + outML + outNL)*2.0));
			
			aBR[countBR] = ((outCR*3.0) - ((outHR + outMR + outRR + outWR)*2.0));
			aGR[countGR] = ((outHR*3.0) - ((outCR + outMR + outRR + outWR)*2.0));
			aLR[countLR] = ((outMR*3.0) - ((outCR + outHR + outRR + outWR)*2.0));
			aQR[countQR] = ((outRR*3.0) - ((outCR + outHR + outMR + outWR)*2.0));
			aVR[countVR] = ((outWR*3.0) - ((outCR + outHR + outMR + outRR)*2.0));
			
			countPL++; if (countPL < 0 || countPL > delayP) countPL = 0;
			countQL++; if (countQL < 0 || countQL > delayQ) countQL = 0;
			countRL++; if (countRL < 0 || countRL > delayR) countRL = 0;
			countSL++; if (countSL < 0 || countSL > delayS) countSL = 0;
			countTL++; if (countTL < 0 || countTL > delayT) countTL = 0;
			
			countBR++; if (countBR < 0 || countBR > delayB) countBR = 0;
			countGR++; if (countGR < 0 || countGR > delayG) countGR = 0;
			countLR++; if (countLR < 0 || countLR > delayL) countLR = 0;
			countQR++; if (countQR < 0 || countQR > delayQ) countQR = 0;
			countVR++; if (countVR < 0 || countVR > delayV) countVR = 0;
			
			double earlyPL = ePL[countPL-((countPL > delayP)?delayP+1:0)];
			double earlyQL = eQL[countQL-((countQL > delayQ)?delayQ+1:0)];
			double earlyRL = eRL[countRL-((countRL > delayR)?delayR+1:0)];
			double earlySL = eSL[countSL-((countSL > delayS)?delayS+1:0)];
			double earlyTL = eTL[countTL-((countTL > delayT)?delayT+1:0)];
			
			double earlyBR = eBR[countBR-((countBR > delayB)?delayB+1:0)];
			double earlyGR = eGR[countGR-((countGR > delayG)?delayG+1:0)];
			double earlyLR = eLR[countLR-((countLR > delayL)?delayL+1:0)];
			double earlyQR = eQR[countQR-((countQR > delayQ)?delayQ+1:0)];
			double earlyVR = eVR[countVR-((countVR > delayV)?delayV+1:0)];
			
			double outPL = aPL[countPL-((countPL > delayP)?delayP+1:0)];
			double outQL = aQL[countQL-((countQL > delayQ)?delayQ+1:0)];
			double outRL = aRL[countRL-((countRL > delayR)?delayR+1:0)];
			double outSL = aSL[countSL-((countSL > delayS)?delayS+1:0)];
			double outTL = aTL[countTL-((countTL > delayT)?delayT+1:0)];
			
			double outBR = aBR[countBR-((countBR > delayB)?delayB+1:0)];
			double outGR = aGR[countGR-((countGR > delayG)?delayG+1:0)];
			double outLR = aLR[countLR-((countLR > delayL)?delayL+1:0)];
			double outQR = aQR[countQR-((countQR > delayQ)?delayQ+1:0)];
			double outVR = aVR[countVR-((countVR > delayV)?delayV+1:0)];
			
			//-------- four
			
			eUL[countUL] = ((earlyPL*3.0) - ((earlyQL + earlyRL + earlySL + earlyTL)*2.0));
			eVL[countVL] = ((earlyQL*3.0) - ((earlyPL + earlyRL + earlySL + earlyTL)*2.0));
			eWL[countWL] = ((earlyRL*3.0) - ((earlyPL + earlyQL + earlySL + earlyTL)*2.0));
			eXL[countXL] = ((earlySL*3.0) - ((earlyPL + earlyQL + earlyRL + earlyTL)*2.0));
			eYL[countYL] = ((earlyTL*3.0) - ((earlyPL + earlyQL + earlyRL + earlySL)*2.0));
			
			eAR[countAR] = ((earlyBR*3.0) - ((earlyGR + earlyLR + earlyQR + earlyVR)*2.0));
			eFR[countFR] = ((earlyGR*3.0) - ((earlyBR + earlyLR + earlyQR + earlyVR)*2.0));
			eKR[countKR] = ((earlyLR*3.0) - ((earlyBR + earlyGR + earlyQR + earlyVR)*2.0));
			ePR[countPR] = ((earlyQR*3.0) - ((earlyBR + earlyGR + earlyLR + earlyVR)*2.0));
			eUR[countUR] = ((earlyVR*3.0) - ((earlyBR + earlyGR + earlyLR + earlyQR)*2.0));
			
			aUL[countUL] = ((outPL*3.0) - ((outQL + outRL + outSL + outTL)*2.0));
			aVL[countVL] = ((outQL*3.0) - ((outPL + outRL + outSL + outTL)*2.0));
			aWL[countWL] = ((outRL*3.0) - ((outPL + outQL + outSL + outTL)*2.0));
			aXL[countXL] = ((outSL*3.0) - ((outPL + outQL + outRL + outTL)*2.0));
			aYL[countYL] = ((outTL*3.0) - ((outPL + outQL + outRL + outSL)*2.0));
			
			aAR[countAR] = ((outBR*3.0) - ((outGR + outLR + outQR + outVR)*2.0));
			aFR[countFR] = ((outGR*3.0) - ((outBR + outLR + outQR + outVR)*2.0));
			aKR[countKR] = ((outLR*3.0) - ((outBR + outGR + outQR + outVR)*2.0));
			aPR[countPR] = ((outQR*3.0) - ((outBR + outGR + outLR + outVR)*2.0));
			aUR[countUR] = ((outVR*3.0) - ((outBR + outGR + outLR + outQR)*2.0));
			
			countUL++; if (countUL < 0 || countUL > delayU) countUL = 0;
			countVL++; if (countVL < 0 || countVL > delayV) countVL = 0;
			countWL++; if (countWL < 0 || countWL > delayW) countWL = 0;
			countXL++; if (countXL < 0 || countXL > delayX) countXL = 0;
			countYL++; if (countYL < 0 || countYL > delayY) countYL = 0;
			
			countAR++; if (countAR < 0 || countAR > delayA) countAR = 0;
			countFR++; if (countFR < 0 || countFR > delayF) countFR = 0;
			countKR++; if (countKR < 0 || countKR > delayK) countKR = 0;
			countPR++; if (countPR < 0 || countPR > delayP) countPR = 0;
			countUR++; if (countUR < 0 || countUR > delayU) countUR = 0;
			
			double earlyUL = eUL[countUL-((countUL > delayU)?delayU+1:0)];
			double earlyVL = eVL[countVL-((countVL > delayV)?delayV+1:0)];
			double earlyWL = eWL[countWL-((countWL > delayW)?delayW+1:0)];
			double earlyXL = eXL[countXL-((countXL > delayX)?delayX+1:0)];
			double earlyYL = eYL[countYL-((countYL > delayY)?delayY+1:0)];
			
			double earlyAR = eAR[countAR-((countAR > delayA)?delayA+1:0)];
			double earlyFR = eFR[countFR-((countFR > delayF)?delayF+1:0)];
			double earlyKR = eKR[countKR-((countKR > delayK)?delayK+1:0)];
			double earlyPR = ePR[countPR-((countPR > delayP)?delayP+1:0)];
			double earlyUR = eUR[countUR-((countUR > delayU)?delayU+1:0)];
			
			double outUL = aUL[countUL-((countUL > delayU)?delayU+1:0)];
			double outVL = aVL[countVL-((countVL > delayV)?delayV+1:0)];
			double outWL = aWL[countWL-((countWL > delayW)?delayW+1:0)];
			double outXL = aXL[countXL-((countXL > delayX)?delayX+1:0)];
			double outYL = aYL[countYL-((countYL > delayY)?delayY+1:0)];
			
			double outAR = aAR[countAR-((countAR > delayA)?delayA+1:0)];
			double outFR = aFR[countFR-((countFR > delayF)?delayF+1:0)];
			double outKR = aKR[countKR-((countKR > delayK)?delayK+1:0)];
			double outPR = aPR[countPR-((countPR > delayP)?delayP+1:0)];
			double outUR = aUR[countUR-((countUR > delayU)?delayU+1:0)];
			
			//-------- five
			
			earlyReflectionL = (earlyUL + earlyVL + earlyWL + earlyXL + earlyYL)*0.0008;
			earlyReflectionR = (earlyAR + earlyFR + earlyKR + earlyPR + earlyUR)*0.0008;
			//and take the final combined sum of outputs, corrected for Householder gain
			
			feedbackAL = ((outAR*3.0) - ((outFR + outKR + outPR + outUR)*2.0)); feedbackSum = fabs(feedbackAL);
			feedbackER = ((outUL*3.0) - ((outVL + outWL + outXL + outYL)*2.0)); feedbackSum += fabs(feedbackER);
			feedbackBL = ((outVL*3.0) - ((outUL + outWL + outXL + outYL)*2.0)); feedbackSum += fabs(feedbackBL);
			feedbackJR = ((outFR*3.0) - ((outAR + outKR + outPR + outUR)*2.0)); feedbackSum += fabs(feedbackJR);
			feedbackCL = ((outWL*3.0) - ((outUL + outVL + outXL + outYL)*2.0)); feedbackSum += fabs(feedbackCL);
			feedbackOR = ((outKR*3.0) - ((outAR + outFR + outPR + outUR)*2.0)); feedbackSum += fabs(feedbackOR);
			feedbackDL = ((outXL*3.0) - ((outUL + outVL + outWL + outYL)*2.0)); feedbackSum += fabs(feedbackDL);
			feedbackTR = ((outPR*3.0) - ((outAR + outFR + outKR + outUR)*2.0)); feedbackSum += fabs(feedbackTR);
			feedbackEL = ((outYL*3.0) - ((outUL + outVL + outWL + outXL)*2.0)); feedbackSum += fabs(feedbackEL);
			feedbackYR = ((outUR*3.0) - ((outAR + outFR + outKR + outPR)*2.0)); feedbackSum += fabs(feedbackYR);
			
			inputSampleL = (outUL + outVL + outWL + outXL + outYL)*0.0008;
			inputSampleR = (outAR + outFR + outKR + outPR + outUR)*0.0008;
			//and take the final combined sum of outputs, corrected for Householder gain			
			
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
		double CBAL = (bez[bez_BL]+(CBL*(1.0-bez[bez_cycle]))+(BAL*bez[bez_cycle]))*0.125;
		double CBAR = (bez[bez_BR]+(CBR*(1.0-bez[bez_cycle]))+(BAR*bez[bez_cycle]))*0.125;
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
