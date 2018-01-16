#include <iostream>
#include <ostream>
#include <string>

using namespace std;

class A {
public:

	A(void) : SomeInt(0) {}
	virtual ~A(void) {}

	const string Id(void) const { return "A"; } //non virtual function
	virtual const string VirtId(void) const { return "A"; } //virtual function (overridable)

	int GetSomeInt(void) const { return SomeInt; }
	int SomeInt;
};

class B1 : virtual public A { //virtual inheritance
public:
	B1(void) : A(), m_fValue(10.0) {
		// Because SomeInt isn't a member of B, we cannot initialize it in the initializer list before the open brace where we initialize the A base class and the m_fValue member data.   
		SomeInt = 10;
	}
	virtual ~B1(void) {}

	const string Id(void) const { return "B1"; }
	virtual const string VirtId(void) const override { return "B1"; }

	const string Conflict(void) const { return "B1::Conflict()"; }
private:
	float   m_fValue;
};

class B2 : virtual public A { //virtual inheritance
public:
	B2(void) : A() {}
	virtual ~B2(void) {}

	const string Id(void) const { return "B2"; }
	virtual const string VirtId(void) const override { return "B2"; }

	const string Conflict(void) const { return "B2::Conflict()"; }
};

class B3 : public A { //non virtual inheritance
public:
	B3(void) : A() {}
	virtual ~B3(void) {}

	const string Id(void) const { return "B3"; }
	virtual const string VirtId(void) const override { return "B3"; }

	const string Conflict(void) const { return "B3::Conflict()"; }
};

class VirtualInheritanceClass : virtual public B1, virtual public B2 {
public:
	VirtualInheritanceClass(void) : B1(), B2(), m_id("VirtualClass") {}
	virtual ~VirtualInheritanceClass(void) {}

	const string Id(void) const { return m_id; }
	virtual const string VirtId(void) const override { return m_id; } //override: need disambiguate A::VirtId

private:  string  m_id;
};

class NonVirtualInheritanceClass : public B1, public B3 {
public:
	NonVirtualInheritanceClass(void) : B1(), B3(), m_id("NonVirtualClass") {}
	virtual ~NonVirtualInheritanceClass(void) {}

	const string Id(void) const { return m_id; }
	virtual const string VirtId(void) const override { return m_id; } //override: need disambiguate A::VirtId

	// If we decided we wanted to use B1::Conflict, we could use  
	// a using declaration. In this case, we would be saying that   
	// calling NonVirtualInheritanceClass::Conflict means call B1::Conflict  	
	//using B1::Conflict;
	// We can also use it to resolve ambiguity between member  
	// data. In this case, we would be saying that   
	// NonVirtualClass::SomeInt means B3::SomeInt, so   
	// the nvC.SomeInt statement in  
	// DemonstrateNonVirtualInheritance would be legal, even  
	// though IntelliSense says otherwise.  
	//using B3::SomeInt;

private:  string    m_id;
};

void DemonstrateNonVirtualInheritance(void) {

	cout << "------------- DemonstrateNonVirtualInheritance -------------" << endl;
	NonVirtualInheritanceClass nvC = NonVirtualInheritanceClass();

	// SomeInt is ambiguous since there are two copies of A, one  
	// indirectly from B1 and the other indirectly from B3.  
	//nvC.SomeInt = 30;
	//or
	//nvC.A::SomeInt = 30;

	// But you can access the two copies of SomeInt by specifying which  
	// base class' SomeInt you want. Note that if NonVirtualInheritanceClass also  
	// directly inherited from A, then this too would be impossible.  	
	nvC.B1::SomeInt = 20;
	nvC.B3::SomeInt = 20;

	// It is impossible to create a reference to A due to ambiguity.  
	//A& nvCA = nvC; 

	// We can create references to B1 and B3 though.  
	B1& nvCB1 = nvC;
	B3& nvCB3 = nvC;
	
	// If we want a reference to some particular A, we can now get one.  
	A& nvCAfromB1 = nvCB1;
	A& nvCAfromB3 = nvCB3;

	// To demonstrate that there are two copies of A's data. 
	cout <<				
		"B1::A::SomeInt = " << nvCAfromB1.SomeInt << endl <<
		"B3::A::SomeInt = " << nvCAfromB3.SomeInt << endl <<
		"B1::SomeInt = " << nvCB1.SomeInt << endl <<
		"B3::SomeInt = " << nvCB3.SomeInt << endl <<
		endl;

	++nvCB1.SomeInt;
	nvCB3.SomeInt += 20;


	cout <<				
		"B1::A::SomeInt = " << nvCAfromB1.SomeInt << endl <<
		"B3::A::SomeInt = " << nvCAfromB3.SomeInt << endl <<
		"B1::SomeInt = " << nvCB1.SomeInt << endl <<
		"B3::SomeInt = " << nvCB3.SomeInt << endl <<
		endl;

	// Let's see a final demo of the result. Note that the Conflict  
	// member function is also ambiguous because both B1 and B3 have  
	// a member function named Conflict with the same signature.  
	cout <<
		typeid(nvC).name() << endl <<
		nvC.Id() << endl << nvC.VirtId() << endl <<
		//// This is ambiguous between B1 and B3   
		//nvC.Conflict() << endl <<   
		// But we can solve that ambiguity.   
		nvC.B3::Conflict() << endl <<
		nvC.B1::Conflict() << endl <<
		//// GetSomeInt is ambiguous too.   
		//nvC.GetSomeInt() << endl <<   
		endl <<      
		
		typeid(nvCB3).name() << endl << 
		nvCB3.Id() << endl << 
		nvCB3.VirtId() << endl << nvCB3.Conflict() << endl << 
		endl <<

		typeid(nvCB1).name() << endl << 
		nvCB1.Id() << endl << 
		nvCB1.VirtId() << endl << 
		nvCB1.GetSomeInt() << endl << 
		nvCB1.Conflict() << endl << 
		endl;
}

void DemonstrateVirtualInheritance(void) {

	cout << "------------- DemonstrateVirtualInheritance -------------" << endl;
	VirtualInheritanceClass vC = VirtualInheritanceClass();

	// This works since VirtualInheritanceClass has virtual inheritance of B1,  
	// which has virtual inheritance of A, and VirtualInheritanceClass has virtual  
	// inheritance of A, which means all inheritances of A are virtual  
	// and thus there is only one copy of A.  	
	vC.SomeInt = 30; 

	// We can create a reference directly to A and also to B1 and B2.  
	A& vCA = vC;  
	B1& vCB1 = vC;  
	B2& vCB2 = vC; 

	// To demonstrate that there is just one copy of A's data.  
	cout <<   
		"this::SomeInt = " << vC.SomeInt << endl <<
		"A::SomeInt = " << vCA.SomeInt << endl <<
		"B1::SomeInt = " << vCB1.SomeInt << endl <<   
		"B3::SomeInt = " << vCB2.SomeInt << endl << 
		endl;

	++vCB1.SomeInt;  
	vCB2.SomeInt += 20;

	cout <<
		"this::SomeInt = " << vC.SomeInt << endl <<
		"A::SomeInt = " << vCA.SomeInt << endl <<
		"B1::SomeInt = " << vCB1.SomeInt << endl <<
		"B3::SomeInt = " << vCB2.SomeInt << endl <<
		endl;

	// Let's see a final demo of the result. Note that the Conflict  
	// member function is still ambiguous because both B1 and B2 have  
	// a member function named Conflict with the same signature.  
	cout <<   
		typeid(vC).name() << endl <<   
		vC.Id() << endl <<   
		vC.VirtId() << endl <<   
		vC.B2::Id() << endl <<   
		vC.B2::VirtId() << endl <<   
		vC.B1::Id() << endl <<   
		vC.B1::VirtId() << endl <<   
		vC.A::Id() << endl <<   
		vC.A::VirtId() << endl <<   
		// This is ambiguous between B1 and B2   
		//vC.Conflict() << endl <<   
		// But we can solve that ambiguity.   
		vC.B2::Conflict() << endl <<   
		vC.B1::Conflict() << endl <<   
		// There's no ambiguity here because of virtual inheritance.   
		vC.GetSomeInt() << endl <<   
		endl <<      

		typeid(vCB2).name() << endl <<   vCB2.Id() << endl <<   vCB2.VirtId() << endl <<   vCB2.Conflict() << endl <<  
		endl << 

		typeid(vCB1).name() << endl << vCB1.Id() << endl << vCB1.VirtId() << endl << vCB1.GetSomeInt() << endl << vCB1.Conflict() << endl << 
		endl <<

		typeid(vCA).name() << endl << vCA.Id() << endl << vCA.VirtId() << endl << vCA.GetSomeInt() << endl << 
		endl;
}


int _main() {
	DemonstrateNonVirtualInheritance();
	DemonstrateVirtualInheritance();
	return 0;
}