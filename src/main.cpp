// =============================================================================
// ConnectorX Animate - Custom Firmware Example
// =============================================================================
//
// This example demonstrates how to create and register custom animations
// using the Lumyn Labs SDK.
//
// For more examples, see the animations/ folder.
// =============================================================================

#include <Arduino.h>
#include <LumynLabs.h>
#include <LumynLabs/CxAnimate.h>

// =============================================================================
// Custom Animations
// =============================================================================

/**
 * Example: Simple fill animation
 * Fills all LEDs with the selected color.
 */
static LumynLabs::AnimationInstance CustomFill = {
    .id = "CUSTOM_FILL",
    .stateMode = LumynLabs::AnimationStateMode::Constant,
    .stateCount = 1, // Single state - static fill
    .defaultDelay = 20000,
    .defaultColor = LumynLabs::Color::Blue(),
    .cb = [](LumynLabs::Color *strip, LumynLabs::Color color, uint16_t state, uint16_t count)
    {
      for (uint16_t i = 0; i < count; i++)
      {
        strip[i] = color;
      }
      return true; // Update the LEDs
    }};

/**
 * Example: Simple blink animation
 * Alternates between color and off.
 */
static LumynLabs::AnimationInstance CustomBlink = {
    .id = "CUSTOM_BLINK",
    .stateMode = LumynLabs::AnimationStateMode::Constant,
    .stateCount = 2, // Two states: on and off
    .defaultDelay = 500,
    .defaultColor = LumynLabs::Color::Red(),
    .cb = [](LumynLabs::Color *strip, LumynLabs::Color color, uint16_t state, uint16_t count)
    {
      LumynLabs::Color c = (state == 0) ? color : LumynLabs::Color::Black();
      for (uint16_t i = 0; i < count; i++)
      {
        strip[i] = c;
      }
      return true;
    }};

/**
 * Example: Chase animation (LED count based)
 * A single lit LED moves along the strip.
 */
static LumynLabs::AnimationInstance CustomChase = {
    .id = "CUSTOM_CHASE",
    .stateMode = LumynLabs::AnimationStateMode::LedCount, // State count = LED count
    .stateCount = 0,
    .defaultDelay = 50,
    .defaultColor = LumynLabs::Color::Green(),
    .cb = [](LumynLabs::Color *strip, LumynLabs::Color color, uint16_t state, uint16_t count)
    {
      for (uint16_t i = 0; i < count; i++)
      {
        strip[i] = (i == state) ? color : LumynLabs::Color::Black();
      }
      return true;
    }};

// =============================================================================
// Setup & Loop
// =============================================================================

void setup()
{
  // Initialize the system (networking, display, etc.)
  bool success = LumynLabs::System::init(true);
  if (!success)
  {
    Serial.println("System init failed");
    while (1)
    {
      delay(1000);
    }
  }

  // Register your custom animations
  LumynLabs::registerAnimation(CustomFill);
  LumynLabs::registerAnimation(CustomBlink);
  LumynLabs::registerAnimation(CustomChase);

  // Complete initialization (starts services)
  success = LumynLabs::System::initServices();
  if (!success)
  {
    Serial.println("Service init failed");
    while (1)
    {
      delay(1000);
    }
  }

  Serial.printf("ConnectorX Animate ready! SDK v%s\n", LumynLabs::getSDKVersion());

  // =========================================================================
  // Example: Set an animation on a zone using the LED Service API
  // =========================================================================
  // Note: Zone IDs are configured in Lumyn Studio and stored on the device.
  // Common zone IDs include "zone1", "zone2", etc. or custom names you define.
  //
  // This sets the custom chase animation on "zone1" with:
  //   - 100ms delay between animation frames
  //   - Purple color
  //   - Normal direction (not reversed)
  //   - Continuous loop (not one-shot)
  LumynLabs::Led::setAnimation("zone1", "CUSTOM_CHASE", 100,
                               LumynLabs::Color::Purple());

  // You can also set animations on zone groups:
  LumynLabs::Led::setAnimationGroup("all", "CUSTOM_BLINK", 500,
                                    LumynLabs::Color::Red());

  // Or set a solid color:
  // LumynLabs::Led::setColor("zone2", LumynLabs::Color::Green());
}

void loop() { delay(10000); }

// Second core setup, safest to do nothing here
void setup1() {}

void loop1() { delay(10000); }