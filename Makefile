CXX=g++
CXXFLAGS=-g -O2 -MP -MMD -std=c++0x
TARGET=parseman
CPPFILES=parseman.cpp scanner.cpp token.cpp
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
