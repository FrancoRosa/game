# Install TimerOne dependency
# arduino-cli lib search TimerOne
# arduino-cli lib install TimerOne

echo '... compile & upload'
arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328old car_stepper.ino -u -p /dev/ttyUSB0
echo '... done'
