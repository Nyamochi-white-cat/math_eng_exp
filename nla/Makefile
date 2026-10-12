CXX = clang++
EIGEN3_INCLUDE ?= /opt/homebrew/include/eigen3
CXXFLAGS = -std=c++14 -O2 -Wall -I$(EIGEN3_INCLUDE)

SRCS = assignment1.cpp assignment2.cpp
BUILD = build
PROGS = $(SRCS:%.cpp=$(BUILD)/%)
CACHE = $(BUILD)/.cache
RUN_TARGETS = $(SRCS:%.cpp=run-%)

.PHONY: all run clean $(RUN_TARGETS)

all: $(PROGS)

$(BUILD)/%: %.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD) $(CACHE):
	mkdir -p $@

run: $(SRCS:%.cpp=$(CACHE)/%.done)

$(CACHE)/%.done: $(BUILD)/% | $(CACHE)
	@mkdir -p data
	./$<
	touch $@

$(RUN_TARGETS): run-%: $(BUILD)/%
	@mkdir -p data
	./$<

clean:
	rm -f data/assignment*_*.dat
	rm -rf $(BUILD)
