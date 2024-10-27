echo '... compile & upload'
arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328old test.ino -u -p /dev/ttyUSB0
echo '... done'
