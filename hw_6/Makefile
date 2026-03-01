# HW6: Stopwatch, ELMA processes (RandomProcess, Filter, Integrator)
# Requires: ELMA installed (e.g. /usr/local/include/elma, /usr/local/lib/libelma.a)
#           Google Test, pthread
# Optional: ELMA_DIR to override include/lib path

CC          := g++ -std=c++17
ifneq ($(wildcard ./elma/include/elma.h),)
ELMA_INC    := -I./elma/include
ELMA_LIB    := -L./elma/lib -lelma
ELMA_DEP    := ./elma/lib/libelma.a
else
ELMA_DIR    ?= /usr/local
ELMA_INC    := -I$(ELMA_DIR)/include
ELMA_LIB    := -L$(ELMA_DIR)/lib -lelma
ELMA_DEP    :=
endif

SRCDIR      := .
INCDIR      := .
BUILDDIR    := ./build
TARGETDIR   := ./bin
CFLAGS      := -g
LDFLAGS     := -L/opt/homebrew/lib
LIB         := -lgtest -lpthread
INC         := -I$(INCDIR) -I/opt/homebrew/include $(ELMA_INC)

# Object files from Part 1 & 2 (exclude main and unit_tests)
PART_SRCS   := stopwatch.cc random_process.cc filter.cc integrator.cc
PART_OBJS   := $(patsubst %.cc,$(BUILDDIR)/%.o,$(PART_SRCS))

# Two targets: main (demo) and test (unit tests)
all: directories $(ELMA_DEP) $(TARGETDIR)/main $(TARGETDIR)/test

./elma/lib/libelma.a:
	cd elma && $(MAKE) lib/libelma.a

directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

# Main demo: main.cc + Part objects + elma (no gtest)
$(TARGETDIR)/main: $(BUILDDIR)/main.o $(PART_OBJS) $(ELMA_DEP)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(BUILDDIR)/main.o $(PART_OBJS) $(ELMA_LIB)

# Unit tests: unit_tests.cc + Part objects + elma + gtest
$(TARGETDIR)/test: $(BUILDDIR)/unit_tests.o $(PART_OBJS) $(ELMA_DEP)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(BUILDDIR)/unit_tests.o $(PART_OBJS) $(ELMA_LIB) $(LIB)

$(BUILDDIR)/main.o: main.cc stopwatch.h random_process.h filter.h integrator.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ main.cc

$(BUILDDIR)/unit_tests.o: unit_tests.cc stopwatch.h random_process.h filter.h integrator.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ unit_tests.cc

$(BUILDDIR)/stopwatch.o: stopwatch.cc stopwatch.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ stopwatch.cc

$(BUILDDIR)/random_process.o: random_process.cc random_process.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ random_process.cc

$(BUILDDIR)/filter.o: filter.cc filter.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ filter.cc

$(BUILDDIR)/integrator.o: integrator.cc integrator.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ integrator.cc

clean:
	@$(RM) -rf $(BUILDDIR)/*.o

spotless: clean
	@$(RM) -rf $(TARGETDIR)/main $(TARGETDIR)/test $(BUILDDIR) $(TARGETDIR)

.PHONY: directories all clean spotless
