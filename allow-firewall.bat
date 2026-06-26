@echo off
echo Windows Firewall: port 8000 aciliyor (ESP8266 icin)...
echo Yonetici izni gerekebilir.
netsh advfirewall firewall add rule name="Sera Dashboard TCP 8000" dir=in action=allow protocol=TCP localport=8000
if %errorlevel%==0 (
  echo Basarili — ESP artik PC'ye baglanabilir.
) else (
  echo Hata — sag tik -> Yonetici olarak calistir
)
pause
