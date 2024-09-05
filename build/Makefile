CC=g++
INCDIRS=./../include
CLIENTDIR=../client
CODEDIRS=../src/rayMarching ../src/vec
DEPFLAGS=-MP -MD
CFLAGS=$(foreach dir, $(INCDIRS), -I$(dir)) $(DEPFLAGS) -lpthread

CLIENT_CFILES=$(wildcard $(CLIENTDIR)/*.cpp)
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
	@$(CC) -c $(CFLAGS) -o $@ $<

clean:
	$(info Cleaning up)
	$(info Removing $(OBJECTS) $(DEPFILES) $(CLIENT_OBJECTS) $(CLIENT_DEPFILES) $(BINARY))
	@rm -rf $(OBJECTS) $(DEPFILES) $(CLIENT_OBJECTS) $(CLIENT_DEPFILES) $(BINARY)

-include $(DEPFILES) $(CLIENT_DEPFILES)