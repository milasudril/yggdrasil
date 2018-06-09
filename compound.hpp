//@	{
//@	 "targets":[{"name": "compound.hpp", "type":"include"}]
//@	,"dependencies_extra":[{"ref":"compound.o","rel":"implementation"}]
//@	}

#ifndef TDS_COMPOUND_HPP
#define TDS_COMPOUND_HPP

#include "strint/common_types.hpp"

#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include <memory>

namespace Tds
	{
	class Compound
		{
		public:

		private:
			std::unordered_map
				<
				 std::string
				,std::variant
					<
					 Strint::Int8
					,Strint::Int16
					,Strint::Int32
					,Strint::Int64
					,Strint::UInt8
					,Strint::UInt16
					,Strint::UInt32
					,Strint::UInt64
					,float
					,double
					,std::unique_ptr<std::string>
					,std::unique_ptr<Compound>
					
					,std::vector<Strint::Int8 >
					,std::vector<Strint::Int16>
					,std::vector<Strint::Int32>
					,std::vector<Strint::Int64>
					,std::vector<Strint::UInt8>
					,std::vector<Strint::UInt16>
					,std::vector<Strint::UInt32>
					,std::vector<Strint::UInt64>
					,std::vector<float>
					,std::vector<double>
					,std::vector<std::string>
					,std::vector<Compound>
					>
				> m_content;
		};
	};

#endif

