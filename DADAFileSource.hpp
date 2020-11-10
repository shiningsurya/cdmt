#pragma once
#include "cdmt.hpp"
#include "Header.hpp"
#include "DataGeometry.hpp"

// DADA header
#include <ascii_header.h>

constexpr int ASCII_HEADER_SIZE = 4096;

class DADAFileSource {
	private:
		FILE *          dd_fp;
		std::string     filename;

		Byte            ascii_header[ASCII_HEADER_SIZE];

	public:
		DADAFileSource (const std::string& _filename) :
			filename(_filename), dd_fp (nullptr) {
				dd_fp = fopen(filename.c_str(), "rb");
				if (dd_fp == NULL) {
					std::cerr << "Error opening dada file" << std::endl;
				}
		}
		~DADAFileSource () {
			if (dd_fp) fclose (dd_fp);
		}

		int    ReadHeader (header_t& h);
		lindex ReadData (PtrByte, uindex);

		DataType     type;
		DataPol      pol;
};
