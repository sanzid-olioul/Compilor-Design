#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<regex>
#include<stack>
#include<map>
using namespace std;
vector<string> main_elements;
vector<string> source_code;
vector<int> line_number,funcl;
vector<string> functions;
vector<string> headers;


map<string,vector<string>> header_functions;
/*
For adding standard functions of the header files;
*/
void add_headers(){
    header_functions["stdio.h"].push_back("printf");
    header_functions["stdio.h"].push_back("scanf");
    header_functions["stdio.h"].push_back("gets");
    header_functions["stdio.h"].push_back("puts");
    header_functions["stdio.h"].push_back("getline");

    header_functions["conio.h"].push_back("clrscr");
    header_functions["conio.h"].push_back("getch");
    header_functions["conio.h"].push_back("getche");
    header_functions["math.h"].push_back("abs");
    header_functions["math.h"].push_back("floor");
    header_functions["math.h"].push_back("ceil");
    header_functions["math.h"].push_back("random");
    header_functions["math.h"].push_back("round");
    header_functions["math.h"].push_back("truncat");    
}

int main()
{
    
    ifstream file("source.c");//initialize a file variable for reading data
    string line;//string variables for reading line by line
    string raw_code = "";//raw string of full source code.
    bool flag = true;//taking a flag for skipping multi line comments.
    int line_count = 0;//for keep track of user source code line.
    add_headers();//for initializing library headers.
    while (getline (file, line)) {
        line_count++;//for every iterate increasing line number
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
        /*
        taking the pure source code.
        */
        if(flag){
            if(line != ""){
                source_code.push_back(line);//storing each line into a vector.
                line_number.push_back(line_count);
                raw_code+= line + "\n";//storing each line into a string.
            }
        }  
    }
    file.close();//Reading file is done and so closing the file.
    flag = false;//For checking main function source code.
    bool flg2 = false;
    stack<string> braices;//main source code grabbing and braices check.
    int main_start = 0;//from where main functions start.
    int main_end = 0;//from where main functions end.
    bool is_found = false;//whether main function is found or not?
    int last_braices = 0;
    for(int it = 0;it < source_code.size();it++){
        smatch head;
        if(regex_match(source_code[it],head,regex("#\\s*include\\s*<([\\w\\s\\.]*)>"))){
            headers.push_back(head.str(1));
        }


        if(regex_match(source_code[it],regex("(?:int|void)\\s+main\\s*\\(\\)\\s*\\{?"))){
            flag = true;
            main_start = it;
        }
        
        if(flag){
            if(regex_match(source_code[it],regex(".*\\{.*"))){
                braices.push("{");
                is_found = true;
            }
            if(regex_match(source_code[it],regex(".*\\}.*"))){
                if(!braices.empty()){
                    braices.pop();
                }else{
                    cout<<"Imbalance got } at "<<line_number[last_braices]<<endl;
                }
                
                if(braices.empty() && is_found){
                    main_end = it;
                    flag = false;   
                }
            }
        }
        else{
            if(regex_match(source_code[it],regex(".*\\}.*"))){
                braices.push("{");
                last_braices = it;
            }
            else if(regex_match(source_code[it],regex(".*\\}.*")) && flg2){
                if(braices.empty()){
                    cout<<"Imbalance got { at"<<line_number[last_braices];
                    continue;
                }
                braices.pop();
            }
        }
    }
    if(!braices.empty()){
        cout<<"Imbalance { at"<<line_number[last_braices]<<endl;
    }
    for(int it = main_start;it <= main_end;it++){
        //cout<<source_code[it]<<endl;
        if(regex_match(source_code[it],regex(".*;$"))){
            if(regex_match(source_code[it],regex("(?:.*\\{.*)|(?:.*\\}.*)|(?:.*if\\s*\\(.*\\).*)|(?:while\\s*\\(.+\\).*)|(?:int|void|float|double|char)\\s+[A-Za-z_][A-Za-z0-9_]*\\s*\\(\\)\\s*\\{?"))){
                cout<<"Extra semiclone is given on line: "<<line_number[it]<<" "<<source_code[it]<<endl;
            }
        }else{
            if(!regex_match(source_code[it],regex("(?:.*\\{.*)|(?:.*\\}.*)|(?:.*if\\s*\\(.*\\).*)|(?:while\\s*\\(.+\\))|(?:int|void|float|double|char)\\s+[A-Za-z_][A-Za-z0-9_]*\\s*\\(\\)\\s*\\{?"))){
                cout<<"no semiclone at "<<source_code[it]<<" "<<line_number[it]<<endl;
                
            }
        }
        smatch func;
        if(regex_match(source_code[it],func,regex("\\s*([a-zA-Z_][a-zA-Z0-0_]*)\\([\\w,\"%&]*\\)\\s*;"))){
            functions.push_back(func.str(1));
            funcl.push_back(line_number[it]);
            // cout<<func.str(1)<<endl;
        } 
    }
    int i=0,j=0,k=0;
    while(i<functions.size()){
        j=0;
        bool found = false;
        while(j<headers.size()){
            k=0;
            while(k<header_functions[headers[j]].size()){
                if(header_functions[headers[j]][k] == functions[i]){
                    found = true;
                    break;
                }
                // cout<<header_functions[headers[j]][k]<<endl;
                k++;
            }
            j++;
        }
        if(!found){
            cout<<functions[i]<<" is not defined on line "<<funcl[i]<<endl;
        }
        i++;
    }
    
    
    return 0;
}