boolean debounce (int pin)
{
  boolean state;
  boolean previousState;
  int debounceDelay = 10;
  previousState = digitalRead(pin);
  for (int counter = 0; counter <debounceDelay; counter++)
  {
    delay(1);
    state = digitalRead(pin);
    if (state != previousState)
    {
      counter = 0;
      previousState = state;
    }
  }
  if (state == LOW)
    return true;
  else
    return false;
}

