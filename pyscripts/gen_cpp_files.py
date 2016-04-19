#!/usr/bin/python 
# encoding: utf-8

import os
import time
import sys

###############################
#                             #
# a script to auto generate   #
# the c++ header file and cpp #
# files                       #
#                             #
###############################

NAMESPACE = 'Tan'
MAIL = 'feilengcui008@gmail.com'

def genCppFiles(namespace, mail, *names):
    headerTemplate = "// Copyright [%s] <%s>\n#ifndef %s\n#define %s\n\n" \
            "namespace %s {\n\n}  // namespace %s\n\n#endif  // end %s"
    cppTemplate = "// Copyright [%s] <%s>\n\n#include \"%s\"\n\n" \
            "namespace %s {\n\n}  // namespace %s"
    for i in names[0]:
        headerContent = headerTemplate % (str(time.localtime()[0]), mail, i.upper() \
                + '_H_', i.upper() + '_H_', namespace, namespace, i.upper() + '_H_')
        cppContent = cppTemplate % (str(time.localtime()[0]), mail, i + '.h', namespace, namespace)
        with open(i + '.h', 'w') as fp:
            fp.write(headerContent)
        with open(i + '.cpp', 'w') as fp:
            fp.write(cppContent)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print "useage: python gen_cpp_files.py filename1 filename2"
        sys.exit()
    genCppFiles(NAMESPACE, MAIL, sys.argv[1:])

