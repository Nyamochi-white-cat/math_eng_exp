CXX      = clang++
CXXFLAGS = -std=c++14 -O2 -Wall -I/opt/homebrew/include/eigen3

SRCS  = $(wildcard assignment*.cpp)
PROGS = $(SRCS:.cpp=)
CACHE = .cache

all: $(PROGS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run: $(PROGS:%=$(CACHE)/%.done)

$(CACHE)/%.done: %
	./$<
	@mkdir -p $(CACHE)
	touch $@

run-%: %
	./$<

clean:
	rm -f $(PROGS) data/assignment*_*.dat
	rm -rf $(CACHE)

clean-%:
	rm -f $* data/$*_*.dat $(CACHE)/$*.done

.PHONY: all run clean
