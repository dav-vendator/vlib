#include <iostream>
#include <cassert>
#include <vector>
#include <cstddef>


bool next_index(std::vector<std::size_t>& index, 
                 const std::vector<std::size_t>& shape)  {
        for (std::size_t dim  = shape.size(); dim-- > 0;){
            ++index[dim];
            if (index[dim] < shape[dim])
                return true;
            index[dim] = 0;
        }
        return false;

}

void print(std::vector<std::size_t>& vec){
    for (std::size_t i=0; i < vec.size(); i++)
        std::cout<<vec[i]<<" ";
    std::cout<<"\n";
}


int main() {

    std::vector<std::size_t> shape ({4,2,2});
    std::vector<std::size_t> index({0,0,0});
    std::size_t sr = 1;
    std::cout<<sr<<": ";
    print(index);
    sr += 1;
    while (next_index(index, shape)){
        std::cout<<sr<<": ";
        print(index);
        sr+=1;
    }

    return 0;
}