#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include "application.h"
#include "chatmessage.h"

#include <unistd.h>

#include <zmq.hpp>
using namespace zmq;

#include <ctime>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
using namespace std;

/*
 * Chat Client for messaging
 *
 * Uses ChatMessage class objects to send/receive messages over ZeroMQ sockets
 * to the Chat Server
 */
class ChatClient : public Application {

  CLASS_WITH_NAME;

  int clientId;       // unique Id of the Client
  string clientAlias; // may be not unique

  /*
   * Network Interfaces
   */
  string serverAddress; // IPv4 address of the Server
  bool initSocket(context_t &context, int type);
  bool connectSocket(socket_t &socket, int portNumber);

  int connectionPort;                 // to talk with Server
  unique_ptr<socket_t> socketRequest; //

  int messagingPort; // to get other Clients messages
  unique_ptr<socket_t> socketSubscribe;

  string tcpServerPortAddress(int portNumber);
  const int timeout = TIMEOUT; // ms reaction time for interact with server

  /*
   * Main functions runs in different thread
   */
  void send();    // connect/send message to the Server
  void receive(); // and get messages from other Clients

  // different interruption codes to process
  enum Interrupt { NOPE = 0, USER_QUIT = 1, CHANGE_PORTS = 2 };

  int interruptCode;    // follow interruption code
  mutex interruptMutex; // lock from different threads
  void setInterrupt(int code);
  bool notInterrupted(); // check

  void invite();  // User input
  string clear(); // cout clear

  void setConnectionPort(int connectionPort); // should reconnect Server ITF
  void setMessagingPort(int messagingPort);   // etc, make public ITF

  void setServerAddress(string serverAddress); // IPv4 x.x.x.x

public:
  ChatClient(int connectionPort = CONNECTION_PORT,
             int messagingPort = MESSAGING_PORT,
             string serverAddress = LOCALHOST, string clientAlias = "anon");

  void setClientAlias(string clientAlias); // name which will see other Client

  void run() override; // start up the Client
};

#endif // CHATCLIENT_H
