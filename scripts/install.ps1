Push-Location

Set-Location ($PSScriptRoot | Split-Path)

if (-not (Test-Path "packages/ada-url/ada"))
{
    gh release -R ada-url/ada download "v3.2.1" -p "singleheader.zip" -D "packages/ada-url"
    Expand-Archive -Path "packages/ada-url/singleheader.zip" -DestinationPath "packages/ada-url/ada"
    Remove-Item -Path "packages/ada-url/singleheader.zip"
}

if (-not (Test-Path "packages/stephenberry/glaze"))
{
    gh release -R stephenberry/glaze download "v5.0.0" -A zip -D "packages/stephenberry"
    Expand-Archive -Path "packages/stephenberry/glaze-5.0.0.zip" -DestinationPath "packages/stephenberry"
    Rename-Item -Path "packages/stephenberry/glaze-5.0.0" -NewName "glaze"
    Remove-Item -Path "packages/stephenberry/glaze-5.0.0.zip"
}

nuget install -x -OutputDirectory "packages"

Pop-Location
