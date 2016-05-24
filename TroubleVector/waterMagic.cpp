//#include <iostream>
//#include <sstream>
//#include <vector>
//using namespace std;
//
//struct word {
//	string s;
//	int i;
//};
//
//bool strMatch(char in) {
//	return in >= 'a' && in <= 'z';
//}
//
//int main() {
//	int i;
//	cin >> i;
//	string str;
//	getline(cin, str);
//	while (i--)
//	{	
//		getline(cin, str);
//		int strL = str.length();
//		int j = 0;
//		int counter[26] = { 0 };
//		vector<struct word> v;
//		ostringstream tmpStr;
//		while (j < strL)
//		{
//			if (strMatch(str[j])) {
//				tmpStr << str[j];
//				if (j + 1 < strL && (str[j + 1])) {
//					++j;
//					while (j < strL && strMatch(str[j])) {
//						tmpStr << str[j];
//						++j;
//					}
//				}
//				else {
//					++j;
//				}
//			}
//			else {
//				++j;
//			}
//		
//
//			vector<struct word>::iterator itr = v.begin();
//
//			while (itr != v.end())
//			{
//				if ((*itr).s.compare(tmpStr.str()) == 0)
//				{
//					++((*itr).i);
//					break;
//				}
//				++itr;
//			}
//			if (itr == v.end() && tmpStr.str() != "")
//			{
//				struct word w;
//				w.s = tmpStr.str();
//				w.i = i;
//				v.push_back(w);
//			}
//			tmpStr.str("");
//		}
//		
//		cout << v.size() << endl;
//	}
//}