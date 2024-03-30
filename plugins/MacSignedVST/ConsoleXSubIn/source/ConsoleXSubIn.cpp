/* ========================================
 *  ConsoleXSubIn - ConsoleXSubIn.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleXSubIn_H
#include "ConsoleXSubIn.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ConsoleXSubIn(audioMaster);}

ConsoleXSubIn::ConsoleXSubIn(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for (int x = 0; x < biq_total; x++) {biquad[x] = 0.0;}
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

ConsoleXSubIn::~ConsoleXSubIn() {}
VstInt32 ConsoleXSubIn::getVendorVersion () {return 1000;}
void ConsoleXSubIn::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ConsoleXSubIn::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!


VstInt32 ConsoleXSubIn::getChunk (void** data, bool isPreset)
{
	return kNumParameters * sizeof(float);
}

VstInt32 ConsoleXSubIn::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	return 0;
}

void ConsoleXSubIn::setParameter(VstInt32 index, float value) {
}

float ConsoleXSubIn::getParameter(VstInt32 index) {
	return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ConsoleXSubIn::getParameterName(VstInt32 index, char *text) {
}

void ConsoleXSubIn::getParameterDisplay(VstInt32 index, char *text) {
}

void ConsoleXSubIn::getParameterLabel(VstInt32 index, char *text) {
}

VstInt32 ConsoleXSubIn::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ConsoleXSubIn::getEffectName(char* name) {
    vst_strncpy(name, "ConsoleXSubIn", kVstMaxProductStrLen); return true;
}

VstPlugCategory ConsoleXSubIn::getPlugCategory() {return kPlugCategEffect;}

bool ConsoleXSubIn::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ConsoleXSubIn", kVstMaxProductStrLen); return true;
}

bool ConsoleXSubIn::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
