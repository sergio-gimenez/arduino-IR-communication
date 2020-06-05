#include <IRremote.h>

IRsend irsend;


// New function to send 'items' codes which are held in the array 'data'
void sendMulti (const uint8_t& items, const uint32_t* data)
{
  for (int i = 0; i < 2; i++)
  {
    for (uint8_t itemIndex = 0; itemIndex < items; ++itemIndex)
    {
      irsend.sendNEC(data[itemIndex], 32);
      delay(40);
    }
  }
  delay(500); //5 second delay between each signal burst
}

void setup() {}

void loop () {
  static const uint8_t MaxItems = 3;
  uint32_t  data[MaxItems] = {0x106722DD, 0x10676897, 0x10678877};

  sendMulti(MaxItems, data);
}
