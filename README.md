# Yggrdrasil

Yggdrasil is a binary hierarcical file format inteded for simple serialization/deserialization.
The format stores data items as tuples each consisting of a key, a type identifier, and its value.
If the type identifier indicates an array or a sub-object, the number of items in that item is
stored directly after the type identifer. For more details including supported types, see
`yggdrasil_defs.hpp`.

## When is Yggdrasil a good format (aka chosen optimization points)

 * Records are changed infrequently, or at least data does not need to be reserialized for every
   update.
 * There are not too many keys at each level
 * The actual payload is typically long arrays
 * When data needs to be loaded from a non-seekable source, such as a compressed file, network
   socket, pipe, or a tape drive.

## When is Yggrasil a bad format

 * Random access to serialized data is needed
 * There are frequent updates to the serialized data

## Usage

The class Yggdrasil::Compound is used to represent the "world tree" internally. This is basically
a recursive std::map, with appropriate getters and setters. Data in a world tree can be serialized
by calling `Yggdrasil::store`. To load a world tree, call `Yggdrasil::load`. An example of basic
usage can be found in `compound.test.cpp`, as well as in `test/compound_test.hpp`.
