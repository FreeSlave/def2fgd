CXX?=c++
LINK=$(CXX)
BIN_DIR=bin
SRC_DIR=src
OBJ_DIR=build
PO_DIR=po
MO_DIR=locale

prefix=/usr/local
INSTALL_BIN_DIR=$(DESTDIR)$(prefix)/bin
INSTALL_MAN1_DIR=$(DESTDIR)$(prefix)/share/man/man1

OBJS = \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/defreader.o \
	$(OBJ_DIR)/entity.o \
	$(OBJ_DIR)/entreader.o \
	$(OBJ_DIR)/translate.o

INCLUDE=-I$(SRC_DIR)/rapidxml-1.13
PROGRAM=def2fgd
MAN1=def2fgd.1
TARGET=$(BIN_DIR)/$(PROGRAM)

ifeq ($(USE_LOCALE),gettext)
LOCALE_FLAGS=-DUSE_GETTEXT
else
ifeq ($(USE_LOCALE),boost)
LOCALE_FLAGS=-DUSE_BOOST_LOCALE -lboost_locale -licuuc
endif
endif

ifeq ($(LOCALEDIR),)

else
LOCALEDIR=-DLOCALEDIR=$(LOCALEDIR)
endif

DEF2FGD_VERSION:=$(shell cat version)
SOURCES=src/main.cpp src/defreader.cpp src/entity.cpp src/entreader.cpp

all: make_obj_dir make_bin_dir $(TARGET)

make_obj_dir:
	-mkdir -p $(OBJ_DIR)

make_bin_dir:
	-mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(INCLUDE) -DDEF2FGD_VERSION=\"$(DEF2FGD_VERSION)\" -o $@ -c $< $(CXXFLAGS) $(USER_FLAGS) $(LOCALE_FLAGS) $(LOCALEDIR)

$(TARGET): $(OBJS)
	$(LINK) $^ -o $@ $(LOCALE_FLAGS) $(LDFLAGS) $(CXXFLAGS) $(USER_FLAGS)

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

update-po: $(PO_DIR)/ru.po
	
$(PO_DIR)/def2fgd.pot: $(SOURCES)
	xgettext --keyword=translate:1 --keyword=translate_n:1,2 $^ -o $@

$(PO_DIR)/ru.po: $(PO_DIR)/def2fgd.pot
	msgmerge --update $@ $^

$(MO_DIR)/ru/LC_MESSAGES/def2fgd.mo: $(PO_DIR)/ru.po
	-mkdir -p $(MO_DIR)/ru/LC_MESSAGES
	msgfmt $^ -o $@

translations: $(MO_DIR)/ru/LC_MESSAGES/def2fgd.mo

