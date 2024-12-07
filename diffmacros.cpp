#include<bits/stdc++.h>
using namespace std;

#define num 2

#define SIZE 5

#define Multiple(r) ((r) * (r))


int main() {
    cout << "Number is : " << num << endl;
    cout << "multiple of num is : " << Multiple(num) << endl;


    int arr[SIZE] = {1, 2, 3, 4, 5};
    cout << "Array elements are: ";
    for(int i = 0; i < SIZE; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
