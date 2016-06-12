%{
	#include <stdio.h>  
	#include <stdlib.h>  
	#include <unistd.h>  
	#include <fcntl.h>  
	#include <string.h>  
	#include <sys/types.h>  
	#include <sys/stat.h>
	extern "C"
	{
    	int yywrap(void);  
    	int yylex(void);
	}  
	FILEFILE * file_fd; 
%}
name 		FN:.+
%%
{name}		{
				tmpStr << yytext;
				tmpStr << "\r\n";
			}
.			{ }
%%
int main() {
 
    int read_len, i;  
  
    if ((file_fd = open("IntermidiateOutputFile", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {  
        fprintf(stderr, "Open %s Error：%s\n", "IntermidiateOutputFile", strerror(errno));  
        exit(1);  
    } 
    else   
    {  
       printf("File Open successed!\n");  
    }  
  
    while(1)  
    {  
       read_len = fread(file_buffer, 1, 16, file_fd);  
         
       if(read_len == -1)  
       {  
           printf("File read error!\n");  
           perror("errno");  
           exit(0);  
       }  
       else if(read_len == 0)  
       {  
           printf("File read Over!\n");  
           break;  
       }  
       else   
       {  
           printf("Read %d Byte From download.bin: ",read_len);  
           for(i = 0; i < read_len; i++)  
           {  
                printf(" %02x",file_buffer[i]);  
           }  
           printf("\n");  
       }  
      
       usleep(20000);  
    }  
  
    fclose(file_fd);  
    return 0;  
	return (0);
}