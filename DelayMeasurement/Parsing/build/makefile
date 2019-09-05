# VARIABLES
SHELL=/bin/sh
MF_PATH=.

CXX=g++
OPT=-Wall -Wfatal-errors -std=c++11

INC_DIR=$(MF_PATH)/../include
PART_DIR=./../part

# List of all object files
DRIVER_OBJs=$(PART_DIR)/parser-utility.o $(PART_DIR)/parser-message.o $(PART_DIR)/parser-request.o $(PART_DIR)/parser-response.o $(PART_DIR)/driver.o

# LIBRARIES
LIB_DIR=./../lib
JSON_LIB=$(LIB_DIR)/libjsoncpp.so.21
LIBS= $(JSON_LIB)

# BINARIES
BIN_DIR=$(MF_PATH)/../bin
BINS=$(BIN_DIR)/driver
SRC_DIR=$(MF_PATH)/../src



# PHONIES
.PHONY: jsonlib clean binclean libclean test driver allclean



# Parser
$(BIN_DIR)/driver: $(DRIVER_OBJs) $(LIB_DIR)/libjsoncpp.so.21
	$(CXX) $(OPT) $(DRIVER_OBJs) $(LIB_DIR)/libjsoncpp.so.21 -o $(BIN_DIR)/driver

$(PART_DIR)/parser-utility.o: $(INC_DIR)/Parser.hpp $(SRC_DIR)/parser-utility.cpp
	$(CXX) $(OPT) $(SRC_DIR)/parser-utility.cpp -c -I$(INC_DIR)
	cp -f ./parser-utility.o $(PART_DIR)/parser-utility.o
	rm -f ./parser-utility.o

$(PART_DIR)/parser-message.o: $(INC_DIR)/Parser.hpp $(SRC_DIR)/parser-message.cpp
	$(CXX) $(OPT) $(SRC_DIR)/parser-message.cpp -c -I$(INC_DIR)
	cp -f ./parser-message.o $(PART_DIR)/parser-message.o
	rm -f ./parser-message.o

$(PART_DIR)/parser-request.o: $(INC_DIR)/Parser.hpp $(SRC_DIR)/parser-request.cpp
	$(CXX) $(OPT) $(SRC_DIR)/parser-request.cpp -c -I$(INC_DIR)
	cp -f ./parser-request.o $(PART_DIR)/parser-request.o
	rm -f ./parser-request.o

$(PART_DIR)/parser-response.o: $(INC_DIR)/Parser.hpp $(SRC_DIR)/parser-response.cpp
	$(CXX) $(OPT) $(SRC_DIR)/parser-response.cpp -c -I$(INC_DIR)
	cp -f ./parser-response.o $(PART_DIR)/parser-response.o
	rm -f ./parser-response.o

$(PART_DIR)/driver.o: $(SRC_DIR)/driver.cpp
	$(CXX) $(OPT) $(SRC_DIR)/driver.cpp -c -I$(INC_DIR)
	cp -f ./driver.o $(PART_DIR)/driver.o
	rm -f ./driver.o


# JSONCPP LIBRARY
jsonlib: $(LIB_DIR)/libjsoncpp.so.21 $(INC_DIR)/json.h

$(LIB_DIR)/libjsoncpp.so.21: $(PART_DIR)/jsoncpp.o 
	$(CXX) -shared -o $(LIB_DIR)/libjsoncpp.so.21 $(PART_DIR)/jsoncpp.o
	

$(PART_DIR)/jsoncpp.o: $(SRC_DIR)/jsoncpp.cpp
	$(CXX) $(OPT) -fPIC -I$(INC_DIR) -c $(SRC_DIR)/jsoncpp.cpp
	cp -f ./jsoncpp.o $(PART_DIR)/jsoncpp.o
	rm -f ./jsoncpp.o



# CLEANERS
allclean: clean binclean libclean

clean:
	rm -f $(DRIVER_OBJs)

binclean:
	rm -f $(BINS)

libclean:
	rm -f $(LIBS)

# TEST



