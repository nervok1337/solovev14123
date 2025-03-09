#include <iostream>
#include "Array.h"

using namespace std;

int main()
{
    Array arr(1);

    arr.insert(432);

    cout << arr[0] << '\n';

    arr.insert(12313);

    cout << arr[1] << '\n';

    cout << arr.getCapacity() << '\n';
    cout << arr.getSize() << '\n';

    return 0;
}
