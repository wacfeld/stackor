/*
 * a postfix stack based programming language
 */


#include <stdio.h>
// #include <assert.h>
#include <ctype.h>
// #include <errno.h>
// #include <float.h>
// #include <limits.h>
// #include <locale.h>
// #include <math.h>
// #include <setjmp.h>
// #include <signal.h>
// #include <stdarg.h>
// #include <stddef.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>
#define putd(x) printf(#x ": %d\n", x)

double stack[100000] = {1,1};
int si = 2;

void printstack()
{
    for(int i = 0; i < si; i++)
    {
        printf("%f ", stack[i]);
    }
    putchar('\n');
}

int is_op(int c)
{
    return
        (c == '+') ||
        (c == '-') ||
        (c == '_') || // reverse minus
        (c == '/') ||
        (c == '\\') || // reverse divide
        (c == '*') ||
        (c == 'x') || // exchange
        (c == 'd') || // duplicate
        (c == 'p') || // pop
        (c == 'D') || // duplicate second
        (c == 'r') || // rotate
        (c == 'R'); // reverse rotate
}

int is_num(int c)
{
    return
        isdigit(c) ||
        (c == '.') ||
        (c == '-') ||
        (c == 'e') ||
        (c == 'E');
}

enum type {OP, NUM, END};
typedef struct token
{
    enum type type;
    char op;
    double num;
} token;

token gettok()
{
    int c;

    while((c = getchar()) == ' ' || c == '\n') ; // skip whitespace

    // end of file
    if(c == EOF)
    {
        token t = {.type=END};
        return t;
    }

    // operators are always single characters
    if(is_op(c))
    {
        token t = {.type=OP, .op=c};
        return t;
    }

    // assume number
    char tokstr[1000];
    int ti = 0;
    tokstr[ti++] = c;
    while(is_num((c = getchar()))) // get rest of number
    {
        tokstr[ti++] = c;
    }
    ungetc(c, stdin); // undo overreading

    // turn string into number
    tokstr[ti] = 0; // terminate string
    double tok;
    sscanf(tokstr, "%lf", &tok);

    token t = {.type=NUM, .num=tok};
    return t;
}

void rotleft()
{
    double temp = stack[0];
    for(int i = 0; i < si-1; i++)
    {
        stack[i] = stack[i+1];
    }
    stack[si-1] = temp;
}

void rotright()
{
    double temp = stack[si-1];
    for(int i = si-1; i > 0; i--)
    {
        stack[i] = stack[i-1];
    }
    stack[0] = temp;
}

void proctok(token t)
{
    if(t.type == OP)
    {
        char op = t.op;
        if(op == '+')
        {
            si -= 2;
            stack[si] = stack[si] + stack[si+1];
            si++;
        }
        else if (op == '-')
        {
            si -= 2;
            stack[si] = stack[si] - stack[si+1];
            si++;
        }
        else if (op == '_')
        {
            si -= 2;
            stack[si] = stack[si+1] - stack[si];
            si++;
        }
        else if (op == '/')
        {
            si -= 2;
            stack[si] = stack[si] / stack[si+1];
            si++;
        }
        else if (op == '\\')
        {
            si -= 2;
            stack[si] = stack[si+1] / stack[si];
            si++;
        }
        else if (op == '*')
        {
            si -= 2;
            stack[si] = stack[si] * stack[si+1];
            si++;
        }
        else if (op == 'x')
        {
            double temp = stack[si-1];
            stack[si-1] = stack[si-2];
            stack[si-2] = temp;
        }
        else if (op == 'd')
        {
            stack[si] = stack[si-1];
            si++;
        }
        else if (op == 'p')
        {
            si--;
        }
        else if (op == 'D')
        {
            stack[si] = stack[si-2];
            si++;
        }
        else if (op == 'r')
        {
            rotleft();
        }
        else if (op == 'R')
        {
            rotright();
        }
    }

    else if (t.type == NUM)
    {
        stack[si++] = t.num;
    }
}

int main()
{
    token t;
    while((t = gettok()).type != END)
    {
        proctok(t);
        printstack();
    }

}
