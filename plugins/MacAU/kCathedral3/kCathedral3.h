/*
*	File:		kCathedral3.h
*	
*	Version:	1.0
* 
*	Created:	5/7/24
*	
*	Copyright:  Copyright © 2024 Airwindows, Airwindows uses the MIT license
* 
*	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc. ("Apple") in 
*				consideration of your agreement to the following terms, and your use, installation, modification 
*				or redistribution of this Apple software constitutes acceptance of these terms.  If you do 
*				not agree with these terms, please do not use, install, modify or redistribute this Apple 
*				software.
*
*				In consideration of your agreement to abide by the following terms, and subject to these terms, 
*				Apple grants you a personal, non-exclusive license, under Apple's copyrights in this 
*				original Apple software (the "Apple Software"), to use, reproduce, modify and redistribute the 
*				Apple Software, with or without modifications, in source and/or binary forms; provided that if you 
*				redistribute the Apple Software in its entirety and without modifications, you must retain this 
*				notice and the following text and disclaimers in all such redistributions of the Apple Software. 
*				Neither the name, trademarks, service marks or logos of Apple Computer, Inc. may be used to 
*				endorse or promote products derived from the Apple Software without specific prior written 
*				permission from Apple.  Except as expressly stated in this notice, no other rights or 
*				licenses, express or implied, are granted by Apple herein, including but not limited to any 
*				patent rights that may be infringed by your derivative works or by other works in which the 
*				Apple Software may be incorporated.
*
*				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO WARRANTIES, EXPRESS OR 
*				IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY 
*				AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE 
*				OR IN COMBINATION WITH YOUR PRODUCTS.
*
*				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL 
*				DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*				OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, 
*				REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER 
*				UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN 
*				IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
#include "AUEffectBase.h"
#include "kCathedral3Version.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __kCathedral3_h__
#define __kCathedral3_h__


#pragma mark ____kCathedral3 Parameters

// parameters
static const float kDefaultValue_ParamA = 100.0;
static const float kDefaultValue_ParamB = 0.5;
static const float kDefaultValue_ParamC = 1.0;
static const float kDefaultValue_ParamD = 0.0;
static const float kDefaultValue_ParamE = 0.25;

static CFStringRef kParameterAName = CFSTR("Top dB");
static CFStringRef kParameterBName = CFSTR("Regen");
static CFStringRef kParameterCName = CFSTR("DeRez");
static CFStringRef kParameterDName = CFSTR("Predelay");
static CFStringRef kParameterEName = CFSTR("Wetness");

enum {
	kParam_A =0,
	kParam_B =1,
	kParam_C =2,
	kParam_D =3,
	kParam_E =4,
	//Add your parameters here...
	kNumberOfParameters=5
};

const int dscBuf = 90;

const int predelay = 15000; const int vlfpredelay = 11000;

const int delayA = 824; const int delayB = 982; const int delayC = 753; const int delayD = 402; const int delayE = 1206; const int delayF = 1235; const int delayG = 1396; const int delayH = 1330; const int delayI = 248; const int delayJ = 62; const int delayK = 1407; const int delayL = 40; const int delayM = 245; const int delayN = 1088; const int delayO = 775; const int delayP = 13; const int delayQ = 1299; const int delayR = 127; const int delayS = 1497; const int delayT = 386; const int delayU = 11; const int delayV = 971; const int delayW = 1403; const int delayX = 81; const int delayY = 1480; //7 to 162 ms, 860 seat hall  
//860-SLLU rated incompressible if filesize larger than 25,270,485 bytes

#pragma mark ____kCathedral3
class kCathedral3 : public AUEffectBase
{
public:
	kCathedral3(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~kCathedral3 () { delete mDebugDispatcher; }
#endif
	
	virtual ComponentResult Reset(AudioUnitScope inScope, AudioUnitElement inElement);

	virtual OSStatus ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags, 
						const AudioBufferList & inBuffer, AudioBufferList & outBuffer, 
						UInt32 inFramesToProcess);
	virtual UInt32 SupportedNumChannels(const AUChannelInfo ** outInfo);

	virtual	ComponentResult		GetParameterValueStrings(AudioUnitScope			inScope,
														 AudioUnitParameterID		inParameterID,
														 CFArrayRef *			outStrings);
    
	virtual	ComponentResult		GetParameterInfo(AudioUnitScope			inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo);
    
	virtual ComponentResult		GetPropertyInfo(AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &			outDataSize,
												Boolean	&			outWritable );
	
	virtual ComponentResult		GetProperty(AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 		inElement,
											void *			outData);

	virtual ComponentResult    Initialize();
	virtual bool				SupportsTail () { return true; }
    virtual Float64				GetTailTime() {return (1.0/GetSampleRate())*0.0;} //in SECONDS! gsr * a number = in samples
    virtual Float64				GetLatency() {return (1.0/GetSampleRate())*0.0;}	// in SECONDS! gsr * a number = in samples
	
	/*! @method Version */
	virtual ComponentResult		Version() { return kkCathedral3Version; }
	
	private: 
	double gainOutL;
 	double gainOutR;
		
	double aAL[delayA+5];
	double aBL[delayB+5];
	double aCL[delayC+5];
	double aDL[delayD+5];
	double aEL[delayE+5];
	double aFL[delayF+5];
	double aGL[delayG+5];
	double aHL[delayH+5];
	double aIL[delayI+5];
	double aJL[delayJ+5];
	double aKL[delayK+5];
	double aLL[delayL+5];
	double aML[delayM+5];
	double aNL[delayN+5];
	double aOL[delayO+5];
	double aPL[delayP+5];
	double aQL[delayQ+5];
	double aRL[delayR+5];
	double aSL[delayS+5];
	double aTL[delayT+5];
	double aUL[delayU+5];
	double aVL[delayV+5];
	double aWL[delayW+5];
	double aXL[delayX+5];
	double aYL[delayY+5];
	
	double aAR[delayA+5];
	double aBR[delayB+5];
	double aCR[delayC+5];
	double aDR[delayD+5];
	double aER[delayE+5];
	double aFR[delayF+5];
	double aGR[delayG+5];
	double aHR[delayH+5];
	double aIR[delayI+5];
	double aJR[delayJ+5];
	double aKR[delayK+5];
	double aLR[delayL+5];
	double aMR[delayM+5];
	double aNR[delayN+5];
	double aOR[delayO+5];
	double aPR[delayP+5];
	double aQR[delayQ+5];
	double aRR[delayR+5];
	double aSR[delayS+5];
	double aTR[delayT+5];
	double aUR[delayU+5];
	double aVR[delayV+5];
	double aWR[delayW+5];
	double aXR[delayX+5];
	double aYR[delayY+5];
	
	double aZL[predelay+5];
	double aZR[predelay+5];
	
	double aVLFL[vlfpredelay+5];
	double aVLFR[vlfpredelay+5];
	
	double feedbackAL;
	double feedbackBL;
	double feedbackCL;
	double feedbackDL;
	double feedbackEL;
	
	double feedbackER;
	double feedbackJR;
	double feedbackOR;
	double feedbackTR;
	double feedbackYR;
	
	int countAL;
	int countBL;
	int countCL;
	int countDL;
	int countEL;
	int countFL;
	int countGL;
	int countHL;
	int countIL;
	int countJL;
	int countKL;
	int countLL;		
	int countML;		
	int countNL;		
	int countOL;		
	int countPL;		
	int countQL;		
	int countRL;		
	int countSL;		
	int countTL;		
	int countUL;		
	int countVL;		
	int countWL;		
	int countXL;		
	int countYL;		
	
	int countAR;
	int countBR;
	int countCR;
	int countDR;
	int countER;
	int countFR;
	int countGR;
	int countHR;
	int countIR;
	int countJR;
	int countKR;
	int countLR;		
	int countMR;		
	int countNR;		
	int countOR;		
	int countPR;		
	int countQR;		
	int countRR;		
	int countSR;		
	int countTR;		
	int countUR;		
	int countVR;		
	int countWR;		
	int countXR;		
	int countYR;
	
	int countZ;		
	int countVLF;		
		
	enum {
		prevSampL1,
		prevSlewL1,
		prevSampR1,
		prevSlewR1,
		prevSampL2,
		prevSlewL2,
		prevSampR2,
		prevSlewR2,
		prevSampL3,
		prevSlewL3,
		prevSampR3,
		prevSlewR3,
		prevSampL4,
		prevSlewL4,
		prevSampR4,
		prevSlewR4,
		prevSampL5,
		prevSlewL5,
		prevSampR5,
		prevSlewR5,
		prevSampL6,
		prevSlewL6,
		prevSampR6,
		prevSlewR6,
		prevSampL7,
		prevSlewL7,
		prevSampR7,
		prevSlewR7,
		prevSampL8,
		prevSlewL8,
		prevSampR8,
		prevSlewR8,
		prevSampL9,
		prevSlewL9,
		prevSampR9,
		prevSlewR9,
		prevSampL10,
		prevSlewL10,
		prevSampR10,
		prevSlewR10,
		pear_total
	}; //fixed frequency pear filter for ultrasonics, stereo
	
	double pearA[pear_total]; //probably worth just using a number here
	double pearB[pear_total]; //probably worth just using a number here
	double pearC[pear_total]; //probably worth just using a number here
	double pearD[pear_total]; //probably worth just using a number here
	double pearE[pear_total]; //probably worth just using a number here
	double pearF[pear_total]; //probably worth just using a number here
	
	double vibratoL;
	double vibratoR;
	double vibAL;
	double vibAR;
	double vibBL;
	double vibBR;
	
	double subAL;
	double subAR;
	double subBL;
	double subBR;
	double subCL;
	double subCR;
	
	double sbAL;
	double sbAR;
	double sbBL;
	double sbBR;
	double sbCL;
	double sbCR;
	
	double dBaL[dscBuf+5];
	double dBaPosL;
	int dBaXL;
	
	double dBaR[dscBuf+5];
	double dBaPosR;
	int dBaXR;
	
	double dBbL[dscBuf+5];
	double dBbPosL;
	int dBbXL;
	
	double dBbR[dscBuf+5];
	double dBbPosR;
	int dBbXR;
	
	double dBcL[dscBuf+5];
	double dBcPosL;
	int dBcXL;	
	
	double dBcR[dscBuf+5];
	double dBcPosR;
	int dBcXR;	
	
	double dBdL[dscBuf+5];
	double dBdPosL;
	int dBdXL;	
	
	double dBdR[dscBuf+5];
	double dBdPosR;
	int dBdXR;	
	
	double dBeL[dscBuf+5];
	double dBePosL;
	int dBeXL;	
	
	double dBeR[dscBuf+5];
	double dBePosR;
	int dBeXR;	
	
	enum {
		bez_AL,
		bez_AR,
		bez_BL,
		bez_BR,
		bez_CL,
		bez_CR,	
		bez_InL,
		bez_InR,
		bez_UnInL,
		bez_UnInR,
		bez_SampL,
		bez_SampR,
		bez_cycle,
		bez_total
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bez[bez_total];
	
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif