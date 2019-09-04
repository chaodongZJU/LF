#include "UserDev.h"
#include "csl_cache.h"
#include "fastmath67x.h"
#define PI 3.14159265358979
float f;
float GoalAmp;
float GoalPhase;
int iFrameCount=0;
float w0 = 0.005;
float w1 = 0;
float StepSize;
float StepIdent;
float alpha = 0.0001;
float beta = 0.0001;  
float xa,xb,xaHat,xbHat;  
float Ref; 
float error = 0;  
float uInput,feedback;
float FIR_order = 300;
float sn[300];
float xaVec[300];
float xbVec[300];
int Start;
int StepTime;

void CaluInput()
{
    int i,j,k;
    float temp;	
    xa = sinsp(2*PI*f*iFrameCount/fixed_samplefreq); 
    xb = cossp(2*PI*f*iFrameCount/fixed_samplefreq);
    Ref = GoalAmp*sinsp(2*PI*f*iFrameCount/fixed_samplefreq+GoalPhase);
    error = Ref - feedback;
    iFrameCount++;    
    if (iFrameCount<Start)
    {				
        uInput = w0*xa+w1*xb;
        xaHat = 0;
        xbHat = 0;
        for(i=0;i<FIR_order;i++)
        {
            xaHat+=xaVec[i]*sn[i];
            xbHat+=xbVec[i]*sn[i];
        }
        for(i=FIR_order-1;i>0;i--)
        {
            xaVec[i]=xaVec[i-1];
            xbVec[i]=xbVec[i-1];
        }
        xaVec[0] = xa;
        xbVec[0] = xb;
        }
        else 
        {				
            uInput = w0*xa+w1*xb;	
            xaHat = 0;
            xbHat = 0;
            for(i=0;i<FIR_order;i++)
            {
                xaHat+=xaVec[i]*sn[i];
                xbHat+=xbVec[i]*sn[i];
            }
            for(i=FIR_order-1;i>0;i--)
            {
                xaVec[i]=xaVec[i-1];
                xbVec[i]=xbVec[i-1];
            }
            xaVec[0] = xa;
            xbVec[0] = xb;
            temp = xaHat*xaHat+xbHat*xbHat; 
            w0=w0+StepSize*xaHat*error/temp;
            w1=w1+StepSize*xbHat*error/temp;
    }
}	
