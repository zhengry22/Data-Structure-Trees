#include<iostream>
using namespace std;

int search_iterative(int* num, int l, int r, int obj)
{
	while ((l < r)&&(num[l]!=obj)&&(num[r]!=obj))
	{
		int mid = (l + r) / 2;
	    if (obj >= num[mid])
		{
			r = mid;
		}
		else
		{
			l = mid + 1;
		}
	}
	if (num[l] == obj)
		return l;
	if (num[r] == obj)
		return r;
	if (l == r){
        return l;
    }
}

int main() {
    int a[18]= {10,8,7,7,7,7,7,5,4,3,2,2,2,1,1,1};
    cout << search_iterative(a, 5, 17, 6);
    return 0;
}