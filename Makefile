.PHONY: all rbuf_test list base clean

all: rbuf_test list base

base: base.c
	@gcc -o $@ $^

list: list.c
	@gcc -o $@ $^

rbuf_test: rbuf_test.c ring_buffer.c
	@gcc -o $@ $^ -lpthread

clean:
	@rm -rf rbuf_test list base
