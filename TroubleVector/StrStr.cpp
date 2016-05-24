//#include <iostream>
//#include <sstream>
//
//int main() {
//	int i, c;
//	std::cin >> i;
//	std::string src;
//	std::string dest;
//	while (i--)
//	{
//		c = 0;
//		std::cin >> src;
//		std::cin >> dest;
//
//		int destLen = dest.length();
//		std::string::iterator itr = src.begin();
//		std::string::iterator itrd = dest.begin();
//		while (itr <= (src.end() - destLen))
//		{
//			if (*itr == *itrd)
//			{
//				int count = 1;
//				for (int j = 1; j < destLen; j++)
//				{
//					if (*(itr+j) == *(itrd+j))
//					{
//						++count;
//					}
//				}
//				if (count == destLen)
//				{
//					++c;
//					itr += destLen - 1;
//				}
//			}
//			++itr;
//		}
//		std::cout << c << std::endl;
//	}
//}
