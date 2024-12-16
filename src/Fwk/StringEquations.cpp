#include <stack>
#include "include.h"

static TXT RemoveSpaceAndDealWithUnaryMinusAndPlus(TXT infix)
{
    TXT removespaces = infix;
    TXT result = "";
    for(IND i = 0; i < removespaces.size(); i++){
        if((removespaces[i] != ' ' )
        && (removespaces[i] != '\t')
        && (removespaces[i] != '\r')
        && (removespaces[i] != '\n')){
            result += removespaces[i];
        }
    }
    for(IND i = 0; i < result.size(); i++){
        if(((result[i] == '-') || (result[i] == '+'))
        && ((i == 0) || (
            (result[i-1] == '(') || (result[i-1] == '*') || (result[i-1] == '/')
         || (result[i-1] == '-') || (result[i-1] == '+') || (result[i-1] == '^')
         || (result[i-1] == '\x10') || (result[i-1] == '\x11')
        ))){
            result[i] = ((result[i] == '-')?('\x10'):('\x11'));
        }
    }
    return result;
}
static Vx<TXT> ShuntingYardAlgorithm(TXT infix)
{
    const TXT ops = "-+/*^\x10\x11";
    const INT prc[] = {2, 2, 3, 3, 4, 5, 5};
    Vx<TXT> ss;
    std::stack<INT> s;
    
    Vx<TXT> input = String.SplitWithAll_WithEachDelimiter(infix, "-+/*^()\x10\x11");
    
    for(IND i = 0; i < input.size(); i++){
        TXT token = input[i];
        if(token.empty()){
            continue;
        }
        
        CHR c = token[0];
        size_t idx = ops.find(c);

        // check for operator
        if(idx != TXT::npos){
            while(!s.empty()){
                INT prec2 = prc[s.top()];
                if(s.top() == -2) prec2 = -1;
                INT prec1 = prc[idx];
                if((prec2 > prec1) || ((prec2 == prec1) && (c != '^'))){
                    ///(TXT("Pop STACK to output:\t")+ops[s.top()]);
                    ss.push_back(TXT(1,ops[s.top()]));
                    s.pop();
                }else{
                    break;
                }
            }
            ///(TXT("Push token to stack:\t")+ops[idx]);
            s.push(idx);
        }else if(c == '('){
            ///(TXT("Push token to stack:\t")+'(');
            s.push(-2); // -2 stands for '('
        }else if(c == ')'){
            // until '(' on stack, pop operators.
            if(s.empty()) return Vx<TXT>();
            while(s.top() != -2){
                ///(TXT("Pop stack to output:\t")+ops[s.top()]);
                if(s.empty()) return Vx<TXT>();
                ss.push_back(TXT(1,ops[s.top()]));
                s.pop();
            }
            if(s.empty()) return Vx<TXT>();
            ///(TXT("Pop stack and discard:\t")+'(');
            s.pop();
        }else{
            ///(TXT("Add token to output:\t")+token);
            ss.push_back(token);
        }
    }
    while(!s.empty()){
        ss.push_back(TXT(1,ops[s.top()]));
        s.pop();
    }
    return ss;
}
Vx<TXT> KString::InfixToPostfix(TXT infix)
{
    return ShuntingYardAlgorithm(RemoveSpaceAndDealWithUnaryMinusAndPlus(infix));
}
