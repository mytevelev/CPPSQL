call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" 
cmake -G "Visual Studio 17 2022" -A "x64" -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake -Bbuild . 
cmake --build build 
PAUSE