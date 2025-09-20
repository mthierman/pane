Push-Location

$packages = (Get-Item $PSScriptRoot).Parent.FullName | Join-Path -ChildPath "packages"

if (-not (Test-Path $packages))
{
    New-Item -Path $packages -ItemType Directory | Out-Null
}

Set-Location $packages

gh release -R ada-url/ada download -p 'singleheader.zip' --skip-existing | Out-Null
7z x .\singleheader.zip -oada-url/ada | Out-Null

gh release -R stephenberry/glaze download -A zip --skip-existing | Out-Null
7z x .\glaze*.zip -ostephenberry | Out-Null
Rename-Item .\stephenberry\glaze-* glaze -Force

Invoke-WebRequest -Uri "https://sqlite.org/2025/sqlite-src-3500400.zip" -OutFile sqlite.zip
7z x .\sqlite*.zip -osqlite | Out-Null
Rename-Item .\sqlite\sqlite-* sqlite -Force

nuget install Microsoft.Web.WebView2 -DirectDownload -x -OutputDirectory nuget -Verbosity quiet | Out-Null
nuget install Microsoft.Windows.ImplementationLibrary -DirectDownload -x -OutputDirectory nuget -Verbosity quiet | Out-Null
nuget install Microsoft.Windows.CppWinRT -DirectDownload -x -OutputDirectory nuget -Verbosity quiet | Out-Null

Pop-Location
