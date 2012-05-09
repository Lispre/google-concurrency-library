# Copyright 2011 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

######## General Configuration

#### Source Locations

VPATH = ../src ../testing \
	../third_party/googletest/src ../third_party/googlemock/src

#### Compilation Flags

DFlags = -MMD -MP
EFlags = -Wall -Werror
IFlags = -I. -I../include
BFlags = $(DFlags) $(EFlags) $(IFlags) $(BFLAGS)
CFlags = $(BFlags) $(CFLAGS)
CxxFlags = $(BFlags) $(CXXFLAGS)
LFlags = -pthread $(CXXFLAGS)

#### Compilation Rules

%.o: %.c
	$(CC) $(CFlags) $< -c -o $*.o 

%.o: %.cc
	$(CXX) $(CxxFlags) $< -c -o $*.o 

%.a :
	/bin/rm -f $@
	/usr/bin/ar -rc $@ $^

%.exe :
	$(CXX) $^ $(LFlags) -o $*.exe

-include *.d

######## Source File Generation

generate: atomic.h atomic_c_test.c

# The atomic header has a lot of redundancy, so it is generated.
atomic.h : ../util/atomic.sh
	/bin/bash ../util/atomic.sh > atomic.h

# The atomic test is the same for both C++ and C, so copy to the latter.
atomic_c_test.c : ../testing/atomic_cpp_test.cc
	cp ../testing/atomic_cpp_test.cc atomic_c_test.c

######## Library Build

build : stdfake.a lower.a

stdfake.a : atomic.o mutex.o condition_variable.o thread.o

lower.a : countdown_latch.o serial_executor.o barrier.o \
	mutable_thread.o simple_thread_pool.o

######## Testing

test : cxx0x_test.o atomic_c_test.run atomic_cpp_test.run counter_test.run \
	stdfake_test.run lower_test.run mutex_test.run higher_test.run \
	pipeline_test.run

#### Simple Tests

atomic_c_test.exe : atomic_c_test.o stdfake.a
atomic_c_test.run : atomic_c_test.exe
	./atomic_c_test.exe

atomic_cpp_test.exe : atomic_cpp_test.o stdfake.a
atomic_cpp_test.run : atomic_cpp_test.exe
	./atomic_cpp_test.exe

counter_test.exe : counter_test.o stdfake.a
counter_test.run : counter_test.exe
	./counter_test.exe

#### GTest GMock Builds

GTEST_INC := -I../third_party/googletest/include
GTEST_SRC := -I../third_party/googletest
GTEST_OBJ := gtest-all.o gtest_main.o
$(GTEST_OBJ) : CxxFlags += $(GTEST_INC) $(GTEST_SRC)

GMOCK_INC := -I../third_party/googlemock/include
GMOCK_SRC := -I../third_party/googlemock
GMOCK_OBJ := gtest-all.o gmock-all.o gmock_main.o
$(GMOCK_OBJ) : CxxFlags += $(GTEST_INC) $(GTEST_SRC) $(GMOCK_INC) $(GMOCK_SRC)

#### Tests for the Fake Standard Library

STDFAKE_TESTS := lock_test.o thread_test.o
$(STDFAKE_TESTS) : CxxFlags += $(GTEST_INC) $(GMOCK_INC)
stdfake_test.exe : $(STDFAKE_TESTS) $(GMOCK_OBJ) stdfake.a
stdfake_test.run : stdfake_test.exe
	./stdfake_test.exe

#### Test for the Google Concurrency Library Proper

LOWER_TESTS := barrier_test.o serial_executor_test.o mutable_thread_test.o \
	simple_thread_pool_test.o
$(LOWER_TESTS) : CxxFlags += $(GTEST_INC) $(GMOCK_INC)
lower_test.exe : $(LOWER_TESTS) $(GMOCK_OBJ) lower.a stdfake.a
lower_test.run : lower_test.exe
	./lower_test.exe

MUTEX_TESTS := test_mutex_test.o condition_variable_test.o lock_test.o \
	race_test.o concurrent_priority_queue_test.o blockable_thread.o \
	blocking_queue_closed_test.o test_mutex.o
$(MUTEX_TESTS) : CxxFlags += $(GTEST_INC) $(GMOCK_INC)
mutex_test.exe : $(MUTEX_TESTS) $(GMOCK_OBJ) lower.a stdfake.a
mutex_test.run : mutex_test.exe
	./mutex_test.exe

HIGHER_TESTS := blocking_queue_test.o source_test.o iterator_queue_test.o \
	stream_mutex_test.o buffer_queue_test.o
$(HIGHER_TESTS) : CxxFlags += $(GTEST_INC) $(GMOCK_INC)
higher_test.exe : $(HIGHER_TESTS) $(GMOCK_OBJ) lower.a stdfake.a
higher_test.run : higher_test.exe
	./higher_test.exe

PIPELINE_TESTS := pipeline_test.o
$(PIPELINE_TESTS) : CxxFlags += $(GTEST_INC) $(GMOCK_INC)
pipeline_test.exe : $(PIPELINE_TESTS) $(GMOCK_OBJ) lower.a stdfake.a
pipeline_test.run : pipeline_test.exe
	./pipeline_test.exe