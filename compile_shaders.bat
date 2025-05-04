@echo off
setlocal enabledelayedexpansion

:: Définir les répertoires
set SHADER_DIR=.\shaders
set OUTPUT_DIR=.\bin\Debug\WxVulkanEngine\shaders

:: Vérifier si glslangValidator est disponible
where glslangValidator >nul 2>&1
if errorlevel 1 (
    echo Erreur : glslangValidator n'est pas installé ou n'est pas dans le PATH.
    pause
    exit /b 1
)

:: Créer le répertoire de sortie si inexistant
if not exist "%OUTPUT_DIR%" (
    mkdir "%OUTPUT_DIR%"
)

:: Compiler tous les fichiers en conservant l'extension originale + .spv
for %%f in ("%SHADER_DIR%\*") do (
    echo Compilation du shader : %%~nxf
    glslangValidator -V "%%f" -o "%OUTPUT_DIR%\%%~nxf"
    if errorlevel 1 (
        echo Échec de la compilation de %%~nxf
        pause
        exit /b 1
    )
)

echo Compilation des shaders terminée avec succès !