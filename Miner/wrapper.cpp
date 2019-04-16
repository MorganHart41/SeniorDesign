#include "wrapper.h"

Wrapper::Wrapper(){}

/*********************************************************
*                 WRAPPER SPECIFIC METHODS               *
**********************************************************/

//Method Used To Print Data To The Screen
void Wrapper::printStats(){
    std::cout << "Current Step: " << step << std::endl;
    std::cout << "Desired X: " << desiredPositionX << " Desired Y: " << desiredPositionY << " Desired Z: " << desiredPositionZ << std::endl;
    std::cout << "Arduino X: " << arduinoPositionX << " Arduino Y: " << arduinoPositionY << " Arduino Z: " << arduinoPositionZ << std::endl;
    std::cout << "Trax Heading: " << traxHeading << " Trax Pitch: " << traxPitch << " Trax Roll: " << traxRoll << std::endl;
    std::cout << "*******************************************************" << std::endl;
}

//Method Used To Check If We Are At The Final Step
bool Wrapper::checkFinalStep(){
    if(tm.getSampleCount() == 18.0){
        printMessage("At 18th Point");
        return true;
    }
    return false;
}

//Method Used To Update All Trax Variables And Arduino Variables
void Wrapper::updateAllExternalVariables(){
    //Update All Arduino Variables To Most Recent
    //getArduinoX();
    //getArduinoY();
    //getArduinoZ();

    //Update All Trax Variables To Most Recent
    getTraxHeading();
    getTraxPitch();
    getTraxRoll();
}

//Method Used For Updating The Desired Sub Position To The Next One
void Wrapper::updateDesiredSubPosition(){
    SubPosition * nextPosition;
    nextPosition = allSubPositions.back();
    desiredPositionX = nextPosition->getPositionX();
    desiredPositionY = nextPosition->getPositionY();
    desiredPositionZ = nextPosition->getPositionZ();
    allSubPositions.pop_back();
}

//Method Used To Compare If The Arduino Coordinated Are Within 1 Degree Of The Desired Sub Position
bool Wrapper::comparePointValues(){
    if(desiredPositionX+1 > arduinoPositionX && desiredPositionX-1 < arduinoPositionX && desiredPositionY+1 > arduinoPositionY && desiredPositionY-1 < arduinoPositionY && desiredPositionZ+1 > arduinoPositionZ && desiredPositionZ-1 < arduinoPositionZ){
        return true;
    }
    return false;
}

//Method For Calibration
bool Wrapper::calibrationInit(){
    if(initTraxSettings()){
        delayBy(2);
        if(startTraxCalibration()){
            delayBy(2);
            return true;
        }
    }
    return false;
}

//Method For Delay In Seconds
void Wrapper::delayBy(int amount){
    printMessage("DELAY");
    for(int i = 0; i < amount; i++){ //Sleep For 30 Seconds
        usleep(1000000); //One Second Delay
    }
}

//Method For Debug Print Statements
void Wrapper::printMessage(std::string s){
    std::cout << s << std::endl;
}

//Method To Delay Until Enter Button Is Hit
void Wrapper::enterButtonDelay() {
    printMessage("USER: Press Enter To Take Point");
    while(std::cin.get() == '\r'){}
}

//Method For Selecting Automated Or Manual Mode
char Wrapper::automatedOrManual(){
    printMessage("Please Enter 'a' For Automated Calibration or 'm' For Manual Calibration");
    while(true){
        if(std::cin.get() == 'a'){
            printMessage("Automated Calibration Selected");
            return 'a';
        }
        else if (std::cin.get() == 'm'){
            printMessage("Manual Calibration Selected");
            return 'm';
        }
        else{
            printMessage("Please Type Either 'm' or 'a'");
        }
    }
}

//Method For Automated Calibration Logic
void Wrapper::automatedLogic(){
    tm.initCal();
    while(1){
        printMessage("********************************************************");
        //Formulate TX
        //Send TX
        //Delay
        //Read RX
        //Decompose RX
        if(comparePointValues()){
            if(takeTraxPoint()){
                if(checkFinalStep()){
                    printMessage("********************************************************");
                    if(getTraxCalScore()){
                        if(saveTraxCal()){
                            if(setDefaultTraxSettings()){
                                printMessage("Successfully Calibrated!!!");
                            }
                        }
                    }
                    else{
                        if(setDefaultTraxSettings()){
                            printMessage("Unsuccesfully Calibrated...");
                        }
                    }
                }
            }
        }
    }
}

//Method For Manual Logic Calibration
void Wrapper::manualLogic(){
    calibrationInit();
    while(1){
        printMessage("********************************************************");
        enterButtonDelay();
        if(takeTraxPoint()){
            if(checkFinalStep()){
                printMessage("********************************************************");
                if(getTraxCalScore()){ //Change To Grab Variable?
                    if(saveTraxCal()){ //Commented Out To Prevent Bad Saves
                        if(setDefaultTraxSettings()){
                            std::cout << "Successfully Calibrated!!!" << std::endl;
                            break;
                        }
                    }
                }
                else{
                    if(setDefaultTraxSettings()){
                        std::cout << "Unsuccesfully Calibrated..." << std::endl;
                        break;
                    }
                }
            }
        }
    }
}

bool Wrapper::CSVInput(){
    std::fstream fout;
    float tempStep, tempX, tempY, tempZ;
    fout.open("inputtedData.csv");
    for(int i = 0; i < 18; i++){
        cin >>tempStep >> tempX >> tempY >> tempZ;
        SubPosition * step = new SubPosition(tempStep,tempX,tempY,tempZ);
        allSubPositions.push_front(step);
    }
}
/*********************************************************
*                   TRAX USE METHODS                     *
**********************************************************/

//Method Used To Initialize Trax Settings And Handle Errors Thrown
bool Wrapper::initTraxSettings(){
    printMessage("********************************************************");
    printMessage("initCal: ");
    int results = tm.initCal();
    if(results == -1){
        std::cout << "Error: Init Of Calibration Failed" << std::endl;
        return false;
    }
    return true;
}

//Method Used For Starting The Trax Calibration Procedure
bool Wrapper::startTraxCalibration(){
    printMessage("********************************************************");
    printMessage("startCal: ");
    try{ //Try A startCal
        tm.startCal();
        return true; //Positive Return
    }
    catch(...){
        std::cout << "Error: Start Calibration Failed" << std::endl;
        return false; //Negative Return
    }
}

//Method For Taking A Point For The Trax
bool Wrapper::takeTraxPoint(){
    int results = tm.takePoint();
    for(int i = 0; i < 10; i++){ //Loop For Attempting 10 Times
        if(results == 0){ //Check If Pass
            std::cout << "Taken Point: " << tm.getSampleCount() << std::endl;
            return true; //Positive Return
        }
    }
    std::cout << "Error: Take Point Failed (Attempted 10 Times)" << std::endl;
    return false; //Negative Return
}

//Method For Getting The Trax Calibration Score (Good Or Bad Eh)
bool Wrapper::getTraxCalScore(){
    if(tm.getFinalCalScore() == false){ //Check For Fail
        std::cout << "Error: Calibration Failed" << std::endl;
        return false;
    }
    return true;
}

//Will Save The Settings To The Trax If A Good Calibration Occurs
bool Wrapper::saveTraxCal(){
    int results = tm.save();
    if(results == -1){
        std::cout << "Error: Save Failed" << std::endl;
        return false;
    }
    return true;
}

//Will Abort The Calibration Procedure For The Trax
bool Wrapper::abortTraxCal(){
    try{ //Will Attempt A Abort
        tm.abortCal();
        return true;
    }
    catch(...){
        std::cout << "Error: Abort Calibration Failed" << std::endl;
        return false;
    }
}

//Will Reset The Default Settings For The Trax
bool Wrapper::setDefaultTraxSettings(){
    int results = tm.setDefaultSettings();
    if(results == -1){
        std::cout << "Error: Set Default Settings Failed" << std::endl;
        return false;
    }
    return true;
}

//Method For Setting The Heading Variable Accordingly
void Wrapper::setHeadingFromTrax(){
    try{
        float tempHeading = tm.getHeading();
        setTraxHeading(tempHeading);
    }
    catch(...){
        std::cout << "Error: Set Trax Heading Failed" << std::endl;
    }
}

//Method For Setting The Pitch Variable Accordingly
void Wrapper::setPitchFromTrax(){
    try{
        float tempPitch = tm.getPitch();
        setTraxPitch(tempPitch);
    }
    catch(...){
        std::cout << "Error: Set Trax Pitch Failed" << std::endl;
    }
}

//Method For Setting The Roll Variable Accordingly
void Wrapper::setRollFromTrax(){
    try{
        float tempRoll = tm.getRoll();
        setTraxRoll(tempRoll);
    }
    catch(...){
        std::cout << "Error: Set Trax Roll Failed" << std::endl;
    }
}

//Method For Getting All The Trax Variables
void Wrapper::setAllTraxHPR(){
    std::cout << "Entered getAllTraxHPR" << std::endl;
    if(tm.getPosition() != -1){
        std::cout << "Got Position" << std::endl;
        getTraxHeading();
        getTraxPitch();
        getTraxRoll();
    }
    else{
        std::cout << "Couldnt Update Position For Trax" << std::endl;
    }
}

void Wrapper::printAllTraxData(){
    std::cout << "TRAX DATA: " << std::endl;
    std::cout << "Heading: " << traxHeading << std::endl;
    std::cout << "Pitch: " << traxPitch << std::endl;
    std::cout << "Roll: " << traxRoll << std::endl;
}


/*********************************************************
*                   LOCAL MAILBOX METHODS                *
**********************************************************/

//Method For Getting Arduino X Variable From LocalMailbox
void Wrapper::getArduinoX(){
    arduinoPositionX = lm.getArduinoX();
}

//Method For Getting Arduino Y Variable From LocalMailbox
void Wrapper::getArduinoY(){
    arduinoPositionY = lm.getArduinoY();
}

//Method For Getting Arduino Z Variable From LocalMailbox
void Wrapper::getArduinoZ(){
    arduinoPositionZ = lm.getArduinoZ();
}

//Method For Getting All Arduino XYZ
void Wrapper::getAllArduinoXYZ(){
    getArduinoX();
    getArduinoY();
    getArduinoZ();
}

//Method For Setting The ArduinoX Desired Variable
void Wrapper::setArduinoX(float newArduinoX){
    lm.setArduinoX(newArduinoX);
}

//Method For Setting The ArduinoY Desired Variable
void Wrapper::setArduinoY(float newArduinoY){
    lm.setArduinoY(newArduinoY);
}

//Method For Setting The ArduinoZ Desired Variable
void Wrapper::setArduinoZ(float newArduinoZ){
    lm.setArduinoZ(newArduinoZ);
}
