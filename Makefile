.DEFAULT_GOAL := all

CC         := g++
CXXFLAGS   := -Wall -Wshadow -Ofast

RM         := rm -f

SRCS       := $(wildcard src/*.cpp)
OBJS       := $(SRCS:.cpp=.o)
LD_LIBS    := pcap

LD_FLAGS   := $(addprefix -l, ${LD_LIBS})

TARGET     := stream

src/%.o: src/%.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

${TARGET}: ${OBJS}
	$(CC) -o $@ $^ ${LD_FLAGS}

.PHONY: all clean

all: ${TARGET}

clean:
	$(RM) ${OBJS} ${TARGET}
