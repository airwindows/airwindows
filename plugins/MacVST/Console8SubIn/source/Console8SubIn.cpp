/* ========================================
 *  Console8SubIn - Console8SubIn.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#ifndef __Console8SubIn_H
#include "Console8SubIn.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Console8SubIn(audioMaster);}

Console8SubIn::Console8SubIn(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	iirAL = 0.0; iirBL = 0.0; iirAR = 0.0; iirBR = 0.0;
	for (int x = 0; x < fix_total; x++) fix[x] = 0.0;
	for (int x = 0; x < 10; x++) {softL[x] = 0.0; softR[x] = 0.0;}
	if (getSampleRate() > 49000.0) hsr = true;
	else hsr = false;
	fix[fix_freq] = 24000.0 / getSampleRate();
    fix[fix_reso] = 0.59435114;
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

Console8SubIn::~Console8SubIn() {}
VstInt32 Console8SubIn::getVendorVersion () {return 1000;}
void Console8SubIn::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Console8SubIn::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

VstInt32 Console8SubIn::getChunk (void** data, bool isPreset)
{
	return kNumParameters * sizeof(float);
}

VstInt32 Console8SubIn::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	return 0;
}

void Console8SubIn::setParameter(VstInt32 index, float value) {
    switch (index) {
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Console8SubIn::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Console8SubIn::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Console8SubIn::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Console8SubIn::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Console8SubIn::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Console8SubIn::getEffectName(char* name) {
    vst_strncpy(name, "Console8SubIn", kVstMaxProductStrLen); return true;
}

VstPlugCategory Console8SubIn::getPlugCategory() {return kPlugCategEffect;}

bool Console8SubIn::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Console8SubIn", kVstMaxProductStrLen); return true;
}

bool Console8SubIn::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
