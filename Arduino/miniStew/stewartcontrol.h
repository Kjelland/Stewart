#ifndef STEWART_CONTROL
#define STEWART_CONTROL

#include "lynxstructure.h"


using namespace LynxLib;
enum commands
{
    on = (1<<0),
    gyroCompensation = (1<<1),
};

struct StewartControl
{
    StewartControl(LynxManager & lynx, char _structId, const LynxString & description) :
        _lynxId(lynx.addStructure(_structId,description, 7)),
        command(lynx,_lynxId),
        setpointX(lynx,_lynxId),
        setpointY(lynx,_lynxId),
        setpointZ(lynx,_lynxId),
        setpointRoll(lynx,_lynxId),
        setpointPitch(lynx,_lynxId),
        setpointYaw(lynx,_lynxId)
    {}

    LYNX_STRUCTURE_MACRO
    LynxVar_u8 command;
    LynxVar_float setpointX;
    LynxVar_float setpointY;
    LynxVar_float setpointZ;
    LynxVar_float setpointRoll;
    LynxVar_float setpointPitch;
    LynxVar_float setpointYaw;

};
struct StewartFeedback
{
    StewartFeedback(LynxManager & lynx, char _structId,const LynxString & description) :
        _lynxId(lynx.addStructure(_structId,description,10)),
        sta(lynx,_lynxId),
        feedbackX(lynx,_lynxId,"Feedback X [cm]"),
        feedbackY(lynx,_lynxId),
        feedbackZ(lynx,_lynxId),
        feedbackRoll(lynx,_lynxId),
        feedbackPitch(lynx,_lynxId),
        feedbackYaw(lynx,_lynxId),
        imuRoll(lynx,_lynxId),
        imuPitch(lynx,_lynxId),
        imuYaw(lynx,_lynxId)
    {}

    LYNX_STRUCTURE_MACRO
LynxVar_u8 sta;
    LynxVar_float feedbackX;
    LynxVar_float feedbackY;
    LynxVar_float feedbackZ;
    LynxVar_float feedbackRoll;
    LynxVar_float feedbackPitch;
    LynxVar_float feedbackYaw;
    LynxVar_float imuRoll;
    LynxVar_float imuPitch;
    LynxVar_float imuYaw;

};

#endif // !STEWART_CONTROL

