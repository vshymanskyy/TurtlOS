#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum NumberBase{
	nbOct, nbDec, nbHex
};

enum LineTerminator{
	ltUnix, ltMac, ltWindows
};

enum DataType{
	dtByte, dtWord, dtDword, Qword
};

struct Parameters{
	const char* srcFile;
	int width;
	int nul;
	int line;

	bool append;
	bool tabs;
	bool align;
	bool header;
//	bool hArrSize;
	const char* hArrName;

	NumberBase nb;
	LineTerminator nl;

	Parameters()
		: nb(nbHex)
		, nl(ltUnix)
		, width(16)
		, tabs(true)
		, align(true)
		, hArrName("data")
	{}
} params;

void PrintUsage(FILE* f) {
	fprintf(f,"================================================\n");
	fprintf(f,"                    BIN2INL\n");
	fprintf(f,"                                     Version 1.0\n");
	fprintf(f,"================================================\n");
	fprintf(f,"    by Shymanskyi Volodymyr (aka stealther)\n");
	fprintf(f,"          e-mail:    go.to@rambler.ru\n");
	fprintf(f,"================================================\n");
	fprintf(f," Feel free to thank/report bugs/share ideas/etc\n");
	fprintf(f,"================================================\n");
	fprintf(f,"Written specially for the ShymanOS project. ;)\n");
	fprintf(f,"Usage: bin2inl src dest [params]\n");
	fprintf(f," Parameters:\n");
	fprintf(f,"    -append Append to destination file instead of rewriting\n");
	fprintf(f,"    -null   Write a null-terminator at the end of data\n");
	fprintf(f,"    -line   Write one more empty line at the end of file\n");
	fprintf(f,"    -header   Create C/C++ header (also enables '-line')\n");
	fprintf(f,"   Header:\n");
	//fprintf(f,"    -htype  variable type\n");
	fprintf(f,"    -hvar S variable name\n");
	//fprintf(f,"    -hsize  put items count in []\n");
	fprintf(f,"   Layout:\n");
	fprintf(f,"    -w N    Write N items per line\n");
	fprintf(f,"    -tabs   Use tabs instead of spaces\n");
	fprintf(f,"    -align  Align numbers\n");
	fprintf(f,"   Output number base:\n");
	fprintf(f,"    -oct    Output octal numbers\n");
	fprintf(f,"    -dec    Output decimal numbers\n");
	fprintf(f,"    -hex    Output hexadecimal numbers\n");
	fprintf(f,"   Line terminating:\n");
	fprintf(f,"    -mac    Mac     [0D]\n");
	fprintf(f,"    -unix   Unix    [0A]\n");
	fprintf(f,"    -win    Windows [0D0A]\n");
	fprintf(f," Default configuration:  -w 16 -hex -win\n");
	exit(0);
}

bool ParseParams(int argc, char* argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "Error: Input file not specified.\n");
		return false;
	}
	params.srcFile = argv[1];

	for(int i=2; i<argc; i++) {
		if (!strcmp(argv[i],"-w")) {
			const int val = atoi(argv[++i]);
			if (val <= 0) {
				fprintf(stderr, "Error on parameter #%d, '%s': The value must be positive integer.\n", i, argv[i]);
				return false;
			} else {
				params.width = val;
			}
		}else if (!strcmp(argv[i],"-header")) {
			params.header = true;
			params.line++;
		}else if (!strcmp(argv[i],"-append")) {		params.append = true;
		}else if (!strcmp(argv[i],"-null")) {			params.nul++;
		}else if (!strcmp(argv[i],"-line")) {			params.line++;
		}else if (!strcmp(argv[i],"-hvar")) {			params.hArrName = argv[++i];
		}else if (!strcmp(argv[i],"-tabs")) {			params.tabs = true;
		}else if (!strcmp(argv[i],"-align")) {		params.align = true;
		}else if (!strcmp(argv[i],"-oct")) {			params.nb = nbOct;
		}else if (!strcmp(argv[i],"-dec")) {			params.nb = nbDec;
		}else if (!strcmp(argv[i],"-hex")) {			params.nb = nbHex;
		}else if (!strcmp(argv[i],"-unix")) {			params.nl = ltUnix;
		}else if (!strcmp(argv[i],"-mac")) {			params.nl = ltMac;
		}else if (!strcmp(argv[i],"-win")) {			params.nl = ltWindows;
		}else if (!strcmp(argv[i],"-help")) {			PrintUsage(stderr);
		} else {
			fprintf(stderr,"Error: Parameter '%s' not recognized.\n", argv[i]);
			return false;
		}
	}
	return true;
}

void WriteChar(FILE *f, int c) {
	if (params.align) {
		switch(params.nb) {
			case nbOct:
				fprintf(f,"0%03o",c);
				break;
			case nbDec:
				fprintf(f,"%3d",c);
				break;
			case nbHex:
				fprintf(f,"0x%02X",c);
				break;
		}
	} else {
		switch(params.nb) {
			case nbOct:
				fprintf(f,"0%o",c);
				break;
			case nbDec:
				fprintf(f,"%d",c);
				break;
			case nbHex:
				fprintf(f,"0x%X",c);
				break;
		}
	}
}

void WriteNewLine(FILE* f) {
/*	switch(params.nl) {
	case ltUnix:
		fputc(0xA, f);
		break;
	case ltMac:
		fputc(0xD, f);
		break;
	case ltWindows:
		fputc(0xD, f);
		fputc(0xA, f);
		break;
	}
*/
	fputc('\n', f);
}

void WriteLineTerm(FILE* f, int n) {
	fputc(',', f);
	if ((n+1)%params.width == 0) {
		WriteNewLine(f);
	} else {
		if (params.tabs) {
			fputc('\t', f);
		} else {
			fputc(' ', f);
		}
	}
}

int main(int argc, char* argv[]) {
	if (!ParseParams(argc, argv))
		return 1;

	if (FILE* f_src = fopen(params.srcFile, "rb")) {
		FILE* f_dest = stdout;
		//if (f_dest) {
			if (params.header) {
				fprintf(f_dest, "unsigned char %s[] = {", params.hArrName);
				WriteNewLine(f_dest);
			}
			int c = fgetc(f_src);
			if (!feof(f_src)) {
				unsigned int n = 0;
				while (true) {
					WriteChar(f_dest, c);
					c = fgetc(f_src);
					if (feof(f_src))
						break;
					WriteLineTerm(f_dest, n++);
				}
				while(params.nul--) {
					WriteLineTerm(f_dest, n++);
					WriteChar(f_dest, 0);
				}
			} else {
				fprintf(stderr,"Warning: The input file was empty. Empty file generated.\n");
			}
			if (params.header) {
				WriteNewLine(f_dest);
				fputs("};", f_dest);
			}
			while(params.line--) {
				WriteNewLine(f_dest);
			}
			//fclose(f_dest);
			fclose(f_src);
			return 0;
		//} else {
			//fclose(f_src);
			//fprintf(stderr,"Error: Can't create file '%s'\n",params.dstFile);
			//return 3;
		//}
	} else {
		fprintf(stderr,"Error: Can't open file '%s'\n",params.srcFile);
		return 2;
	}
}

