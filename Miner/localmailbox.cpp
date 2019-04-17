#include "localmailbox.h"


bool LocalMailbox::readRX(){
    try{
        uint8_t received[29];
        serPort1.read(received,29);
        printf("READING IN: ");
        std::cout << received << std::endl;;
       //printf("%x %x %x %x\n", received[0], received[1], received[2], received[3]);

        uint8_t tempPos[4];     // array to hold x, y, z pos

        // Put pos x into tempPos
        tempPos[0] = received[5];
        tempPos[1] = received[6];
        tempPos[2] = received[7];
        tempPos[3] = received[8];
        //Convert bytes To Float
        receivedArduinoX = bytesToFloat(tempPos);
        // Put pos y into tempPos
        tempPos[0] = received[9];
        tempPos[1] = received[10];
        tempPos[2] = received[11];
        tempPos[3] = received[12];
        //Convert bytes To Float
        receivedArduinoY = bytesToFloat(tempPos);
        // Put pos z into tempPos
        tempPos[0] = received[13];
        tempPos[1] = received[14];
        tempPos[2] = received[15];
        tempPos[3] = received[16];
        //Convert bytes To Float
        receivedArduinoZ = bytesToFloat(tempPos);

        return true;
    }
    catch(...){
        return false;
    }
}

bool LocalMailbox::sendTX(){
    try{

        uint8_t send[29];
        uint8_t tempPos[4];

        //Sequence Number
        uint8_t tempSequence[4];
        computeSequence(tempSequence);
        send[0] = tempSequence[0];
        send[1] = tempSequence[1];
        send[2] = tempSequence[2];
        send[3] = tempSequence[3];

        //stStatus
        send[4] = 0xF;

        //Convert pos x float to byte
        floatToBytes(arduinoX, tempPos);
        // set pos x data
        send[5] = tempPos[0];
        send[6] = tempPos[1];
        send[7] = tempPos[2];
        send[8] = tempPos[3];
        //Convert pos y float to byte
        floatToBytes(arduinoY, tempPos);
        // set pos x data
        send[9] = tempPos[0];
        send[10] = tempPos[1];
        send[11] = tempPos[2];
        send[12] = tempPos[3];
        //Convert pos z float to byte
        floatToBytes(arduinoZ, tempPos);
        // set pos x data
        send[13] = tempPos[0];
        send[14] = tempPos[1];
        send[15] = tempPos[2];
        send[16] = tempPos[3];

        //Velocity Leave For nOW 11:51 4-16
        send[17] = 0xF;
        send[18] = 0xF;
        send[19] = 0xF;
        send[20] = 0xF;
        send[21] = 0xF;
        send[22] = 0xF;
        send[23] = 0xF;
        send[24] = 0xF;
        send[25] = 0xF;
        send[26] = 0xF;
        send[27] = 0xF;
        send[28] = 0xF;

        //Actual Send
        serPort1.write(send,29);
        return true;
    }
    catch(...){
        return false;
    }
}

void LocalMailbox::computeSequence(uint8_t sequenceArray[4]){
    sequenceNum++;
    int tempInt = this->sequenceNum;        // temp int to store current private sequence number

    // Shift all bits into correct section of byte array
    sequenceArray[0] = (tempInt >> 24) & 0xFF;
    sequenceArray[1] = (tempInt >> 16) & 0xFF;
    sequenceArray[2] = (tempInt >> 8) & 0xFF;
    sequenceArray[3] = tempInt & 0xFF;

    //printf("%x %x %x %x\n", sequenceArray[0], sequenceArray[1], sequenceArray[2], sequenceArray[3]);
}

float bytesToFloat(uint8_t bytes[4]) {
    float tempFloat = 0;

    // type cast byte array into float
    tempFloat = *(float*)(bytes);
    //printf("tempFloat = %f\r\n", tempFloat);
    return tempFloat;
}

void floatToBytes(float passedFloat, uint8_t bytes[4]) {
    *(float*)(bytes) = passedFloat;
    printf("bytes = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", bytes[0], bytes[1], bytes[2], bytes[3]);
    bytes[0] = bytes[0];
    bytes[1] = bytes[1];
    bytes[2] = bytes[2];
    bytes[3] = bytes[3];
}
