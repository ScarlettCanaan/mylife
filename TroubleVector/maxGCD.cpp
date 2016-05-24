//#include <iostream>
//#include <vector>
//using namespace std;
////
////int getMax(int* Array, int size) {
////	int max = 0;
////	for (int i = 0; i < size; i++)
////	{
////		if (max < Array[i]) max = Array[i];
////	}
////	return max;
////}
//
//int main() {
//	int i = 0, count;
//	cin >> i;
//	vector<int> vnum, vcount;
//	while (i--)
//	{
//		count = 0;
//		cin >> count;
//		for (int j = 0; j < count; j++)
//		{
//			int num;
//			cin >> num;
//			vnum.push_back(num);
//		}
//		int max = 0;
//		for (unsigned int i = 0; i < vnum.size(); i++)
//		{
//			if (max < vnum.at(i)) {
//				max = vnum.at(i);
//			}
//		}
//		int *counter = new int[max];
//		for (int k = 0; k < max; k++)
//		{
//				counter[k] = 0;
//		}
//		for (int j = 0; j < count; j++)
//		{
//			for (int k = 0; k < vnum.at(j); k++)
//			{
//				if (vnum.at(j) % (k + 1) == 0)
//				{
//					counter[k] += 1;
//				}
//			}
//		}
//		int maxGCD = 1;
//		for (int j = 0; j < max; j++)
//		{
//			if (counter[j] == count - 1 && counter[j] > maxGCD)
//			{
//				maxGCD = j + 1;
//			}
//		}
//		cout << maxGCD << endl;
//		vnum.clear();
//	}
//}
