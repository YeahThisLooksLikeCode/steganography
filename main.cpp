#include <iostream>
#include <fstream>
#include <bitset>
#include <string>


void SpliceBits(char* &a, std::bitset<8> bits, int start) {
	int j = start;
	for (int i = 0; i < 8; i += 2) {
		char c = a[j];
		std::bitset<8> bits2(c);
		bits2[6] = bits[i];
		bits2[7] = bits[i + 1];
		unsigned long n = bits2.to_ulong();
		a[j] = static_cast<char>(n);
		j++;
	}
}
bool CheckChar(char input) {
	std::string lut = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.!? ";
	for (int i = 0; i < lut.length(); i++) {
		if (char(lut[i]) == input) {
			return true;
			break;
		}
	}
	return false;
}
std::string DecryptBits(char* &buffer, std::ifstream::pos_type size){
	std::string output;
	for (int i = 16; i < size; i += 4) {
		int j = 0, k = 0;
		std::bitset<8> collectBit;
		while (j < 8) {
			std::bitset<8> imgBit(buffer[i + k]);
			collectBit[j] = imgBit[6];
			collectBit[j + 1] = imgBit[7];
			j += 2;
			k++;
		}
		unsigned long l = collectBit.to_ulong();
		char c = static_cast<char>(l);
		if (CheckChar(c)) {
			output += c;
		}
	}
	return output;
}
void Encryption(std::string str1, std::string str2, std::string str3) {
	std::ifstream readFile;
	std::ifstream readTxt;
	std::ofstream writeFile;
	readFile.open(str1, std::ios_base::binary);
	readTxt.open(str2, std::ios_base::binary);
	writeFile.open( str3, std::ios_base::binary | std::ios_base::trunc);
	readFile.seekg(0, std::ios::end);
	std::ifstream::pos_type size = readFile.tellg();
	readFile.seekg(0);
	readTxt.seekg(0, std::ios::end);
	std::ifstream::pos_type size2 = readTxt.tellg();
	readTxt.seekg(0);
	char* buffer = new char[size];
	char* buffer2 = new char[size2];
	readFile.read(buffer, size);
	readTxt.read(buffer2, size2);
	int j = 0;
	for (int i = 0; i < size2; i++) {
		char c = buffer2[i];
		std::bitset<8> bitTxt(c);
		SpliceBits(buffer, bitTxt, (i + 64) * 4);
	}
	writeFile.write(buffer, size);
	delete[] buffer;
	delete[] buffer2;
	readFile.close();
	readTxt.close();
	writeFile.close();
}
void Decryption(std::string str) {
	std::ifstream checkFile;
	std::string retrieveMsg;
	checkFile.open(str, std::ios_base::binary);
	checkFile.seekg(0, std::ios::end);
	std::ifstream::pos_type size3 = checkFile.tellg();
	checkFile.seekg(0);
	char* buffer3 = new char[size3];
	checkFile.read(buffer3, size3);
	retrieveMsg = DecryptBits(buffer3, size3);
	delete[] buffer3;
	checkFile.close();

	std::cout << retrieveMsg << "\n";
}
int main(int argc, char* argv[]) {
	
	Encryption(argv[1], argv[2], "new.bmp");
	Decryption("new.bmp");

	std::string y;
	getline(std::cin, y);
	return 0;
}