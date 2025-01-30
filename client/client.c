#include <stdio.h>
#include <stdlib.h>
#include <enet/enet.h>

int main(int argc, char ** argv) {
	// initialize enet
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occurred while initializing ENET!\n");
		return EXIT_FAILURE;
	}
	// deinit at exit
	atexit(enet_deinitialize);

	// create and initialize a client
	ENetHost * client;
	client = enet_host_create(NULL, 1, 1, 0, 0);
	if (client == NULL) {
		fprintf(stderr, "An error occurred while trying to create an \
		ENET client host!\n");
		return EXIT_FAILURE;
	}
	
	ENetAddress address;
	ENetEvent event;
	ENetPeer * peer;

	// set address and ip of host
	enet_address_set_host(&address, "127.0.0.1");
	address.port = 7777;
	peer = enet_host_connect(client, &address, 1, 0);
	if (peer == NULL) {
		fprintf(stderr, "No available peers for initializing an ENET connection!\n");
		return EXIT_FAILURE;
	}

	if (enet_host_service(client, &event, 5000) // check if it's been 5 seconds
	> 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		printf(
			"Connection to %s:%s succeeded!\n",
			"127.0.0.1", "7777"
		);
	}
	else {
		enet_peer_reset(peer);
		printf(
			"Connection to %s:%s failed!\n",
			"127.0.0.1", "7777"
		);
		return EXIT_SUCCESS;
	}

	while (enet_host_service(client, &event, 1000) > 0) {
		switch (event.type) {
			case ENET_EVENT_TYPE_RECEIVE:
				printf(
					"A packet of length %zu containing %s was received from %x:%u on \
					channel %u!\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->address.host,
					event.peer->address.port,
					event.channelID
				);
				break;
			default: {
				break;
			}
		}
	}

	// GAME LOOP BEGIN
	while (1) {
		while (enet_host_service(client, &event, 3000) > 0) {
			switch (event.type) {
				case ENET_EVENT_TYPE_RECEIVE:
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					printf("Disconnection succeeded!\n");
					break;
				default: {
					break;
				}
			}
		}
	}
	// GAME LOOP END

	// disconnect from peer (server)
	enet_peer_disconnect(peer, 0);

	return EXIT_SUCCESS;
}
