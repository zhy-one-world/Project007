# Pass 2: remove full-line and trailing garbled // comments from UTF-8 BOM sources.
param(
    [string[]]$Roots = @(
        "E:\Project\Project007\server",
        "E:\Project\SeverBase\include",
        "E:\Project\SeverBase\src"
    )
)

$utf8Bom = New-Object System.Text.UTF8Encoding $true
$excludeDirPattern = '(\\build\\|\\dep\\|\\vcpkg\\|\\.git\\|\\node_modules\\|\\\.vs\\)'
$extensions = @('.cpp', '.hpp', '.h', '.c', '.cc', '.inl')

function Test-GarbledCommentText([string]$text) {
    $t = $text.Trim()
    if ($t.Length -eq 0) { return $true }
    if ($t.IndexOf([char]0xFFFD) -ge 0) { return $true }
    if ($t -match '\?{2,}') { return $true }
    if ($t -match '^[\?\s\.]+$') { return $true }
    if ($t -match '[\u4E00-\u9FFF]') { return $false }
    if ($t -match '[A-Za-z]{2,}') { return $false }
    if ($t -match '[\u0080-\u00FF]') { return $true }
    if ($t -match '^[^\x20-\x7E]+$' -and $t.Length -ge 2) { return $true }
    return $false
}

function Clean-Line([string]$line) {
    if ($line -match '^\s*//(?<comment>.*)$') {
        if (Test-GarbledCommentText $Matches['comment']) { return $null }
        return $line
    }
    if ($line -match '^(?<code>.*?)(?<slash>//)(?<comment>.*)$') {
        if (Test-GarbledCommentText $Matches['comment']) {
            $code = $Matches['code'].TrimEnd()
            if ($code.Length -gt 0) { return $code }
            return $null
        }
    }
    return $line
}

$changed = 0
foreach ($root in $Roots) {
    if (-not (Test-Path $root)) { continue }
    $files = Get-ChildItem -Path $root -Recurse -File -ErrorAction SilentlyContinue |
        Where-Object { $extensions -contains $_.Extension -and $_.FullName -notmatch $excludeDirPattern }
    foreach ($file in $files) {
        $path = $file.FullName
        try {
            $text = [System.IO.File]::ReadAllText($path, $utf8Bom)
            $lines = $text -split "\r?\n"
            $out = New-Object System.Collections.Generic.List[string]
            foreach ($line in $lines) {
                $cleaned = Clean-Line $line
                if ($null -ne $cleaned) { $out.Add($cleaned) | Out-Null }
            }
            $newText = ($out -join "`r`n")
            if ($newText -ne $text) {
                [System.IO.File]::WriteAllText($path, $newText, $utf8Bom)
                $changed++
            }
        }
        catch {
            Write-Warning "Failed: $path - $_"
        }
    }
}
Write-Host "Files cleaned: $changed"
