CXX=g++
LINK=$(CXX)
BUILD_DIR=build
BIN_DIR=$(BUILD_DIR)/bin
SRC_DIR=src
OBJ_DIR=$(BUILD_DIR)/obj

INSTALL_PATH=/usr/local/bin

OBJS = \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/defreader.o \
	$(OBJ_DIR)/entity.o \
	$(OBJ_DIR)/entreader.o

INCLUDE=-I$(SRC_DIR)/rapidxml-1.13
PROGRAM=def2fgd
TARGET=$(BIN_DIR)/$(PROGRAM)

all: make_obj_dir make_bin_dir $(TARGET)

make_obj_dir:
	-mkdir -p $(OBJ_DIR)

make_bin_dir:
	-mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(INCLUDE) $(CFLAGS) $(USER_FLAGS) -o $@ -c $<

$(TARGET): $(OBJS)
	$(LINK) $(CFLAGS) $(USER_FLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJ_DIR)

cleanall: 
	rm -rf $(BUILD_DIR)

install:
	cp $(TARGET) $(INSTALL_PATH)

uninstall:
	rm $(INSTALL_PATH)/$(PROGRAM)