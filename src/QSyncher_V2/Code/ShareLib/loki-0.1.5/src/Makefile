SRC = $(wildcard *.cpp)
STATIC_OBJ = $(SRC:.cpp=.o)
SHARED_OBJ = $(SRC:.cpp=.lo)

CPPFLAGS = -I../include -DNDEBUG

STATIC_LIB = libloki.a

ifeq ($(OS), Darwin)
  SHARED_LIB_BASE = libloki.dylib
  SHARED_LIB_VERSIONED = libloki.$(VERSION).dylib
  LDFLAGS = -dynamiclib -single_module -install_name $(SHARED_LIB_VERSIONED) -fPIC
  LDLIBS = -lpthread
  RESULT_DIR = ../lib/
else
ifeq ($(OS), Linux)
  SHARED_LIB_BASE = libloki.so
  SHARED_LIB_VERSIONED = libloki.so.$(VERSION)
  LDFLAGS = --shared -Wl,-soname=$(SHARED_LIB_VERSIONED) -fPIC
  LDLIBS = -lpthread
  RESULT_DIR = ../lib/
else
  SHARED_LIB_BASE = libloki.dll
  LDFLAGS = --shared
  LDLIBS =
  RESULT_DIR = ../lib/
endif
endif

.PHONY: all build-static build-shared clean distclean
all: build-static build-shared
build-static: $(RESULT_DIR)$(STATIC_LIB)
build-shared: $(RESULT_DIR)$(SHARED_LIB_BASE)

clean:
	$(RM) $(STATIC_OBJ)
ifneq ($(OS), Windows)
	$(RM) $(SHARED_OBJ)
endif

distclean: clean
	$(RM) $(RESULT_DIR)$(STATIC_LIB)
	$(RM) $(RESULT_DIR)$(SHARED_LIB_BASE)
ifneq ($(OS),Windows)
	$(RM) $(RESULT_DIR)$(SHARED_LIB_VERSIONED)

INSTALL = install
INSTALL_DATA = $(INSTALL) -m 644

.PHONY: install install-static install-shared
install: install-static install-shared

install-static: $(RESULT_DIR)$(STATIC_LIB)
	mkdir -p $(prefix)/lib
	$(INSTALL_DATA) $(RESULT_DIR)$(STATIC_LIB) $(prefix)/lib

install-shared: $(RESULT_DIR)$(SHARED_LIB_BASE)
	mkdir -p $(prefix)/lib
	$(INSTALL_DATA) $(RESULT_DIR)$(SHARED_LIB_BASE) $(prefix)/lib
	$(INSTALL_DATA) $(RESULT_DIR)$(SHARED_LIB_VERSIONED) $(prefix)/lib

$(RESULT_DIR)$(SHARED_LIB_BASE): $(RESULT_DIR)$(SHARED_LIB_VERSIONED)
	ln -sf $^ $@

%.lo : %.cpp
	$(CXX) -c $(CXXFLAGS) -fPIC $(CPPFLAGS) -o $@ $<

$(RESULT_DIR)$(SHARED_LIB_VERSIONED): $(SHARED_OBJ)
else
$(RESULT_DIR)$(SHARED_LIB_BASE): $(STATIC_OBJ)
endif
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(RESULT_DIR)$(STATIC_LIB): $(STATIC_OBJ)
	$(AR) $(ARFLAGS) $@ $^
