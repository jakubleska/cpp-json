CPPFLAGS=-std=c++11 -g -Wall -pedantic -Werror
LDLIBS=

SRCS=../json_value_t.cc main.cc
OBJS=$(subst .cc,.o,$(SRCS))


all: $(OBJS)
	g++ $(LDFLAGS) -o unittest_json  $(OBJS) $(LDLIBS) -lgtest -lpthread
	./unittest_json

json.o: json_value_t.cc json_value_t.h json_value_t.i
	g++ $(CPPFLAGS) -c ../json_value_t.cc

main.o: ../json_value_t.i ../json_value_t.h main.cc
	g++ $(CPPFLAGS) -c main.cc

clean:
	rm *.o
