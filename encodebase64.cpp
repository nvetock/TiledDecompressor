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



// IN MAIN
//std::string start{"49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"};
//std::string outputToMatch{"SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t"};
//std::vector<std::uint8_t> arr;
//arr = hexStringToByteArr(start);
//std::vector<std::uint8_t> newarr = convertByteToBase64(arr);
//
//std::string out = byteArrToBase64(newarr);
//std::cout << outputToMatch << '\n';
//std::cout << out << std::endl;