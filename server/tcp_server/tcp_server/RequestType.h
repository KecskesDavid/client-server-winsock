#pragma once

enum class RequestType {
	CREATE_SOCKET = 0,
	CREATE_GROUP = 1,
	SEND_PUBLIC = 2,
	SEND_GROUP = 3,
	SEND_PRIVATE = 4,
	ADD_TO_GROUP = 5,
};