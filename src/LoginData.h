#ifndef LOGINDATA_H_INCLUDED
#define LOGINDATA_H_INCLUDED


#include<LoginField.h>
#include<sha256.h>
#include<noobcrypt.h>
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
    Methods to manipulate the data file that stores the passwords.
    Includes Saving, Loading, Encrypting, and Decrypting the data aswell as checking and setting the master password.


    When loading getting data from the file, the ENTIRE file is decrypted and saved,
        then the data is extracted,
        then the ENTIRE file is encrypted and saved.

    When saving data to the file the master password is extracted from the file,
        the file is deleted and a new one is created,
        the master password is placed in the file,
        then the data is placed in the file,
        then the file is encrypted.

        **NOTE The master password is stored as a 64 char hash at the begining of the file**
        **NOTE The master password is not encrypted**
        **NOTE The encryption is simple xor. It's simple and weak. The encryption class is called Noobcrypt**




*/

vector<unsigned char> key = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };

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


///Will fail if data file is open in another stream. Close other streams to data file first
void EncryptDataFile()
{
    if(DataFileExists() == false)
    {
        return;
    }

    fstream file(dataFileLocation, ios::in|ios::binary);
    int fileSize = file.seekg(0, ios::end).tellg();
    file.seekg(0, ios::beg);

    if(fileSize <= passwordHashSize)
    {
        return;
    }

    string plainText;
    string encryptedText;
    string password;
    password.resize(passwordHashSize);

    file.read(&password[0], passwordHashSize);
    for(int i = 0; i < fileSize - passwordHashSize; i++)
    {
        plainText.push_back(file.get() );
    }
    file.close();


    Noobcrypt::Encrypt(key, plainText, encryptedText);
    file.open(dataFileLocation, ios::out|ios::trunc);
    file.write(password.c_str(), passwordHashSize);
    file.write(encryptedText.c_str(), encryptedText.size());
}

///Will fail if data file is open in another stream. Close other streams to data file first
void DecryptDataFile()
{
    if(DataFileExists() == false)
    {
        return;
    }

    fstream file(dataFileLocation, ios::in|ios::binary);
    int fileSize = file.seekg(0, ios::end).tellg();
    file.seekg(0, ios::beg);

    if(fileSize <= passwordHashSize)
    {
        return;
    }

    string encryptedText;
    string plainText;
    string password;
    password.resize(passwordHashSize);

    file.read(&password[0], passwordHashSize);
    for(int i = 0; i < fileSize - passwordHashSize; i++)
    {
        encryptedText.push_back(file.get() );
    }
    file.close();


    Noobcrypt::Decrypt(key, plainText, encryptedText);

    file.open(dataFileLocation, ios::out|ios::trunc);
    file.write(password.c_str(), passwordHashSize);
    file.write(plainText.c_str(), plainText.size());
    file.close();
}


void GetLoginData(vector<LoginData>* data)
{
    if(DataFileExists() == false)
    {
        cout << "No Data File Exists" << endl;
        return;
    }

    DecryptDataFile();
    ifstream file;
    file.open(dataFileLocation, ios::binary);

    int fileSize = file.seekg(0,ios::end).tellg();
    file.seekg(0, ios::beg);

    if(fileSize <= passwordHashSize)
    {
        cout << "File size to small" << endl;
        return;
    }

    LoginData currentData;
    int fieldSize;
    while(true)
    {
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
        if(fieldSize + file.tellg() >= fileSize || fieldSize <= 0)
        {
            cout << "Field size to great for file at position: " << file.tellg() << endl;
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
            cout << "Field size to great for file at position: " << file.tellg() << endl;
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
    EncryptDataFile();

    /* ifstream.get(char*, size) when called at the end of the file sets end of file. Thus, we check for eof() in the loop*/
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

        cout << "Saving data: " << field << " with size: " << fieldSize << "\n";

        fieldSize = (*data)[i].GetPasswordSize();
        (*data)[i].GetPasswordText(&field);
        file.put(fieldSize);
        file.write(field.c_str(), fieldSize);

        cout << "Saving data: " << field << " with size: " << fieldSize << "\n";
    }
    file.close();
    EncryptDataFile();
}







#endif // LOGINDATA_H_INCLUDED
