echo '... compile & upload'
arduino-cli compile --fqbn arduino:avr:leonardo media.ino -u -p /dev/ttyACM0
echo '... done'
