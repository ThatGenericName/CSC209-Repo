FLAGS = -Wall -g
all : simple large die server

simple : simple.o cgi.o
	gcc ${FLAGS} -o $@ $^

large : large.o cgi.o 
	gcc ${FLAGS} -o $@ $^

die : die.o
	gcc ${FLAGS} -o $@ $^

server : server.o
	gcc ${FLAGS} -o $@ $^

%.o : %.c 
	gcc ${FLAGS} -c $<


# test commands

test : simple large die server
	./runTests.sh testfiles/msg1 testfiles/msg1.exp
	./runTests.sh testfiles/msg2 testfiles/msg2.exp
	./runTests.sh testfiles/msg3 testfiles/msg3.exp
	./runTests.sh testfiles/realmessages testfiles/realmessages.exp
	./runTests.sh testfiles/testBadResource testfiles/testBadResource.exp
	./runTests.sh testfiles/testMultiReqNoSpace testfiles/testMultiReqNoSpace.exp

# clean commands

clean1 : 
	rm *.o

clean2 :
	rm simple large die server
