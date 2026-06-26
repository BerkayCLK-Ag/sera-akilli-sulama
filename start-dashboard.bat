@echo off
echo Sera Dashboard baslatiliyor...
echo.
echo Bilgisayar LAN IP (ESP otomatik bulur, elle yazmana gerek yok):
for /f "tokens=2 delims=:" %%a in ('ipconfig ^| findstr /c:"IPv4"') do echo   %%a
echo.
echo Firewall port 8000 aciliyor...
netsh advfirewall firewall add rule name="Sera Dashboard TCP 8000" dir=in action=allow protocol=TCP localport=8000 >nul 2>&1
set PATH=%PATH%;C:\Users\berka\AppData\Local\Programs\Python\Python312;C:\Users\berka\AppData\Local\Programs\Python\Python312\Scripts
cd /d "%~dp0backend"
start "Sera Backend" cmd /k "py -m uvicorn main:app --host 0.0.0.0 --port 8000 --reload"
timeout /t 3 /nobreak >nul
start http://localhost:8000
echo Dashboard: http://localhost:8000
echo ESP UDP ile PC'yi port 9999 uzerinden bulur.
