# parallel-aes

Optimized for OSX

Dependencies:
* find
* openssl

This program encrypt or decrypt every file inside an specific directory.
Each directory can have other directories and files and so on.

The process is simple:
* File paths are listed
* The list of files is divided and assigned to each available thread
* Each thread encrypt and decrypt sequentially the files assigned
* When every thread has finished its work, then the program ends

To install the program correctly, is necesary to compile it using make.
The program is generated at ./bin, named "paes"

To use the program is necessary understand what parameters you can cofigure:

./bin/paes -p PATH -k PASSWORD -t THREADS

Where:

* PATH: The directory path (default: ./tests)
* PASSWORD: Password associeted to the AES algorithm to encrypt or decrypt files (default: PASSWORD)
* THREADS: The amount of threads used to encrypt or decrypt files (default: 1)

Also, there are other parameters, used to determine the function of the program, there are:

* --parallel or --p: To run the program using the parallel version, this parameter is not necessary when the amount of thread is set (-t THREADS)
* --encrypt or --e: To set de encryption process (by default)
* --decrypt or --d: To set the decryption process
* --verbose or --v: To run the program in a verbose mode

If you want to install some files to test it, you can do it running:

source ./scripts/test/install.sh

After that, if you want to run that test, you can do it executing the next line:

source ./scripts/test/run.sh

Run these commands from root directory if you use the default parameters.

Enjoy it! :)

Note: I wrote this code for academic purposes. Please read the instructions carefully before use it.
I am not responsible for the misuse of this program.
