#include "Debug_Uart.h"
#if 0
#include <iostream>
#include <cstdlib>
#include <string>
#include <stddef.h>
#include <cstring>
#include <cstdint>
#include <stdint.h>
#include <vector>
#include <map>
#include <functional>

using namespace std;

class ObjectWindow
{
private:
    
protected:
	
public:
	void	(ObjectWindow::*pFunc1)(void);
	void	Paint_Event(void)
	{
		//this->TestWindow(0, 0, 0, 0, this->*pFunc1);
	}
	void 	CallFunc(void) 
	{ 
		(this->*pFunc1)(); 
	}
	void   pTest(void)
	{
		Dprintf("TEST\n");
	}		
	void	TestWindow(int sx, int sy, int width, int height, void	(*pFunc)(void))
	{
		(this->*pFunc1)(); 
	}
    ObjectWindow()
	{
		pFunc1 = &ObjectWindow::Paint_Event;
	}
    ~ObjectWindow()
	{
	}
};

void WinTest(void)
{
	void (ObjectWindow::*f3)() = &ObjectWindow::CallFunc;
	
	ObjectWindow win;
	win.CallFunc();
	(win.*f3)();
	//win.TestWindow(0, 0, 100, 100, TestFunc);
	//win.TestWindow(0, 0, 100, 100, win.CallFunc);
	//win.TestWindow(0, 0, 100, 100, static_cast<void	(*pFunc)(void)>(win.CallFunc));
}

//================================================================
struct ltstr
 {
     bool operator() ( const char * s1, const char * s2 ) const
     {
          return strcmp( s1, s2 ) < 0;
     }
 };

 class CFunc
 {
 public:
     typedef int (CFunc::*_Func)( int, int );
     map< const char *, _Func, ltstr > mapFunctor;
     int GetArea( int x, int y );
     void CallFunc( const char * szFuncName, int x, int y ) 
     { 
          (this->*mapFunctor[szFuncName])( x, y ); 
     }

     CFunc();
     ~CFunc();
 };

 CFunc::CFunc()
 {
     // init
     mapFunctor["GetArea"] = &CFunc::GetArea;
     //mapFunctor["GetAreaEx"] = GetAreaEx;
 }
 CFunc::~CFunc()
 {
     // map Å¬¸®¾î
    mapFunctor.clear();
 }
 int CFunc::GetArea( int x, int y )
 {
	 Dprintf("Get Area Exec\n");
     return x * y;
 }

extern "C" {
void MapTest(void)
{
	 CFunc  A;
	A.CallFunc( "GetAreaEx", 10, 20 );	 
}
}

#endif

