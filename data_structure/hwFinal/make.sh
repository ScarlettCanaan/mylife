#lex compiler
flex  contactParser.l
cc -g lex.yy.c -lfl

#C program compiler
cc -o ContactGraph -g main.c contacts.c queue.c
