pkill -f picocom
echo '... compile & upload'
arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328old matrix.ino -u -p /dev/ttyUSB0
echo '... done'
