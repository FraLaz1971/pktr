# set this to the top level directory, where the packets package is installed
PACKETS_DIR=nasapkt

# set the full path to the default APID list that squirt should use
# the examples/swift_apids.list file is a pretty good one to use.
DEFAULT_APID_LIST=examples/swift_apids.list

# uncomment these two lines if you are using an older compiler without <sstream>
OLD_COMPILER_INC = -I.
OLD_COMPILER_INSTALL = cp sstream $(INCLUDE)

###############################################
# shouldn't need to touch anything below here
###############################################

CCFLAGS = -Wall $(OLD_COMPILER_INC)

CXX=g++ 
DB2CC=$(PACKETS_DIR)/src/db2class

INSTALL_DIR=$(PACKETS_DIR)

BIN=$(INSTALL_DIR)/bin
DOC=$(INSTALL_DIR)/doc
LIB=$(INSTALL_DIR)/src
INCLUDE=$(INSTALL_DIR)/src

TEST=../test

LIBRARY_NAME=packets
LIBRARY=lib$(LIBRARY_NAME).a

LIBS = -L $(LIB) -l$(LIBRARY_NAME)
INC = -I$(INCLUDE)

DBSOURCES= 
DBCLASSES = 

NASA_LIBPKT = $(PACKETS_DIR)/src/libpackets.a

.PHONY: all install clean distclean distprep libpkt cleanlibpkt

TOOLS = pktr
CLASSES=$(EXPRESSION_CLASSES)

all:	$(DBSOURCES) $(CLASSES) $(TOOLS)

libpkt:
	bash buildnasapkt.sh

$(NASA_LIBPKT): libpkt

cleanlibpkt:
	bash cleannasapkt.sh

EXPRESSION_CLASSES = TokenList.o Expression.o OperatorExpression.o \
                     ConstantExpression.o FieldExpression.o ParenExpression.o\
                     PlusMinusExpression.o TimesOverExpression.o \
                     ComparisonExpression.o LogicExpression.o NotExpression.o \
		     StringExpression.o  VariableExpression.o \
                     PacketFilter.o Context.o AssignmentOperator.o \
                     DataFieldExpression.o BitwiseExpression.o

install:	all
		mv $(TOOLS) ${BIN}
		rm -f *.o *.html
		$(OLD_COMPILER_INSTALL)

classes:	$(DBSOURCES)

doc:
	rm -f $(DOC)/*
#	mv *.html $(DOC)

clean:
	rm -f $(DBSOURCES) $(CLASSES) $(TOOLS) *.html *% core

distclean:	clean classes doc cleanlibpkt
		rm -f *core

$(LIBRARY):	$(CLASSES)
		ar -r $(LIBRARY) $(CLASSES)


pktr:	$(NASA_LIBPKT)  $(CLASSES) pktr.cc
	$(CXX) -o $@ pktr.cc -DDEFAULT_APID_LIST=\"$(DEFAULT_APID_LIST)\" $(INC) $(CLASSES) $(LIBS) $(CCFLAGS)

%.cc:	%.db $(DB2CC)
	$(DB2CC) $<

%.o:	%.cc %.h
	$(CXX) -c $< $(INC) $(CCFLAGS)
