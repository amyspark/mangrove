// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/console>
#include "symbolTable.hxx"
#include "../parser/parser.hxx"

using substrate::console;
using namespace mangrove::ast::symbolTable;
using mangrove::parser::Parser;

// XXX: Because C++ needs use of std::shared_ptr here, we cannot auto-push the table
// onto the parser stack.
SymbolTable::SymbolTable(const Parser &parser) noexcept : _parentTable{parser.symbolTable()} { }

[[nodiscard]] Symbol *SymbolTable::add(String ident) noexcept
{
	// Check if the ident is already in the table, if it is this must fail.
	if (_table.find(ident) != _table.end())
	{
		console.error("Symbol already defined in current scope"sv);
		return nullptr;
	}
	auto symbol{std::make_unique<Symbol>(std::move(ident))};
	const auto entry{_table.emplace(symbol->value(), std::move(symbol))};
	// Validate the insertion succeeded
	if (!entry.second)
	{
		console.error("Failed to insert symbol into table"sv);
		return nullptr;
	}
	// entry is a pair of <pair, bool> - the first member of the first pair is the map iterator
	// the second member of the second pair is the value member of the set entry.
	return entry.first->second.get();
}
