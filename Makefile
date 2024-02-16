CC=gcc
CFLAGS=-Wall -std=c99 -g -O2 -I./include
LDFLAGS=
OBJDIR=./obj
BINDIR=./bin
SRCDIR=./src
TESTDIR=./tests
INCDIR=./include

OBJS=$(OBJDIR)/secp256r1_bigint.o $(OBJDIR)/secp256r1_utils.o \
	$(OBJDIR)/secp256r1_validation.o \
	$(OBJDIR)/main.o

TARGET=$(BINDIR)/a.out

.PHONY: all clean dir rebuild

all: dir $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
$(OBJDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) $(TARGET) $(OBJDIR)/*.d

dir:
	@mkdir -p $(OBJDIR) $(BINDIR)

rebuild: clean all

run:
	(cd bin && ./a.out)

leak: 
	(cd bin && valgrind --leak-check=full --show-leak-kinds=all ./a.out)

comp:
	(cd bin && ./a.out > speed.txt)
	(cd bin && mv speed.txt ../view/)
	(cd view && python3 compare_opt.py)