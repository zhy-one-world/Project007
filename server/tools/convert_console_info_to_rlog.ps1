$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot

$SkipFiles = @(
    (Join-Path $Root "def\log\server_log.hpp")
)

function Find-MatchingParen([string]$Text, [int]$OpenIndex) {
    $depth = 0
    $inString = $null
    for ($i = $OpenIndex; $i -lt $Text.Length; $i++) {
        $ch = $Text[$i]
        if ($null -ne $inString) {
            if ($ch -eq '\' -and ($i + 1) -lt $Text.Length) { $i++; continue }
            if ($ch -eq $inString) { $inString = $null }
            continue
        }
        if ($ch -eq "'" -or $ch -eq '"') { $inString = $ch; continue }
        if ($ch -eq '(') { $depth++ }
        elseif ($ch -eq ')') {
            $depth--
            if ($depth -eq 0) { return $i }
        }
    }
    return -1
}

function Split-TopLevelArgs([string]$ArgsText) {
    $args = New-Object System.Collections.Generic.List[string]
    $current = New-Object System.Text.StringBuilder
    $depthParen = 0; $depthBrace = 0; $depthBracket = 0
    $inString = $null

    foreach ($ch in $ArgsText.ToCharArray()) {
        if ($null -ne $inString) {
            [void]$current.Append($ch)
            if ($ch -eq '\' ) { continue }
            if ($ch -eq $inString) { $inString = $null }
            continue
        }
        if ($ch -eq "'" -or $ch -eq '"') { $inString = $ch; [void]$current.Append($ch); continue }
        switch ($ch) {
            '(' { $depthParen++ }
            ')' { $depthParen-- }
            '{' { $depthBrace++ }
            '}' { $depthBrace-- }
            '[' { $depthBracket++ }
            ']' { $depthBracket-- }
            ',' {
                if ($depthParen -eq 0 -and $depthBrace -eq 0 -and $depthBracket -eq 0) {
                    $part = $current.ToString().Trim()
                    if ($part) { $args.Add($part) }
                    $current.Clear() | Out-Null
                    continue
                }
            }
        }
        [void]$current.Append($ch)
    }
    $tail = $current.ToString().Trim()
    if ($tail) { $args.Add($tail) }
    return ,$args
}

function Convert-ConsoleInfoArgs([string]$ArgsText) {
    $parts = Split-TopLevelArgs $ArgsText
    if ($parts.Count -eq 0) { return '_RLOG_(MINFO, "")' }
    if ($parts.Count -eq 1) { return "_RLOG_(MINFO, $($parts[0]))" }
    $rest = ($parts[1..($parts.Count - 1)] -join ', ')
    return "_RLOG_(MINFO, ::faith::log_detail::format_message($($parts[0]), $rest))"
}

function Convert-Content([string]$Content) {
    $pattern = '(?<!//)\bCONSOLE_INFO\s*\('
    $matches = [regex]::Matches($Content, $pattern)
    if ($matches.Count -eq 0) { return @{ Content = $Content; Count = 0 } }

    $pieces = New-Object System.Collections.Generic.List[string]
    $last = 0
    $count = 0
    foreach ($match in $matches) {
        $start = $match.Index
        $openParen = $match.Index + $match.Length - 1
        $closeParen = Find-MatchingParen $Content $openParen
        if ($closeParen -lt 0) { continue }
        $argsText = $Content.Substring($openParen + 1, $closeParen - $openParen - 1)
        $replacement = Convert-ConsoleInfoArgs $argsText
        [void]$pieces.Add($Content.Substring($last, $start - $last))
        [void]$pieces.Add($replacement)
        $last = $closeParen + 1
        $count++
    }
    [void]$pieces.Add($Content.Substring($last))
    return @{ Content = ($pieces -join ''); Count = $count }
}

function Ensure-RlogInclude([string]$Content) {
    if ($Content -notmatch '_RLOG_\(') { return $Content }
    if ($Content -match '#include\s*[<"]rlog\.hpp[>"]') { return $Content }
    if ($Content -match '#include\s*<common/rlog\.hpp>') { return $Content }

    $all = $Content -split "`r?`n"
    $lastInclude = -1
    for ($i = 0; $i -lt $all.Length; $i++) {
        if ($all[$i] -match '^\s*#include') { $lastInclude = $i }
    }
    if ($lastInclude -lt 0) { return "#include <rlog.hpp>`r`n$Content" }
    $before = $all[0..$lastInclude] -join "`r`n"
    $after = if ($lastInclude + 1 -lt $all.Length) { "`r`n" + ($all[($lastInclude + 1)..($all.Length - 1)] -join "`r`n") } else { "" }
    return "$before`r`n#include <rlog.hpp>$after"
}

$totalFiles = 0
$totalReplacements = 0
$files = Get-ChildItem -Path $Root -Recurse -Include *.cpp,*.hpp,*.h,*.cc -File |
    Where-Object { $_.FullName -notmatch '\\build\\|\\.vs\\' -and ($SkipFiles -notcontains $_.FullName) }

foreach ($file in $files) {
    $original = [System.IO.File]::ReadAllText($file.FullName)
    $result = Convert-Content $original
    if ($result.Count -eq 0) { continue }
    $converted = Ensure-RlogInclude $result.Content
    if ($converted -ne $original) {
        $utf8Bom = New-Object System.Text.UTF8Encoding $true
        [System.IO.File]::WriteAllText($file.FullName, $converted, $utf8Bom)
        $totalFiles++
        $totalReplacements += $result.Count
        Write-Host ("{0,3}  {1}" -f $result.Count, $file.FullName.Substring($Root.Length + 1))
    }
}

Write-Host ""
Write-Host "Updated $totalReplacements call sites in $totalFiles files."
