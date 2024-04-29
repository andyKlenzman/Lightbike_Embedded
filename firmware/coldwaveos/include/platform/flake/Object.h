/**
 * @file FlakeObject.h
 */


#ifndef FLAKEOBJECT_H_
#define FLAKEOBJECT_H_

#include "FlakeConst.h"
#include "FlakeTypes.h"
#include "macro.h"
#include <functional>

using namespace std;

#define TYPE_UUID_SERVICE "00000000-0000-0000-0000-000000000000"

#define FLK_OPEN_RW 1

namespace flake
{
    class Table;

    /**
     * Document me
     */
    class Indication {

     public:

      virtual ~Indication() {};
      ///
      /// \return
      virtual uint8_t
      message () = 0;

      ///
      /// \return
      virtual addr_t
      source () = 0;

      ///
      /// \return
      virtual addr_t
      destination () = 0;

      ///
      /// \return
      virtual PropArray&
      getData () = 0;

      ///
      /// \param result
      virtual void
      ack (int8_t result) = 0;

      ///
      /// \param result
      /// \param va
      virtual void
      ack (int8_t result, PropArray& va) = 0;

      ///
      /// \return
      virtual bool
      acknowledged () = 0;
    };

    typedef uint16_t Token;

    /**
     * Document me
     */
    class Confirmation {

     public:

      virtual ~Confirmation() {};
      ///
      /// \return
      virtual uint8_t
      message () = 0;

      ///
      /// \return
      virtual addr_t
      source () = 0;

      ///
      /// \return
      virtual addr_t
      destination () = 0;

      ///
      /// \return
      virtual PropArray&
      getData () = 0;

      ///
      /// \return
      virtual int8_t
      result () = 0;

      ///
      /// \return
      virtual Token
      token () = 0;

    };

    class IndicationSink {
     protected:
      ~IndicationSink()  =default;
     public:
      ///
      /// \param indication
      virtual void
      onIndication (Indication& indication) = 0;
    };

    class ConfirmationSink {
     protected:
      ~ConfirmationSink()  =default;
     public:
      ///
      /// \param confirmation
      virtual void
      onConfirmation (Confirmation& confirmation) = 0;

    };

    /**
    * Document me
    */
    class ConnectedObject : public IndicationSink, public ConfirmationSink {
     protected:
      ~ConnectedObject()  =default;
     public:
      static const addr_t EMPTY_ADDR;

      ///
      /// \return
      virtual addr_t addr () const = 0;

      ///
      /// \return
      virtual addr_t broadcastAddr () const = 0;

      ///
      virtual void ref () = 0;

      ///
      virtual void unref () = 0;

    };

    class Stream {
     protected:
      ~Stream()   =default;

     public:
      ///
      /// \param data
      /// \param len
      /// \return
      virtual int write (byte *data, unsigned int len) = 0;

      ///
      /// \param data
      /// \param len
      /// \return
      virtual int read (byte **data, unsigned int *len) = 0;

      ///
      virtual void close () = 0;

      ///
      /// \return
      virtual string streamName () = 0;

      ///
      /// \return
      virtual uint16_t streamId () = 0;

    };

    /**
    * Document me
    */
    class Object : public virtual ConnectedObject {
     protected:
      ~Object()   =default;

     public:

      /// invokes a function on the object. functions are implementation specific
      /// and not pre-defined for any device-class.
      /// \param command name of the function to invoke
      /// \param params the functions parameters as ValueArray
      /// \param blocking wait for the return value of the function, can be set to false if the function is "void"
      /// \param conf return value of the function
      /// \return
      virtual int
      invoke (string command, PropArray& params,
              bool blocking, Confirmation **conf) = 0;

      /// broadcasts a message to all subscribers. messages are implementation specific
      /// and not pre-defined for any device-class.
      /// \param command name of the function to invoke
      /// \param params the functions parameters as ValueArray
      /// \return
      virtual int
      broadcast (string message, PropArray& params) = 0;

      ///
      /// \param properties
      /// \return
      virtual int
      getProperties (PropArray& properties) = 0;

      ///
      /// \param properties
      /// \param timeout_ms
      /// \return
      virtual int
      setProperties (PropArray& properties, int timeout_ms) = 0;

      /// document me
      /// \param property
      /// \return
      virtual int
      setProperty (const Property& property) = 0;

      ///
      /// \param property
      virtual void
      getProperty (Property& property) = 0;

#if FLAKE_STREAM_SUPPORT
      ///
      /// \param name
      /// \param stream
      /// \return
      virtual int
      openStream (string name, Stream **stream) = 0;
#endif

      /// Call subscribe to get informed, when the object has updates, e.g. changed properties
      /// \param f callback that gets called whenever anything in the object changes
      /// \return E_OK or an Error value, if the object cannot be subscribed for any reason
      virtual int
      subscribe (std::function<void (Indication&)> f) = 0;

      /// Unsubscribes from the object, effectively removing ALL subscriptions if there are multiple ones
      virtual void
      unsubscribe () = 0;

      /// if this is called before a call to setProperty all subsequent calls are queued
      /// and only sent, when commitUpdate() is called.
      virtual void
      beginUpdate () = 0;

      /// commits all setProperty calls since the last call of beginUpdate()
      /// \return E_OK or an error value, if the call couldn't be executed
      virtual int8_t
      commitUpdate (int timeout_ms = 0) = 0;

      /// commits all setProperty calls since the last call of beginUpdate()
      /// with the ValueArray parameter it returns either the new property values or error messages
      /// for all properties that couldn't be set
      /// \param result Pointer to a ValueArray that will be filled with the properties that have been set
      /// \return E_OK or an error value, if the call couldn't be executed
      virtual int8_t
      commitUpdate (PropArray *result, int timeout_ms = 0) = 0;

      ///
      /// \param property
      virtual void
      getPendingProperty (Property& property) = 0;

      ///
      /// \param properties
      /// \return
      virtual int
      getPendingProperties (PropArray& properties) = 0;

    };

}

#endif /* FLAKEOBJECT_H_ */
