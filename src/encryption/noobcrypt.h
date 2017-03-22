#ifndef NOOBCRYPT_H
#define NOOBCRYPT_H

#include<string>
#include<vector>
using std::vector;
using std::string;

class Noobcrypt
{
    public:
        static void Encrypt(const std::vector<unsigned char>& key, const string& plainText, string& encryptedText);
        static void Decrypt(const std::vector<unsigned char>& key, string& plainText, const string& encryptedText);
};

#endif // NOOBCRYPT_H
