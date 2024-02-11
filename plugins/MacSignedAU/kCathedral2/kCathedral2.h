/*
*	File:		kCathedral2.h
*	
*	Version:	1.0
* 
*	Created:	1/31/24
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
#include "kCathedral2Version.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __kCathedral2_h__
#define __kCathedral2_h__


#pragma mark ____kCathedral2 Parameters

// parameters
static const float kDefaultValue_ParamOne = 1.0;
static CFStringRef kParameterOneName = CFSTR("Wetness");
//Alter the name if desired, but using the plugin name is a start

enum {
	kParam_One =0,
	//Add your parameters here...
	kNumberOfParameters=1
};

const int predelay = 1014; const int vlfpredelay = 11000;

const int shortA = 78; const int shortB = 760; const int shortC = 982; const int shortD = 528; const int shortE = 445; const int shortF = 1128; const int shortG = 130; const int shortH = 708; const int shortI = 22; const int shortJ = 2144; const int shortK = 354; const int shortL = 1169; const int shortM = 11; const int shortN = 2782; const int shortO = 58; const int shortP = 1515; //5 to 159 ms, 809 seat hall. Scarcity, 1 in 212274
//Short809

const int delayA = 871; const int delayB = 1037; const int delayC = 1205; const int delayD = 297; const int delayE = 467; const int delayF = 884; const int delayG = 173; const int delayH = 1456; const int delayI = 799; const int delayJ = 361; const int delayK = 1432; const int delayL = 338; const int delayM = 186; const int delayN = 1408; const int delayO = 1014; const int delayP = 23; const int delayQ = 807; const int delayR = 501; const int delayS = 1468; const int delayT = 1102; const int delayU = 11; const int delayV = 1119; const int delayW = 1315; const int delayX = 94; const int delayY = 1270; //15 to 155 ms, 874 seat hall  
//874b-U rated incompressible if filesize larger than 25,298,231 bytes

#pragma mark ____kCathedral2
class kCathedral2 : public AUEffectBase
{
public:
	kCathedral2(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~kCathedral2 () { delete mDebugDispatcher; }
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
	virtual ComponentResult		Version() { return kkCathedral2Version; }
	
		private: 
	double gainOutL;
 	double gainOutR;
	
	
	double eAL[shortA+5];
	double eBL[shortB+5];
	double eCL[shortC+5];
	double eDL[shortD+5];
	double eEL[shortE+5];
	double eFL[shortF+5];
	double eGL[shortG+5];
	double eHL[shortH+5];
	double eIL[shortI+5];
	double eJL[shortJ+5];
	double eKL[shortK+5];
	double eLL[shortL+5];
	double eML[shortM+5];
	double eNL[shortN+5];
	double eOL[shortO+5];
	double ePL[shortP+5];
	
	double eAR[shortA+5];
	double eBR[shortB+5];
	double eCR[shortC+5];
	double eDR[shortD+5];
	double eER[shortE+5];
	double eFR[shortF+5];
	double eGR[shortG+5];
	double eHR[shortH+5];
	double eIR[shortI+5];
	double eJR[shortJ+5];
	double eKR[shortK+5];
	double eLR[shortL+5];
	double eMR[shortM+5];
	double eNR[shortN+5];
	double eOR[shortO+5];
	double ePR[shortP+5];

	int shortAL;
	int shortBL;
	int shortCL;
	int shortDL;
	int shortEL;
	int shortFL;
	int shortGL;
	int shortHL;
	int shortIL;
	int shortJL;
	int shortKL;
	int shortLL;		
	int shortML;		
	int shortNL;		
	int shortOL;		
	int shortPL;		
	
	int shortAR;
	int shortBR;
	int shortCR;
	int shortDR;
	int shortER;
	int shortFR;
	int shortGR;
	int shortHR;
	int shortIR;
	int shortJR;
	int shortKR;
	int shortLR;		
	int shortMR;		
	int shortNR;		
	int shortOR;		
	int shortPR;	
	
	
	
		
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
		
	double lastRefL[7];
	double lastRefR[7];
	
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
	
	int cycle;
	
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
	
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif