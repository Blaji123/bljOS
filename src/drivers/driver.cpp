#include <drivers/driver.h>
using namespace bljOS::drivers;

Driver::Driver(){}

Driver::~Driver(){}

void Driver::Activate(){}

int Driver::Reset(){
    return 0;
}

void Driver::Deactivate(){}

DriverManager::DriverManager(){
    numDrivers = 0;
}

void DriverManager::AddDriver(Driver* driver){
    drivers[numDrivers] = driver;
    numDrivers++;
}

void DriverManager::ActivateAll(){
        for(int i=0;i<numDrivers;i++){
            drivers[i]->Activate();
        }
}
