/* ========================================
 *  UltrasonicMed - UltrasonicMed.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __UltrasonicMed_H
#include "UltrasonicMed.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new UltrasonicMed(audioMaster);}

UltrasonicMed::UltrasonicMed(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for (int x = 0; x < 15; x++) {biquadA[x] = 0.0;biquadB[x] = 0.0;}
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

UltrasonicMed::~UltrasonicMed() {}
VstInt32 UltrasonicMed::getVendorVersion () {return 1000;}
void UltrasonicMed::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void UltrasonicMed::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 UltrasonicMed::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 UltrasonicMed::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	return 0;
}

void UltrasonicMed::setParameter(VstInt32 index, float value) {
    switch (index) {
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float UltrasonicMed::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void UltrasonicMed::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void UltrasonicMed::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void UltrasonicMed::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 UltrasonicMed::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool UltrasonicMed::getEffectName(char* name) {
    vst_strncpy(name, "UltrasonicMed", kVstMaxProductStrLen); return true;
}

VstPlugCategory UltrasonicMed::getPlugCategory() {return kPlugCategEffect;}

bool UltrasonicMed::getProductString(char* text) {
  	vst_strncpy (text, "airwindows UltrasonicMed", kVstMaxProductStrLen); return true;
}

bool UltrasonicMed::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
