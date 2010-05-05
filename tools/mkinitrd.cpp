#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define INITRDFS_FN_LEN 120
#define INITRDFS_MAGIC 0x73264320
#define INITRDFS_VERSION 0x01

struct InitrdHeader {
	uint32_t magic;
	uint8_t version;
	uint32_t count;
};

struct InitrdFileHeader {
	uint32_t offset;
	uint32_t length;
	char name[INITRDFS_FN_LEN];
};

int fgetsize(FILE* f) {
	fseek(f, 0, SEEK_END);
	return ftell(f);
}

size_t fwritef(FILE* from, FILE* to, size_t count) {
	unsigned char* buff = new unsigned char[count];
	size_t result = fread(buff, 1, count, from);
	fwrite(buff, 1, result, to);
	delete[] buff;
	return result;
}

int main(int argc, char* argv[]) {
	if (isatty(fileno(stdout))) {
		fprintf(stderr, "Error: I won't print binary data to terminal.\n");
		exit(1);
	}
	const int nfiles = (argc-1)/2;
	unsigned offset = sizeof(InitrdHeader) + sizeof(InitrdFileHeader)*nfiles;
	char** srcfiles = new char*[nfiles];
	InitrdFileHeader* headers = new InitrdFileHeader[nfiles];
	for(int i=0; i<nfiles; i++) {
		srcfiles[i] = argv[i*2+1];
		const char* dstfile = argv[i*2+2];
		if (strlen(dstfile) >= INITRDFS_FN_LEN) {
			fprintf(stderr, "Error: filename '%s' is too long.\n", dstfile);
			return 1;
		}
		if (FILE *f = fopen(srcfiles[i], "rb")) {
			const int fsize = fgetsize(f);
			fclose(f);
			fprintf(stderr, "Writing file %s->%s at 0x%x [%d bytes]\n", srcfiles[i], dstfile, offset, fsize);
			strncpy(headers[i].name, dstfile, INITRDFS_FN_LEN);
			headers[i].offset = offset;
			headers[i].length = fsize;
			offset += fsize;
		} else {
			fprintf(stderr, "Error: cannot open file: %s\n", srcfiles[i]);
			return 1;
		}
	}
	if (FILE *of = freopen(NULL, "wb", stdout)) {
		InitrdHeader header;
		header.magic = INITRDFS_MAGIC;
		header.version = INITRDFS_VERSION;
		header.count = nfiles;
		fwrite(&header, sizeof(InitrdHeader), 1, of);
		fwrite(headers, sizeof(InitrdFileHeader), nfiles, of);
		for(int i=0; i<nfiles; i++) {
			if (FILE *f = fopen(srcfiles[i], "rb")) {
				fwritef(f, of, headers[i].length);
				fclose(f);
			} else {
				fprintf(stderr, "Error: cannot open file: %s\n", srcfiles[i]);
				return 1;
			}
		}
		fclose(of);
	} else {
		fprintf(stderr, "Error: cannot open output file\n");
		return 1;
	}
	delete[] srcfiles;
	delete[] headers;
	return 0;
}

