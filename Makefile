CC     = gcc
CCOPTS = -O2
CCFLAGS =
#LDOPTS = -s
LDFLAGS = -lssl -lcrypto
viper:
	$(CC) $(CCOPTS) $(CCFLAGS) -c viper.c crack.c convert.c
	$(CC) $(LDOPTS) $(LDFLAGS) -o viper viper.o crack.o convert.o
clean:
	rm -f *.o viper
