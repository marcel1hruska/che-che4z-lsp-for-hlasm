#ifndef CONTEXT_SECTION_H
#define CONTEXT_SECTION_H

#include "location_counter.h"

#include <vector>

namespace hlasm_plugin {
namespace parser_library {
namespace context {

enum class section_kind
{
	DUMMY, COMMON, EXECUTABLE, READONLY
};


//class representing section (CSECT/DSECT ...)
class section
{
	//location counter assigned to this section
	std::vector<loctr_ptr> loctrs_;
	location_counter* curr_loctr_;

	id_storage& ids_;
public:
	//unique identifier
	const id_index name;
	const section_kind kind;

	//access list of location counters
	const std::vector<loctr_ptr>& location_counters() const;

	section(id_index name, const section_kind kind, id_storage& ids);

	//sets current location counter
	void set_location_counter(id_index loctr_name);

	//checker method whether the location counter with provided name already exists in the section
	bool counter_defined(id_index name);

	//access current location counter
	location_counter& current_location_counter() const;

	//creates layout of the section
	void finish_layout();
};

}
}
}
#endif
