#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <tuple>
#include <fstream>
#include <random>

#define MATCH     2
#define MISMATCH -1
#define GAP      -1

using namespace std;

int match(char a, char b)
{
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

int retScore(string subA, string subB, int k)
{
    int score = 0;
    for (int i = 0; i < k; i++)
    {
        score += match(subA[i], subB[i]);
    }

    return score;
}

struct seqScore
{
    string sa;
    string sb;
    int score;
};

int main(){

    int n, m, i, j, k, p;
    string a, b;
    vector<seqScore> bestSequences;

    ofstream file;
    file.open("outputLocal.txt");

    cin >> n >> m;
    cin >> a >> b;

    file << "n: " << n << endl;
    file << "m: " << m << endl;
    file << endl;
    file << "DNA-A: " << a << endl;
    file << "DNA-B: " << b << endl;
    file << endl;
    if (n > m)
    {
        a.swap(b);
        swap(n,m);
    }
    random_device seeed;
    uniform_int_distribution<int> distP(1, n+m);
    p = distP(seeed);

    int score = 0;
    seqScore seq = {.sa = "", 
                    .sb = "", 
                    .score = 0
                    };
                    
    int max_score = -std::max(n,m);

// #pragma omp parallel for
    for (int l = 0; l < 50; l++)
    {   
        uniform_int_distribution<int> distK(1, n);
        k = distK(seeed);
        // cout<< "k: " << k << endl;
        uniform_int_distribution<int> distJ(0, m-k);
        j = distJ(seeed);
        // cout<< "j: " << j << endl;
        string sb;

        sb = b.substr(j, k);
        p = n-k+1;
        
        // AAT
        // BBCAT
#pragma omp parallel for
        for (i = 0; i < p; i++)
        {
            string sa;
            // cout<< "i: " << i;
            sa = a.substr(i,k);
            score = retScore(sa, sb, k);

#pragma omp critical
            if (score > max_score)
            {   
                bestSequences.clear();
                max_score = score;
                seq.sa = sa;
                seq.sb = sb;
                seq.score = score;
                bestSequences.push_back(seq);
            } 
            else if (score == max_score)
            {
                seq.sa = sa;
                seq.sb = sb;
                seq.score = score;
                bestSequences.push_back(seq);
            }
        }

    }


    for (auto& el : bestSequences){
        file << "Sequences:" <<       endl;
        file << "\tA: "      << el.sa << endl;
        file << "\tB: "      << el.sb << endl;
        file << "\tScore: "  << el.score << endl;
        file << "---------------------------------" << endl;
    }
}