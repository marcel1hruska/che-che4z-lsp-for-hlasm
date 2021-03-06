/*
 * Copyright (c) 2019 Broadcom.
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Broadcom, Inc. - initial API and implementation
 */

#pragma once
#include "gmock/gmock.h"
#include <utility>
#include "antlr4-runtime.h"
#include "../src/ebcdic_encoding.h"
#include "../include/shared/lexer.h"
#include "hlasmparser.h"
#include "shared/token_stream.h"
#include "shared/input_source.h"
#include "../src/analyzer.h"
#include "../src/processing/context_manager.h"
#include "../src/expressions/visitors/expression_analyzer.h"
#include "../src/processing/instruction_sets/macro_processor.h"


using namespace hlasm_plugin::parser_library;
using namespace hlasm_plugin::parser_library::context;
using namespace hlasm_plugin::parser_library::semantics;
using namespace hlasm_plugin::parser_library::processing;
using namespace hlasm_plugin::parser_library::expressions;

const size_t size_t_zero = static_cast<size_t>(0);

class empty_attribute_provider : public attribute_provider
{
	virtual const attribute_provider::resolved_reference_storage&
		lookup_forward_attribute_references(attribute_provider::forward_reference_storage references)
	{
		(void)references;
		return resolved_symbols;
	}
};

//returns contents of source file
std::string get_content(std::string source)
{
	std::ifstream ifs(source);
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	return content;
}

std::pair<bool, antlr4::ParserRuleContext*> try_parse_sll(hlasm_plugin::parser_library::generated::hlasmparser& h_parser)
{
	h_parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->setPredictionMode(antlr4::atn::PredictionMode::SLL); // try with simpler/faster SLL(*)
	// we don't want error messages or recovery during first try
	h_parser.removeErrorListeners();
	h_parser.setErrorHandler(std::make_shared<antlr4::BailErrorStrategy>());
	try {
		auto tree = h_parser.program();
		// if we get here, there was no syntax error and SLL(*) was enough;
		// there is no need to try full LL(*)
		return { true,tree };
	}
	catch (antlr4::RuntimeException &) {
		std::cout << "SLL FAILURE" << std::endl;

		auto tokens = h_parser.getTokenStream();
		std::cout << tokens->get(tokens->index())->getLine() << std::endl;
		// The BailErrorStrategy wraps the RecognitionExceptions in
		// RuntimeExceptions so we have to make sure we're detecting
		// a true RecognitionException not some other kind
		dynamic_cast<antlr4::BufferedTokenStream*>( tokens)->reset(); // rewind input stream
		// back to standard listeners/handlers
		h_parser.addErrorListener(&antlr4::ConsoleErrorListener::INSTANCE);
		h_parser.setErrorHandler(std::make_shared<antlr4::DefaultErrorStrategy>());
		h_parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->setPredictionMode(antlr4::atn::PredictionMode::LL); // try full LL(*)
		auto tree = h_parser.program();
		return { false,tree };
	}
}
