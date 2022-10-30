#include<iostream>
#include <fstream>
#include <string>
#include<vector>
#include<iterator>
#include <regex>
#include<stack>
using namespace std;
vector<string> source_code;
vector<string> preprocessors;
vector<string> main_elements;
vector<string>::iterator it,main_start,main_end;
int main()
{
    ifstream file("source.c");
    string line;
    string raw_code = "";
    bool flag = true;
    while (getline (file, line)) {
        line = regex_replace(line,regex("^\\s+|\\s+$"),""); //For removing white spaces.
        line = regex_replace(line,regex("//.+$"),"");//Deleting single line comments.
        /*
            Finding the multi line comments initil flag
            and skipping lines till end flag.
        */
        if(regex_match(line,regex("/\\*.*"))){
            flag = false;
        }
        /*
            End flag of the multi line comments check to stop skipping comments
        */
        if(regex_match(line,regex("\\*/.*"))){
            flag = true;
            continue;
        }
        if(flag){
            if(line != ""){
                source_code.push_back(line);//storing each line into a vector.
                raw_code+= line + "\n";//storing each line into a string.
            }
        }  
    }
    file.close();
    flag = false;
    stack<string> braices;
    for(it = source_code.begin();it < source_code.end();it++){
        if(regex_match(*it,regex("int\\s+main\\s*\\(\\)"))){
            flag = true;
            main_start = it;
        }
        bool is_found = false;
        if(flag){
            if(regex_match(*it,regex(".*\\{.*"))){
                braices.push("{");
                is_found = true;
            }
            if(regex_match(*it,regex(".*\\}.*"))){
                braices.pop();
                
            }
            if(braices.empty() && is_found){
                main_end = it;
                flag = false;
            }
            cout<<*it<<endl;
        }
    }
    for(it = main_start;it <= main_end;it++){






        
        cout<<*it<<std::endl;
    }
    return 0;
}