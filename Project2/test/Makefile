#Adapted from the 'cmocka-embedded-example' repo of github user samvrlewis
vpath %.c ../src

INCLUDE_DIRS = -I../src
INCLUDE_DIRS += -I../inc
INCLUDE_DIRS += -I$(CMOCKA_DIR)/include

CFLAGS	= $(INCLUDE_DIRS)

MOCKS = ucRead_rom_byte
LDFLAGS	= -L$(CMOCKA_DIR)/build/src -Wl,-rpath=$(CMOCKA_DIR)/build/src 
LDFLAGS += $(foreach MOCK,$(MOCKS),-Wl,--wrap=$(MOCK))
LDFLAGS += -lcmocka

TARGET = test_cartridge

SRC = cartridge.c
TEST_SRC = mock_cart.c test_cartridge.c

SRC_OBJ = $(patsubst %.c,obj/%.o,$(SRC)) 
TEST_OBJ = $(patsubst %.c,obj/%.o,$(TEST_SRC)) 
OBJ_DIR = obj

CMOCKA_TAR = cmocka-1.1.1.tar.xz
CMOCKA_DIR = cmocka

all: $(CMOCKA_DIR) $(TARGET)
	./$(TARGET)

$(SRC_OBJ): | $(OBJ_DIR)

$(OBJ_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o : %.c
	@echo $< 
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET):  $(SRC_OBJ) $(TEST_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(CMOCKA_DIR):
	mkdir -p $@
	tar -xf $(CMOCKA_TAR) -C $@ --strip-components 1
	mkdir -p $@/build
	@(cd $@/build && cmake ../ && make -s)

clean:
	rm -rf $(TARGET) $(OBJ_DIR) $(CMOCKA_DIR)