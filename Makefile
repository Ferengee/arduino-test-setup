
.PHONY: test_string_reader

test_serial_reader:
	$(MAKE) -C test_string_reader
       
clean: 
	find ./Arduino -name "*.o" -delete	
	$(MAKE) -C test_string_reader clean
