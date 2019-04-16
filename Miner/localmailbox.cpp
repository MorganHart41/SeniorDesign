#include "localmailbox.h"


bool LocalMailbox::readRX(){
    try{
        //Read In Message
        //Decompose Message
        //Update Variables
        return true;
    }
    catch(...){
        return false;
    }
}

bool LocalMailbox::sendTX(){
    try{
        //Compose Message
        //Send Message
        return true;
    }
    catch(...){
        return false;
    }
}

