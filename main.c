// #include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
// #include <random>
// #include <vector>
// #include <string>
// #include <ctime>

int main () {
	printf("Hola mundo\n");
	system("openssl aes-256-cbc -a -salt -in /Users/tiantg/Projects/parallel-rsa/tests/lorem-ipsum/data.txt -out /Users/tiantg/Projects/parallel-rsa/tests/lorem-ipsum/data.txt.enc -k passpass");
	printf("Hola mundo\n");
	system("openssl aes-256-cbc -d -a -in /Users/tiantg/Projects/parallel-rsa/tests/lorem-ipsum/data.txt.enc -out /Users/tiantg/Projects/parallel-rsa/tests/lorem-ipsum/data.txt.dec -k passpass");
	printf("Hola mundo\n");
	return 0;
}