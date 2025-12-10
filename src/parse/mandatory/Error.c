
#include <fcntl.h>
#include <unistd.h>
#include "parser_internal.h"

char	*get_error_msg(int code)
{
	static char *errors[] = {
		[OK] = "Success",
		[ERR_ARGS] = "Invalid arguments",
		[ERR_EXTENSION] = "File must have .cub extension",
		[ERR_OPEN] = "Cannot open file",
		[ERR_MALLOC] = "Memory allocation failed",
		[ERR_DUPLICATE] = "Duplicate configuration",
		[ERR_MISSING_CONFIG] = "Missing required configuration",
		[ERR_INVALID_TEXTURE] = "Invalid texture file",
		[ERR_INVALID_COLOR] = "Invalid color format (R,G,B 0-255)",
		[ERR_NO_MAP] = "Map not found",
		[ERR_INVALID_CHAR] = "Invalid character in map",
		[ERR_INVALID_CHARACTHER] = "Invalid character",
		[ERR_NO_PLAYER] = "No player spawn point",
		[ERR_MULTI_PLAYER] = "Multiple player spawn points",
		[ERR_MAP_NOT_CLOSED] = "Map is not closed by walls",
		[20] = "Invalid door placement (must be between walls and adjacent to walkable space)",
		[21] = "Invalid sprite placement",
		[22] = "Door texture not found or invalid",
		[23] = "Sprite texture not found or invalid",
		[24] = "Bonus configuration error"
	};
	if (code < 0 || (size_t)code >= sizeof(errors) / sizeof(errors[0]))
		return ("Unknown error");
	return (errors[code]);
}
