// Ref: docs/malzemeler.md#ds1302
// Faz 3 — Modül 20: DS1302 RTC
// Kurulum: "DS1302RTC" veya "Rtc_By_Makuna" kutuphanesi
// Pinler: CLK→D5, DAT→D6, RST→D7 (3-wire)

#include <ThreeWire.h>
#include <RtcDS1302.h>

ThreeWire myWire(6, 7, 5); // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

void setup() {
  Serial.begin(9600);
  Rtc.Begin();

  if (!Rtc.IsDateTimeValid()) {
    Serial.println("RTC tarihi gecersiz - varsayilan ayarlaniyor");
    Rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));
  }

  Serial.println("DS1302 RTC hazir");
}

void loop() {
  RtcDateTime now = Rtc.GetDateTime();

  Serial.print(now.Year());
  Serial.print("-");
  Serial.print(now.Month());
  Serial.print("-");
  Serial.print(now.Day());
  Serial.print(" ");
  Serial.print(now.Hour());
  Serial.print(":");
  Serial.print(now.Minute());
  Serial.print(":");
  Serial.println(now.Second());

  // Sulama penceresi: 06:00 - 08:00
  if (now.Hour() >= 6 && now.Hour() < 8) {
    Serial.println("  -> Sulama penceresi AKTIF");
  }

  delay(5000);
}
