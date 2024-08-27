CC=gcc
BUILD_DIR=build
SRC_DIR=src

.PHONY: all app clean

app: Ydbg

Ydbg: $(BUILD_DIR)/main.o $(BUILD_DIR)/cmd.o $(BUILD_DIR)/debugger.o $(BUILD_DIR)/breakpoints.o  | $(BUILD_DIR)
	$(CC) -static -o $@ $^

$(BUILD_DIR)/main.o: main.c | $(BUILD_DIR)
	$(CC) -c $< -o $@

$(BUILD_DIR)/cmd.o: $(SRC_DIR)/cmd.c $(SRC_DIR)/cmd.h | $(BUILD_DIR)
	$(CC) -c $< -o $@

$(BUILD_DIR)/debugger.o: $(SRC_DIR)/debugger.c $(SRC_DIR)/debugger.h | $(BUILD_DIR)
	$(CC) -c $< -o $@

$(BUILD_DIR)/breakpoints.o: $(SRC_DIR)/breakpoints.c $(SRC_DIR)/breakpoints.h | $(BUILD_DIR)
	$(CC) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
