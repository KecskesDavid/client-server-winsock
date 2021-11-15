#pragma once

enum class RequestType {
	CREATE_SOCKET = 0,
	CREATE_GROUP = 1,
	SEND_PUBLIC = 3,
	SEND_GROUP = 4,
	SEND_PRIVATE = 5
};