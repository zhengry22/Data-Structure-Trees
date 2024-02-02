#pragma warning (disable:4996)
#include <stdio.h>
#include <iostream>
#include <limits.h>
using namespace std;
#define LEAF 1
//#define MYDEBUG

// Up to 5 dimensions
enum Dimensions {
    D1, D2, D3, D4, D5,
};

// The basic data structures
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

struct Node {
    Point point[LEAF];
    Dimensions direction;
    int size = 1;
    bool is_leaf = false;
    friend std::ostream& operator<<(std::ostream& os, const Node& obj) {
        for (int i = 0; i < obj.size; i++) {
            os << "point" << i << ": " << obj.point[i] << ' ';
        }
        return os;
    }
};

void merge_sort(Point* num, int l, int r, Dimensions d)
{
    int middle = (l + r) / 2,  lp = l, rp = middle + 1;
    Point * tmp;
    //when to return 
    if (l == r)
        return;
    //first, merge the left and right part
    merge_sort(num, l, middle, d);
    merge_sort(num, middle + 1, r, d);
    //create a new array to dispose number
    tmp = new Point[r - l + 1];
    int flag = 0;
    while ((lp <= middle) && (rp <= r))
    {
        if (num[lp].getcomponent(d) <= num[rp].getcomponent(d))
        {
            tmp[flag] = num[lp];
            lp++;
        }
        else
        {
            tmp[flag] = num[rp];
            rp++;
        }
        flag++;
    }
    if (lp <= middle)
    {
        for (int i = lp; i <= middle; i++)
        {
            tmp[flag] = num[i];
            flag++;
        }
    }
    if (rp <= r)
    {
        for (int i = rp; i <= r; i++)
        {
            tmp[flag] = num[i];
            flag++;
        }
    }
    //put the numbers in tmp into num
    for (int i = 0, j = l; i < r - l + 1; i++, j++)
    {
        num[j] = tmp[i];
    }
    delete[] tmp;
}

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

class KdTree {
private:
    int dimension = 2;
    Point* points;
    Node* container;
    int size;
public:
    KdTree() {
        points = new Point[100010];
        container = new Node[200020];
    }
    ~KdTree() {
        delete[] points;
        delete[] container;
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
        merge_sort(points, left_ptr, right_ptr, dim);
        //int mid = getmid(points, left_ptr, right_ptr, dim);
        int mid = (left_ptr + right_ptr) >> 1;
        // Basis
        container[index].point[0] = points[mid];
        container[index].direction = dim;
        Build(left_ptr, mid-1, (index << 1) + 1, cyc_dim+1);
        Build(mid+1, right_ptr, (index << 1) + 2, cyc_dim+1);
    }
    void PrintTree() {
        int sign = 1;
        for (int i = 0; i <= size; i++) {
            cout << "index: " << i << ", " << container[i] << " ";
            if (i + 1 == sign) {
                cout << endl;
                sign <<= 1;
                sign += 1;
            }
        }
    }
    long long LeastDistSquare(Point &query, int index) {
        long long answer = 0;
        if (container[index].is_leaf) {
            long long answer = LLONG_MAX;
            for (int i = 0; i < container[index].size; i++) {
#ifdef MYDEBUG
                cout << "Point: " << container[index].point[i] << " ";
#endif
                long long newdist = dist_square(container[index].point[i], query, dimension);
                answer = (answer < newdist) ? answer : newdist;
            }
#ifdef MYDEBUG
            cout << "Min length: " << answer << " ";
#endif
            return answer;
        }
#ifdef MYDEBUG
        cout << endl;
#endif
        Dimensions d = container[index].direction;
        if (query.getcomponent(d) <= container[index].point[0].getcomponent(d)) {
            long long left_answer = LeastDistSquare(query, (index << 1) + 1);
            answer = left_answer;
            long long least_dist = container[index].point[0].getcomponent(d) - query.getcomponent(d);
            least_dist *= least_dist;
            if (least_dist <= left_answer) {
                long long root_dist = dist_square(container[index].point[0], query, dimension);
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
            answer = right_answer;
            long long least_dist = container[index].point[0].getcomponent(d) - query.getcomponent(d);
            least_dist *= least_dist;
            if (least_dist <= right_answer) {
                long long root_dist = dist_square(container[index].point[0], query, dimension);
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
    Point* queries;
    KdTree mytree;
public:
    NearestNeighbor() {
        queries = new Point[200010];
    };
    ~NearestNeighbor() {
        delete[] queries;
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
        long long answer = 0;
        for (int i = 0; i < query; i++) {
            answer = mytree.LeastDistSquare(queries[i], 0);
            printf("%lld\n", answer);
        }
    }
};

int main() {
    NearestNeighbor neighbor;
    neighbor.Input();
    neighbor.BuildTree();
    neighbor.QueryLeastDistanceSquare();
    return 0;
}