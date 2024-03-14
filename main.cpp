#include "mainwindow.h"

#include <QApplication>
#include <windows.h>
#include <stdio.h>

HHOOK keyBoardHook = NULL;
MainWindow *mainwin;
LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam )
{
    if(nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;

        if(ks->vkCode == VK_F1)
        {
            mainwin->startShoot();
            mainwin->show();
            return true;
        }else if(ks->vkCode == VK_ESCAPE)
        {
            //if(mainwin->isActiveWindow())
            mainwin->stopShoot();
        }

    }
    return CallNextHookEx(keyBoardHook,nCode,wParam,lParam);
}

void manageHook(){
    if(keyBoardHook == NULL)
        keyBoardHook = SetWindowsHookEx( WH_KEYBOARD_LL,KeyboardProc,GetModuleHandleA(NULL),0);
    else if(keyBoardHook != NULL){
        UnhookWindowsHookEx(keyBoardHook);
        keyBoardHook = NULL;
    }
}

int main(int argc, char *argv[])
{
    //FreeConsole();

    //// create a separate new console window
    //AllocConsole();

    //// attach the new console to this application's process
    //AttachConsole(GetCurrentProcessId());

    //// reopen the std I/O streams to redirect I/O to the new console
    //freopen("CON", "w", stdout);
    //freopen("CON", "w", stderr);
    //freopen("CON", "r", stdin);
    QApplication a(argc, argv);
    MainWindow w;
    w.magHook = manageHook;
    mainwin = &w;
    manageHook();
    //LabelTest w;
    w.show();
    return a.exec();
}
