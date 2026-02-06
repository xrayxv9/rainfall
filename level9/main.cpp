#include <iostream>
#include <cstring>
#include <unistd.h>

class N {
public:
    char annotation[100]; 
    int value;

    N(int v) : value(v) {
        std::memset(this->annotation, 0, 100);
    }

    virtual void operator+(N &other) {
		this->value += other.value;
    }

    void setAnnotation(char *s) {
        std::memcpy(this->annotation, s, std::strlen(s));
    }
};

int main(int argc, char **argv) {
    if (argc < 2)
		_exit(1);

    N *s1 = new N(5);
    N *s2 = new N(6);

    s1->setAnnotation(argv[1]);

    *s2 + *s1; 
    return 0;
}
