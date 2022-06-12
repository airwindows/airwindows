/* ========================================
 *  Console8BussIn - Console8BussIn.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#ifndef __Console8BussIn_H
#include "Console8BussIn.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Console8BussIn(audioMaster);}

Console8BussIn::Console8BussIn(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	iirAL = 0.0; iirBL = 0.0; iirAR = 0.0; iirBR = 0.0;
	for (int x = 0; x < fix_total; x++) fix[x] = 0.0;
	for (int x = 0; x < 10; x++) {softL[x] = 0.0; softR[x] = 0.0;}
	if (getSampleRate() > 49000.0) hsr = true;
	else hsr = false;
	fix[fix_freq] = 24000.0 / getSampleRate();
    fix[fix_reso] = 0.5;
	double K = tan(M_PI * fix[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / fix[fix_reso] + K * K);
	fix[fix_a0] = K * K * norm;
	fix[fix_a1] = 2.0 * fix[fix_a0];
	fix[fix_a2] = fix[fix_a0];
	fix[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fix[fix_b2] = (1.0 - K / fix[fix_reso] + K * K) * norm;
	//this is the fixed biquad distributed anti-aliasing filter
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd == 3) cycleEnd = 4;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 4 for 176 or 192k
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

Console8BussIn::~Console8BussIn() {}
VstInt32 Console8BussIn::getVendorVersion () {return 1000;}
void Console8BussIn::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Console8BussIn::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

VstInt32 Console8BussIn::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	return kNumParameters * sizeof(float);
}

VstInt32 Console8BussIn::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	return 0;
}

void Console8BussIn::setParameter(VstInt32 index, float value) {
    switch (index) {
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Console8BussIn::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Console8BussIn::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Console8BussIn::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Console8BussIn::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Console8BussIn::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Console8BussIn::getEffectName(char* name) {
    vst_strncpy(name, "Console8BussIn", kVstMaxProductStrLen); return true;
}

VstPlugCategory Console8BussIn::getPlugCategory() {return kPlugCategEffect;}

bool Console8BussIn::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Console8BussIn", kVstMaxProductStrLen); return true;
}

bool Console8BussIn::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
