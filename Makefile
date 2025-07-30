SUNDIALS_DIR ?= /usr/local
CC := gcc
CFLAGS := -O2 -I$(SUNDIALS_DIR)/include
LDFLAGS := -L$(SUNDIALS_DIR)/lib -Wl,-rpath,$(SUNDIALS_DIR)/lib
LDLIBS := -lsundials_cvode -lsundials_nvecserial -lm

SRC := cvSimple.c
TARGET := cvSimple

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(TARGET) *.o
