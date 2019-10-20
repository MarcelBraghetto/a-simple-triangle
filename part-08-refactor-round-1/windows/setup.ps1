# Don't allow our script to continue if any errors are observed
$ErrorActionPreference = "Stop"

# Check that we have a 'third-party' folder
Push-Location -Path "..\..\"
if (!(Test-Path "third-party")) {
    New-Item -ItemType Directory -Path "third-party"
}
Pop-Location

# Check that we have the SDL2 third party Windows dev library
if (!(Test-Path "..\..\third-party\sdl-windows")) {
    Write-Host "Downloading SDL Windows Dev library into third party folder sdl-windows ..."
    $WebClient = New-Object System.Net.WebClient
    $WebClient.DownloadFile("https://www.libsdl.org/release/SDL2-devel-2.0.9-VC.zip", "..\..\third-party\SDL2-devel-2.0.9-VC.zip")

    Push-Location -Path "..\..\third-party"
        Write-Host "Unzipping SDL Windows Dev library into third-party\sdl-windows ..."
        cmd.exe /c 'tar -xf SDL2-devel-2.0.9-VC.zip'
        Move-Item -Path SDL2-2.0.9 -Destination sdl-windows
        Remove-Item -Path SDL2-devel-2.0.9-VC.zip
    Pop-Location
}

# Check that we have the GLEW third party library
if (!(Test-Path "..\..\third-party\glew")) {
    Write-Host "Downloading GLEW into third party folder glew ..."
    $WebClient = New-Object System.Net.WebClient
    $WebClient.DownloadFile("https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0-win32.zip", "..\..\third-party\glew-2.1.0-win32.zip")

    Push-Location -Path "..\..\third-party"
        Write-Host "Unzipping GLEW library into third-party\glew ..."
        cmd.exe /c 'tar -xf glew-2.1.0-win32.zip'
        Move-Item -Path glew-2.1.0 -Destination glew
        Remove-Item -Path glew-2.1.0-win32.zip
    Pop-Location
}
