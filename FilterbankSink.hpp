#pragma once
#include "cdmt.hpp"
#include "Header.hpp"
#include <fstream>

class FilterbankSink{
	private:
		FILE * fb_fp;
		std::string filename;

		inline double sigproc_ra  (double);
		inline double sigproc_dec (double);

		template<typename T>
			inline void send (const char * , const T);
		inline void send (const char *);
		inline void send (const string_t& );

	public:
		FilterbankSink(const std::string& _filename) :
			filename(_filename), fb_fp (nullptr) {
				fb_fp = fopen(filename.c_str(), "wb+");
				if (fb_fp == NULL) {
					std::cerr << "Error creating filterbank file" << std::endl;
				}
			}

		void Close () {
			// write duration
			if(fb_fp) fclose(fb_fp);
			fb_fp = nullptr;
		}

		~FilterbankSink() {
			Close ();
		}

		void Header (const header_t& );
		void Data   (PtrByte, uindex);
};
