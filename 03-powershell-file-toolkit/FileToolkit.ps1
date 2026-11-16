# File Toolkit - folder report and duplicate file finder (PowerShell 7 advanced functions)
# Load   : . ./FileToolkit.ps1

function Get-FolderReport {
    <#
    .SYNOPSIS
        Shows how many files of each type are in a folder and their size.

    .DESCRIPTION
        Get-FolderReport groups the files of a folder by extension
        and shows the count, total size in MB and percent of the folder size.

    .PARAMETER Path
        Folder to scan. Must exist. Can come from the pipeline.

    .PARAMETER Top
        How many file types to show (1 - 20). Default is 5.

    .PARAMETER Recurse
        Switch. If used, sub-folders are scanned too.

    .PARAMETER Color
        Switch. If used, types bigger than 50% are red, others green.

    .EXAMPLE
        Get-FolderReport -Path ~/Downloads

    .EXAMPLE
        Get-FolderReport ~/Documents -Top 3 -Recurse -Color

    .EXAMPLE
        "~/Desktop", "~/Downloads" | Get-FolderReport
    #>

    [CmdletBinding()]
    param(
        [Parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true,
                   HelpMessage = 'Enter a folder path')]
        [ValidateScript({ Test-Path $_ -PathType Container })]
        [string]$Path,

        [Parameter()]
        [ValidateRange(1, 20)]
        [int]$Top = 5,

        [switch]$Recurse,
        [switch]$Color
    )

    BEGIN {
        Write-Verbose "Report started"
    }

    PROCESS {
        Write-Host "`nFolder: $Path" -ForegroundColor Cyan

        $files = Get-ChildItem -Path $Path -File -Recurse:$Recurse
        if (-not $files) {
            Write-Host "Folder is empty"
            return
        }

        $total = ($files | Measure-Object -Property Length -Sum).Sum
        if ($total -eq 0) { $total = 1 }   # avoid division by zero

        $groups = $files | Group-Object -Property Extension

        $result = foreach ($g in $groups) {
            $size = ($g.Group | Measure-Object -Property Length -Sum).Sum

            # variable validation: percent can only be between 0 and 100
            [ValidateRange(0, 100)][double]$percent = [math]::Round($size / $total * 100, 1)

            [PSCustomObject]@{
                Type    = $g.Name
                Count   = $g.Count
                SizeMB  = [math]::Round($size / 1MB, 2)
                Percent = $percent
            }
        }

        $result = $result | Sort-Object -Property SizeMB -Descending | Select-Object -First $Top

        foreach ($r in $result) {
            $line = "{0,-8} {1,5} files {2,8} MB {3,6} %" -f $r.Type, $r.Count, $r.SizeMB, $r.Percent
            if ($Color -and $r.Percent -gt 50) {
                Write-Host $line -ForegroundColor Red
            }
            elseif ($Color) {
                Write-Host $line -ForegroundColor Green
            }
            else {
                Write-Host $line
            }
        }
    }

    END {
        Write-Verbose "Report finished"
    }
}


function Find-DuplicateFile {
    <#
    .SYNOPSIS
        Finds files with the same content in a folder.

    .DESCRIPTION
        Find-DuplicateFile calculates a hash for every file with Get-FileHash.
        Files with the same hash have the same content, even if the names are different.

    .PARAMETER Path
        Folder to search. Must exist.

    .PARAMETER Algorithm
        Hash algorithm: SHA256 (default), SHA1 or MD5.

    .PARAMETER Recurse
        Switch. If used, sub-folders are searched too.

    .EXAMPLE
        Find-DuplicateFile -Path ~/Downloads

    .EXAMPLE
        Find-DuplicateFile ~/Pictures -Algorithm MD5 -Recurse
    #>

    [CmdletBinding()]
    param(
        [Parameter(Mandatory = $true, Position = 0)]
        [ValidateScript({ Test-Path $_ -PathType Container })]
        [string]$Path,

        [Parameter()]
        [ValidateSet('SHA256', 'SHA1', 'MD5')]
        [string]$Algorithm = 'SHA256',

        [switch]$Recurse
    )

    $files = Get-ChildItem -Path $Path -File -Recurse:$Recurse
    if (-not $files) {
        Write-Host "Folder is empty"
        return
    }

    $hashes = $files | Get-FileHash -Algorithm $Algorithm
    $groups = $hashes | Group-Object -Property Hash | Where-Object { $_.Count -gt 1 }

    if (-not $groups) {
        Write-Host "No duplicate files found" -ForegroundColor Green
        return
    }

    foreach ($g in $groups) {
        Write-Host "`nSame content ($($g.Count) files):" -ForegroundColor Yellow
        foreach ($file in $g.Group) {
            Write-Host "  $($file.Path)"
        }
    }
}
