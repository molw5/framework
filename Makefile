CXX           = clang++
CXX_COMPILE   = $(CXX) -c $(CXX_CFLAGS)
CXX_LINK	  = $(CXX) $(CXX_LDFLAGS)
CXX_CFLAGS	  = -std=c++11 -pedantic -Wall -Wextra -Werror -Wno-trigraphs -I./
CXX_LDFLAGS   =

UNAME := $(shell uname -o)

SOURCES = tests/test.cpp \
          tests/serializable/base_types.cpp \
          tests/serializable/containers/value.cpp \
          tests/serializable/containers/optional_field.cpp \
          tests/serializable/containers/bit_field.cpp \
          tests/serializable/mutators/endian.cpp \
          tests/serializable/mutators/fixed_container.cpp \
          tests/serializable/mutators/variable_container.cpp \
          tests/serializable/mutators/terminated_container.cpp \
          tests/serializable/mutators/tuple_container.cpp \
          tests/serializable/mutators/stl.cpp \
          tests/serializable/interfaces/comparable.cpp \
          tests/serializable/streams/modular_sum.cpp \
          tests/serializable/streams/bit_stream.cpp \
          tests/common/variadic_switch_fallthrough.cpp \
          tests/common/variadic_switch_return.cpp \
          tests/protocol_buffers/varint.cpp \
          tests/protocol_buffers/wire_type.cpp \
          tests/protocol_buffers/protocol_buffers.cpp

EXAMPLES = examples/serializable/a_simple_example.cpp \
		   examples/serializable/a_portable_example.cpp \
           examples/serializable/custom_implementation.cpp \
           examples/serializable/custom_structures.cpp \
           examples/serializable/custom_serialization.cpp \
           examples/serializable/adding_mutator_types.cpp \
           examples/serializable/adding_container_types.cpp \
           examples/serializable/custom_serializable_implementation.cpp \
           examples/serializable/non_intrusive_wrappers_1.cpp \
           examples/serializable/non_intrusive_wrappers_2.cpp \
           examples/protocol_buffers/a_simple_example.cpp \
		   examples/protocol_buffers/nested_types.cpp

PERFORMANCE = performance/access_overhead.cpp \
			  performance/comparison_overhead.cpp \
			  performance/copy_overhead.cpp \
			  performance/serialization_overhead.cpp

BENCHMARKS = benchmarks/protocol_buffers.cpp

EXECUTABLE = test
EXECUTABLE_EXAMPLES = $(EXAMPLES:.cpp=)
EXECUTABLE_PERFORMANCE = $(PERFORMANCE:.cpp=)
EXECUTABLE_BENCHMARKS = $(BENCHMARKS:.cpp=)
OBJECTS = $(SOURCES:.cpp=.o)

all: normal

normal: CXX_CFLAGS += -O2
normal: CXX_LDFLAGS += -lUnitTest++
normal: $(SOURCES) $(EXECUTABLE)

examples: CXX_CFLAGS += -O2 -Wno-reorder
examples: CXX_LDFLAGS += 
examples: $(EXECUTABLE_EXAMPLES)

performance: CXX_CFLAGS += -O2
performance: CXX_LDFLAGS +=
performance: $(EXECUTABLE_PERFORMANCE)

benchmarks: CXX_CFLAGS += -O3 -DNDEBUG
benchmarks: CXX_LDFLAGS +=
benchmarks: $(EXECUTABLE_BENCHMARKS)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXX_LDFLAGS) -o $@
	@echo "Running unit tests..."
	@./$@

examples/%: examples/%.cpp
	$(CXX) $(CXX_CFLAGS) $< $(CXX_LDFLAGS) -o $(<:.cpp=)

performance/%: performance/%.cpp
	$(CXX) $(CXX_CFLAGS) $< $(CXX_LDFLAGS) -o $(<:.cpp=)

benchmarks/%: benchmarks/%.cpp
	$(CXX) $(CXX_CFLAGS) $< $(CXX_LDFLAGS) -o $(<:.cpp=)

.cpp.o:
	$(CXX) $(CXX_CFLAGS) -c $< -o $@

clean: 
	rm -f src/test.o $(EXECUTABLE) $(EXECUTABLE_EXAMPLES) $(EXECUTABLE_PERFORMANCE) $(EXECUTABLE_BENCHMARKS) $(OBJECTS)
