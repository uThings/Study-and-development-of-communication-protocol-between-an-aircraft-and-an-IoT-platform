#########################################
##	Universita' degli Studi di Udine	#	
##	Ingegneria Elettronica				#
##	Tesi di Laurea Triennale			#
##	Giacomo Andrioli - 133047			#
##	makefile for driver_server			#
#########################################

# VARIABLES
SHELL=/bin/sh
MF_PATH=.
CXX=g++
OPT=-Wall -Wfatal-errors -std=c++11
INC_DIR=$(MF_PATH)/../include

# PARTIALS OBJECTS
PART_DIR=./../part
DRIVER_SERVER_OBJs= $(PART_DIR)/driver_server.o $(PART_DIR)/ServerAMQP.o

PARSER_OBJs=$(PART_DIR)/parser-utility.o $(PART_DIR)/parser-message.o $(PART_DIR)/parser-request.o $(PART_DIR)/parser-response.o

# LIBRARIES
LIB_DIR=./../lib

JSON_LIB=$(LIB_DIR)/libjsoncpp.so.21
QPID_VERSION=12.4.1
QPID_LIB=/usr/lib/libqpid-proton-cpp.so
PARSER_LIB=$(LIB_DIR)/libparser.so

LIBS=$(QPID_LIB) $(JSON_LIB) $(PARSER_LIB)

# BINARIES
BIN_DIR=$(MF_PATH)/../bin
BINS=$(BIN_DIR)/driver_server 
SRC_DIR=$(MF_PATH)/../src


# PHONIES
.PHONY: jsonlib clean binclean libclean driver_server test libparser allclean


all: jsonlib libparser driver_server



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



# SERVER DRIVER
driver_server: $(BIN_DIR)/driver_server

$(BIN_DIR)/driver_server: $(DRIVER_SERVER_OBJs)
	$(CXX) $(OPT) -o $(BIN_DIR)/driver_server $(DRIVER_SERVER_OBJs) $(LIBS) -pthread

$(PART_DIR)/driver_server.o: $(SRC_DIR)/driver_server.cpp $(INC_DIR)/ServerAMQP.hpp
	$(CXX) $(OPT) -c -I$(INC_DIR) $(SRC_DIR)/driver_server.cpp
	cp -f ./driver_server.o $(PART_DIR)/driver_server.o
	rm -f ./driver_server.o

$(PART_DIR)/ServerAMQP.o: $(SRC_DIR)/ServerAMQP.cpp $(INC_DIR)/ServerAMQP.hpp
	$(CXX) $(OPT) -c -I$(INC_DIR) $(SRC_DIR)/ServerAMQP.cpp
	cp -f ./ServerAMQP.o $(PART_DIR)/ServerAMQP.o
	rm -f ./ServerAMQP.o




# CLEANERS
allclean: clean binclean libclean

clean:
	rm -f $(PARSER_OBJs) $(DRIVER_SERVER_OBJs) jsoncpp.o

binclean:
	rm -f $(BINS)

libclean:
	rm -f $(JSON_LIB) $(PARSER_LIB)

