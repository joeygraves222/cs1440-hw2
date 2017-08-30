CXXFLAGS = -std=c++11 -Wall -Wextra

CPP_SRCS := $(wildcard *.cpp)
OBJS     := ${CPP_SRCS:.cpp=.o}

main: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean
clean:
	-rm -f *.o *.d main

%.d: $(CPP_SRCS)
	$(CXX) -MM $*.cpp | sed -e 's@^\(.*\)\.o:@\1.d \1.o:@' > $@

ifneq "$(MAKECMDGOALS)" "clean"
-include ${CPP_SRCS:.cpp=.d}
endif
