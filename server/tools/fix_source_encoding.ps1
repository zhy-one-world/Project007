# Safe UTF-8 BOM conversion: only remove full-line garbled // comments.
param(
    [string[]]$Roots = @(
        "E:\Project\Project007\server",
        "E:\Project\SeverBase\include",
        "E:\Project\SeverBase\src"
    )
)

$utf8Bom = New-Object System.Text.UTF8Encoding $true
$utf8Strict = New-Object System.Text.UTF8Encoding $false, $true
$gbk = [System.Text.Encoding]::GetEncoding(936)

$excludeDirPattern = '(\\build\\|\\dep\\|\\vcpkg\\|\\.git\\|\\node_modules\\|\\\.vs\\)'
$extensions = @('.cpp', '.hpp', '.h', '.c', '.cc', '.inl')

function Read-SourceText([byte[]]$bytes) {
    if ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) {
        return $utf8Strict.GetString($bytes, 3, $bytes.Length - 3)
    }
    try {
        return $utf8Strict.GetString($bytes)
    }
    catch {
        return $gbk.GetString($bytes)
    }
}

function Test-GarbledFullLineComment([string]$commentText) {
    $t = $commentText.Trim()
    if ($t.Length -eq 0) { return $true }
    if ($t -match '\uFFFD') { return $true }
    if ($t -match '^[\?\s]+$') { return $true }
    if ($t -match '[\u4E00-\u9FFF]') { return $false }
    if ($t -match '[A-Za-z]{2,}') { return $false }
    if ($t -match '^[\u0080-\u00FF\s\?\.\,\;\:\-\(\)]+$') { return $true }
    return $false
}

function Remove-FullLineGarbledComments([string]$content) {
    $lines = $content -split "\r?\n"
    $out = New-Object System.Collections.Generic.List[string]
    foreach ($line in $lines) {
        if ($line -match '^\s*//(?<comment>.*)$') {
            $comment = $Matches['comment']
            if (Test-GarbledFullLineComment $comment) {
                continue
            }
        }
        $out.Add($line) | Out-Null
    }
    return ($out -join "`r`n")
}

$converted = 0
$cleaned = 0
$skipped = 0

foreach ($root in $Roots) {
    if (-not (Test-Path $root)) { continue }
    $files = @()
    try {
        $files = Get-ChildItem -Path $root -Recurse -File -ErrorAction SilentlyContinue |
            Where-Object { $extensions -contains $_.Extension -and $_.FullName -notmatch $excludeDirPattern }
    }
    catch {
        Write-Warning "Scan failed for $root : $_"
        continue
    }
    foreach ($file in $files) {
        $path = $file.FullName
        try {
            $bytes = [System.IO.File]::ReadAllBytes($path)
            if ($bytes.Length -eq 0) { continue }
            $original = Read-SourceText $bytes
            $cleanedText = Remove-FullLineGarbledComments $original
            $normalized = $cleanedText -replace "\r?\n", "`r`n"
            if ($normalized -ne $original) { $script:cleaned++ }
            $newBytes = $utf8Bom.GetBytes($normalized)
            $same = ($bytes.Length -eq $newBytes.Length)
            if ($same) {
                for ($i = 0; $i -lt $bytes.Length; $i++) {
                    if ($bytes[$i] -ne $newBytes[$i]) { $same = $false; break }
                }
            }
            if (-not $same) {
                [System.IO.File]::WriteAllBytes($path, $newBytes)
                $script:converted++
            }
        }
        catch {
            Write-Warning "Failed: $path - $_"
            $script:skipped++
        }
    }
}

Write-Host "Converted files: $converted"
Write-Host "Cleaned content: $cleaned"
Write-Host "Skipped errors: $skipped"
