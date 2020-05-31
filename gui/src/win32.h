#pragma once
#include <stdio.h>
#define STRUCT typedef struct
#define ENUM typedef enum
#define UNION typedef union
#ifndef __cplusplus
typedef _Bool bool;
#define true 1
#define false 0
#endif
#include <stdint.h>
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef float f32;
typedef double f64;
#define COUNT_OF(___arr) ((sizeof(___arr)) / (sizeof(___arr[0])))
#define BYTES(n) 	 ((u64)n)
#define KILOBYTES(n) (1024 * BYTES(n))
#define MEGABYTES(n) (1024 * KILOBYTES(n))
#define GIGABYTES(n) (1024 * MEGABYTES(n))
#define TERABYTES(n) (1024 * GIGABYTES(n))
#define VECTOR_TYPE(length) __attribute__((ext_vector_type(length)))
#define VECTOR_TYPE_ALIGNMENT(length, alignment) __attribute__((ext_vector_type(length))) __attribute__((aligned(alignment)))
#define INTRINSIC_INLINE __inline__
#define ALIGNED(bytes) __declspec(align(bytes))

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
char i_WM_Strings[100000][256];
#define FILL_ARR(msg) strcpy(i_WM_Strings[msg], #msg)
#define abs_T(x) (((x) > 0) ? (x) : -(x))
#define WIN32_WIDTH(_struct) (_struct.right - _struct.left)
#define WIN32_HEIGHT(_struct) (_struct.bottom - _struct.top)

bool g_Running = true;
HWND g_WindowHandle = NULL;

void initWMStrings(void)
{
	FILL_ARR(WM_WINDOWPOSCHANGING);
	FILL_ARR(WM_MOUSEMOVE);
	FILL_ARR(WM_NULL);
	FILL_ARR(WM_CREATE);
	FILL_ARR(WM_DESTROY);
	FILL_ARR(WM_MOVE);
	FILL_ARR(WM_SIZE);
	FILL_ARR(WM_ACTIVATE);
	FILL_ARR(WM_SETFOCUS);
	FILL_ARR(WM_KILLFOCUS);
	FILL_ARR(WM_ENABLE);
	FILL_ARR(WM_SETREDRAW);
	FILL_ARR(WM_SETTEXT);
	FILL_ARR(WM_GETTEXT);
	FILL_ARR(WM_GETTEXTLENGTH);
	FILL_ARR(WM_PAINT);
	FILL_ARR(WM_CLOSE);
	FILL_ARR(WM_QUERYENDSESSION);
	FILL_ARR(WM_QUIT);
	FILL_ARR(WM_QUERYOPEN);
	FILL_ARR(WM_ERASEBKGND);
	FILL_ARR(WM_SYSCOLORCHANGE);
	FILL_ARR(WM_ENDSESSION);
	FILL_ARR(WM_SHOWWINDOW);
	FILL_ARR(WM_WININICHANGE);
	FILL_ARR(WM_SETTINGCHANGE);
	FILL_ARR(WM_DEVMODECHANGE);
	FILL_ARR(WM_ACTIVATEAPP);
	FILL_ARR(WM_FONTCHANGE);
	FILL_ARR(WM_TIMECHANGE);
	FILL_ARR(WM_CANCELMODE);
	FILL_ARR(WM_SETCURSOR);
	FILL_ARR(WM_MOUSEACTIVATE);
	FILL_ARR(WM_CHILDACTIVATE);
	FILL_ARR(WM_QUEUESYNC);
	FILL_ARR(WM_GETMINMAXINFO);
	FILL_ARR(WM_PAINTICON);
	FILL_ARR(WM_ICONERASEBKGND);
	FILL_ARR(WM_NEXTDLGCTL);
	FILL_ARR(WM_SPOOLERSTATUS);
	FILL_ARR(WM_DRAWITEM);
	FILL_ARR(WM_MEASUREITEM);
	FILL_ARR(WM_DELETEITEM);
	FILL_ARR(WM_VKEYTOITEM);
	FILL_ARR(WM_CHARTOITEM);

	FILL_ARR(WM_SETFONT);
	FILL_ARR(WM_GETFONT);
	FILL_ARR(WM_SETHOTKEY);
	FILL_ARR(WM_GETHOTKEY);
	FILL_ARR(WM_QUERYDRAGICON);
	FILL_ARR(WM_COMPAREITEM);
	FILL_ARR(WM_COMPACTING);
	FILL_ARR(WM_WINDOWPOSCHANGED);
	FILL_ARR(WM_POWER);
	FILL_ARR(WM_COPYDATA);
	FILL_ARR(WM_CANCELJOURNAL);
	FILL_ARR(WM_NOTIFY);
	FILL_ARR(WM_INPUTLANGCHANGEREQUEST);
	FILL_ARR(WM_INPUTLANGCHANGE);
	FILL_ARR(WM_TCARD);
	FILL_ARR(WM_HELP);
	FILL_ARR(WM_USERCHANGED);
	FILL_ARR(WM_NOTIFYFORMAT);
	FILL_ARR(WM_CONTEXTMENU);
	FILL_ARR(WM_STYLECHANGING);
	FILL_ARR(WM_STYLECHANGED);
	FILL_ARR(WM_DISPLAYCHANGE);
	FILL_ARR(WM_GETICON);
	FILL_ARR(WM_SETICON);
	FILL_ARR(WM_NCCREATE);
	FILL_ARR(WM_NCDESTROY);
	FILL_ARR(WM_NCCALCSIZE);
	FILL_ARR(WM_NCHITTEST);
	FILL_ARR(WM_NCPAINT);
	FILL_ARR(WM_NCACTIVATE);
	FILL_ARR(WM_GETDLGCODE);
	FILL_ARR(WM_NCMOUSEMOVE);
	FILL_ARR(WM_NCLBUTTONDOWN);
	FILL_ARR(WM_NCLBUTTONUP);
	FILL_ARR(WM_NCLBUTTONDBLCLK);
	FILL_ARR(WM_NCRBUTTONDOWN);
	FILL_ARR(WM_NCRBUTTONUP);
	FILL_ARR(WM_NCRBUTTONDBLCLK);
	FILL_ARR(WM_NCMBUTTONDOWN);
	FILL_ARR(WM_NCMBUTTONUP);
	FILL_ARR(WM_NCMBUTTONDBLCLK);

	FILL_ARR(WM_KEYFIRST);
	FILL_ARR(WM_KEYDOWN);
	FILL_ARR(WM_KEYUP);
	FILL_ARR(WM_CHAR);
	FILL_ARR(WM_DEADCHAR);
	FILL_ARR(WM_SYSKEYDOWN);
	FILL_ARR(WM_SYSKEYUP);
	FILL_ARR(WM_SYSCHAR);
	FILL_ARR(WM_SYSDEADCHAR);
	FILL_ARR(WM_KEYLAST);
	FILL_ARR(WM_IME_STARTCOMPOSITION);
	FILL_ARR(WM_IME_ENDCOMPOSITION);
	FILL_ARR(WM_IME_COMPOSITION);
	FILL_ARR(WM_IME_KEYLAST);

	FILL_ARR(WM_INITDIALOG);
	FILL_ARR(WM_COMMAND);
	FILL_ARR(WM_SYSCOMMAND);
	FILL_ARR(WM_TIMER);
	FILL_ARR(WM_HSCROLL);
	FILL_ARR(WM_VSCROLL);
	FILL_ARR(WM_INITMENU);
	FILL_ARR(WM_INITMENUPOPUP);
	FILL_ARR(WM_MENUSELECT);
	FILL_ARR(WM_MENUCHAR);
	FILL_ARR(WM_ENTERIDLE);

	FILL_ARR(WM_CTLCOLORMSGBOX);
	FILL_ARR(WM_CTLCOLOREDIT);
	FILL_ARR(WM_CTLCOLORLISTBOX);
	FILL_ARR(WM_CTLCOLORBTN);
	FILL_ARR(WM_CTLCOLORDLG);
	FILL_ARR(WM_CTLCOLORSCROLLBAR);
	FILL_ARR(WM_CTLCOLORSTATIC);

	FILL_ARR(WM_MOUSEFIRST);
	FILL_ARR(WM_LBUTTONDOWN);
	FILL_ARR(WM_LBUTTONUP);
	FILL_ARR(WM_LBUTTONDBLCLK);
	FILL_ARR(WM_RBUTTONDOWN);
	FILL_ARR(WM_RBUTTONUP);
	FILL_ARR(WM_RBUTTONDBLCLK);
	FILL_ARR(WM_MBUTTONDOWN);
	FILL_ARR(WM_MBUTTONUP);
	FILL_ARR(WM_MBUTTONDBLCLK);
	FILL_ARR(WM_MOUSEWHEEL);
	FILL_ARR(WM_MOUSEHWHEEL);

	FILL_ARR(WM_PARENTNOTIFY);
	FILL_ARR(WM_ENTERMENULOOP);
	FILL_ARR(WM_EXITMENULOOP);
	FILL_ARR(WM_NEXTMENU);
	FILL_ARR(WM_SIZING);
	FILL_ARR(WM_CAPTURECHANGED);
	FILL_ARR(WM_MOVING);
	FILL_ARR(WM_POWERBROADCAST);
	FILL_ARR(WM_DEVICECHANGE);

	FILL_ARR(WM_MDICREATE);
	FILL_ARR(WM_MDIDESTROY);
	FILL_ARR(WM_MDIACTIVATE);
	FILL_ARR(WM_MDIRESTORE);
	FILL_ARR(WM_MDINEXT);
	FILL_ARR(WM_MDIMAXIMIZE);
	FILL_ARR(WM_MDITILE);
	FILL_ARR(WM_MDICASCADE);
	FILL_ARR(WM_MDIICONARRANGE);
	FILL_ARR(WM_MDIGETACTIVE);

	FILL_ARR(WM_MDISETMENU);
	FILL_ARR(WM_ENTERSIZEMOVE);
	FILL_ARR(WM_EXITSIZEMOVE);
	FILL_ARR(WM_DROPFILES);
	FILL_ARR(WM_MDIREFRESHMENU);

	FILL_ARR(WM_IME_SETCONTEXT);
	FILL_ARR(WM_IME_NOTIFY);
	FILL_ARR(WM_IME_CONTROL);
	FILL_ARR(WM_IME_COMPOSITIONFULL);
	FILL_ARR(WM_IME_SELECT);
	FILL_ARR(WM_IME_CHAR);
	FILL_ARR(WM_IME_KEYDOWN);
	FILL_ARR(WM_IME_KEYUP);

	FILL_ARR(WM_MOUSEHOVER);
	FILL_ARR(WM_NCMOUSELEAVE);
	FILL_ARR(WM_MOUSELEAVE);

	FILL_ARR(WM_CUT);
	FILL_ARR(WM_COPY);
	FILL_ARR(WM_PASTE);
	FILL_ARR(WM_CLEAR);
	FILL_ARR(WM_UNDO);

	FILL_ARR(WM_RENDERFORMAT);
	FILL_ARR(WM_RENDERALLFORMATS);
	FILL_ARR(WM_DESTROYCLIPBOARD);
	FILL_ARR(WM_DRAWCLIPBOARD);
	FILL_ARR(WM_PAINTCLIPBOARD);
	FILL_ARR(WM_VSCROLLCLIPBOARD);
	FILL_ARR(WM_SIZECLIPBOARD);
	FILL_ARR(WM_ASKCBFORMATNAME);
	FILL_ARR(WM_CHANGECBCHAIN);
	FILL_ARR(WM_HSCROLLCLIPBOARD);
	FILL_ARR(WM_QUERYNEWPALETTE);
	FILL_ARR(WM_PALETTEISCHANGING);
	FILL_ARR(WM_PALETTECHANGED);

	FILL_ARR(WM_HOTKEY);
	FILL_ARR(WM_PRINT);
	FILL_ARR(WM_PRINTCLIENT);

	FILL_ARR(WM_HANDHELDFIRST);
	FILL_ARR(WM_HANDHELDLAST);
	FILL_ARR(WM_PENWINFIRST);
	FILL_ARR(WM_PENWINLAST);

	FILL_ARR(WM_USER);
	FILL_ARR(WM_APP);
}

	//fprintf(i_SwitchRegister, "case (%s):\n{\n\t\n} break;\n", i_WM_Strings[message]);
LRESULT win32_windowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// First closing message
		case (WM_CLOSE):
		{
			g_Running = false;
		} break;
		// Second closing message
		case (WM_DESTROY):
		{
			DestroyWindow(window);
			ExitProcess(0);
		} break;
		case (WM_WINDOWPOSCHANGING):
		{
			WINDOWPOS* newPosition = (WINDOWPOS*)lParam;

			RECT windowRect;
			RECT clientRect;
			GetWindowRect(window, &windowRect);
			GetClientRect(window, &clientRect);

			s32 clientWidth = WIN32_WIDTH(clientRect);
			s32 clientHeight = WIN32_HEIGHT(clientRect);
			s32 widthAdd = WIN32_WIDTH(windowRect) - clientWidth;
			s32 heightAdd = WIN32_HEIGHT(windowRect) - clientHeight;

			s32 renderWidth = 16;
			s32 renderHeight = 9;

			if (renderWidth > 0 && renderHeight)
			{
				s32 sugX = newPosition->cx;
				s32 sugY = newPosition->cy;

				s32 newCx = (renderWidth * (sugY - heightAdd)) / renderHeight;
				s32 newCy = (renderHeight * (sugX - widthAdd)) / renderWidth;

				if (abs_T(sugX - newCx) < abs_T(sugY - newCy))
				{
					newPosition->cx = newCx + widthAdd;
				}
				else
				{
					newPosition->cy = newCy + heightAdd;
				}
			}
			return DefWindowProcA(window, message, wParam, lParam);
		} break;
		case (WM_WINDOWPOSCHANGED):
		{
			WINDOWPOS* newPos = (WINDOWPOS*)lParam;
			bool fullScreen = false;
			MONITORINFO monitorInfo = { sizeof(monitorInfo) };
			if (GetMonitorInfoA(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &monitorInfo))
			{
				s32 monitorWidth = WIN32_WIDTH(monitorInfo.rcMonitor);
				s32 monitorHeight = WIN32_HEIGHT(monitorInfo.rcMonitor);
				fullScreen =
					monitorInfo.rcMonitor.left == newPos->x &&
					monitorInfo.rcMonitor.top == newPos->y &&
					monitorWidth == newPos->cx &&
					monitorHeight == newPos->cy;
			}

			DWORD oldStyle = GetWindowLong(window, GWL_STYLE);
			DWORD fullScreenStyle = oldStyle & ~WS_OVERLAPPEDWINDOW;
			DWORD windowedStyle = oldStyle | WS_OVERLAPPEDWINDOW;
			DWORD newStyle = fullScreen ? fullScreenStyle : windowedStyle;

			if (newStyle != oldStyle)
			{
				SetWindowLongA(window, GWL_STYLE, newStyle);
			}

			return DefWindowProcA(window, message, wParam, lParam);
		} break;
		case (WM_NCHITTEST):
		case (WM_GETMINMAXINFO):
		case (WM_NCCREATE):
		case (WM_NCCALCSIZE):
		case (WM_CREATE):
		case (WM_SHOWWINDOW):
		{

		} break;
		case (WM_ACTIVATEAPP):
		{

		} break;
		case (WM_NCACTIVATE):
		{

		} break;
		case (WM_GETICON):
		{

		} break;
		case (WM_ACTIVATE):
		{

		} break;
		case (WM_IME_SETCONTEXT):
		{

		} break;
		case (WM_IME_NOTIFY):
		{

		} break;
		case (WM_SETFOCUS):
		{

		} break;
		case (WM_NCPAINT):
		{

		} break;
		case (WM_ERASEBKGND):
		{

		} break;
		case (WM_SIZE):
		{

		} break;
		case (WM_MOVE):
		{

		} break;
		case (WM_PAINT):
		{

		} break;
		case (WM_SETCURSOR):
		{

		} break;
		case (WM_NCMOUSEMOVE):
		{

		} break;
		case (WM_NCMOUSELEAVE):
		{

		} break;
		case (WM_MOUSEMOVE):
		{

		} break;
		case (WM_NCLBUTTONDOWN):
		{

		} break;
		case (WM_CAPTURECHANGED):
		{

		} break;
		case (WM_SYSCOMMAND):
		{

		} break;
		case (WM_KILLFOCUS):
		{

		} break;
		case (WM_NCDESTROY):
		{

		} break;
		case (WM_ENTERSIZEMOVE):
		{

		} break;
		case (WM_MOVING):
		{

		} break;
		case (WM_MOUSEACTIVATE):
		{

		} break;
		case (WM_SIZING):
		{

		} break;
		case (WM_EXITSIZEMOVE):
		{

		} break;
		case (WM_NCLBUTTONDBLCLK):
		{

		} break;
		case (WM_LBUTTONUP):
		{

		} break;
		case (WM_QUERYOPEN):
		{

		} break;
		case (WM_RBUTTONDOWN):
		{

		} break;
		case (WM_RBUTTONUP):
		{

		} break;
		case (WM_CONTEXTMENU):
		{

		} break;
		case (WM_MBUTTONDOWN):
		{

		} break;
		case (WM_MBUTTONUP):
		{

		} break;
		case (WM_KEYDOWN):
		{

		} break;
		case (WM_KEYUP):
		{

		} break;
		case (WM_CHAR):
		{

		} break;
		case (WM_LBUTTONDOWN):
		{

		} break;
		case (WM_ENTERMENULOOP):
		{

		} break;
		case (WM_INITMENU):
		{

		} break;
		case (WM_MENUSELECT):
		{

		} break;
		case (WM_INITMENUPOPUP):
		{

		} break;
		case (WM_MOUSEWHEEL):
		{

		} break;
		case (WM_MOUSEHWHEEL):
		{

		} break;
		case (WM_SYSKEYDOWN):
		{

		} break;
		case (WM_SYSKEYUP):
		{

		} break;
		case (WM_ENTERIDLE):
		{

		} break;
		case (WM_EXITMENULOOP):
		{

		} break;
		case (WM_INPUTLANGCHANGE):
		{

		} break;
		case (WM_HELP):
		{

		} break;
		case (WM_CANCELMODE):
		{

		} break;
		case (WM_NCRBUTTONDOWN):
		{

		} break;
#ifndef WM_KEYF1
#define WM_KEYF1 0x4D
#endif
		case (WM_KEYF1):
		case (799):
		case (49385):
		case (49361):
#define WM_NCUAHDRAWCAPTION 0xAE
		case (WM_NCUAHDRAWCAPTION):
		case (144):
		{
		} break;

		default:
		{
			printf("%u : %s\n", message, i_WM_Strings[message]);
			//fprintf(i_SwitchRegister, "case (%s):\n{\n\t\n}\n", i_WM_Strings[message]);
		} break;
	}
	return DefWindowProcA(window, message, wParam, lParam);
}

HWND win32_createWindow(HINSTANCE instance, WNDPROC windowProcedure, s32 width, s32 height, const char* title, void* pData)
{
	initWMStrings();
	WNDCLASSEX windowClass =
	{
		.cbWndExtra = 64,
		.cbSize = sizeof(windowClass),
		.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
		.lpfnWndProc = windowProcedure,
		.hCursor = LoadCursor(NULL, IDC_ARROW),
		.lpszClassName = "Window Class",
	};
	RegisterClassExA(&windowClass);

	RECT windowRect = { 0, 0, width, height };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	g_WindowHandle = CreateWindowA(windowClass.lpszClassName, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		NULL, NULL, instance, pData);

	ShowWindow(g_WindowHandle, SW_SHOW);
	UpdateWindow(g_WindowHandle);

	return g_WindowHandle;
} 
