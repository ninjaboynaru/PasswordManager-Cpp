# PasswordManager-Cpp
Win32 based password manager written in c++.

**DO NOT use this program to actualy store passwords and sensitive information. It currently uses xor encryption and thus is not fit for actual data storage. Change the encryption method to something tested and secure if you plan to actualy use this form something besides learning and testing**

# Build Instructions
  - Only 1 external library is used and it's included in the source files
  - Nothing complicated
  - Link with the ws2_32 library (Should come default on evey windows computer).
  - Change your compliers search directorys so it looks in every subfolder in the src folder


# Fetures
  - Only 1 external library used
  - Passwords are stored in displayed in a Label+Password format. Where label is a username or description of the password.
  - Add and delete passwords
  - Change into and outof edit mode to avoid editing a password field by mistake
  - Simple xor encryption. Good for prototypes and learning but not fit for actualy storing sensitive data and passwords
  
