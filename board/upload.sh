echo '... compile & upload'
arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328old board.ino -u -p /dev/ttyUSB0
echo '... done'
