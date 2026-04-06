# BuildOpenssl.py - building openssl-1.0.x using VisualStudio 2015
#
# usage: put the script and the openssl.tar.gz in the same folder and run the following in the folder
#
#     buildopenssl.py openssl.tar.gz 32/64 debug/release lib/dll test
#
#  default is debug/64/lib, i.e.
#     buildopenssl.py openssl.tar.gz 
#  is building debug/64/lib
#
#     buildopenssl.py openssl.tar.gz 32
#  will build debug/32/lib
#
# pre-requirements:
#    VS2015
#    7-zip
#    Perl
#    Python

import os
from subprocess import call, Popen
import sys
import re
import shutil
from os import path


def SetupVSVars(working_dir, arch):
    vs_tools_env_var = "VS" + vs_version + "COMNTOOLS"

    vsfolder = os.getenv(vs_tools_env_var)

    os.chdir(vsfolder)
    os.chdir("..\\..\\vc")
    cwd = os.getcwd()

    vsvars_path = "vcvarsall.bat " + arch

    env_bat_file_path = "setup_build_environment_temp.bat"
    env_txt_file_path = "build_environment_temp.txt"

    os.chdir(working_dir)
    with open(env_bat_file_path, "w") as env_bat_file:
        env_bat_file.write('cd "/d %s"\n' % cwd)
        env_bat_file.write('call %s\n' % vsvars_path)
        env_bat_file.write('cd "/d %s"\n' % working_dir)
        env_bat_file.write("set > %s\n" % env_txt_file_path)

    os.chdir(working_dir)
    os.system(env_bat_file_path)

    with open(env_txt_file_path, "r") as env_txt_file:
        lines = env_txt_file.read().splitlines()

    os.remove(env_bat_file_path)
    os.remove(env_txt_file_path)

    for line in lines:
        pair = line.split("=", 1)
        os.environ[pair[0]] = pair[1]		

# for VS 2015		
vs_version = "140"

compile_flags = " no-asm"

openssl_32_flag = "VC-WIN32"
openssl_64_flag = "VC-WIN64A"
openssl_32_dbg_flag = "debug-VC-WIN32"
openssl_64_dbg_flag = "debug-VC-WIN64A"

src_32_suffix = "_" + "vs" + vs_version + "_32"
src_64_suffix = "_" + "vs" + vs_version + "_64"
src_32_dbg_suffix = "_" + "vs" + vs_version + "_32" + "_dbg"
src_64_dbg_suffix = "_" + "vs" + vs_version + "_64" + "_dbg"

build_32_suffix = "x86\\release"
build_64_suffix = "x64\\release"
build_32_dbg_suffix = "x86\\debug"
build_64_dbg_suffix = "x64\\debug"

filename = "openssl.tar.gz"
if len(sys.argv) < 2:
    print("First argument must be the tar.gz file of OpenSSL source")
else:
    filename = sys.argv[1]

do_test = False
if len(sys.argv) == 6:
    do_test = (sys.argv[5] == "test")

is_dynamic = False
if len(sys.argv) < 5:
    print("Fourth argument missing - should be lib or dll - assume STATIC")
else:
    is_dynamic = (sys.argv[4] == "dll")

is_debug = True
if len(sys.argv) < 4:
    print("Third argument missing - should be debug or release - assume DEBUG")
else:	
    is_debug = (sys.argv[3] == "debug")

arch = "64"
if len(sys.argv) < 3:
    print("Second argument missing - should be 32 or 64 - assume X64")
else:
    arch = sys.argv[2]
	
if arch != "32" and arch != "64":
    print("Second argument must be 32 or 64 - assume 64 ")
    arch = "64"            ## treat it as 64   

dirname  = filename.replace(".tar.gz","")
working_dir = os.getcwd()

os.chdir(working_dir)

#if not bool(re.match("(openssl-){1}(\d)+(.)(\d)+(.)(\d)+(\w)+(.tar.gz)",filename)):
#    print("The file given doesn't seem to be an openssl source file. It must be in the form: openssl-x.y.zw.tar.gz")
#    exit(1)

openssl_tar_file = filename[0:-3]

if os.path.exists(openssl_tar_file):
    os.remove(openssl_tar_file)

call("7z x " + filename) #extract the .gz file

## setup environment for compiler
if arch == "32":
    SetupVSVars(working_dir, "x86")
else:
    SetupVSVars(working_dir, "amd64")

## specify the folder names and flags etc.	
if arch == "32":
    if is_debug:
        dirname_src = dirname + src_32_dbg_suffix
        if is_dynamic:
            dirname_bin = "dll\\" + build_32_dbg_suffix
        else:
            dirname_bin = build_32_dbg_suffix
        openssl_flag = openssl_32_dbg_flag
    else:
        dirname_src = dirname + src_32_suffix
        if is_dynamic:
            dirname_bin = "dll\\" + build_32_suffix
        else:
            dirname_bin = build_32_suffix
        openssl_flag = openssl_32_flag
else:
    if is_debug:
        dirname_src = dirname + src_64_dbg_suffix
        if is_dynamic:
            dirname_bin = "dll\\" + build_64_dbg_suffix
        else:
            dirname_bin = build_64_dbg_suffix
        openssl_flag = openssl_64_dbg_flag
    else:
        dirname_src = dirname + src_64_suffix
        if is_dynamic:
            dirname_bin = "dll\\" + build_64_suffix
        else:
            dirname_bin = build_64_suffix
        openssl_flag = openssl_64_flag

## clear the folders		
if os.path.exists(os.path.join(working_dir,dirname_src)):
    shutil.rmtree(os.path.join(working_dir,dirname_src), True)
if os.path.exists(os.path.join(working_dir,dirname_bin)):
    shutil.rmtree(os.path.join(working_dir,dirname_bin), True)

#extract tar file
call("7z x " + openssl_tar_file)

#rename the folder
os.rename(dirname, dirname_src)

os.chdir(dirname_src)
call("perl Configure " + openssl_flag + " --prefix=" + os.path.join(working_dir,dirname_bin) + compile_flags, shell=True)

if arch == "32":
    call(r"ms\do_ms.bat",shell=True)
else:
    call(r"ms\do_win64a.bat",shell=True)
		
os.chdir(os.path.join(working_dir,dirname_src))

if is_dynamic:
    call(r"nmake -f ms\ntdll.mak",shell=True)
    if do_test:
        call(r"nmake -f ms\ntdll.mak test",shell=True)
    call(r"nmake -f ms\ntdll.mak install",shell=True)
else:
    call(r"nmake -f ms\nt.mak",shell=True)
    if do_test:
        call(r"nmake -f ms\nt.mak test",shell=True)
    call(r"nmake -f ms\nt.mak install",shell=True)

#copy the .pdb file - to silence compiler warnings when the libs are used
if is_dynamic:
    if (is_debug):
        shutil.copy2(os.path.join(working_dir, os.path.join(dirname_src, "tmp32dll.dbg\\lib.pdb")), os.path.join(working_dir, os.path.join(dirname_bin, "lib")))
    else:
        shutil.copy2(os.path.join(working_dir, os.path.join(dirname_src, "tmp32dll\\lib.pdb")), os.path.join(working_dir, os.path.join(dirname_bin, "lib")))
else:
    if (is_debug):
        shutil.copy2(os.path.join(working_dir, os.path.join(dirname_src, "tmp32.dbg\\lib.pdb")), os.path.join(working_dir, os.path.join(dirname_bin, "lib")))
    else:
        shutil.copy2(os.path.join(working_dir, os.path.join(dirname_src, "tmp32\\lib.pdb")), os.path.join(working_dir, os.path.join(dirname_bin, "lib")))

print("compilation complete.")

#Go back to base dir
os.chdir(working_dir)

os.remove(openssl_tar_file)
