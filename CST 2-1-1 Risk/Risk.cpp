#include<iostream>
#include<stdio.h>
#include<assert.h>
using namespace std;

struct queue_elem{
    long long num;
    int index;
};

long long diagnose[1000020]; // Numbers of people diagnosed DIVOC everyday.
queue_elem queue[1000010]; // The data strcture we use to represent a queue, using which we can get the greatest element in O(1) via queue_head.
int queue_head = 1, queue_tail = 1; // The index of head and tail of the queue.
long long retrack[1000010]; // Numbers of days to retrack every day
long long largest[1000010]; // The largest result of each query
long long low_bound[100010]; // Boundaries of low risk and mid risk
long long high_bound[100010]; // Boudaries of mid risk and high risk
long long buckets[2000010]; // Considering the diagnosis everyday doesn't exceed 2*10^6, bucket sort would
// be a time-saving plan.
long long bucket_prefix_sums[2000010]; // The sum of the first i buckets

int main() {
    int n, queries;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &diagnose[i]);
    }
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &retrack[i]);
    }
    scanf("%d", &queries);
    for (int i = 1; i <= queries; i++) {
        scanf("%lld", &low_bound[i]);
        scanf("%lld", &high_bound[i]);
    }
    for (int i = 1; i <= n; i++) {
        // When i = 1, there should be nothing in the queue.
        if (i == 1) {
            // Special cases
            largest[i-1] = 0;
        }
        else {
            // Enqueue
            queue[queue_tail].num = diagnose[i-1];
            queue[queue_tail].index = i-1;
            queue_tail++;
            while (queue_tail - queue_head > 1 && queue[queue_tail-1].num > queue[queue_tail-2].num) {
                queue[queue_tail-2] = queue[queue_tail-1];
                queue_tail--;
            }
            // In this way, we guarantee the elements in the queue form a non-increasing sequence.
            
            // Dequeue 
            while(queue[queue_head].index < i-retrack[i]) {
                queue_head++;
            }
            // Now, the status of i is formed! By calling queue[queue_head], we can get the greatest element
            // in the queue.
            largest[i-1] = queue[queue_head].num;
        }
    }

    // Sort largest using bucket sort. buckets[i] stands for the number of days with i diagnosis.
    for (int i = 0; i < n; i++) {
        buckets[largest[i]]++;
    }

    // After constructing buckets, scan the buckets to form all the prefix-sums.
    for (int i = 0; i < 2000001; i++) {
        bucket_prefix_sums[i] = (i==0) ? buckets[i] : bucket_prefix_sums[i-1]+buckets[i];
    }

    for (int i = 1; i <= queries; i++) {
        long long low = (low_bound[i]>2000000) ? 2000001 : low_bound[i];
        long long high = (high_bound[i]>2000000) ? 2000001 : high_bound[i];
        long long l_risk = (low==0) ? 0 : bucket_prefix_sums[low-1];
        long long h_risk = bucket_prefix_sums[high-1] - l_risk;
        printf("%lld %lld\n", l_risk, h_risk);
    }
    return 0;
}