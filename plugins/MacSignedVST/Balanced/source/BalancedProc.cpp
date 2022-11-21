/* ========================================
 *  Balanced - Balanced.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Balanced_H
#include "Balanced.h"
#endif

void Balanced::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
    
    int bitshiftBalanced = (A * 8);
    double gain = 1.0;
    switch (bitshiftBalanced)
    {
        case 0: gain = 0.5; break;
        case 1: gain = 1.0; break;
        case 2: gain = 2.0; break;
        case 3: gain = 4.0; break;
        case 4: gain = 8.0; break;
        case 5: gain = 16.0; break;
        case 6: gain = 32.0; break;
        case 7: gain = 64.0; break;
        case 8: gain = 128.0; break;
    }
    //we are directly punching in the gain values rather than calculating them

    while (--sampleFrames >= 0)
    {
        double inputSampleL = *in1;
        double inputSampleR = *in2;
        if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
        if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

        double side = inputSampleL - inputSampleR;
        //tip is left, to add negative ring (right) to combine 'em is the same as subtracting them
        //end result is, mono output is made up of half of each balanced input combined. Note that we don't just
        //flip the ring input, because we need to combine them to cancel out interference.
        inputSampleL = side*gain;
        inputSampleR = side*gain;
        //assign mono as result of balancing of channels
        
        //begin 32 bit stereo floating point dither
        int expon; frexpf((float)inputSampleL, &expon);
        fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
        inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
        frexpf((float)inputSampleR, &expon);
        fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
        inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
        //end 32 bit stereo floating point dither
        
        *out1 = inputSampleL;
        *out2 = inputSampleR;

        in1++;
        in2++;
        out1++;
        out2++;
    }
}

void Balanced::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames)
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
    
    int bitshiftBalanced = (A * 8);
    double gain = 1.0;
    switch (bitshiftBalanced)
    {
        case 0: gain = 0.5; break;
        case 1: gain = 1.0; break;
        case 2: gain = 2.0; break;
        case 3: gain = 4.0; break;
        case 4: gain = 8.0; break;
        case 5: gain = 16.0; break;
        case 6: gain = 32.0; break;
        case 7: gain = 64.0; break;
        case 8: gain = 128.0; break;
    }
    //we are directly punching in the gain values rather than calculating them
        
    while (--sampleFrames >= 0)
    {
        double inputSampleL = *in1;
        double inputSampleR = *in2;
        if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
        if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
        
        double side = inputSampleL - inputSampleR;
        //tip is left, to add negative ring (right) to combine 'em is the same as subtracting them
        //end result is, mono output is made up of half of each balanced input combined. Note that we don't just
        //flip the ring input, because we need to combine them to cancel out interference.
        inputSampleL = side*gain;
        inputSampleR = side*gain;
        //assign mono as result of balancing of channels
                
        //begin 64 bit stereo floating point dither
        //int expon; frexp((double)inputSampleL, &expon);
        fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
        //inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
        //frexp((double)inputSampleR, &expon);
        fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
        //inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
        //end 64 bit stereo floating point dither
        
        *out1 = inputSampleL;
        *out2 = inputSampleR;

        in1++;
        in2++;
        out1++;
        out2++;
    }
}
