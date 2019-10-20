# Don't allow our script to continue if any errors are observed
$ErrorActionPreference = "Stop"

# We will assume that we'll use Java that is bundled with Android Studio.
$env:JAVA_HOME="C:\Program Files\Android\Android Studio\jre\jre"

# We will assume that the Android SDK is in the default location that Android Studio installs it to.
$env:ANDROID_HOME="C:\Users\$env:UserName\AppData\Local\Android\Sdk"
Write-Host "Using Android SDK at: $env:ANDROID_HOME"

# We will be using a specific version of the Android NDK.
$NDK_VERSION="20.0.5594570"
$env:ANDROID_NDK="$env:ANDROID_HOME\ndk\$NDK_VERSION"
Write-Host "Using Android NDK at: $env:ANDROID_NDK"

Push-Location -Path "$env:ANDROID_HOME\tools\bin"
    Write-Host "Reviewing Android SDK licenses ..."
    
    # Create a file that can answer 'y' to each of the Android licenses automatically.
    Set-Content -Path 'yes.txt' -Value "y`r`ny`r`ny`r`ny`r`ny`r`ny`r`ny`r`ny`r`ny`r`ny`r`n"
    cmd.exe /c 'sdkmanager.bat --licenses < yes.txt'
    Remove-Item -Path 'yes.txt'

    Write-Host "Installing required Android SDK components ..."
    cmd.exe /c "sdkmanager.bat platform-tools build-tools;28.0.3 tools platforms;android-28 cmake;3.10.2.4988404 ndk;$NDK_VERSION"
Pop-Location

# Check that we have a 'third-party' folder
Push-Location -Path "..\..\"
if (!(Test-Path "third-party")) {
    New-Item -ItemType Directory -Path "third-party"
}
Pop-Location

# Check that we have the SDL third party source folder.
if (!(Test-Path "..\..\third-party\SDL")) {
    Write-Host "Downloading SDL source into third party folder SDL ..."
    $WebClient = New-Object System.Net.WebClient
    $WebClient.DownloadFile("https://www.libsdl.org/release/SDL2-2.0.9.zip", "..\..\third-party\SDL2-2.0.9.zip")

    Push-Location -Path "..\..\third-party"
        Write-Host "Unzipping SDL source into third-party\SDL ..."
        cmd.exe /c 'tar -xf SDL2-2.0.9.zip'
        Move-Item -Path SDL2-2.0.9 -Destination SDL
        Remove-Item -Path SDL2-2.0.9.zip
    Pop-Location
}

# If required, create the SDL symlink into the Android library project so it can include it in its build.
Push-Location "sdl\jni"
if (!(Test-Path "SDL")) {
    Write-Host "Linking third-party\SDL to sdl\jni\SDL."
    cmd.exe /c 'mklink /d SDL ..\..\..\..\third-party\SDL'
}
Pop-Location

# Copy the Java classes from the SDL library source into the Android library project.
Push-Location "sdl\src\main"
if (!(Test-Path "java\org\libsdl")) {
    Write-Host "Copying base SDL Java source to Android library project ..."
    Copy-Item -Path ..\..\..\..\..\third-party\SDL\android-project\app\src\main\java -Recurse -Destination java
}
Pop-Location

# Check that we have the GLM third party library
if (!(Test-Path "..\..\third-party\glm")) {
    Write-Host "Downloading GLM into third party folder ..."
    $WebClient = New-Object System.Net.WebClient
    $WebClient.DownloadFile("https://github.com/g-truc/glm/releases/download/0.9.9.3/glm-0.9.9.3.zip", "..\..\third-party\glm-0.9.9.3.zip")

    Push-Location -Path "..\..\third-party"
        Write-Host "Unzipping GLM into third party folder ..."
        cmd.exe /c 'tar -xf glm-0.9.9.3.zip'
        Remove-Item -Path glm-0.9.9.3.zip
    Pop-Location
}

# Check that we have the Tiny OBJ Loader third party library
if (!(Test-Path "..\..\third-party\tiny-obj-loader")) {
    Write-Host "Downloading Tiny OBJ Loader into third party folder ..."
    $WebClient = New-Object System.Net.WebClient
    $WebClient.DownloadFile("https://github.com/syoyo/tinyobjloader/archive/v1.4.1.zip", "..\..\third-party\tiny-obj-loader-v1.4.1.zip")

    Push-Location -Path "..\..\third-party"
        Write-Host "Unzipping Tiny OBJ Loader into third party folder ..."
        cmd.exe /c 'tar -xf tiny-obj-loader-v1.4.1.zip'
        Move-Item -Path tinyobjloader-1.4.1 -Destination tiny-obj-loader
        Remove-Item -Path tiny-obj-loader-v1.4.1.zip
    Pop-Location
}

# If the main Android application doesn't yet have an assets folder, create one.
Push-Location "app\src\main"
if (!(Test-Path "assets")) {
    Write-Host "Creating Android root 'assets' folder ..."
    New-Item -Path assets -ItemType Directory
}
Pop-Location

# Create a symlink to the main assets folder so they are included in the Android application.
Push-Location "app\src\main\assets"
if (!(Test-Path "assets")) {
    Write-Host "Linking assets"
    cmd.exe /c 'mklink /d assets ..\..\..\..\..\main\assets'
}
Pop-Location

Write-Host "All done - import the project in this folder into Android Studio to run it!"
