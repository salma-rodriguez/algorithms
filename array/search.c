#include <array.h>
#include <search.h>

int find(void *key, struct array_list *arr)
{
	int low;
	int mid;
	int high;

	low = 0;
	high = arr->count-1;
	
	while (low <= high) {
		mid = low + (high - low) / 2;
		if (!(arr->compare(key, arr->array[mid])))
			return 1;
		else if (arr->compare(key, arr->array[mid]) < 0)
			high = mid - 1;
		else
			low = mid + 1;
	}

	return 0;
}
