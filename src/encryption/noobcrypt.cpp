#include "noobcrypt.h"

/*
    Simple xor encryption.
*/

void Noobcrypt::Encrypt(const vector<unsigned char>& key, const string& plainText, string& encryptedText)
{
    int keyLength = key.size();
    for(int i = 0; i < plainText.size(); i++)
    {
        int keyIndex = i;
        if(keyIndex >= keyLength)
        {
            keyIndex = keyIndex%keyLength;
        }
        encryptedText.push_back(plainText[i] ^ key[keyIndex]);
    }
}

void Noobcrypt::Decrypt(const vector<unsigned char>& key, string& plainText, const string& encryptedText)
{
    Encrypt(key, encryptedText, plainText);
}
