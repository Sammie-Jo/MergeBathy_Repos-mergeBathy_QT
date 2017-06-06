<<<<<<< HEAD
ECHO Setting up QGIS DEV ENV
 
set PYTHONPATH=C:\Python27;C:\Python27\Scripts;C:\Python27\libs;
 
set OSGEO4W_ROOT=C:\OSGeo4W
call "%OSGEO4W_ROOT%\bin\o4w_env.bat"

set GISF=qgis-ltr
@set QMAKESPEC=win32-msvc2010
@set PATH=%OSGEO4W_ROOT%\bin;%OSGEO4W_ROOT%\apps\%GISF%\bin;%PATH%
 
@set INCLUDE=%INCLUDE%;%OSGEO4W_ROOT%\include;%OSGEO4W_ROOT%\apps\%GISF%\include
@set LIB=%LIB%;%OSGEO4W_ROOT%\lib;%OSGEO4W_ROOT%\apps\%GISF%\lib

path %OSGEO4W_ROOT%\bin;%SYSTEMROOT%\System32;%SYSTEMROOT%;%SYSTEMROOT%\System32\wbem;C:\Progra~1\Git\bin;C:\Qt\Tools\QtCreator\bin;%PATH%

set VS100COMNTOOLS=C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Tools\
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
  
start "Qt Creator" /B F:\Qt\Tools\QtCreator\bin\qtcreator.exe %*

=======
ECHO Setting up QGIS DEV ENV
 
set PYTHONPATH=C:\Python27;C:\Python27\Scripts;C:\Python27\libs;
 
set OSGEO4W_ROOT=C:\OSGeo4W
call "%OSGEO4W_ROOT%\bin\o4w_env.bat"

set GISF=qgis-ltr
@set QMAKESPEC=win32-msvc2010
@set PATH=%OSGEO4W_ROOT%\bin;%OSGEO4W_ROOT%\apps\%GISF%\bin;%PATH%
 
@set INCLUDE=%INCLUDE%;%OSGEO4W_ROOT%\include;%OSGEO4W_ROOT%\apps\%GISF%\include
@set LIB=%LIB%;%OSGEO4W_ROOT%\lib;%OSGEO4W_ROOT%\apps\%GISF%\lib

path %OSGEO4W_ROOT%\bin;%SYSTEMROOT%\System32;%SYSTEMROOT%;%SYSTEMROOT%\System32\wbem;C:\Progra~1\Git\bin;C:\Qt\Tools\QtCreator\bin;%PATH%

set VS100COMNTOOLS=C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Tools\
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
  
start "Qt Creator" /B F:\Qt\Tools\QtCreator\bin\qtcreator.exe %*

>>>>>>> origin/master
