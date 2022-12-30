#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<regex>
#include<stack>
#include<map>
#include "math_expressions.h"
#include "condition_check.h"
using namespace std;
vector<string> main_elements;
vector<string> source_code;
vector<int> line_number,funcl;
vector<string> functions;
vector<string> headers;
map<string,vector<string>> header_functions;
map<string,pair<string,string>> variable;
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


void print_variable(){
    for (auto& x: variable) {  
        cout << x.first << ": " << x.second.first << ": "<<x.second.second<<'\n';  
    }  
}


bool printf_function(string line){
    vector<string> v;
    stringstream ss(line);
    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        v.push_back(substr);
    }
    const regex r("(%d|%c|%lf|%f|%s)");
    const std::string in = v[0].substr(1,v[0].length()-2);
    std::string out = in;
    int i = 1;
    while (std::regex_search(out, r)) {
        out = std::regex_replace(out, r, variable[v[i]].second,regex_constants::format_first_only);
        i++;
    }
    cout<<out<<endl;
    return true;
}

bool condition_checker(string tokens){
    smatch condition;
    if(regex_match(tokens,condition,regex("\\s*(.*)\\s*(==|>=|<=|>|<|&&|\\|\\|)\\s*(.*)\\s*"))){
        string first,op,second;
        first = condition.str(1);
        op = condition.str(2);
        second = condition.str(3);
        int a,b;
        if(isdigit(first[0])){
            a = stoi(first);
        }else{
            a = stoi(variable[first].second);
        }
        if(isdigit(second[0])){
            b = stoi(second);
        }else{
            b = stoi(variable[second].second);
        }

        cout<<condition.str(1)<<" : "<<condition.str(2)<<" : "<<condition.str(3)<<endl;
        // cout<<"Condition is the: "<<<<endl;
        return checkOp(a,b,op);

    }
    return false;
}

int main()
{
    
    ifstream file("source.c");//initialize a file variable for reading data
    string line;//string variables for reading line by line
    string raw_code = "";//raw string of full source code.
    bool flag = true;//taking a flag for skipping multi line comments.
    int line_count = 0;//for keep track of user source code line.
    bool err = false; //whteher we found any err or not
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
    int main_end = -1;//from where main functions end.
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
                    err = true;
                }
                if(braices.empty() && is_found){
                    main_end = it;
                    flag = false;   
                }
            }
        }
        else{
            if(regex_match(source_code[it],regex(".*\\{.*"))){
                braices.push("{");
                last_braices = it;
            }
            else if(regex_match(source_code[it],regex(".*\\}.*"))){
                if(braices.empty()){
                    cout<<"Imbalance got } at "<<line_number[last_braices]<<endl;
                    err = true;
                    continue;
                }
                braices.pop();
            }
        }
    }
    if(!braices.empty()){
        cout<<"Imbalance { at"<<line_number[last_braices]<<endl;
        err = true;
    }
    bool if_founded = false;
    bool if_true = false;
    stack<string> condition_braices;
    for(int it = main_start;it <= main_end;it++){

        
        smatch cond;
        
        //Checking condition
        if(regex_match(source_code[it],cond,regex("if\\s*\\(\\s*(.*)\\s*\\)\\{?"))){
            if_founded = true;
            bool result = condition_checker(cond.str(1));
            if(result){
                if_true = true;
            }
            else{
                while(it<=main_end){
                    if(regex_match(source_code[it],regex(".*\\{.*"))){
                        condition_braices.push("{");
                    }
                    if(regex_match(source_code[it],regex(".*\\}.*"))){
                        if(!condition_braices.empty()){
                            condition_braices.pop();
                        }
                    }
                    if(condition_braices.empty()){
                        break;
                    }
                    it++;
                }
            }
        }
        if(regex_match(source_code[it],cond,regex(".*else\\s*\\{?"))){
            if(if_founded){
                if(if_true){
                    while(it<=main_end){
                        if(regex_match(source_code[it],regex(".*\\{.*"))){
                            condition_braices.push("{");
                        }
                        if(regex_match(source_code[it],regex(".*\\}.*"))){
                            if(!condition_braices.empty()){
                                condition_braices.pop();
                            }
                        }
                        if(condition_braices.empty()){
                            break;
                        }
                        it++;
                    }
                }
            }else{
                cout<<"Else statement is not allow without if statement! on line "<<line_number[it]<<endl;
            }



        }
























        //cout<<source_code[it]<<endl;
        if(regex_match(source_code[it],regex(".*;$"))){
            if(regex_match(source_code[it],regex("(?:.*\\{.*)|(?:.*\\}.*)|(?:.*if\\s*\\(.*\\).*)|(?:while\\s*\\(.+\\).*)|(?:for\\s*\\(.*;.*;.*\\)).*|(?:int|void|float|double|char)\\s+[A-Za-z_][A-Za-z0-9_]*\\s*\\(\\)\\s*\\{?"))){
                cout<<"Extra semiclone is given on line: "<<line_number[it]<<" "<<source_code[it]<<endl;
                err = true;
            }
        }else{
            if(!regex_match(source_code[it],regex("(?:.*\\{.*)|(?:.*\\}.*)|(?:.*if\\s*\\(.*\\).*)|(?:while\\s*\\(.+\\))|(?:for\\s*\\(.*;.*;.*\\))|(?:int|void|float|double|char)\\s+[A-Za-z_][A-Za-z0-9_]*\\s*\\(\\)\\s*\\{?"))){
                cout<<"no semiclone at "<<source_code[it]<<" "<<line_number[it]<<endl;
                err = true;
            }
        }
        smatch func;
        //For seeking the printf function
        if(regex_match(source_code[it],func,regex("printf\\((.*)\\)\\s*;"))){
            printf_function(func.str(1));
        }
        else if(regex_match(source_code[it],func,regex("\\s*([a-zA-Z_][a-zA-Z0-0_]*)\\([\\w,\"%&]*\\)\\s*;"))){
            functions.push_back(func.str(1));
            funcl.push_back(line_number[it]);
            // cout<<func.str(1)<<endl;
        }
        







        smatch var;

        // For declearing a new variable
        if(regex_match(source_code[it],var,regex("(int|float|double|char)\\s+([a-zA-Z_][a-zA-Z0-0_]*)\\s*;"))){
            if(!variable.count(var.str(2))){
                variable[var.str(2)].first = var.str(1);
                variable[var.str(2)].second = "Nan";
                // cout<<variable[var.str(2)].first<<" " <<variable[var.str(2)].second<<endl; 
            }
            else{
                err = true;
                cout<<"Variable "<<var.str(2)<<" on line "<<line_number[it]<<" has already been decleared!"<<endl;
            }
        }

        // For declrae and initialize a variable
        if(regex_match(source_code[it],var,regex("(int|float|double|char)\\s+([a-zA-Z_][a-zA-Z0-0_]*)\\s*=\\s*(.+)\\s*;"))){
            if(!variable.count(var.str(2))){
                variable[var.str(2)].first = var.str(1);
                variable[var.str(2)].second = var.str(3);
                // cout<<variable[var.str(2)].first<<" " <<variable[var.str(2)].second<<endl; 
            }
            else{
                err = true;
                cout<<"Variable "<<var.str(2)<<" on line "<<line_number[it]<<" has already been decleared!"<<endl;
            }
        }

        




        // For assigning a value to a existing variable
        if(regex_match(source_code[it],var,regex("([a-zA-Z_][a-zA-Z0-0_]*)\\s*=\\s*(.+)\\s*;"))){
            if(variable.count(var.str(1))){
                //Mathematical expressions
                smatch exp;
                string str = var.str(2);
                if(regex_match(str,exp,regex("(.*(?:\\+|-|\\*|/).*)"))){
                    
                    
                    variable[var.str(1)].second = to_string(evaluate(exp.str(1)));
                }
                else{
                    // variable[var.str(1)].first = var.str(1);
                    variable[var.str(1)].second = var.str(2);
                    
                    // cout<<variable[var.str(2)].first<<" " <<variable[var.str(2)].second<<endl;
                }
            }
            else{
                err = true;
                cout<<"Variable "<<var.str(1)<<" on line "<<line_number[it]<<" has never been decleared!"<<endl;
            }
        }


        


        




    }
    if(!is_found){
        cout<<"No main function found!"<<endl;
        err = true;
        return 0;
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
            err = true;
        }
        i++;
    }
    if(!err){
        cout<<"build Successfully."<<endl;
    }
    // print_variable();
    // printf_function("\"hii there %d is the %d \",a,b");
    return 0;
}