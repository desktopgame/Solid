# シェーダーコードをC++のコードに変換します。

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
Write-Output "        enum class Type : uint32_t {"
Write-Output "            CBV = 0,"
Write-Output "            SRV,"
Write-Output "            UAV,"
Write-Output "        };"
Write-Output "        size_t size;"
Write-Output "        Type type;"
Write-Output "    };"
Write-Output ""
Write-Output "    class Stencil {"
Write-Output "    public:"
Write-Output "        bool isRead;"
Write-Output "        bool isWrite;"
Write-Output "        Reflect::StencilOp frontFaceFailOp;"
Write-Output "        Reflect::StencilOp frontFaceDepthFailOp;"
Write-Output "        Reflect::StencilOp frontFacePassOp;"
Write-Output "        Reflect::StencilFunc frontFaceFunc;"
Write-Output "        Reflect::StencilOp backFaceFailOp;"
Write-Output "        Reflect::StencilOp backFaceDepthFailOp;"
Write-Output "        Reflect::StencilOp backFacePassOp;"
Write-Output "        Reflect::StencilFunc backFaceFunc;"
Write-Output "    };"
Write-Output ""
Write-Output "    class Program {"
Write-Output "    public:"
Write-Output "        Reflect::InputLayout inputLayout;"
Write-Output "        std::vector<Reflect::InstanceBufferType> instanceBufferLayout;"
Write-Output "        Reflect::PrimitiveType primitiveType;"
Write-Output "        bool isWireframe;"
Write-Output "        Stencil stencil;"
Write-Output "        const char* vsCode;"
Write-Output "        std::vector<Uniform> vsUniforms;"
Write-Output "        const char* gsCode;"
Write-Output "        std::vector<Uniform> gsUniforms;"
Write-Output "        const char* psCode;"
Write-Output "        std::vector<Uniform> psUniforms;"
Write-Output "        const char* csCode;"
Write-Output "        std::vector<Uniform> csUniforms;"
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
    Write-Output "            // instanceBufferLayout"
    Write-Output "            std::vector<Reflect::InstanceBufferType> {"
    $instanceBufferCount = (GetOrThrow $properties "InstanceBufferCount")
    for ($i = 0; $i -lt $instanceBufferCount; $i++) {
        Write-Output ("                Reflect::InstanceBufferType::{0}," -f (GetOrThrow $properties $("InstanceBuffer[$i]")))
    }
    Write-Output "            },"
    Write-Output "            // primitiveType"
    Write-Output ("            Reflect::PrimitiveType::{0}," -f (GetOrThrow $properties "PrimitiveType"))
    Write-Output "            // isWireframe"
    Write-Output ("            {0}," -f (GetOrThrow $properties "IsWireframe"))
    Write-Output "            // stencil"
    $stencilRead = (GetOrThrow $properties "Stencil.Read")
    $stencilWrite = (GetOrThrow $properties "Stencil.Write")
    $stencilFrontFaceFailOp = (GetOrThrow $properties "Stencil.FrontFace.FailOp")
    $stencilFrontFaceDepthFailOp = (GetOrThrow $properties "Stencil.FrontFace.DepthFailOp")
    $stencilFrontFacePassOp = (GetOrThrow $properties "Stencil.FrontFace.PassOp")
    $stencilFrontFaceFunc = (GetOrThrow $properties "Stencil.FrontFace.Func")
    $stencilFrontBackFailOp = (GetOrThrow $properties "Stencil.BackFace.FailOp")
    $stencilFrontBackDepthFailOp = (GetOrThrow $properties "Stencil.BackFace.DepthFailOp")
    $stencilFrontBackPassOp = (GetOrThrow $properties "Stencil.BackFace.PassOp")
    $stencilFrontBackFunc = (GetOrThrow $properties "Stencil.BackFace.Func")
    Write-Output ("            Stencil { {0}, {1}, Reflect::StencilOp::{2}, Reflect::StencilOp::{3}, Reflect::StencilOp::{4}, Reflect::StencilFunc::{5}, Reflect::StencilOp::{6}, Reflect::StencilOp::{7}, Reflect::StencilOp::{8}, Reflect::StencilFunc::{9} }," -f $stencilRead $stencilWrite $stencilFrontFaceFailOp $stencilFrontFaceDepthFailOp $stencilFrontFacePassOp $stencilFrontFaceFunc $stencilFrontBackFailOp $stencilFrontBackDepthFailOp $stencilFrontBackPassOp $stencilFrontBackFunc)

    Write-Output "            // vs"
    $vsCode = (GetOrThrow $properties "VS.Code")
    foreach ($vsLine in Get-Content $("./embed/$vsCode") -Encoding UTF8) {
        Write-Output ('            "{0}\n"' -f $vsLine)
    }
    Write-Output ('            ,')
    Write-Output "            // vsUniforms"
    Write-Output "            std::vector<Uniform> {"
    $vsUniformCount = [int](GetOrThrow $properties "VS.UniformCount")
    for ($i = 0; $i -lt $vsUniformCOunt; $i++) {
        Write-Output ("                Uniform {{ sizeof(Reflect::{0}), Uniform::Type::CBV }}," -f (GetOrThrow $properties $("VS.Uniform[$i]")))
    }
    Write-Output "            },"

    if ((GetOrThrow $properties "UseGeometryShader") -eq "true") {
        Write-Output "            // gs"
        $gsCode = (GetOrThrow $properties "GS.Code")
        foreach ($gsLine in Get-Content $("./embed/$gsCode") -Encoding UTF8) {
            Write-Output ('            "{0}\n"' -f $gsLine)
        }
        Write-Output ('            ,')
        Write-Output "            // gsUniforms"
        Write-Output "            std::vector<Uniform> {"
        $gsUniformCount = [int](GetOrThrow $properties "GS.UniformCount")
        for ($i = 0; $i -lt $gsUniformCOunt; $i++) {
            Write-Output ("                Uniform {{ sizeof(Reflect::{0}), Uniform::Type::CBV }}," -f (GetOrThrow $properties $("GS.Uniform[$i]")))
        }
        Write-Output "            },"
    } else {
        Write-Output "            // gs"
        Write-Output ('            nullptr,')
        Write-Output "            // gsUniforms"
        Write-Output "            std::vector<Uniform> {"
        Write-Output "            },"
    }

    Write-Output "            // ps"
    $psCode = (GetOrThrow $properties "PS.Code")
    foreach ($psLine in Get-Content $("./embed/$psCode") -Encoding UTF8) {
        Write-Output ('            "{0}\n"' -f $psLine)
    }
    Write-Output ('            ,')
    Write-Output "            // psUniforms"
    Write-Output "            std::vector<Uniform> {"
    $psUniformCount = [int](GetOrThrow $properties "PS.UniformCount")
    for ($i = 0; $i -lt $psUniformCOunt; $i++) {
        $psUniform = (GetOrThrow $properties $("PS.Uniform[$i]"))
        if ($psUniform -eq "UTexture") {
            Write-Output "                Uniform { 0, Uniform::Type::SRV },"
        } else {
            Write-Output ("                Uniform {{ sizeof(Reflect::{0}), Uniform::Type::CBV }}," -f $psUniform)
        }
    }
    Write-Output "            },"

    if ((GetOrThrow $properties "UseComputeShader") -eq "true") {
        Write-Output "            // cs"
        $csCode = (GetOrThrow $properties "CS.Code")
        foreach ($csLine in Get-Content $("./embed/$csCode") -Encoding UTF8) {
            Write-Output ('            "{0}\n"' -f $csLine)
        }
        Write-Output ('            ,')
        Write-Output "            // csUniforms"
        Write-Output "            std::vector<Uniform> {"
        $csUniformCount = [int](GetOrThrow $properties "CS.UniformCount")
        for ($i = 0; $i -lt $csUniformCount; $i++) {
            $csUniform = (GetOrThrow $properties $("CS.Uniform[$i]"))
            $pattern = "UInstance<([^>]+)>"
            if ($csUniform -eq "UTexture") {
                Write-Output "                Uniform { 0, Uniform::Type::SRV },"
            } elseif ($csUniform -match $pattern) {
                $innerType = $matches[1]
                Write-Output ("                Uniform {{ Reflect::InstanceBufferSize[static_cast<int32_t>(Reflect::InstanceBufferType::{0})], Uniform::Type::UAV }}," -f $innerType)
            } else {
                Write-Output ("                Uniform {{ sizeof(Reflect::{0}), Uniform::Type::CBV }}," -f $csUniform)
            }
        }
        Write-Output "            },"
    } else {
        Write-Output "            // cs"
        Write-Output ('            nullptr,')
        Write-Output "            // csUniforms"
        Write-Output "            std::vector<Uniform> {"
        Write-Output "            },"
    }

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