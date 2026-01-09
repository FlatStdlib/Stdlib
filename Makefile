.PHONY: all clean reset dir move compile cloader tcc tcc_compile loader_bin test_run riscv-gcc count

BUILD_DIR := build
SRC := $(wildcard src/*.c src/stdlib/*.c src/libs/*.c)
HEADERS := $(wildcard headers/*.h)
LOADER_SRC := $(wildcard loader/*.c)
LINKER_SRC := linker/gcc_clibp.c

all: dir compile cloader move

dir:
	@mkdir -p $(BUILD_DIR)

OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))
LOADER_OBJECTS := $(patsubst loader/%.c,$(BUILD_DIR)/loader_%.o,$(LOADER_SRC))

$(BUILD_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	@gcc -c $< -o $@ -nostdlib -nostdinc -Iheaders

$(BUILD_DIR)/loader_%.o: loader/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@gcc -c $< -o $@ -nostdlib -ffunction-sections -Wl,--gc-sections -Iheaders

compile: $(BUILD_DIR)/libclibp.a $(BUILD_DIR)/clibp.o

$(BUILD_DIR)/libclibp.a: $(OBJECTS)
	@ar rcs $@ $^

$(BUILD_DIR)/clibp.o: $(OBJECTS)
	@ar rcs $@ $^

cloader: $(BUILD_DIR)/gcc_clibp

$(BUILD_DIR)/gcc_clibp: $(LINKER_SRC) $(BUILD_DIR)/clibp.o
	@mkdir -p $(BUILD_DIR)
	@gcc -c $(LINKER_SRC) -o $(BUILD_DIR)/gcc_clibp.o -nostdlib
	@ld -o $@ $(BUILD_DIR)/gcc_clibp.o $(BUILD_DIR)/clibp.o
	@echo "gcc_clibp built"
	@rm -f $(BUILD_DIR)/gcc_clibp.o

loader_bin: $(LOADER_OBJECTS) $(BUILD_DIR)/clibp.o
	@ld -o $(BUILD_DIR)/loader $(LOADER_OBJECTS) $(BUILD_DIR)/clibp.o
	@echo "loader built"

move: $(BUILD_DIR)/gcc_clibp
	@cp -r headers/* /usr/local/include
	@cp $(BUILD_DIR)/libclibp.a /usr/lib
	@cp $(BUILD_DIR)/gcc_clibp /bin
	@chmod +x /bin/gcc_clibp
	@echo "Installation done"

clean:
	@rm -rf $(BUILD_DIR)/*

reset:
	@rm -rf $(BUILD_DIR)

tcc: tcc_compile

tcc_compile:
	@mkdir -p $(BUILD_DIR)
	@tcc -ffreestanding -std=c99 -c $(SRC) -nostdlib
	@ar rcs $(BUILD_DIR)/libclibp.a *.o
	@ar rcs $(BUILD_DIR)/clibp.o *.o
	@rm -f *.o
	@tcc -ffreestanding -std=c99 -c $(LINKER_SRC) -o $(BUILD_DIR)/gcc_clibp.o -ffunction-sections -Wl
	@tcc -ffreestanding -std=c99 -c loader/loader.c -o $(BUILD_DIR)/loader.o -ffunction-sections -Wl
	@execstack -c $(BUILD_DIR)/gcc_clibp.o
	@execstack -c $(BUILD_DIR)/loader.o
	@ld -o $(BUILD_DIR)/gcc_clibp $(BUILD_DIR)/gcc_clibp.o $(BUILD_DIR)/clibp.o
	@rm -f $(BUILD_DIR)/gcc_clibp.o
	@cp -r headers/* /usr/local/include
	@cp $(BUILD_DIR)/libclibp.a /usr/lib
	@echo "TCC build complete"

test_run:
	@make && ./build/gcc_clibp tests/heap.c -o t && ./t
	@c99 tests/heap.c -o t -lclibp && ./t

riscv-gcc:
	@mkdir -p $(BUILD_DIR)
	@clang --target=riscv64-unknown-elf -c $(SRC) -nostdlib -nostdinc
	@ar rcs $(BUILD_DIR)/riscv_libclibp.a *.o
	@ar rcs $(BUILD_DIR)/riscv_clibp.o *.o
	@rm -f *.o
	@clang --target=riscv64-unknown-elf -c $(LINKER_SRC) -o $(BUILD_DIR)/riscv_gcc_clibp.o -nostdlib
	@riscv64-unknown-elf-ld -o $(BUILD_DIR)/riscv_gcc_clibp $(BUILD_DIR)/riscv_gcc_clibp.o $(BUILD_DIR)/*.o
	@rm -f *.o

count:
	@wc -l tests/*.c src/*.c src/stdlib/*.c headers/*.h
