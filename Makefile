CXX?=c++
LINK=$(CXX)
BIN_DIR=bin
SRC_DIR=src
OBJ_DIR=build

prefix=/usr/local
INSTALL_BIN_DIR=$(DESTDIR)$(prefix)/bin
INSTALL_MAN1_DIR=$(DESTDIR)$(prefix)/share/man/man1

OBJS = \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/defreader.o \
	$(OBJ_DIR)/entity.o \
	$(OBJ_DIR)/entreader.o

INCLUDE=-I$(SRC_DIR)/rapidxml-1.13
PROGRAM=def2fgd
MAN1=def2fgd.1
TARGET=$(BIN_DIR)/$(PROGRAM)

DEF2FGD_VERSION:=$(shell cat version)

all: make_obj_dir make_bin_dir $(TARGET)

make_obj_dir:
	-mkdir -p $(OBJ_DIR)

make_bin_dir:
	-mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(INCLUDE) $(CXXFLAGS) $(USER_FLAGS) -DDEF2FGD_VERSION=\"$(DEF2FGD_VERSION)\" -o $@ -c $<

$(TARGET): $(OBJS)
	$(LINK) $(LDFLAGS) $(CXXFLAGS) $(USER_FLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJ_DIR)

distclean: 
	rm -rf $(OBJ_DIR) $(BIN_DIR)

install:
	install -d $(INSTALL_BIN_DIR) $(INSTALL_MAN1_DIR)
	install $(TARGET) $(INSTALL_BIN_DIR)
	install -m644 $(MAN1) $(INSTALL_MAN1_DIR)

uninstall:
	rm $(INSTALL_BIN_DIR)/$(PROGRAM)
	rm $(INSTALL_MAN1_DIR)/$(MAN1)
