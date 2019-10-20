# Don't allow our script to continue if any errors are observed
$ErrorActionPreference = "Stop"

Push-Location -Path "out"
if (!(Test-Path "SDL2.dll")) {
    Copy-Item -Path "..\..\..\third-party\sdl-windows\lib\x64\SDL2.dll"
}

# If there is no 'assets' folder in our output we need to create a new symlink for it
if (!(Test-Path "assets")) {
	cmd.exe /c 'mklink /d assets ..\..\main\assets'
}
Pop-Location
