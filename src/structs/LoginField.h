#ifndef LOGINFIELD_H_INCLUDED
#define LOGINFIELD_H_INCLUDED
#include<windows.h>
#include<string>
using std::string;

struct LoginField
{
    HWND label;
    HWND password;
    HWND labelStatic;
    HWND passwordStatic;

    HWND editButton;
    HWND deleteButton;


    int GetLabelSize()
    {
        return SendMessage(label, WM_GETTEXTLENGTH, 0,0);
    }
    int GetPasswordSize()
    {
        return SendMessage(password, WM_GETTEXTLENGTH, 0,0);
    }

    void GetLabelText(string* text)
    {
        int labelSize = GetLabelSize();
        (*text).clear();
        (*text).resize(labelSize + 1);

        GetWindowText(label, &(*text)[0], labelSize + 1);
        (*text).pop_back();

        //+1 because GetWindowText() automaticly places a NULL char as the last char. pop_back() to remove that last NULL char
    }

    void GetPasswordText(string* text)
    {
        int passwordSize = GetPasswordSize();
        (*text).clear();
        (*text).resize(passwordSize + 1);

        GetWindowText(password, &(*text)[0], passwordSize + 1);
        (*text).pop_back();

        //+1 because GetWindowText() automaticly places a NULL char as the last char. pop_back() to remove that last NULL char
    }


    void Delete()
    {
        DestroyWindow(label);
        DestroyWindow(password);
        DestroyWindow(labelStatic);
        DestroyWindow(passwordStatic);
        DestroyWindow(editButton);
        DestroyWindow(deleteButton);
    }

    void DefaultShow()
    {
        ShowWindow(label, SW_HIDE);
        ShowWindow(password, SW_HIDE);
        ShowWindow(labelStatic, SW_SHOWDEFAULT);
        ShowWindow(passwordStatic, SW_SHOWDEFAULT);
        ShowWindow(editButton, SW_SHOWDEFAULT);
        ShowWindow(deleteButton, SW_SHOWDEFAULT);
    }

    void BeginEdit()
    {
        ShowWindow(label, SW_SHOW);
        ShowWindow(labelStatic, SW_HIDE);
        ShowWindow(password, SW_SHOW);
        ShowWindow(passwordStatic, SW_HIDE);

        string buffer;
        int bufferSize;

        bufferSize = GetWindowTextLength(labelStatic) + 1;
        buffer.resize(bufferSize);
        GetWindowText(labelStatic, &buffer[0], bufferSize);
        SetWindowText(label, buffer.c_str());

        buffer.clear();

        bufferSize = GetWindowTextLength(passwordStatic) + 1;
        buffer.resize(bufferSize);
        GetWindowText(passwordStatic, &buffer[0], bufferSize);
        SetWindowText(password, buffer.c_str());
    }

    void EndEdit ()
    {
        ShowWindow(label, SW_HIDE);
        ShowWindow(labelStatic, SW_SHOW);
        ShowWindow(password, SW_HIDE);
        ShowWindow(passwordStatic, SW_SHOW);

        string buffer;
        int bufferSize;

        bufferSize = GetWindowTextLength(label) + 1;
        buffer.resize(bufferSize);
        GetWindowText(label, &buffer[0], bufferSize);
        SetWindowText(labelStatic, buffer.c_str());

        buffer.clear();

        bufferSize = GetWindowTextLength(password) + 1;
        buffer.resize(bufferSize);
        GetWindowText(password, &buffer[0], bufferSize);
        SetWindowText(passwordStatic, buffer.c_str());
    }
};
#endif // LOGINFIELD_H_INCLUDED
