#define pi 3.1415
#include <Servo.h> 
#include "lynxstructure.h"
#include "lynxuartarduino.h"
#include "stewartcontrol.h"

using namespace LynxLib;

LynxManager lynx(0x02);

LynxUartArduino uart(lynx);

LynxInfo updateInfo;

StewartControl stewartControl(lynx, 0x22);
StewartFeedback stewartFeedback(lynx, 0x23);
/************************************************************************
  BASE Definition
************************************************************************/
#define base_radius          9.4          // [cm]  Radius of the base
#define base_delta_angle    27.57/180*pi  // [rad] Angle between arm pair
#define platform_radius      6.5          // [cm]  Radius of the platform
#define platform_delta_angle 4/180*pi     // [rad] Angle between the connecting joint pair [rad] 
#define gamma              -5/180*pi     // [rad] Tilted angle of the platform. Negative means tilted inwards. 
#define aServo               4.2          // [cm]  Length of servo arm
#define sServo              14.0          // [cm]  Length of carbon arm
 
/************************************************************************
  Servo Definition
************************************************************************/
#define PWM_cof 100/90          // Constant from rad to PWM. +100% = 90deg --> 100/90
#define Offset_1 9  // + = down
#define Offset_2 0  // - = down
#define Offset_3 -2 // + = down 
#define Offset_4 6  // - = down
#define Offset_5 10 // + = down
#define Offset_6 4  // - = down

/************************************************************************
  Variables & Functions
************************************************************************/
float alpha1 = 0;
float alpha2 = 0;
float alpha3 = 0;
float alpha4 = 0;
float alpha5 = 0;
float alpha6 = 0;
float iAX,iAY,iAZ,iGX,iGY,iGZ;
float i;
float fnc = 0;
byte tall;
int x_en = 0;   float A_x = 2;
int y_en = 0;   float A_y = 2;
int z_en = 0;   float A_z = 2;
int r_en = 0;   float A_r = 0.5;
int p_en = 0;   float A_p = 0.5;
int yw_en = 0;  float A_yw = 0.7;

int mode, act_en;
int timerOut;
bool valid,newMode;

void SendResponse();
void GetData();
void AlphaCalc(float x, float y, float z, float phi, float theta, float psi);

Servo Servo1,Servo2,Servo3,Servo4,Servo5,Servo6;

int timePin = 46;

void setup()
{
  pinMode(timePin, OUTPUT);
  digitalWrite(timePin,LOW);
  uart.open(0, 19200);
  //Serial.begin(9600);//Starts serial via USB
  //Serial1.begin(115200);//Starts serial via RX1/TX1 for BLE communication
  Servo1.attach(2);  Servo2.attach(3);  Servo3.attach(5);  Servo4.attach(6);  Servo5.attach(7);  Servo6.attach(8);
  mode=1;
  act_en = 1;
  valid=false;
//  Serial.println("Starting: ");
//  Serial.println("AC:Zero ");
//  Serial.println("AD:Stop ");
//  Serial.println("AE:Park ");
//  Serial.println("AF:Pose ");
//  Serial.println("AG:Demo ");
  newMode=true;
}

  
void loop()

{
  
  
  valid=true;
  updateInfo = uart.update();

  if (updateInfo.state == eNewDataReceived)
  {
   // AlphaCalc(stewartControl.setpointX,stewartControl.setpointY,stewartControl.setpointZ+12.0, stewartControl.setpointRoll, stewartControl.setpointPitch, stewartControl.setpointYaw);
 
   // Servo1.write(Offset_1 + 90 + alpha1*90/pi);
    //Servo2.write(Offset_2 + 90 + alpha2*90/pi);
    //Servo3.write(Offset_3 + 90 + alpha3*90/pi);
    //Servo4.write(Offset_4 + 90 + alpha4*90/pi);
   // Servo5.write(Offset_5 + 90 + alpha5*90/pi);
   // Servo6.write(Offset_6 + 90 + alpha6*90/pi);

      stewartFeedback.feedbackX = stewartControl.setpointX;

      stewartFeedback.feedbackY = stewartControl.setpointY;
 
      stewartFeedback.feedbackZ = stewartControl.setpointZ;

      stewartFeedback.feedbackRoll = stewartControl.setpointRoll;

      stewartFeedback.feedbackPitch = stewartControl.setpointPitch;

      stewartFeedback.feedbackYaw = stewartControl.setpointYaw;

      stewartFeedback.imuRoll = 1.2;

      stewartFeedback.imuPitch = 2.4;
    

    uart.send(stewartFeedback.lynxId());
    
  }
}


void AlphaCalc(float x, float y, float z, float phi, float theta, float psi)
{
  float c_phi   = cos(phi);
  float c_theta = cos(theta);
  float c_psi   = cos(psi);

  float s_phi   = sin(phi);
  float s_theta = sin(theta);
  float s_psi   = sin(psi);
  
  alpha1 = atan((aServo*(y+base_radius*sin(base_delta_angle)-platform_radius*sin(platform_delta_angle)*(c_phi*c_psi+s_phi*s_psi*s_theta)*1.0+platform_radius*cos(platform_delta_angle)*c_theta*s_psi)*1.0)/(aServo*cos(gamma)*(z*-1.0+platform_radius*cos(platform_delta_angle)*s_theta+platform_radius*c_theta*s_phi*sin(platform_delta_angle))*1.0-aServo*sin(gamma)*(x-base_radius*cos(base_delta_angle)*1.0+platform_radius*sin(platform_delta_angle)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)+platform_radius*cos(platform_delta_angle)*c_psi*c_theta)*1.0))*1.0-asin(1.0/sqrt((aServo*aServo)*pow(y+base_radius*sin(base_delta_angle)-platform_radius*sin(platform_delta_angle)*(c_phi*c_psi+s_phi*s_psi*s_theta)*1.0+platform_radius*cos(platform_delta_angle)*c_theta*s_psi,2.0)+pow(aServo*cos(gamma)*(z*-1.0+platform_radius*cos(platform_delta_angle)*s_theta+platform_radius*c_theta*s_phi*sin(platform_delta_angle))*1.0-aServo*sin(gamma)*(x-base_radius*cos(base_delta_angle)*1.0+platform_radius*sin(platform_delta_angle)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)+platform_radius*cos(platform_delta_angle)*c_psi*c_theta),2.0))*(pow(fabs(x-base_radius*cos(base_delta_angle)*1.0+platform_radius*sin(platform_delta_angle)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)+platform_radius*cos(platform_delta_angle)*c_psi*c_theta),2.0)*5.0E-1+pow(fabs(y+base_radius*sin(base_delta_angle)-platform_radius*sin(platform_delta_angle)*(c_phi*c_psi+s_phi*s_psi*s_theta)*1.0+platform_radius*cos(platform_delta_angle)*c_theta*s_psi),2.0)*5.0E-1+pow(fabs(z*-1.0+platform_radius*cos(platform_delta_angle)*s_theta+platform_radius*c_theta*s_phi*sin(platform_delta_angle)),2.0)*5.0E-1+(aServo*aServo)*5.0E-1-(sServo*sServo)*5.0E-1))*1.0;
  alpha2 = atan((aServo*(y-base_radius*sin(base_delta_angle)*1.0+platform_radius*sin(platform_delta_angle)*(c_phi*c_psi+s_phi*s_psi*s_theta)+platform_radius*cos(platform_delta_angle)*c_theta*s_psi))/(aServo*cos(gamma)*(z-platform_radius*cos(platform_delta_angle)*s_theta*1.0+platform_radius*c_theta*s_phi*sin(platform_delta_angle))+aServo*sin(gamma)*(x-base_radius*cos(base_delta_angle)*1.0-platform_radius*sin(platform_delta_angle)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)*1.0+platform_radius*cos(platform_delta_angle)*c_psi*c_theta)))*-1.0+asin(1.0/sqrt((aServo*aServo)*pow(y-base_radius*sin(base_delta_angle)*1.0+platform_radius*sin(platform_delta_angle)*(c_phi*c_psi+s_phi*s_psi*s_theta)+platform_radius*cos(platform_delta_angle)*c_theta*s_psi,2.0)+pow(aServo*cos(gamma)*(z-platform_radius*cos(platform_delta_angle)*s_theta*1.0+platform_radius*c_theta*s_phi*sin(platform_delta_angle))+aServo*sin(gamma)*(x-base_radius*cos(base_delta_angle)*1.0-platform_radius*sin(platform_delta_angle)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)*1.0+platform_radius*cos(platform_delta_angle)*c_psi*c_theta),2.0))*(pow(fabs(z-platform_radius*cos(platform_delta_angle)*s_theta*1.0+platform_radius*c_theta*s_phi*sin(platform_delta_angle)),2.0)*5.0E-1+pow(fabs(y-base_radius*sin(base_delta_angle)*1.0+platform_radius*sin(platform_delta_angle)*(c_phi*c_psi+s_phi*s_psi*s_theta)+platform_radius*cos(platform_delta_angle)*c_theta*s_psi),2.0)*5.0E-1+(aServo*aServo)*5.0E-1-(sServo*sServo)*5.0E-1+pow(fabs(x-base_radius*cos(base_delta_angle)*1.0-platform_radius*sin(platform_delta_angle)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)*1.0+platform_radius*cos(platform_delta_angle)*c_psi*c_theta),2.0)*5.0E-1));
  alpha3 = asin(1.0/sqrt(pow(aServo*sin(gamma)*(x-base_radius*cos(base_delta_angle-2.09439510240918)*1.0+platform_radius*sin(platform_delta_angle-2.09439510240918)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)+platform_radius*c_psi*c_theta*cos(platform_delta_angle-2.09439510240918))*5.0E-1-aServo*sin(gamma)*(y+base_radius*sin(base_delta_angle-2.09439510240918)-platform_radius*sin(platform_delta_angle-2.09439510240918)*(c_phi*c_psi+s_phi*s_psi*s_theta)*1.0+platform_radius*c_theta*s_psi*cos(platform_delta_angle-2.09439510240918))*8.660254037822597E-1+aServo*cos(gamma)*(z*-1.0+platform_radius*s_theta*cos(platform_delta_angle-2.09439510240918)+platform_radius*sin(platform_delta_angle-2.09439510240918)*c_theta*s_phi),2.0)+pow(aServo*(x-base_radius*cos(base_delta_angle-2.09439510240918)*1.0+platform_radius*sin(platform_delta_angle-2.09439510240918)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)+platform_radius*c_psi*c_theta*cos(platform_delta_angle-2.09439510240918))*8.660254037822597E-1+aServo*(y+base_radius*sin(base_delta_angle-2.09439510240918)-platform_radius*sin(platform_delta_angle-2.09439510240918)*(c_phi*c_psi+s_phi*s_psi*s_theta)*1.0+platform_radius*c_theta*s_psi*cos(platform_delta_angle-2.09439510240918))*5.0E-1,2.0))*(pow(fabs(x-base_radius*cos(base_delta_angle-2.09439510240918)*1.0+platform_radius*sin(platform_delta_angle-2.09439510240918)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)+platform_radius*c_psi*c_theta*cos(platform_delta_angle-2.09439510240918)),2.0)*5.0E-1+(aServo*aServo)*5.0E-1+pow(fabs(y+base_radius*sin(base_delta_angle-2.09439510240918)-platform_radius*sin(platform_delta_angle-2.09439510240918)*(c_phi*c_psi+s_phi*s_psi*s_theta)*1.0+platform_radius*c_theta*s_psi*cos(platform_delta_angle-2.09439510240918)),2.0)*5.0E-1-(sServo*sServo)*5.0E-1+pow(fabs(z*-1.0+platform_radius*s_theta*cos(platform_delta_angle-2.09439510240918)+platform_radius*sin(platform_delta_angle-2.09439510240918)*c_theta*s_phi),2.0)*5.0E-1))*-1.0-atan((aServo*(x-base_radius*cos(base_delta_angle-2.09439510240918)*1.0+platform_radius*sin(platform_delta_angle-2.09439510240918)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)+platform_radius*c_psi*c_theta*cos(platform_delta_angle-2.09439510240918))*8.660254037822597E-1+aServo*(y+base_radius*sin(base_delta_angle-2.09439510240918)-platform_radius*sin(platform_delta_angle-2.09439510240918)*(c_phi*c_psi+s_phi*s_psi*s_theta)*1.0+platform_radius*c_theta*s_psi*cos(platform_delta_angle-2.09439510240918))*5.0E-1)/(aServo*sin(gamma)*(x-base_radius*cos(base_delta_angle-2.09439510240918)*1.0+platform_radius*sin(platform_delta_angle-2.09439510240918)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)+platform_radius*c_psi*c_theta*cos(platform_delta_angle-2.09439510240918))*5.0E-1-aServo*sin(gamma)*(y+base_radius*sin(base_delta_angle-2.09439510240918)-platform_radius*sin(platform_delta_angle-2.09439510240918)*(c_phi*c_psi+s_phi*s_psi*s_theta)*1.0+platform_radius*c_theta*s_psi*cos(platform_delta_angle-2.09439510240918))*8.660254037822597E-1+aServo*cos(gamma)*(z*-1.0+platform_radius*s_theta*cos(platform_delta_angle-2.09439510240918)+platform_radius*sin(platform_delta_angle-2.09439510240918)*c_theta*s_phi)))*1.0;
  alpha4 = atan((aServo*(y-base_radius*sin(base_delta_angle+2.09439510240918)*1.0+platform_radius*sin(platform_delta_angle+2.09439510240918)*(c_phi*c_psi+s_phi*s_psi*s_theta)+platform_radius*cos(platform_delta_angle+2.09439510240918)*c_theta*s_psi)*5.0E-1+aServo*(x-base_radius*cos(base_delta_angle+2.09439510240918)*1.0-platform_radius*sin(platform_delta_angle+2.09439510240918)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)*1.0+platform_radius*cos(platform_delta_angle+2.09439510240918)*c_psi*c_theta)*8.660254037822597E-1)/(aServo*sin(gamma)*(y-base_radius*sin(base_delta_angle+2.09439510240918)*1.0+platform_radius*sin(platform_delta_angle+2.09439510240918)*(c_phi*c_psi+s_phi*s_psi*s_theta)+platform_radius*cos(platform_delta_angle+2.09439510240918)*c_theta*s_psi)*8.660254037822597E-1+aServo*cos(gamma)*(z-platform_radius*cos(platform_delta_angle+2.09439510240918)*s_theta*1.0+platform_radius*sin(platform_delta_angle+2.09439510240918)*c_theta*s_phi)-aServo*sin(gamma)*(x-base_radius*cos(base_delta_angle+2.09439510240918)*1.0-platform_radius*sin(platform_delta_angle+2.09439510240918)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)*1.0+platform_radius*cos(platform_delta_angle+2.09439510240918)*c_psi*c_theta)*5.0E-1))+asin(1.0/sqrt(pow(aServo*sin(gamma)*(y-base_radius*sin(base_delta_angle+2.09439510240918)*1.0+platform_radius*sin(platform_delta_angle+2.09439510240918)*(c_phi*c_psi+s_phi*s_psi*s_theta)+platform_radius*cos(platform_delta_angle+2.09439510240918)*c_theta*s_psi)*8.660254037822597E-1+aServo*cos(gamma)*(z-platform_radius*cos(platform_delta_angle+2.09439510240918)*s_theta*1.0+platform_radius*sin(platform_delta_angle+2.09439510240918)*c_theta*s_phi)-aServo*sin(gamma)*(x-base_radius*cos(base_delta_angle+2.09439510240918)*1.0-platform_radius*sin(platform_delta_angle+2.09439510240918)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)*1.0+platform_radius*cos(platform_delta_angle+2.09439510240918)*c_psi*c_theta)*5.0E-1,2.0)+pow(aServo*(y-base_radius*sin(base_delta_angle+2.09439510240918)*1.0+platform_radius*sin(platform_delta_angle+2.09439510240918)*(c_phi*c_psi+s_phi*s_psi*s_theta)+platform_radius*cos(platform_delta_angle+2.09439510240918)*c_theta*s_psi)*5.0E-1+aServo*(x-base_radius*cos(base_delta_angle+2.09439510240918)*1.0-platform_radius*sin(platform_delta_angle+2.09439510240918)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)*1.0+platform_radius*cos(platform_delta_angle+2.09439510240918)*c_psi*c_theta)*8.660254037822597E-1,2.0))*(pow(fabs(y-base_radius*sin(base_delta_angle+2.09439510240918)*1.0+platform_radius*sin(platform_delta_angle+2.09439510240918)*(c_phi*c_psi+s_phi*s_psi*s_theta)+platform_radius*cos(platform_delta_angle+2.09439510240918)*c_theta*s_psi),2.0)*5.0E-1+pow(fabs(z-platform_radius*cos(platform_delta_angle+2.09439510240918)*s_theta*1.0+platform_radius*sin(platform_delta_angle+2.09439510240918)*c_theta*s_phi),2.0)*5.0E-1+pow(fabs(x-base_radius*cos(base_delta_angle+2.09439510240918)*1.0-platform_radius*sin(platform_delta_angle+2.09439510240918)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)*1.0+platform_radius*cos(platform_delta_angle+2.09439510240918)*c_psi*c_theta),2.0)*5.0E-1+(aServo*aServo)*5.0E-1-(sServo*sServo)*5.0E-1));
  alpha5 = asin(1.0/sqrt(pow(aServo*sin(gamma)*(x-base_radius*cos(base_delta_angle-4.189000000013039)*1.0+platform_radius*sin(platform_delta_angle-4.189000000013039)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)+platform_radius*c_psi*c_theta*cos(platform_delta_angle-4.189000000013039))*5.0E-1+aServo*sin(gamma)*(y+base_radius*sin(base_delta_angle-4.189000000013039)-platform_radius*sin(platform_delta_angle-4.189000000013039)*(c_phi*c_psi+s_phi*s_psi*s_theta)*1.0+platform_radius*c_theta*s_psi*cos(platform_delta_angle-4.189000000013039))*8.659999999945285E-1+aServo*cos(gamma)*(z*-1.0+platform_radius*s_theta*cos(platform_delta_angle-4.189000000013039)+platform_radius*sin(platform_delta_angle-4.189000000013039)*c_theta*s_phi),2.0)+pow(aServo*(x-base_radius*cos(base_delta_angle-4.189000000013039)*1.0+platform_radius*sin(platform_delta_angle-4.189000000013039)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)+platform_radius*c_psi*c_theta*cos(platform_delta_angle-4.189000000013039))*8.659999999945285E-1-aServo*(y+base_radius*sin(base_delta_angle-4.189000000013039)-platform_radius*sin(platform_delta_angle-4.189000000013039)*(c_phi*c_psi+s_phi*s_psi*s_theta)*1.0+platform_radius*c_theta*s_psi*cos(platform_delta_angle-4.189000000013039))*5.0E-1,2.0))*(pow(fabs(x-base_radius*cos(base_delta_angle-4.189000000013039)*1.0+platform_radius*sin(platform_delta_angle-4.189000000013039)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)+platform_radius*c_psi*c_theta*cos(platform_delta_angle-4.189000000013039)),2.0)*5.0E-1+(aServo*aServo)*5.0E-1+pow(fabs(y+base_radius*sin(base_delta_angle-4.189000000013039)-platform_radius*sin(platform_delta_angle-4.189000000013039)*(c_phi*c_psi+s_phi*s_psi*s_theta)*1.0+platform_radius*c_theta*s_psi*cos(platform_delta_angle-4.189000000013039)),2.0)*5.0E-1-(sServo*sServo)*5.0E-1+pow(fabs(z*-1.0+platform_radius*s_theta*cos(platform_delta_angle-4.189000000013039)+platform_radius*sin(platform_delta_angle-4.189000000013039)*c_theta*s_phi),2.0)*5.0E-1))*-1.0+atan((aServo*(x-base_radius*cos(base_delta_angle-4.189000000013039)*1.0+platform_radius*sin(platform_delta_angle-4.189000000013039)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)+platform_radius*c_psi*c_theta*cos(platform_delta_angle-4.189000000013039))*8.659999999945285E-1-aServo*(y+base_radius*sin(base_delta_angle-4.189000000013039)-platform_radius*sin(platform_delta_angle-4.189000000013039)*(c_phi*c_psi+s_phi*s_psi*s_theta)*1.0+platform_radius*c_theta*s_psi*cos(platform_delta_angle-4.189000000013039))*5.0E-1)/(aServo*sin(gamma)*(x-base_radius*cos(base_delta_angle-4.189000000013039)*1.0+platform_radius*sin(platform_delta_angle-4.189000000013039)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)+platform_radius*c_psi*c_theta*cos(platform_delta_angle-4.189000000013039))*5.0E-1+aServo*sin(gamma)*(y+base_radius*sin(base_delta_angle-4.189000000013039)-platform_radius*sin(platform_delta_angle-4.189000000013039)*(c_phi*c_psi+s_phi*s_psi*s_theta)*1.0+platform_radius*c_theta*s_psi*cos(platform_delta_angle-4.189000000013039))*8.659999999945285E-1+aServo*cos(gamma)*(z*-1.0+platform_radius*s_theta*cos(platform_delta_angle-4.189000000013039)+platform_radius*sin(platform_delta_angle-4.189000000013039)*c_theta*s_phi)))*1.0;
  alpha6 = atan((aServo*(y-base_radius*sin(base_delta_angle+4.188790204818361)*1.0+platform_radius*sin(platform_delta_angle+4.188790204818361)*(c_phi*c_psi+s_phi*s_psi*s_theta)+platform_radius*cos(platform_delta_angle+4.188790204818361)*c_theta*s_psi)*5.0E-1-aServo*(x-base_radius*cos(base_delta_angle+4.188790204818361)*1.0-platform_radius*sin(platform_delta_angle+4.188790204818361)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)*1.0+platform_radius*cos(platform_delta_angle+4.188790204818361)*c_psi*c_theta)*8.660254037822597E-1)/(aServo*sin(gamma)*(y-base_radius*sin(base_delta_angle+4.188790204818361)*1.0+platform_radius*sin(platform_delta_angle+4.188790204818361)*(c_phi*c_psi+s_phi*s_psi*s_theta)+platform_radius*cos(platform_delta_angle+4.188790204818361)*c_theta*s_psi)*8.660254037822597E-1-aServo*cos(gamma)*(z-platform_radius*cos(platform_delta_angle+4.188790204818361)*s_theta*1.0+platform_radius*sin(platform_delta_angle+4.188790204818361)*c_theta*s_phi)*1.0+aServo*sin(gamma)*(x-base_radius*cos(base_delta_angle+4.188790204818361)*1.0-platform_radius*sin(platform_delta_angle+4.188790204818361)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)*1.0+platform_radius*cos(platform_delta_angle+4.188790204818361)*c_psi*c_theta)*5.0E-1))*-1.0+asin(1.0/sqrt(pow(aServo*sin(gamma)*(y-base_radius*sin(base_delta_angle+4.188790204818361)*1.0+platform_radius*sin(platform_delta_angle+4.188790204818361)*(c_phi*c_psi+s_phi*s_psi*s_theta)+platform_radius*cos(platform_delta_angle+4.188790204818361)*c_theta*s_psi)*8.660254037822597E-1-aServo*cos(gamma)*(z-platform_radius*cos(platform_delta_angle+4.188790204818361)*s_theta*1.0+platform_radius*sin(platform_delta_angle+4.188790204818361)*c_theta*s_phi)*1.0+aServo*sin(gamma)*(x-base_radius*cos(base_delta_angle+4.188790204818361)*1.0-platform_radius*sin(platform_delta_angle+4.188790204818361)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)*1.0+platform_radius*cos(platform_delta_angle+4.188790204818361)*c_psi*c_theta)*5.0E-1,2.0)+pow(aServo*(y-base_radius*sin(base_delta_angle+4.188790204818361)*1.0+platform_radius*sin(platform_delta_angle+4.188790204818361)*(c_phi*c_psi+s_phi*s_psi*s_theta)+platform_radius*cos(platform_delta_angle+4.188790204818361)*c_theta*s_psi)*5.0E-1-aServo*(x-base_radius*cos(base_delta_angle+4.188790204818361)*1.0-platform_radius*sin(platform_delta_angle+4.188790204818361)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)*1.0+platform_radius*cos(platform_delta_angle+4.188790204818361)*c_psi*c_theta)*8.660254037822597E-1,2.0))*(pow(fabs(y-base_radius*sin(base_delta_angle+4.188790204818361)*1.0+platform_radius*sin(platform_delta_angle+4.188790204818361)*(c_phi*c_psi+s_phi*s_psi*s_theta)+platform_radius*cos(platform_delta_angle+4.188790204818361)*c_theta*s_psi),2.0)*5.0E-1+pow(fabs(z-platform_radius*cos(platform_delta_angle+4.188790204818361)*s_theta*1.0+platform_radius*sin(platform_delta_angle+4.188790204818361)*c_theta*s_phi),2.0)*5.0E-1+pow(fabs(x-base_radius*cos(base_delta_angle+4.188790204818361)*1.0-platform_radius*sin(platform_delta_angle+4.188790204818361)*(c_phi*s_psi-c_psi*s_phi*s_theta*1.0)*1.0+platform_radius*cos(platform_delta_angle+4.188790204818361)*c_psi*c_theta),2.0)*5.0E-1+(aServo*aServo)*5.0E-1-(sServo*sServo)*5.0E-1));
}

  
 
