CXX=g++
CXXFLAGS=-std=c++17 -I SpacecraftSim
LDFLAGS=-lsqlite3
SOURCES=$(wildcard SpacecraftSim/*.cpp)
TARGET=spacecraft_sim

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LDFLAGS) -o $@

clean:
	rm -f $(TARGET)
	rm -f SpacecraftSim/spacecraft_simulation.db
