#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <sstream> // std::istringstream
#include <tuple>
#include <algorithm>
using namespace std;

struct point {
	point() = default;
	point(const float _x, const float _y) : x(_x), y(_y) { };
	float x{}, y{};
};

double cross(const point &a, const point &b, const point &c, const point &d) {
	return (b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x);
}

double getDistance(const point &a, const point &b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

/// graham_scan�㷨����͹������vectorģ��stack
vector<point> graham_scan(vector<point> p) {	// convex �ǽ��
	printf("**** graham scan begin ****\n\n");

	const int N = p.size();
	vector<point> convex(N);

	// 1���ҳ�y��С�ĵ㣬����ж�����ҳ�����x��С�ĵ㣬��Ϊ��ʼ��
	int ori_idx = 0;
	for (int i = 1; i < N; ++i)
		if (p[i].y < p[ori_idx].y || (p[i].y == p[ori_idx].y) && p[i].x < p[ori_idx].x)
			ori_idx = i;
	printf("origin: p[%d] = (%.2f, %.2f)\n\n", ori_idx, p[ori_idx].x, p[ori_idx].y);
	swap(p[0], p[ori_idx]);

	// 2�����ռ��Ƕ�ʣ�� N - 1 ����������򣬽Ƕ���ͬ��x����
	sort(p.begin() + 1, p.end(), [p0 = p[0]](const point &a, const point &b) {
		const double cro = cross(p0, a, p0, b);
		if (cro == 0) return a.x < b.x;
		return cro > 0;
	}
	);
	printf("����������\n");
	for (int i = 0; i < N; ++i)
		printf("(%.0f, %.0f)\n", p[i].x, p[i].y);
	printf("\n");

	// 3������ graham scan �㷨, top ������
	int top = 0, i = 0;
	while (i <= N && top < N) {
		//��������ֱ����ջ���������жϷ���ֱ���ɹ���ջ
		while (top >= 2 && cross(convex[top - 2], convex[top - 1], convex[top - 2], p[i % N]) < 0) --top;
			convex[top] = p[i % N];
		++top;
		++i;
	}

	// 4������ʱ��˳�����
	convex = vector<point>(convex.begin(), convex.begin() + top);

	cout << "(" << convex[0].x << ", " << convex[0].y << ")" << " - ���" << endl;
	for (int i = 1; i < convex.size(); ++i)
		cout << "(" << convex[i].x << ", " << convex[i].y << ")" << endl;
	cout << "(" << convex[0].x << ", " << convex[0].y << ")" << " - �ص����" << endl;

	printf("\n**** graham scan end ****\n\n");
	return convex;
}

int main()
{
	const vector<point> p{ { 2, 0 },{ 1, 0 }, {3, 0},{ 0, 1 },{ 0, 2 },{ 3, 1 },{ 3, 3 },{ 3, 2 },{ 2, 3 },{ 1, 3 },{ 1, 2 },{ 2, 1 } };
	vector<point> res = graham_scan(p);
	return 0;
}