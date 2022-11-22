# AS5200L Arduino library

Library interfacing AS5200L magnetic encoder over I2C bus.


Notes: 
- When setting start and stop angle, stop angle has to be minimum 18 degrees (205 steps) greater than start angle. Otherwise stop angle won't be setted up and it's MPOS register will respond with the same value as start angle ZPOS register.
  
  
GNU License  
Copyright (c) 2020, Amadeusz Świerk
