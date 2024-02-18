#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<cstdlib>
#include<iomanip>
#include<vector>
#include<algorithm>

using namespace std;

const int M = 8;
int N = 6;
string field[M][M];
int value[M][M];

int list[11];

void renew(){
        for (int i = 0; i < 10; i++)
                list[i] = i;
        list[10] = 0;
}

struct vconstrain{
        vconstrain(const string& source, int i, int j){
                s = source;
                col = j;
                begin = i + 1;

                end = begin;
                while(end < N && field[end][j].size() == 1) end++;
        }

        void update(){
                q = 0;
                for (int i = 0; i < s.size(); i++){
                        q *= 10;
                        q += list[s[i] - 'A'];
                }
                for (int j = begin; j < end; j++)
                        if (value[j][col] <= 0 && value[j][col] != -10)
                                q -= list[-value[j][col]];

        }

        int evaluate(){
                int res = q;
                int mx = q;
                for (int j = begin; j < end; j++)
                        if (value[j][col] > 0){
                                res -= value[j][col];
                                mx  -= value[j][col];
                        }else if (value[j][col] == -10){
                                mx -= 9;
                                res--;
                        }
                if (mx <= 0)
                        return res;
                else return -1;
        }

        bool testUniq(){
                if (list[s[0] - 'A'] == 0) return false;

                vector<int> x;
                for (int j = begin; j < end; j++)
                        if (value[j][col] > 0)
                                x.push_back(value[j][col]);
                        else
                                x.push_back(list[-value[j][col]]);
                sort(x.begin(), x.end());

                if (x[0] == 0) return false;
                for (int i = 1; i < x.size(); i++)
                        if (x[i] == x[i-1]) return false;
                return true;
        }

        private:
                string s;
                int q;
                vector<int> pos;
                int col;
                int begin, end;
};

struct hconstrain{
                hconstrain(const string& source, int i, int j){
                        s = source;
                        row = i;
                        begin = j + 1;

                        end = begin;
                        while(end < N && field[i][end].size() == 1)end++;

                }

                void update(){
                        q = 0;
                        for (int i = 0; i < s.size(); i++){
                                q *= 10;
                                q += list[s[i] - 'A'];
                        }
                        for (int j = begin; j < end; j++)
                                if (value[row][j] <= 0 && value[row][j] != -10)
                                        q -= list[-value[row][j]];
                }

                int evaluate(){
                        int res = q;
                        int mx = q;
                        for (int j = begin; j < end; j++)
                                if (value[row][j] > 0){
                                        res -= value[row][j];
                                        mx -= value[row][j];
                                }
                                else if (value[row][j] == -10){
                                        mx -= 9;
                                                               res--;
                                }
                        if (mx <= 0)
                                return res;
                        else return -1;

                }

                bool testUniq(){
                        if (list[s[0] - 'A'] == 0) return false;
                        vector<int> x;
                        for (int j = begin; j < end; j++)
                                if (value[row][j] > 0)
                                        x.push_back(value[row][j]);
                                else
                                        x.push_back(list[-value[row][j]]);

                        sort(x.begin(), x.end());

                        if (x[0] == 0) return false;
                        for (int i = 1; i < x.size(); i++)
                                if (x[i] == x[i-1]) return false;
                        return true;
                }

        private:
                string s;
                int q;
                vector<int> pos;
                int row;
                int begin, end;
};

vector<hconstrain> hc;
vector<vconstrain> vc;

bool testConfig(){
        for (int i = 0; i < hc.size(); i++)
                if (hc[i].evaluate() < 0) return false;
                                                         
        for (int i = 0; i < vc.size(); i++)
                if (vc[i].evaluate() < 0) return false;
        return true;
}

bool testFull(){
        for (int i = 0; i < hc.size(); i++)
                if (hc[i].evaluate() != 0 || !(hc[i].testUniq())) return false;
        for (int i = 0; i < vc.size(); i++)
                if (vc[i].evaluate() != 0 || !(vc[i].testUniq())) return false;

        return true;
}

bool test(int i, int j){
     if (i > N) return test(0, j + 1);
     if (j > N) return testFull();

     if (field[i][j] != "O") return test(i+1, j);

     for (int v = 1; v < 10; v++){
             value[i][j] = v;
             if (testConfig() && test(i+1, j)) return true;
     }

     value[i][j] = -10;
     return false;
}

void printField(){

        for (int i = 0; i < N; i++){
                for (int j = 0; j < N; j++){
                        cout << setw(10) << field[i][j] << "\t";
                }
                cout << "\n";
        }
}

void analyzeField(){
        hc.clear();
        vc.clear();
        renew();
        for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++){
                        if (field[i][j] == "") continue;

                        if (field[i][j].size() == 1 && field[i][j][0] >= 'A' && field[i][j][0] <= 'J') value[i][j] = 'A' - field[i][j][0];
                        if (field[i][j][0] == '('){
                                string s = field[i][j];
                                s.erase(0,1);
                                if (s[0] == 'h'){
                                        int k = 1;
                                        while (s[k] != ',' and s[k] != ')') k++;
                                        hconstrain h(s.substr(1, k-1), i, j);
                                        s.erase(0, k+1);
                                        hc.push_back(h);
                                }
                                if (!s.size()) continue;
                                vconstrain v(s.substr(1, s.size() - 2), i, j);
                                vc.push_back(v);
                        }
                }
}

void read(string& s){
        s.erase(s.size() -1);
        switch(s[0]){
                case '6': N = 6; break;
                case '7': N = 7; break;
                default:
                        cerr <<"Wrong size information: " << s[0] <<".\nExiting...";
                        exit(0);
        }
        s.erase(0, 2);
        for (int i = 0; i < M; i++)
                for (int j = 0; j < M; j++)
                        field[i][j].clear(), value[i][j] = -10;

        int c = 0;
        for (int i = 0; i < s.size(); i++)
                switch(s[i]){
                        case ',':
                                if (!c) s[i] =';';
                                break;
                        case '(':
                                c++;
                                break;
                        case ')':
                                c--;
                }

        stringstream ss(s);

        for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++){
                        getline(ss, field[i][j], ';');
                        if (field[i][j] == "X") field[i][j] = "";
                }

}


long long getNumber(string s){
        static int kk = 0;
        read(s);
        analyzeField();

        do{
                for (int i = 0; i< vc.size(); i++)
                        vc[i].update();
                for (int i = 0; i < hc.size(); i++)
                        hc[i].update();

                if (test(0, 0)){
                        long long res = 0;
                        for (int i = 0; i < 10; i++){
                                res *= 10;
                                res += list[i];
                        }
                        cerr << ++kk << ":  " << res << endl;
                        return res;
                }

        }while(next_permutation(list, list+10));

        cerr <<  ++kk << ":  " << 0 << endl;
        return 0;
}


int main(){
        ifstream f("kakuro200.txt");
        long long sum = 0;

        for (int i = 0; i < 200; i++){
                string s;
                getline(f, s);
                sum += getNumber(s);
        }

        cerr << sum << endl;
}  