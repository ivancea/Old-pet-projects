#include<windows.h>
#include<iostream>
#include<list>
#include<map>
#include<thread>
#include<algorithm>
#include <GL/gl.h>

using namespace std;

struct color{
    char r,g,b;
    color():r(0),g(0),b(0){}
    color(char _r,char _g,char _b):r(_r),g(_g),b(_b){}
    operator COLORREF(){return RGB(r,g,b);}
};

struct Event{
    UINT msg;
    WPARAM w;
    LPARAM l;

    Event():msg(0),w(0),l(0){}
    Event(UINT a,WPARAM b,LPARAM c):msg(a),w(b),l(c){}
    operator bool(){return msg||w||l;}
};

enum eventsToHandleMode{
    NOTHING=0,
    KEYS = 1,
    MOUSE = 2,
    KEYSANDMOUSE = 3
};

class Window{
    HWND _hwnd;
	HDC _hdc;
	HGLRC _hrc;
	
    size_t _width, _height;
    list<Event> _events;

    static string generateClassName(){
        static int count = 1;
        return "ICF_CLASS_"+to_string(count++);
    }

    static LRESULT CALLBACK call(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
		static PAINTSTRUCT ps;
		
        if(msg==WM_CREATE){
			SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG)((CREATESTRUCT*)lParam)->lpCreateParams);
        }
		
		Window* wnd = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if(wnd!=0){
			if(find(wnd->eventsToHandle.begin(),wnd->eventsToHandle.end(),msg)!=wnd->eventsToHandle.end())
				wnd->pushEvent(msg,wParam,lParam);
			
			switch(msg){
			case WM_CLOSE: return 0;
			case WM_PAINT:
				BeginPaint(wnd->_hwnd, &ps);
				EndPaint(wnd->_hwnd, &ps);
				break;
			}
		}
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

public:
    list<UINT> eventsToHandle;

    Window():_hwnd(0),_width(0),_height(0){}
    ~Window(){destroy();}
	
	void makeCurrent(){
        if(_hwnd==0) return;
		
		wglMakeCurrent(_hdc, _hrc);
	}

    bool create(std::string title, int width, int height, eventsToHandleMode ev=NOTHING){
        destroy();

        HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
        WNDCLASSEX WndClsEx;
        WndClsEx.cbSize        = sizeof(WNDCLASSEX);
        WndClsEx.style         = CS_HREDRAW | CS_VREDRAW;
        WndClsEx.lpfnWndProc   = &call;
        WndClsEx.cbClsExtra    = 0;
        WndClsEx.cbWndExtra    = 0;
        WndClsEx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
        WndClsEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
        WndClsEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        WndClsEx.lpszMenuName  = NULL;
        WndClsEx.lpszClassName = generateClassName().c_str();
        WndClsEx.hInstance     = hInstance;
        WndClsEx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
        RegisterClassEx(&WndClsEx);

		DWORD style = WS_VISIBLE | WS_SYSMENU | WS_CAPTION;
		
        _hwnd = CreateWindow(WndClsEx.lpszClassName,title.c_str(),style,
                             CW_USEDEFAULT,CW_USEDEFAULT,width,height,NULL,NULL,hInstance,this);

        if(!_hwnd){
            _hwnd = 0;
            return false;
        }
        _width = width;
        _height = height;
        setDisplay(true);
        update();
		
        eventsToHandle.push_back(WM_CLOSE);

        if(ev==MOUSE || ev==KEYSANDMOUSE){
            eventsToHandle.push_back(WM_MOUSEHOVER);
            eventsToHandle.push_back(WM_MOUSEMOVE);
            eventsToHandle.push_back(WM_MOUSELEAVE);
            eventsToHandle.push_back(WM_MOUSEWHEEL);
            eventsToHandle.push_back(WM_RBUTTONDOWN);
            eventsToHandle.push_back(WM_RBUTTONUP);
            eventsToHandle.push_back(WM_LBUTTONDOWN);
            eventsToHandle.push_back(WM_LBUTTONUP);
        }
        if(ev==KEYS || ev==KEYSANDMOUSE){
            eventsToHandle.push_back(WM_KEYDOWN);
            eventsToHandle.push_back(WM_KEYUP);
        }
		
		_hdc = GetDC(_hwnd);
		
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(pfd));
		pfd.nSize        = sizeof(pfd);
		pfd.nVersion     = 1;
		pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
		pfd.iPixelType   = PFD_TYPE_RGBA;
		pfd.cColorBits   = 32;

		int pf = ChoosePixelFormat(_hdc, &pfd);
		SetPixelFormat(_hdc, pf, &pfd);

		DescribePixelFormat(_hdc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
		
		_hrc = wglCreateContext(_hdc);
		
		
		
		makeCurrent();
        return true;
    }

    void destroy(){
        if(_hwnd==0) return;
        char className[1024];
        GetClassName(_hwnd, className, 1024);
		
		ReleaseDC(_hwnd, _hdc);
		wglDeleteContext(_hrc);
		
        DestroyWindow(_hwnd);
        _hwnd=0;
        UnregisterClass(className,(HINSTANCE)GetModuleHandle(NULL));

    };
    void update(){
		glFlush();
	}
    inline void clear(){
		glClear(GL_COLOR_BUFFER_BIT);
    }
    inline void setDisplay(bool display){
        if(display)
            ShowWindow(_hwnd, SW_SHOWNORMAL);
        else
            ShowWindow(_hwnd, SW_HIDE);
    }
    inline void enableInput(bool inputEvents){
        EnableWindow(_hwnd,inputEvents);
    }
    void setTitle(string title){
        SetWindowText(_hwnd, title.c_str());
    }

    /// PIXEL OPERATIONS

    __attribute__ ((deprecated)) inline void setPixel(int x, int y, int r, int g, int b){SetPixel(_hdc,x,y,RGB(r,g,b));}
    __attribute__ ((deprecated)) inline void setPixel(int x, int y, color c){SetPixel(_hdc,x,y,c);}
    __attribute__ ((deprecated)) inline color getPixel(int x, int y){GetPixel(_hdc,x,y);}

    /// NORMAL GETTERS AND EVENTS

    int getWidth(){return _width;}
    int getHeight(){return _height;}
    operator bool(){return _hwnd!=0;}

    void pushEvent(UINT msg,WPARAM w,LPARAM l){
        _events.push_back(Event(msg,w,l));
    }
    Event getEvent(){
        MSG msg;
        if(!PeekMessage(&msg, _hwnd, 0, 0, PM_REMOVE)){
            return Event();
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if(_events.size()==0) return Event();
        Event e = *_events.begin();
        _events.pop_front();
        return e;
    }
};

int main(){
    Window window;
    Event e;
    window.create("Ventana guapa", 500,500, KEYSANDMOUSE);
    glViewport(0, 0, window.getWidth(), window.getHeight());
    glOrtho(0,window.getWidth(), window.getHeight(),0, 0,1);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	Window wnd;
    wnd.create("Ventana chula", 300,200, KEYS);
    glViewport(0, 0, wnd.getWidth(), wnd.getHeight());
    glOrtho(0,wnd.getWidth(), wnd.getHeight(),0, 0,1);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //window.eventsToHandle.push_back(WM_KEYDOWN);
    //window.eventsToHandle.push_back(WM_KEYUP);
    while(window && wnd){
        while(e=window.getEvent()){
            if(e.msg==WM_KEYDOWN){
                if(e.w==VK_SPACE){
                    cout << "Has pulsado espacio." << endl;
                }else if(e.w==VK_SHIFT){
                    window.setTitle("Yeah!");
                }else if(e.w==VK_ESCAPE){
                    window.destroy();
                }
            }else if(e.msg == WM_CLOSE){
				window.destroy();
			}
        }
		
        while(e=wnd.getEvent()){
            if(e.msg==WM_KEYDOWN){
                if(e.w==VK_ESCAPE){
                    wnd.destroy();
                }
            }else if(e.msg == WM_CLOSE){
				wnd.destroy();
			}
        }
		
		wnd.makeCurrent();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glBegin(GL_TRIANGLES);
			glColor3ub(0,0,255);
			glVertex2d(10, 10);
			glColor3ub(0,255,0);
			glVertex2d(275, 20);
			glColor3ub(255,0,0);
			glVertex2d(150, 200);
		glEnd();
		wnd.update();
		
		
		window.makeCurrent();
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glColor3ub(255,0,0);
		glBegin(GL_TRIANGLES);
			glVertex2d(0,0);
			glVertex2d(250, 250);
			glVertex2d(0,500);
		glEnd();
        window.update();
        Sleep(10);
    }
}
