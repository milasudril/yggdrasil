//@	{"targets":[{"name":"yggdrasil_defs.hpp", "type":"include"}]}

#ifndef YGGDRASIL_DEFS_HPP
#define YGGDRASIL_DEFS_HPP

#include "analib/inlinestring/inlinestring.hpp"

#include <cstdint>
#include <cstddef>
#include <cassert>

namespace Yggdrasil
	{
	constexpr Analib::InlineString MagicNumber{"yggdrasil v 1.0"};
	constexpr uint32_t ByteOrderMarker{0x01020304};
	constexpr size_t KeySize{32};
	constexpr size_t ArrayElemCountSize{4};
	constexpr size_t TypeIdSize{1};
	}

#endif
