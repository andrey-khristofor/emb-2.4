// -------------------- TASK 1 --------------------

// #include <Arduino.h>

// constexpr uint8_t BUTTON_PIN = 6;

// bool isButtonPressed = false;

// volatile int pressCount = 0;

// IRAM_ATTR void handleClick()
// {
//   pressCount++;
//   Serial.println(pressCount);
// }

// void setup()
// {
//   Serial.begin(115200);

//   pinMode(BUTTON_PIN, INPUT_PULLDOWN);
//   attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleClick, RISING);
// }

// void loop()
// {
// }

// -------------------- TASK 2 --------------------

// #include <Arduino.h>

// constexpr uint8_t BUTTON_PIN = 6;
// constexpr long DEBOUNCE = 50;

// bool isButtonPressed = false;

// volatile int pressCount = 0;
// volatile long lastPress = 0;
// volatile bool isPressedTrigger = false;

// IRAM_ATTR void handleClick()
// {
//   isPressedTrigger = true;
// }

// void setup()
// {
//   Serial.begin(115200);

//   pinMode(BUTTON_PIN, INPUT_PULLDOWN);
//   attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleClick, RISING);
// }

// void loop()
// {
//   if (isPressedTrigger)
//   {
//     if (digitalRead(BUTTON_PIN) == HIGH && !isButtonPressed)
//     {
//       isButtonPressed = true;

//       pressCount++;

//       Serial.println(pressCount);
//     }

//     isPressedTrigger = false;
//   }

//   if (isButtonPressed && digitalRead(BUTTON_PIN) == LOW)
//   {
//     isButtonPressed = false;
//   }
// }

// -------------------- TASK 3 --------------------

// #include <Arduino.h>

// constexpr int BUTTON_PIN = 6;
// constexpr int STABLE_READS = 2000;

// volatile bool isPressedTrigger = false;

// IRAM_ATTR void handleClick()
// {
//   isPressedTrigger = true;
// }

// bool eventPending = false;
// bool pressHandled = false;
// uint32_t pressCount = 0;

// bool isStableLevel(int);

// void setup()
// {
//   Serial.begin(115200);

//   pinMode(BUTTON_PIN, INPUT_PULLDOWN);
//   attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleClick, RISING);
// }

// void loop()
// {
//   if (isPressedTrigger)
//   {
//     isPressedTrigger = false;
//     eventPending = true;
//   }

//   if (eventPending && !pressHandled)
//   {
//     if (isStableLevel(HIGH))
//     {
//       eventPending = false;
//       pressHandled = true;
//       pressCount++;
//       Serial.println(pressCount);
//     }
//     else if (digitalRead(BUTTON_PIN) == LOW && !isPressedTrigger)
//     {
//       eventPending = false;
//     }
//   }

//   if (pressHandled && digitalRead(BUTTON_PIN) == LOW)
//   {
//     if (isStableLevel(LOW))
//     {
//       pressHandled = false;
//       eventPending = false;
//     }
//   }
// }

// bool isStableLevel(int level)
// {
//   isPressedTrigger = false;
//   for (int i = 0; i < STABLE_READS; i++)
//   {
//     if (digitalRead(BUTTON_PIN) != level || isPressedTrigger)
//     {
//       return false;
//     }
//   }
//   return true;
// }

// -------------------- TASK 4 --------------------

#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 6;
constexpr int POLLING_TIMEOUT_MS = 5;
constexpr int STATES_TO_CONFIRM_COUNT = 4;

enum ButtonState
{
  RELEASED,
  PRESSED
};

ButtonState state = ButtonState::RELEASED;
int pressCount = 0;

long lastPollingTimeMs = 0;

int differentStatesCount = 0;

void handleButtonEvent();

void setup()
{
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
}

void loop()
{
  long now = millis();

  if (now - lastPollingTimeMs > POLLING_TIMEOUT_MS)
  {
    lastPollingTimeMs = now;
    handleButtonEvent();
  }
}

void handleButtonEvent()
{
  bool currentState = digitalRead(BUTTON_PIN) == HIGH;
  bool stableState = state == ButtonState::PRESSED;

  if (currentState == stableState)
  {
    differentStatesCount = 0;
    return;
  }

  differentStatesCount++;

  if (differentStatesCount > STATES_TO_CONFIRM_COUNT)
  {
    state = currentState ? ButtonState::PRESSED : ButtonState::RELEASED;
    if (currentState)
    {
      pressCount++;
      Serial.println(pressCount);
    }
  }
}
