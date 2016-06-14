#lex compiler
flex  contactParser.l
cc -g lex.yy.c -lfl

#C program compiler
cc -std=c99 -o ContactGraph -g main.c contacts.c queue.c
