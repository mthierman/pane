if (-not (Test-Path "packages/ada-url/ada"))
{
    gh release -R ada-url/ada download -p "singleheader.zip" -D "packages/ada-url"
    Expand-Archive -Path "packages/ada-url/singleheader.zip" -DestinationPath "packages/ada-url/ada"
    Remove-Item -Path "packages/ada-url/singleheader.zip"
}

if (-not (Test-Path "packages/stephenberry/glaze-5.0.0"))
{
    gh release -R stephenberry/glaze download -A zip -D "packages/stephenberry"
    Expand-Archive -Path "packages/stephenberry/glaze-5.0.0.zip" -DestinationPath "packages/stephenberry"
    Rename-Item -Path "packages/stephenberry/glaze-5.0.0" -NewName "glaze"
    Remove-Item -Path "packages/stephenberry/glaze-5.0.0.zip"
}

nuget install "Microsoft.Web.WebView2" -Version "1.0.3124.44" -x -OutputDirectory "packages"
nuget install "Microsoft.Windows.CppWinRT" -Version "2.0.240405.15" -x -OutputDirectory "packages"
nuget install "Microsoft.Windows.ImplementationLibrary" -Version "1.0.240803.1" -x -OutputDirectory "packages"
