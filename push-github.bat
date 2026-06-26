@echo off
echo === Sera GitHub Push ===
echo.
cd /d "%~dp0"
git status --short
echo.
set /p ONAY="Commit + push yapilsin mi? (E/H): "
if /i not "%ONAY%"=="E" exit /b 0
echo.
set /p MESAJ="Commit mesaji (docs: ile basla): "
git add docs/ README.md AGENTS.md .cursor/rules/github-portfolio.mdc push-github.bat
git commit -m "%MESAJ%"
git push
echo.
echo Tamam. GitHub: https://github.com/BerkayCLK-Ag/sera-akilli-sulama
pause
