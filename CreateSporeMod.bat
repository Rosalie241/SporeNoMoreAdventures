@echo on

set CUR_DIR="%~dp0"
mkdir "%CUR_DIR%\obj"
mkdir "%CUR_DIR%\bin"

copy "C:\ProgramData\SPORE ModAPI Launcher Kit\mLibs\SporeNoMoreAdventures.dll" "%CUR_DIR%\obj\"
copy "%CUR_DIR%\ModInfo.xml" "%CUR_DIR%\obj\"
copy "%CUR_DIR%\Package\MrsWhitesAdventuresBegoneV1.package" "%CUR_DIR%\obj\"

cd "%CUR_DIR%\obj\"

"C:\Program Files\7-Zip\7z" a -tzip %CUR_DIR%\bin\SporeNoMoreAdventures.sporemod *

cd "%CUR_DIR%"

rmdir /s /q "%CUR_DIR%\obj\"