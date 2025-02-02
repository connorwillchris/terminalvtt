#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <enet/enet.h>

#include "../common/ansi.h"

const int port = 7777;
const int max_clients = 32;

typedef struct {
	unsigned int id;
	char username[80];
} Client;

void log_message(const char * color, const char * message) {
	time_t now;
	time(&now);
	struct tm * local = localtime(&now);
	char time_buffer[80];
	strftime(time_buffer, 80, "%Y-%m-%d %H:%M:%S", local);
	printf("%s%s[%s] %s%s\n", color, BOLD, time_buffer, CLEAR_ALL, message);
}

void send_packet(ENetPeer * client, const char * data) {
	ENetPacket * packet = enet_packet_create(
		data, // the char pointer
		strlen(data) + 1, // reserve null byte
		ENET_PACKET_FLAG_RELIABLE
	);
	enet_peer_send(client, 0, packet); // send it to the server on channel 0.
}

void parse_data(ENetHost * server, int id, char * data) {
	// data and how to parse it...
	char data_type;
	data_type = data[0];

	switch(data_type) {
		case 0x01: {
			char username[80];
			// get username
			sscanf(data, "%*c%s", username);
			char send_data[1024] = { 0 };
			sprintf(send_data, "The user '%s' has logged in.", username);
			log_message(FG_RED, send_data);
			break;
		}
		case 0x02: {
			char username[80];
			sscanf("%*c%[^\n]", username);
			char send_data[1024] = { 0 };
			sprintf(send_data, "The user %s has logged in.", username);
			log_message(FG_RED, send_data);
			break;
		}
		default: break;
	}
}

int main(int argc, char ** argv) {
	Client * clients = (Client *)malloc(max_clients * sizeof(Client));
	int logged_in_players = 0;

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
	address.port = port;
	server = enet_host_create(
		&address,
		max_clients, // peers that are allowed to connect.
		1,
		0, 0 // unused...
	);
	if (server == NULL) {
		fprintf(stderr, "An error occured while trying to create an ENET server \
host!\n");
		return EXIT_FAILURE;
	}

	// log the first message to the console.
	log_message(FG_RED, "Server initialized.");

	int player_id = 0;
	int clients_connected_len = 0;

	// forever loop
	while (1) {
		ENetEvent event;
		while (enet_host_service(server, &event, 0) > 0) {
			switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT: {
					char buffer[256] = {0};
					sprintf(
						buffer,
						"A new client connected from %x:%u!\n",
						event.peer->address.host,
						event.peer->address.port
					);
					log_message(FG_BLUE, buffer);

					logged_in_players++;
					for (int i = 0; i < logged_in_players; i++) {
						
					}
					player_id++;
					Client c = {
						.id = player_id,
						.username = { 0 },
					};
					clients[player_id] = c;
					event.peer->data = clients[player_id].username;
					char data_to_send[128 + 1] = {0};
					sprintf(data_to_send, "\x02", player_id);
					send_packet(event.peer, data_to_send);

					break;
				}
				
				case ENET_EVENT_TYPE_RECEIVE:
					printf(
						"A packet of length %zu containing '%s' was received from \
%x:%u on channel %u.\n",
						event.packet->dataLength,
						event.packet->data,
						event.peer->address.host,
						event.peer->address.port,
						event.channelID
					);
					parse_data(server, -1, (char *)event.packet->data);
					enet_packet_destroy(event.packet);
					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					printf("%x:%u disconnected.\n",
						event.peer->address.host,
						event.peer->address.port
					);
					event.peer->data = NULL;
					break;
				
				default: break;
			}
		}
	}

	free(clients);
	// finally, deinitialize everything.
	enet_host_destroy(server);

	return EXIT_SUCCESS;
}
