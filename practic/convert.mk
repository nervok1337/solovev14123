CC = gcc
CFLAGS = -Iinclude/superdarn -Iinclude/base -Iinclude/general -I/usr/include/hdf5/serial -Wall -g
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lhdf5_serial \
          -Wl,--start-group -lm -Llib -lz -Llib -ldmap.1 -lrcnv.1 -lfit.1 -lrscan.1 -lcfit.1 -lcnvmap.1 \
          -lrtime.1 -lradar.1 -lraw.1 -laacgm.1 -lmlt.1 -lastalg.1 -lraw.1 -lfitacfex2.1 -lrmath.1 -llmfit.1 -lmpfit -lfitacf.1 \
          -Wl,--end-group

TARGET = fitacf2hdf
SRC = fitacf2hdf.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(TARGET).o
	$(CC) $(TARGET).o -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(TARGET).o

run:
	LD_LIBRARY_PATH=$(PWD)/lib ./fitacf2hdf 1.fitacf

