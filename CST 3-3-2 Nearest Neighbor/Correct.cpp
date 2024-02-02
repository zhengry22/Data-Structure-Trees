#include<stdio.h>
using namespace std;

class Point {
    // Points stands for each point in the space. Note that Point and Node are 2 different concepts.
private:
    int dimension = 2; // Dimension of each point
    long long component[5] = { 0, 0, 0, 0, 0 }; // Component of each point, default to be 0 in case
    // dimension is less than 5.
public:
    Point() {}
    ~Point() {}
    long long getcomponent(Dimensions d) {
        return component[d];
    }
    void input(int dim) {
        dimension = dim;
        for (int i = 0; i < dimension; i++) {
            scanf("%lld", &component[i]);
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const Point& obj) {
        for (int i = 0; i < obj.dimension; i++) {
            os << obj.component[i] << ' ';
        }
        return os;
    }
    friend long long dist_square(Point x, Point y, int dimension) {
        long long answer = 0;
        for (int i = 0; i < dimension; i++) {
            answer += (x.component[i] - y.component[i]) * (x.component[i] - y.component[i]);
        }
        return answer;
    }
};

Point points[100010], queries[200010];
int d, n, q;

int main() {

}