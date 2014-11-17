TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = \
    src \
    tests
OTHER_FILES += \
    style.astylerc

#SUBDIRS += app
#app.depends = src
tests.depends = src

# A custom target to directly run tests
runtests.depends = all
unix:runtests.commands = $${OUT_PWD}/tests/runtests.sh
win32:runtests.commands = $${OUT_PWD}/tests/runtests.bat
QMAKE_EXTRA_TARGETS += runtests
