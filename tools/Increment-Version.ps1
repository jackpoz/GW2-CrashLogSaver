param(
    [Parameter(Mandatory=$true)]
    [string]$versionHeaderFile
)

$ErrorActionPreference = "Stop"

if (!(Test-Path $versionHeaderFile)) {
    Write-Host "Header file not found: $versionHeaderFile"
    exit 1
}

# Read file
$content = Get-Content $versionHeaderFile

# Regex to match the VERSION_BUILD define
$pattern = '^\s*#define\s+VERSION_BUILD\s+(\d+)\s*$'

$newContent = $content | ForEach-Object {
    if ($_ -match $pattern) {
        $current = [int]$Matches[1]
        $next = $current + 1
        "#define VERSION_BUILD $next"
    } else {
        $_
    }
}

# Write back to file
$newContent | Set-Content $versionHeaderFile -Encoding UTF8

Write-Host "Incremented VERSION_BUILD to $next"
