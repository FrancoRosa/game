echo '... compile & upload'
arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328old car.ino -u -p /dev/ttyUSB0
echo '... done'
