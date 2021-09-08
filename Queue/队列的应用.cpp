#include "Queue.h"
using std::cin;
using std::cout;
using std::endl;
struct Coordinate {
	int x, y;
	Coordinate() :x(0), y(0) {}
	Coordinate(const int &x) :x(x), y(x) {}
	Coordinate(const int &x, const int &y) :x(x), y(y) {}
	Coordinate operator + (const Coordinate &cor) const {
		return Coordinate(x + cor.x, y + cor.y);
	}
};
int main() {
	int n,m;
	cin >> n >> m;
	bool **matrix = new bool*[n];
	bool **visited = new bool *[n];
	for (int i = 0; i < n; i++) {
		matrix[i] = new bool[m];
		visited[i] = new bool[m];
		memset(matrix[i], false, sizeof(bool) * m);
		memset(visited[i], false, sizeof(bool) * m);
		int value;
		for (int j = 0; j < m; j++) {
			cin >> value;
			matrix[i][j] = (bool)value;
		}
	}
	const Coordinate delta[4] = { {-1,0},{0,-1},{1,0},{0,1} };
	Queue<Coordinate>queue(n * m);
	int answer = 0;
	for (int i = 1; i < n - 1; i++) {
		for (int j = 1; j < m - 1; j++) 
			if (matrix[i][j] && !visited[i][j]) {
				++answer;
				queue.push(Coordinate(i, j));
				while (!queue.empty()) {
					Coordinate curCor = queue.pop();
					visited[curCor.x][curCor.y] = true;
					for (int k = 0; k < 4; k++) {
						Coordinate newCor = curCor + delta[k];
						if (newCor.x >= 0 && newCor.x < n && newCor.y >= 0 && newCor.y < m) {
							if (matrix[newCor.x][newCor.y] && !visited[newCor.x][newCor.y]) 
								queue.push(newCor);
						}
					}
				}
			}
		
	}
	cout << answer << endl;
	return 0;
}