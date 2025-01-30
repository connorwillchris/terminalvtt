#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <enet/enet.h>

typedef struct {
	int client_id;
	char username[80 + 1];
} Client;

void send_packet(ENetPeer * client, const char * data) {
	ENetPacket * packet = enet_packet_create(
		data, // the char pointer
		strlen(data) + 1, // reserve null byte
		ENET_PACKET_FLAG_RELIABLE
	);
	enet_peer_send(client, 0, packet); // send it to the server on channel 0.
}

void parse_data(ENetHost * server, int id, char * data) {
	printf("PARSE: ");
	char data_type;
	data_type = data[0];

	switch(data_type) {
		case 0x01: {
			char username[80];
			sscanf(data, "%*c%s", username);
			printf("The user '%s' has just connected!\n", username);
			char send_data[1024] = {0};
			sprintf(send_data, "1|%d|%s", id, username);
			printf("SEND: %s\n", send_data);

			break;
		}
	}
}

int main(int argc, char ** argv) {
	Client * clients = (Client *)malloc(1 * sizeof(Client));
	int clients_malloced_len = 1 * sizeof(Client);

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
		0, 0 // unused...
	);
	if (server == NULL) {
		fprintf(stderr, "An error occured while trying to create an ENET server \
		host!\n");
		return EXIT_FAILURE;
	}

	printf("Server is up!\n");

	int player_id = 0;
	int clients_connected_len = 0;
	// forever loop
	while (1) {
		ENetEvent event;
		while (enet_host_service(server, &event, 1000) > 0) {
			switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT:
					printf(
						"A new client connected from %x:%u!\n",
						event.peer->address.host,
						event.peer->address.port
					);
					clients_connected_len++;

					for (int i = 0; i < clients_connected_len; i++) {
						char send_data[1024] = {0};
						sprintf(send_data, "2|%d|%s", clients[i].client_id, clients[i].username);
						// broadcast this packet
					}
					player_id++;

					break;
				
				case ENET_EVENT_TYPE_RECEIVE:
					/*
					printf(
						"A packet of length %zu containing '%s' was received from %x:%u \
						on channel %u.\n",
						event.packet->dataLength,
						event.packet->data,
						event.peer->address.host,
						event.peer->address.port,
						event.channelID
					);
					*/

					parse_data(server, -1, event.packet->data);
					enet_packet_destroy(event.packet);
					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					printf("%x:%u disconnected.\n", event.peer->address.host,
					event.peer->address.port);
					event.peer->data = NULL;
					break;
				
				default: {
					break;
				}
			}
		}
	}

	enet_host_destroy(server);
	free(clients);

	return EXIT_SUCCESS;
}
