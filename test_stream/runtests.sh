while true; do inotifywait -e modify -e close_write *.c *.h ../Arduino/DummySerial.* ; make test ;done
