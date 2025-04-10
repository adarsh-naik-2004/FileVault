# File Encryption/Decryption System
C++ utility for AES-256 file encryption/decryption using OpenSSL.
---

##  Features

-  **AES-256-CBC encryption** using OpenSSL
-  **Recursive directory encryption/decryption**
-  **Parallel task execution**
-  Handles **PKCS#7 padding** during encryption
-  Truncates extra padding bytes after decryption
-  Configurable via a `.env` file

---

##  Requirements

- C++17-compatible compiler (GCC / Clang)
- OpenSSL 1.1.1 or newer
- Linux or macOS

---

## Installation

Clone the repository and build:

```bash
git clone https://github.com/yourusername/file-encryption
cd file-encryption
make
```
---
##  Configuration
Create a .env file in the root directory with a 32-byte encryption key:
```bash
MYSECRETKEY1234567890ABCDEF12345678
```
- The key must be exactly 32 characters for AES-256.
---
##  Usage
Run the built application:
```bash
./bin/FileVault
```
---
##  Security Notes
- Key Handling: The encryption key is loaded from .env. Never commit it to version control.

- IV Usage: This version uses a static IV (all zeroes) for demonstration. In production, use a random IV per encryption, stored alongside the ciphertext.

- Backups: Always maintain backups of original files before encryption or decryption.
