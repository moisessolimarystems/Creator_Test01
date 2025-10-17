import re, string, sys, os


os.rename("_SolimarLicenseManager.h", "tmpFile")
origFile = open('tmpFile','r')
newFile = open('_SolimarLicenseManager.h','w')
for line in origFile.readlines():
    if re.compile('#include "shobjidl.h"').search(line, 0):
        newFile.write('//#include "shobjidl.h" //Comment out this file, causes problem with the SDK Visual Studio 6.0 uses - JWL.\n')
    else:
        newFile.write(line)

origFile.close()
newFile.close()
os.remove("tmpFile")
