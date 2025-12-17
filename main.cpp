#include <iostream>
#include <map>
#include <sstream>
#include <cmath>
#include <vector>
#include <cstdint>

#include <filesystem>
#include <stdio.h>
#include <string>
#include <assert.h>
#include <zlib.h>

// avoid corruption on Windows
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#include <fcntl.h>
#include <io.h>
#define SET_BINARY_MODE(file) _setmode(_file, O_BINARY)
#else
#define SET_BINARY_MODE(file)
#endif


#define CHUNK 16384


// Decompress data from input to output file until EOF on source.
// Returns Z_OK on success
int inf(const std::string& source, FILE *dest)
{
	int ret
	unsigned int have;
	z_stream strm;
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];

	// allocate deflate state
	strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK) return ret;

	// decompress until end of file
	do {
		strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
        	(void)inflateEnd(&strm);
        	return Z_ERRNO;
		}
		if (strm.avail_in == 0) break;
		strm.next_in = in;

		// run inflate() on input until output buffer not full
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;

			ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);
            switch (ret) {
            case Z_NEED_DICT: ret = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
			}
			have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
            	(void)inflateEnd(&strm);
            	return Z_ERRNO;
            }
		} while (strm.avail_out == 0);
	} while (ret != Z_STREAM_END);

    // cleanup
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}




int main(int argc, char** argv) {


	std::string start{
		"H4sIAAAAAAAACp2V223DMBAEqRbYAntKgkDAd1AbFhETiMZqUgH4QtkvfavVuO1to41hZWOqv7Hf7cd5xNsp3h58uusy+47zLneH3Oc+bWtutOtnPuQM2LT27l21po4MOxrvJoH7zLWlaEk0npuTS7rLOxKhzD3yRruUJ+u1fkg4Wn9c4Wz51lppr+eK4vWd+yV2tizjYHV3XP59qFy4kY5C9xy9prT1X7iftd9g1+iSVzNZW9H3cvHDPetR+ybPVjv7cXjjxnONmffDf9UAd5NfyNuxMs5PWprr+wm+Kb322fnfEZV417t07kGJbLnWxrsqv6b+9MaYnKZfqnzWaShSbPv3pWkD3WOTCPp9w7npNsTqgb7Dfqz93bQ7t176e95+lT4vLt4Btk2sV9ag5xqPEXDsTXNDFpGPG1vq7+VvwHYnGOWCtxZj8ZdsY9+U1vj30Td5t9Ypbmy3qZWJHX1G9Wh+kJ59DwN1wT1sYZ45rmMndqCLkhHpZnSPrS5uHhKNvs1BnZcpfkxHko6ZDfvC7s12rnWDjemG8ZbeBusPxn2tX8nPLc0QDgAA"
	};

	int ret;
	SET_BINARY_MODE(stdin);
	SET_BINARY_MODE(stdout);

	std::ofstream out{"decode_test.txt"};
	std::ifstream ifs{"encrypt.txt"};
	if (out.is_open() && ifs.is_open()) {
		ret = inf(ifs, out);
	}
	out.close();

	return 0;
}
