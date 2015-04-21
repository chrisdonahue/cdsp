CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

###########################################

vpath %.c src

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -Isrc
#CFLAGS += -DUSE_STDPERIPH_DRIVER

SRCS = dsp.cpp exception.cpp helper.cpp parameter.cpp sample_buffer.cpp \
	   primitive_base.cpp schedule_linear.cpp impulse.cpp noise.cpp add.cpp multiply.cpp table_abstract.cpp table_oscillator.cpp

OBJS = $(SRCS:.c=.o)

.PHONY: libcdsp.a

all: libcdsp.a

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libstm32f4.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libstm32f4.a
