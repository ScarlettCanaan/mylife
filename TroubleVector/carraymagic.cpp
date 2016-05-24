//#include <iostream>
//
//using namespace std;
//
//int main() {
//	int i;
//	std::cin >> i;
//	while (i--)
//	{
//		int n, m, z, l;
//		int result;
//		cin >> n >> m >> z >> l;
//		int *v = new int[n];
//		v[0] = 0;
//		for (int j = 1; j < n; j++)
//		{
//			v[j] = (v[j - 1] * m + z) % l;
//		}
//		result = v[0] + v[0];
//		for (int j = 1; j < n; j++)
//		{
//			result ^= v[j] + v[j];
//		}
//		cout << result << endl;
//	}
//}
