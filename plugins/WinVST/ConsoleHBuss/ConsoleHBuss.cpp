/* ========================================
 *  ConsoleHBuss - ConsoleHBuss.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleHBuss_H
#include "ConsoleHBuss.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ConsoleHBuss(audioMaster);}

ConsoleHBuss::ConsoleHBuss(audioMasterCallback audioMaster) :
AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	HIG = 0.5;
	MID = 0.5;
	LOW = 0.5;
		
	THR = 1.0;
	
	LOP = 1.0;
	HIP = 0.0;
	PAN = 0.5;
	FAD = 0.5;
	
	for (int x = 0; x < biq_total; x++) {
		highFast[x] = 0.0;
		lowFast[x] = 0.0;
	}
	highFastLIIR = 0.0;
	highFastRIIR = 0.0;
	lowFastLIIR = 0.0;
	lowFastRIIR = 0.0;
	//SmoothEQ3
		
	for (int x = 0; x < bez_total; x++) {bezCompF[x] = 0.0;bezCompS[x] = 0.0;}
	bezCompF[bez_cycle] = 1.0;
	bezCompS[bez_cycle] = 1.0;
	//Dynamics2
	
	for (int x = 0; x < hilp_total; x++) {
		highpass[x] = 0.0;
		lowpass[x] = 0.0;
	}	
	
	for (int x = 0; x < 33; x++) {avg32L[x] = 0.0; avg32R[x] = 0.0;}
	for (int x = 0; x < 17; x++) {avg16L[x] = 0.0; avg16R[x] = 0.0;}
	for (int x = 0; x < 9; x++) {avg8L[x] = 0.0; avg8R[x] = 0.0;}
	for (int x = 0; x < 5; x++) {avg4L[x] = 0.0; avg4R[x] = 0.0;}
	for (int x = 0; x < 3; x++) {avg2L[x] = 0.0; avg2R[x] = 0.0;}
	avgPos = 0;
	lastSlewL = 0.0; lastSlewR = 0.0;
	lastSlewpleL = 0.0; lastSlewpleR = 0.0;
	//preTapeHack
	
	panA = 0.5; panB = 0.5;
	inTrimA = 0.5; inTrimB = 0.5;
	
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

ConsoleHBuss::~ConsoleHBuss() {}
VstInt32 ConsoleHBuss::getVendorVersion () {return 1000;}
void ConsoleHBuss::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ConsoleHBuss::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 ConsoleHBuss::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = HIG;
	chunkData[1] = MID;
	chunkData[2] = LOW;
	chunkData[3] = THR;
	chunkData[4] = LOP;
	chunkData[5] = HIP;
	chunkData[6] = PAN;
	chunkData[7] = FAD;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 ConsoleHBuss::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	HIG = pinParameter(chunkData[0]);
	MID = pinParameter(chunkData[1]);
	LOW = pinParameter(chunkData[2]);
	THR = pinParameter(chunkData[3]);
	LOP = pinParameter(chunkData[4]);
	HIP = pinParameter(chunkData[5]);
	PAN = pinParameter(chunkData[6]);
	FAD = pinParameter(chunkData[7]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void ConsoleHBuss::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParam_HIG: HIG = value; break;
        case kParam_MID: MID = value; break;
        case kParam_LOW: LOW = value; break;
        case kParam_THR: THR = value; break;
        case kParam_LOP: LOP = value; break;
        case kParam_HIP: HIP = value; break;
        case kParam_PAN: PAN = value; break;
        case kParam_FAD: FAD = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float ConsoleHBuss::getParameter(VstInt32 index) {
    switch (index) {
        case kParam_HIG: return HIG; break;
        case kParam_MID: return MID; break;
        case kParam_LOW: return LOW; break;
        case kParam_THR: return THR; break;
        case kParam_LOP: return LOP; break;
        case kParam_HIP: return HIP; break;
        case kParam_PAN: return PAN; break;
        case kParam_FAD: return FAD; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ConsoleHBuss::getParameterName(VstInt32 index, char *text) {
    switch (index) {
		case kParam_HIG: vst_strncpy (text, "High", kVstMaxParamStrLen); break;
		case kParam_MID: vst_strncpy (text, "Mid", kVstMaxParamStrLen); break;
		case kParam_LOW: vst_strncpy (text, "Low", kVstMaxParamStrLen); break;
		case kParam_THR: vst_strncpy (text, "Thresh", kVstMaxParamStrLen); break;
		case kParam_LOP: vst_strncpy (text, "Lowpass", kVstMaxParamStrLen); break;
		case kParam_HIP: vst_strncpy (text, "Hipass", kVstMaxParamStrLen); break;
		case kParam_PAN: vst_strncpy (text, "Pan", kVstMaxParamStrLen); break;
		case kParam_FAD: vst_strncpy (text, "Fader", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ConsoleHBuss::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParam_HIG: float2string (HIG, text, kVstMaxParamStrLen); break;
        case kParam_MID: float2string (MID, text, kVstMaxParamStrLen); break;
        case kParam_LOW: float2string (LOW, text, kVstMaxParamStrLen); break;
        case kParam_THR: float2string (THR, text, kVstMaxParamStrLen); break;
        case kParam_LOP: float2string (LOP, text, kVstMaxParamStrLen); break;
        case kParam_HIP: float2string (HIP, text, kVstMaxParamStrLen); break;
        case kParam_PAN: float2string (PAN, text, kVstMaxParamStrLen); break;
        case kParam_FAD: float2string (FAD, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ConsoleHBuss::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParam_HIG: vst_strncpy (text, "eq", kVstMaxParamStrLen); break;
        case kParam_MID: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_LOW: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_THR: vst_strncpy (text, "dyn", kVstMaxParamStrLen); break;
        case kParam_LOP: vst_strncpy (text, "fltr", kVstMaxParamStrLen); break;
        case kParam_HIP: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_PAN: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_FAD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ConsoleHBuss::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ConsoleHBuss::getEffectName(char* name) {
    vst_strncpy(name, "ConsoleHBuss", kVstMaxProductStrLen); return true;
}

VstPlugCategory ConsoleHBuss::getPlugCategory() {return kPlugCategEffect;}

bool ConsoleHBuss::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ConsoleHBuss", kVstMaxProductStrLen); return true;
}

bool ConsoleHBuss::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
