// =============================================================================
// ConnectorX Animate - Custom Firmware Example
// =============================================================================
//
// Three custom animations and a custom module (OledModule.h), registered
// with the Lumyn Labs SDK.
// =============================================================================

#include <Arduino.h>
#include <LumynLabs.h>
#include <LumynLabs/CxAnimate.h>

#include "OledModule.h"

// =============================================================================
// Custom Animations
// =============================================================================
//
// An animation is an Effect: step() moves it on by one tick and draw() paints
// the current frame. Ticks run at 60 per second at the animation's default
// delay; a command with a shorter delay runs them faster, a longer one slower.
// make() builds a fresh effect for each zone it plays on.

/**
 * Example: Simple fill animation
 * Fills the zone with the selected color.
 */
class FillEffect : public LumynLabs::Effect
{
public:
  void step() override {}
  void draw(LumynLabs::Frame &frame) override
  {
    for (int i = 0; i < frame.size(); i++)
    {
      frame.pixels[i] = frame.color;
    }
  }
};

static const LumynLabs::AnimationInstance CustomFill = {
    .id = "CUSTOM_FILL",
    .defaultDelay = 20000,
    .defaultColor = LumynLabs::Color::Blue(),
    .cycleTicks = 1,
    // Still: it redraws only when its color changes.
    .flags = LumynLabs::kStill,
    .make = LumynLabs::makeEffect<FillEffect>};

/**
 * Example: Simple blink animation
 * On for half a second, then off for half a second.
 */
class BlinkEffect : public LumynLabs::Effect
{
public:
  static constexpr uint32_t kPeriod = LumynLabs::kTicksPerSecond;

  void step() override { _tick = (_tick + 1) % kPeriod; }
  void draw(LumynLabs::Frame &frame) override
  {
    if (_tick >= kPeriod / 2)
      return; // A frame starts cleared, so off is drawing nothing.
    for (int i = 0; i < frame.size(); i++)
    {
      frame.pixels[i] = frame.color;
    }
  }

private:
  uint32_t _tick = 0;
};

static const LumynLabs::AnimationInstance CustomBlink = {
    .id = "CUSTOM_BLINK",
    .defaultDelay = 500,
    .defaultColor = LumynLabs::Color::Red(),
    .cycleTicks = BlinkEffect::kPeriod,
    .flags = 0,
    .make = LumynLabs::makeEffect<BlinkEffect>};

/**
 * Example: Chase animation
 * A single lit LED crosses the zone every two seconds.
 */
class ChaseEffect : public LumynLabs::Effect
{
public:
  static constexpr uint32_t kCycle = 2 * LumynLabs::kTicksPerSecond;

  void step() override { _tick = (_tick + 1) % kCycle; }
  void draw(LumynLabs::Frame &frame) override
  {
    if (frame.size() == 0)
      return;
    frame.pixels[_tick * frame.size() / kCycle] = frame.color;
  }

private:
  uint32_t _tick = 0;
};

static const LumynLabs::AnimationInstance CustomChase = {
    .id = "CUSTOM_CHASE",
    .defaultDelay = 50,
    .defaultColor = LumynLabs::Color::Green(),
    .cycleTicks = ChaseEffect::kCycle,
    .flags = 0,
    .make = LumynLabs::makeEffect<ChaseEffect>};

// =============================================================================
// Setup & Loop
// =============================================================================

void setup()
{
  // Start the board's services; true sends logs to the second USB serial port.
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

  // Register your custom modules. A config's module of type "SSD1306" runs
  // this one.
  LumynLabs::registerModule<OledStatus, OledModule>("SSD1306");

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
  //   - a delay of 100: half speed, since its default delay is 50
  //     (speed = default delay / delay)
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

// The SDK runs everything in its own tasks, so loop() ends its task and
// frees the task's 4 KB stack for the heap. To run code in a loop instead,
// replace this with your loop() and keep in mind the 4 KB it costs.
void loop() { vTaskDelete(NULL); }

// No setup1() or loop1(): defining either starts a second task with another
// 4 KB stack.