#include <string>
#include <utility>
#include <string>
#include <iostream>
#include <type_traits>
#include <string_view>
#include "sampleReflection.hpp"

// g++-11 main.cpp -o main -std=c++20
class People
{
public:
	std::string name;
	int age;
	std::string get_name() const { return name; };
};

class Student{
public:
	int grade;
	std::string name;
	void setName(){};
};


REFL_TYPE(People)
	REFL_FIELD(age)
	REFL_FIELD(name)
	REFL_METHOD(get_name)
REFL_TYPE_END

REFL_TYPE(Student)
	REFL_FIELD(name)
	REFL_FIELD(grade)
	REFL_METHOD(setName)
REFL_TYPE_END

int main()
{

	static_assert(FixedString{"Hello"} == "Hello");

	//part 1
	static_assert(ReflectionType<People>::getMember<"name">::name == "name");
	static_assert(ReflectionType<People>::getMember<"get_name">::name == "get_name");

	using resultType = ReflectionType<People>::getMember<"get_name">::resultType;
	static_assert(std::is_same_v<resultType, std::string>);

	static_assert(ReflectionType<Student>::getFieldMemberViaIndex<1>::name == "name");
	static_assert(ReflectionType<Student>::getFieldMemberViaIndex<2>::name == "grade");

	// //part 2
	People p{ "Jim", 20 };
	travelType(&p);

	return 1;
}











