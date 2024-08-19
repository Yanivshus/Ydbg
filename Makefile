CC=gcc
BUILD_DIR=build
SRC_DIR=src

.PHONY: all app clean

app: Ydbg

Ydbg: $(BUILD_DIR)/main.o $(BUILD_DIR)/cmd.o  | $(BUILD_DIR)
	$(CC) -o $@ $^

$(BUILD_DIR)/main.o: main.c | $(BUILD_DIR)
	$(CC) -c $< -o $@

$(BUILD_DIR)/cmd.o: $(SRC_DIR)/cmd.c $(SRC_DIR)/cmd.h | $(BUILD_DIR)
	$(CC) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
