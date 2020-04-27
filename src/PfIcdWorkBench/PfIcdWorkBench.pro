TEMPLATE = subdirs

include (./common/common.pri)

SUBDIRS += \
    icdData \
    icdFrameAdapter\
    #variableFrame \
    generalFrame \
    frame93 \
    frameFE \
    frameMiddle \
    frameBE \
    frameXK \
    frameDM \
    frameCZXK \
    frameC3 \
    frame1553B
