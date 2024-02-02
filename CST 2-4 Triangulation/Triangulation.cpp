#include<iostream>
#include<stdio.h>
//#include <chrono>
using namespace std;

enum Sides {
    UP,
    DOWN,
    BORDER
};

struct points {
    long long x;
    long long y;
    Sides side;
};

int n;
points Vertices[1000010];
points Stack[1000010];
points sorted[1000010];
int stack_ptr = 0;

void cycle(int *ptr) {
    if (*ptr < 0) { *ptr = n-1; }
    if (*ptr >= n) { *ptr = 0; }
}

// There is a trap in this function: it aims to judge whether a particular angle is greater than pi.
// Note that division in integer can be very dangerous: the precision is awful especially when dealing
// with such problems with numbers ranging in a large scale.

bool less_than_pi(points vi, points vi_1, points vi_2) {
    long long slope_comp = (vi.y-vi_1.y)*(vi_1.x-vi_2.x) - (vi_1.y-vi_2.y)*(vi.x-vi_1.x);
    return ((slope_comp>0 && vi_1.side == DOWN) || (slope_comp<0 && vi_1.side == UP));
}

int main() {

    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%lld", &Vertices[i].x);
        scanf("%lld", &Vertices[i].y);
    }

    // First, we need to determine the Up side and the Down side of the polygon.
    // Note that the most left/right vertices belong to both the Up side and the Down side.
    points most_left, most_right;
    bool left_found = false, right_found = false;
    int left_rank, right_rank; // rank in Vertices
    for (int i = 0; i < n; i++) {
        if (i != 0 && i != n-1) {
            if (Vertices[i].x > Vertices[i-1].x && Vertices[i].x > Vertices[i+1].x) {
                most_right = Vertices[i];
                right_found = true;
                right_rank = i;
            }
            if (Vertices[i].x < Vertices[i-1].x && Vertices[i].x < Vertices[i+1].x) {
                most_left = Vertices[i];
                left_found = true;
                left_rank = i;
            }
        }
    }
    // There might be a possibility that most_left and most_right have rank 0 or n-1
    if (!left_found) {
        bool m = Vertices[0].x < Vertices[n-1].x;
        most_left = m ? Vertices[0] : Vertices[n-1];
        left_rank = m ? 0 : n-1;
    }
    if (!right_found) {
        bool m = Vertices[0].x > Vertices[n-1].x;
        most_right = m ? Vertices[0] : Vertices[n-1];
        right_rank = m ? 0 : n-1;
    }

    // In this problem, we need 2 basic data structures: A vector(array) of points in which the points are sorted by x, and arranged 
    // in an incremental order; A stack for determining when to form a triangle.
    
    sorted[0] = Vertices[left_rank];
    sorted[0].side = BORDER;
    int index = 1, ptr_1 = left_rank + 1, ptr_2 = left_rank - 1;
    cycle(&ptr_1); cycle(&ptr_2);
    // For the use of stack, we need to note down which side each vertice belongs to.
    Sides ptr_1_side, ptr_2_side;
    bool ptr_1_down = (Vertices[ptr_1].y < Vertices[ptr_2].y);
    ptr_1_side = ptr_1_down ? DOWN : UP;
    ptr_2_side = ptr_1_down ? UP : DOWN;

    // Fill in the array "sorted".
    while (index < n) {
        cycle(&ptr_1);
        cycle(&ptr_2);
        bool comp = Vertices[ptr_1].x < Vertices[ptr_2].x;
        sorted[index] = comp ? Vertices[ptr_1] : Vertices[ptr_2];
        sorted[index].side = comp ? ptr_1_side : ptr_2_side;
        comp ? ptr_1++ : ptr_2--;
        index++;
    }
    sorted[n-1].side = BORDER;

    // The main algorithm starts here. If stack_ptr = 0, then we have an empty stack. During each operation, the number
    // of elements in the stack either
    for (int i = 0; i < n; i++) {
        if (stack_ptr < 2) {
            // The stack is empty
            Stack[stack_ptr] = sorted[i];
            stack_ptr++;
        }   
        else {
            // Situation 1
            if (Stack[stack_ptr-1].side != sorted[i].side) {
                points top = Stack[stack_ptr-1];
                while (stack_ptr > 1) {
                    printf("%lld %lld %lld %lld ", sorted[i].x, sorted[i].y, 
                    Stack[stack_ptr-1].x, Stack[stack_ptr-1].y);
                    stack_ptr--;
                    printf("%lld %lld\n", Stack[stack_ptr-1].x, Stack[stack_ptr-1].y);
                }
                stack_ptr--;
                // push top and sorted[i]
                Stack[stack_ptr] = top;
                stack_ptr++;
                Stack[stack_ptr] = sorted[i];
                stack_ptr++;
            }
            else {
                bool is_less_than_pi = false;
                // Using slope to determine the angle
                is_less_than_pi = less_than_pi(sorted[i], Stack[stack_ptr-1], Stack[stack_ptr-2]);
                while (is_less_than_pi && stack_ptr > 1) {
                    printf("%lld %lld %lld %lld ", sorted[i].x, sorted[i].y, 
                    Stack[stack_ptr-1].x, Stack[stack_ptr-1].y);
                    stack_ptr--;
                    printf("%lld %lld\n", Stack[stack_ptr-1].x, Stack[stack_ptr-1].y);
                    if (stack_ptr > 1) {
                        is_less_than_pi = less_than_pi(sorted[i], Stack[stack_ptr-1], Stack[stack_ptr-2]);
                    }
                }
                Stack[stack_ptr] = sorted[i];
                stack_ptr++;
            }
        }
    }    

    return 0;
}