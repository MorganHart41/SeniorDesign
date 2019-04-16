#include "wrapper.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    Wrapper w;
    std::cout << w.automatedOrManual();



    //NEED TO POPULATE WITH CSV
    /*char userChoice = w.automatedOrManual();
    if(userChoice == 'a'){
        w.automatedLogic();
    }
    else{
        w.manualLogic();
    }*/
}
