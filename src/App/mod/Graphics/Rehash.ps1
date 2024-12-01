# コード生成を毎回行うとタイムスタンプが変わってインクリメンタルビルドを邪魔してしまう
# なので前回生成時のハッシュと比べて変化があったときだけコード生成する
function Get-StringHash {
    param (
        [string]$InputString,
        [string]$Algorithm = "SHA256"
    )
    $hashAlgorithm = [System.Security.Cryptography.HashAlgorithm]::Create($Algorithm)
    $byteArray = [System.Text.Encoding]::UTF8.GetBytes($InputString)
    $hashBytes = $hashAlgorithm.ComputeHash($byteArray)
    return -join ($hashBytes | ForEach-Object { "{0:x2}" -f $_ })
}

$source = Invoke-Expression -Command ".\Generate.ps1"
$newHash = (Get-StringHash $source).Trim()
if (Test-Path -Path "./hash.txt") {
    $hash = (Get-Content "./hash.txt").Trim()

    if ($hash -ne $newHash) {
        Set-Content "./include/Graphics/Metadata.hpp" -Encoding UTF8 $source
    }
} else {
    Set-Content "./include/Graphics/Metadata.hpp" -Encoding UTF8 $source
}
Set-Content "./hash.txt" $newHash