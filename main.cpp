#include <iostream>
#include <cmath>
#include <vector>
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

/// graham_scan算法计算凸包，用vector模拟stack
void graham_scan(vector<point> p, vector<point>& convex) {	// convex 是结果
	printf("**** graham scan begin ****\n\n");
	
	const int N = p.size();
	convex.resize(N);

	// 1、找出y最小的点，如果有多个，找出其中x最小的点，作为起始点
	int ori_idx = 0;
	for (int i = 1; i < N; ++i)
		if (p[i].y < p[ori_idx].y || (p[i].y == p[ori_idx].y) && p[i].x < p[ori_idx].x)
			ori_idx = i;
	printf("origin: p[%d] = (%.2f, %.2f)\n\n", ori_idx, p[ori_idx].x, p[ori_idx].y);
	swap(p[0], p[ori_idx]);

	// 2、按照极角对剩余 N - 1 个点进行排序，角度相同按x升序
	sort(p.begin() + 1, p.end(), [p0 = p[0]](const point &a, const point &b) {
			const double cro = cross(p0, a, p0, b);
			if (cro == 0) return a.x < b.x;
			return cro > 0;
		}
	);
	printf("极角排序结果\n");
	for (int i = 0; i < N; ++i)
		printf("(%.2f, %.2f)\n", p[i].x, p[i].y);

	// 3、进行 graham scan 算法, top 是总数
	int top = 0, i = 0;
	while (i <= N) {
		//不足两个直接入栈，否则叉积判断方向，直到成功入栈
		while (top >= 2 && cross(convex[top - 2], convex[top - 1], convex[top - 2], p[i % N]) < 0) --top;
		convex[top] = p[i % N];
		++top;
		++i;
	}

	// 4、按逆时针顺序输出
	printf("\n按逆时针顺序输出凸包点\n");
	for (int i = 0; i < top; ++i)
		cout << convex[i].x << " " << convex[i].y << endl;

	convex = vector<point>(convex.begin(), convex.begin() + top);
	printf("\n**** graham scan end ****\n\n");
}

int main()
{
	const vector<point> p{ {1, 0}, {2, 0}, {0, 1}, {0, 2}, {3, 1}, {3, 3}, {3, 2}, {2, 3},{1, 3}, {1, 2}, {2, 1} };
	vector<point> res;
	graham_scan(p, res);
	printf("凸包结果为\n");
	for (const auto& ri : res)
		cout << ri.x << " " << ri.y << endl;
	return 0;
}