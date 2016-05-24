//#include <iostream>
//#include <sstream>
//#include <vector>
//#include <string.h>
//using namespace std;
//
//typedef struct Word {
//	string s;
//	int i;
//}Word;
//
//int main() {
//	int i;
//	std::cin >> i;
//	vector<Word> v;
//	int max = 0;
//	while (i--)
//	{
//		int n;
//		cin >> n;
//		max += n;
//		while (n--)
//		{
//			string str;
//			std::cin >> str;
//			int i = 0;
//			while (i < v.size())
//			{
//				if (v.at(i).s.compare(str) == 0)
//				{
//					++v.at(i).i;
//					break;
//				}
//				++i;
//			}
//			if (i == v.size())
//			{
//				Word w;
//				w.s = str;
//				w.i = 0;
//				v.push_back(w);
//			}
//		}
//	}
//	int min = max;
//	vector<Word> v2;
//	while (!v.empty())
//	{
//		vector<Word>::iterator itr = v.begin();
//		for (int i = 0; i < v.size(); i++)
//		{
//			if (v.at(i).i < min) {
//				min = i;
//				itr = v.begin() + i;
//			}
//			else if (v.at(i).i == min){
//				if (strcmp((*itr).s.c_str(), v.at(i).s.c_str()) > 0)
//				{
//					min = i;
//					itr = v.begin() + i;
//				}
//			}
//		}
//		cout << (*itr).s << endl;
//		v.erase(itr);
//	}
//}