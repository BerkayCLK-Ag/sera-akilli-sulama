@echo off
echo Sera Dashboard Demo aciliyor...
start "" "%~dp0frontend\demo.html"
echo.
echo Tarayicida demo.html acildi.
echo Python kuruluysa gercek backend icin:
echo   cd backend
echo   py -m pip install -r requirements.txt
echo   py -m uvicorn main:app --reload
echo   http://localhost:8000
pause
