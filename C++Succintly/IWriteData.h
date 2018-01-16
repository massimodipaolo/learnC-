#pragma once

#include <string>

class IWriteData {
public:
	IWriteData(void) {}
	virtual ~IWriteData(void) {}
	virtual void Write(const std::string input) = 0;
	virtual void WriteLine(const std::string input) = 0;
};

/*
__interface IWriteData {
	// No constructor / deconstructor
	virtual void Write(const std::string input) = 0;
	virtual void WriteLine(const std::string input) = 0;
};
*/