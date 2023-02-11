/* ========================================
 *  Console8LiteBuss - Console8LiteBuss.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Console8LiteBuss_H
#include "Console8LiteBuss.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Console8LiteBuss(audioMaster);}

Console8LiteBuss::Console8LiteBuss(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	
	iirAL = 0.0; iirBL = 0.0; iirAR = 0.0; iirBR = 0.0;
	for (int x = 0; x < fix_total; x++) {fix[x] = 0.0; fixB[x] = 0.0;}
	for (int x = 0; x < 10; x++) {softL[x] = 0.0; softR[x] = 0.0;}
	
	inTrimA = 0.5; inTrimB = 0.5;
	lastSampleL = 0.0; wasPosClipL = false; wasNegClipL = false;
	lastSampleR = 0.0; wasPosClipR = false; wasNegClipR = false;
	for (int x = 0; x < 17; x++) {intermediateL[x] = 0.0; intermediateR[x] = 0.0;} //ADClip2
	
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

Console8LiteBuss::~Console8LiteBuss() {}
VstInt32 Console8LiteBuss::getVendorVersion () {return 1000;}
void Console8LiteBuss::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Console8LiteBuss::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 Console8LiteBuss::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 Console8LiteBuss::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void Console8LiteBuss::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Console8LiteBuss::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Console8LiteBuss::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Fader", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Console8LiteBuss::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Console8LiteBuss::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Console8LiteBuss::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Console8LiteBuss::getEffectName(char* name) {
    vst_strncpy(name, "Console8LiteBuss", kVstMaxProductStrLen); return true;
}

VstPlugCategory Console8LiteBuss::getPlugCategory() {return kPlugCategEffect;}

bool Console8LiteBuss::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Console8LiteBuss", kVstMaxProductStrLen); return true;
}

bool Console8LiteBuss::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
