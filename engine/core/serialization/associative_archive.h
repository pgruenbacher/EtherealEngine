#pragma once

// #include "cereal/archives/xml.hpp"
// namespace cereal
// {
// 	using oarchive_associative_t = XMLOutputArchive;
// 	using iarchive_associative_t = XMLInputArchive;
// }

#include "cereal/archives/json.hpp"
// #include "cereal/archives/xml.hpp"
namespace cereal
{
using oarchive_associative_t = JSONOutputArchive;
using iarchive_associative_t = JSONInputArchive;
// I prefer xml since I believe it associates better with git tracking for arrays of components.
// hmm actually maybe not, xml is pretty messy too.
// using oarchive_associative_t = XMLOutputArchive;
// using iarchive_associative_t = XMLInputArchive;
}
