# Don't allow our script to continue if any errors are observed
$ErrorActionPreference = "Stop"

Push-Location -Path "out"
if (!(Test-Path "SDL2.dll")) {
    Copy-Item -Path "..\..\..\third-party\sdl-windows\lib\x64\SDL2.dll"
}
Pop-Location
