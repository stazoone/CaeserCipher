# Caesar Cipher Application 

A powerful command-line application for encrypting, decrypting, and analyzing text using the Caesar cipher method made for my OOP course. This application includes advanced features like frequency analysis and automatic cipher breaking.

## Features

- **Text Encryption**: Encrypt text using a specified shift value
- **Text Decryption**: Decrypt text using a known shift value
- **File Operations**: Read text from files
- **Frequency Analysis**: Compute and display letter frequency distribution
- **Cipher Breaking**: Automatically break encrypted text using three different distance metrics:
  - Chi-squared distance
  - Euclidean distance
  - Cosine distance
- **Interactive Menu**: User-friendly command-line interface

## Prerequisites

- C compiler (GCC recommended)
- Standard C libraries
- Windows operating system (for character encoding support)

## Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/CeaserCipher.git
cd CeaserCipher
```

2. Compile the program:
```bash
gcc main.c -o caesar_cipher
```

## Usage

Run the compiled program:
```bash
./caesar_cipher
```

### Menu Options

1. **Print Current Text**: Display the currently loaded text
2. **Input Text**: Enter new text manually
3. **Read from File**: Load text from a file
4. **Encrypt Text**: Encrypt the current text with a specified shift
5. **Decrypt Text**: Decrypt the current text with a known shift
6. **Frequency Analysis**: Show letter frequency distribution
7. **Break Cipher**: Attempt to break encrypted text using frequency analysis
8. **Exit**: Close the program

### Breaking the Cipher

When using the cipher breaking feature, you can choose between three distance metrics:
- `chi_squared`: Best for general-purpose frequency analysis
- `euclidian`: Good for comparing overall distribution patterns
- `cosine`: Useful for comparing relative frequencies

## File Requirements

The program requires a `distribution.txt` file containing the expected frequency distribution of letters in the target language. This file should contain 26 space-separated numbers representing the frequency of each letter (A-Z).

## Example Usage

1. Input text:
```
Enter your text: HELLO WORLD
```

2. Encrypt with shift 3:
```
Enter the shift number: 3
Result: KHOOR ZRUOG
```

3. Break the cipher:
```
Enter the distance metric: chi_squared
Most likely shift: 3
```