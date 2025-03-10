if (-not (Test-Path "packages/ada-url/ada"))
{
    gh release -R ada-url/ada download -p "singleheader.zip" -D "packages/ada-url/ada"
    Expand-Archive -Path "packages/ada-url/ada/singleheader.zip" -DestinationPath "packages/ada-url/ada"
    Remove-Item -Path "packages/ada-url/ada/singleheader.zip"
}

if (-not (Test-Path "packages/stephenberry/glaze"))
{
    gh release -R stephenberry/glaze download -A zip -D "packages/stephenberry/glaze"
    Expand-Archive -Path "packages/stephenberry/glaze/glaze-5.0.0.zip" -DestinationPath "packages/stephenberry/glaze"
    Remove-Item -Path "packages/stephenberry/glaze/glaze-5.0.0.zip"
}

nuget install "Microsoft.Web.WebView2" -Version "1.0.3124.44" -x -OutputDirectory "packages"
nuget install "Microsoft.Windows.CppWinRT" -Version "2.0.240405.15" -x -OutputDirectory "packages"
nuget install "Microsoft.Windows.ImplementationLibrary" -Version "1.0.240803.1" -x -OutputDirectory "packages"
