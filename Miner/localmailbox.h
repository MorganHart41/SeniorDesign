#ifndef LOCALMAILBOX_H
#define LOCALMAILBOX_H

#include "serial.h"

class LocalMailbox
{
public:
    LocalMailbox(){};

    /*********************************************************
    *                         SETTERS                        *
    **********************************************************/
    void setArduinoX(float newArduinoX){arduinoX = newArduinoX;}
    void setArduinoY(float newArduinoY){arduinoY = newArduinoY;}
    void setArduinoZ(float newArduinoZ){arduinoZ = newArduinoZ;}
    void setVelocityX(float newVelocityX){velocityX = newVelocityX;}
    void setVelocityY(float newVelocityY){velocityY = newVelocityY;}
    void setVelocityZ(float newVelocityZ){velocityZ = newVelocityZ;}

    /*********************************************************
    *                         GETTERS                        *
    **********************************************************/
    float getArduinoX(){return receivedArduinoX;}
    float getArduinoY(){return receivedArduinoY;}
    float getArduinoZ(){return receivedArduinoZ;}
    float getVelocityX(){return velocityX;}
    float getVelocityY(){return velocityY;}
    float getVelocityZ(){return velocityZ;}


    /*********************************************************
    *                 LOCAL MAILBOX SPECIFIC                 *
    **********************************************************/
    bool sendTX();
    bool readRX();
private:
    float arduinoX, arduinoY, arduinoZ; //For Send
    float receivedArduinoX, receivedArduinoY, receivedArduinoZ; //For Received
    float velocityX, velocityY, velocityZ;
};

#endif // LOCALMAILBOX_H
