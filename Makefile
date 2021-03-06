CXX=g++
STD=c++0x
CXXFLAGS=-g -O0 -MP -MMD -std=$(STD)
TARGET=parseman
CPPFILES=parseman.cpp scanner.cpp token.cpp table_scanner.cpp explicit_scanner.cpp
BINDIR=bin
OFILES=$(CPPFILES:%.cpp=$(BINDIR)/%.o)
DEPDIR=$(BINDIR)
DEPFILES=$(OFILES:$(BINDIR)/%.o=$(DEPDIR)/%.d)

print-%: ; @echo $*=$($*)

all: $(TARGET)

$(TARGET): $(OFILES)
	$(CXX) -o $(TARGET) $(OFILES)

-include $(DEPFILES)
$(BINDIR)/%.o: %.cpp | $(BINDIR)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

$(BINDIR):
	mkdir -vp $(BINDIR)

.PHONY: clean
clean:
	$(RM) -rf $(TARGET) $(BINDIR)
