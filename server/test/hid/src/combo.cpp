#include "mbed.h"
#include "USBMouseKeyboard.h"

USBMouseKeyboard key_mouse;

int main(void)
{
  while(1)
  {
      key_mouse.move(20, 0);
      key_mouse.printf("Hello From MBED\r\n");
      delay(1);
  }
}

