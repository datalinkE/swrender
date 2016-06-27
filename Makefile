SYSCONF_LINK = g++
CPPFLAGS     = -pg -std=c++11 -Wall -Wextra -Werror
LDFLAGS      = -pg
LIBS         = -L/usr/lib -lm -lprofiler

DESTDIR = ./
TARGET  = swrender

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(DESTDIR)$(TARGET)
	@echo "running"
	time $(DESTDIR)$(TARGET)
	@echo "done"

run: $(DESTDIR)$(TARGET)
	$(DESTDIR)$(TARGET) & open output.tga

deps:
	apt-get install google-perftools | brew install google-perftools

$(DESTDIR)$(TARGET): $(OBJECTS)
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $(DESTDIR)$(TARGET) $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
	-rm -f *.tga

