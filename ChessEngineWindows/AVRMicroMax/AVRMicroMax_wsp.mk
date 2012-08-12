.PHONY: clean All

All:
	@echo ----------Building project:[ AVRMicroMax - Debug ]----------
	@"mingw32-make.exe"  -j 2 -f "AVRMicroMax.mk"
clean:
	@echo ----------Cleaning project:[ AVRMicroMax - Debug ]----------
	@"mingw32-make.exe"  -j 2 -f "AVRMicroMax.mk" clean
