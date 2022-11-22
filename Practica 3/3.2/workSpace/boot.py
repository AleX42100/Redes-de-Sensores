
import time
from machine import Pin, ADC, PWM
from machine import UART

#uart = UART(1, 115200)                         # init with given baudrate
#uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters



led=Pin(5,Pin.OUT)          #create LED object from pin5,Set Pin5 to output
#button = Pin(12, Pin.IN)

while True:
  led.value(1)              #turn off
  time.sleep(0.1)
  led.value(0)              #turn on
  time.sleep(0.1)
  #uart.write('Ceci est un test')
  #print('LOOP')
  #button_state = button.value()
  pot = ADC(Pin(12))
  pot_value = pot.read()
  print('Potentiometer raw value: ',pot_value)
  
