#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
#include <utility>

using namespace std;

void output(vector<string>);
bool is_num(string);
void remove_redundant(vector<string>);
void temp_var_replace(vector<string>);
void fold_and_propogate(vector<string>);

int main(int argc, char **argv)
{
    //int number = atoi(argv[1]);
    vector<string> code;
    string s;

    ifstream ip ("../../test.icg");    
    if(ip.is_open()) {
        while(getline(ip, s)) {
            code.push_back(s);
        }
    }
    remove_redundant(code);

    s = "";
    code.clear();
    ifstream p1 ("../../tests/outputs/pass1");    
    if(p1.is_open()) {
        while(getline(p1, s)) {
            code.push_back(s);
        }
    }
    temp_var_replace(code);

    s = "";
    code.clear();
    ifstream p2 ("../../tests/outputs/pass2");    
    if(p2.is_open()) {
        while(getline(p2, s)) {
            code.push_back(s);
        }
    }
    fold_and_propogate(code);

    return 0;
}

void remove_redundant(vector<string> code)
{
    ofstream op1;
    op1.open ("../../tests/outputs/pass1");

    vector<string> first;
    stringstream ss(code[0]);
    string tok;
    while(getline(ss, tok, ' ')) {
        first.push_back(tok);
    }
    
    for(int i = 1; i < code.size(); ++i) {
        vector<string> second;
        stringstream ss(code[i]);
        string tok;
        while(getline(ss, tok, ' ')) {
            second.push_back(tok);
        }

        if(second[1][0] == '=') {
            if(!(first[0].compare(second[2])) && second[0][0] == 'i') {
                op1 << second[0] << " ";
                for(int q = 1; q < first.size(); ++q) {
                    op1 << first[q] << " ";
                }   
                op1 << "\n";
                i = i + 1;
                second.clear();
                stringstream ss(code[i]);
                string tok;
                while(getline(ss, tok, ' ')) {
                    second.push_back(tok);
                }

            } else {
            for(string s: first)    op1 << s << " ";
            op1 << "\n";
            }        
        } else {
            for(string s: first)    op1 << s << " ";
            op1 << "\n";
        }
        first = second;   
    }
    for(string s: first)    op1 << s << " ";
    op1 << "\n";
    op1.close();
}

void temp_var_replace(vector<string> code)
{
    ofstream op2;
    op2.open ("../../tests/outputs/pass2");

    vector<pair<int,int>> temp_var_corr;
    for(int i = 0; i < code.size(); ++i) {
        vector<string> parts;
        stringstream ss(code[i]);
        string tok;
        while(getline(ss, tok, ' ')) {
            parts.push_back(tok);
        }
        if(parts.size() == 3) {
            if(parts[0][0] == 'T' && parts[2][0] == 'i') {
                int x1 = stoi(parts[0].substr(parts[0].find_first_of("0123456789")));
                int x2 = stoi(parts[2].substr(parts[0].find_first_of("0123456789")));
                temp_var_corr.push_back(make_pair(x1,x2));
            } else {
                op2 << code[i] << endl;
            }
        }
        else if(parts.size() == 5) {
            if(parts[2][0] == 'T' && parts[4][0] == 'T') {
                int i1_index = -1, i2_index = -1;
                int t1_index = stoi(parts[2].substr(parts[2].find_first_of("0123456789")));
                int t2_index = stoi(parts[4].substr(parts[4].find_first_of("0123456789")));
                for(vector<pair<int,int>>::iterator it = temp_var_corr.begin(); it != temp_var_corr.end(); ++it) {
                    if(t1_index == it->first) {
                        i1_index = it->second;
                    }
                    if(t2_index == it->first) {
                        i2_index = it->second;
                    }
                }
                op2 << parts[0] << " " << parts[1] << " ";
                i1_index != -1 ? op2 << "i" << i1_index << " " : op2 << parts[2] << " ";
                op2 << parts[3] << " "; 
                i2_index != -1 ? op2 << "i" << i2_index << " " : op2 << parts[4] << " ";
            } 
            op2 << endl;
        } else {
            op2 << code[i] << endl;
        }
        
    }

}

void fold_and_propogate(vector<string> code)
{
    ofstream op3;
    op3.open ("../../tests/outputs/pass3");

    vector<pair<int,float>> refer_t;
    vector<pair<int,float>> refer_i;
    for(int i = 0; i < code.size(); ++i) {
        vector<string> parts;
        stringstream ss(code[i]);
        string tok;
        while(getline(ss, tok, ' ')) {
            parts.push_back(tok);
        }

        if(parts.size() == 3) {
            if(parts[0][0] == 'i' /*&& is_num(parts[2])*/) {
                int x1 = stoi(parts[0].substr(parts[0].find_first_of("0123456789")));
                float x2 = stof(parts[2]);
                refer_i.push_back(make_pair(x1,x2));
            } else if(parts[0][0] == 'T' /*&& is_num(parts[2])*/) {
                int x1 = stoi(parts[0].substr(parts[0].find_first_of("0123456789")));
                float x2 = stof(parts[2]);
                refer_t.push_back(make_pair(x1,x2));
            }
            //refer.push_back(make_pair(parts[0],stof(parts[2])));
            
            /*for(int ii = 0; ii < refer_t.size(); ii++)
                cout << "\t\t" << refer_t[ii].first << ", " << refer_t[ii].second << endl;
            cout << "\t\t#####" << endl;;*/

        } else {
            op3 << parts[0] << " ";
            for(int a = 1; a < parts.size(); ++a) {
                if(parts[a][0] == 'i' || parts[a][0] == 'T') {
                    int x = stoi(parts[a].substr(parts[a].find_first_of("0123456789")));
                    bool found = false;
                    if(parts[a][0] == 'i') {    
                        for(vector<pair<int,float>>::iterator it = refer_i.begin(); it != refer_i.end(); ++it) {
                                if(x == it->first) {
                                    op3 << it->second << " ";
                                    found = true;
                                } 
                        }
                        if(!found)  op3 << parts[a] << " ";
                    } else {
                        for(vector<pair<int,float>>::iterator it = refer_t.begin(); it != refer_t.end(); ++it) {
                                if(x == it->first) {
                                    op3 << it->second << " ";
                                    found = true;
                                }
                        }
                        if(!found)  op3 << parts[a] << " ";
                    }
                } else {
                    op3 << parts[a] << " ";
                }   

            } 

            op3 << endl;
        } 

            /*if(parts[0][0] == 'i' && is_num(parts[2])) {
                int x1 = stoi(parts[0].substr(parts[0].find_first_of("0123456789")));
                float x2 = stof(parts[2]);
                vars.push_back(make_pair(x1,x2));
            } else if(parts[0][0] == 'T' && is_num(parts[2])) {
                int x1 = stoi(parts[0].substr(parts[0].find_first_of("0123456789")));
                float x2 = stof(parts[2]);
                vars.push_back(make_pair(x1,x2));
            } else if(parts[0][0] == 'T' && parts[2][0] == 'i') {

            }*/
    }

}

void output(vector<string> v) 
{
    for(string s:v)     cout << s << " ";
}

bool is_num(string s)
{
    return s.find_first_not_of( "0123456789." ) == string::npos;
}