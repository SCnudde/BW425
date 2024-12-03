#pragma once

namespace our
{
	enum EXIT_CODES : unsigned int
	{
		SUCCESS,
		DIVISION_BY_0,
		UNDEFINED,
		UNIMPLEMENTED,
		UNKNOWN_OPERATION,
		INVALID_ARGUMENT,
		INSUFFICIENT_RAM,
		ERROR_OPENING_FILE,
		ERROR_WRITING_FILE,
		ERROR_CLOSING_FILE,
		ERROR_READING_FILE,
		INVALID_DATE
	};
}