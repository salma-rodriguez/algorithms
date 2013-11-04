#ifndef __SEARCH_H
#define __SEARCH_H

#include <array.h>
#include <types.h>
#include <search.h>

int locate(comparable_t obj, array_t arr)
{
	int low;
	int mid;
	int high;

	low = 0;
	high = arr->get_count(arr)-1;
	
	while (low <= high) {
		mid = low + (high - low) / 2;
		if (!(arr->compare(obj, arr->lookup(mid, arr))))
			return 1;
		else if (arr->compare(obj, arr->lookup(mid, arr)) < 0)
			high = mid - 1;
		else
			low = mid + 1;
	}

	return 0;
}

#endif /* __SEARCH_H */
