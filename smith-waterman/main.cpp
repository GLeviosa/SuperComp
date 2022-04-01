#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <tuple>
#include <fstream>

#define DIAGON 1
#define DELETION  2
#define INSERTION 3

using namespace std;


struct alignment
{
    int row;
    int col;
    int value;
    int direction;
};

struct finalAnswer{
    string dnaA;
    string dnaB;
    int score;
};

struct matrixAndMax{
    vector < vector <alignment>> matrix;
    alignment max;
};

int match(char a, char b)
{
    if (a == b)
    {
        return 2;
    }
    else
    {
        return -1;
    }
}


int main(){
    string dnaA,dnaB;
    int n,m;
    ofstream file;
    file.open("output.txt");

    cin >> n >> m;
    cin >> dnaA >> dnaB;

    file << "n: " << n << endl;
    file << "m: " << m << endl;
    file << endl;
    file << "DNA-A: " << dnaA << endl;
    file << "DNA-B: " << dnaB << endl;
    file << endl;

    int diagonal, delecao, insercao, w;
    alignment globalMax = {.value = 0};

    vector<vector<alignment>> H;
    H.resize(n+1);

    for(int row = 0; row < n+1; row++){
        H[row].resize(m+1);
    }

    // Monta matriz
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            H[i][j].row = i;
            H[i][j].col = j;

            w = match(dnaA[i-1], dnaB[j-1]);

            diagonal = H[i-1][j-1].value + w;
            delecao = H[i-1][j].value - 1;
            insercao = H[i][j- 1].value - 1;

            int max = std::max({0, diagonal, delecao, insercao});
            H[i][j].value = max;

            
            
            if (max == diagonal || max == 0)
            {
                H[i][j].direction = DIAGON;
            }
            else if (max == delecao)
            {
                H[i][j].direction = DELETION;
            }
            else
            {
                H[i][j].direction = INSERTION;
            }

            if (max >= globalMax.value){
                globalMax = H[i][j];
            }

        }
    }

    string dnaAMatch, dnaBMatch;
    vector<alignment> path;
    alignment current, next;
    current = globalMax;

    while(current.value != 0){
        int i = current.row, j = current.col;
        if (current.direction == DIAGON)
        {
            dnaAMatch += dnaA[i-1];
            dnaBMatch += dnaB[j-1];
            next = H[i-1][j-1];
        }
        else if (current.direction == DELETION)
        {
            dnaAMatch += dnaA[i-1];
            dnaBMatch += "_";
            next = H[i-1][j];
        }
        else
        {
            dnaAMatch += "_";
            dnaBMatch += dnaB[j-1];
            next = H[i][j-1];;
        }
        current = next;
    }

    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= m; j++)
        {
            file << H[i][j].value << " ";
        }
            
        // Newline for new row
        file << endl;
    }
    reverse(dnaAMatch.rbegin(), dnaAMatch.rend());
    reverse(dnaBMatch.rbegin(), dnaBMatch.rend());
    finalAnswer answer = {.dnaA = dnaAMatch, 
                          .dnaB = dnaBMatch, 
                          .score = globalMax.value};

    file << endl;
    file << "Score:" << answer.score << endl;
    file << endl;
    file << "Sequence A:" << answer.dnaA << endl;
    file << endl;
    file << "Sequence B:" << answer.dnaB << endl;
    

}