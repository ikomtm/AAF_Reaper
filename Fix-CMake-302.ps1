$root = "C:\Users\KSN_PC\C++Projects\AAF_Reaper\AAF_SDK"

$files = Get-ChildItem -Path $root -Recurse -Include *.cmake, CMakeLists.txt -File

foreach ($file in $files) {
    Write-Host "Patching file: $($file.FullName)"
    
    $content = Get-Content $file.FullName
    $newContent = @()

    foreach ($line in $content) {
        if ($line -match 'cmake_minimum_required\s*\(VERSION\s*3\.0\.2\)') {
            $newContent += 'cmake_minimum_required(VERSION 3.14)'
            $newContent += 'cmake_policy(VERSION 3.14)'
        } else {
            $newContent += $line
        }
    }

    Set-Content -Path $file.FullName -Value $newContent
}

Write-Host ""
Write-Host "All version statements patched successfully."
