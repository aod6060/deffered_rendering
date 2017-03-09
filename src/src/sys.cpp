#include "sys.h"


std::string util_loadFile(std::string fn) {
	std::ifstream in(fn.c_str());
	std::stringstream ss;
	std::string temp;
	while (std::getline(in, temp)) {
		ss << temp << std::endl;
	}
	in.close();
	return ss.str();
}

void util_loadFile(std::string fn, std::vector<std::string>& out) {
	std::ifstream in(fn.c_str());
	std::string temp;
	while (std::getline(in, temp)) {
		out.push_back(temp);
	}
	in.close();
}

void util_strSplit(std::string str, char delim, std::vector<std::string>& out) {
	std::stringstream ss(str);
	std::string temp;
	while (std::getline(ss, temp, delim)) {
		out.push_back(temp);
	}
}

int util_toInt(std::string value) {
	return atoi(value.c_str());
}

float util_toFloat(std::string value) {
	return atof(value.c_str());
}

double util_toDouble(std::string value) {
	return atof(value.c_str());
}

bool util_toBool(std::string value) {
	return (value == "true") ? true : false;
}