#include<LoginData.h>
#include<Vector2.h>
#include<windows.h>
using std::cin;
void CreateLoginField(const char*, const char*);
void RepositionLoginFields();

/*
    Vector2 is a stuct to store 2 dimnsional data/axis, x and y, wich are just ints.
*/

HINSTANCE programInstance;
WNDPROC originalButtonProc;
HWND mainWindow;
HWND addButton;
vector<LoginField> loginFields;


const Vector2 mainSize = Vector2{700,400};
const Vector2 fieldSize = Vector2{300,20};
const Vector2 buttonSize = {20,fieldSize.y};

const Vector2 addButtonSize = {80,20};
const Vector2 addButtonPosition = {(mainSize.x/2)-(addButtonSize.x/2), 0};

const Vector2 loginFieldOrigin = Vector2{10,5};
const Vector2 loginFieldPadding = Vector2{5,5};




LRESULT CALLBACK MainProc(HWND handle, UINT message, WPARAM paramW, LPARAM paramL)
{
    switch(message)
    {
        case WM_CREATE:
            break;
        case WM_CLOSE:
            DestroyWindow(handle);
            break;
        case WM_DESTROY:
            SaveLoginData(&loginFields);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(handle, message, paramW, paramL);
    }
}

LRESULT CALLBACK AddFieldProc(HWND handle, UINT message, WPARAM paramW, LPARAM paramL)
{
    if(message == WM_LBUTTONDOWN)
    {
        CreateLoginField("Label", "Password");
    }
    return CallWindowProc(originalButtonProc, handle, message, paramW, paramL);
}

LRESULT CALLBACK EditFieldProc(HWND handle, UINT message, WPARAM paramW, LPARAM paramL)
{
    if(message == WM_LBUTTONDOWN)
    {
        for(int i = 0; i < loginFields.size(); i++)
        {
            if(loginFields[i].editButton == handle)
            {
                if(IsWindowVisible(loginFields[i].label) )
                {
                    loginFields[i].EndEdit();
                }
                else
                {
                    loginFields[i].BeginEdit();
                }
            }
        }
    }
    return CallWindowProc(originalButtonProc, handle, message, paramW, paramL);
}

LRESULT CALLBACK DeleteFieldProc(HWND handle, UINT message, WPARAM paramW, LPARAM paramL)
{
    if(message == WM_LBUTTONDOWN)
    {
        for(int i = 0; i < loginFields.size(); i++)
        {
            if(loginFields[i].deleteButton == handle)
            {
                loginFields[i].Delete();
                loginFields.erase(loginFields.begin()+i);
                RepositionLoginFields();
            }
        }
    }
    return CallWindowProc(originalButtonProc, handle, message, paramW, paramL);
}



void InitializeMainWindow()
{
    const char className[] = "MainWinClass";
    WNDCLASS mainClass;

    mainClass.style = 0;
    mainClass.lpfnWndProc = MainProc;
    mainClass.cbClsExtra = 0;
    mainClass.cbWndExtra = 0;
    mainClass.hInstance = programInstance;
    mainClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    mainClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    mainClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    mainClass.lpszClassName = className;


    RegisterClass(&mainClass);
    mainWindow = CreateWindow(className, "Login Manager", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, mainSize.x, mainSize.y, NULL, NULL, programInstance, NULL);
    ShowWindow(mainWindow, SW_SHOWDEFAULT);
    UpdateWindow(mainWindow);
}

void InitializeOtherButtons()
{
    addButton = CreateWindow("BUTTON", "+", WS_CHILD|WS_BORDER, addButtonPosition.x, addButtonPosition.y, addButtonSize.x, addButtonSize.y, mainWindow, NULL, programInstance, NULL);
    originalButtonProc = (WNDPROC)SetWindowLong(addButton, GWL_WNDPROC, (LONG)AddFieldProc);
    ShowWindow(addButton, SW_SHOWDEFAULT);
}

void InitializeLoginFields()
{
    vector<LoginData> data;
    GetLoginData(&data);
    if(data.size() == 0)
    {
        CreateLoginField("Label", "Password");
    }
    else
    {
        for(int i = 0; i < data.size(); i++)
        {
            CreateLoginField(data[i].label.c_str(), data[i].password.c_str() );
        }
    }
}

void CreateLoginField (const char* labelText, const char* passwordText)
{
    int yPos = (loginFields.size()*(fieldSize.y+loginFieldPadding.y)) + fieldSize.y + loginFieldOrigin.y;
    int labelXPos = loginFieldOrigin.x;
    int passwordXPos = labelXPos + fieldSize.x + loginFieldPadding.x;
    int editButtonXPos = passwordXPos + fieldSize.x + loginFieldPadding.x;
    int deleteButtonXPos = editButtonXPos + buttonSize.x + loginFieldPadding.x;

    LoginField logField;
    logField.label = CreateWindow("EDIT", "Label", WS_CHILD|WS_BORDER, labelXPos, yPos, fieldSize.x, fieldSize.y, mainWindow, NULL, programInstance, NULL);
    logField.password = CreateWindow("EDIT", "Password", WS_CHILD|WS_BORDER, passwordXPos, yPos, fieldSize.x, fieldSize.y, mainWindow, NULL, programInstance, NULL);
    logField.labelStatic = CreateWindow("STATIC", labelText, WS_CHILD|WS_BORDER, labelXPos, yPos, fieldSize.x, fieldSize.y, mainWindow, NULL, programInstance, NULL);
    logField.passwordStatic = CreateWindow("STATIC", passwordText, WS_CHILD|WS_BORDER, passwordXPos, yPos, fieldSize.x, fieldSize.y, mainWindow, NULL, programInstance, NULL);
    logField.editButton = CreateWindow("BUTTON", "E", WS_CHILD|WS_BORDER, editButtonXPos, yPos, buttonSize.x, buttonSize.y, mainWindow, (HMENU)"EDIT", programInstance, NULL);
    logField.deleteButton = CreateWindow("BUTTON", "-", WS_CHILD|WS_BORDER, deleteButtonXPos, yPos, buttonSize.x, buttonSize.y, mainWindow, (HMENU)"DELETE", programInstance, NULL);

    SetWindowLong(logField.editButton, GWL_WNDPROC, (LONG)EditFieldProc);
    SetWindowLong(logField.deleteButton, GWL_WNDPROC, (LONG)DeleteFieldProc);

    logField.DefaultShow();
    logField.BeginEdit();
    logField.EndEdit();

    loginFields.push_back(logField);
}



void RepositionLoginFields()
{
    for(int i=0; i<loginFields.size(); i++)
    {
        int yPos = (i*(fieldSize.y+loginFieldPadding.y)) + fieldSize.y + loginFieldOrigin.y;
        int labelXPos = loginFieldOrigin.x;
        int passwordXPos = labelXPos + fieldSize.x + loginFieldPadding.x;
        int editButtonXPos = passwordXPos + fieldSize.x + loginFieldPadding.x;
        int deleteButtonXPos = editButtonXPos + buttonSize.x + loginFieldPadding.x;

        MoveWindow(loginFields[i].label, labelXPos, yPos, fieldSize.x, fieldSize.y, true);
        MoveWindow(loginFields[i].password, passwordXPos, yPos, fieldSize.x, fieldSize.y, true);
        MoveWindow(loginFields[i].labelStatic, labelXPos, yPos, fieldSize.x, fieldSize.y, true);
        MoveWindow(loginFields[i].passwordStatic, passwordXPos, yPos, fieldSize.x, fieldSize.y, true);
        MoveWindow(loginFields[i].editButton, editButtonXPos, yPos, buttonSize.x, buttonSize.y, true);
        MoveWindow(loginFields[i].deleteButton, deleteButtonXPos, yPos, buttonSize.x, buttonSize.y, true);
    }
}

bool ValidateInitialEntry()
{
    string input;
    if(DataFileExists() )
    {
        cout << "Enter Master Password" << endl;
        while(true)
        {
            cout << ">> ";
            getline(cin, input);
            if(PasswordMatch(input) )
            {
                cout << endl << "Access Granted" << endl;
                return true;
            }
            else
            {
                cout << endl << "Wrong password entered" << endl;
            }
        }
    }
    else
    {
        cout << "Hello new user. Please enter a new master password" << endl;
        while(true)
        {
            cout << ">> ";
            getline(cin, input);
            if(PasswordMeetsRequirements(input) )
            {
                cout << "Access Granted" << endl;
                SetInitialPassword(input.c_str());
                return true;
            }
            else
            {
                cout << "Master password must be between " << minPasswordSize << " and " << maxPasswordSize << " characters long" << endl;
                cout << "Enter a valid master password" << endl;
            }
        }
    }

}




