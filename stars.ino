#include <FastLED.h>

#define NUM_LEDS 26
#define LED_PIN 5

#define BRIGHT_IN A0

#define LED_TYPE WS2811
#define COLOR_ORDER RGB
#define DEFAULT_BRIGHTNESS 80

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  
  randomSeed(analogRead(0));
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( 0 );
  pinMode(BRIGHT_IN, INPUT);
}

void loop() {

  int v = analogRead(BRIGHT_IN);

  int bright;
  if (v < 50) {
    bright = 0;
  } else if (v > 650) {
    bright = 255;
  } else {
    bright = map(v, 50, 650, 0, 255);
  }
  
  FastLED.setBrightness(  bright );
//  Serial.print("v=");
//  Serial.print(v);
//  Serial.print(", bright=");
//  Serial.println(bright);

  if (bright == 0) {
    for(int i=0; i<NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
    FastLED.delay(100);
    return;
  }
  

  static float flashBright = 1.0f;
  static int8_t flashIdx = -1;
  static bool flashUp = true;
  
  CRGB flashColor;
  // save flash color apart from rainbow
  if (flashIdx != -1) {
    flashColor = leds[flashIdx];
  }
  
  // draw a generic, no-name rainbow
  static uint8_t starthue = 0;
  FastLED.setTemperature( OvercastSky ); // first temperature
  fill_rainbow( leds, NUM_LEDS, --starthue, 5);
  for(int i=0; i<NUM_LEDS; i++) {
    leds[i].nscale8(128);
  }

  if (flashIdx != -1) {
    if (flashColor.b == 0) {
      // flash is done
      flashIdx = -1;
    } else {
      leds[flashIdx] = flashColor;
    }
  }

  if (flashIdx == -1 || leds[flashIdx].b == 0) {
    // new flasher! start low and increase
    flashUp = true;
    flashIdx = random(NUM_LEDS);
    flashBright = 1.0;
  }

  if (flashUp) {
    flashBright *= 1.2;
    int b = (int)flashBright;
    if (b >= 255) {
      b = 255;
      flashUp = false;
    }
    leds[flashIdx] = CRGB( b, b, b);
  } else {
    leds[flashIdx].fadeToBlackBy(4);
  }

  FastLED.show();
  FastLED.delay(36);
}
