#pragma warning (disable:4996)
#include<iostream>
#include<stdio.h>
#include <limits.h>
//#define MYDEBUG
using namespace std;

struct Node {
    long long lazy_sign = -1; // Lazy sign
    long long reverse_time; // How many times it has been reversed
    int left;
    int right;
    bool is_leaf;
};

struct Operation {
    char op_type;
    int left;
    int right;
};

class SegmentTree {
private:
    Node* container;
    int size;
public:
    SegmentTree() {
        container = new Node[1000000];
    }

    ~SegmentTree() {
        delete[] container;
    }

    void Build(int left_ptr, int right_ptr, int index, int* endings) {
        // Open on the left, close on the right.
        // Basis
        container[index].left = endings[left_ptr];
        container[index].right = endings[right_ptr];
        container[index].lazy_sign = 0;
        container[index].reverse_time = 0;
        size = index;
        if (left_ptr == right_ptr - 1) {
            container[index].is_leaf = true;
            return;
        }
        else {
            container[index].is_leaf = false;
            // First, build the current interval
            int middle = left_ptr + ((right_ptr - left_ptr) >> 1);
            Build(left_ptr, middle, (index << 1) + 1, endings);
            Build(middle, right_ptr, (index << 1) + 2, endings);
        }
    }

    void PrintTree() {
        int sign = 1;
        for (int i = 0; i <= size; i++) {
            cout << "index: " << i << ", " << container[i].left << ' ' << container[i].right << " reversed: " << container[i].reverse_time << " lazy: " << container[i].lazy_sign;
            if (i + 1 == sign) {
                cout << endl;
                sign <<= 1;
                sign += 1;
            }
        }
    }

    int GetIntersect(Node node, int left, int right) {
        int l = node.left + 1 > left ? node.left + 1 : left;
        int r = node.right < right ? node.right : right;
        return r - l + 1 > 0 ? r - l + 1 : 0;
    }

    void Flip(int left, int right, int index) {
        // Note that left and right can both be flipped, but the Nodes in container is left open.
        // Flip starts from the root
        // Basis
        if (container[index].is_leaf) {
            container[index].reverse_time += GetIntersect(container[index], left, right);
            return;
        }
        if ((container[index].left + 1 >= left) && (container[index].right <= right)) {
            // The node's interval is in the query interval. To perform lazy operations, we store the reverse_time
            // and lazy_sign, and return immediately.
            container[index].reverse_time += container[index].right - container[index].left;
            container[index].lazy_sign += 1;
            return;
        }
        if ((container[index].right < left) || (container[index].left + 1 > right)) {
            // No intersection
            return;
        }
        container[index].reverse_time += GetIntersect(container[index], left, right);
        Flip(left, right, (index << 1) + 1);
        Flip(left, right, (index << 1) + 2);
    }

    void Lazy(int index) {
        long long lazy = container[index].lazy_sign;
        int lc = (index << 1) + 1;
        int rc = lc + 1;
        container[lc].lazy_sign += lazy;
        container[rc].lazy_sign += lazy;
        container[lc].reverse_time += lazy * (container[lc].right - container[lc].left);
        container[rc].reverse_time += lazy * (container[rc].right - container[rc].left);
        container[index].lazy_sign = 0;
    }

    long long Query(int left, int right, int index) {
        if (container[index].is_leaf) {
            int a = GetIntersect(container[index], left, right);
            long long b = container[index].reverse_time / (container[index].right - container[index].left) * a;
            return b;
        }
        if ((container[index].right < left) || (container[index].left + 1 > right)) {
            // No intersection
            return 0;
        }
        if ((container[index].left + 1 >= left) && (container[index].right <= right)) {
            // To fully perform laziness, we need to unleash the laziness til the node that contains in the query interval.
            return container[index].reverse_time;
        }
        // To deal with lazy recursively, update lc, rc's lazy_sign and reverse_time
        if (container[index].lazy_sign != 0) {
            Lazy(index);
        }
        return Query(left, right, (index << 1) + 1) + Query(left, right, (index << 1) + 2);
    }
};

void quick_sort(int* p, int n) {
    if (n <= 1)
        return;
    int t = p[n - 1], l = 0, r = n - 2;
    do {
        for (; r >= l; l++)
            if (p[l] > t)
                break;
        for (; r >= l; r--)
            if (p[r] < t)
                break;
        if (r >= l)
            swap(p[l], p[r]);
    } while (r >= l);
    swap(p[l], p[n - 1]);
    quick_sort(&p[0], l);
    quick_sort(&p[l + 1], n - l - 1);
}

void merge_sort(int* num, int l, int r)
{
    int middle = (l + r) / 2, * tmp, lp = l, rp = middle + 1;
    //when to return 
    if (l == r)
        return;
    //first, merge the left and right part
    merge_sort(num, l, middle);
    merge_sort(num, middle + 1, r);
    //create a new array to dispose number
    tmp = new int[r - l + 1];
    int flag = 0;
    while ((lp <= middle) && (rp <= r))
    {
        if (num[lp] <= num[rp])
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

class Kidd {
private:
    int* endings; // All the endings of H operations, used for building a segment tree.
    Operation* operations; // All the operations, including H and Q in a chronological order.
    SegmentTree mytree; // The segment tree we use to conduct our algorithm.
    int n, m, left, right;
    int endings_ptr = 0, op_ptr = 0; // ptrs of the 2 arrays.
    char op_type; // The operation to be executed each time.
public:
    Kidd() {
        endings = new int[400010];
        operations = new Operation[200000];
    }

    ~Kidd() {
        delete[] endings;
        delete[] operations;
    }

    void Input() {
        scanf("%d %d", &n, &m);
        for (int i = 0; i < m; i++) {
            scanf(" %c%d%d", &op_type, &left, &right);
            if (op_type == 'H') {
                endings[endings_ptr] = left - 1;
                endings_ptr++;
                endings[endings_ptr] = right;
                endings_ptr++;
            }
            operations[op_ptr].op_type = op_type;
            operations[op_ptr].left = left;
            operations[op_ptr].right = right;
            op_ptr++;
        }
        // At last, don't forget there should be 0 and 2^31-1 in endings.
        endings[endings_ptr] = 0;
        endings_ptr++;
        endings[endings_ptr] = INT_MAX;
        endings_ptr++;
    }

    void Preprocess() {
        // Sort endings, and deduplicate
        merge_sort(endings, 0, endings_ptr-1);
        int new_endings_ptr = 1;
        // deduplicate, note that endings_ptr might be 0 or 1. We don't need to deduplicate in such cases.
        if (endings_ptr > 1) {
            for (int i = 1; i < endings_ptr; i++) {
                if (endings[i] != endings[new_endings_ptr - 1]) {
                    endings[new_endings_ptr] = endings[i];
                    new_endings_ptr++;
                }
            }
            // Afterwards, update endings_ptr
            endings_ptr = new_endings_ptr;
        }
#ifdef MYDEBUG
        for (int i = 0; i < endings_ptr; i++) {
            cout << endings[i] << " ";
        }
        cout << endl;
#endif
    }

    void BuildTree() {
        mytree.Build(0, endings_ptr - 1, 0, endings);
    }

    void ReverseAndQuery() {
        for (int i = 0; i < op_ptr; i++) {
            if (operations[i].op_type == 'H') {
                // Reverse, use lazy method
                mytree.Flip(operations[i].left, operations[i].right, 0);
            }
            else {
                // Query
                printf("%lld\n", mytree.Query(operations[i].left, operations[i].right, 0));
            }
#ifdef MYDEBUG
            mytree.PrintTree();
#endif

        }
    }
};

int main() {
    // There are maximum 2^31 cards, so it's impossible to build a segment tree that contains such amount of nodes.
    // However, note that there's only 200,000 queries, so the number of different endings of segments is less than
    // 400,000. In this way, we can build a tree that only contains 400,000*2 - 1 nodes.
    Kidd kidd;
    kidd.Input();
    kidd.Preprocess();
    kidd.BuildTree();
    kidd.ReverseAndQuery();
    return 0;
}