/* ========================================
 *  PurestConsole3Buss - PurestConsole3Buss.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestConsole3Buss_H
#include "PurestConsole3Buss.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new PurestConsole3Buss(audioMaster);}

PurestConsole3Buss::PurestConsole3Buss(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
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

PurestConsole3Buss::~PurestConsole3Buss() {}
VstInt32 PurestConsole3Buss::getVendorVersion () {return 1000;}
void PurestConsole3Buss::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void PurestConsole3Buss::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!


VstInt32 PurestConsole3Buss::getChunk (void** data, bool isPreset)
{
	return kNumParameters * sizeof(float);
}

VstInt32 PurestConsole3Buss::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	return 0;
}

void PurestConsole3Buss::setParameter(VstInt32 index, float value) {
}

float PurestConsole3Buss::getParameter(VstInt32 index) {
    return 0.0; //we only need to update the relevant name, this is simple to manage
}

void PurestConsole3Buss::getParameterName(VstInt32 index, char *text) {
}

void PurestConsole3Buss::getParameterDisplay(VstInt32 index, char *text) {
}

void PurestConsole3Buss::getParameterLabel(VstInt32 index, char *text) {
}

VstInt32 PurestConsole3Buss::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool PurestConsole3Buss::getEffectName(char* name) {
    vst_strncpy(name, "PurestConsole3Buss", kVstMaxProductStrLen); return true;
}

VstPlugCategory PurestConsole3Buss::getPlugCategory() {return kPlugCategEffect;}

bool PurestConsole3Buss::getProductString(char* text) {
  	vst_strncpy (text, "airwindows PurestConsole3Buss", kVstMaxProductStrLen); return true;
}

bool PurestConsole3Buss::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
