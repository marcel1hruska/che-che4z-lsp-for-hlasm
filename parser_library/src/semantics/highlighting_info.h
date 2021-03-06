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

#ifndef HIGHLIGHTING_INFO
#define HIGHLIGHTING_INFO

#include <map>
#include <string>
#include <vector>

#include "shared/protocol.h"
#include "operand.h"

namespace hlasm_plugin {
namespace parser_library {
namespace semantics {

struct versioned_document
{ 
	versioned_document() : version(0) {};
	versioned_document(std::string uri) : uri(std::move(uri)), version(0) {}
	std::string uri;
	version_t version;
};

using lines_info = std::vector<token_info>;

struct continuation_info
{
	std::vector<position> continuation_positions;
	size_t continue_column = 15;
	size_t continuation_column = 71;
};

struct highlighting_info
{
	highlighting_info() {};
	highlighting_info(std::string document_uri) : document(std::move(document_uri)) {}

	highlighting_info(versioned_document document, lines_info lines) : document(std::move(document)), lines(std::move(lines)) {};
	versioned_document document;
	lines_info lines;
	continuation_info cont_info;
};

}
}
}

#endif
