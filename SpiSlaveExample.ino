// SPI Slave Example, using interrupt for SS pin
// Runs on Arduino Uno
// Adapted from Nick Gammon
// http://www.gammon.com.au/spi

#include <PinChangeInterrupt.h>

// operation is 'a' (add) or 'm' (multiply)
volatile byte operation = 0;
byte total;

void setup() {
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  // turn on interrupts
  SPCR |= _BV(SPIE);

  // Attach interrupt for SS falling edge using PinChangeInterrupt.
  // If not using PCINT library, connect SS to pin 2:
  //attachInterrupt(digitalPinToInterrupt(2), ss_falling, FALLING);
  attachPCINT(digitalPinToPCINT(SS), ss_falling, FALLING);
}

void loop() {
  // all done with interrupts
}

// interrupt service routine (ISR) ss_falling
void ss_falling() {
  operation = 0;
}

// SPI interrupt routine
ISR(SPI_STC_vect) {
  byte c = SPDR;

  switch (operation) {
    // no operation? then this is the operation
    case 0:
      operation = c;
      total = identity(operation);
      break;

    // add to incoming byte
    case 'a':
      total += c;
      break;

    // multiply by incoming byte
    case 'm':
      total *= c;
      break;
  }

  // return running total
  SPDR = total;
}

int identity(byte op) {
  return (op == 'a') ? 0 : 1;
}

