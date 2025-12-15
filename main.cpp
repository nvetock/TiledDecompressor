#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <cmath>
#include <vector>
#include <cstdint>
#include <bitset>
#include <zlib.h>

std::map<char, int> hexMap{
	{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
	{'8', 8}, {'9', 9}, {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15}
};






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

std::string base64chars {
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ" // 0-25
	"abcdefghijklmnopqrstuvwxyz" // 26-51
	"01234567890+/"              // 52-63
};
std::optional<std::uint8_t> base64Lookup(const char& c) {
	if (c == '=') return nullptr;

	return static_cast<std::uint8_t>(std::stoi(c, nullptr, 64));
}


int main(int argc, char** argv) {
	
	//std::string start{"49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"};
	//std::string outputToMatch{"SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t"};
	//std::vector<std::uint8_t> arr;
	//arr = hexStringToByteArr(start);
	//std::vector<std::uint8_t> newarr = convertByteToBase64(arr);
	//
	//std::string out = byteArrToBase64(newarr);
	//std::cout << outputToMatch << '\n';
	//std::cout << out << std::endl;

	std::string start{
		"H4sIAAAAAAAACp2V223DMBAEqRbYAntKgkDAd1AbFhETiMZqUgH4QtkvfavVuO1to41hZWOqv7Hf7cd5xNsp3h58uusy+47zLneH3Oc+bWtutOtnPuQM2LT27l21po4MOxrvJoH7zLWlaEk0npuTS7rLOxKhzD3yRruUJ+u1fkg4Wn9c4Wz51lppr+eK4vWd+yV2tizjYHV3XP59qFy4kY5C9xy9prT1X7iftd9g1+iSVzNZW9H3cvHDPetR+ybPVjv7cXjjxnONmffDf9UAd5NfyNuxMs5PWprr+wm+Kb322fnfEZV417t07kGJbLnWxrsqv6b+9MaYnKZfqnzWaShSbPv3pWkD3WOTCPp9w7npNsTqgb7Dfqz93bQ7t176e95+lT4vLt4Btk2sV9ag5xqPEXDsTXNDFpGPG1vq7+VvwHYnGOWCtxZj8ZdsY9+U1vj30Td5t9Ypbmy3qZWJHX1G9Wh+kJ59DwN1wT1sYZ45rmMndqCLkhHpZnSPrS5uHhKNvs1BnZcpfkxHko6ZDfvC7s12rnWDjemG8ZbeBusPxn2tX8nPLc0QDgAA"
	};

	z_stream_s zss{};
	zss.zalloc = Z_NULL;
	zss.zfree = Z_NULL;
	zss.opaque = Z_NULL;
	int ret = deflateInit(&zss, Z_NULL);
	for (std::size_t i{0}; i<start.size(); ++i) {
		if (start[i] == '=') continue;

		std::uint8_t byteVal = start[i];
		if (!zss.avail_in) continue; // maybe?
		else zss.next_in = &byteVal;

		if (i+1 >= start.size() || start[i+1] == '=') zss.avail_in = 0;
		else zss.avail_in = 1;


	}


	return 0;
}
