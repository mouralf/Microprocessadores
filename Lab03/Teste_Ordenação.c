#include <stdio.h>


int main(){
    unsigned char arr[] = { 12, 11, 13, 5, 6 };
    unsigned char n = 8;
		
		int i, key, j;
    
		for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    while(1);
}