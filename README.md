# PasswordManager-Cpp
Win32 base password manager written in C++.  

Download the **PassManager.exe** to see it in action. A data file is automatically created in the location of the .exe when the program is run.


![Example Image 1](https://raw.githubusercontent.com/ninjaboynaru/PasswordManager-Cpp/master/images/_PassManager_Img_1.png "Password Manager Screen")
![Example Image 2](https://raw.githubusercontent.com/ninjaboynaru/PasswordManager-Cpp/master/images/_PassManager_Img_2.png "Password Manager Console Screen")

**DO NOT use this program to actually store passwords and sensitive information. It currently has no encryption method. Add a secure and tested encryption method if you plan to actually use this for something besides learning and testing**

# Build Instructions
  - Only 1 external library is used and it's included in the source files
  - Nothing complicated
  - Link with the ws2_32 library (Should come default on every windows computer).
  - Change your compliers search directories so it looks in every subfolder in the src folder


# Features
  - Passwords are stored in displayed in a Label+Password format, where label is a username or description of the password.
  - Add and delete passwords
  - Change into and out of edit mode to avoid editing a password field by mistake
  - Included is a NOTES file that contains information about some of the specifics of the program
   
