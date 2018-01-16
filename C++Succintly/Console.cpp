#include <iostream>
#include <ostream>
#include "Console.h"

using namespace std;

void Console::Write(const string input) { cout << input; }
void Console::WriteLine(const string input) { cout << input << endl; }

int main() {
	//// The following line is illegal since IWriteData is abstract.  
	//IWriteData iwd = IWriteData(); 

	//// The following line is also illegal. You cannot have an  instance of IWriteData.   
	//IWriteData iwd = ConsoleWriteData(); 

	Console c = Console();

	// You can create an IWriteData reference to an instance of a class that derives from IWriteData.  
	IWriteData& r_iwd = c; 

	// You can also create an IWriteData pointer to an instance of a class that derives from IWriteData.  
	IWriteData* p_iwd = &c; 

	c.WriteLine("Class");  
	r_iwd.WriteLine("Reference");  
	p_iwd->WriteLine("Pointer");

	return 0;
}