# シェーダーコードをC++のコードに変換します。
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
Write-Output "// Auto generated. DO NOT EDIT."
Write-Output "#pragma once"
Write-Output ""
Write-Output "namespace Lib::Graphics::Program {"

Get-ChildItem -Path ./embed -File | ForEach-Object {
    $filename = $_.BaseName
    $ext = $_.Extension.TrimStart('.') | ForEach-Object { $_.ToUpper() }
    Write-Output ("    namespace {0} {{" -f $filename)
    Write-Output ('        const char* {0} = R"(' -f $ext)
    Get-Content $_.FullName -Encoding UTF8 | ForEach-Object {
        Write-Output $_
    }
    Write-Output '        )";'
    Write-Output "    }"
}

Write-Output "}"