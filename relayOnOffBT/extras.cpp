#include "extras.h"
bool strcmp2(char* a, char* b) {
  int la = strlen(a);
  int lb = strlen(b);
  if (la != lb) return false;
  for (lb = 0; lb < la; lb++) {
    if (toupper(a[lb]) != toupper(b[lb])) return false;
  }
  return true;
}

int strcmp3(char* a, char* b, bool sensitive) {
  int la = strlen(a);
  int lb = strlen(b);
  int ct = la;
  if (la > lb) ct = lb;
  for (lb = 0; lb < ct + 1; lb++) {
	  printf("Comparing %c to %c\n",a[lb],b[lb]);
    if (sensitive != true) {
      if (toupper(a[lb]) < toupper(b[lb])) return -1;
      if (toupper(a[lb]) > toupper(b[lb])) return 1;
    } else {
      if (a[lb] < b[lb]) return -1;
      if (a[lb] > b[lb]) return 1;
    }
  }
  return 0;
}

char* stristr( const char* str1, const char* str2 )
{
    const char* p1 = str1 ;
    const char* p2 = str2 ;
    const char* r = *p2 == 0 ? str1 : 0 ;

    while( *p1 != 0 && *p2 != 0 )
    {
        if( tolower( (unsigned char)*p1 ) == tolower( (unsigned char)*p2 ) )
        {
            if( r == 0 )
            {
                r = p1 ;
            }

            p2++ ;
        }
        else
        {
            p2 = str2 ;
            if( r != 0 )
            {
                p1 = r + 1 ;
            }

            if( tolower( (unsigned char)*p1 ) == tolower( (unsigned char)*p2 ) )
            {
                r = p1 ;
                p2++ ;
            }
            else
            {
                r = 0 ;
            }
        }

        p1++ ;
    }

    return *p2 == 0 ? (char*)r : 0 ;
}