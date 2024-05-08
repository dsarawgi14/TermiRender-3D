CC=g++
INCDIRS=./../include
CLIENTDIR=../src/client
CODEDIRS=../src/rayMarching ../src/vec
DEPFLAGS=-MP -MD
CFLAGS=$(foreach dir, $(INCDIRS), -I$(dir)) $(DEPFLAGS) -lpthread

CLIENT_CFILES=$(wildcard ../src/client/*.cpp)
CLIENT_OBJECTS=$(CLIENT_CFILES:.cpp=.o)
CLIENT_DEPFILES=$(CLIENT_CFILES:.cpp=.d)

CFILES=$(foreach file, $(CODEDIRS), $(wildcard $(file)/*.cpp))
OBJECTS=$(CFILES:.cpp=.o)
DEPFILES=$(CFILES:.cpp=.d)

BINARY=bin

all: $(BINARY)

$(BINARY): $(CLIENT_OBJECTS) $(OBJECTS)
	$(info Making Binary in build folder - bin)
	@$(CC) -o $@ $^ -lpthread
	$(info Binary created)

CLIENT: $(CLIENT_OBJECTS) $(OBJECTS)

%.o: %.cpp
	$(info Compiling $<)
	@$(CC) -c $(CFLAGS) -o $@ $^

clean:
	$(info Cleaning up)
	$(info Removing $(OBJECTS) $(CLIENT_OBJECTS) $(BINARY))
	@rm -rf $(OBJECTS) $(CLIENT_OBJECTS) $(BINARY)