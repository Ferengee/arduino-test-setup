
.PHONY: test_string_reader

test_serial_reader:
	$(MAKE) -C test_string_reader
       
clean: 
	$(MAKE) -C test_string_reader clean