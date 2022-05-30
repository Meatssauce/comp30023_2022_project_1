#include <stdio.h>
#include <assert.h>

int isin(int *arr, int element, int last) {
    for (int i=0; i<=last; i++) {
        if (arr[i] == element) {
            return 1;
        }
    }
    return 0;
}

int *dappendint(int *arr, int *last, int element, int *size) {
    // realloc if size exceeds limit
    if (last >= size) {
        *size *= 2;
        int *new_arr = realloc(arr, *size * sizeof(int));
        assert(new_arr);
        arr = new_arr;
    }
    arr[*last++] = element;

    return arr;
}

int main() {
    int arr_size = 100;
    int arr_last = 0;
    int *arr = (int *) malloc(arr_size * sizeof(int));
    
    arr = dappendint(arr, &arr_last, 9, &arr_size);
    arr = dappendint(arr, &arr_last, 8, &arr_size);
    
    printf("%d\n", arr[1]);
    
    return 0;
}