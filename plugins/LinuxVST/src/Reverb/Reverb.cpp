/* ========================================
 *  Reverb - Reverb.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Reverb_H
#include "Reverb.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Reverb(audioMaster);}

Reverb::Reverb(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for (int x = 0; x < 11; x++) {biquadA[x] = 0.0;biquadB[x] = 0.0;biquadC[x] = 0.0;}
	
	feedbackAL = feedbackAR = 0.0;
	feedbackBL = feedbackBR = 0.0;
	feedbackCL = feedbackCR = 0.0;
	feedbackDL = feedbackDR = 0.0;
	feedbackEL = feedbackER = 0.0;
	feedbackFL = feedbackFR = 0.0;
	feedbackGL = feedbackGR = 0.0;
	feedbackHL = feedbackHR = 0.0;
	
	int count;
	for(count = 0; count < 8110; count++) {aAL[count] = aAR[count] = 0.0;}
	for(count = 0; count < 7510; count++) {aBL[count] = aBR[count] = 0.0;}
	for(count = 0; count < 7310; count++) {aCL[count] = aCR[count] = 0.0;}
	for(count = 0; count < 6910; count++) {aDL[count] = aDR[count] = 0.0;}
	for(count = 0; count < 6310; count++) {aEL[count] = aER[count] = 0.0;}
	for(count = 0; count < 6110; count++) {aFL[count] = aFR[count] = 0.0;}
	for(count = 0; count < 5510; count++) {aGL[count] = aGR[count] = 0.0;}
	for(count = 0; count < 4910; count++) {aHL[count] = aHR[count] = 0.0;}
	//maximum value needed will be delay * 100, plus 206 (absolute max vibrato depth)
	for(count = 0; count < 4510; count++) {aIL[count] = aIR[count] = 0.0;}
	for(count = 0; count < 4310; count++) {aJL[count] = aJR[count] = 0.0;}
	for(count = 0; count < 3910; count++) {aKL[count] = aKR[count] = 0.0;}
	for(count = 0; count < 3310; count++) {aLL[count] = aLR[count] = 0.0;}
	//maximum value will be delay * 100
	for(count = 0; count < 3110; count++) {aML[count] = aMR[count] = 0.0;}	
	//maximum value will be delay * 100
	countA = 1; delayA = 79;
	countB = 1; delayB = 73;
	countC = 1; delayC = 71;
	countD = 1; delayD = 67;	
	countE = 1; delayE = 61;
	countF = 1; delayF = 59;
	countG = 1; delayG = 53;
	countH = 1; delayH = 47;
	//the householder matrices
	countI = 1; delayI = 43;
	countJ = 1; delayJ = 41;
	countK = 1; delayK = 37;
	countL = 1; delayL = 31;
	//the allpasses
	countM = 1; delayM = 29;
	//the predelay
	depthA = 0.003251;
	depthB = 0.002999;
	depthC = 0.002917;
	depthD = 0.002749;
	depthE = 0.002503;
	depthF = 0.002423;
	depthG = 0.002146;
	depthH = 0.002088;
	//the individual vibrato rates for the delays
	vibAL = rand()*-2147483647;
	vibBL = rand()*-2147483647;
	vibCL = rand()*-2147483647;
	vibDL = rand()*-2147483647;
	vibEL = rand()*-2147483647;
	vibFL = rand()*-2147483647;
	vibGL = rand()*-2147483647;
	vibHL = rand()*-2147483647;
	
	vibAR = rand()*-2147483647;
	vibBR = rand()*-2147483647;
	vibCR = rand()*-2147483647;
	vibDR = rand()*-2147483647;
	vibER = rand()*-2147483647;
	vibFR = rand()*-2147483647;
	vibGR = rand()*-2147483647;
	vibHR = rand()*-2147483647;

	A = 0.5;
	B = 0.5;
	
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

Reverb::~Reverb() {}
VstInt32 Reverb::getVendorVersion () {return 1000;}
void Reverb::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Reverb::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 Reverb::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 Reverb::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void Reverb::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Reverb::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
		default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Reverb::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Big", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Reverb::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Reverb::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Reverb::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Reverb::getEffectName(char* name) {
    vst_strncpy(name, "Reverb", kVstMaxProductStrLen); return true;
}

VstPlugCategory Reverb::getPlugCategory() {return kPlugCategEffect;}

bool Reverb::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Reverb", kVstMaxProductStrLen); return true;
}

bool Reverb::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
