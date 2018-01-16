#pragma once

#include "IWriteData.h"

class Console : public IWriteData {
public:
	Console(void) { std::cout << "Console instance created" << std::endl; } // function with body {}
	virtual ~Console(void) {}
	
	virtual void Write(const std::string input); // function without body
	virtual void WriteLine(const std::string input);
};
