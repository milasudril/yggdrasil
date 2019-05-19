//@	{"targets":[{"name":"yggdrasil_typeid.test", "type":"application", "autorun":1}]}

#undef NDEBUG

#include "compound.hpp"
#include "yggdrasil_typeid.hpp"

struct MyExceptionPolicy
	{
	[[noreturn]]
	static void keyNotFound(std::string_view key)
		{throw std::string(key);}

	[[noreturn]]
	static void keyValueHasWrongType(std::string_view key, size_t actualType)
		{throw std::string(key);}

	[[noreturn]]
	static void keyAlreadyExists(Yggdrasil::KeyType const& key)
		{throw std::runtime_error(std::string(std::begin(key), std::end(key)));}
	};

using Compound = Yggdrasil::Compound<MyExceptionPolicy>;
using SupportedTypes = Compound::SupportedTypes;
template<class T>
using vec4_t = Yggdrasil::vec4_t<T>;
using Minifloat = Yggdrasil::Minifloat;
using Half = Yggdrasil::Half;
using String = Yggdrasil::String;


static_assert(Yggdrasil::TypeId{}.value() == Yggdrasil::TypeIdValue::VecInt8);
static_assert(SupportedTypes::getTypeIndex<vec4_t<int8_t>>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::VecInt8));
static_assert(Yggdrasil::TypeId{}.category(Yggdrasil::TypeCategory::UnsignedInteger).value()
	== Yggdrasil::TypeIdValue::VecUint8);
static_assert(SupportedTypes::getTypeIndex<vec4_t<uint8_t>>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::VecUint8));
static_assert(Yggdrasil::TypeId{}.category(Yggdrasil::TypeCategory::Float).value()
	== Yggdrasil::TypeIdValue::VecFloat8);
static_assert(SupportedTypes::getTypeIndex<vec4_t<Minifloat>>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::VecFloat8));

static_assert(Yggdrasil::TypeId{}.size(Yggdrasil::ElementSize::Two).value() == Yggdrasil::TypeIdValue::VecInt16);
static_assert(SupportedTypes::getTypeIndex<vec4_t<int16_t>>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::VecInt16));
static_assert(Yggdrasil::TypeId{}.size(Yggdrasil::ElementSize::Two).category(Yggdrasil::TypeCategory::UnsignedInteger).value()
	== Yggdrasil::TypeIdValue::VecUint16);
static_assert(SupportedTypes::getTypeIndex<vec4_t<uint16_t>>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::VecUint16));
static_assert(Yggdrasil::TypeId{}.size(Yggdrasil::ElementSize::Two).category(Yggdrasil::TypeCategory::Float).value()
	== Yggdrasil::TypeIdValue::VecFloat16);
static_assert(SupportedTypes::getTypeIndex<vec4_t<Half>>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::VecFloat16));

static_assert(Yggdrasil::TypeId{}.size(Yggdrasil::ElementSize::Four).value() == Yggdrasil::TypeIdValue::VecInt32);
static_assert(SupportedTypes::getTypeIndex<vec4_t<int32_t>>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::VecInt32));
static_assert(Yggdrasil::TypeId{}.size(Yggdrasil::ElementSize::Four).category(Yggdrasil::TypeCategory::UnsignedInteger).value()
	== Yggdrasil::TypeIdValue::VecUint32);
static_assert(SupportedTypes::getTypeIndex<vec4_t<uint32_t>>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::VecUint32));
static_assert(Yggdrasil::TypeId{}.size(Yggdrasil::ElementSize::Four).category(Yggdrasil::TypeCategory::Float).value()
	== Yggdrasil::TypeIdValue::VecFloat32);
static_assert(SupportedTypes::getTypeIndex<vec4_t<float>>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::VecFloat32));

static_assert(Yggdrasil::TypeId{}.size(Yggdrasil::ElementSize::Eight).value() == Yggdrasil::TypeIdValue::VecInt64);
static_assert(SupportedTypes::getTypeIndex<vec4_t<int64_t>>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::VecInt64));
static_assert(Yggdrasil::TypeId{}.size(Yggdrasil::ElementSize::Eight).category(Yggdrasil::TypeCategory::UnsignedInteger).value()
	== Yggdrasil::TypeIdValue::VecUint64);
static_assert(SupportedTypes::getTypeIndex<vec4_t<uint64_t>>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::VecUint64));
static_assert(Yggdrasil::TypeId{}.size(Yggdrasil::ElementSize::Eight).category(Yggdrasil::TypeCategory::Float).value()
	== Yggdrasil::TypeIdValue::VecFloat64);
static_assert(SupportedTypes::getTypeIndex<vec4_t<double>>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::VecFloat64));



static_assert(Yggdrasil::TypeId{}.attributes(0).value() == Yggdrasil::TypeIdValue::Int8);
static_assert(SupportedTypes::getTypeIndex<int8_t>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::Int8));
static_assert(Yggdrasil::TypeId{}.attributes(0).category(Yggdrasil::TypeCategory::UnsignedInteger).value()
	== Yggdrasil::TypeIdValue::Uint8);
static_assert(SupportedTypes::getTypeIndex<uint8_t>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::Uint8));
static_assert(Yggdrasil::TypeId{}.attributes(0).category(Yggdrasil::TypeCategory::Float).value()
	== Yggdrasil::TypeIdValue::Float8);
static_assert(SupportedTypes::getTypeIndex<Minifloat>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::Float8));
static_assert(Yggdrasil::TypeId{}.category(Yggdrasil::TypeCategory::String).value() == Yggdrasil::TypeIdValue::String);
static_assert(SupportedTypes::getTypeIndex<String>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::String));

static_assert(Yggdrasil::TypeId{}.attributes(0).size(Yggdrasil::ElementSize::Two).value() == Yggdrasil::TypeIdValue::Int16);
static_assert(SupportedTypes::getTypeIndex<int16_t>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::Int16));
static_assert(Yggdrasil::TypeId{}.attributes(0).size(Yggdrasil::ElementSize::Two).category(Yggdrasil::TypeCategory::UnsignedInteger).value()
	== Yggdrasil::TypeIdValue::Uint16);
static_assert(SupportedTypes::getTypeIndex<uint16_t>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::Uint16));

static_assert(Yggdrasil::TypeId{}.attributes(0).size(Yggdrasil::ElementSize::Two).category(Yggdrasil::TypeCategory::Float).value()
	== Yggdrasil::TypeIdValue::Float16);
static_assert(SupportedTypes::getTypeIndex<Half>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::Float16));

static_assert(Yggdrasil::TypeId{}.attributes(0).size(Yggdrasil::ElementSize::Four).value() == Yggdrasil::TypeIdValue::Int32);
static_assert(SupportedTypes::getTypeIndex<int32_t>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::Int32));
static_assert(Yggdrasil::TypeId{}.attributes(0).size(Yggdrasil::ElementSize::Four).category(Yggdrasil::TypeCategory::UnsignedInteger).value()
	== Yggdrasil::TypeIdValue::Uint32);
static_assert(SupportedTypes::getTypeIndex<uint32_t>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::Uint32));
static_assert(Yggdrasil::TypeId{}.attributes(0).size(Yggdrasil::ElementSize::Four).category(Yggdrasil::TypeCategory::Float).value()
	== Yggdrasil::TypeIdValue::Float32);
static_assert(SupportedTypes::getTypeIndex<float>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::Float32));

static_assert(Yggdrasil::TypeId{}.attributes(0).size(Yggdrasil::ElementSize::Eight).value() == Yggdrasil::TypeIdValue::Int64);
static_assert(SupportedTypes::getTypeIndex<int64_t>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::Int64));
static_assert(Yggdrasil::TypeId{}.attributes(0).size(Yggdrasil::ElementSize::Eight).category(Yggdrasil::TypeCategory::UnsignedInteger).value()
	== Yggdrasil::TypeIdValue::Uint64);
static_assert(SupportedTypes::getTypeIndex<uint64_t>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::Uint64));
static_assert(Yggdrasil::TypeId{}.attributes(0).size(Yggdrasil::ElementSize::Eight).category(Yggdrasil::TypeCategory::Float).value()
	== Yggdrasil::TypeIdValue::Float64);
static_assert(SupportedTypes::getTypeIndex<double>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::Float64));
static_assert(Yggdrasil::TypeId{}.category(Yggdrasil::TypeCategory::Compound).value() == Yggdrasil::TypeIdValue::Compound);
static_assert(SupportedTypes::getTypeIndex<Compound>() == static_cast<uint8_t>(Yggdrasil::TypeIdValue::Compound));

static_assert(Yggdrasil::TypeId{}
	.attributes(Yggdrasil::TypeAttribute::Array)
	.size(Yggdrasil::ElementSize::Four)
	.category(Yggdrasil::TypeCategory::Float).value() == Yggdrasil::TypeIdValue::ArrayFloat32);

static_assert(Yggdrasil::TypeId{}
	.attributes(Yggdrasil::TypeAttribute::Array | Yggdrasil::TypeAttribute::Vector)
	.size(Yggdrasil::ElementSize::Four)
	.category(Yggdrasil::TypeCategory::Float).value() == Yggdrasil::TypeIdValue::ArrayVecFloat32);


int main()
	{return 0;}
