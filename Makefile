all: boat.bin sieve.bin

CFLAGS+=-Wall 

%.bin: %.o
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^ -lm -pthread

.PHONY: clean all
clean:
	rm -rf *.bin *.o