.DEFAULT_GOAL = all

.PHONY: all client server
all: client server

client:
	$(MAKE) -C client

server:
	$(MAKE) -C server

