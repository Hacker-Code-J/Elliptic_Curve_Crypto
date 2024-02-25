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

FILES_TO_DELETE1 = test_vector/add_and_sub/TV_MY_PFADD.rsp \
				  test_vector/add_and_sub/TV_MY_PFSUB.rsp

FILES_TO_DELETE2 = test_vector/mul_and_squ/TV_MY_MUL.rsp \
				  test_vector/mul_and_squ/TV_MY_SQU.rsp

clean:
	rm -f $(OBJS) $(TARGET) $(OBJDIR)/*.d
	rm -f bin/*.txt
	rm -f view/*.txt
	@echo "Removing Validation files ..."
	rm -f $(FILES_TO_DELETE1)
	rm -f $(FILES_TO_DELETE2)
	@echo "Validation files removal completed  successfully."

dir:
	@mkdir -p $(OBJDIR) $(BINDIR)

rebuild: clean all

run:
	(cd bin && ./a.out)

leak: 
	(cd bin && valgrind --leak-check=full --show-leak-kinds=all ./a.out)

cycle:
	(cd bin && ./a.out > cycle.txt)
	(cd bin && mv cycle.txt ../view/)
	(cd view && python3 measure_cycle.py)

cycle-s:
	(cd bin && ./a.out > cycle_single.txt)
	(cd bin && mv cycle_single.txt ../view/)
	(cd view && python3 measure_cycle_single.py)