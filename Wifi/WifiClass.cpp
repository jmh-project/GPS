/******************************************************************************
*    	File name	: CPPTest.c 
*		Description : TEECO Project Entry File
*		Company		: idnics co.,
*		Version		: 1.00
******************************************************************************/
//컴파일 옵션에서 라이브러리를 변경해서 사용해야한다.
/******************************************************************************
							Include File
******************************************************************************/
//#include "TEECO_System.h"

#include "Debug_Uart.h"
//#include <stdio.h>
//#include <rt_misc.h>

#include <iostream>
#include <cstdlib>

#include <string>
#include <stddef.h>
#include <cstring>
#include <cstdint>
#include <stdint.h>
#include <vector>
#include <map>

using namespace std;

/******************************************************************************
							Constant & Macros
******************************************************************************/

/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/

extern "C" {
	int CPPTest(void); 
	void Dprintf(char *fmt, ...);     
}
     
//std::string str;     
/******************************************************************************
							Main Program Entry
******************************************************************************/
class TestClass
{
private:
    
protected:
	
public:
    int s;
    TestClass();
    TestClass(int val);
    ~TestClass();
    int Read(void);
    std::string  str;    
    std::wstring  wstr;    
};

//생성자 지정함수
TestClass::TestClass()
{
	str = "Test String";
    s = 100;
}

//생성자 지정함수
TestClass::TestClass(int val)
{
	s = val;
}
//소멸자 지정함수
TestClass::~TestClass(void)
{
}

//!< 함수를 외부에정의하면 콜을하지만 내부에 정의하면 inline 으로 지정된다.
int TestClass::Read(void)
{
    return(1);
}

class CVector 
{
public:
	int x, y;
    CVector () 
	{
		std::cout << "CVector 생성" << std::endl;
	};
    CVector (int a,int b) : x(a), y(b) {}
    CVector operator + (const CVector&);
};

CVector CVector::operator+ (const CVector& param) 
{
	CVector temp;
	temp.x = x + param.x;
	temp.y = y + param.y;
	return temp;
}

class Point2D
{
public:
	Point2D();
	Point2D(int x, int y);
	void SetX(int x);
	void SetY(int y);
	int x() const;
	int y() const;

	Point2D  &operator+=(const Point2D &other)
	{
		xVal+=other.xVal;
		yVal+=other.yVal;
		return *this;
	}
	Point2D  &operator-=(const Point2D &other)
	{
		xVal-=other.xVal;
		yVal-=other.yVal;
		return *this;
	}
private:
	int	xVal;
	int	yVal;
};

inline Point2D operator+(const Point2D &a, Point2D &b)
{
	return Point2D(a.x() + b.x(), a.y() + b.y());
}

template <typename T>
T Add(T num1, T num2)
{
	return num1 + num2;
}

template <class T>
T Add1(T num1, T num2)
{
	return num1 + num2;
}

int CPPTest(void)
{
    TestClass s;
    TestClass *sd = new TestClass(100);
    s.s = 100;
    s.Read();    
    sd->s = 200; 

	CVector foo (3,1);
	CVector bar (1,2);
	CVector result;
	int rr = Add<int>(100, 200);
	result = foo + bar;	
	std::cout << "--------------TEST C++  " << Add<int>(2,5) << rr<<  123 << "   " << s.s << __FILE__ << std::endl;
	printf("==================\n");
	Dprintf("result = %d %d\n", result.x, result.y);
	
    Dprintf("CPP test =  %s %d %d %d \n",  s.str.c_str(), s.Read(), sd->Read(), sd->s);
    return(s.s);
}

class Element{
    public:
    Element();
    int odd;
    int even;
};

Element::Element()
{
      static int a=0; even=a*2; odd=a*2+1; a++;
};

class List
{
    Element list[10];


public: 
    int test(int a, int b, int(List::*func_argument)(const Element & e)); 
    int in_class_odd(const Element & e){ return e.odd; } 
    int in_class_even(const Element & e){ return e.even; } 
};


int List::test(int a, int b,
               int (List::*func_argument)(const Element & e))
{
    int sum=0;
    for (int i=a; i<b; i++){
        sum+=(this->*func_argument)(list[i]);
        Dprintf("%d ,", (this->*func_argument)(list[i]));
    }
    Dprintf("sum: %d\n", sum);
    return sum;
}

extern "C" void Testmain(void);

void Testmain(void)
{
    List l;
    l.test(0, 10, &List::in_class_odd);
    l.test(0, 10, &List::in_class_even);
}


