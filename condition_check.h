#include<regex>
bool checkOp(int a, int b, string op){
    if(op == "==")  return a == b;
    else if(op == ">")   return a > b;
    else if(op == "<")   return a < b;
    else if(op == ">=")  return a >= b;
    else if(op == "<=")  return a <= b;
    else if(op == "&&")  return a && b;
    else if(op == "||")  return a || b;
    else return false;
}
