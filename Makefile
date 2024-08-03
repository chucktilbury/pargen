
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
			main.o
INCDIRS	=
LIBDIRS =
LIBS	=

CC		= clang

DEBUG	=	-g
# Disable specific warnings that we don't have control over
WARNS	=	-Wno-parentheses-equality \
			-Wno-unused-variable \
			-Wno-sign-compare \
			-Wno-implicit-function-declaration
WARNS1	=	-Wno-gnu-zero-variadic-macro-arguments
CONFIG	=	-DUSE_AST_TRACE #-DUSE_PARSE_TRACE
CARGS	=	-Wall -Wextra -Wpedantic -std=c2x \
			$(DEBUG) \
			$(CONFIG) \
			$(WARNS1) \
			$(INCDIRS)

%.o:%.c
	$(CC) $(CARGS) -c -o $@ $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CARGS) $(LIBDIRS) -o $(TARGET) $(OBJS) $(LIBS)

scan.c: scan.l parse.h
	flex -o scan.c scan.l

parse.c parse.h: parse.y
	bison -r all -do parse.c parse.y

scan.o: scan.c scan.h
	$(CC) $(CARGS) $(WARNS) -c -o scan.o scan.c

parse.o: parse.c parse.h
	$(CC) $(CARGS) -c -o parse.o parse.c

memory.o: memory.c memory.h
main.o: main.c
hash.o: hash.c hash.h
buffer.o: buffer.c buffer.h
ptr_lst.o: ptr_lst.c ptr_lst.h
str.o: str.c str.h
str_lst.o: str_lst.c str_lst.h
ast.o: ast.c ast.h

clean:
	$(RM) $(TARGET) $(OBJS) parse.c parse.h parse.output scan.c
