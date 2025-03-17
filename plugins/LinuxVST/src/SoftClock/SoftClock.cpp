/* ========================================
 *  SoftClock - SoftClock.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SoftClock_H
#include "SoftClock.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new SoftClock(audioMaster);}

SoftClock::SoftClock(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.2;
	B = 0.2;
	C = 0.2;
	D = 0.0;
	E = 0.0;
	F = 0.5;
	G = 0.5;
	H = 0.5;

	sinePos = 0.0;
	barPos = 0.0;
	beatPos = 0;
	for (int x = 0; x < 34; x++) {beatAccent[x] = 0.0; beatSwing[x] = 0.0;}
	inc = 0.0;
	beatTable[0]=0;
	beatTable[1]=1;
	beatTable[2]=2;
	beatTable[3]=3;
	beatTable[4]=4;
	beatTable[5]=5;
	beatTable[6]=6;
	beatTable[7]=7;
	beatTable[8]=8;
	beatTable[9]=9;
	beatTable[10]=10;
	beatTable[11]=11;
	beatTable[12]=11;
	beatTable[13]=11;
	beatTable[14]=11;
	beatTable[15]=13;
	beatTable[16]=16;
	beatTable[17]=13;
	beatTable[18]=13;
	beatTable[19]=17;
	beatTable[20]=17;
	beatTable[21]=17;
	beatTable[22]=17;
	beatTable[23]=19;
	beatTable[24]=24;
	beatTable[25]=19;
	beatTable[26]=19;
	beatTable[27]=19;
	beatTable[28]=23;
	beatTable[29]=23;
	beatTable[30]=23;
	beatTable[31]=23;
	beatTable[32]=32;
	beatTable[33]=32;
	beatTable[34]=32;
	
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

SoftClock::~SoftClock() {}
VstInt32 SoftClock::getVendorVersion () {return 1000;}
void SoftClock::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void SoftClock::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 SoftClock::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	chunkData[3] = D;
	chunkData[4] = E;
	chunkData[5] = F;
	chunkData[6] = G;
	chunkData[7] = H;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 SoftClock::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	D = pinParameter(chunkData[3]);
	E = pinParameter(chunkData[4]);
	F = pinParameter(chunkData[5]);
	G = pinParameter(chunkData[6]);
	H = pinParameter(chunkData[7]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void SoftClock::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        case kParamG: G = value; break;
        case kParamH: H = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float SoftClock::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        case kParamF: return F; break;
        case kParamG: return G; break;
        case kParamH: return H; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void SoftClock::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Tempo", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Count", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Tuple", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Swing", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "BigBeat", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Accents", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "Boost", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "Speed", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void SoftClock::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: int2string ((int)(A*200.99)+40, text, kVstMaxParamStrLen); break;
        case kParamB: int2string ((int)(B*32.99), text, kVstMaxParamStrLen); break;
        case kParamC: int2string ((int)(C*16.99), text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        case kParamG: float2string (G, text, kVstMaxParamStrLen); break;
        case kParamH: float2string (H, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void SoftClock::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamH: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 SoftClock::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool SoftClock::getEffectName(char* name) {
    vst_strncpy(name, "SoftClock", kVstMaxProductStrLen); return true;
}

VstPlugCategory SoftClock::getPlugCategory() {return kPlugCategEffect;}

bool SoftClock::getProductString(char* text) {
  	vst_strncpy (text, "airwindows SoftClock", kVstMaxProductStrLen); return true;
}

bool SoftClock::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
