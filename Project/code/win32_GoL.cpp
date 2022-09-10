#include <windows.h>
#include <stdint.h>
#include <math.h>

#define internal_function static
#define local_persist static
#define global_variable static

struct offscreen_buffer
{
    BITMAPINFO Info;
    void* Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
};

struct win32_window_dimension
{
    int Width;
    int Height;
};

global_variable bool running;
global_variable offscreen_buffer Global_Backbuffer;

win32_window_dimension Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Dimension;

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Dimension.Width = ClientRect.right - ClientRect.left;
    Dimension.Height = ClientRect.bottom - ClientRect.top;

    return Dimension;
}

internal_function void RenderGradient(offscreen_buffer Buffer, int xOffset, int yOffset)
{
    int width = Buffer.Width;
    int height = Buffer.Height;

    int pitch = width * Buffer.BytesPerPixel;
    uint8_t* row = (uint8_t*)Buffer.Memory;
    for (int y = 0; y < height; y++)
    {
        uint32_t* pixel = (uint32_t*)row;
        for (int x = 0; x < width; x++)
        {
            uint8_t blue = (uint8_t)(x + xOffset);
            uint8_t red = (uint8_t)(y + yOffset);

            *pixel++ = ((red << 16) | blue);
        }
        row += pitch;
    }
}

internal_function void RenderCells(offscreen_buffer Buffer, int xOffset, int yOffset)
{
    int width = Buffer.Width;
    int height = Buffer.Height;

    int pitch = width * Buffer.BytesPerPixel;
    uint8_t* row = (uint8_t*)Buffer.Memory;
    for (int y = 0; y < height; y++)
    {
        uint32_t* pixel = (uint32_t*)row;
        for (int x = 0; x < width; x++)
        {
            uint8_t blue = (uint8_t)(255);
            uint8_t green = (uint8_t)(255);
            uint8_t red = (uint8_t)(255);
            int xFloor = floor(x/5);
            int yFloor = floor(y/5);

            if (xFloor % 2 == 0 && yFloor % 2 == 0)
            {
                blue  = (uint8_t)(0);
                green = (uint8_t)(0);
                red   = (uint8_t)(0);
            }
            else if (xFloor % 2 != 0 && yFloor % 2 != 0)
            {
                blue  = (uint8_t)(0);
                green = (uint8_t)(0);
                red   = (uint8_t)(0);
            }

            *pixel++ = ((red << 16) | (green << 8) | blue);
        }
        row += pitch;
    }
}


internal_function void Win32ResizeDIBSection(offscreen_buffer *Buffer, int width, int height)
{

    if (Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }

    Buffer->Width = width;
    Buffer->Height = height;
    Buffer->BytesPerPixel = 4;

    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height; // Negative biHeight is to make Windows treat bitmap as top-down instead of bottom-up, meaning first bytes are for top-left pixel
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32; // Only need 24 bits but add a buffer of 1 byte so this takes up 4x8 bits in memory instead of 3x4
    Buffer->Info.bmiHeader.biCompression = BI_RGB;
    
    int BitmapMemorySize = (Buffer->Width*Buffer->Height)* Buffer->BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

    Buffer->Pitch = Buffer->Width * Buffer->BytesPerPixel;
}

internal_function void Win32UpdateWindow(HDC DeviceContext,
                                         int WindowWidth, int WindowHeight,
                                         offscreen_buffer Buffer)
{
    StretchDIBits(DeviceContext,
                  0, 0, WindowWidth, WindowHeight,
                  0, 0, Buffer.Width, Buffer.Height,
                  Buffer.Memory,
                  &Buffer.Info,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}

LRESULT CALLBACK Win32MainWindowCallback(HWND   window, //hwnd
                                         UINT   message, //uMsg
                                         WPARAM wParam,
                                         LPARAM lParam)
{
    LRESULT Result = 0;
    
    switch (message)
    {

        case WM_SIZE:
        {
            OutputDebugStringA("WM_SIZE\n");
            break;
        }

        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
            running = false;
            break;
        }

        case WM_CLOSE:
        {            
            OutputDebugStringA("WM_CLOSE\n");
            running = false;
            break;
        }

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
            break;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC DeviceContext =  BeginPaint(window, &paint);
            int width = paint.rcPaint.right - paint.rcPaint.left;
            int height = paint.rcPaint.bottom - paint.rcPaint.top;
            win32_window_dimension Dimension = Win32GetWindowDimension(window);

            Win32UpdateWindow(DeviceContext, Dimension.Width, Dimension.Height, Global_Backbuffer);
            EndPaint(window, &paint);
            break;
        }

        default:
        {
            Result = DefWindowProc(window, message, wParam, lParam);
            break;
        }
    }

    return(Result);
} 

int CALLBACK WinMain(HINSTANCE Instance,
                     HINSTANCE PrevInstance,
                     LPSTR     lpCmdLine,
                     int       nShowCmd)
{
    WNDCLASS WindowClass = {};

   Win32ResizeDIBSection(&Global_Backbuffer, 1288, 720);

    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "GoL";
    
    if (RegisterClass(&WindowClass))
    {
        HWND Window = CreateWindowEx(0,
                                    WindowClass.lpszClassName,
                                    "GoL",
                                    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                    CW_USEDEFAULT,
                                    CW_USEDEFAULT,
                                    CW_USEDEFAULT,
                                    CW_USEDEFAULT,
                                    0,
                                    0,
                                    Instance,
                                    0);
        if (Window)
        {
            running = true;
            MSG message;
            int xOffset = 0;
            int yOffset = 0;

            while (running)
            {
                while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
                {
                    if (message.message == WM_QUIT)
                    {
                        running = false;
                    }
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
                
                RenderCells(Global_Backbuffer, xOffset, yOffset);

                HDC DeviceContext = GetDC(Window);
                win32_window_dimension Dimension = Win32GetWindowDimension(Window);
                Win32UpdateWindow(DeviceContext, Dimension.Width, Dimension.Height, Global_Backbuffer);
                ReleaseDC(Window, DeviceContext);

                xOffset++;
                yOffset++;
            }
            
        }
        else
        {
            // TODO: Log when logging system implemented
        }
    }
    else
    {
        // TODO: Log when logging system implemented
    }

    return 0;
}