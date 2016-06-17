	#include <unistd.h>  
	#include <fcntl.h>  
	#include <stdio.h>  
	#include <sys/types.h>  
	#include <sys/stat.h>  
	#include <errno.h>  
	#include <malloc.h>
	#include <iostream>
	#include <sstream>
	#include <string>

int main() {
	std::string str;
	std::string match("\"name\": ");
	std::ostringstream tmpStr;
	std::cout << match;
	while(getline(std::cin, str))
	{
		//std::cout << str;
		int found;
		if ((found = str.find(match)) != std::string::npos) {
			for (int i = found+9; str[i] != '\"'; ++i)
			{
				tmpStr << str[i];
			}
			tmpStr << std::endl;
		}
	}
	std::cout << tmpStr;
	int  file_fd;
    if ((file_fd = open("IntermidiateOutputFile2", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {  
    	std::cout << "1";
    } 
    else   
    {  
       printf("File Open successed!\n");  
    }  
    std::string rStr(tmpStr.str());
    if ((write(file_fd, rStr.c_str(), rStr.length()) == -1))
    {
    	std::cout << "2";
    }
    close(file_fd);  
	return (0);
}
