#include<stdio.h>
#include<ctype.h>
#include<string.h>
#define ILNGMAX 30 //标识符长度最大值
#define STRLNGMAX 100 //字符串长度最大值
#define MAX_DIGIT 9 //int最大位数
enum symbol{
    notsy, plus, minus, times, div, becomes,
    eql, neq, gtr, geq, lss, leq,
    lbrace, rbrace, lparent, rparent, lbrack, rbrack, comma, semicolon,
    intcon, charcon, stringcon,
    ident,
    constsy, intsy, charsy, voidsy, mainsy,
    ifsy, elsesy, dosy, whilesy, forsy,
    scanfsy, printfsy, returnsy
}; //枚举类型
const char symbolstr[][12] = {
    "notsy", "plus", "minus", "times", "div", "becomes",
    "eql", "neq", "gtr", "geq", "lss", "leq",
    "lbrace", "rbrace", "lparent", "rparent", "lbrack", "rbrack", "comma", "semicolon",
    "intcon", "charcon", "stringcon",
    "ident",
    "constsy", "intsy", "charsy", "voidsy", "mainsy",
    "ifsy", "elsesy", "dosy", "whilesy", "forsy",
    "scanfsy", "printfsy", "returnsy"
}; //枚举类型
const char symbolvalue[][8] = {
    "", "+", "-", "*", "/", "=",
    "==", "!=", ">", ">=", "<", "<=",
    "{", "}", "(", ")", "[", "]", ",", ";",
    "", "", "",
    "",
    "const", "int", "char", "void", "main",
    "if", "else", "do", "while", "for",
    "scanf", "printf", "return"
};
const char errormessage[][50] = {
    "identifier is too long",
    "invalid character",
    "there is only one character in \'",
    "string is too long",
    "lack of \"",
    "the number is too big"
};
const int nkw = 13; //保留字个数
const char word[13][8] = { //保留字数组
    "char", "const", "do", "else", "for", "if", "int",
    "main", "printf", "return", "scanf", "void", "while",
};
enum symbol wsym[13] = {//保留字对应类型
    charsy, constsy, dosy, elsesy, forsy, ifsy, intsy,
    mainsy, printfsy, returnsy, scanfsy, voidsy, whilesy
};
FILE *fin; //输入文件指针
char ch; //当前所读字符
int cc; //当前所取字符在该行的位置
int l; //当前所取字符所在行数
enum symbol sym; //识别到的单词类型
int inum; //识别到的数字
char c; //识别到的字符
char id[ILNGMAX+1]; //识别到的标识符
char string[STRLNGMAX+1]; //识别到的字符串

void getch()//获取一个字符
{
    if(feof(fin))
        return;
    ch = fgetc(fin);
    if(ch == '\n')
    {
        cc = 0;
        l++;
    }
    else
        cc++;
}
void error(int n)//error都直接return了，这里应该再读一个字符
{
    printf("Error %d! Message: %s in line%d.%d\n", n, errormessage[n], l, cc);
}
void getsym()
{
    int k, i, j;
    char a[ILNGMAX+1];
    while(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
        getch();
    if(ch == -1)
        return;
    if(isalpha(ch) || ch == '_')
    {
        for(k = 0; k < ILNGMAX && (isalpha(ch) || isdigit(ch) || ch == '_'); k++)
        {
            a[k] = ch;
            getch();
        }
        a[k] = '\0';
        if(k == ILNGMAX && (isalpha(ch) || isdigit(ch) || ch == '_'))//标识符超长
        {
            error(0);
            while(isalpha(ch) || isdigit(ch) || ch == '_')
            {
                getch();
            }
            return;
        }
        strcpy(id, a);
        i = 0;
        j = nkw - 1;
        while(i <= j)
        {
            k = (i + j) / 2;
            if(strcmp(id, word[k]) <= 0)
                j = k - 1;
            if(strcmp(id, word[k]) >= 0)
                i = k + 1;
        }
        if(i - 1 > j)
            sym = wsym[k];
        else
            sym = ident;
    }
    else if(ch == '0')
    {
        getch();
        if(isdigit(ch))
        {
            error(1);
            return;
        }
        else
        {
            sym = intcon;
            inum = 0;
        }
    }
    else if(isdigit(ch))
    {
        sym = intcon;
        k = 0;
        inum = 0;
        while(k < MAX_DIGIT && isdigit(ch))
        {
            inum = inum * 10 + (ch - '0');
            k++;
            getch();
        }
        if(k == MAX_DIGIT && isdigit(ch))
        {
            error(5);
            while(isdigit(ch))
            {
                getch();
            }
            return;
        }
    }
    else
    {
        switch(ch)
        {
            case '\'':  sym = charcon;
                        getch();
                        if(isalpha(ch) || isdigit(ch) || ch == '_' ||
                           ch == '+' || ch == '-' || ch == '*' || ch == '/')
                           c = ch;
                        else
                        {
                            error(1);
                            return;
                        }
                        getch();
                        if(ch != '\'')
                        {
                            error(2);
                            return;
                        }
                        getch();
                break;
            case '\"':  getch();
                        k = 0;
                        while(k < STRLNGMAX && ch != '\"' && ch >= 32 && ch <= 126)
                        {
                            string[k++] = ch;
                            getch();
                        }
                        string[k] = '\0';
                        if(k == STRLNGMAX && ch != '\"' && ch >= 32 && ch <= 126) //字符串超长
                        {
                            error(3);
                            return;
                        }
                        if(ch == '\"')
                            sym = stringcon;
                        else
                        {
                            error(4);
                            return;
                        }
                        getch();
                break;
            case '=' :  getch();
                        if(ch == '=')
                        {
                            sym = eql;
                            getch();
                        }
                        else
                        {
                            sym = becomes;
                        }
                break;
            case '+' :  sym = plus;
                        getch();
                break;
            case '-' :  sym = minus;
                        getch();
                break;
            case '*' :  sym = times;
                        getch();
                break;
            case '/' :  sym = div;
                        getch();
                break;
            case '<' :  getch();
                        if(ch == '=')
                        {
                            sym = leq;
                            getch();
                        }
                        else
                        {
                            sym = lss;
                        }
                break;
            case '>' :  getch();
                        if(ch == '=')
                        {
                            sym = geq;
                            getch();
                        }
                        else
                        {
                            sym = gtr;
                        }
                break;
            case '!' :  getch();
                        if(ch == '=')
                        {
                            sym = neq;
                            getch();
                        }
                        else
                        {
                            error(1);
                        }
                break;
            case '{' :  sym = lbrace;
                        getch();
                break;
            case '}' :  sym = rbrace;
                        getch();
                break;
            case '(' :  sym = lparent;
                        getch();
                break;
            case ')' :  sym = rparent;
                        getch();
                break;
            case '[' :  sym = lbrack;
                        getch();
                break;
            case ']' :  sym = rbrack;
                        getch();
                break;
            case ';' :  sym = semicolon;
                        getch();
                break;
            case ',' :  sym = comma;
                        getch();
                break;
            default:    sym = notsy;
                        error(1);
        }
    }
}
void print(int n)
{
    printf("%d %s ", n, symbolstr[sym]);
    switch(sym)
    {
        case intcon:printf("%d", inum);
            break;
        case charcon:printf("%c", c);
            break;
        case stringcon:printf("%s", string);
            break;
        case ident: printf("%s", id);
            break;
        default: printf("%s", symbolvalue[sym]);
    }
    putchar('\n');
}
int main()
{
    int i;
    char sin[FILENAME_MAX];
    printf("please input source program file name : \n");
    scanf("%s", sin);
    fin = fopen(sin, "r");
    if(fin == NULL)
    {
        printf("Failed to open!\n");
        return 1;
    }
    printf("Open successfully!\n");

    cc = 0;
    l = 1;
    i = 1;
    getch();
    while(!feof(fin))
    {
        getsym();
        if(ch == -1)
            break;
        print(i++);
    }
    fclose(fin);
}
