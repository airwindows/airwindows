/* ========================================
 *  kCosmos - kCosmos.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __kCosmos_H
#include "kCosmos.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new kCosmos(audioMaster);}

kCosmos::kCosmos(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 1.0;
	C = 0.5;
	D = 0.5;
	E = 0.0;
	F = 0.5;
	
	for(int count = 0; count < delayA+2; count++) {eAL[count] = 0.0; eAR[count] = 0.0; aAL[count] = 0.0; aAR[count] = 0.0;}
	for(int count = 0; count < delayB+2; count++) {eBL[count] = 0.0; eBR[count] = 0.0; aBL[count] = 0.0; aBR[count] = 0.0;}
	for(int count = 0; count < delayC+2; count++) {eCL[count] = 0.0; eCR[count] = 0.0; aCL[count] = 0.0; aCR[count] = 0.0;}
	for(int count = 0; count < delayD+2; count++) {eDL[count] = 0.0; eDR[count] = 0.0; aDL[count] = 0.0; aDR[count] = 0.0;}
	for(int count = 0; count < delayE+2; count++) {eEL[count] = 0.0; eER[count] = 0.0; aEL[count] = 0.0; aER[count] = 0.0;}
	for(int count = 0; count < delayF+2; count++) {eFL[count] = 0.0; eFR[count] = 0.0; aFL[count] = 0.0; aFR[count] = 0.0;}
	for(int count = 0; count < delayG+2; count++) {eGL[count] = 0.0; eGR[count] = 0.0; aGL[count] = 0.0; aGR[count] = 0.0;}
	for(int count = 0; count < delayH+2; count++) {eHL[count] = 0.0; eHR[count] = 0.0; aHL[count] = 0.0; aHR[count] = 0.0;}
	for(int count = 0; count < delayI+2; count++) {eIL[count] = 0.0; eIR[count] = 0.0; aIL[count] = 0.0; aIR[count] = 0.0;}
	for(int count = 0; count < delayJ+2; count++) {eJL[count] = 0.0; eJR[count] = 0.0; aJL[count] = 0.0; aJR[count] = 0.0;}
	for(int count = 0; count < delayK+2; count++) {eKL[count] = 0.0; eKR[count] = 0.0; aKL[count] = 0.0; aKR[count] = 0.0;}
	for(int count = 0; count < delayL+2; count++) {eLL[count] = 0.0; eLR[count] = 0.0; aLL[count] = 0.0; aLR[count] = 0.0;}
	for(int count = 0; count < delayM+2; count++) {eML[count] = 0.0; eMR[count] = 0.0; aML[count] = 0.0; aMR[count] = 0.0;}
	for(int count = 0; count < delayN+2; count++) {eNL[count] = 0.0; eNR[count] = 0.0; aNL[count] = 0.0; aNR[count] = 0.0;}
	for(int count = 0; count < delayO+2; count++) {eOL[count] = 0.0; eOR[count] = 0.0; aOL[count] = 0.0; aOR[count] = 0.0;}
	for(int count = 0; count < delayP+2; count++) {ePL[count] = 0.0; ePR[count] = 0.0; aPL[count] = 0.0; aPR[count] = 0.0;}
	for(int count = 0; count < delayQ+2; count++) {eQL[count] = 0.0; eQR[count] = 0.0; aQL[count] = 0.0; aQR[count] = 0.0;}
	for(int count = 0; count < delayR+2; count++) {eRL[count] = 0.0; eRR[count] = 0.0; aRL[count] = 0.0; aRR[count] = 0.0;}
	for(int count = 0; count < delayS+2; count++) {eSL[count] = 0.0; eSR[count] = 0.0; aSL[count] = 0.0; aSR[count] = 0.0;}
	for(int count = 0; count < delayT+2; count++) {eTL[count] = 0.0; eTR[count] = 0.0; aTL[count] = 0.0; aTR[count] = 0.0;}
	for(int count = 0; count < delayU+2; count++) {eUL[count] = 0.0; eUR[count] = 0.0; aUL[count] = 0.0; aUR[count] = 0.0;}
	for(int count = 0; count < delayV+2; count++) {eVL[count] = 0.0; eVR[count] = 0.0; aVL[count] = 0.0; aVR[count] = 0.0;}
	for(int count = 0; count < delayW+2; count++) {eWL[count] = 0.0; eWR[count] = 0.0; aWL[count] = 0.0; aWR[count] = 0.0;}
	for(int count = 0; count < delayX+2; count++) {eXL[count] = 0.0; eXR[count] = 0.0; aXL[count] = 0.0; aXR[count] = 0.0;}
	for(int count = 0; count < delayY+2; count++) {eYL[count] = 0.0; eYR[count] = 0.0; aYL[count] = 0.0; aYR[count] = 0.0;}
	for(int count = 0; count < predelay+2; count++) {aZL[count] = 0.0; aZR[count] = 0.0;}
	
	feedbackAL = 0.0;
	feedbackBL = 0.0;
	feedbackCL = 0.0;
	feedbackDL = 0.0;
	feedbackEL = 0.0;
	feedbackER = 0.0;
	feedbackJR = 0.0;
	feedbackOR = 0.0;
	feedbackTR = 0.0;
	feedbackYR = 0.0;
	
	countAL = 1;
	countBL = 1;
	countCL = 1;
	countDL = 1;	
	countEL = 1;
	countFL = 1;
	countGL = 1;
	countHL = 1;
	countIL = 1;
	countJL = 1;
	countKL = 1;
	countLL = 1;
	countML = 1;
	countNL = 1;
	countOL = 1;
	countPL = 1;
	countQL = 1;
	countRL = 1;
	countSL = 1;
	countTL = 1;
	countUL = 1;
	countVL = 1;
	countWL = 1;
	countXL = 1;
	countYL = 1;
	
	countAR = 1;
	countBR = 1;
	countCR = 1;
	countDR = 1;	
	countER = 1;
	countFR = 1;
	countGR = 1;
	countHR = 1;
	countIR = 1;
	countJR = 1;
	countKR = 1;
	countLR = 1;
	countMR = 1;
	countNR = 1;
	countOR = 1;
	countPR = 1;
	countQR = 1;
	countRR = 1;
	countSR = 1;
	countTR = 1;
	countUR = 1;
	countVR = 1;
	countWR = 1;
	countXR = 1;
	countYR = 1;
	
	countZ = 1;	
	
	for (int x = 0; x < bez_total; x++) bez[x] = 0.0;
	bez[bez_cycle] = 1.0;
	
	for(int count = 0; count < 32767; count++) {firBufferL[count] = 0.0; firBufferR[count] = 0.0;}
	firPosition = 0;
	
	earlyReflectionL = 0.0; earlyReflectionR = 0.0;
	prevAL = 0.0;
	prevBL = 0.0;
	prevCL = 0.0;
	prevDL = 0.0;
	prevEL = 0.0;
	prevER = 0.0;
	prevJR = 0.0;
	prevOR = 0.0;
	prevTR = 0.0;
	prevYR = 0.0;
	
	derezA = derezB = 0.0;

	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	//this is reset: values being initialized only once. Startup values, whatever they are.
	
    _canDo.insert("plugAsChannelInsert"); // plug-in can be used as a channel insert effect.
    _canDo.insert("plugAsSend"); // plug-in can be used as a send effect.
    _canDo.insert("x2in2out"); 
    setNumInputs(kNumInputs);
    setNumOutputs(kNumOutputs);
    setUniqueID(kUniqueId);
    canProcessReplacing();     // supports output replacing
    canDoubleReplacing();      // supports double precision processing
	programsAreChunks(true);
    vst_strncpy (_programName, "Default", kVstMaxProgNameLen); // default program name
}

kCosmos::~kCosmos() {}
VstInt32 kCosmos::getVendorVersion () {return 1000;}
void kCosmos::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void kCosmos::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 kCosmos::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	chunkData[3] = D;
	chunkData[4] = E;
	chunkData[5] = F;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 kCosmos::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	D = pinParameter(chunkData[3]);
	E = pinParameter(chunkData[4]);
	F = pinParameter(chunkData[5]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void kCosmos::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float kCosmos::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        case kParamF: return F; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void kCosmos::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Regen", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Derez", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Filter", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "EarlyRF", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Predlay", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void kCosmos::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void kCosmos::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 kCosmos::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool kCosmos::getEffectName(char* name) {
    vst_strncpy(name, "kCosmos", kVstMaxProductStrLen); return true;
}

VstPlugCategory kCosmos::getPlugCategory() {return kPlugCategEffect;}

bool kCosmos::getProductString(char* text) {
  	vst_strncpy (text, "airwindows kCosmos", kVstMaxProductStrLen); return true;
}

bool kCosmos::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
