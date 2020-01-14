CXX?=c++
LINK=$(CXX)
BIN_DIR=bin
SRC_DIR=src
OBJ_DIR=build
PO_DIR=po
MO_DIR=locale

prefix=/usr/local
INSTALL_BIN_DIR=$(DESTDIR)$(prefix)/bin
INSTALL_SHARE_DIR=$(DESTDIR)$(prefix)/share
INSTALL_MAN_DIR=$(INSTALL_SHARE_DIR)/man
INSTALL_MAN1_DIR=$(INSTALL_MAN_DIR)/man1
INSTALL_RU_MAN_DIR=$(INSTALL_MAN_DIR)/ru
INSTALL_RU_MAN1_DIR=$(INSTALL_RU_MAN_DIR)/man1
INSTALL_LOCALE_DIR=$(INSTALL_SHARE_DIR)/locale

INSTALL_BASH_COMPLETION_DIR=$(INSTALL_SHARE_DIR)/bash-completion/completions

OBJS = \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/defreader.o \
	$(OBJ_DIR)/entity.o \
	$(OBJ_DIR)/entreader.o \
	$(OBJ_DIR)/translate.o

INCLUDE=-I$(SRC_DIR)/rapidxml-1.13
PROGRAM=def2fgd
MAN1=def2fgd.1
RU_MAN1=def2fgd.ru.1
TARGET=$(BIN_DIR)/$(PROGRAM)

ifeq ($(USE_LOCALE),gettext)
LOCALE_FLAGS=-DUSE_GETTEXT
else
ifeq ($(USE_LOCALE),boost)
LOCALE_FLAGS=-DUSE_BOOST_LOCALE
LDFLAGS+=-lboost_locale -licuuc
endif
endif

ifeq ($(LOCALEDIR),)

else
LOCALE_FLAGS+=-DLOCALEDIR=\"$(LOCALEDIR)\"
endif

ifdef SystemRoot
	DEF2FGD_VERSION:=$(shell more version)
else
	DEF2FGD_VERSION:=$(shell cat version)
endif

SOURCES=src/main.cpp src/defreader.cpp src/entity.cpp src/entreader.cpp

all: make_obj_dir make_bin_dir $(TARGET)

make_obj_dir:
	-mkdir -p $(OBJ_DIR)

make_bin_dir:
	-mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(INCLUDE) -DDEF2FGD_VERSION=\"$(DEF2FGD_VERSION)\" -o $@ -c $< $(CXXFLAGS) $(USER_FLAGS) $(LOCALE_FLAGS)

$(TARGET): $(OBJS)
	$(LINK) $^ -o $@ $(LOCALE_FLAGS) $(LDFLAGS) $(CXXFLAGS) $(USER_FLAGS)

clean:
	rm -rf $(OBJ_DIR)

distclean: 
	rm -rf $(OBJ_DIR) $(BIN_DIR)

install-bin:
	install -d $(INSTALL_BIN_DIR)
	install $(TARGET) $(INSTALL_BIN_DIR)

uninstall-bin:
	rm $(INSTALL_BIN_DIR)/$(PROGRAM)

install-man:
	install -d $(INSTALL_MAN1_DIR) $(INSTALL_RU_MAN1_DIR)
	install -m644 $(MAN1) $(INSTALL_MAN1_DIR)
	install -m644 $(RU_MAN1) $(INSTALL_RU_MAN1_DIR)/$(MAN1)

uninstall-man:
	rm $(INSTALL_MAN1_DIR)/$(MAN1)
	rm $(INSTALL_RU_MAN1_DIR)/$(MAN1)

update-po: $(PO_DIR)/ru.po
	
$(PO_DIR)/def2fgd.pot: $(SOURCES)
	xgettext --keyword=translate:1 --keyword=translate_n:1,2 $^ -o $@

$(PO_DIR)/ru.po: $(PO_DIR)/def2fgd.pot
	msgmerge --update $@ $^

$(MO_DIR)/ru/LC_MESSAGES/def2fgd.mo: $(PO_DIR)/ru.po
	-mkdir -p $(MO_DIR)/ru/LC_MESSAGES
	msgfmt $^ -o $@

translations: $(MO_DIR)/ru/LC_MESSAGES/def2fgd.mo

clean-translations:
	rm -rf $(MO_DIR)

install-translations:
	install -d $(INSTALL_LOCALE_DIR)/ru/LC_MESSAGES
	install -m644 $(MO_DIR)/ru/LC_MESSAGES/def2fgd.mo $(INSTALL_LOCALE_DIR)/ru/LC_MESSAGES
	
uninstall-translations:
	rm $(INSTALL_LOCALE_DIR)/ru/LC_MESSAGES/def2fgd.mo
	-rmdir $(INSTALL_LOCALE_DIR)/ru/LC_MESSAGES

install-bash-completion:
	install -d $(INSTALL_BASH_COMPLETION_DIR)
	install -m644 bash_completion/def2fgd $(INSTALL_BASH_COMPLETION_DIR)

uninstall-bash-completion:
	rm $(INSTALL_BASH_COMPLETION_DIR)/def2fgd
	-rmdir $(INSTALL_BASH_COMPLETION_DIR)

install: install-bin install-man install-translations install-bash-completion

uninstall: uninstall-bin uninstall-man uninstall-translations uninstall-bash-completion
