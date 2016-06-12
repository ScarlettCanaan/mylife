%{
	#include <unistd.h>  
	#include <fcntl.h>  
	#include <stdio.h>  
	#include <sys/types.h>  
	#include <sys/stat.h>  
	#include <errno.h>  
	#include <string.h> 
	#include <malloc.h>
	int  file_fd; 
	char strPtr[100000];
%}
name 		FN:.+\n
%%
{name}		{
				ECHO;
				strcat(strPtr, yytext);
			}
.|\n			{ }
%%
int main() {
	yylex();
    if ((file_fd = open("IntermidiateOutputFile", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {  
        fprintf(stderr, "Open %s Error：%s\n", "IntermidiateOutputFile", strerror(errno));  
        exit(1);  
    } 
    else   
    {  
       printf("File Open successed!\n");  
    }  
    if ((write(file_fd, strPtr, strlen(strPtr)) == -1))
    {
    	fprintf(stderr, "Write %s Error：%s\n", "IntermidiateOutputFile", strerror(errno));  
    }
    close(file_fd);  
	return (0);
}