client:
	$(MAKE) -C client

server:
	$(MAKE) -C server

.PHONY: all
all: client server
