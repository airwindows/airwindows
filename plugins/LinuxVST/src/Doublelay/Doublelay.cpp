/* ========================================
 *  Doublelay - Doublelay.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Doublelay_H
#include "Doublelay.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Doublelay(audioMaster);}

Doublelay::Doublelay(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.2;
	B = 0.1;
	C = 0.2;
	D = 0.0;
	E = 0.6;
	
	for(int count = 0; count < 48009; count++) {dL[count] = 0.0; dR[count] = 0.0;}
	dcount = 0;
	for(int count = 0; count < 5009; count++) {pL[count] = 0.0; pR[count] = 0.0;}
	for(int count = 0; count < 8; count++)
	{tempL[count] = 0.0; positionL[count] = 0.0; lastpositionL[count] = 0.0; trackingL[count] = 0.0;}
	for(int count = 0; count < 8; count++)
	{tempR[count] = 0.0; positionR[count] = 0.0; lastpositionR[count] = 0.0; trackingR[count] = 0.0;}
	gcountL = 0;
	lastcountL = 0;
	gcountR = 0;
	lastcountR = 0;
	prevwidth = 0;
	feedbackL = 0.0;
	feedbackR = 0.0;
	activeL = 0;
	bestspliceL = 4;
	activeR = 0;
	bestspliceR = 4;
	bestyetL = 1.0;
	bestyetR = 1.0;
	airPrevL = 0.0;
	airEvenL = 0.0;
	airOddL = 0.0;
	airFactorL = 0.0;
	airPrevR = 0.0;
	airEvenR = 0.0;
	airOddR = 0.0;
	airFactorR = 0.0;
	flip = false;
	
	for(int count = 0; count < 6; count++) {lastRefL[count] = 0.0;lastRefR[count] = 0.0;}
	cycle = 0;
	
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

Doublelay::~Doublelay() {}
VstInt32 Doublelay::getVendorVersion () {return 1000;}
void Doublelay::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Doublelay::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 Doublelay::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	chunkData[3] = D;
	chunkData[4] = E;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 Doublelay::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	D = pinParameter(chunkData[3]);
	E = pinParameter(chunkData[4]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void Doublelay::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Doublelay::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Doublelay::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Detune", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Delay L", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Delay R", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Feedbk", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Doublelay::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((A*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Doublelay::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "sec", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "sec", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Doublelay::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Doublelay::getEffectName(char* name) {
    vst_strncpy(name, "Doublelay", kVstMaxProductStrLen); return true;
}

VstPlugCategory Doublelay::getPlugCategory() {return kPlugCategEffect;}

bool Doublelay::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Doublelay", kVstMaxProductStrLen); return true;
}

bool Doublelay::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
