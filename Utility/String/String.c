/**
  ******************************************************************************
  * @file       	String.c
  * @author  	(주)자이언텍 기술개발지원팀
  * @version 	V0.1
  * @date    	2014년 12월19일
  * @brief    	이파일은  문자열처리에관한 함수의 집합이다.
                
  *          
  * @attention
  * 이파일은 자이언텍 지적재산권이므로 타사에 소스를 제공해서는 안됩니다. 
  * <h2><center>&copy; COPYRIGHT 2014 ZionTech Inc.</center></h2>
  * @warning  
  * @verbatim
  *  Edit History
  * @endverbatim  
  ******************************************************************************  
  */
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "TEECO_System.h"
#include <math.h>
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @fn        StrToHex
  * @brief    문자열로 헥사값이 주어지면 변환하는 함수
  * @date    2014년 12월1일  
  * @param    data (문자열 헥사)
  * @retval  	int
  * @todo    
  */
int StrToHex(char *data)
{
    char 	*string = data;
    int 		base     = 10;
    int 		minus    = 0;
    int 		result   = 0;
    int 		lastIndex;    
    int 		i;
    
    if(string[0]=='-')
    {
        minus = 1;
        string++;
    }    
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
        base    = 16;
        string += 2;
    }    
    lastIndex = strlen(string) - 1;
    
    if(lastIndex<0)
        return -1;
    
    if(string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
        base = 16;
        string[lastIndex] = 0;
        lastIndex--;
    }

    if(base==10)
    {
        result = atoi(string);
        result = minus ? (-1*result):result;
    }
    else
    {
        for(i=0;i<=lastIndex;i++)
        {
            if(isalpha(string[i]))
            {
                if(isupper(string[i]))
                    result = (result<<4) + string[i] - 'A' + 10;
                else
                    result = (result<<4) + string[i] - 'a' + 10;
            }
            else
                result = (result<<4) + string[i] - '0';
        }
        result = minus ? (-1*result):result;
    }
    return result;
}

/**
	@brief 주어진 문자열을 뒤집는 함수이다.
*/
int StringReverse(char *str)
{
	int 	len = strlen(str);
	char	sbuf[len];
	int	arr_cnt=0;	
	for(int i=len-1; i>=0; i--)
	{
		sbuf[i] = str[arr_cnt++];
	}
	for(int i=0; i<len; i++)
	{
		str[i] = sbuf[i];
	}	
	return(1);
}

/**
	@brief 정수를 매개변수로 원단위 표시의 문자열로 변환한다.
	value 에 12345라는 정수값이 입력되면 str에 123,456으로 반환해준다.
	array_size 는 str의 배열사이즈이다.
*/
int IntToStringWon(int value, char *str, int array_size)
{
	char	buf[128];
	sprintf(buf, "%lu", value);

	int 	len = strlen(buf);
	int	arr_cnt=0;
	int	comma_cnt=0;

	for(int i=len-1; i>=0; i--)
	{
		str[arr_cnt++] = buf[i];
		comma_cnt++;
		if(comma_cnt==3) { str[arr_cnt++] = ','; comma_cnt=0; }
	}
	if(comma_cnt==0)
	{
		str[arr_cnt-1] = 0;
	}
	str[arr_cnt]=0;
	StringReverse(str);
	return(1);
}

//
/**
	* @fn        StringGetLine
	* @brief    	이함수는  문장문자열에서 1라인을 가져온다.
	* @date    	2014년 12월16일  
	* @param  src_buf : 문장이 들어있는 버퍼
	* @param  getbuf : 한라인을 읽어서 저장되는 버퍼
	* @param  getbuf_size : 
	* @param  startpos : 문장의 시작위치
	* @param  read_cnt : 한라인을 읽은길이
	* @retval  	true, false
	* @todo    
  */ 
int StringGetLine(char *src_buf, char *getbuf,  int getbuf_size, int startpos, int *read_cnt)
{
	int	cnt=0;
	int Done=1;
	while(Done)
	{
		if(src_buf[startpos+cnt]==0x0d) break;	//!< 문자가 행바꿈이면 탈출한다.
		getbuf[cnt] = src_buf[startpos+cnt];
		cnt++;
		if(cnt +1 == getbuf_size) break; //버퍼의 경계를 초과하면 안된다.			
	}
	getbuf[cnt]=0;		//문자열의 끝코드를 추가한다.
	*read_cnt = (cnt+2); //0x0d, 0x0a 를 제거하기위해 +2 을 해준다.
	if(getbuf[0] =='/' && getbuf[1]=='/') return(0);	//주석문이면 처리하지 않는다.
	return(1);
}

//문자열중에서 \ 가 있으면 그이후의 문자열만 얻는다.
int StringSlashParsing(char *buf)
{
	int		i;
	int		len = strlen(buf);
	char	name[32];
	int		cnt=0;
	for(i=0; i< len; i++)
	{
		if(buf[i]=='\\') break;
	}	
	if(i==len) return(0); 
	i++;
	for(; i< len; i++)
	{
		name[cnt++] = buf[i];
	}
	name[cnt]=0;
	//Dprintf("StringSlashParsing:: Conv Buf=%s\n", name);
	strcpy(buf, name);
	return(1);
}

// 매개변수문자열에서 공백이나 특수문자(콘트롤코드)을제거한다.
void StringTrim(char *str)
{
	int		len = strlen(str);
	int		i;
	char		string[sizeof(str)];
	int		cnt=0;

	for(i=0; i<len; i++)
	{
		if(str[i] != ' ' || str[i] > 0x20)
		{
			string[cnt++] = str[i];
		}
	}
	string[cnt] = 0;
	strcpy(str, string);
}

// 주어진 문자열중에서 문자열을 찾는다.
// 검색문자열은 64개를 초과할수 없다.
int StringSearch(const char *src, char *compare)
{
	int	i, y;
	int	len = strlen(compare);
	if(len > 64) 
	{
		Dprintf("StringSearch:: string size over\n");
		return(0);
	}
	char	buf[64];
	for(i=0; i<strlen(src); i++)
	{
		if(i+len > strlen(src)) break;
		for(y=0; y<len; y++) buf[y] = src[i+y];
		buf[y]=0;
		if(!strcmp(buf, compare))
		{
			//Dprintf("StringSearch:: Search ok[%s]index=%d\n", buf, i);
			return(1);
		}
	}
	return(0);
}

// 주어진 특정문자열에서 주어진 특정문자열까지의 문자열을 버퍼에 넣어서 리턴한다.
// '='로시작해서 0x0d로 끝나는 문자열을 찾고 싶을때 사용
// "12345yhjkk==dsdcnsdkjcndj&" 중에서 =로시작 &로 끝난사이의 내용추출
int StringSeparator(const char *src, char *buf, char start, char end)
{
	int	begin=0;
	int	cnt=0;
	int 	len = strlen(src);
	
	for(int i=0; i<len; i++)
	{
		if(src[i] == start && begin==0)		//!< 시작코드를 찾았으면 시작한다.
		{
			begin=1;
			Dprintf("StringSeparator::First Code find i=%d %02x\n", i, start);
		}
		else
		{
			if(begin==1)
			{
				if(src[i] == end)
				{
					buf[cnt]=0;
					Dprintf("StringSeparator::End Code find i=%d %02x\n", i, end);
					return(cnt);
				}
				else
				{
					buf[cnt++] = src[i];
				}
			}
		}
	}
	buf[cnt-1]=0;
	return(0);
}

//!< "192.168.0.1" 등의 IP이름을 uint8_t 로 분해할때 사용한다.
int inet_pton4(char *ip, uint8_t *sbuf)
{
	int 	i;
	char	buf[12];
	int	cnt=0;
	int	digit=0;

	for(i=0; i<strlen(ip); i++)
	{
		if(ip[i] != '.')
			buf[cnt++] = ip[i];
		else
		{
			buf[cnt]=0;
			sbuf[digit++] = (char)atoi(buf);
			cnt=0;
		}
	}
	buf[cnt]=0;
	sbuf[digit++] = (char)atoi(buf);
	return(1);
}

int inet_ntop4(char *ip, uint8_t *sbuf)
{
	char	buf[32];
	sprintf(buf, "%d.",sbuf[0]);
	strcpy(ip, buf);
	sprintf(buf, "%d.",sbuf[1]);
	strcat(ip, buf);
	sprintf(buf, "%d.",sbuf[2]);
	strcat(ip, buf);
	sprintf(buf, "%d",sbuf[3]);
	strcat(ip, buf);
	return(1);
}


/**
	@brief 주어진 문자열에서 주어진 구분자로 2개의 문자로 구분한다.
				구분자가  있으면 1을 리턴하고 없으면 0을 리턴한다.
*/

int Words_Separator(const char *src, char *firstname, char *secondname, char sep)
{
	int	ret=0;
	int	delimiter=0;
	int	cnt=0;
	
	for(int i=0; i<strlen(src); i++)	//!< 전체문자열중에서 구분자 있는지를 검사한다.
	{
		if(src[i] == sep) delimiter=1;
	}
	if(delimiter==0)	//!< 구분자가 없으면 전체가 파일명이므로
	{
		strcpy(secondname, src);
		ret=0;
	}
	else			// 구분자가 있으면
	{
		for(int i=0; i<strlen(src); i++)	//!< 전체문자열중에서 구분자 가있는지를 검사한다.
		{
			if(delimiter==1)		//!< 처음은 앞의 문자열을  추출한다.
			{
				if(src[i] == sep) 
				{
					firstname[i] = 0;
					delimiter=2;	//다음작업
					cnt=0;
				}
				else 
				{
					firstname[i] = src[i];
					ret=1;
				}
			}
			else //!<디렉토리추출이 끝나고 파일명을 추출한다.
			{
				secondname[cnt++] = src[i];
			}
		}		
		secondname[cnt]=0;
	}
	return(ret);
}

/**
  * strlcpy - Copy a %NUL terminated string into a sized buffer
  * @dest: Where to copy the string to
  * @src: Where to copy the string from
  * @size: size of destination buffer
  *
  * Compatible with *BSD: the result is always a valid
  * NUL-terminated string that fits in the buffer (unless,
  * of course, the buffer size is zero). It does not pad
  * out the result like strncpy() does.
  */
//!< 리눅스의 커널소스이다.
#if 0
size_t strlcpy(char *dest, const char *src, size_t size)
{
	size_t ret = strlen(src);

	if (size) 
	{
		size_t len = (ret >= size) ? size - 1 : ret;
		memcpy(dest, src, len);
		dest[len] = '\0';
	}
	 return ret;
}
#endif


int IntCompare(const void *key, const void *element)
{
    return *(int *)key - *(int *)element;
}

int FloatCompare(const void *key, const void *element)
{
    return *(float *)key - *(float *)element;
}

/* mbtowc example */
void printbuffer (const char* pt, size_t max)
{
	int length;
	wchar_t dest;

	mbtowc (NULL, NULL, 0);  /* reset mbtowc */

	while (max>0) 
	{
		length = mbtowc(&dest,pt,max);
		if (length<1) break;
		Dprintf ("[%lc]",dest);
		pt+=length; max-=length;
	}
}

void	MyTestFunc1(void)
{
	char szOrbits[] = "90613.305 365.24 123.45";
	char * pEnd;
	char * pEnd1;
	long double f1, f2, f3;
	f1 = strtold (szOrbits, &pEnd);
	f2 = strtold (pEnd, &pEnd1);
	f3 = strtold (pEnd1, NULL);
	float f4;
	Dprintf("f1 = %Lf f2 = %Lf  f3 = %f f4 = %f\n", f1, f2, f3, f4);

	Dprintf ("Randum = %d Ver=%d\n", rand(), __C_library_version_number());
	const char str [] = "대한민국";

	printbuffer (str,sizeof(str));
	
    //qsort((void *)Temp_Buf1, sizeof(Temp_Buf1) / sizeof(int32_t), sizeof(int32_t), FloatCompare);	
}


 /*
  * Program to convert float number to string without using sprintf
  */

# define PRECISION 5

char *floatToString(float num)
{
   int whole_part = num;
   int digit = 0, reminder =0;
   int log_value = log10(num), index = log_value;
   long wt =0;

   // String containg result
   char* str = malloc(20);	//new char[20];

   //Initilise stirng to zero
   memset(str, 0 ,20);

   //Extract the whole part from float num
   for(int  i = 1 ; i < log_value + 2 ; i++)
   {
       wt  =  pow(10.0,i);
       reminder = whole_part  %  wt;
       digit = (reminder - digit) / (wt/10);

       //Store digit in string
       str[index--] = digit + 48;              // ASCII value of digit  = digit + 48
       if (index == -1)
          break;    
   }

    index = log_value + 1;
    str[index] = '.';

   float fraction_part  = num - whole_part;
   float tmp1 = fraction_part,  tmp =0;

   //Extract the fraction part from  num
   for( int i= 1; i < PRECISION; i++)
   {
      wt =10; 
      tmp  = tmp1 * wt;
      digit = tmp;

      //Store digit in string
      str[++index] = digit +48;           // ASCII value of digit  = digit + 48
      tmp1 = tmp - digit;
   }    

   return str;
}

// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}
 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating point number to string.
//!< 사용하기 좋은 함수로 등록된다.
void ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
 
    // Extract floating part
    float fpart = n - (float)ipart;
 
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
 
    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot
 
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
 
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

#if 0
/**
 * Double to ASCII 이함수는 실험결과 정밀도가 떨어진다.
 */
char * dtoa(char *s, double n) {
    // handle special cases
    if (isnan(n)) {
        strcpy(s, "nan");
    } else if (isinf(n)) {
        strcpy(s, "inf");
    } else if (n == 0.0) {
        strcpy(s, "0");
    } else {
        int digit, m, m1;
        char *c = s;
        int neg = (n < 0);
        if (neg)
            n = -n;
        // calculate magnitude
        m = log10(n);
        int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
        if (neg)
            *(c++) = '-';
        // set up for scientific notation
        if (useExp) {
            if (m < 0)
               m -= 1.0;
            n = n / pow(10.0, m);
            m1 = m;
            m = 0;
        }
        if (m < 1.0) {
            m = 0;
        }
        // convert the number
        while (n > PRECISION || m >= 0) {
            double weight = pow(10.0, m);
            if (weight > 0 && !isinf(weight)) {
                digit = floor(n / weight);
                n -= (digit * weight);
                *(c++) = '0' + digit;
            }
            if (m == 0 && n > 0)
                *(c++) = '.';
            m--;
        }
        if (useExp) {
            // convert the exponent
            int i, j;
            *(c++) = 'e';
            if (m1 > 0) {
                *(c++) = '+';
            } else {
                *(c++) = '-';
                m1 = -m1;
            }
            m = 0;
            while (m1 > 0) {
                *(c++) = '0' + m1 % 10;
                m1 /= 10;
                m++;
            }
            c -= m;
            for (i = 0, j = m-1; i<j; i++, j--) {
                // swap without temporary
                c[i] ^= c[j];
                c[j] ^= c[i];
                c[i] ^= c[j];
            }
            c += m;
        }
        *(c) = '\0';
    }
    return s;
}
#endif

/**
	src 로 주어진 문자열중 , 로 구분된 3개의 문자열을 분리한다.
	first, second, third 는 길이가 부족하면  치명적인 에러가 발생할수 있다.
*/
int StringCommaSeparator(const char *src, char *first, char *second, char *third)
{
	int	len = strlen(src);	//원본의 길이를 계산한다.
	int	i;
	int	string_no=0;
	int	cnt=0;
	int	start=0;
	
	for(i=0; i<len; i++)
	{
		if(start==0)
		{
			if(src[i] !='?') continue;
			else 
			{
				start=1;
				i++;
			}
		}
		if(start==1)
		{
			if(src[i] != ',')
			{
				if(string_no==0) first[cnt++] = src[i];
				else if(string_no==1) second[cnt++] = src[i];
				else if(string_no==2) 
				{
					if(src[i]==0x0d || src[i]==0x0a)	
					{
						third[cnt++] = 0;
						break;
					}
					else third[cnt++] = src[i];
				}
			}
			else
			{
				if(string_no==0) first[cnt] = 0;
				else if(string_no==1) second[cnt] = 0;
				else if(string_no==2 ) third[cnt] = 0;
				cnt=0;
				string_no++;
				if(string_no > 2)  
				{
					Dprintf("StringCommaSeparator::컴마가 2개보다 많음\n");
					return(0);
				}
			}
		}
	}
	third[cnt] = 0;
	Dprintf("StringCommaSeparator:: 1--->%s\n", first);
	Dprintf("StringCommaSeparator:: 2--->%s\n", second);
	Dprintf("StringCommaSeparator:: 3--->%s\n", third);
	return(1);
}


//Main program
#if 0
void MyTestFunc(void)
{
    //int i;
    float f = 123456.789;
    char* str =  floatToString(f);
	char	buf[64];
	ftoa(777.123, buf, 5);
	Dprintf("%s %s\n", str, buf);
	//
	Dprintf("FloatToStr=>%s \n",  buf);
	if(isnan(f)) Dprintf("error");
}
#endif

#if 0
void MyTestFunc(void)
{
	char	fbuf[64] = { 0 };
	char	sbuf[64] = { 0 };
	int retval = Words_Separator("Test:program",  fbuf, sbuf, ':');
	strcat(fbuf,":");
	Dprintf("Words_Separator:: %s <-> %s\n", fbuf, sbuf);	
}
#endif
#ifdef __cplusplus
}
#endif
