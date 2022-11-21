/* ========================================
 *  ClipOnly2 - ClipOnly2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ClipOnly2_H
#include "ClipOnly2.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ClipOnly2(audioMaster);}

ClipOnly2::ClipOnly2(audioMasterCallback audioMaster) :
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

ClipOnly2::~ClipOnly2() {}
VstInt32 ClipOnly2::getVendorVersion () {return 1000;}
void ClipOnly2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ClipOnly2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

VstInt32 ClipOnly2::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 ClipOnly2::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	return 0;
}

void ClipOnly2::setParameter(VstInt32 index, float value) {
    switch (index) {
		default: throw; // unknown parameter, shouldn't happen!
    }
}

float ClipOnly2::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ClipOnly2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ClipOnly2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ClipOnly2::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ClipOnly2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ClipOnly2::getEffectName(char* name) {
    vst_strncpy(name, "ClipOnly2", kVstMaxProductStrLen); return true;
}

VstPlugCategory ClipOnly2::getPlugCategory() {return kPlugCategEffect;}

bool ClipOnly2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ClipOnly2", kVstMaxProductStrLen); return true;
}

bool ClipOnly2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
