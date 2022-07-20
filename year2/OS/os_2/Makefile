#
# pagetables -- A framework to experiment with memory management
#
# Copyright (C) 2017-2020  Leiden University, The Netherlands.
#

CXX = c++
CXXFLAGS = -g -Wall -Weffc++ -std=c++17 -O3 -Iinclude
LDFLAGS = -lz

BOOST_CXXFLAGS = -DBOOST_TEST_DYN_LINK
BOOST_LIBS = -lboost_unit_test_framework


HEADERS = \
	include/settings.h	\
	include/exceptions.h	\
	include/process.h	\
	include/mmu.h		\
	include/oskernel.h	\
	include/processor.h

HW_OBJS = \
	hw/mmu.o		\
	hw/processor.o

OS_HEADERS = \
	os/tracereader.h	\
	os/linereader.h		\
	os/physmemmanager.h

OS_OBJS = \
	os/tracereader.o	\
	os/linereader.o		\
	os/physmemmanager.o	\
	os/process.o		\
	os/oskernel.o

ARCH_HEADERS = \
	arch/include/simple.h  \
	arch/include/aarch64.h

ARCH_OBJS = \
	arch/simple/driver.o	\
	arch/simple/mmu.o       \
	arch/aarch64/driver.o	\
	arch/aarch64/mmu.o      


OBJS = \
	main.o			\
	settings.o		\
	$(HW_OBJS)

# unit tests

TESTS =	\
	tests/physmemmanager	\
	tests/simple            \
	tests/aarch64

TEST_OBJS =	\
	$(HW_OBJS)	\
	$(OS_OBJS)	\
	$(ARCH_OBJS)	\
	settings.o


all:		pagetables tests

tests:		$(TESTS)

pagetables:	$(OS_OBJS) $(OBJS) $(ARCH_OBJS)
		$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

main.o:		main.cc $(HEADERS) $(ARCH_HEADERS)
		$(CXX) $(CXXFLAGS) -Iarch/include -o $@ -c $<

%.o:		%.cc $(HEADERS)
		$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OS_OBJS):	%.o:		%.cc $(HEADERS) $(OS_HEADERS)
				$(CXX) $(CXXFLAGS) -o $@ -c $<

$(ARCH_OBJS):	%.o:		%.cc $(HEADERS) $(ARCH_HEADERS)
				$(CXX) $(CXXFLAGS) -Iarch/include -o $@ -c $<

#
# unit tests
#

tests/%:	tests/%.cc $(HEADERS) $(OS_HEADERS) $(ARCH_HEADERS) $(TEST_OBJS)
		$(CXX) $(CXXFLAGS) -I. $(BOOST_CXXFLAGS) -o $@ $< $(TEST_OBJS) $(BOOST_LIBS) $(LDFLAGS)

check:		$(TESTS)
		@for i in $(TESTS); do ./$$i; done


#
# clean
#

clean:
		rm -f pagetables $(TESTS)
		rm -f $(OBJS) $(OS_OBJS) $(ARCH_OBJS)
