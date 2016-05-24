//#include <iostream>
//#include <sstream>
//#include <vector>
//using namespace std;
//
//struct catFood{
//	int id;
//	int price;
//	int weight;
//	float costPerformance;
//};
//
//int main() {
//	int i;
//	cin >> i;
//	while (i--)
//	{
//		vector<struct catFood> v;
//		int n;
//		cin >> n;
//		while (n--)
//		{
//			struct catFood tmp;
//			cin >> tmp.id >> tmp.price >> tmp.weight;
//			tmp.costPerformance = tmp.weight / tmp.price;
//			v.push_back(tmp);
//		}
//		while (!v.empty())
//		{
//			int maxPerformance = 0;
//			int maxid = v.size();
//			//vector<struct catFood>::iterator itr = v.begin();
//			//vector<struct catFood>::iterator tmpitr = v.end();
//			vector<struct catFood>::iterator tmpitr = v.begin();
//			int tmp = 0;
//			for (int j = 0; j < v.size(); ++j)
//			{
//				if (v.at(j).costPerformance > maxPerformance)
//				{
//					maxid = v.at(j).id;
//					maxPerformance = v.at(j).costPerformance;
//					tmp = j;
//				}
//				else if (v.at(j).costPerformance == maxPerformance){
//					if (v.at(j).id < maxid)
//					{
//						maxid = v.at(j).id;
//						maxPerformance = v.at(j).costPerformance;
//						tmp = j;
//					}
//				}
//			}
//			//while (itr != v.end())
//			//{
//			//	if ((*itr).costPerformance > maxPerformance)
//			//	{
//			//		maxid = (*itr).id;
//			//		maxPerformance = (*itr).costPerformance;
//			//		tmpitr = itr;
//			//	}
//			//	else if ((*itr).costPerformance == maxPerformance) {
//			//		if ((*itr).id < maxid)
//			//		{
//			//			maxid = (*itr).id;
//			//			maxPerformance = (*itr).costPerformance;
//			//			tmpitr = itr;
//			//		}
//			//	}
//			//	++itr;
//			//}
//			tmpitr += tmp;
//			cout << maxid << " ";
//			v.erase(tmpitr);
//		}
//		cout << endl;
//	}
//}