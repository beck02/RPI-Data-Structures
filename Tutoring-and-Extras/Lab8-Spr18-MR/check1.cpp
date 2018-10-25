#include <iostream>

int paths(int x, int y){
    if (x <= 0 || y <= 0) {
        return 1;
    }
    else {
        return paths(x-1,y) + paths(x,y-1);
    }
}

int main(int argc, char const *argv[]) {
    std::cout << "==========Testing recursion==========" << '\n';
    int x = 3;
    int y = 3;
    int num_paths = paths(x,y);
    std::cout << "Number of paths from (" << x << ", " << y << "): " << num_paths << std::endl;
    std::cout << "=====================================" << '\n';

    return 0;
}
