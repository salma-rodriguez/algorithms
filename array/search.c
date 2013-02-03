#include <array.h>
#include <search.h>

int find(int key, int arr[])
{
	int low;
	int mid;
	int high;

	low = 0;
	high = length(arr);
	
	while (low <= high) {
		mid = low + (high - low) / 2;
		if (key == arr[mid])
			return 1;
		else if (key < arr[mid])
			high = mid - 1;
		else
			low = mid + 1;
	}

	return 0;
}
