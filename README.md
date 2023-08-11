# sampleReflection
主要参考refl-cpp 的实现，在C++中使用 宏和模板 实现中很简单的反射机制

## 编译环境
需要编译器支持c++20，主要原因是因为C++20非类型模板参数（NTTP）得到了扩展，能够支持的类型不仅仅是整型、枚举、指针和引用。

FixedString 就是为了支持让字符串能够成为一个非类型模板参数

## example 
参考main.cpp中的实现，定义类型通过以下方式

```
REFL_TYPE(Student)
	REFL_FIELD(fieldName)
	REFL_METHOD(methodName)
REFL_TYPE_END
```

1. getMember 通过名字来访问对应的member，包括REFL_METHOD定义的函数也能获取
2. getFieldMemberViaIndex 通过数字来访问对应的 member，下标从1开始，注意这个接口只算REFL_FIELD 定义的member

member 提供了 类成员变量的名字、类型、指针的接口