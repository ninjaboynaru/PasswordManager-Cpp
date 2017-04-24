#ifndef LOGINDATA_H_INCLUDED
#define LOGINDATA_H_INCLUDED


#include<LoginField.h>
#include<sha256.h>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::ios;
using std::vector;
using std::string;

/*
    File Contains methods to manipulate the data file that stores the passwords and labels.
    Includes Saving and Loading the data as well as checking and setting the master password.


    When saving data to the file the master password is extracted from the file,
        the file is deleted and a new one is created,
        the master password is placed in the file,
        then the data is placed in the file.


        **NOTE The master password is stored as a 64 char hash at the begining of the file**
        **NOTE The master password is not encrypted**
*/

const char* dataFileLocation = "./Data.lfd";
const int minPasswordSize = 4;
const int maxPasswordSize = 32;
const int passwordHashSize = 64;
SHA256 sha256;


struct LoginData
{
    string label;
    string password;
};


/*===========================================================*/


bool DataFileExists()
{
    ifstream file;
    file.open(dataFileLocation);

    if(!file)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void GetInitialPassword(string* password)
{
    ifstream file;
    file.open(dataFileLocation, ios::binary);

    if(!file) {return;}
    int fileSize = file.seekg(0, ios::end).tellg();
    file.seekg(0, ios::beg);
    if(fileSize < passwordHashSize){return;}

    (*password).clear();
    (*password).resize(passwordHashSize);
    file.read(&(*password)[0], passwordHashSize);

}

void SetInitialPassword(const char* password)
{
    //Deletes existing data file if there is one. Creates new data file
    ofstream file;
    file.open(dataFileLocation, ios::binary|ios::trunc);
    file.write(&sha256(password)[0], passwordHashSize);
}


/*===========================================================*/


bool PasswordMatch(string passwordToCompare)
{
    if(DataFileExists() == false)
    {
        return false;
    }


    string initiaPassword;
    GetInitialPassword(&initiaPassword);

    if(initiaPassword == sha256(passwordToCompare) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool PasswordMeetsRequirements(string newPassword)
{
    if(newPassword.length() < minPasswordSize || newPassword.length() > maxPasswordSize)
    {
        return false;
    }
    return true;
}
/*===========================================================*/




void GetLoginData(vector<LoginData>* data)
{
    if(DataFileExists() == false)
    {
        cout << "No Data File Exists" << endl;
        return;
    }

    ifstream file;
    file.open(dataFileLocation, ios::binary);

    int fileSize = file.seekg(0,ios::end).tellg();
    file.seekg(0, ios::beg);

    if(fileSize <= passwordHashSize)
    {
        return;
    }

    LoginData currentData;
    int fieldSize;
    while(true)
    {
        /* ifstream.get(char*, size) when called at the end of the file sets end of file. Thus, we check for eof() in the loop*/
        if( file.tellg() == 0 )
        {
            file.seekg(passwordHashSize);
        }
        if( file.eof() == true || file == false  || file.tellg() == fileSize)
        {
            cout << "End of file reached" << endl;
            break;
        }

        fieldSize = file.get();
        if(fieldSize + file.tellg() > fileSize || fieldSize <= 0)
        {
            cout << "Label field size to great for file at position: " << file.tellg() << endl;
            break;
        }
        else
        {
            currentData.label.clear();
            currentData.label.resize(fieldSize);
            file.get(&currentData.label[0], fieldSize + 1);
        }

        fieldSize = file.get();
        if(fieldSize + file.tellg() > fileSize || fieldSize <= 0)
        {
            cout << "Password field size to great for file at position: " << file.tellg()  << " Size: " << fieldSize << " Char: " << (char)fieldSize << endl;
            break;
        }
        else
        {
            currentData.password.clear();
            currentData.password.resize(fieldSize);
            file.get(&currentData.password[0], fieldSize + 1);
        }

        (*data).push_back(currentData);
    }
    file.close();
}

void SaveLoginData(vector<LoginField>* data)
{
    if((*data).size() == 0)
    {
        return;
    }

    if(DataFileExists() == false)
    {
        return;
    }

    string passwordHash;
    GetInitialPassword(&passwordHash);

    ofstream file;
    file.open(dataFileLocation, ios::binary|ios::trunc);

    file.write(passwordHash.c_str(), passwordHashSize);

    string field;
    int fieldSize;
    for(int i = 0; i < (*data).size(); i++)
    {
        fieldSize = (*data)[i].GetLabelSize();
        (*data)[i].GetLabelText(&field);
        file.put(fieldSize);
        file.write(field.c_str(), fieldSize);

        cout << "Saving label: " << field.c_str() << " with size: " << fieldSize << "\n";

        fieldSize = (*data)[i].GetPasswordSize();
        (*data)[i].GetPasswordText(&field);
        file.put(fieldSize);
        file.write(field.c_str(), fieldSize);

        cout << "Saving password: " << field.c_str() << " with size: " << fieldSize << "\n";
    }


    file.close();
}







#endif // LOGINDATA_H_INCLUDED
