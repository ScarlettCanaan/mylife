#lex compiler
flex  contactParser.l
cc -g -o contactParser lex.yy.c -lfl
g++ -g -o fbFriendsParser fbFriendsParser.cpp

#C program compiler
cc -std=c99 -o ContactGraph -g main.c contacts.c queue.c
