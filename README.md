# PasswordManager-Cpp
Win32 based password manager written in C++.
![Example Image 1](https://raw.githubusercontent.com/ninjaboynaru/PasswordManager-Cpp/master/images/_PassManager_Img_1.png "Password Manager Screen")
![Example Image 2](https://raw.githubusercontent.com/ninjaboynaru/PasswordManager-Cpp/master/images/_PassManager_Img_2.png "Password Manager Console Screen")

**DO NOT use this program to actually store passwords and sensitive information. It currently uses xor encryption and thus is not fit for actual data storage. Change the encryption method to something tested and secure if you plan to actually use this form something besides learning and testing**

# Build Instructions
  - Only 1 external library is used and it's included in the source files
  - Nothing complicated
  - Link with the ws2_32 library (Should come default on every windows computer).
  - Change your compliers search directories so it looks in every subfolder in the src folder


# Features
  - Passwords are stored in displayed in a Label+Password format, where label is a username or description of the password.
  - Add and delete passwords
  - Change into and out of edit mode to avoid editing a password field by mistake
  - Simple xor encryption. Good for prototypes and learning but not fit for actually storing sensitive data and passwords
  - Included is a NOTES file that contains information about some of the specifics of the program
   
