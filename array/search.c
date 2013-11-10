#include <array.h>
#include <types.h>
#include <search.h>

/*
 * Find an object in the array
 * @parm1 comparable_t: the object
 * @parm2 array_t: the array
 * @return int:
 *      true if the comparable item is in the array
 *      false if the comparable item is not in the array
 */
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
