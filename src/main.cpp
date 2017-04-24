#include <LoginManager.h>


/*
    Prototpye password manager buit with Win32 UI.
    Currently there is no encryption for the data.

    All of the UI work and layout is done by the LoginManager.h file.
    Data management is done by the LoginData.h file.

    The program stores and displays "LoginFields" wich are basicly usernames and passwords.
    The LoginField.h file contains the struct used to hold such data and some helper methods for the UI.
*/

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInstance, LPSTR cmdArgs, int displayOptions)
{
    ValidateInitialEntry(); //Have user enter password to access program
    InitializeMainWindow();
    InitializeOtherButtons();
    InitializeLoginFields();


    MSG message;
    while(GetMessage(&message,NULL,0,0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}






