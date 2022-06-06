#include <thrust/random.h>
#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/transform_reduce.h>
#include <thrust/functional.h>
#include <vector>
#include <iostream>
#include <vector>
#include <algorithm>

#define MATCH     2
#define MISMATCH -1
#define GAP      -1


struct Combination {
    int init_a;
    int init_b;
    int size;
};

std::vector<Combination> makeAllCombinations(int size_a, int size_b) {
    std::vector<Combination> combinations;
    for (int size = 1; size <= size_b; size++){
        for (int i_a = 0; i_a < size_a; i_a++){
            if (i_a + size <= size_a){
                for (int i_b = 0; i_b <= size_b; i_b++){
                    if (i_b + size <= size_b){
                        Combination comb = {.init_a = i_a,
                                            .init_b = i_b,
                                            .size = size};
                        combinations.push_back(comb);
                    }
                }
            }
        }
    }
    return combinations;
}

struct match_gpu
{

    __host__ __device__
    double operator()(const char& a, const char& b) {
            if (a == b)
            {
                return MATCH;
            }
            else if (a == '-' || b == '-')
            {
                return MISMATCH;
            }
            else
            {
                return GAP;
            }
    }
};

int main() {
    int size_a, size_b;
    std::vector<char> dna_a, dna_b;

    std::cin >> size_a >> size_b;

    std::cout << "DNA-A: ";
    for (int a = 0; a < size_a; a++){
        char tmp;
        std::cin >> tmp;
        dna_a.push_back(tmp);
        std::cout << tmp;
    }
    std::cout << "\nDNA-B: ";
    for (int b = 0; b < size_b; b++){
        char tmp;
        std::cin >> tmp;
        dna_b.push_back(tmp);
        std::cout << tmp;
    }
    std::cout << std::endl;

    if (size_a > size_b)
    {
        dna_a.swap(dna_b);
        std::swap(size_a, size_b);
    }

    std::vector<Combination> all_combinations = makeAllCombinations(size_a, size_b);

    thrust::device_vector<char> dna_a_gpu(dna_a);
    thrust::device_vector<char> dna_b_gpu(dna_b);
    thrust::device_vector<int> matches(size_b, 0);
    std::vector<int> scores;
    scores.resize(all_combinations.size());

    int best_score = 0;
    for (int i = 0; i < all_combinations.size(); i++){
        Combination kombi = all_combinations[i];

        thrust::transform(dna_b_gpu.begin() + kombi.init_b, dna_b_gpu.begin() + kombi.init_b + kombi.size,
                          dna_a_gpu.begin() + kombi.init_a, matches.begin(),
                          match_gpu());
        int score = thrust::reduce(matches.begin(), matches.end(), (int) 0, thrust::plus<int>());
        if (score > best_score){
            best_score = score;
        }
        scores[i] = score;
    }

    std::vector<int>::iterator itr = std::find(scores.begin(), scores.end(), best_score);
    std::vector<char> sub_a, sub_b;

    if (itr != scores.cend()) {
        int index = std::distance(scores.begin(), itr);
        sub_a = std::vector<char>(dna_a.begin() + all_combinations[index].init_a, 
                                  dna_a.begin() + all_combinations[index].init_a + all_combinations[index].size);
        sub_b = std::vector<char>(dna_b.begin() + all_combinations[index].init_b, 
                                  dna_b.begin() + all_combinations[index].init_b + all_combinations[index].size);

        std::cout << "Sub A: ";
        for (int ca = 0; ca < sub_a.size(); ca++){
            std::cout << sub_a[ca];
        }
        std::cout << std::endl;

        std::cout << "Sub B: ";
        for (int cb = 0; cb < sub_b.size(); cb++){
            std::cout << sub_b[cb];
        }
        std::cout << std::endl;

        std::cout << "Score: " << best_score << std::endl;
    }
    else {
        std::cout << "Element not found";
    }



}