rem Use this batch file to build box2d for Visual Studio
rmdir /s /q build32
mkdir build32
cd build32
cmake -A Win32 ..
cmake --build build32 ..
start box2d.sln
