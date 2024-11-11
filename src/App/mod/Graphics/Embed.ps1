# シェーダーコードをC++のコードに変換します。
function Assert {
    param (
        [bool]$condition,
        [string]$message = "Assertion failed"
    )
    
    if (-not $condition) {
        throw $message
    }
}

function GetOrThrow {
    param (
        [hashtable]$hashtable,
        [string]$key
    )
    
    if (-not $hashtable.ContainsKey($key)) {
        throw $("$key is not exist.")
    }
    return $hashtable[$key]
}


[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
Write-Output "// Auto generated. DO NOT EDIT."
Write-Output "#pragma once"
Write-Output "#include <Graphics/Reflect.hpp>"
Write-Output ""
Write-Output "namespace Lib::Graphics::Metadata {"
Write-Output "    class Uniform {"
Write-Output "    public:"
Write-Output "        size_t size;"
Write-Output "        bool isShaderResource;"
Write-Output "    };"
Write-Output ""
Write-Output "    class Program {"
Write-Output "    public:"
Write-Output "        Reflect::InputLayout inputLayout;"
Write-Output "        Reflect::PrimitiveType primitiveType;"
Write-Output "        const char* vsCode;"
Write-Output "        std::vector<Uniform> vsUniforms;"
Write-Output "        const char* psCode;"
Write-Output "        std::vector<Uniform> psUniforms;"
Write-Output "    };"
Write-Output ""

$programs = @()
$propertiesList = @()
foreach ($metaMetaData in Get-Content "./embed/Meta.meta" -Encoding UTF8) {
    if ($metaMetaData.StartsWith("#")) {
        continue
    }
    $programs += $metaMetaData.SubString(0, $metaMetaData.IndexOf("."))
    $properties = @{}
    foreach ($metaData in Get-Content $("./embed/$metaMetaData") -Encoding UTF8) {
        if ($metaData -match '^\s*$' -or $metaData -match '^\s*[;#]') {
            return
        }
        if ($metaData -eq "") {
            return
        }
        $words = $metaData -split "="
        $key = $words[0].Trim()
        $value = $words[1].Trim()
        $properties[$key] = $value
    }
    $propertiesList += $properties
}

Write-Output "    enum ProgramTable : int32_t {"
foreach ($name in $programs) {
    Write-Output ("        {0}," -f $name)
}
Write-Output "        Count"
Write-Output "    };"
Write-Output ""

Write-Output "    const std::vector<Program> k_programs = std::vector<Program> {"
foreach ($properties in $propertiesList) {
    Write-Output "        Program {"
    Write-Output "            // inputLayout"
    Write-Output ("            Reflect::InputLayout::{0}," -f (GetOrThrow $properties "InputLayout"))
    Write-Output "            // primitiveType"
    Write-Output ("            Reflect::PrimitiveType::{0}," -f (GetOrThrow $properties "PrimitiveType"))

    Write-Output "            // vs"
    $vsCode = (GetOrThrow $properties "VS.Code")
    foreach ($vsLine in Get-Content $("./embed/$vsCode") -Encoding UTF8) {
        Write-Output ('            "{0}"' -f $vsLine)
    }
    Write-Output ('            ,')
    Write-Output "            // vsUniforms"
    Write-Output "            std::vector<Uniform> {"
    $vsUniformCount = [int](GetOrThrow $properties "VS.UniformCount")
    for ($i = 0; $i -lt $vsUniformCOunt; $i++) {
        Write-Output ("                Uniform {{ sizeof(Reflect::{0}), false }}," -f (GetOrThrow $properties $("VS.Uniform[$i]")))
    }
    Write-Output "            },"

    Write-Output "            // ps"
    $psCode = (GetOrThrow $properties "PS.Code")
    foreach ($psLine in Get-Content $("./embed/$psCode") -Encoding UTF8) {
        Write-Output ('            "{0}"' -f $psLine)
    }
    Write-Output ('            ,')
    Write-Output "            // psUniforms"
    Write-Output "            std::vector<Uniform> {"
    $psUniformCount = [int](GetOrThrow $properties "PS.UniformCount")
    for ($i = 0; $i -lt $psUniformCOunt; $i++) {
        $psUniform = (GetOrThrow $properties $("PS.Uniform[$i]"))
        if ($psUniform -eq "Texture") {
            Write-Output "                Uniform { 0, true },"
        } else {
            Write-Output ("                Uniform {{ sizeof(Reflect::{0}), false }}," -f $psUniform)
        }
    }
    Write-Output "            },"
    Write-Output "        },"
}
Write-Output "    };"
Write-Output ""

Write-Output "    template<int32_t N>"
Write-Output "    class Signature {"
Write-Output "    public:"
Write-Output "        static inline void set() { }"
Write-Output "    };"
Write-Output ""

for ($i = 0; $i -lt $programs.Length; $i++) {
    Write-Output "    template<>"
    Write-Output ("    class Signature<ProgramTable::{0}> {{" -f $programs[$i])
    Write-Output "    public:"
    Write-Output "        static inline void set() { }"
    Write-Output "    };"
    Write-Output ""
}
Write-Output "}"