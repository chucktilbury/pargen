
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
			main.o
INCDIRS	=	-I.
LIBDIRS =	-L.
LIBS	=

CC		= clang

DEBUG	=	-g
# Disable specific warnings that we don't have control over
WARNS	=	-Wno-parentheses-equality \
			-Wno-unused-variable \
			-Wno-sign-compare \
			-Wno-implicit-function-declaration \
			-Wno-gnu-zero-variadic-macro-arguments
CONFIG	=	#-DUSE_AST_TRACE -DUSE_PARSE_TRACE
CARGS	=	-Wall -Wextra -Wpedantic -std=c2x \
			$(DEBUG) \
			$(CONFIG) \
			$(INCDIRS)

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
	$(CC) $(CARGS) -Wno-gnu-zero-variadic-macro-arguments -c -o parse.o parse.c

memory.o: memory.c memory.h
	$(CC) $(CARGS) -c -o memory.o memory.c

main.o: main.c
	$(CC) $(CARGS) -c -o main.o main.c

hash.o: hash.c hash.h
	$(CC) $(CARGS) -c -o hash.o hash.c

buffer.o: buffer.c buffer.h
	$(CC) $(CARGS) -c -o buffer.o buffer.c

ptr_lst.o: ptr_lst.c ptr_lst.h
	$(CC) $(CARGS) -c -o ptr_lst.o ptr_lst.c

str.o: str.c str.h
	$(CC) $(CARGS) -c -o str.o str.c

str_lst.o: str_lst.c str_lst.h
	$(CC) $(CARGS) -c -o str_lst.o str_lst.c

ast.o: ast.c ast.h
	$(CC) $(CARGS) -Wno-gnu-zero-variadic-macro-arguments -c -o ast.o ast.c

cmdline.o: cmdline.c cmdline.h
	$(CC) $(CARGS) -Wno-gnu-zero-variadic-macro-arguments -c -o cmdline.o cmdline.c

cmderrors.o: cmderrors.c cmderrors.h
	$(CC) $(CARGS) -c -o cmderrors.o cmderrors.c

cmdparse.o: cmdparse.c cmdparse.h
	$(CC) $(CARGS) -c -o cmdparse.o cmdparse.c

regurg.o: regurg.c regurg.h
	$(CC) $(CARGS) -c -o regurg.o regurg.c

clean:
	$(RM) $(TARGET) $(OBJS) parse.c parse.h parse.output scan.c

remk: clean all

remake: remk
