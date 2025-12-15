#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <cmath>
#include <vector>
#include <cstdint>
#include <bitset>


std::map<char, int> hexMap{
	{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
	{'8', 8}, {'9', 9}, {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15}
};

std::string base64chars {
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"01234567890+/"
};

// assumes input of two chars in a string. example: "1a"
std::string hexToBase64_old(const std::string& input) {

	std::size_t hexPairSize = input.size() * 0.5;
	std::string test{};
	int totalDecimal{0};
	for (std::size_t i{0}; i < input.size(); i+=2) {
		char lhs{input[i]};   // left digit 16^1
		char rhs{input[i+1]}; // right digit 16^0
		totalDecimal += hexMap[rhs] + (hexMap[lhs] * 16); // convert to base 10
		std::size_t index = hexMap[rhs] + (hexMap[lhs] * 16);
		test += base64chars[index % 64];
	}

	std::cout << totalDecimal << '\n';

	std::string result{};
	while (totalDecimal > 0) {
		size_t index = static_cast<size_t>(totalDecimal % 64); // convert total num to base 64 index
		result += base64chars[index]; // place the indexed char
		totalDecimal /= 64;
	}

	return test;
}

std::vector<std::uint8_t>
	hexStringToByteArr(const std::string& input) {
		std::vector<std::uint8_t> byteArr;
		
		for (std::size_t i{0}; i < input.size(); i+=2) {
			std::string byteStr = input.substr(i, 2); // grab next two chars that make up hex

			// convert bytestring to uint8_t val
			std::uint8_t byteVal = static_cast<std::uint8_t>(std::stoi(byteStr, nullptr, 16/*base*/));

			// Add byte to the byte array
			//byteArr.push_back(byteVal);
			byteArr.push_back(byteVal);
		}

		return byteArr;
	}

std::vector<std::uint8_t>
	convertByteToBase64(const std::vector<std::uint8_t>& arr) {
		std::vector<std::uint8_t> out;
		for (std::size_t i{0}; i<arr.size(); i+=3) {
			unsigned char a = arr[i];
			unsigned char b = arr[i+1];
			unsigned char c = arr[i+2];

			std::uint8_t val1 = (a & 0b11111100) >> 2; // grab 11111100 and shift right 2
			std::uint8_t val2 = ((a & 0b00000011) << 4) | ((b & 0b11110000) >> 4);
			std::uint8_t val3 = ((b & 0b00001111) << 2) | ((c & 0b11000000) >> 6);
			std::uint8_t val4 = c & 0b00111111;

			out.push_back(val1);
			out.push_back(val2);
			out.push_back(val3);
			out.push_back(val4);
		}
		return out;
	}

std::string
	byteArrToBase64(const std::vector<std::uint8_t>& arr) {
		std::string out{};
		for (std::uint8_t item : arr) {
			out += base64chars[item];
		}

		return out;		
	}

int main(int argc, char** argv) {
	
	std::string start{"49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"};
	std::string outputToMatch{"SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t"};
	std::vector<std::uint8_t> arr;
	arr = hexStringToByteArr(start);
	std::vector<std::uint8_t> newarr = convertByteToBase64(arr);
	
	std::string out = byteArrToBase64(newarr);
	std::cout << outputToMatch << '\n';
	std::cout << out << std::endl;

	return 0;
}
