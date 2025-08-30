@echo off
setlocal enabledelayedexpansion

:: 目标目录（存放复制的 .wff 文件）
set "target_dir=watchfaces"

:: 如果目标目录不存在，则创建
if not exist "%target_dir%" (
    mkdir "%target_dir%"
)

:: 遍历所有子文件夹中的 Bin 目录
for /r %%d in (Bin) do (
    if exist "%%d" (
        pushd "%%d"
        for %%f in (*.wff) do (
            echo 复制: "%%d\%%f" → "%target_dir%\%%f"
            copy "%%f" "..\..\%target_dir%\%%f" /Y
        )
        popd
    )
)

echo 完成！所有 .wff 文件已复制到 %target_dir%\
pause