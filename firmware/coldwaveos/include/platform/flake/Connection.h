/**
 * @file Connection.h
 * @version Alpha
 * @author IMAGINEON
 * @date 11/3/2010
 * @since Alpha
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "Service.h"
#include "macro.h"
#include "Object.h"
#include "Wire.h"
#include "ServerWire.h"

using namespace std;

#define FLAKE_DEFAULT_PORT 9986


namespace flake {


    struct ConnectionSink {
     protected:
      ~ConnectionSink() =default;

     public:
        ///
        virtual void onDisconnected() = 0;
    };

    struct AuthenticationSink {
     protected:
      ~AuthenticationSink() =default;

     public:
        /// called if we need to authenticate (Client and Router)
        virtual void onAuthChallengeReceived(char* challenge_type, byte* challenge,
                                                int challenge_len, byte** response, int *response_len) = 0;

        /// called on a new connection on a wire  (Router only)
        /// return 0 if challenge is present, -1 if no auth wanted/needed
        virtual int onAuthChallengeRequested (void* wire, char** callenge_type,
                                               byte** challenge, int *challenge_len) = 0;

        /// called if we asked for challenge auth on a wire. Returns 0 if auth is ok (Router only)
        /// the previously allocated challenge_type and challenge should be freed here.
        virtual int onAuthResponseReceived (char* challenge_type, byte* challenge,
                                            int challenge_len, byte* response, int response_len) = 0;

        /// called if authentication type is 'atInteractive'. Return 0 if auth is accepted (Router only)
        virtual int onConnect(const PropArray& props) = 0;

        /// return the desired authentication type
        virtual flakeAuthType authenticationType() = 0;

    };

    /**
    * Document me
    */
    class Connection : public virtual ConnectedObject {
     protected:
      ~Connection() =default;

    public:

        /// Establish a connection to the router
        /// \return E_OK if the connection has been established or an error if it failed

        virtual int  connect(AuthenticationSink* authentication_sink) = 0;

        virtual int  connect(PropArray& props) = 0;

        /// Disconnect from the router
        virtual void  disconnect() = 0;

        virtual bool  connected() = 0;

        virtual int8_t  registerService(Service* srv, uniqueId_t& uuid, bool requires_auth = false) = 0;

        virtual int8_t  createObject(uniqueId_t& objectType, PropArray& properties, Object** object) = 0;

        /// Queries the router for exisiting service-objects.
        /// it is important to call 'unref()' on any returned object, that the caller doesn't want to use
        /// \param type the type of the service (the service-class)
        /// \param numObjects number of found objects
        /// \param objects array of objects
        virtual void  queryObjects(uniqueId_t type, int* numObjects, Object** objects[]) = 0;

    };

    typedef Connection Device;

}

#if defined (__cplusplus)
extern "C" {
#endif

/// First function to call to initialize the physical layer connection to the router
/// \param wire the type of physical connection (at the moment TCP/LocalSocket/Serial)
/// \param sink a sink pointer to a connection sink for information about connect/disconnect events
/// \param conn pointer to the logical flake-connection to access the system
/// \return
DLLEXPORT int flakeInitialize(flake::Wire *wire, flake::ConnectionSink *sink, flake::Connection **conn);

/// First function to call to initialize the physical layer with a locally running router
/// \param sink a sink pointer to a connection sink for information about connect/disconnect events
/// \param conn pointer to the logical flake-connection to access the system
/// \return
DLLEXPORT int flakeInitializeWithRouter(flake::ConnectionSink *sink, flake::Connection **conn);

/// First function to call to initialize the physical layer with a locally running router
/// \param conn_sink a sink pointer to a connection sink for information about connect/disconnect events
/// \param autho_sink a sink pointer to an auhtentiaction sink for authenticating ourselves against peers
/// \param conn pointer to the logical flake-connection to access the system
/// \return
DLLEXPORT int flakeInitializeWithRouterAndAuth(flake::ConnectionSink *conn_sink, flake::AuthenticationSink *auth_sink,
                                            flake::Connection **conn);

/// First function to call to initialize if the system should run as Router only
/// \return
DLLEXPORT int flakeInitializeRouter();

/// First function to call to initialize if the system should run as Router only with client authentication
/// \return
DLLEXPORT int flakeInitializeRouterWithAuth(flake::AuthenticationSink *auth_sink);

/// Releases any Memory used by the flake library, call this before your program's exit.
/// \param c The initally via the initialize call retrieved Connection
DLLEXPORT void flakeUninitialize(flake::Connection *c);

/// Releases any Memory used by the flake library, call this before your program's exit.
/// \param c The initally via the initialize call retrieved Connection, if any. can be null.
DLLEXPORT void flakeUninitializeRouter (flake::Connection *c);

/// Add a Wire to the Router
DLLEXPORT int flakeRouterAddServerWire(flake::ServerWire *w);

/// Convenience Function for TCP Wires
DLLEXPORT int flakeRouterAddTcpServerWire(int port, int use_tls);

/// Convenience Function for UDP Wires
DLLEXPORT int flakeRouterAddUdpServerWire(int port);

#ifdef __CXOS__
/// Convenience Function for BLE Wire
DLLEXPORT int flakeRouterAddBLEServerWire(int hdev);
#endif

typedef struct flakeSerialWireCallbacks_t {
    void (*onConnect)(void);
    void (*onDisconnect)(void);
} flakeSerialWireCallbacks_t;

/// Convenience Function for UART/Serial Wires
DLLEXPORT int flakeRouterAddSerialServerWire (int hdev, flakeSerialWireCallbacks_t* cb);

/// Convenience Function for Reversed TCP Wires, used for Routers behind firewalls
DLLEXPORT int flakeRouterAddReversedTcpServerWire(const char *ip, int port, bool use_tls,
                                                  unsigned char* x509_ca_cert, unsigned int x509_ca_cert_len);

#if FLAKE_TLS

///
/// \param ip
/// \param port
/// \param x509_ca_cert
/// \param x509_ca_cert_len
/// \return
DLLEXPORT flake::Wire *flakeCreateTLSWire(string ip, int port,unsigned char* x509_ca_cert, unsigned int x509_ca_cert_len);

///
/// \param ip
/// \param port
/// \param x509_ca_cert
/// \param x509_ca_cert_len
/// \return
DLLEXPORT flake::Wire *flakeCreateDTLSWire(string ip, int port,unsigned char* x509_ca_cert, unsigned int x509_ca_cert_len);
#endif

///
/// \param ip
/// \param port
/// \return
DLLEXPORT flake::Wire *flakeCreateTcpWire(string ip, int port);

///
/// \param ip
/// \param port
/// \return
DLLEXPORT flake::Wire *flakeCreateUdpWire(string ip, int port);

///
/// \return
DLLEXPORT const char *flakeFindRouter(int timeout_s);


#if defined (__cplusplus)
}
#endif

#endif /* CONNECTION_H_ */

