TEMPLATE = subdirs

include (./runObj/runObj.pri)

SUBDIRS += \
    condition \
    autoRun \   
    ifStep \
    endStep \
    printfStep \    
    breakStep \
    continueStep \
    elseIfStep \    
    elseStep \
    sleepStep \
    whileStep \
    temperatureStep \
    getAnalogInStep \
    getSwitchInStep \
    setAnalogOutStep \
    setSwitchOutStep


