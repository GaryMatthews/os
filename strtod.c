#include <stdlib.h>
#include <ctype.h>

double strtod(const char *nptr, char ** endptr) {
    (void)endptr;
    unsigned short negative;
    double number;
    double fp_part;
    int exponent = 0;
    unsigned short exp_negative;
    
    /* advance beyond any leading whitespace */
    while (isspace(*nptr))
        nptr++;
    
    /* check for optional '+' or '-' */
    negative=0;
    if (*nptr=='-')
    {
        negative=1;
        nptr++;
    }
    else
        if (*nptr=='+')
            nptr++;
    
    number=0;
    while (isdigit(*nptr))
    {
        number=number*10+(*nptr-'0');
        nptr++;
    }
    
    if (*nptr=='.')
    {
        nptr++;
        fp_part=0;
        while (isdigit(*nptr))
        {
            fp_part=fp_part/10.0 + (*nptr-'0')/10.0;
            nptr++;
        }
        number+=fp_part;
    }
    
    if (*nptr=='e' || *nptr=='E')
    {
        nptr++;
        exp_negative=0;
        if (*nptr=='-')
        {
            exp_negative=1;
            nptr++;
        }
        else
            if (*nptr=='+')
                nptr++;
        
        exponent=0;
        while (isdigit(*nptr))
        {
            exponent=exponent*10+(*nptr-'0');
            exponent++;
        }
    }
    
    while (exponent)
    {
        if (exp_negative)
            number/=10;
        else
            number*=10;
        exponent--;
    }
    return (negative ? -number:number);
}
