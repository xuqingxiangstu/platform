TEMPLATE = subdirs

SUBDIRS += \
    PfAdapterManager \
    #AnalogInAdapter \
    #BroadcastAdapter \
    #MulticastAdapter \
    #UnicastAdapter \
    #SerialPortAdapter \
    #AnalogOutAdapter \
    #DigitalIoAdapter \
    #DigitalOutAdapter \
    #RelayCtrlAdapter

include (./adapter/adapter.pri)
