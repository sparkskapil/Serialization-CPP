#pragma once
#include "../Serialization/Serialization.h"
using namespace Serialization;

class MockClass :virtual public ISerializable {
	std::string String;
	int Integer;
	float Float;

public:
	MockClass();
	MockClass(std::string,int,float);
	void serialize(ofstream&) override;
	void deserialize(ifstream&) override;

	bool operator==(const MockClass&);
};