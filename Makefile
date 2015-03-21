SYSCONF_LINK = g++
CPPFLAGS     = -pg
LDFLAGS      = -pg
LIBS         = -L/usr/lib -lm -lprofiler

DESTDIR = ./
TARGET  = swrender

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(DESTDIR)$(TARGET)
	@echo "running"
	time $(DESTDIR)$(TARGET)
	@echo "done"

$(DESTDIR)$(TARGET): $(OBJECTS)
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $(DESTDIR)$(TARGET) $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
	-rm -f *.tga

