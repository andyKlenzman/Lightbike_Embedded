/*
 * Service.h
 *
 *  Created on: 13.12.2010
 *      Author: Sebastian
 */

#ifndef SERVICE_H_
#define SERVICE_H_


#include "FlakeTypes.h"
#include "Object.h"
#include "macro.h"
#include <string>
#include <map>

using namespace std;


namespace flake {

    /**
    * The alias on_property_cb is the alias name, it is a pointer to a function that takes one parameter and returns an int
     *
     *
    */
    typedef int (*on_property_cb)(const Property& prop);

    class Connection;

    class Service  {


     protected:

      void *hObj;


      Connection* connection();

      std::map<uint32_t, std::function<int (Property&, const PropArray&, bool)>> p_callbacks;
      std::map<std::string, std::function<int (PropArray&, PropArray&)>> m_callbacks;

     public:
      void* handle();

      Service();
      virtual ~Service();


      ///
      /// \return
      virtual TagArray defaultPropset();

      virtual void onInitialized();

      virtual bool isInitialized();

      virtual int
      setPropertyRequested(uint32_t tag, Property& value, const PropArray& transaction, bool internal);

      virtual int
      getPropertyRequested(uint32_t tag, Property & value);

      virtual int
      broadcast (const string command, PropArray& params);

      void
      beginUpdate();

      virtual int8_t
      commitUpdate(int timeout_ms = 0);

      virtual int
      setProperties(PropArray &properties, int timeout_ms);

      virtual int
      set(Property& property);

      virtual int
      set(const Property& property);


      virtual Property
      get(uint32_t tag);

      virtual int
      on(uint32_t tag, std::function<int ( Property&, const PropArray&, bool)> cb);

      virtual int
      onMessage (string message, std::function<int (PropArray&, PropArray&)> cb);

      ///
      const uniqueId_t DLLCALL id();

      ///
      /// \param message
      /// \param params
      /// \return
      virtual int8_t DLLCALL handleCustomMessage(string message, PropArray& params, PropArray& outProps);

      ///
      /// \param stream
      /// \param data
      /// \param len
      /// \return
      virtual int8_t DLLCALL handleStreamData(Stream* stream, byte* data, int len);
      
    };

}




#endif /* SERVICE_H_ */