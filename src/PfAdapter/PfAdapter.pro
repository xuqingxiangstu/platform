TEMPLATE = subdirs

SUBDIRS += \
    PfAdapterManager \
    #AnalogInAdapter \
    BroadcastAdapter \
    MulticastAdapter \
    UnicastAdapter \
    SerialPortAdapter \
    #AnalogOutAdapter \
    #DigitalIoAdapter \
    #DigitalOutAdapter \
    #RelayCtrlAdapter   \
    ZmqSendAdapter  \
    ZmqRcvAdapter  \
    virtualUnicastAdapter \

include (./adapter/adapter.pri)
