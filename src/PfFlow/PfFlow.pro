TEMPLATE = subdirs

include (./runObj/runObj.pri)

SUBDIRS += \
    condition \
    RunFlow \
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
    setSwitchOutStep \
    formulaCalcStep \
    judgeMultiStep \
    judgeStep \
    variateStep


