/* ========================================
 *  CStrip2 - CStrip2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __CStrip2_H
#include "CStrip2.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new CStrip2(audioMaster);}

CStrip2::CStrip2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5; //Treble -12 to 12
	B = 0.5; //Mid -12 to 12
	C = 0.5; //Bass -12 to 12
	D = 0.5; //TrebFrq 6.0 log 1 to 16 defaulting to 6K
	E = 0.5; //BassFrq 100.0 log 30 to 1600 defaulting to 100 hz
	F = 1.0; //Low Cap
	G = 0.0; //High Cap
	H = 0.0; //Compres 0-1
	I = 0.0; //CompSpd 0-1
	J = 0.33; //Output
		
	iirHighpassAL = 0.0;
	iirHighpassBL = 0.0;
	iirHighpassCL = 0.0;
	iirHighpassDL = 0.0;
	iirHighpassEL = 0.0;
	iirHighpassFL = 0.0;
	iirLowpassAL = 0.0;
	iirLowpassBL = 0.0;
	iirLowpassCL = 0.0;
	iirLowpassDL = 0.0;
	iirLowpassEL = 0.0;
	iirLowpassFL = 0.0;
	
	iirHighpassAR = 0.0;
	iirHighpassBR = 0.0;
	iirHighpassCR = 0.0;
	iirHighpassDR = 0.0;
	iirHighpassER = 0.0;
	iirHighpassFR = 0.0;
	iirLowpassAR = 0.0;
	iirLowpassBR = 0.0;
	iirLowpassCR = 0.0;
	iirLowpassDR = 0.0;
	iirLowpassER = 0.0;
	iirLowpassFR = 0.0;
	count = 0;
	
	lastSampleL = 0.0;
	last2SampleL = 0.0;
	lastSampleR = 0.0;
	last2SampleR = 0.0;	
	
	iirHighSampleLA = 0.0;
	iirHighSampleLB = 0.0;
	iirHighSampleLC = 0.0;
	iirHighSampleLD = 0.0;
	iirHighSampleLE = 0.0;
	iirLowSampleLA = 0.0;
	iirLowSampleLB = 0.0;
	iirLowSampleLC = 0.0;
	iirLowSampleLD = 0.0;
	iirLowSampleLE = 0.0;
	iirHighSampleL = 0.0;
	iirLowSampleL = 0.0;
	
	iirHighSampleRA = 0.0;
	iirHighSampleRB = 0.0;
	iirHighSampleRC = 0.0;
	iirHighSampleRD = 0.0;
	iirHighSampleRE = 0.0;
	iirLowSampleRA = 0.0;
	iirLowSampleRB = 0.0;
	iirLowSampleRC = 0.0;
	iirLowSampleRD = 0.0;
	iirLowSampleRE = 0.0;
	iirHighSampleR = 0.0;
	iirLowSampleR = 0.0;
	
	tripletLA = 0.0;
	tripletLB = 0.0;
	tripletLC = 0.0;
	tripletFactorL = 0.0;
	tripletRA = 0.0;
	tripletRB = 0.0;
	tripletRC = 0.0;
	tripletFactorR = 0.0;
	flip = false;
	flipthree = 0;
	//end EQ
		
	//begin ButterComp
	controlAposL = 1.0;
	controlAnegL = 1.0;
	controlBposL = 1.0;
	controlBnegL = 1.0;
	targetposL = 1.0;
	targetnegL = 1.0;	
	avgLA = avgLB = 0.0;
	nvgLA = nvgLB = 0.0;
	
	controlAposR = 1.0;
	controlAnegR = 1.0;
	controlBposR = 1.0;
	controlBnegR = 1.0;
	targetposR = 1.0;
	targetnegR = 1.0;	
	avgRA = avgRB = 0.0;
	nvgRA = nvgRB = 0.0;
	//end ButterComp	
	
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

CStrip2::~CStrip2() {}
VstInt32 CStrip2::getVendorVersion () {return 1000;}
void CStrip2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void CStrip2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 CStrip2::getChunk (void** data, bool isPreset)
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
	chunkData[8] = I;
	chunkData[9] = J;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 CStrip2::setChunk (void* data, VstInt32 byteSize, bool isPreset)
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
	I = pinParameter(chunkData[8]);
	J = pinParameter(chunkData[9]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void CStrip2::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        case kParamG: G = value; break;
        case kParamH: H = value; break;
        case kParamI: I = value; break;
        case kParamJ: J = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float CStrip2::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        case kParamF: return F; break;
        case kParamG: return G; break;
        case kParamH: return H; break;
        case kParamI: return I; break;
        case kParamJ: return J; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void CStrip2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Treble", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Mid", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Bass", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "TrebFrq", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "BassFrq", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "LowCap", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "HiCap", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "Compres", kVstMaxParamStrLen); break;
		case kParamI: vst_strncpy (text, "CompSpd", kVstMaxParamStrLen); break;
		case kParamJ: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void CStrip2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((A*24.0)-12.0, text, kVstMaxParamStrLen); break; //Treble -12 to 12
        case kParamB: float2string ((B*24.0)-12.0, text, kVstMaxParamStrLen); break; //Mid -12 to 12
        case kParamC: float2string ((C*24.0)-12.0, text, kVstMaxParamStrLen); break; //Bass -12 to 12
        case kParamD: float2string ((D*D*15.0)+1.0, text, kVstMaxParamStrLen); break; //Lowpass 16.0K log 1 to 16 defaulting to 16K
        case kParamE: float2string ((E*E*1570.0)+30.0, text, kVstMaxParamStrLen); break; //TrebFrq 6.0 log 1 to 16 defaulting to 6K
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break; //BassFrq 100.0 log 30 to 1600 defaulting to 100 hz
        case kParamG: float2string (G, text, kVstMaxParamStrLen); break; //Hipass 30.0 log 30 to 1600 defaulting to 30
        case kParamH: float2string (H, text, kVstMaxParamStrLen); break; //Gate 0-1
        case kParamI: float2string (I, text, kVstMaxParamStrLen); break; //Compres 0-1
        case kParamJ: float2string (J, text, kVstMaxParamStrLen); break; //CompSpd 0-1
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void CStrip2::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "Khz", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "hz", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamH: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamI: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamJ: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 CStrip2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool CStrip2::getEffectName(char* name) {
    vst_strncpy(name, "CStrip2", kVstMaxProductStrLen); return true;
}

VstPlugCategory CStrip2::getPlugCategory() {return kPlugCategEffect;}

bool CStrip2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows CStrip2", kVstMaxProductStrLen); return true;
}

bool CStrip2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
