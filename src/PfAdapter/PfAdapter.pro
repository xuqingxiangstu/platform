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
    tcpAdapter \
    tcpServerAdapter \
    m1553Adapter

include (./adapter/adapter.pri)
