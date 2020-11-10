#include "cdmt.hpp"
#include "FilterbankSink.hpp"


double FilterbankSink::sigproc_ra(double ra) {
	float hh = (180.0f / M_PI) * (24.0f / 360.0f) * ra;
	float mm = (hh - int(hh))  * 60.0f;
	float ss = (mm - int(mm))  * 60.0f;
	return ( int(hh) * 1e4 ) + ( int(mm) * 1e2 ) + ss;
}
double FilterbankSink::sigproc_dec(double dec) {
	float dd = (180.0f / M_PI) * fabs(dec);
	float mm = (dd - int(dd))  * 60.0f;
	float ss = (mm - int(mm))  * 60.0f;
	return ( int(dd) * 1e4 ) + ( int(mm) * 1e2 ) + ss;
}

template<typename T>
void FilterbankSink::send(const char *str, const T data) {
	// label
	send(str);
	// data
	fwrite(&data, sizeof(T), 1, fb_fp);
}
void FilterbankSink::send(const char * str) {
	int len = strlen(str);
	// first goes length of string
	fwrite(&len, sizeof(int), 1, fb_fp);
	// writing str
	fwrite(str, sizeof(char), len, fb_fp);
}
void FilterbankSink::send(const std::string& str) {
	auto s = str;
	int len = s.size();
	// first goes length of string
	fwrite(&len, sizeof(int), 1, fb_fp);
	// writing str
	fwrite(str.c_str(), sizeof(char), len, fb_fp);
}

void FilterbankSink::Header(const header_t& inhead) {
	// dance of functions
	send("HEADER_START");
	// source_name
	send("source_name"); 
	send(inhead.name);
	// barycentric
	send("barycentric", 0);
	// telescope ID
	//send("stationid"); send(inhead.observatory);
	// positions
	/**
		XXX sigproc filterbank header format 
		requires `src_raj` and `src_dej` to be doubles
		dada has them in str.

		Low priority work
	**/
	//send("src_raj"); send(inhead.ra);
	//send("src_dej"); send(inhead.dec);
	// datatype
	send("data_type", 1);
	// freq
	send("fch1", inhead.fch1);
	send("foff", inhead.foff);
	// data
	send("nchans", inhead.nchans);
	send("nbits", inhead.nbits);
	send("nifs", 1);
	// time
	send("tstart",inhead.tstart);
	// division by 1e6 bc DADA tsamp unit is us
	send("tsamp", inhead.tsamp/1e6);
	send("HEADER_END");
}

void FilterbankSink::Data(PtrByte in, uindex bytes_chunk) {
	fwrite(in, 1, bytes_chunk, fb_fp);
}

