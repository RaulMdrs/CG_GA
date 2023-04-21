#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Obj.h"


class LeitorOBJ
{

public:
	LeitorOBJ();
	~LeitorOBJ();


	Obj LerOBJ(std::string name);

};