rmdir /s /q .vs
rmdir /s /q Binaries
rmdir /s /q DerivedDataCache
rmdir /s /q Intermediate
rmdir /s /q Saved
del /s .vsconfig
del /s *.sln
del /s *.slnx

REM ---- Currently no Plugins
REM cd Plugins\
REM call !cleanup.bat
REM cd /d "%~dp0"
