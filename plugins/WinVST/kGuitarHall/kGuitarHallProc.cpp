/* ========================================
 *  kGuitarHall - kGuitarHall.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __kGuitarHall_H
#include "kGuitarHall.h"
#endif

void kGuitarHall::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double refdB = (A*70.0)+70.0;
	double topdB = 0.000000075 * pow(10.0,refdB/20.0) * overallscale * 0.000025;
	//the final 0.0001 scales it to the internal reverb path
	
	double regen = (1.0-pow(1.0-B,3.0))*0.00029;
	double derez = C/overallscale;
	if (derez < 0.0005) derez = 0.0005; if (derez > 1.0) derez = 1.0;
	derez = 1.0 / ((int)(1.0/derez));
	//this hard-locks derez to exact subdivisions of 1.0
	
	double pear = 0.22/derez; if (pear > 1.0) pear = 1.0; //inside derez filter
	double pearScaled = 0.19/overallscale; //outside derez filter
	
	int adjPredelay = predelay*D*derez;	
	int adjSubDelay = vlfpredelay*derez;
	double wet = E*2.0;
	double dry = 2.0 - wet;
	if (wet > 1.0) wet = 1.0; else wet *= wet;
	if (wet < 0.0) wet = 0.0;
	if (dry > 1.0) dry = 1.0;
	if (dry < 0.0) dry = 0.0;
	//this reverb makes 50% full dry AND full wet, not crossfaded.
	//that's so it can be on submixes without cutting back dry channel when adjusted:
	//unless you go super heavy, you are only adjusting the added verb loudness.
	
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
			
			//predelay
			aZL[countZ] = bez[bez_SampL];
			aZR[countZ] = bez[bez_SampR];
			countZ++; if (countZ < 0 || countZ > adjPredelay) countZ = 0;
			double avgSampL = aZL[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			double avgSampR = aZR[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			//end predelay
			
			//begin SubTight section
			double outSampleL = avgSampL * 0.0019;
			double outSampleR = avgSampR * 0.0019;
			double scale = 0.5+fabs(outSampleL*0.5);
			outSampleL = (subAL+(sin(subAL-outSampleL)*scale));
			subAL = outSampleL*scale;
			scale = 0.5+fabs(outSampleR*0.5);
			outSampleR = (subAR+(sin(subAR-outSampleR)*scale));
			subAR = outSampleR*scale;
			scale = 0.5+fabs(outSampleL*0.5);
			outSampleL = (subBL+(sin(subBL-outSampleL)*scale));
			subBL = outSampleL*scale;
			scale = 0.5+fabs(outSampleR*0.5);
			outSampleR = (subBR+(sin(subBR-outSampleR)*scale));
			subBR = outSampleR*scale;
			scale = 0.5+fabs(outSampleL*0.5);
			outSampleL = (subCL+(sin(subCL-outSampleL)*scale));
			subCL = outSampleL*scale;
			scale = 0.5+fabs(outSampleR*0.5);
			outSampleR = (subCR+(sin(subCR-outSampleR)*scale));
			subCR = outSampleR*scale;
			outSampleL = -outSampleL; outSampleR = -outSampleR;
			if (outSampleL > 0.25) outSampleL = 0.25; if (outSampleL < -0.25) outSampleL = -0.25;
			if (outSampleR > 0.25) outSampleR = 0.25; if (outSampleR < -0.25) outSampleR = -0.25;
			outSampleL *= 16.0;
			outSampleR *= 16.0;
			avgSampL -= outSampleL;
			avgSampR -= outSampleR;
			//end SubTight section
			
			//VLF predelay
			aVLFL[countVLF] = outSampleL;
			aVLFR[countVLF] = outSampleR;
			countVLF++; if (countVLF < 0 || countVLF > adjSubDelay) countVLF = 0;
			outSampleL = aVLFL[countVLF-((countVLF > adjSubDelay)?adjSubDelay+1:0)] * 2.0;
			outSampleR = aVLFR[countVLF-((countVLF > adjSubDelay)?adjSubDelay+1:0)] * 2.0;
			//end VLF predelay
			
			avgSampL += outSampleL;
			avgSampR += outSampleR;
			//having re-added our VLF delayed channel we can now re-use outSample
			
			aAL[countAL] = avgSampL + (feedbackAL * regen);
			aBL[countBL] = avgSampL + (feedbackBL * regen);
			aCL[countCL] = avgSampL + (feedbackCL * regen);
			aDL[countDL] = avgSampL + (feedbackDL * regen);
			aEL[countEL] = avgSampL + (feedbackEL * regen);
			
			aER[countER] = avgSampR + (feedbackER * regen);
			aJR[countJR] = avgSampR + (feedbackJR * regen);
			aOR[countOR] = avgSampR + (feedbackOR * regen);
			aTR[countTR] = avgSampR + (feedbackTR * regen);
			aYR[countYR] = avgSampR + (feedbackYR * regen);
			
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
			
			aVL[countVL] = ((outQL*3.0) - ((outPL + outRL + outSL + outTL)*2.0));
			aWL[countWL] = ((outRL*3.0) - ((outPL + outQL + outSL + outTL)*2.0));
			aXL[countXL] = ((outSL*3.0) - ((outPL + outQL + outRL + outTL)*2.0));
			aYL[countYL] = ((outTL*3.0) - ((outPL + outQL + outRL + outSL)*2.0));
			
			aAR[countAR] = ((outBR*3.0) - ((outGR + outLR + outQR + outVR)*2.0));
			aFR[countFR] = ((outGR*3.0) - ((outBR + outLR + outQR + outVR)*2.0));
			aKR[countKR] = ((outLR*3.0) - ((outBR + outGR + outQR + outVR)*2.0));
			aPR[countPR] = ((outQR*3.0) - ((outBR + outGR + outLR + outVR)*2.0));
			
			double outUL = ((outPL*3.0) - ((outQL + outRL + outSL + outTL)*2.0)) - (aUL[(countUL+1)-((countUL+1 > delayU)?delayU+1:0)]*0.618033988749894848204586);
			double outUR = ((outVR*3.0) - ((outBR + outGR + outLR + outQR)*2.0)) - (aUR[(countUR+1)-((countUR+1 > delayU)?delayU+1:0)]*0.618033988749894848204586);
			aUL[countUL] = outUL; outUL *= 0.618033988749894848204586;
			aUR[countUR] = outUR; outUR *= 0.618033988749894848204586;
			countUL++; if (countUL < 0 || countUL > delayU) countUL = 0;
			countUR++; if (countUR < 0 || countUR > delayU) countUR = 0;
			outUL += aUL[countUL-((countUL > delayU)?delayU+1:0)];
			outUR += aUR[countUR-((countUR > delayU)?delayU+1:0)];
			//the 11-length delay slot becomes a sole allpass
			
			vibBL = vibAL; vibAL = outUL;
			vibBR = vibAR; vibAR = outUR; //tiny two sample delay chains
			vibratoL += fpdL * 0.5e-13; if (vibratoL > M_PI*2.0) vibratoL -= M_PI*2.0;
			vibratoR += fpdR * 0.5e-13; if (vibratoR > M_PI*2.0) vibratoR -= M_PI*2.0;
			double quadL = sin(vibratoL)+1.0;
			double quadR = sin(vibratoR)+1.0;
			//quadrature delay points play back from a position in delay chains
			if (quadL < 1.0) outUL = (outUL*(1.0-quadL))+(vibAL*quadL);
			else outUL = (vibAL*(1.0-(quadL-1.0)))+(vibBL*(quadL-1.0));
			if (quadR < 1.0) outUR = (outUR*(1.0-quadR))+(vibAR*quadR);
			else outUR = (vibAR*(1.0-(quadR-1.0)))+(vibBR*(quadR-1.0));
			//also, pitch drift this allpass slot for very subtle motion
			
			countVL++; if (countVL < 0 || countVL > delayV) countVL = 0;
			countWL++; if (countWL < 0 || countWL > delayW) countWL = 0;
			countXL++; if (countXL < 0 || countXL > delayX) countXL = 0;
			countYL++; if (countYL < 0 || countYL > delayY) countYL = 0;
			
			countAR++; if (countAR < 0 || countAR > delayA) countAR = 0;
			countFR++; if (countFR < 0 || countFR > delayF) countFR = 0;
			countKR++; if (countKR < 0 || countKR > delayK) countKR = 0;
			countPR++; if (countPR < 0 || countPR > delayP) countPR = 0;
			
			double outVL = aVL[countVL-((countVL > delayV)?delayV+1:0)];
			double outWL = aWL[countWL-((countWL > delayW)?delayW+1:0)];
			double outXL = aXL[countXL-((countXL > delayX)?delayX+1:0)];
			double outYL = aYL[countYL-((countYL > delayY)?delayY+1:0)];
			
			double outAR = aAR[countAR-((countAR > delayA)?delayA+1:0)];
			double outFR = aFR[countFR-((countFR > delayF)?delayF+1:0)];
			double outKR = aKR[countKR-((countKR > delayK)?delayK+1:0)];
			double outPR = aPR[countPR-((countPR > delayP)?delayP+1:0)];
			
			//-------- five
			
			feedbackAL = ((outAR*3.0) - ((outFR + outKR + outPR + outUR)*2.0));
			feedbackER = ((outUL*3.0) - ((outVL + outWL + outXL + outYL)*2.0));
			double slew = ((feedbackAL - pearA[0]) + pearA[1])*pear*0.5;
			pearA[0] = feedbackAL = (pear * feedbackAL) + ((1.0-pear) * (pearA[0] + pearA[1]));
			pearA[1] = slew;
			slew = ((feedbackER - pearA[2]) + pearA[3])*pear*0.5;
			pearA[2] = feedbackER = (pear * feedbackER) + ((1.0-pear) * (pearA[2] + pearA[3]));
			pearA[3] = slew;
			//Air Discontinuity A begin
			feedbackAL *= topdB;
			if (feedbackAL < -0.222) feedbackAL = -0.222; if (feedbackAL > 0.222) feedbackAL = 0.222;
			dBaL[dBaXL] = feedbackAL; dBaPosL *= 0.5; dBaPosL += fabs((feedbackAL*((feedbackAL*0.25)-0.5))*0.5);
			int dBdly = floor(dBaPosL*dscBuf);
			double dBi = (dBaPosL*dscBuf)-dBdly;
			feedbackAL = dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackAL += dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*dBi;
			dBaXL++; if (dBaXL < 0 || dBaXL >= dscBuf) dBaXL = 0;
			feedbackAL /= topdB;
			feedbackER *= topdB;
			if (feedbackER < -0.222) feedbackER = -0.222; if (feedbackER > 0.222) feedbackER = 0.222;
			dBaR[dBaXR] = feedbackER; dBaPosR *= 0.5; dBaPosR += fabs((feedbackER*((feedbackER*0.25)-0.5))*0.5);
			dBdly = floor(dBaPosR*dscBuf);
			dBi = (dBaPosR*dscBuf)-dBdly;
			feedbackER = dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackER += dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*dBi;
			dBaXR++; if (dBaXR < 0 || dBaXR >= dscBuf) dBaXR = 0;
			feedbackER /= topdB;
			//Air Discontinuity A end			
			
			feedbackBL = ((outVL*3.0) - ((outUL + outWL + outXL + outYL)*2.0));
			feedbackJR = ((outFR*3.0) - ((outAR + outKR + outPR + outUR)*2.0));
			slew = ((feedbackBL - pearB[0]) + pearB[1])*pear*0.5;
			pearB[0] = feedbackBL = (pear * feedbackBL) + ((1.0-pear) * (pearB[0] + pearB[1]));
			pearB[1] = slew;
			slew = ((feedbackJR - pearB[2]) + pearB[3])*pear*0.5;
			pearB[2] = feedbackJR = (pear * feedbackJR) + ((1.0-pear) * (pearB[2] + pearB[3]));
			pearB[3] = slew;
			//Air Discontinuity B begin
			feedbackBL *= topdB;
			if (feedbackBL < -0.222) feedbackBL = -0.222; if (feedbackBL > 0.222) feedbackBL = 0.222;
			dBbL[dBbXL] = feedbackBL;  dBbPosL *= 0.5; dBbPosL += fabs((feedbackBL*((feedbackBL*0.25)-0.5))*0.5);
			dBdly = floor(dBbPosL*dscBuf); dBi = (dBbPosL*dscBuf)-dBdly;
			feedbackBL = dBbL[dBbXL-dBdly +((dBbXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackBL += dBbL[dBbXL-dBdly +((dBbXL-dBdly < 0)?dscBuf:0)]*dBi;
			dBbXL++; if (dBbXL < 0 || dBbXL >= dscBuf) dBbXL = 0;
			feedbackBL /= topdB;
			feedbackJR *= topdB;
			if (feedbackJR < -0.222) feedbackJR = -0.222; if (feedbackJR > 0.222) feedbackJR = 0.222;
			dBbR[dBbXR] = feedbackJR;  dBbPosR *= 0.5; dBbPosR += fabs((feedbackJR*((feedbackJR*0.25)-0.5))*0.5);
			dBdly = floor(dBbPosR*dscBuf); dBi = (dBbPosR*dscBuf)-dBdly;
			feedbackJR = dBbR[dBbXR-dBdly +((dBbXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackJR += dBbR[dBbXR-dBdly +((dBbXR-dBdly < 0)?dscBuf:0)]*dBi;
			dBbXR++; if (dBbXR < 0 || dBbXR >= dscBuf) dBbXR = 0;
			feedbackJR /= topdB;
			//Air Discontinuity B end			
			
			feedbackCL = ((outWL*3.0) - ((outUL + outVL + outXL + outYL)*2.0));
			feedbackOR = ((outKR*3.0) - ((outAR + outFR + outPR + outUR)*2.0));
			slew = ((feedbackCL - pearC[0]) + pearC[1])*pear*0.5;
			pearC[0] = feedbackCL = (pear * feedbackCL) + ((1.0-pear) * (pearC[0] + pearC[1]));
			pearC[1] = slew;
			slew = ((feedbackOR - pearC[2]) + pearC[3])*pear*0.5;
			pearC[2] = feedbackOR = (pear * feedbackOR) + ((1.0-pear) * (pearC[2] + pearC[3]));
			pearC[3] = slew;
			//Air Discontinuity C begin
			feedbackCL *= topdB;
			if (feedbackCL < -0.222) feedbackCL = -0.222; if (feedbackCL > 0.222) feedbackCL = 0.222;
			dBcL[dBcXL] = feedbackCL;  dBcPosL *= 0.5; dBcPosL += fabs((feedbackCL*((feedbackCL*0.25)-0.5))*0.5);
			dBdly = floor(dBcPosL*dscBuf); dBi = (dBcPosL*dscBuf)-dBdly;
			feedbackCL = dBcL[dBcXL-dBdly +((dBcXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackCL += dBcL[dBcXL-dBdly +((dBcXL-dBdly < 0)?dscBuf:0)]*dBi;
			dBcXL++; if (dBcXL < 0 || dBcXL >= dscBuf) dBcXL = 0;
			feedbackCL /= topdB;
			feedbackOR *= topdB;
			if (feedbackOR < -0.222) feedbackOR = -0.222; if (feedbackOR > 0.222) feedbackOR = 0.222;
			dBcR[dBcXR] = feedbackOR;  dBcPosR *= 0.5; dBcPosR += fabs((feedbackOR*((feedbackOR*0.25)-0.5))*0.5);
			dBdly = floor(dBcPosR*dscBuf); dBi = (dBcPosR*dscBuf)-dBdly;
			feedbackOR = dBcR[dBcXR-dBdly +((dBcXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackOR += dBcR[dBcXR-dBdly +((dBcXR-dBdly < 0)?dscBuf:0)]*dBi;
			dBcXR++; if (dBcXR < 0 || dBcXR >= dscBuf) dBcXR = 0;
			feedbackOR /= topdB;
			//Air Discontinuity C end			
			
			feedbackDL = ((outXL*3.0) - ((outUL + outVL + outWL + outYL)*2.0));
			feedbackTR = ((outPR*3.0) - ((outAR + outFR + outKR + outUR)*2.0));
			slew = ((feedbackDL - pearD[0]) + pearD[1])*pear*0.5;
			pearD[0] = feedbackDL = (pear * feedbackDL) + ((1.0-pear) * (pearD[0] + pearD[1]));
			pearD[1] = slew;
			slew = ((feedbackTR - pearD[2]) + pearD[3])*pear*0.5;
			pearD[2] = feedbackTR = (pear * feedbackTR) + ((1.0-pear) * (pearD[2] + pearD[3]));
			pearD[3] = slew;
			//Air Discontinuity D begin
			feedbackDL *= topdB;
			if (feedbackDL < -0.222) feedbackDL = -0.222; if (feedbackDL > 0.222) feedbackDL = 0.222;
			dBdL[dBdXL] = feedbackDL;  dBdPosL *= 0.5; dBdPosL += fabs((feedbackDL*((feedbackDL*0.25)-0.5))*0.5);
			dBdly = floor(dBdPosL*dscBuf); dBi = (dBdPosL*dscBuf)-dBdly;
			feedbackDL = dBdL[dBdXL-dBdly +((dBdXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackDL += dBdL[dBdXL-dBdly +((dBdXL-dBdly < 0)?dscBuf:0)]*dBi;
			dBdXL++; if (dBdXL < 0 || dBdXL >= dscBuf) dBdXL = 0;
			feedbackDL /= topdB;
			feedbackTR *= topdB;
			if (feedbackTR < -0.222) feedbackTR = -0.222; if (feedbackTR > 0.222) feedbackTR = 0.222;
			dBdR[dBdXR] = feedbackTR;  dBdPosR *= 0.5; dBdPosR += fabs((feedbackTR*((feedbackTR*0.25)-0.5))*0.5);
			dBdly = floor(dBdPosR*dscBuf); dBi = (dBdPosR*dscBuf)-dBdly;
			feedbackTR = dBdR[dBdXR-dBdly +((dBdXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackTR += dBdR[dBdXR-dBdly +((dBdXR-dBdly < 0)?dscBuf:0)]*dBi;
			dBdXR++; if (dBdXR < 0 || dBdXR >= dscBuf) dBdXR = 0;
			feedbackTR /= topdB;
			//Air Discontinuity D end
			
			feedbackEL = ((outYL*3.0) - ((outUL + outVL + outWL + outXL)*2.0));
			feedbackYR = ((outUR*3.0) - ((outAR + outFR + outKR + outPR)*2.0));
			slew = ((feedbackEL - pearE[0]) + pearE[1])*pear*0.5;
			pearE[0] = feedbackEL = (pear * feedbackEL) + ((1.0-pear) * (pearE[0] + pearE[1]));
			pearE[1] = slew;
			slew = ((feedbackYR - pearE[2]) + pearE[3])*pear*0.5;
			pearE[2] = feedbackYR = (pear * feedbackYR) + ((1.0-pear) * (pearE[2] + pearE[3]));
			pearE[3] = slew;
			//Air Discontinuity E begin
			feedbackEL *= topdB;
			if (feedbackEL < -0.222) feedbackEL = -0.222; if (feedbackEL > 0.222) feedbackEL = 0.222;
			dBeL[dBeXL] = feedbackEL;  dBePosL *= 0.5; dBePosL += fabs((feedbackEL*((feedbackEL*0.25)-0.5))*0.5);
			dBdly = floor(dBePosL*dscBuf); dBi = (dBePosL*dscBuf)-dBdly;
			feedbackEL = dBeL[dBeXL-dBdly +((dBeXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackEL += dBeL[dBeXL-dBdly +((dBeXL-dBdly < 0)?dscBuf:0)]*dBi;
			dBeXL++; if (dBeXL < 0 || dBeXL >= dscBuf) dBeXL = 0;
			feedbackEL /= topdB;
			feedbackYR *= topdB;
			if (feedbackYR < -0.222) feedbackYR = -0.222; if (feedbackYR > 0.222) feedbackYR = 0.222;
			dBeR[dBeXR] = feedbackYR;  dBePosR *= 0.5; dBePosR += fabs((feedbackYR*((feedbackYR*0.25)-0.5))*0.5);
			dBdly = floor(dBePosR*dscBuf); dBi = (dBePosR*dscBuf)-dBdly;
			feedbackYR = dBeR[dBeXR-dBdly +((dBeXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackYR += dBeR[dBeXR-dBdly +((dBeXR-dBdly < 0)?dscBuf:0)]*dBi;
			dBeXR++; if (dBeXR < 0 || dBeXR >= dscBuf) dBeXR = 0;
			feedbackYR /= topdB;
			//Air Discontinuity E end			
			//which we need to feed back into the input again, a bit
			
			inputSampleL = (outUL + outVL + outWL + outXL + outYL)*0.0008;
			inputSampleR = (outAR + outFR + outKR + outPR + outUR)*0.0008;
			//and take the final combined sum of outputs, corrected for Householder gain
						
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
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		
		double slew = ((inputSampleL - pearF[0]) + pearF[1])*pearScaled*0.5;
		pearF[0] = inputSampleL = (pearScaled * inputSampleL) + ((1.0-pearScaled) * (pearF[0] + pearF[1]));
		pearF[1] = slew;
		slew = ((inputSampleR - pearF[2]) + pearF[3])*pearScaled*0.5;
		pearF[2] = inputSampleR = (pearScaled * inputSampleR) + ((1.0-pearScaled) * (pearF[2] + pearF[3]));
		pearF[3] = slew;
		
		if (wet < 1.0) {inputSampleL *= wet; inputSampleR *= wet;}
		if (dry < 1.0) {drySampleL *= dry; drySampleR *= dry;}
		inputSampleL += drySampleL; inputSampleR += drySampleR;
		//this is our submix verb dry/wet: 0.5 is BOTH at FULL VOLUME
		//purpose is that, if you're adding verb, you're not altering other balances
		
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

void kGuitarHall::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double refdB = (A*70.0)+70.0;
	double topdB = 0.000000075 * pow(10.0,refdB/20.0) * overallscale * 0.000025;
	//the final 0.0001 scales it to the internal reverb path
	
	double regen = (1.0-pow(1.0-B,3.0))*0.00029;
	double derez = C/overallscale;
	if (derez < 0.0005) derez = 0.0005; if (derez > 1.0) derez = 1.0;
	derez = 1.0 / ((int)(1.0/derez));
	//this hard-locks derez to exact subdivisions of 1.0
	
	double pear = 0.22/derez; if (pear > 1.0) pear = 1.0; //inside derez filter
	double pearScaled = 0.19/overallscale; //outside derez filter
	
	int adjPredelay = predelay*D*derez;	
	int adjSubDelay = vlfpredelay*derez;
	double wet = E*2.0;
	double dry = 2.0 - wet;
	if (wet > 1.0) wet = 1.0; else wet *= wet;
	if (wet < 0.0) wet = 0.0;
	if (dry > 1.0) dry = 1.0;
	if (dry < 0.0) dry = 0.0;
	//this reverb makes 50% full dry AND full wet, not crossfaded.
	//that's so it can be on submixes without cutting back dry channel when adjusted:
	//unless you go super heavy, you are only adjusting the added verb loudness.
	
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
			
			//predelay
			aZL[countZ] = bez[bez_SampL];
			aZR[countZ] = bez[bez_SampR];
			countZ++; if (countZ < 0 || countZ > adjPredelay) countZ = 0;
			double avgSampL = aZL[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			double avgSampR = aZR[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			//end predelay
			
			//begin SubTight section
			double outSampleL = avgSampL * 0.0019;
			double outSampleR = avgSampR * 0.0019;
			double scale = 0.5+fabs(outSampleL*0.5);
			outSampleL = (subAL+(sin(subAL-outSampleL)*scale));
			subAL = outSampleL*scale;
			scale = 0.5+fabs(outSampleR*0.5);
			outSampleR = (subAR+(sin(subAR-outSampleR)*scale));
			subAR = outSampleR*scale;
			scale = 0.5+fabs(outSampleL*0.5);
			outSampleL = (subBL+(sin(subBL-outSampleL)*scale));
			subBL = outSampleL*scale;
			scale = 0.5+fabs(outSampleR*0.5);
			outSampleR = (subBR+(sin(subBR-outSampleR)*scale));
			subBR = outSampleR*scale;
			scale = 0.5+fabs(outSampleL*0.5);
			outSampleL = (subCL+(sin(subCL-outSampleL)*scale));
			subCL = outSampleL*scale;
			scale = 0.5+fabs(outSampleR*0.5);
			outSampleR = (subCR+(sin(subCR-outSampleR)*scale));
			subCR = outSampleR*scale;
			outSampleL = -outSampleL; outSampleR = -outSampleR;
			if (outSampleL > 0.25) outSampleL = 0.25; if (outSampleL < -0.25) outSampleL = -0.25;
			if (outSampleR > 0.25) outSampleR = 0.25; if (outSampleR < -0.25) outSampleR = -0.25;
			outSampleL *= 16.0;
			outSampleR *= 16.0;
			avgSampL -= outSampleL;
			avgSampR -= outSampleR;
			//end SubTight section
			
			//VLF predelay
			aVLFL[countVLF] = outSampleL;
			aVLFR[countVLF] = outSampleR;
			countVLF++; if (countVLF < 0 || countVLF > adjSubDelay) countVLF = 0;
			outSampleL = aVLFL[countVLF-((countVLF > adjSubDelay)?adjSubDelay+1:0)] * 2.0;
			outSampleR = aVLFR[countVLF-((countVLF > adjSubDelay)?adjSubDelay+1:0)] * 2.0;
			//end VLF predelay
			
			avgSampL += outSampleL;
			avgSampR += outSampleR;
			//having re-added our VLF delayed channel we can now re-use outSample
			
			aAL[countAL] = avgSampL + (feedbackAL * regen);
			aBL[countBL] = avgSampL + (feedbackBL * regen);
			aCL[countCL] = avgSampL + (feedbackCL * regen);
			aDL[countDL] = avgSampL + (feedbackDL * regen);
			aEL[countEL] = avgSampL + (feedbackEL * regen);
			
			aER[countER] = avgSampR + (feedbackER * regen);
			aJR[countJR] = avgSampR + (feedbackJR * regen);
			aOR[countOR] = avgSampR + (feedbackOR * regen);
			aTR[countTR] = avgSampR + (feedbackTR * regen);
			aYR[countYR] = avgSampR + (feedbackYR * regen);
			
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
			
			aVL[countVL] = ((outQL*3.0) - ((outPL + outRL + outSL + outTL)*2.0));
			aWL[countWL] = ((outRL*3.0) - ((outPL + outQL + outSL + outTL)*2.0));
			aXL[countXL] = ((outSL*3.0) - ((outPL + outQL + outRL + outTL)*2.0));
			aYL[countYL] = ((outTL*3.0) - ((outPL + outQL + outRL + outSL)*2.0));
			
			aAR[countAR] = ((outBR*3.0) - ((outGR + outLR + outQR + outVR)*2.0));
			aFR[countFR] = ((outGR*3.0) - ((outBR + outLR + outQR + outVR)*2.0));
			aKR[countKR] = ((outLR*3.0) - ((outBR + outGR + outQR + outVR)*2.0));
			aPR[countPR] = ((outQR*3.0) - ((outBR + outGR + outLR + outVR)*2.0));
			
			double outUL = ((outPL*3.0) - ((outQL + outRL + outSL + outTL)*2.0)) - (aUL[(countUL+1)-((countUL+1 > delayU)?delayU+1:0)]*0.618033988749894848204586);
			double outUR = ((outVR*3.0) - ((outBR + outGR + outLR + outQR)*2.0)) - (aUR[(countUR+1)-((countUR+1 > delayU)?delayU+1:0)]*0.618033988749894848204586);
			aUL[countUL] = outUL; outUL *= 0.618033988749894848204586;
			aUR[countUR] = outUR; outUR *= 0.618033988749894848204586;
			countUL++; if (countUL < 0 || countUL > delayU) countUL = 0;
			countUR++; if (countUR < 0 || countUR > delayU) countUR = 0;
			outUL += aUL[countUL-((countUL > delayU)?delayU+1:0)];
			outUR += aUR[countUR-((countUR > delayU)?delayU+1:0)];
			//the 11-length delay slot becomes a sole allpass
			
			vibBL = vibAL; vibAL = outUL;
			vibBR = vibAR; vibAR = outUR; //tiny two sample delay chains
			vibratoL += fpdL * 0.5e-13; if (vibratoL > M_PI*2.0) vibratoL -= M_PI*2.0;
			vibratoR += fpdR * 0.5e-13; if (vibratoR > M_PI*2.0) vibratoR -= M_PI*2.0;
			double quadL = sin(vibratoL)+1.0;
			double quadR = sin(vibratoR)+1.0;
			//quadrature delay points play back from a position in delay chains
			if (quadL < 1.0) outUL = (outUL*(1.0-quadL))+(vibAL*quadL);
			else outUL = (vibAL*(1.0-(quadL-1.0)))+(vibBL*(quadL-1.0));
			if (quadR < 1.0) outUR = (outUR*(1.0-quadR))+(vibAR*quadR);
			else outUR = (vibAR*(1.0-(quadR-1.0)))+(vibBR*(quadR-1.0));
			//also, pitch drift this allpass slot for very subtle motion
			
			countVL++; if (countVL < 0 || countVL > delayV) countVL = 0;
			countWL++; if (countWL < 0 || countWL > delayW) countWL = 0;
			countXL++; if (countXL < 0 || countXL > delayX) countXL = 0;
			countYL++; if (countYL < 0 || countYL > delayY) countYL = 0;
			
			countAR++; if (countAR < 0 || countAR > delayA) countAR = 0;
			countFR++; if (countFR < 0 || countFR > delayF) countFR = 0;
			countKR++; if (countKR < 0 || countKR > delayK) countKR = 0;
			countPR++; if (countPR < 0 || countPR > delayP) countPR = 0;
			
			double outVL = aVL[countVL-((countVL > delayV)?delayV+1:0)];
			double outWL = aWL[countWL-((countWL > delayW)?delayW+1:0)];
			double outXL = aXL[countXL-((countXL > delayX)?delayX+1:0)];
			double outYL = aYL[countYL-((countYL > delayY)?delayY+1:0)];
			
			double outAR = aAR[countAR-((countAR > delayA)?delayA+1:0)];
			double outFR = aFR[countFR-((countFR > delayF)?delayF+1:0)];
			double outKR = aKR[countKR-((countKR > delayK)?delayK+1:0)];
			double outPR = aPR[countPR-((countPR > delayP)?delayP+1:0)];
			
			//-------- five
			
			feedbackAL = ((outAR*3.0) - ((outFR + outKR + outPR + outUR)*2.0));
			feedbackER = ((outUL*3.0) - ((outVL + outWL + outXL + outYL)*2.0));
			double slew = ((feedbackAL - pearA[0]) + pearA[1])*pear*0.5;
			pearA[0] = feedbackAL = (pear * feedbackAL) + ((1.0-pear) * (pearA[0] + pearA[1]));
			pearA[1] = slew;
			slew = ((feedbackER - pearA[2]) + pearA[3])*pear*0.5;
			pearA[2] = feedbackER = (pear * feedbackER) + ((1.0-pear) * (pearA[2] + pearA[3]));
			pearA[3] = slew;
			//Air Discontinuity A begin
			feedbackAL *= topdB;
			if (feedbackAL < -0.222) feedbackAL = -0.222; if (feedbackAL > 0.222) feedbackAL = 0.222;
			dBaL[dBaXL] = feedbackAL; dBaPosL *= 0.5; dBaPosL += fabs((feedbackAL*((feedbackAL*0.25)-0.5))*0.5);
			int dBdly = floor(dBaPosL*dscBuf);
			double dBi = (dBaPosL*dscBuf)-dBdly;
			feedbackAL = dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackAL += dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*dBi;
			dBaXL++; if (dBaXL < 0 || dBaXL >= dscBuf) dBaXL = 0;
			feedbackAL /= topdB;
			feedbackER *= topdB;
			if (feedbackER < -0.222) feedbackER = -0.222; if (feedbackER > 0.222) feedbackER = 0.222;
			dBaR[dBaXR] = feedbackER; dBaPosR *= 0.5; dBaPosR += fabs((feedbackER*((feedbackER*0.25)-0.5))*0.5);
			dBdly = floor(dBaPosR*dscBuf);
			dBi = (dBaPosR*dscBuf)-dBdly;
			feedbackER = dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackER += dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*dBi;
			dBaXR++; if (dBaXR < 0 || dBaXR >= dscBuf) dBaXR = 0;
			feedbackER /= topdB;
			//Air Discontinuity A end			
			
			feedbackBL = ((outVL*3.0) - ((outUL + outWL + outXL + outYL)*2.0));
			feedbackJR = ((outFR*3.0) - ((outAR + outKR + outPR + outUR)*2.0));
			slew = ((feedbackBL - pearB[0]) + pearB[1])*pear*0.5;
			pearB[0] = feedbackBL = (pear * feedbackBL) + ((1.0-pear) * (pearB[0] + pearB[1]));
			pearB[1] = slew;
			slew = ((feedbackJR - pearB[2]) + pearB[3])*pear*0.5;
			pearB[2] = feedbackJR = (pear * feedbackJR) + ((1.0-pear) * (pearB[2] + pearB[3]));
			pearB[3] = slew;
			//Air Discontinuity B begin
			feedbackBL *= topdB;
			if (feedbackBL < -0.222) feedbackBL = -0.222; if (feedbackBL > 0.222) feedbackBL = 0.222;
			dBbL[dBbXL] = feedbackBL;  dBbPosL *= 0.5; dBbPosL += fabs((feedbackBL*((feedbackBL*0.25)-0.5))*0.5);
			dBdly = floor(dBbPosL*dscBuf); dBi = (dBbPosL*dscBuf)-dBdly;
			feedbackBL = dBbL[dBbXL-dBdly +((dBbXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackBL += dBbL[dBbXL-dBdly +((dBbXL-dBdly < 0)?dscBuf:0)]*dBi;
			dBbXL++; if (dBbXL < 0 || dBbXL >= dscBuf) dBbXL = 0;
			feedbackBL /= topdB;
			feedbackJR *= topdB;
			if (feedbackJR < -0.222) feedbackJR = -0.222; if (feedbackJR > 0.222) feedbackJR = 0.222;
			dBbR[dBbXR] = feedbackJR;  dBbPosR *= 0.5; dBbPosR += fabs((feedbackJR*((feedbackJR*0.25)-0.5))*0.5);
			dBdly = floor(dBbPosR*dscBuf); dBi = (dBbPosR*dscBuf)-dBdly;
			feedbackJR = dBbR[dBbXR-dBdly +((dBbXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackJR += dBbR[dBbXR-dBdly +((dBbXR-dBdly < 0)?dscBuf:0)]*dBi;
			dBbXR++; if (dBbXR < 0 || dBbXR >= dscBuf) dBbXR = 0;
			feedbackJR /= topdB;
			//Air Discontinuity B end			
			
			feedbackCL = ((outWL*3.0) - ((outUL + outVL + outXL + outYL)*2.0));
			feedbackOR = ((outKR*3.0) - ((outAR + outFR + outPR + outUR)*2.0));
			slew = ((feedbackCL - pearC[0]) + pearC[1])*pear*0.5;
			pearC[0] = feedbackCL = (pear * feedbackCL) + ((1.0-pear) * (pearC[0] + pearC[1]));
			pearC[1] = slew;
			slew = ((feedbackOR - pearC[2]) + pearC[3])*pear*0.5;
			pearC[2] = feedbackOR = (pear * feedbackOR) + ((1.0-pear) * (pearC[2] + pearC[3]));
			pearC[3] = slew;
			//Air Discontinuity C begin
			feedbackCL *= topdB;
			if (feedbackCL < -0.222) feedbackCL = -0.222; if (feedbackCL > 0.222) feedbackCL = 0.222;
			dBcL[dBcXL] = feedbackCL;  dBcPosL *= 0.5; dBcPosL += fabs((feedbackCL*((feedbackCL*0.25)-0.5))*0.5);
			dBdly = floor(dBcPosL*dscBuf); dBi = (dBcPosL*dscBuf)-dBdly;
			feedbackCL = dBcL[dBcXL-dBdly +((dBcXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackCL += dBcL[dBcXL-dBdly +((dBcXL-dBdly < 0)?dscBuf:0)]*dBi;
			dBcXL++; if (dBcXL < 0 || dBcXL >= dscBuf) dBcXL = 0;
			feedbackCL /= topdB;
			feedbackOR *= topdB;
			if (feedbackOR < -0.222) feedbackOR = -0.222; if (feedbackOR > 0.222) feedbackOR = 0.222;
			dBcR[dBcXR] = feedbackOR;  dBcPosR *= 0.5; dBcPosR += fabs((feedbackOR*((feedbackOR*0.25)-0.5))*0.5);
			dBdly = floor(dBcPosR*dscBuf); dBi = (dBcPosR*dscBuf)-dBdly;
			feedbackOR = dBcR[dBcXR-dBdly +((dBcXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackOR += dBcR[dBcXR-dBdly +((dBcXR-dBdly < 0)?dscBuf:0)]*dBi;
			dBcXR++; if (dBcXR < 0 || dBcXR >= dscBuf) dBcXR = 0;
			feedbackOR /= topdB;
			//Air Discontinuity C end			
			
			feedbackDL = ((outXL*3.0) - ((outUL + outVL + outWL + outYL)*2.0));
			feedbackTR = ((outPR*3.0) - ((outAR + outFR + outKR + outUR)*2.0));
			slew = ((feedbackDL - pearD[0]) + pearD[1])*pear*0.5;
			pearD[0] = feedbackDL = (pear * feedbackDL) + ((1.0-pear) * (pearD[0] + pearD[1]));
			pearD[1] = slew;
			slew = ((feedbackTR - pearD[2]) + pearD[3])*pear*0.5;
			pearD[2] = feedbackTR = (pear * feedbackTR) + ((1.0-pear) * (pearD[2] + pearD[3]));
			pearD[3] = slew;
			//Air Discontinuity D begin
			feedbackDL *= topdB;
			if (feedbackDL < -0.222) feedbackDL = -0.222; if (feedbackDL > 0.222) feedbackDL = 0.222;
			dBdL[dBdXL] = feedbackDL;  dBdPosL *= 0.5; dBdPosL += fabs((feedbackDL*((feedbackDL*0.25)-0.5))*0.5);
			dBdly = floor(dBdPosL*dscBuf); dBi = (dBdPosL*dscBuf)-dBdly;
			feedbackDL = dBdL[dBdXL-dBdly +((dBdXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackDL += dBdL[dBdXL-dBdly +((dBdXL-dBdly < 0)?dscBuf:0)]*dBi;
			dBdXL++; if (dBdXL < 0 || dBdXL >= dscBuf) dBdXL = 0;
			feedbackDL /= topdB;
			feedbackTR *= topdB;
			if (feedbackTR < -0.222) feedbackTR = -0.222; if (feedbackTR > 0.222) feedbackTR = 0.222;
			dBdR[dBdXR] = feedbackTR;  dBdPosR *= 0.5; dBdPosR += fabs((feedbackTR*((feedbackTR*0.25)-0.5))*0.5);
			dBdly = floor(dBdPosR*dscBuf); dBi = (dBdPosR*dscBuf)-dBdly;
			feedbackTR = dBdR[dBdXR-dBdly +((dBdXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackTR += dBdR[dBdXR-dBdly +((dBdXR-dBdly < 0)?dscBuf:0)]*dBi;
			dBdXR++; if (dBdXR < 0 || dBdXR >= dscBuf) dBdXR = 0;
			feedbackTR /= topdB;
			//Air Discontinuity D end
			
			feedbackEL = ((outYL*3.0) - ((outUL + outVL + outWL + outXL)*2.0));
			feedbackYR = ((outUR*3.0) - ((outAR + outFR + outKR + outPR)*2.0));
			slew = ((feedbackEL - pearE[0]) + pearE[1])*pear*0.5;
			pearE[0] = feedbackEL = (pear * feedbackEL) + ((1.0-pear) * (pearE[0] + pearE[1]));
			pearE[1] = slew;
			slew = ((feedbackYR - pearE[2]) + pearE[3])*pear*0.5;
			pearE[2] = feedbackYR = (pear * feedbackYR) + ((1.0-pear) * (pearE[2] + pearE[3]));
			pearE[3] = slew;
			//Air Discontinuity E begin
			feedbackEL *= topdB;
			if (feedbackEL < -0.222) feedbackEL = -0.222; if (feedbackEL > 0.222) feedbackEL = 0.222;
			dBeL[dBeXL] = feedbackEL;  dBePosL *= 0.5; dBePosL += fabs((feedbackEL*((feedbackEL*0.25)-0.5))*0.5);
			dBdly = floor(dBePosL*dscBuf); dBi = (dBePosL*dscBuf)-dBdly;
			feedbackEL = dBeL[dBeXL-dBdly +((dBeXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackEL += dBeL[dBeXL-dBdly +((dBeXL-dBdly < 0)?dscBuf:0)]*dBi;
			dBeXL++; if (dBeXL < 0 || dBeXL >= dscBuf) dBeXL = 0;
			feedbackEL /= topdB;
			feedbackYR *= topdB;
			if (feedbackYR < -0.222) feedbackYR = -0.222; if (feedbackYR > 0.222) feedbackYR = 0.222;
			dBeR[dBeXR] = feedbackYR;  dBePosR *= 0.5; dBePosR += fabs((feedbackYR*((feedbackYR*0.25)-0.5))*0.5);
			dBdly = floor(dBePosR*dscBuf); dBi = (dBePosR*dscBuf)-dBdly;
			feedbackYR = dBeR[dBeXR-dBdly +((dBeXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; feedbackYR += dBeR[dBeXR-dBdly +((dBeXR-dBdly < 0)?dscBuf:0)]*dBi;
			dBeXR++; if (dBeXR < 0 || dBeXR >= dscBuf) dBeXR = 0;
			feedbackYR /= topdB;
			//Air Discontinuity E end			
			//which we need to feed back into the input again, a bit
			
			inputSampleL = (outUL + outVL + outWL + outXL + outYL)*0.0008;
			inputSampleR = (outAR + outFR + outKR + outPR + outUR)*0.0008;
			//and take the final combined sum of outputs, corrected for Householder gain
						
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
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		
		double slew = ((inputSampleL - pearF[0]) + pearF[1])*pearScaled*0.5;
		pearF[0] = inputSampleL = (pearScaled * inputSampleL) + ((1.0-pearScaled) * (pearF[0] + pearF[1]));
		pearF[1] = slew;
		slew = ((inputSampleR - pearF[2]) + pearF[3])*pearScaled*0.5;
		pearF[2] = inputSampleR = (pearScaled * inputSampleR) + ((1.0-pearScaled) * (pearF[2] + pearF[3]));
		pearF[3] = slew;
		
		if (wet < 1.0) {inputSampleL *= wet; inputSampleR *= wet;}
		if (dry < 1.0) {drySampleL *= dry; drySampleR *= dry;}
		inputSampleL += drySampleL; inputSampleR += drySampleR;
		//this is our submix verb dry/wet: 0.5 is BOTH at FULL VOLUME
		//purpose is that, if you're adding verb, you're not altering other balances
		
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
