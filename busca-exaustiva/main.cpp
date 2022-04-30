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
        return GAP;
    }
    else
    {
        return MISMATCH;
    }
}

int retScore(string subA, string subB)
{
    int score = 0;
    int size = subA.length();

    for (int i = 0; i < size; i++)
    {
        score += match(subA[i], subB[i]);
    }

    return score;
}

vector<string> makeSubsets(string str)
{
    vector<string> subsets;
    int size = str.length();
    for (int i = 0; i <= size; i++)
    {
        for (int j = 1; j <= size - i; j ++)
        {
            subsets.push_back(str.substr(i, j));
        }
    }
    return subsets;
}

struct seqScore
{
    string sa;
    string sb;
    int score;
};

int main(){

    ofstream file;
    file.open("outputExaustiva.txt");

    int size_a, size_b;
    string a, b;

    cin >> size_a >> size_b;
    cin >> a >> b;

    if (size_b > size_a)
    {
        a.swap(b);
        swap(size_a,size_b);
    }

    file << "n: " << size_a << endl;
    file << "m: " << size_b << endl;
    file << endl;
    file << "DNA-A: " << a << endl;
    file << "DNA-B: " << b << endl;
    file << endl;

    vector<string> subsetsA = makeSubsets(a);
    vector<string> subsetsB = makeSubsets(b);

    int score = -size_a;
    int bestScore = -size_a;

    string bestA, bestB, bigger, smaller;

    for (auto &sub_a : subsetsA)
    {
        for (auto &sub_b : subsetsB)
        {
            if (sub_a.length() == sub_b.length())
            {
                score = retScore(sub_a, sub_b);
                if (score > bestScore)
                {
                    bestScore = score;
                    bestA = sub_a;
                    bestB = sub_b;
                }
            }
        }
    }

    file << "A: "      << bestA << endl;
    file << "B: "      << bestB << endl;
    file << "Score: "  << bestScore << endl;
    file << "---------------------------------" << endl;
}