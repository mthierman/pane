Push-Location

$packages = $PSScriptRoot | Split-Path | Join-Path -ChildPath "packages"

New-Item -ItemType Directory $packages -ErrorAction SilentlyContinue
Set-Location -Path $packages

$ada_src = "pane_ada-src"
if (-not (Test-Path "$ada_src"))
{
    gh release -R ada-url/ada download "v3.2.1" -p "singleheader.zip"
    Expand-Archive -Path "singleheader.zip" -DestinationPath "$ada_src"
    Remove-Item -Path "singleheader.zip"
}

$glaze_src = "pane_glaze-src"
if (-not (Test-Path "$glaze_src"))
{
    gh release -R stephenberry/glaze download "v5.0.0" -A zip
    Expand-Archive -Path "glaze-5.0.0.zip" -DestinationPath "."
    Rename-Item -Path "glaze-5.0.0" -NewName $glaze_src
    Remove-Item -Path "glaze-5.0.0.zip"
}

$webview2_src = "pane_webview2-src"
if (-not (Test-Path "$webview2_src"))
{
    nuget install Microsoft.Web.WebView2 -Version "1.0.3124.44" -x
    Rename-Item -Path "Microsoft.Web.WebView2" -NewName $webview2_src
}

$cppwinrt_src = "pane_cppwinrt-src"
if (-not (Test-Path "$cppwinrt_src"))
{
    nuget install Microsoft.Windows.CppWinRT -Version "2.0.240405.15" -x
    Rename-Item -Path "Microsoft.Windows.CppWinRT" -NewName $cppwinrt_src
}

$wil_src = "pane_wil-src"
if (-not (Test-Path "$wil_src"))
{
    nuget install Microsoft.Windows.ImplementationLibrary -Version "1.0.240803.1" -x
    Rename-Item -Path "Microsoft.Windows.ImplementationLibrary" -NewName $wil_src
}

Pop-Location
