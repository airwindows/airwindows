/* ========================================
 *  FinalClip - FinalClip.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __FinalClip_H
#include "FinalClip.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new FinalClip(audioMaster);}

FinalClip::FinalClip(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	lastSampleL = 0.0;
	wasPosClipL = false;
	wasNegClipL = false;
	lastSampleR = 0.0;
	wasPosClipR = false;
	wasNegClipR = false;
	for (int x = 0; x < 16; x++) {intermediateL[x] = 0.0; intermediateR[x] = 0.0;}
	//fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	//fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX; //leave off for ClipOnly
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

FinalClip::~FinalClip() {}
VstInt32 FinalClip::getVendorVersion () {return 1000;}
void FinalClip::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void FinalClip::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

VstInt32 FinalClip::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 FinalClip::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	return 0;
}

void FinalClip::setParameter(VstInt32 index, float value) {
    switch (index) {
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float FinalClip::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void FinalClip::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void FinalClip::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void FinalClip::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 FinalClip::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool FinalClip::getEffectName(char* name) {
    vst_strncpy(name, "FinalClip", kVstMaxProductStrLen); return true;
}

VstPlugCategory FinalClip::getPlugCategory() {return kPlugCategEffect;}

bool FinalClip::getProductString(char* text) {
  	vst_strncpy (text, "airwindows FinalClip", kVstMaxProductStrLen); return true;
}

bool FinalClip::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
