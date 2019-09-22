#ifndef STEWART_CONTROL
#define STEWART_CONTROL

#include "lynxstructure.h"


//using namespace LynxLib;
enum commands
{
    on = (1<<0),
    gyroCompensation = (1<<1),
};

struct StewartControl
{
    StewartControl(LynxManager & lynx, char _structId, const LynxString & description) :
        _lynxId(lynx.addStructure(_structId,description, 7)),
        command(lynx,_lynxId,"Command"),
        setpointX(lynx,_lynxId,"Setpoint X [cm]"),
        setpointY(lynx,_lynxId,"Setpoint Y [cm]"),
        setpointZ(lynx,_lynxId,"Setpoint Z [cm]"),
        setpointRoll(lynx,_lynxId,"Setpoint Roll [degree]"),
        setpointPitch(lynx,_lynxId,"Setpoint Pitch [degree]"),
        setpointYaw(lynx,_lynxId,"Setpoint Yaw [degree]")
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
        sta(lynx,_lynxId,"Status"),
        feedbackX(lynx,_lynxId,"Feedback X [cm]"),
        feedbackY(lynx,_lynxId,"Feedback Y [cm]"),
        feedbackZ(lynx,_lynxId,"Feedback Z [cm]"),
        feedbackRoll(lynx,_lynxId,"Feedback Roll [cm]"),
        feedbackPitch(lynx,_lynxId,"Feedback Pitch [cm]"),
        feedbackYaw(lynx,_lynxId,"Feedback Yaw [cm]"),
        imuRoll(lynx,_lynxId,"IMU Roll [degree]"),
        imuPitch(lynx,_lynxId,"IMU Pitch [degree]"),
        imuYaw(lynx,_lynxId,"IMU Yaw [degree]")
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

