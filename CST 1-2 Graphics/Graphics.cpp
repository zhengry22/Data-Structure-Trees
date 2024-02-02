#include<iostream>
using namespace std;

int n, m;
long long *seg_x, *seg_y;
long long *x, *y;

void quick_sort(long long* p, int n) {
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

int ToLeft(long long seg_x, long long seg_y, long long x, long long y){
    long long toleft = seg_y*x + seg_x*y - seg_y*seg_x;
    if(toleft > 0){
        //toright
        return -1;
    }
    else if(toleft == 0){
        //on the line
        return 0;
    }
    else{
        //toleft
        return 1;
    }
}

bool FallsIn(int num, long long x, long long y){// the point falls in section num
    if(num==0 && ToLeft(seg_x[0], seg_y[0], x, y)>0){
        return true;
    }
    if(num==n && ToLeft(seg_x[n-1], seg_y[n-1], x, y)<=0){
        return true;
    }
    if(num!=0 && num!=n && ToLeft(seg_x[num-1], seg_y[num-1], x, y)<=0 && ToLeft(seg_x[num], seg_y[num], x, y)>0){
        return true;
    }
    return false;
}

int search_iterative(long long* seg_x, long long* seg_y, int l, int r, long long x, long long y)
{
	while ((l < r)&&(//there are intersections
        !FallsIn(l, x, y)
    )&&(//but not with all segments
        !FallsIn(r, x, y)
    )){
		int mid = (l + r) / 2;
        //if segment
	    if (ToLeft(seg_x[mid], seg_y[mid], x, y)>0){
			r = mid;
		}
		else{
			l = mid + 1;
		}
	}
	if (FallsIn(l, x, y))
		return l;
	if (FallsIn(r, x, y))
		return r;
	if (l == r)
		return -1;
}

int main(){
    scanf("%d", &n);
    seg_x = new long long[n];
    seg_y = new long long[n];
    for(int i=0; i<n; i++){
        scanf("%lld", &seg_x[i]);
    }
    for(int i=0; i<n; i++){
        scanf("%lld", &seg_y[i]);
    }
    scanf("%d", &m);
    x = new long long[m];
    y = new long long[m];
    for(int i=0; i<m; i++){
        scanf("%lld", &x[i]);
        scanf("%lld", &y[i]);
    }
    //sort seg_x, seg_y
    quick_sort(seg_x, n);
    quick_sort(seg_y, n);
    //bin_search
    for(int i=0; i<m; i++){
        printf("%d\n", search_iterative(seg_x, seg_y, 0, n, x[i], y[i]));
    }
    delete[] seg_x;
    delete[] seg_y;
    delete[] x;
    delete[] y;
    return 0;
}