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
vector<int> line_number;
int main()
{
    ifstream file("source.c");
    string line;
    string raw_code = "";
    bool flag = true;
    int line_count = 0;
    while (getline (file, line)) {
        line_count++;
        line = regex_replace(line,regex("//.+$"),"");//Deleting single line comments.
        line = regex_replace(line,regex("^\\s+|\\s+$"),""); //For removing white spaces.
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
                line_number.push_back(line_count);
                raw_code+= line + "\n";//storing each line into a string.
            }
        }  
    }
    file.close();
    flag = false;
    stack<string> braices;
    int main_start = 0;
    int main_end = 0;
    bool is_found = false;
    for(int it = 0;it < source_code.size();it++){
        if(regex_match(source_code[it],regex("int\\s+main\\s*\\(\\)"))){
            flag = true;
            main_start = it;
        }
        
        if(flag){
            if(regex_match(source_code[it],regex(".*\\{.*"))){
                braices.push("{");
                is_found = true;
            }
            if(regex_match(source_code[it],regex(".*\\}.*"))){
                braices.pop();
                if(braices.empty() && is_found){
                    main_end = it;
                    flag = false;   
                }
            }
        }
    }
    for(int it = main_start;it <= main_end;it++){
        if(regex_match(source_code[it],regex(".*;$"))){
            if(regex_match(source_code[it],regex("(?:.*\\{.*)|(?:.*\\}.*)|(?:.*if\\s*\\(.*\\).*)|(?:while\\s*\\(.+\\).*)"))){
                cout<<"Extra semiclone is given on line: "<<line_number[it]<<" "<<source_code[it]<<endl;
            }
        }else{
            if(regex_match(source_code[it],regex("(?:.*\\{.*)|(?:.*\\}.*)|(?:.*if\\s*\\(.*\\).*)|(?:while\\s*\\(.+\\))"))){
                continue;
            }
            cout<<"no semiclone at "<<line_number[it]<<" ";
            cout<<source_code[it]<<endl;
        }
    }
    return 0;
}