#pragma warning (disable:4996)
#include <stdio.h>
#include <iostream>
#include <limits.h>
using namespace std;
#define LEAF 4
//#define MYDEBUG

// Up to 5 dimensions
enum Dimensions {
    D1, D2, D3, D4, D5,
};

// The basic data structures
class Point {
    // Points stands for each point in the space. Note that Point and Node are 2 different concepts.
private:
    int component[5] = { 0, 0, 0, 0, 0 }; // Component of each point, default to be 0 in case
    // dimension is less than 5.
public:
    Point() {}
    ~Point() {}
    long long getcomponent(Dimensions d) {
        return component[d];
    }
    void input(int dim) {
        switch(dim){
            case 2:
                scanf("%d%d", &component[0], &component[1]);
                break;
            case 3:
                scanf("%d%d%d", &component[0], &component[1], &component[2]);
                break;
            case 4:
                scanf("%d%d%d%d", &component[0], &component[1], &component[2], &component[3]);
                break;
            case 5:
                scanf("%d%d%d%d%d", &component[0], &component[1], &component[2], &component[3], &component[4]);
                break;
            default:
                break;
        }
    }
    // friend std::ostream& operator<<(std::ostream& os, const Point& obj) {
    //     for (int i = 0; i < obj.dimension; i++) {
    //         os << obj.component[i] << ' ';
    //     }
    //     return os;
    // }
    long long dist_square(Point &x, int dimension) {
        long long answer = 0;
        for (int i = 0; i < dimension; i++) {
            long long mtp = component[i] - x.component[i];
            answer += mtp * mtp;
        }
        return answer;
    }
};

struct Node {
    Point point[LEAF];
    Dimensions direction;
    int size = 1;
    bool is_leaf = false;
    // friend std::ostream& operator<<(std::ostream& os, const Node& obj) {
    //     for (int i = 0; i < obj.size; i++) {
    //         os << "point" << i << ": " << obj.point[i] << ' ';
    //     }
    //     return os;
    // }
};

int partition(Point *array, int left, int right, Dimensions d) {
    int l = left, r = right;
    Point pivot = array[left];
    while (left < right) {
        while (left < right && array[right].getcomponent(d) > pivot.getcomponent(d)) {
            right--;
        }
        if (left < right) {
            array[left] = array[right];
        }
        while (left < right && array[left].getcomponent(d) <= pivot.getcomponent(d)) {
            left++;
        }
        if (left < right) {
            array[right] = array[left];
        }
    }
    array[left] = pivot;
    return left;
}

int getmid(Point *array, int left, int right, Dimensions d) {
    int mid_index = (right+left) >> 1;
    while(true) {
        int pivot = partition(array, left, right, d);
        // return if pivot is already the mid number
        if (pivot == mid_index) {
            return pivot;
        }
        else {
            // Judge whether the pivot is greater or smaller than mid num
            if (pivot > mid_index) {
                right = pivot - 1;
            }
            else {
                left = pivot + 1;
            }
        }
    }
}

static long long answer = LLONG_MAX;

class KdTree {
private:
    int dimension = 2;
    Point points[100010];
    Node container[200020];
    int size;
public:
    KdTree() {
        //points = new Point[100010];
        //container = new Node[200020];
    }
    ~KdTree() {
        //delete[] points;
        //delete[] container;
    }
    void SetDim(int d) {
        dimension = d;
    }
    void Input(int number, int dimension) {
        for (int i = 0; i < number; i++) {
            points[i].input(dimension);
        }
    }
    void Build(int left_ptr, int right_ptr, int index, int cyc_dim) {
        // First, get the midnum of this section as root.
        size = index;
        Dimensions dim = (Dimensions)(cyc_dim % dimension);
        if (right_ptr - left_ptr < LEAF) {
            container[index].direction = dim;
            for (int i = left_ptr; i <= right_ptr; i++) {
                container[index].point[i-left_ptr] = points[i];
            }
            container[index].size = right_ptr - left_ptr + 1;
            container[index].is_leaf = true;
            return;
        }
        int mid = getmid(points, left_ptr, right_ptr, dim);
        // Basis
        container[index].point[0] = points[mid];
        container[index].direction = dim;
        Build(left_ptr, mid-1, (index << 1) + 1, cyc_dim+1);
        Build(mid+1, right_ptr, (index << 1) + 2, cyc_dim+1);
    }
    // void PrintTree() {
    //     int sign = 1;
    //     for (int i = 0; i <= size; i++) {
    //         cout << "index: " << i << ", " << container[i] << " ";
    //         if (i + 1 == sign) {
    //             cout << endl;
    //             sign <<= 1;
    //             sign += 1;
    //         }
    //     }
    // }
    long long LeastDistSquare(Point &query, int index) {
        
        if (container[index].is_leaf) {
            long long leaf_answer = LLONG_MAX;
            for (int i = 0; i < container[index].size; i++) {
#ifdef MYDEBUG
                cout << "Point: " << container[index].point[i] << " ";
#endif
                long long newdist = container[index].point[i].dist_square(query, dimension);
                leaf_answer = (leaf_answer < newdist) ? leaf_answer : newdist;
            }
#ifdef MYDEBUG
            cout << "Min length: " << answer << " ";
#endif
            answer = (answer < leaf_answer) ? answer : leaf_answer;
            return leaf_answer;
        }
#ifdef MYDEBUG
        cout << endl;
#endif
        Dimensions d = container[index].direction;
        if (query.getcomponent(d) <= container[index].point[0].getcomponent(d)) {
            long long left_answer = LeastDistSquare(query, (index << 1) + 1);
            answer = (answer < left_answer) ? answer : left_answer;
            long long least_dist = container[index].point[0].getcomponent(d) - query.getcomponent(d);
            least_dist *= least_dist;
            if (least_dist <= left_answer) {
                long long root_dist = container[index].point[0].dist_square(query, dimension);
                answer = (answer < root_dist) ? answer : root_dist;
    // #ifdef MYDEBUG
    //         cout << "Point: " << container[index].point[0] << " " << "dist_square: " << root_dist << endl; 
    // #endif
                long long right_answer = LeastDistSquare(query, (index << 1) + 2);
                answer = (answer < right_answer) ? answer : right_answer;
            }
        }
        else {
            long long right_answer = LeastDistSquare(query, (index << 1) + 2);
            answer = (answer < right_answer) ? answer : right_answer;
            long long least_dist = container[index].point[0].getcomponent(d) - query.getcomponent(d);
            least_dist *= least_dist;
            if (least_dist <= right_answer) {
                long long root_dist = container[index].point[0].dist_square(query, dimension);
                answer = (answer < root_dist) ? answer : root_dist;
    // #ifdef MYDEBUG
    //         cout << "Point: " << container[index].point[0] << " " << "dist_square: " << root_dist << endl; 
    // #endif
                long long left_answer = LeastDistSquare(query, (index << 1) + 1);
                answer = (answer < left_answer) ? answer : left_answer;
            }
        }

        return answer;
    }
};

class NearestNeighbor {
private:
    int dimension, number, query; //d, n, q
    Point queries[200010];
    KdTree mytree;
public:
    NearestNeighbor() {
        //queries = new Point[200010];
    };
    ~NearestNeighbor() {
        //delete[] queries;
    };
    void Input() {
        scanf("%d", &dimension);
        scanf("%d", &number);
        mytree.Input(number, dimension);
        scanf("%d", &query);
        for (int i = 0; i < query; i++) {
            queries[i].input(dimension);
        }
    }
    void BuildTree() {
        mytree.SetDim(dimension);
        mytree.Build(0, number-1, 0, 0);
#ifdef MYDEBUG
        mytree.PrintTree();
#endif
    }
    void QueryLeastDistanceSquare() {
        long long result = 0;
        for (int i = 0; i < query; i++) {
            result = mytree.LeastDistSquare(queries[i], 0);
            answer = LLONG_MAX;
            printf("%lld\n", result);
        }
    }
};

NearestNeighbor neighbor;

int main() {
    
    neighbor.Input();
    neighbor.BuildTree();
    neighbor.QueryLeastDistanceSquare();
    //cout << 0;
    return 0;
}