# PowerShell File Toolkit

Two advanced functions (PowerShell 7):

- **Get-FolderReport** — groups a folder's files by extension and shows count, size and share of the folder. Accepts folders from the pipeline (`BEGIN/PROCESS/END`), `-Top` (1–20), `-Recurse`, `-Color`.
- **Find-DuplicateFile** — finds files with identical content using `Get-FileHash`; `-Algorithm` is limited to `SHA256`, `SHA1`, `MD5` with `ValidateSet`.

Both use `[CmdletBinding()]`, parameter validation and comment-based help (`Get-Help Get-FolderReport -Full`).

```powershell
. ./FileToolkit.ps1
Get-FolderReport -Path ~/Documents -Recurse -Top 10 -Color
"./a", "./b" | Get-FolderReport
Find-DuplicateFile -Path ~/Downloads -Recurse -Algorithm MD5
```
