# Don't allow our script to continue if any errors are observed
$ErrorActionPreference = "Stop"

# Check that we have a 'vulkan' shader asset folder
Push-Location -Path "..\assets\shaders"
if (!(Test-Path "vulkan")) {
    New-Item -ItemType Directory -Path "vulkan"
}
Pop-Location

# Grab all the files in the current directory ending with 'vert' or 'frag'
# and iterate them one at a time, invoking the Vulkan shader compiler for each.
Get-ChildItem -Name -Include *.vert,*.frag | Foreach-Object {
    $outputFileName = "..\assets\shaders\vulkan\" + $_
    Write-Host "Compiling Vulkan shader file"$_"..."

    ..\..\..\third-party\vulkan-windows\Bin\glslangValidator.exe -V --target-env vulkan1.0 -o $outputFileName $_

    # Check if the compilation exit code was successful.
    if($LASTEXITCODE -eq 0)
    {
        Write-Host "Compiled"$_" into "$outputFileName" ..."
    } 
    else 
    {
        Write-Host "Error! $_ failed to validate!"
        Exit-PSSession
    }
}
