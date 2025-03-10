if (-not (Test-Path "packages/ada-3.2.1.zip"))
{
    gh release -R ada-url/ada download -A zip -D "packages"
    Expand-Archive -Path "packages/ada-3.2.1.zip" -DestinationPath "packages"
}

if (-not (Test-Path "packages/glaze-5.0.0.zip"))
{
    gh release -R stephenberry/glaze download -A zip -D "packages"
    Expand-Archive -Path "packages/glaze-5.0.0.zip" -DestinationPath "packages"
}

nuget install "Microsoft.Web.WebView2" -Version "1.0.3124.44" -x -OutputDirectory "packages"
nuget install "Microsoft.Windows.CppWinRT" -Version "2.0.240405.15" -x -OutputDirectory "packages"
nuget install "Microsoft.Windows.ImplementationLibrary" -Version "1.0.240803.1" -x -OutputDirectory "packages"
