TEMPLATE = subdirs

include (./runObj/runObj.pri)

SUBDIRS += \
    autoRun \   
    ifStep \
    endStep \
    printfStep \
    condition \
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


