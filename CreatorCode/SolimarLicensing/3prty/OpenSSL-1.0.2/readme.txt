========================================================================
    BuildOpenssl.py : Build Openssl 1.0.x Using VS 2015
========================================================================

1) Pre-requirements

   The following applications are installed on the PC:
    
     a) VisualStudio 2015
     b) 7-zip
     c) Perl
     d) Python

   And directories where the relevant executable programs riside are in the PATH.

2) Usage

   Put the script and the '.tar.gz' file downloaded from https://www.openssl.org/source/ in the same folder and run the following in the folder:

        python buildopenssl.py openssl.tar.gz 32/64 debug/release static/dll test

   Default is to build debug/64/static, i.e.

        python buildopenssl.py openssl.tar.gz 

   will build static x64 debug build.
 
   If want to build dll 64-bit release build from 'openssl2.1.02r.tar.gz', use:
  
        python buildopenssl.py openssl2.1.02r.tar.gz 64 release dll
		
   If 'test' is specified, the script will run the simple tests supplied in the release before copy the results.

3) Where to find the build results:

   The script copies the relevant build results to the following folder structure:

   a) for static libs:
      \x64\release
          \debug
      \x86\release
          \debug

   b) for dynamic libs:
      dll\x64\release
             \debug
         \x86\release
             \debug
   
   The build results include 
       
      header files
      static/dynamic libs
      lib.pdb (for silencing compilation warnings)
      engine files (may not be relevant)
      openssl.cnf 


/////////////////////////////////////////////////////////////////////////////
