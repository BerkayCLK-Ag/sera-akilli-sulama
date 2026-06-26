// Ref: docs/devre-semalari/30-sera-v3-pompa-oled-baglanti.md
// OLED tek basina test — once bunu calistir, ekran gelince sera-v3'e gec
//
// Baglanti: VCC=3V3, GND, SDA=IO21, SCL=IO22
// Kutuphane: Adafruit SSD1306 + Adafruit GFX
// Board: WEMOS D1 MINI ESP32 | Baud: 115200

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_W 128
#define OLED_H 64
#define OLED_ADDR 0x3C
#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_SSD1306 display(OLED_W, OLED_H, &Wire, -1);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED BULUNAMADI — SDA/SCL veya adres (0x3C/0x3D) kontrol");
    for (;;) delay(1000);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Sera v3");
  display.println("OLED OK!");
  display.println("");
  display.println("IO21=SDA IO22=SCL");
  display.display();
  Serial.println("OLED calisiyor");
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Uptime: ");
  display.print(millis() / 1000);
  display.println(" sn");
  display.println("Sonraki: sera-v3");
  display.display();
  delay(1000);
}
