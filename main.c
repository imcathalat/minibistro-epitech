#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

size_t calc_size(const char *s)
{
    size_t n = 0;
    while (s && s[n]) n++;
    return n;
}

void show_help(void){
    const char *msg =
        "USAGE\n"
        "   ./calc\n\n"
        "DESCRIPTION\n"
        "    Reads a line on the standard input and evaluates the expression, until there are no more data to read.\n";
    write(1, msg, calc_size(msg));
}

void s_copy(char *d, const char *s, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        d[i] = s[i];
    }
}

int s_cmp(const char *a, const char *b)
{
    size_t i = 0;
    while (a[i] && b[i])
    {
        if (a[i] != b[i]) return (a[i] < b[i]) ? -1 : 1;
        i++;
    }
    if (a[i]==0 && b[i]==0) return 0;
    return a[i]==0 ? -1 : 1;
}

char *s_dup(const char *s)
{
    size_t n = calc_size(s);
    char *p = (char*)malloc(n+1);
    if (!p) return NULL;
    for (size_t i=0; i<=n; i++) p[i] = s[i];
    return p;
}

int is_digit(char c)
{
    return c>='0' && c<='9';
}

// numbers helpers

int is_neg(const char *s)
{
    return s && s[0]=='-';
}
const char* abs_part(const char *s)
{
    return is_neg(s)? s+1 : s;
}

char *strip_zeros(const char *m)
{
    while (m[0]=='0' && m[1]) m++;
    return s_dup(m);
}

char *norm_sign(const char *sin)
{
    int neg = is_neg(sin);
    const char *mag = abs_part(sin);
    char *mz = strip_zeros(mag);

    if (s_cmp(mz, "0")==0)
    {
        return mz;
    }

    if (neg)
    {
        size_t L = calc_size(mz);
        char *o = (char*)malloc(L+2);

        o[0]='-';
        s_copy(o+1,mz,L+1);
        free(mz);
        return o;
    }
    return mz;
}

int cmp_abs(const char *a, const char *b)
{
    size_t la = calc_size(a), lb = calc_size(b);
    if (la>lb) return 1;
    if (la<lb) return -1;
    return s_cmp(a,b);
}

char *add_abs(const char *a, const char *b)
{
    size_t la = calc_size(a), lb = calc_size(b);
    size_t mx = la>lb? la:lb;
    char *r = (char*)malloc(mx+2);

    size_t ia=la, ib=lb, ir=0;
    int carry=0;
    while (ia||ib||carry)
    {
        int da=0, db=0;
        if (ia) da = a[--ia]-'0';
        if (ib) db = b[--ib]-'0';
        int s = da+db+carry;
        carry = s/10;
        r[ir++] = (char)('0'+(s%10));
    }
    r[ir]=0;
    for (size_t i=0; i<ir/2; i++)
    {
        char t=r[i];
        r[i]=r[ir-1-i];
        r[ir-1-i]=t;
    }
    return r;
}

char *sub_abs(const char *a, const char *b)
{
    size_t la = calc_size(a), lb = calc_size(b);
    char *r = (char*)malloc(la+1);

    size_t ia=la, ib=lb, ir=0;
    int bor=0;
    while (ia||ib)
    {
        int da=0, db=0;
        if (ia) da = a[--ia]-'0';
        if (ib) db = b[--ib]-'0';
        da -= bor;
        if (da<db)
        {
            da+=10;
            bor=1;
        }
        else bor=0;
        r[ir++] = (char)('0'+(da-db));
    }
    while (ir>1 && r[ir-1]=='0') ir--;
    r[ir]=0;
    for (size_t i=0; i<ir/2; i++)
    {
        char t=r[i];
        r[i]=r[ir-1-i];
        r[ir-1-i]=t;
    }
    return r;
}

char *mul_abs(const char *a, const char *b)
{
    size_t la=calc_size(a), lb=calc_size(b);
    if ((la==1&&a[0]=='0')||(lb==1&&b[0]=='0'))
    {
        char *z=(char*)malloc(2);
        z[0]='0';
        z[1]=0;
        return z;
    }
    size_t L = la+lb;
    int *acc = (int*)malloc(sizeof(int)*L);
    for (size_t i=0; i<L; i++) acc[i]=0;
    for (size_t i=0; i<la; i++)
    {
        int da = a[la-1-i]-'0';
        for (size_t j=0; j<lb; j++)
        {
            int db = b[lb-1-j]-'0';
            acc[i+j] += da*db;
        }
    }
    for (size_t i=0; i<L-1; i++)
    {
        acc[i+1]+=acc[i]/10;
        acc[i]%=10;
    }
    while (L>1 && acc[L-1]==0)
    {
        L--;
    }
    char *r=(char*)malloc(L+1);

    for (size_t i=0; i<L; i++)
    {
        r[L-1-i] = (char)('0'+acc[i]);
    }
    r[L]=0;
    free(acc);
    return r;
}

char *mul_abs_digit(const char *a, int d)
{
    if (d==0)
    {
        char *z=(char*)malloc(2);
        z[0]='0';
        z[1]=0;
        return z;
    }
    size_t la=calc_size(a);
    char *r=(char*)malloc(la+2);

    size_t ia=la, ir=0;
    int c=0;
    while (ia||c)
    {
        int da=0;
        if (ia) da=a[--ia]-'0';
        int p=da*d+c;
        c=p/10;
        r[ir++]=(char)('0'+(p%10));
    }
    r[ir]=0;
    for (size_t i=0; i<ir/2; i++)
    {
        char t=r[i];
        r[i]=r[ir-1-i];
        r[ir-1-i]=t;
    }
    char *z = strip_zeros(r);
    free(r);
    return z;
}

char *append_digit_norm(const char *s, char d)
{
    size_t L=calc_size(s);
    char *t=(char*)malloc(L+2);
    s_copy(t,s,L);
    t[L]=d;
    t[L+1]=0;
    char *z=strip_zeros(t);
    free(t);
    return z;
}

char *div_abs(const char *a, const char *b)
{

    int c0 = cmp_abs(a,b);
    if (c0<0)
    {
        char *z=(char*)malloc(2);
        z[0]='0';
        z[1]=0;
        return z;
    }
    if (c0==0)
    {
        char *o=(char*)malloc(2);
        o[0]='1';
        o[1]=0;
        return o;
    }

    size_t la=calc_size(a);
    char *q=(char*)malloc(la+1);
    size_t qn=0;
    char *rem = s_dup("0");

    for (size_t i=0; i<la; i++)
    {
        char *nr = append_digit_norm(rem, a[i]);
        free(rem);
        rem = nr;

        int qd=0;
        for (int d=9; d>=1; d--)
        {
            char *t = mul_abs_digit(b,d);
            int cmp = cmp_abs(rem,t);
            if (cmp>=0)
            {
                qd = d;
                char *newr = sub_abs(rem,t);
                free(rem);
                free(t);
                rem = strip_zeros(newr);
                free(newr);
                break;
            }
            free(t);
        }
        q[qn++] = (char)('0'+qd);
    }
    q[qn]=0;
    char *out = strip_zeros(q);
    free(q);
    free(rem);
    return out;
}


char *big_add(const char *sa, const char *sb)
{
    int na=is_neg(sa), nb=is_neg(sb);
    const char *a=abs_part(sa), *b=abs_part(sb);
    if (!na && !nb)
    {
        char *s=add_abs(a,b);
        if(!s) return NULL;
        char *n=norm_sign(s);
        free(s);
        return n;
    }
    if (na && nb)
    {
        char *s=add_abs(a,b);
        size_t L=calc_size(s);

        char *t=(char*)malloc(L+2);
        t[0]='-';
        s_copy(t+1,s,L+1);
        free(s);
        char *n=norm_sign(t);
        free(t);
        return n;
    }

    if (na && !nb)
    {
        int c=cmp_abs(b,a);
        if (c==0) return s_dup("0");
        if (c>0)
        {
            char *d=sub_abs(b,a);
            char *n=norm_sign(d);
            free(d);
            return n;
        }

        char *d=sub_abs(a,b);
        size_t L=calc_size(d);

        char *t=(char*)malloc(L+2);
        t[0]='-';

        s_copy(t+1,d,L+1);
        free(d);
        char *n=norm_sign(t);
        free(t);
        return n;
    }
    else
    {
        int c=cmp_abs(a,b);
        if (c==0) return s_dup("0");
        if (c>0)
        {
            char *d=sub_abs(a,b);
            if(!d) return NULL;
            char *n=norm_sign(d);
            free(d);
            return n;
        }
        char *d=sub_abs(b,a);
        size_t L=calc_size(d);

        char *t=(char*)malloc(L+2);
        t[0]='-';

        s_copy(t+1,d,L+1);
        free(d);
        char *n=norm_sign(t);
        free(t);
        return n;
    }
}

char *big_sub(const char *sa, const char *sb)
{
    const char *bb = abs_part(sb);
    size_t L = calc_size(sb);
    char *nb = NULL;
    if (is_neg(sb))
    {
        nb = s_dup(bb);
    }
    else
    {
        nb = (char*)malloc(L+2);
        nb[0]='-';
        s_copy(nb+1,sb,L+1);
    }
    char *r = big_add(sa, nb);
    free(nb);
    return r;
}

char *big_mul(const char *sa, const char *sb)
{
    int na=is_neg(sa), nb=is_neg(sb);
    char *mag = mul_abs(abs_part(sa), abs_part(sb));
    char *norm = norm_sign(mag);
    free(mag);
    if (s_cmp(norm,"0")==0)
    {
        return norm;
    }
    if (na ^ nb)
    {
        size_t L=calc_size(norm);

        char *t=(char*)malloc(L+2);
        t[0]='-';

        s_copy(t+1,norm,L+1);
        free(norm);
        char *f=norm_sign(t);
        free(t);
        return f;
    }
    return norm;
}

char *big_div(const char *sa, const char *sb)
{
    const char *bb = abs_part(sb);
    int na=is_neg(sa), nb=is_neg(sb);
    char *q = div_abs(abs_part(sa), bb);
    char *n = norm_sign(q);
    free(q);
    if (na ^ nb)
    {
        size_t L=calc_size(n);
        char *t=(char*)malloc(L+2);
        t[0]='-';
        s_copy(t+1,n,L+1);
        free(n);
        char *f=norm_sign(t);
        free(t);
        return f;
    }
    return n;
}

// helpers

char *clean_expr(const char *src)
{
    size_t n = calc_size(src);
    char *out = (char*)malloc(n+1);
    size_t j=0;
    for (size_t i=0; i<n; i++)
    {
        char c = src[i];
        if (!(c==' ' || c=='\t')) out[j++] = c;
    }
    out[j]=0;
    return out;
}

static int is_op(char c)
{
    return c=='+'||c=='-'||c=='*'||c=='/';
}
static int prec(char c)
{
    return (c=='+'||c=='-')?1 : (c=='*'||c=='/')?2 : -1;
}

static int grow_buf(char **buf, size_t *cap, size_t need_more)
{
    if (*cap >= need_more) return 1;
    size_t newcap = *cap? *cap : 64;
    while (newcap < need_more)
    {
        newcap *= 2;
    }
    char *nb = (char*)malloc(newcap);
    if (!nb)
    {
        return 0;
    }
    for (size_t i=0; i<*cap; i++) nb[i]=(*buf)[i];
    free(*buf);
    *buf=nb;
    *cap=newcap;
    return 1;
}

int push_char(char **buf, size_t *cap, size_t *len, char c)
{
    if (!grow_buf(buf, cap, *len+1+1))
    {
        return 0;
    }
    (*buf)[(*len)++] = c;
    (*buf)[*len]=0;
    return 1;
}

int push_span(char **buf, size_t *cap, size_t *len, const char *p, size_t n)
{
    if (!grow_buf(buf, cap, *len+n+1))
    {
        return 0;
    }
    for (size_t i=0; i<n; i++)
    {
        (*buf)[(*len)++] = p[i];

    }
    (*buf)[*len]=0;
    return 1;
}

char *infix_to_postfix(const char *operation)
{
    if(!operation)
    {
        char* error_message = "input expression pointer is NULL";
        write(2, error_message, calc_size(error_message));
        write(2, "\n", 1);
        return NULL;
    }

    if (*operation == '\0')
    {
        char* error_message = "empty expression";
        write(2, error_message, calc_size(error_message));
        write(2, "\n", 1);
        return NULL;
    }

    char  *stack = NULL;
    size_t stack_capacity = 0;
    int    top = -1;

    size_t input_len = calc_size(operation);
    size_t capacity = 2 * input_len + 2;
    char  *operation_postfix = (char*)malloc(capacity);

    if (!operation_postfix)
    {
        const char *msg = "memory allocation failed for output buffer";
        write(2, msg, calc_size(msg));
        write(2, "\n", 1);
        return NULL;
    }

    size_t final_size = 0;
    operation_postfix[0] = '\0';

    int if_minus = 1;
    size_t index = 0;

    while (operation[index])
    {
        char character = operation[index];

        if (is_digit(character))
        {
            size_t number_first_index = index;
            while (is_digit(operation[index]))
            {
                index++;
            }
            size_t ndigits = index - number_first_index;

            s_copy(operation_postfix + final_size, operation + number_first_index, ndigits);
            final_size += ndigits;
            operation_postfix[final_size++] = ' ';
            operation_postfix[final_size] = '\0';

            if_minus = 0;
            continue;
        }

        if (character == '(')
        {
            if ((size_t)(top + 1) >= stack_capacity)
            {
                size_t new_cap = stack_capacity ? stack_capacity * 2 : 16;
                char *new_stack = (char*)realloc(stack, new_cap);
                stack = new_stack;
                stack_capacity = new_cap;
            }
            stack[++top] = '(';
            index++;
            if_minus = 1;
            continue;
        }

        if (character == ')')
        {
            int found_left = 0;
            while (top >= 0)
            {
                char op_on_top = stack[top--];
                if (op_on_top == '(')
                {
                    found_left = 1;
                    break;
                }
                operation_postfix[final_size++] = op_on_top;
                operation_postfix[final_size++] = ' ';
                operation_postfix[final_size]   = '\0';
            }
            index++;
            if_minus = 0;
            continue;
        }

        if (character=='+' || character=='-' || character=='*' || character=='/')
        {

            if (character=='-' && if_minus)
            {
                operation_postfix[final_size++] = '0';
                operation_postfix[final_size++] = ' ';
                operation_postfix[final_size]   = '\0';
            }

            int p_cur = prec(character);

            while (top >= 0)
            {
                char op_on_top = stack[top];
                int p_top = prec(op_on_top);
                if (p_top < 0) break; /* '(' */
                if (p_top >= p_cur)
                {
                    top--;
                    operation_postfix[final_size++] = op_on_top;
                    operation_postfix[final_size++] = ' ';
                    operation_postfix[final_size]   = '\0';
                }
                else break;
            }

            if ((size_t)(top + 1) >= stack_capacity)
            {
                size_t new_cap = stack_capacity ? stack_capacity * 2 : 16;
                char *new_stack = (char*)realloc(stack, new_cap);
                stack = new_stack;
                stack_capacity = new_cap;
            }
            stack[++top] = character;

            index++;
            if_minus = 1;
            continue;
        }
    }

    while (top >= 0)
    {
        char op = stack[top--];
        if (op == '(' || op == ')')
        {
            free(stack);
            free(operation_postfix);
            const char *msg = "mismatched parentheses: found '(' without ')'";
            write(2, msg, calc_size(msg));
            write(2, "\n", 1);
            return NULL;
        }
        operation_postfix[final_size++] = op;
        operation_postfix[final_size++] = ' ';
        operation_postfix[final_size]   = '\0';
    }
    free(stack);

    if (final_size && operation_postfix[final_size - 1] == ' ')
        operation_postfix[--final_size] = '\0';

    return operation_postfix;
}

char *eval_postfix(const char *post)
{

    size_t cap=16, top=0;
    char **stk = (char**)malloc(sizeof(char*)*cap);

    size_t i=0;
    while (post[i])
    {
        while (post[i]==' ')
        {
            i++;
        }

        if (post[i]==0)
        {
            break;
        }

        if (is_op(post[i]) && (post[i+1]==0 || post[i+1]==' '))
        {
            char op = post[i++];
            if (top<2)
            {
                for(size_t k=0; k<top; k++) free(stk[k]);
                free(stk);
                return NULL;
            }
            char *b = stk[--top];
            char *a = stk[--top];
            char *r = NULL;

            if (op == '+')
            {
                r = big_add(a, b);
            }
            else if (op == '-')
            {
                r = big_sub(a, b);
            }
            else if (op == '*')
            {
                r = big_mul(a, b);
            }
            else if (op == '/')
            {
                r = big_div(a, b);
            }

            free(a);
            free(b);
            if (top>=cap)
            {
                size_t ncap = cap*2;
                char **ns = (char**)malloc(sizeof(char*)*ncap);
                for (size_t z=0; z<cap; z++)
                {
                    ns[z]=stk[z];
                }
                free(stk);
                stk=ns;
                cap=ncap;
            }
            stk[top++] = r;
        }
        else
        {

            size_t j=i;
            while (post[j] && post[j]!=' ') j++;

            char *tmp = (char*)malloc(j-i+1);
            s_copy(tmp, post+i, j-i);
            tmp[j-i]=0;

            char *nz = strip_zeros(tmp);
            free(tmp);
            if (top>=cap)
            {
                size_t ncap = cap*2;
                char **ns = (char**)malloc(sizeof(char*)*ncap);
                for (size_t z=0; z<cap; z++)
                {
                    ns[z]=stk[z];
                }
                free(stk);
                stk=ns;
                cap=ncap;
            }
            stk[top++] = nz;
            i = j;
        }
    }
    char *res = stk[0];
    free(stk);
    char *fin = norm_sign(res);
    free(res);
    return fin;
}


int main(int argc, char **argv)
{

    for (int i = 1; i < argc; i++) {
        if (s_cmp(argv[i], "-h") || s_cmp(argv[i], "--help")) {
            show_help();
            return 0;
        }
    }

    char *operation = NULL;
    size_t operation_size = 0;
    ssize_t n;

    while ((n = getline(&operation, &operation_size, stdin)) != -1)
    {
        if (n>0 && operation[n-1]=='\n')
        {
            operation[n-1]=0;
        }

        char *clean = clean_expr(operation);

        char *post = infix_to_postfix(clean);
        free(clean);

        char *result = eval_postfix(post);

        write(1, result, calc_size(result));
        write(1, "\n", 1);
        free(result);
    }
    free(operation);
    return 0;
}
