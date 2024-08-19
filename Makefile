CC=gcc
BUILD=build

.PHONY: all clean

all: $(BUILD)/Ydbg

$(BUILD)/Ydbg: main.c | $(BUILD)
	$(CC) -o $(BUILD)/Ydbg main.c

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)
