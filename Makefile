CC     = gcc
CCOPTS = -O2
CCFLAGS =
#LDOPTS = -s
LDFLAGS = -lcrypto
viper:
	$(CC) -c viper.c crack.c convert.c $(CCOPTS) $(CCFLAGS)
	$(CC) -o viper viper.o crack.o convert.o $(LDOPTS) $(LDFLAGS)
clean:
	rm -f *.o viper
