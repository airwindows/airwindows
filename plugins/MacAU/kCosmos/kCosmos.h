/*
*	File:		kCosmos.h
*	
*	Version:	1.0
* 
*	Created:	4/29/25
*	
*	Copyright:  Copyright © 2025 Airwindows, Airwindows uses the MIT license
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
#include "kCosmosVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __kCosmos_h__
#define __kCosmos_h__


#pragma mark ____kCosmos Parameters

// parameters
static const float kDefaultValue_ParamA = 0.5;
static const float kDefaultValue_ParamB = 1.0;
static const float kDefaultValue_ParamC = 0.5;
static const float kDefaultValue_ParamD = 0.5;
static const float kDefaultValue_ParamE = 0.0;
static const float kDefaultValue_ParamF = 0.5;

static CFStringRef kParameterAName = CFSTR("Regen");
static CFStringRef kParameterBName = CFSTR("Derez");
static CFStringRef kParameterCName = CFSTR("Filter");
static CFStringRef kParameterDName = CFSTR("EarlyRf");
static CFStringRef kParameterEName = CFSTR("Predlay");
static CFStringRef kParameterFName = CFSTR("Dry/Wet");

enum {
	kParam_A =0,
	kParam_B =1,
	kParam_C =2,
	kParam_D =3,
	kParam_E =4,
	kParam_F =5,
	//Add your parameters here...
	kNumberOfParameters=6
};

const int predelay = 15000;

const int delayA = 857; const int delayB = 1433; const int delayC = 1597; const int delayD = 1789; const int delayE = 1987; const int delayF = 373; const int delayG = 883; const int delayH = 1471; const int delayI = 1601; const int delayJ = 1973; const int delayK = 191; const int delayL = 397; const int delayM = 941; const int delayN = 1483; const int delayO = 1663; const int delayP = 149; const int delayQ = 227; const int delayR = 593; const int delayS = 1061; const int delayT = 1549; const int delayU = 137; const int delayV = 167; const int delayW = 313; const int delayX = 641; const int delayY = 1153; //38 to 188 ms, 1538 seat hall  
//1538-OGBP 2025-05-05 - kCosmos



#pragma mark ____kCosmos
class kCosmos : public AUEffectBase
{
public:
	kCosmos(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~kCosmos () { delete mDebugDispatcher; }
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
	virtual ComponentResult		Version() { return kkCosmosVersion; }
	
	private:
	
	double eAL[delayA+5];
	double eBL[delayB+5];
	double eCL[delayC+5];
	double eDL[delayD+5];
	double eEL[delayE+5];
	double eFL[delayF+5];
	double eGL[delayG+5];
	double eHL[delayH+5];
	double eIL[delayI+5];
	double eJL[delayJ+5];
	double eKL[delayK+5];
	double eLL[delayL+5];
	double eML[delayM+5];
	double eNL[delayN+5];
	double eOL[delayO+5];
	double ePL[delayP+5];
	double eQL[delayQ+5];
	double eRL[delayR+5];
	double eSL[delayS+5];
	double eTL[delayT+5];
	double eUL[delayU+5];
	double eVL[delayV+5];
	double eWL[delayW+5];
	double eXL[delayX+5];
	double eYL[delayY+5];
	
	double eAR[delayA+5];
	double eBR[delayB+5];
	double eCR[delayC+5];
	double eDR[delayD+5];
	double eER[delayE+5];
	double eFR[delayF+5];
	double eGR[delayG+5];
	double eHR[delayH+5];
	double eIR[delayI+5];
	double eJR[delayJ+5];
	double eKR[delayK+5];
	double eLR[delayL+5];
	double eMR[delayM+5];
	double eNR[delayN+5];
	double eOR[delayO+5];
	double ePR[delayP+5];
	double eQR[delayQ+5];
	double eRR[delayR+5];
	double eSR[delayS+5];
	double eTR[delayT+5];
	double eUR[delayU+5];
	double eVR[delayV+5];
	double eWR[delayW+5];
	double eXR[delayX+5];
	double eYR[delayY+5];
	
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
	
	double firBufferL[32768];
	double firBufferR[32768];
	int firPosition;
	
	double earlyReflectionL;
	double earlyReflectionR;
	
	double prevAL;
	double prevBL;
	double prevCL;
	double prevDL;
	double prevEL;
	double prevER;
	double prevJR;
	double prevOR;
	double prevTR;
	double prevYR;
	
	double derezA, derezB;
	
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif