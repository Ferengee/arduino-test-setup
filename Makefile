
.PHONY: test_string_reader

test_serial_reader:
	$(MAKE) -C test_string_reader
	$(MAKE) -C office_distraction

       
clean: 
	find ./Arduino -name "*.o" -delete	
	$(MAKE) -C test_string_reader clean
