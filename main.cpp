#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdio>

#define OFFSET 20
#define N 567890

#define VECSORT(X) std::sort(X.begin(), X.end())

// #define Input "t.in"
// #define Output "t.out"
#define Input "/data/test_data.txt"
#define Output "/projects/student/result.txt"


struct tre {
	long long x, y, z;
	tre(long long x, long long y, long long z) : x(x), y(y), z(z) {}
	bool operator< (tre k) const {
		return x < k.x ||
			x == k.x && y < k.y ||
			x == k.x && y == k.y && z < k.z;
	}
	long long operator[] (int k) {
		return k == 0 ? x : k == 1 ? y : z;
	}
};

std::vector<tre> res, data;
std::vector<int> disc, stack;
std::vector<std::pair<int, int>> hh;
int visit[N];

// void dfs(int k, int depth){//std::cout << k << ' ' << depth << '\n';
// 	visit[k] = depth;
// 	stack.push_back(k);
// 	for (auto i : edge[k])
// 		if (visit[i] && stack[visit[i]] == i){
// 			int len = visit[k] - visit[i] + 1;
// 			if (len >= 3 && len <= 7){
// 				int start = depth;
// 				for (int j = visit[i]; j < depth; j ++ )
// 					if (stack[j] < stack[start])
// 						start = j;
// 				start -= visit[i];
// 				long long arr[3] = {0};
// 				arr[0] = len * 1LL << (OFFSET * 2);
// 				//std::cout << start << ',' << depth << ' ' << len << ' ';
// 				for (int j = 0; j < len; j ++ ){
// 					long long num = stack[visit[i] + (j + start) % len];
// 					//std::cout << num << ' ';
// 					int index = (j + 1) / 3, move = 2 - (j + 1) % 3;
// 					arr[index] += num << (OFFSET * move);
// 				}
// 				//std:: cout << arr[0] << ',' << arr[1] << ',' << arr[2] << '\n';
// 				res.push_back(tre(arr[0], arr[1], arr[2]));
// 			}
// 		}
// 		else if (!visit[i]) dfs(i, depth + 1);
// 	stack.pop_back();
// }

std::vector<std::vector<std::pair<int, int>>> listf[N], listr[N];
std::vector<std::vector<std::pair<int, int>>>* list;
std::vector<int> edgef[N], edger[N];
std::vector<int>* edge;

void dfs3(int K) {
	list[K].resize(3);
	//TODO: change to bfs
	for (auto i : edge[K]) {
		list[K][0].push_back(std::make_pair(i, 0));
		for (auto j : edge[i]) {
			if (j == K) continue;
			list[K][1].push_back(std::make_pair(j, i));
			for (auto k : edge[j]) {
				if (k == K || k == i) continue;
				list[K][2].push_back(std::make_pair(k, j));
			}
		}
	}
	VECSORT(list[K][0]);
	VECSORT(list[K][1]);
	VECSORT(list[K][2]);
	for (auto& i : list[K][1])
		i.second = std::lower_bound(list[K][0].begin(), list[K][0].end(), i.second,
			[](std::pair<int, int> x, int y) { return x.first < y; }) - list[K][0].begin();
	for (auto& i : list[K][2])
		i.second = std::lower_bound(list[K][1].begin(), list[K][1].end(), i.second,
			[](std::pair<int, int> x, int y) { return x.first < y; }) - list[K][1].begin();
}

std::vector<std::vector<int>> getroute(int k, int i, int j, std::vector<std::vector<std::pair<int, int>>>* list) {
	// printf("gr: %d %d %d %s\n", k, i, j, list == listf ? "forward" : "backward");
	std::vector<std::vector<int>> ret;
	if (i == -1) {
		std::vector<int> v;
		v.push_back(k);
		ret.push_back(v);
		return ret;
	}
	int first = list[k][i][j].first;
	for (int q = j; q < list[k][i].size() && list[k][i][q].first == first; q++) {
		if (q != j && list[k][i][q].second == list[k][i][q - 1].second) continue;
		auto deep = getroute(k, i - 1, list[k][i][q].second, list);
		for (auto& i : deep) {
			i.push_back(first);
			ret.push_back(i);
		}
	}
	return ret;
}

void push_to_res(int len, long long* arr) {
	// std::cout << "PR: " << len << ' ';
	// for (int i = 0; i < len; i++) std::cout << arr[i] << ' '; std::cout << '\n';
	long long tarr[3] = { 0 };
	tarr[0] = len * 1LL << (OFFSET * 2);
	for (int j = 0; j < len; j++) {
		int index = (j + 1) / 3, move = 2 - (j + 1) % 3;
		tarr[index] += arr[j] << (OFFSET * move);
	}
	res.push_back(tre(tarr[0], tarr[1], tarr[2]));
}

void checksame(int kx, int ix, int ky, int iy) {
	// printf("cs: %d %d %d %d\n", kx, ix, ky, iy);
	for (int p1 = 0, p2 = 0; p1 < listr[kx][ix].size() && p2 < listf[ky][iy].size(); ) {
		int f1 = listr[kx][ix][p1].first, f2 = listf[ky][iy][p2].first;
		if (f1 > kx&& f2 > kx)
			if (f1 == f2) {
				auto xroute = getroute(kx, ix, p1, listr), yroute = getroute(ky, iy, p2, listf);
				for (auto& i : xroute)
					for (auto& j : yroute) {
						// for (auto a : i) std::cout << a; std::cout << '|';
						// for (auto a : j) std::cout << a; std::cout << '\n';
						bool same = 0;
						for (int ii = 0; ii < i.size() - 1; ii++)
							for (int jj = 0; jj < j.size() - 1; jj++)
								same += i[ii] == j[jj];
						for (int ii = 1; ii < i.size() - 1; ii++)
							same += i[ii] <= kx;
						for (int jj = 1; jj < j.size() - 1; jj++)
							same += j[jj] <= kx;
						same += i.size() == 4 && i[1] == i[3];
						same += j.size() == 4 && j[1] == j[3];

						if (same) continue;
						std::vector<long long> num;
						num.push_back(kx);
						for (auto q : j)
							num.push_back(q);
						for (int q = i.size() - 2; q > 0; q--)
							num.push_back(i[q]);
						push_to_res(num.size(), &num[0]);
					}
			}
		if (f1 <= f2) for (; p1 < listr[kx][ix].size() && listr[kx][ix][p1].first == f1; p1++);
		if (f1 >= f2) for (; p2 < listf[ky][iy].size() && listf[ky][iy][p2].first == f2; p2++);
	}
}

int main() {
	freopen(Input, "r", stdin);
	freopen(Output, "w", stdout);
	stack.push_back(0);
	disc.push_back(0);
	int x, y, z;
	while (~scanf("%d,%d,%d", &x, &y, &z)) {
		data.push_back(tre(x, y, z));
		hh.push_back(std::make_pair(x, data.size() * 2 - 2));
		hh.push_back(std::make_pair(y, data.size() * 2 - 1));
	}
	//for (auto i : hh) std::cout << i.first << ',' << i.second << ' ';std::cout << '\n';
	VECSORT(hh);
	//for (auto i : hh) std::cout << i.first << ',' << i.second << ' ';std::cout << '\n';
	for (int i = 0; i < hh.size(); i++) {
		if (!i || hh[i].first != hh[i - 1].first) {
			disc.push_back(hh[i].first);
		}
		int index = hh[i].second / 2, isy = hh[i].second % 2;
		(isy ? data[index].y : data[index].x) = disc.size() - 1;
	}
	//for (auto i : disc) std::cout << i << ' ';std::cout << '\n';
	//for (auto i : data) std::cout << i.x << ',' << i.y << ',' << i.z << '\n'; std::cout << '\n';
	for (auto i : data) {
		edgef[i.x].push_back(i.y);
		edger[i.y].push_back(i.x);
	}

	// for (auto i : edgef) VECSORT(i);
	// for (auto i : edger) VECSORT(i);
	// for (int i = 1; i < disc.size(); i ++ )
	// 	if (!visit[i]) dfs(i, 1);

	// std::cout << "1\n";
	list = listf;
	edge = edgef;
	for (int i = 1; i < disc.size(); i++) dfs3(i);
	list = listr;
	edge = edger;
	for (int i = 1; i < disc.size(); i++) dfs3(i);
	// std::cout << "2\n";

	for (auto& i : data) {
		if (i.x < i.y) {
			// std::cout << "edge: " << i.x << i.y << '\n';
			checksame(i.x, 0, i.y, 0);
			checksame(i.x, 0, i.y, 1);
			checksame(i.x, 0, i.y, 2);
			checksame(i.x, 1, i.y, 2);
			checksame(i.x, 2, i.y, 2);
		}
	}
	// std::cout << "3\n";

	VECSORT(res);
	//for (auto i : res) std::cout << i.x << ',' << i.y << ',' << i.z << ' '; std::cout << '\n';
	printf("%d\n", (int)res.size());
	for (auto i : res) {
		int len = i.x >> (OFFSET * 2);
		for (int j = 1; j <= len; j++) {
			int index = j / 3, move = 2 - j % 3;
			printf("%d", disc[(i[index] >> (OFFSET * move))& ((1 << OFFSET) - 1)]);
			putchar(j == len ? '\n' : ',');
		}
	}
}