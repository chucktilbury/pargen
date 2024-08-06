
TARGET	=	pargen

OBJS	=	scan.o \
			memory.o \
			parse.o \
			hash.o \
			buffer.o \
			ptr_lst.o \
			str.o \
			str_lst.o \
			ast.o \
			regurg.o \
			cmdline.o \
			cmderrors.o \
			cmdparse.o \
			database.o \
			main.o
INCDIRS	=	-I.
LIBDIRS =	-L.
LIBS	=

CC		= 	clang
RM		=	rm -rf

DEBUG	=	-g
CONFIG	=	-DUSE_DEBUGGING #-DUSE_AST_TRACE -DUSE_PARSE_TRACE
CARGS	=	$(DEBUG)\
			$(CONFIG)\
			$(INCDIRS)\
			-Wall -Wextra -Wpedantic -Wno-gnu-zero-variadic-macro-arguments 

all: $(TARGET)

.c.o:
	$(CC) $(CARGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(CARGS) $(LIBDIRS) -o $(TARGET) $(OBJS) $(LIBS)

scan.c: scan.l parse.h
	flex -o scan.c scan.l

parse.c parse.h: parse.y
	bison -r all -do parse.c parse.y

scan.o: scan.c scan.h
	$(CC) $(CARGS) -Wno-parentheses-equality \
			-Wno-unused-variable \
			-Wno-sign-compare \
			-Wno-implicit-function-declaration \
			-c -o scan.o scan.c

parse.o: parse.c parse.h
	$(CC) $(CARGS) -Wno-constant-logical-operand -c -o parse.o parse.c

memory.o: memory.c memory.h
main.o: main.c
hash.o: hash.c hash.h
buffer.o: buffer.c buffer.h
ptr_lst.o: ptr_lst.c ptr_lst.h
str.o: str.c str.h
str_lst.o: str_lst.c str_lst.h
ast.o: ast.c ast.h
cmdline.o: cmdline.c cmdline.h
cmderrors.o: cmderrors.c cmderrors.h
cmdparse.o: cmdparse.c cmdparse.h
regurg.o: regurg.c regurg.h
database.o: database.c database.h

clean:
	-$(RM) $(TARGET) *.core $(OBJS) parse.c parse.h parse.output scan.c

remk: clean all

remake: remk

format: clean
	clang-format -i --verbose *.c *.h