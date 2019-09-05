#########################################
##	Universita' degli Studi di Udine	#	
##	Ingegneria Elettronica				#
##	Tesi di Laurea Triennale			#
##	Giacomo Andrioli - 133047			#
##	makefile for driver_client			#
#########################################

# VARIABLES
SHELL=/bin/sh
MF_PATH=.
CXX=g++
OPT=-Wall -Wfatal-errors -std=c++11
INC_DIR=$(MF_PATH)/../include


# PARTIALS OBJECTS
PART_DIR=./../part
DRIVER_CLIENT_OBJs= $(PART_DIR)/driver_client.o $(PART_DIR)/ClientAMQP.o
PARSER_OBJs=$(PART_DIR)/parser-utility.o $(PART_DIR)/parser-message.o $(PART_DIR)/parser-request.o $(PART_DIR)/parser-response.o
JSON_OBJs=$(PART_DIR)/jsoncpp.o

# LIBRARIES
LIB_DIR=./../lib
JSON_LIB=$(LIB_DIR)/libjsoncpp.so.21
QPID_VERSION=12.4.1
#QPID_LIB=$(LIB_DIR)/libqpid-proton-cpp.so.$(QPID_VERSION)
QPID_LIB=/usr/lib/libqpid-proton-cpp.so
PARSER_LIB=$(LIB_DIR)/libparser.so
LIBS=$(QPID_LIB) $(JSON_LIB) $(PARSER_LIB)


# BINARIES
BIN_DIR=$(MF_PATH)/../bin
BINS=$(BIN_DIR)/driver_client
SRC_DIR=$(MF_PATH)/../src


# PHONIES
.PHONY: jsonlib libparser clean binclean libclean driver_client test allclean


all: jsonlib libparser driver_client


# JSONCPP LIBRARY
jsonlib: $(LIB_DIR)/libjsoncpp.so.21 

$(LIB_DIR)/libjsoncpp.so.21: $(PART_DIR)/jsoncpp.o 
	$(CXX) -shared -o $(LIB_DIR)/libjsoncpp.so.21 $(PART_DIR)/jsoncpp.o

$(PART_DIR)/jsoncpp.o: $(SRC_DIR)/jsoncpp.cpp $(INC_DIR)/json.h
	$(CXX) $(OPT) -fPIC -I$(INC_DIR) -c $(SRC_DIR)/jsoncpp.cpp
	cp -f ./jsoncpp.o $(PART_DIR)/jsoncpp.o
	rm -f ./jsoncpp.o


# PARSER LIB
libparser: $(LIB_DIR)/libparser.so

$(LIB_DIR)/libparser.so: $(PARSER_OBJs)
	$(CXX) -shared -o $(LIB_DIR)/libparser.so $(PARSER_OBJs)

$(PART_DIR)/parser-utility.o: $(INC_DIR)/Parser.hpp $(SRC_DIR)/parser-utility.cpp
	$(CXX) $(OPT) -fPIC $(SRC_DIR)/parser-utility.cpp -c -I$(INC_DIR)
	cp -f ./parser-utility.o $(PART_DIR)/parser-utility.o
	rm -f ./parser-utility.o

$(PART_DIR)/parser-message.o: $(INC_DIR)/Parser.hpp $(SRC_DIR)/parser-message.cpp
	$(CXX) $(OPT) -fPIC $(SRC_DIR)/parser-message.cpp -c -I$(INC_DIR)
	cp -f ./parser-message.o $(PART_DIR)/parser-message.o
	rm -f ./parser-message.o

$(PART_DIR)/parser-request.o: $(INC_DIR)/Parser.hpp $(SRC_DIR)/parser-request.cpp
	$(CXX) $(OPT) -fPIC $(SRC_DIR)/parser-request.cpp -c -I$(INC_DIR)
	cp -f ./parser-request.o $(PART_DIR)/parser-request.o
	rm -f ./parser-request.o

$(PART_DIR)/parser-response.o: $(INC_DIR)/Parser.hpp $(SRC_DIR)/parser-response.cpp
	$(CXX) $(OPT) -fPIC $(SRC_DIR)/parser-response.cpp -c -I$(INC_DIR)
	cp -f ./parser-response.o $(PART_DIR)/parser-response.o
	rm -f ./parser-response.o


# CLIENT DRIVER 
driver_client: $(BIN_DIR)/driver_client

$(BIN_DIR)/driver_client: $(DRIVER_CLIENT_OBJs)
	$(CXX) $(OPT) -o $(BIN_DIR)/driver_client $(DRIVER_CLIENT_OBJs) $(LIBS) -pthread

$(PART_DIR)/driver_client.o: $(SRC_DIR)/driver_client.cpp $(INC_DIR)/ClientAMQP.hpp
	$(CXX) $(OPT) -c -I$(INC_DIR) $(SRC_DIR)/driver_client.cpp
	cp -f ./driver_client.o $(PART_DIR)/driver_client.o
	rm -f ./driver_client.o

$(PART_DIR)/ClientAMQP.o: $(SRC_DIR)/ClientAMQP.cpp $(INC_DIR)/ClientAMQP.hpp
	$(CXX) $(OPT) -c -I$(INC_DIR) $(SRC_DIR)/ClientAMQP.cpp
	cp -f ./ClientAMQP.o $(PART_DIR)/ClientAMQP.o
	rm -f ./ClientAMQP.o


# CLEANERS
allclean: clean binclean libclean

clean:
	rm -f $(DRIVER_CLIENT_OBJs) $(PARSER_OBJs) $(JSON_OBJs)

binclean:
	rm -f $(BINS)

libclean:
	rm -f  $(JSON_LIB) $(PARSER_LIB)
