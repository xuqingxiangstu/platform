TEMPLATE = subdirs

SUBDIRS += \
    PfAdapterManager \
    AnalogInAdapter \
    BroadcastAdapter \
    MulticastAdapter \
    UnicastAdapter \
    SerialPortAdapter

include (./adapter/adapter.pri)
