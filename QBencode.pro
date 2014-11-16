TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = \
    src \
    tests

#SUBDIRS += app
#app.depends = src
tests.depends = src

OTHER_FILES += \
    style.astylerc
