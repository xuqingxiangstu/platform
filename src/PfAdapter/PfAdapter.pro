TEMPLATE = subdirs

SUBDIRS += \
    PfAdapterManager \   
    BroadcastAdapter \
    MulticastAdapter \
    UnicastAdapter \
    ZmqSendAdapter  \
    ZmqRcvAdapter  \
    tcpAdapter \
    tcpServerAdapter \
    m1553Adapter

include (./adapter/adapter.pri)
