#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <enet/enet.h>

#include "../common/lib.h"

const char ip_addr[] = "127.0.0.1";
const unsigned int port = 7777;

char username[80];

void * msg_loop(ENetHost * client) {
	while (1) {
		ENetEvent event;
		while (enet_host_service(client, &event, 0) > 0) {
			switch (event.type)
			{
				case ENET_EVENT_TYPE_CONNECT:
					printf (
						"A new client connected from %x:%u.\n",
						event.peer -> address.host,
						event.peer -> address.port
					);
					break;

				case ENET_EVENT_TYPE_RECEIVE:
					printf(
						"A packet of length %u containing %s was received from %s on channel %u.\n",
						event.packet->dataLength,
						event.packet->data,
						event.peer->data,
						event.channelID
					);
					/* Clean up the packet now that we're done using it. */
					enet_packet_destroy (event.packet);
					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					printf ("%s disconnected.\n", event.peer->data);
					/* Reset the peer's client information. */
					event.peer->data = NULL;
					break;
			}
		}
	}
}

void send_packet(ENetPeer * server, const char * data) {
	ENetPacket * packet = enet_packet_create(
		data, // the char pointer
		strlen(data) + 1, // reserve null byte
		ENET_PACKET_FLAG_RELIABLE
	);
	enet_peer_send(server, 0, packet); // send it to the server on channel 0.
}

int main(int argc, char ** argv) {
	printf("Please enter your username: ");
	fgets(username, 80 - 1, stdin);

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

	// send the server the User's username!
	char strdata[] = { 0x01, 0x00 };
	strcat(strdata, username);
	send_packet(peer, strdata);

	// initialize the chat screen
	chat_screen_init();

	// create a thread for receiving data
	pthread_t thread;
	pthread_create(&thread, NULL, (void *)msg_loop, client);

	// GAME LOOP BEGIN
	char running = 1;
	while (running) {
		char * str = check_box_input();
		post_message(username, str);
		send_packet(peer, str);
		// memory free hack -- frees the memory created by `post_message` so there are no issues.
		free(str);
	}
	// GAME LOOP END
	pthread_join(thread, NULL);

	// disconnect from peer (server)
	enet_peer_disconnect(peer, 0);

	// do some other server connection stuff - just to double check we have
	// 100% left the server.
	while (enet_host_service(client, &event, 0) > 0) {
		switch (event.type) {
			case ENET_EVENT_TYPE_RECEIVE: {
				enet_packet_destroy(event.packet);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT: {
				printf("Disconnection succeeded.\n");
				break;
			}
			default: {
				break;
			}
		}
	}

	return EXIT_SUCCESS;
}
