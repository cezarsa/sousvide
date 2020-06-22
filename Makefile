build:
	pio run -e base

upload:
	pio run -t upload -e base

upload-ota:
	pio run -t upload -e ota

clean:
	pio run -t clean