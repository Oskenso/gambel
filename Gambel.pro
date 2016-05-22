TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    libgambel \
    Gambel

libgambel.subdir = libgambel
Gambel.subdir = Gambel
Gambel.depends = libgambel
