#pragma once

std::string util_loadFile(std::string fn);

void util_loadFile(std::string fn, std::vector<std::string>& out);

void util_strSplit(std::string str, char delim, std::vector<std::string>& out);

int util_toInt(std::string value);

float util_toFloat(std::string value);

double util_toDouble(std::string value);

bool util_toBool(std::string value);