@echo off
REM Compile the Unlit Tint Vertex Shader
fxc /T vs_5_0 /E main /Fo UnlitTintVS.cso UnlitTintVS.hlsl

REM Compile the Unlit Tint Pixel Shader
fxc /T ps_5_0 /E main /Fo UnlitTintPS.cso UnlitTintPS.hlsl

echo Shaders compiled successfully!
pause
