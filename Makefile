################################################################
# Makefile                                                     #
# softlic-collector                                            #
# überträgt Daten von einer Grünbeck SC23 an einen MQTT Server #
################################################################

# C source files
SOURCES = getcurldata.c printsoftliq.c kbhit.c sqlite.c mqtt.c config.c homeassistant.c main.c

# Optimization
OPT = -O3 -flto

# For Debugging
#OPT = -g

TARGET = sofliq-collector

#===================================================================================

GIT_VERSION := "$(shell git describe --long --always --tags)"

CC = gcc
CFLAGS = -std=gnu11 $(OPT) -c -Wall  -DGIT_VERSION=\"$(GIT_VERSION)\"

# uncomment if you want to store values to
# a sqlite3 database
CFLAGS += -D__SQLITE__

# uncomment if you want compile
# homassistant support
CFLAGS += -D__HOASTNT__

LDFLAGS = -LcJSON/build/ -Lpaho.mqtt.c/build/src/ $(OPT) -fuse-linker-plugin -lcurl -lsqlite3 -lpaho-mqtt3c -lcjson -lpthread -lm
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

REMOVE    = rm -f
REMOVEDIR = rm -rf
CREATEDIR = mkdir -p
GET_VERSION = git describe --tags --long
INST_DIR = /usr/local

# Object files directory
OBJDIR = obj

#===================================================================================

all: createdirs $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

createdirs:
	@$(CREATEDIR) $(OBJDIR)

clean:
	$(REMOVEDIR) $(OBJDIR)
	$(REMOVE) $(TARGET)
	
install:
	@$(CREATEDIR) /var/log/$(TARGET)
	@$(CREATEDIR) $(INST_DIR)/share/sofliq-collector
	cp ./$(TARGET) $(INST_DIR)/bin/$(TARGET)
	cp ./$(TARGET).json $(INST_DIR)/etc/$(TARGET).json
	cp ./$(TARGET).service  $(INST_DIR)/bin/$(TARGET).service


