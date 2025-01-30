#include <stdio.h>
#include <stdlib.h>
#include <enet/enet.h>

int main(int argc, char ** argv) {
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occured while initializing ENET!\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetAddress address;
	ENetHost * server;

	/*
	 * Bind the server to the default localhost.
	 * A specified host address can be specified by
	 * enet_address_set_host (&address, "x.x.x.x");
	 */
	address.host = ENET_HOST_ANY;
	// bind the server to port 7777
	address.port = 7777;

	server = enet_host_create(
		&address,
		32, // peers that are allowed to connect.
		1,
		0,
		0
	);
	if (server == NULL) {
		fprintf(stderr, "An error occured while trying to create an ENET server \
		host!\n");
		return EXIT_FAILURE;
	}

	// forever loop
	while (1) {
		ENetEvent event;
		while (enet_host_service(server, &event, 1000) >0) {
			switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT:
					printf(
						"A new client connected from %x:%u!\n",
						event.peer->address.host,
						event.peer->address.port
					);
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					printf(
						"A packet of length %zu containing %s was received from %s \
						on channel %u.\n",
						event.packet->dataLength,
						event.packet->data,
						event.peer->data,
						event.channelID
					);
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					printf(
						"%s disconnected.\n",
						event.peer->data
					);
					event.peer->data = NULL;
					break;
				default: {
					break;
				}
			}
		}
	}

	enet_host_destroy(server);

	return EXIT_SUCCESS;
}
