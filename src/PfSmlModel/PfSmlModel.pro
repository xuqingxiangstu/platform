TEMPLATE = subdirs



SUBDIRS += \
    modelManager \
    randaModel \
    waveformModel\
    temperatureModel


include (./algorithm/algorithm.pri)
