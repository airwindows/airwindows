/* ========================================
 *  BassAmp - BassAmp.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BassAmp_H
#include "BassAmp.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new BassAmp(audioMaster);}

BassAmp::BassAmp(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.75;
	B = 0.0;
	C = 0.75;
	D = 0.0;
	
	LataLast6Sample = 0.0;
	LataLast5Sample = 0.0;
	LataLast4Sample = 0.0;
	LataLast3Sample = 0.0;
	LataLast2Sample = 0.0;
	LataLast1Sample = 0.0;
	LataHalfwaySample = 0.0;
	LataHalfDrySample = 0.0;
	LataHalfDiffSample = 0.0;
	LataLastDiffSample = 0.0;
	LataDrySample = 0.0;
	LataDiffSample = 0.0;
	LataPrevDiffSample = 0.0;
	LWasNegative = false;
	LSubOctave = false;
	LiirDriveSampleA = 0.0;
	LiirDriveSampleB = 0.0;
	LiirDriveSampleC = 0.0;
	LiirDriveSampleD = 0.0;
	LiirDriveSampleE = 0.0;
	LiirDriveSampleF = 0.0;
	LiirHeadBumpA = 0.0;
	LiirHeadBumpB = 0.0;
	LiirHeadBumpC = 0.0;
	LiirSubBumpA = 0.0;
	LiirSubBumpB = 0.0;
	LiirSubBumpC = 0.0;
	LiirSampleA = 0.0;
	LiirSampleB = 0.0;
	LiirSampleC = 0.0;
	LiirSampleD = 0.0;
	LiirSampleE = 0.0;
	LiirSampleF = 0.0;
	LiirSampleG = 0.0;
	LiirSampleH = 0.0;
	LiirSampleI = 0.0;
	LiirSampleJ = 0.0;
	LiirSampleK = 0.0;
	LiirSampleL = 0.0;
	LiirSampleM = 0.0;
	LiirSampleN = 0.0;
	LiirSampleO = 0.0;
	LiirSampleP = 0.0;
	LiirSampleQ = 0.0;
	LiirSampleR = 0.0;
	LiirSampleS = 0.0;
	LiirSampleT = 0.0;
	LiirSampleU = 0.0;
	LiirSampleV = 0.0;
	LiirSampleW = 0.0;
	LiirSampleX = 0.0;
	LiirSampleY = 0.0;
	LiirSampleZ = 0.0;	
	
	RataLast6Sample = 0.0;
	RataLast5Sample = 0.0;
	RataLast4Sample = 0.0;
	RataLast3Sample = 0.0;
	RataLast2Sample = 0.0;
	RataLast1Sample = 0.0;
	RataHalfwaySample = 0.0;
	RataHalfDrySample = 0.0;
	RataHalfDiffSample = 0.0;
	RataLastDiffSample = 0.0;
	RataDrySample = 0.0;
	RataDiffSample = 0.0;
	RataPrevDiffSample = 0.0;
	RWasNegative = false;
	RSubOctave = false;
	RiirDriveSampleA = 0.0;
	RiirDriveSampleB = 0.0;
	RiirDriveSampleC = 0.0;
	RiirDriveSampleD = 0.0;
	RiirDriveSampleE = 0.0;
	RiirDriveSampleF = 0.0;
	RiirHeadBumpA = 0.0;
	RiirHeadBumpB = 0.0;
	RiirHeadBumpC = 0.0;
	RiirSubBumpA = 0.0;
	RiirSubBumpB = 0.0;
	RiirSubBumpC = 0.0;
	RiirSampleA = 0.0;
	RiirSampleB = 0.0;
	RiirSampleC = 0.0;
	RiirSampleD = 0.0;
	RiirSampleE = 0.0;
	RiirSampleF = 0.0;
	RiirSampleG = 0.0;
	RiirSampleH = 0.0;
	RiirSampleI = 0.0;
	RiirSampleJ = 0.0;
	RiirSampleK = 0.0;
	RiirSampleL = 0.0;
	RiirSampleM = 0.0;
	RiirSampleN = 0.0;
	RiirSampleO = 0.0;
	RiirSampleP = 0.0;
	RiirSampleQ = 0.0;
	RiirSampleR = 0.0;
	RiirSampleS = 0.0;
	RiirSampleT = 0.0;
	RiirSampleU = 0.0;
	RiirSampleV = 0.0;
	RiirSampleW = 0.0;
	RiirSampleX = 0.0;
	RiirSampleY = 0.0;
	RiirSampleZ = 0.0;	
	
	flip = false;
	bflip = 0;
	ataK1 = -0.646; //first FIR shaping of interpolated sample, brightens
	ataK2 = 0.311; //second FIR shaping of interpolated sample, thickens
	ataK6 = -0.093; //third FIR shaping of interpolated sample, brings air
	ataK7 = 0.057; //fourth FIR shaping of interpolated sample, thickens
	ataK8 = -0.023; //fifth FIR shaping of interpolated sample, brings air
	ataK3 = 0.114; //add raw to interpolated dry, toughens
	ataK4 = 0.886; //remainder of interpolated dry, adds up to 1.0
	ataK5 = 0.122; //subtract this much prev. diff sample, brightens
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

BassAmp::~BassAmp() {}
VstInt32 BassAmp::getVendorVersion () {return 1000;}
void BassAmp::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void BassAmp::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 BassAmp::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	chunkData[3] = D;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 BassAmp::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	D = pinParameter(chunkData[3]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void BassAmp::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float BassAmp::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void BassAmp::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "High", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Dry", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Dub", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Sub", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void BassAmp::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void BassAmp::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 BassAmp::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool BassAmp::getEffectName(char* name) {
    vst_strncpy(name, "BassAmp", kVstMaxProductStrLen); return true;
}

VstPlugCategory BassAmp::getPlugCategory() {return kPlugCategEffect;}

bool BassAmp::getProductString(char* text) {
  	vst_strncpy (text, "airwindows BassAmp", kVstMaxProductStrLen); return true;
}

bool BassAmp::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
