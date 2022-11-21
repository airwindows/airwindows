/* ========================================
 *  PurestConsole2Buss - PurestConsole2Buss.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestConsole2Buss_H
#include "PurestConsole2Buss.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new PurestConsole2Buss(audioMaster);}

PurestConsole2Buss::PurestConsole2Buss(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for (int x = 0; x < 15; x++) {biquadA[x] = 0.0;}
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

PurestConsole2Buss::~PurestConsole2Buss() {}
VstInt32 PurestConsole2Buss::getVendorVersion () {return 1000;}
void PurestConsole2Buss::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void PurestConsole2Buss::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

VstInt32 PurestConsole2Buss::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 PurestConsole2Buss::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	return 0;
}

void PurestConsole2Buss::setParameter(VstInt32 index, float value) {
    switch (index) {
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float PurestConsole2Buss::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void PurestConsole2Buss::getParameterName(VstInt32 index, char *text) {
    switch (index) {
       default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void PurestConsole2Buss::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void PurestConsole2Buss::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 PurestConsole2Buss::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool PurestConsole2Buss::getEffectName(char* name) {
    vst_strncpy(name, "PurestConsole2Buss", kVstMaxProductStrLen); return true;
}

VstPlugCategory PurestConsole2Buss::getPlugCategory() {return kPlugCategEffect;}

bool PurestConsole2Buss::getProductString(char* text) {
  	vst_strncpy (text, "airwindows PurestConsole2Buss", kVstMaxProductStrLen); return true;
}

bool PurestConsole2Buss::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
