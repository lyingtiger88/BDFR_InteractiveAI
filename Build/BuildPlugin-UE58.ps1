param(
    [string]$UnrealRoot = $env:UE_ROOT,
    [string]$Output = ""
)

$ErrorActionPreference = "Stop"
$repoRoot = Split-Path -Parent $PSScriptRoot
$plugin = Join-Path $repoRoot "BDFR_InteractiveAI.uplugin"

if (-not (Test-Path $plugin)) { throw "Plugin descriptor not found: $plugin" }

if (-not $UnrealRoot) {
    $candidates = @(
        "C:\Program Files\Epic Games\UE_5.8",
        "D:\Epic Games\UE_5.8",
        "D:\Program Files\Epic Games\UE_5.8"
    )
    $UnrealRoot = $candidates | Where-Object { Test-Path $_ } | Select-Object -First 1
}

if (-not $UnrealRoot) { throw "Unreal Engine 5.8 was not found. Pass -UnrealRoot or set UE_ROOT." }

$runUat = Join-Path $UnrealRoot "Engine\Build\BatchFiles\RunUAT.bat"
if (-not (Test-Path $runUat)) { throw "RunUAT.bat not found under UnrealRoot: $UnrealRoot" }

$versionHeader = Join-Path $UnrealRoot "Engine\Source\Runtime\Launch\Resources\Version.h"
if (Test-Path $versionHeader) {
    $versionText = Get-Content $versionHeader -Raw
    if ($versionText -notmatch '#define\s+ENGINE_MAJOR_VERSION\s+5' -or
        $versionText -notmatch '#define\s+ENGINE_MINOR_VERSION\s+8') {
        throw "BDFR_InteractiveAI validation requires Unreal Engine 5.8.x."
    }
}

if (-not $Output) { $Output = Join-Path $repoRoot "Artifacts\UE58" }
New-Item -ItemType Directory -Force -Path $Output | Out-Null

Write-Host "Building BDFR_InteractiveAI with UE 5.8..."
& $runUat BuildPlugin "-Plugin=$plugin" "-Package=$Output" "-TargetPlatforms=Win64" -StrictIncludes
if ($LASTEXITCODE -ne 0) { throw "UE 5.8 BuildPlugin failed with exit code $LASTEXITCODE." }

$builtDescriptor = Join-Path $Output "BDFR_InteractiveAI.uplugin"
if (-not (Test-Path $builtDescriptor)) { throw "BuildPlugin completed but packaged plugin descriptor is missing." }
Write-Host "UE 5.8 plugin build validated: $Output"
