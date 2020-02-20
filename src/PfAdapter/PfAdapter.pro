TEMPLATE = subdirs

SUBDIRS += \
    PfAdapterManager \
    #AnalogInAdapter \
    BroadcastAdapter \
    MulticastAdapter \
    UnicastAdapter \
    #SerialPortAdapter \
    #AnalogOutAdapter \
    #DigitalIoAdapter \
    #DigitalOutAdapter \
    #RelayCtrlAdapter   \
    ZmqSendAdapter  \
    ZmqRcvAdapter  \

include (./adapter/adapter.pri)
