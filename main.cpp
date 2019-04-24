#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <exception>

#define VCLASS(className) \
class className { \
public: \
    className(); \
    std::map<std::string, std::function<void(void)>> _vtable; \

#define END_VCLASS };

#define VSECTION \
    void _add_vmethods() { \
        
#define END_VSECTION };

#define VMETHOD(methodName) \
        this->_vtable[#methodName] = [this]() 

#define CONSTRUCTOR(className) \
className::className() { \
    _add_vmethods(); \
    
#define END_CONSTRUCTOR };

#define VDERIVED(derivedClassName, baseClassName) \
class derivedClassName : public baseClassName { \
public: \
    derivedClassName();

#define END_VDERIVED };


#define VCALL(cls, methodName) \
    if (cls->_vtable.find( #methodName ) == cls->_vtable.end()) { \
        throw std::runtime_error(std::string("Unknown function '") + #methodName + "' in class '" + #cls + "'."); \
    } else \
        cls->_vtable[ #methodName ]();

// ================================== USAGE ===================================

// BASE CLASS EXAMPLE

VCLASS(Base)
public:
    int a = 0;
VSECTION
    VMETHOD(foo) { 
        std::cout << "foo method\n"; 
        std::cout << "a = " << this->a << std::endl; 
    };
    VMETHOD(bar) { 
        std::cout << "bar BASE method\n"; 
        std::cout << "a = " << this->a << std::endl;
    };
END_VSECTION
END_VCLASS

CONSTRUCTOR(Base)
    std::cout << "Base constructor called!\n";
END_CONSTRUCTOR

// DERIVED CLASS EXAMPLE

VDERIVED(Derived, Base)
public:
    int a = 1;
VSECTION
    VMETHOD(bar) { 
        std::cout << "bar OVERLOADED method\n"; 
        std::cout << "a = " << this->a << std::endl;
    };
    VMETHOD(wibble) { 
        std::cout << "wibble not exists in Base, that's never called!\n"; 
    };
END_VSECTION
END_VDERIVED

CONSTRUCTOR(Derived)
    std::cout << "Derived constructor called!\n";
END_CONSTRUCTOR     

int main() 
{
    Base* a = new Base;
    Derived* b = new Derived;

    Base* pbase_b = reinterpret_cast<Base*>(b);

    std::cout << "BASE class, BASE method:" << std::endl;
    VCALL(a, foo);
    VCALL(a, bar);

    std::cout << "DERIVED class, BASE method:" << std::endl;
    VCALL(b, foo);
    std::cout << "DERIVED class, DERIVED method:" << std::endl;
    VCALL(pbase_b, bar);

    // uncomment to see how program fails while calling missed method
    // VCALL(a, wibble);

    return 0;
}
