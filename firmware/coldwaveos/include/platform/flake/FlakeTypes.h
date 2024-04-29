/** @file FlakeTypes.h
 * @version Alpha
 * @author IMAGINEON
 * @date 11/3/2010
 * @since Alpha
 */


#ifndef FLAKETYPES_H_
#define FLAKETYPES_H_

#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include "util.h"

#ifdef _WIN32
#include <stdint.h>
#endif

#if __WORDSIZE == 64
#define UINT32_FMT "u"
#define INT8_FMT "d"
#define INT16_FMT "d"
#define INT32_FMT "d"
#define UINT32_HEX_FMT "X"
#else
#define UINT32_FMT "lu"
#define INT8_FMT "d"
#define INT16_FMT "d"
#define INT32_FMT "ld"
#define UINT32_HEX_FMT "lX"
#endif

#include "FlakeConst.h"
#include "macro.h"

#define DEALLOC_SELF_FLAG   (1)
#define DONT_SERIALIZE_FLAG (2)
#define DONT_COPY_FLAG      (4)
#define DONT_CACHE_FLAG     (8)

/// document me
#define TAG_TYPE(x) ((x) & 0xfff)
/// document me
#define TAG_ID(x) ((x) >> 16)
/// document me
#define TAG_READONLY    (0x1000)
/// document me
#define TAG_ACTIONABLE  (0x2000)
/// document me
#define TAG_VOLATILE    (0x4000)
/// document me
#define TAG_ERROR       (0x0100)
/// document me
#define TAG_ARRAY       (0x0200)
/// document me
#define IS_ACTIONABLE(x) ((x & TAG_ACTIONABLE) == TAG_ACTIONABLE)
/// document me
#define IS_READONLY(x) ((x & TAG_READONLY) == TAG_READONLY)
/// document me
#define IS_BASE_PROPERTY(x) (x >> 16 < 0x200)
///document me
#define IS_ERROR(x) (((x) & TAG_ERROR) == TAG_ERROR)
/// document me
#define IS_VOLATILE(x) ((x & TAG_VOLATILE) == TAG_VOLATILE)
///document me
#define IS_CREATEONLY(x) ((x & TAG_CREATE_ONLY) == TAG_CREATE_ONLY)
///document me
#define IS_ARRAY(x) ((x & TAG_ARRAY) == TAG_ARRAY)
/// document me
#define TT_INT32    (0x001)
/// document me
#define TT_INT16    (0x002)
/// document me
#define TT_INT8     (0x003)
/// document me
#define TT_UINT32    (0x004)
/// document me
#define TT_UINT16    (0x005)
/// document me
#define TT_UINT8    (0x006)
/// document me
#define TT_BOOL    (0x007)
/// document me
#define TT_UUID     (0x008)
/// document me
#define TT_FLOAT    (0x009)
/// document me
#define TT_DATETIME (0x00A)
/// document me
#define TT_BIN      (0x00C)
/// document me
#define TT_STRING   (0x00E)
/// document me
#define ERROR_VAL(x) (((x).tag & TAG_ERROR) == TAG_ERROR)

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

using namespace std;

namespace flake
{

    typedef unsigned char byte;

    class push_ {
     public:

      uint16_t *len;
      byte **buf;

      push_ (uint16_t *len_, byte **buf_)
      {
        this->len = len_;
        this->buf = buf_;
      }

      void
      at (uint16_t offset, const void *value, uint16_t size)
      {
        if (offset + size <= *len)
          memcpy (*buf + offset, value, size);
      }

      uint16_t
      operator() (const void *value, uint16_t size, bool pushSize = true)
      {
        uint16_t offset = *len;

        if (pushSize)
          {
            // *buf = (byte*) realloc(*buf, *len + size + sizeof(uint16_t));
            byte *newBuf = static_cast<byte *>( malloc (*len + size + sizeof (uint16_t)));
            if (*buf != nullptr)
              {
                memcpy (newBuf, *buf, *len);
                free (*buf);
              }
            *buf = newBuf;
            memcpy (*buf + *len, &size, sizeof (uint16_t));
            memcpy (*buf + *len + sizeof (uint16_t), value, size);
            *len = (uint16_t) (*len + size + sizeof (uint16_t));
          }
        else
          {
            //*buf = (byte*) realloc(*buf, *len + size);
            byte *newBuf = (byte *) malloc (*len + size);
            if (*buf != nullptr)
              {
                memcpy (newBuf, *buf, *len);
                free (*buf);
              }
            *buf = newBuf;
            memcpy (*buf + *len, value, size);
            *len = (uint16_t) (*len + size);
          }

        return offset;
      }

    };

    class pop_ {
     public:

      uint16_t *c;
      byte **buf;

      pop_ (byte **buf_, uint16_t *c_)
      {
        this->buf = buf_;
        this->c = c_;
      }

      void
      seek (uint16_t offset)
      {
        *c = offset;
      }

      void
      operator() (void *val, size_t size)
      {
        memcpy (val, *buf + *c, size);
        *c = (uint16_t) (*c + size);
      }
    };

    struct Serializable {
     protected:
      ~Serializable () = default;
     public:
      virtual bool
      serialize (uint16_t *len, flake::byte **buf) = 0;
      virtual bool
      unserialize (uint16_t len, flake::byte *buf) = 0;
    };

    struct UniqueId {
        UniqueId ()
        {
          set_nil ();
        }

        inline void
        set_nil ()
        {
          memset (data, 0, 16);
        }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
        inline char *
        toString () const
        {
#if FLAKE_DEBUG_LOGGING
          char *out = new char[37];

          sprintf (
              out,
              "%02x%02x%02x%02x-"
              "%02x%02x-"
              "%02x%02x-"
              "%02x%02x-"
              "%02x%02x%02x%02x%02x%02x",
              data[0], data[1], data[2], data[3], data[4], data[5], data[6],
              data[7], data[8], data[9], data[10], data[11], data[12], data[13],
              data[14], data[15]);

          return out;
#else
          return 0;
#endif
        }

        inline void initFromString (const char *in)
        {

          if (strlen (in) != 36)
            {
              return;
            }

          if ((in[8] != '-') || (in[13] != '-') || (in[18] != '-') || (in[23] != '-'))
            {
              return;
            }

          memset (data, 0, sizeof (data));

          size_t index = 0;
          size_t i = 0;
          while (index < 36)
            {
              char c = in[index];
              int value;
              if (c >= '0' && c <= '9')
                value = (c - '0');
              else if (c >= 'A' && c <= 'F')
                value = (10 + (c - 'A'));
              else if (c >= 'a' && c <= 'f')
                value = (10 + (c - 'a'));
              else
                {
                  index++;
                  continue;
                }
              data[(i / 2)] += value << (((i + 1) % 2) * 4);
              i++;
              index++;
            }
        }
#pragma GCC diagnostic pop

        inline bool
        is_nil () const
        {

          for (size_t i = 0; i < 16; i++)
            {
              if (data[i] != 0U)
                {
                  return false;
                }
            }
          return true;
        }

        inline bool
        operator== (UniqueId const &rhs) const
        {
          return (memcmp (&data, &rhs, sizeof (UniqueId)) == 0);
        }

        inline bool
        operator!= (UniqueId const &rhs) const
        {
          return !operator== (rhs);
        }

        inline bool
        operator< (UniqueId const &rhs) const
        {
          return (memcmp (&data, &rhs.data, 16 * sizeof (uint8_t)) < 0);
        }

        void
        operator= (UniqueId const &rhs)
        {
          memcpy (&data, &rhs.data, 16 * sizeof (uint8_t));
        }

        uint8_t data[16];

    };

    typedef UniqueId uniqueId_t;

/// document me
    typedef uint16_t addr_t;

    typedef struct {
        uint16_t cb;
        byte *lpb;
    } binary_t;

/// document me
    typedef int32_t tt_int32_t;
/// document me
    typedef int16_t tt_int16_t;
/// document me
    typedef int8_t tt_int8_t;
/// document me
    typedef uint32_t tt_uint32_t;
/// document me
    typedef uint16_t tt_uint16_t;
/// document me
    typedef uint8_t tt_uint8_t;
/// document me
    typedef bool tt_bool_t;
/// document me
    typedef int8_t tt_err_t;
/// document me
    typedef binary_t tt_bin_t;
/// document me
    typedef uniqueId_t tt_uuid_t;
/// document me
    typedef char *tt_str_t;
/// document me
    typedef uint32_t tt_time_t;
/// document me
    typedef float tt_float_t;

    typedef union {

        tt_int8_t i8;

        tt_int16_t i16;

        tt_int32_t i32;

        tt_uint8_t u8;

        tt_uint16_t u16;

        tt_uint32_t u32;

        tt_bool_t b;

        tt_float_t f;

        tt_uuid_t uuid;

        tt_time_t t;

        tt_err_t err;

        // variable length / alloc'd types

        tt_bin_t bin;

        tt_str_t str;

    } value_t;

    typedef struct array_t {

        uint16_t numValues;
        uint16_t valueType;
        uint16_t cValueSize;
        byte *lpValues;

        inline uint16_t
        count () const
        {
          return numValues;
        }

        inline const value_t *
        operator[] (uint16_t __n)
        {
          return (value_t *) (lpValues + (__n * cValueSize));
        }
    } array_t;

/// document me
    typedef array_t tt_array_t;

    namespace BaseProperties
    {

/// document me
        const uint32_t OBJECT_TYPE = 0x0001 << 16 | (TT_UUID | TAG_READONLY);
/// document me
        const uint32_t OBJECT_ADDR = 0x0002 << 16 | (TT_UINT16 | TAG_READONLY);
/// document me
        const uint32_t BROADCAST_ADDR = 0x0003 << 16 | (TT_UINT16 | TAG_READONLY);
/// document me
        const uint32_t MESSAGE_NAME = 0x0004 << 16 | TT_STRING;
/// document me
        const uint32_t CHILDREN_TABLE = 0x0005 << 16 | TT_BIN;
/// document me
        const uint32_t COLUMN_SET = 0x0006 << 16 | TT_UINT16 | TAG_ARRAY;
/// document me
        const uint32_t LAST_MODIFICATION_TIME = 0x0007 << 16 | TT_DATETIME;
/// document me
        const uint32_t CREATION_TIME = 0x0008 << 16 | TT_DATETIME;
/// document me
        const uint32_t OBJECT_TABLE = 0x000D << 16 | TT_BIN;
/// document me
        const uint32_t PROP_TAG = 0x000F << 16 | TT_UINT32;
/// document me
        const uint32_t PROP_NAME = 0x0010 << 16 | TT_STRING;
/// document me
        const uint32_t PROP_TYPE = 0x0011 << 16 | TT_UINT16;
/// document me
        const uint32_t PROP_MAPPINGS = 0x0012 << 16 | (TT_BIN | TAG_ARRAY | TAG_READONLY);
/// document me
        const uint32_t REQUIRES_AUTH = 0x0018 << 16 | (TT_BOOL);
/// document me
        const uint32_t AUTH_TYPE = 0x0031 << 16 | (TT_UINT8 | TAG_READONLY);
/// document me
        const uint32_t AUTH_USER = 0x0032 << 16 | (TT_STRING | TAG_READONLY);
/// document me
        const uint32_t AUTH_PASS = 0x0033 << 16 | (TT_STRING | TAG_READONLY);
/// document me
        const uint32_t SIGN_ALGO = 0x0034 << 16 | (TT_STRING | TAG_READONLY);
/// document me
        const uint32_t SIGN_HASH = 0x0035 << 16 | (TT_BIN | TAG_READONLY);
/// document me
        const uint32_t SIGNATURE = 0x0036 << 16 | (TT_BIN | TAG_READONLY);
        /// document me
        const uint32_t OBJECT_UUID = 0x0100 << 16 | (TT_UUID | TAG_READONLY);
/// document me
        const uint32_t OBJECT_TYPE_NAME = 0x101 << 16 | (TT_STRING | TAG_READONLY);

    }

    typedef enum flakeAuthType {
        atSignature = 1,
        atInteractive = 2
    } flakeAuthType;

/**
   * document me
   */
    struct TagArray {
        inline
        TagArray (const TagArray &ta)
        {
          numTags = ta.numTags;
          if (ta.numTags > 0)
            {
              tags = new uint32_t[numTags];
              for (uint32_t i = 0; i < ta.numTags; i++)
                tags[i] = ta.tags[i];
            }
        }
        inline
        TagArray ()
        {
          tags = 0;
          numTags = 0;
        }

        ~TagArray ()
        {
          if (numTags > 0)
            delete[] tags;
        }

        inline
        TagArray (uint32_t count, ...)
        {
          numTags = count;
          if (count > 0)
            {
              va_list _tags;
              va_start(_tags, count);
              tags = new uint32_t[count];
              for (uint32_t i = 0; i < count; i++)
                tags[i] = va_arg(_tags, uint32_t);
              va_end(_tags);
            }
        }

        inline TagArray &
        operator= (const TagArray &other)
        {
          numTags = other.numTags;
          if (other.numTags > 0)
            {
              tags = new uint32_t[numTags];
              for (uint32_t i = 0; i < other.numTags; i++)
                tags[i] = other.tags[i];
            }
          return *this;
        }

        mutable uint32_t numTags;
        mutable uint32_t *tags;

    };

/**
   * document me
   */

    extern "C"
    {

    struct Property {
        /// document me
        mutable uint32_t tag;
        ///
        byte flags;
        ///
#ifndef EXCLUDE_FROM_DOCS
        union {
#endif

            tt_int8_t i8;

            tt_int16_t i16;

            tt_int32_t i32;

            tt_uint8_t u8;

            tt_uint16_t u16;

            tt_uint32_t u32;

            tt_bool_t b;

            tt_float_t f;

            tt_uuid_t uuid;

            tt_time_t t;

            tt_err_t err;

            // variable length / alloc'd types

            tt_bin_t bin;

            tt_str_t str;

            tt_array_t arr;
#ifndef EXCLUDE_FROM_DOCS
        };
#endif
#if FLAKE_NAMED_TAG_SUPPORT
        mutable string name;

        inline
        Property(string _name, uint16_t _type) :
                tag(_type), name(_name)
        {
            flags = 0;
            memset(&uuid, 0, sizeof(uuid));
        }

#endif

        inline
        Property (uint32_t _tag)
            :
            tag (_tag)
        {
          flags = 0;
          memset (&uuid.data, 0, sizeof (uuid));
        }
        inline
        Property ()
            :
            tag (0)
        {
          flags = 0;
          memset (&uuid.data, 0, sizeof (uuid));
        }

        inline
        Property (const Property &v)
        {
          flags = 0;
          copyFrom (v);
        }

        inline void
        makeReadOnly () const
        {
          tag |= TAG_READONLY;
        }

        inline
        ~Property ()
        {
          if ((flags & DEALLOC_SELF_FLAG) == DEALLOC_SELF_FLAG)
            {
              if ((tag & TAG_ARRAY) == TAG_ARRAY)
                {
                  if (arr.numValues * arr.cValueSize > 0)
                    free (arr.lpValues);
                }
              else
                {
                  switch (TAG_TYPE(tag))
                    {
                      case TT_STRING:
                        if (str != nullptr)
                          {
                            free (str);
                          }
                      break;
                      case TT_BIN:
                        if (bin.cb > 0)
                          {
                            free (bin.lpb);
                          }
                      break;
                      default:
                        break;
                    }
                }
            }
        }

        inline bool
        operator!= (const Property &other)
        {
          return !operator== (other);
        }

        inline void
        __setDeallocSelf ()
        {
          flags |= DEALLOC_SELF_FLAG;
        }

        inline bool
        operator== (const Property &other)
        {

          if (tag != other.tag)
            return false;

          if (IS_ERROR(tag))
            {
              return err == other.err;
            }

          if ((tag & TAG_ARRAY) == TAG_ARRAY)
            {
              if ((arr.numValues == other.arr.numValues) && (arr.cValueSize == other.arr.cValueSize))
                return memcmp (arr.lpValues, other.arr.lpValues, arr.numValues * arr.cValueSize) == 0;
              return false;
            }

          switch (TAG_TYPE(tag))
            {
              case TT_FLOAT:
                return abs (f - other.f) < 0.000001;

              case TT_UINT8:
                return u8 == other.u8;

              case TT_UINT16:
                return u16 == other.u16;

              case TT_UINT32:
                return u32 == other.u32;

              case TT_INT8:
                return i8 == other.i8;

              case TT_INT16:
                return i16 == other.i16;

              case TT_INT32:
                return i32 == other.i32;

              case TT_BOOL:
                return b == other.b;

              case TT_DATETIME:
                return (t == other.t);

              case TT_UUID:
                return (memcmp (&uuid, &other.uuid, 16));

              case TT_STRING:
                if ((str != nullptr && other.str != nullptr) && (strlen (str)
                                                                 == strlen (other.str)))
                  {
                    return strncmp (str, other.str, strlen (str)) == 0;
                  }
                else if (str == nullptr && other.str == nullptr)
                  {
                    return true;
                  }
              break;
              case TT_BIN:
                if (bin.cb == other.bin.cb)
                  {
                    return memcmp (bin.lpb, other.bin.lpb, bin.cb)
                           == 0;
                  }
              break;
              default:
                break;
            }

          return false;

        }

        inline Property &
        operator= (const Property &other)
        {
          copyFrom (other);
          return *this;
        }

        inline void
        copyFrom (const Property &other)
        {
          //  if (IS_ERROR(other.tag))
          //     tag |= TAG_ERROR;
          // else
          tag = other.tag;
          if (other.flags & DONT_COPY_FLAG)
            {
              flags |= DONT_COPY_FLAG;
            }
#if FLAKE_NAMED_TAG_SUPPORT
          name = other.name;
#endif

          if ((other.tag & TAG_ARRAY) == TAG_ARRAY)
            {
              if (arr.lpValues != nullptr && ((flags & DEALLOC_SELF_FLAG) == DEALLOC_SELF_FLAG))
                free (arr.lpValues);

              arr.numValues = other.arr.numValues;
              arr.cValueSize = other.arr.cValueSize;
              arr.valueType = other.arr.valueType;
              if (arr.numValues * arr.cValueSize != 0)
                {
                  __setDeallocSelf ();
                  arr.lpValues = (byte *) malloc (arr.cValueSize
                                                  * arr.numValues);
                  memcpy (arr.lpValues, other.arr.lpValues,
                          arr.cValueSize * arr.numValues);
                }
              else
                {
                  flags = 0;
                }
              return;
            }

          switch (TAG_TYPE(other.tag))
            {
              case TT_BIN:
                if (bin.lpb != nullptr && ((flags & DEALLOC_SELF_FLAG) == DEALLOC_SELF_FLAG))
                  {
                    free (bin.lpb);
                  }
              bin.cb = other.bin.cb;
              if (bin.cb != 0)
                {
                  if (other.flags & DONT_COPY_FLAG)
                    {
                      bin.lpb = other.bin.lpb;
                    }
                  else
                    {
                      __setDeallocSelf ();
                      bin.lpb = (byte *) malloc (bin.cb);
                      memcpy (bin.lpb, other.bin.lpb, bin.cb);
                    }
                }
              else
                {
                  flags = 0;
                }
              break;

              case TT_STRING:
                if (other.str != nullptr)
                  {
                    if (str != nullptr && ((flags & DEALLOC_SELF_FLAG) == DEALLOC_SELF_FLAG))
                      free (str);

                    str = (char *) malloc (sizeof (char)
                                           * (strlen (other.str) + 1));

                    if (str != nullptr)
                      {
                        __setDeallocSelf ();
                        memset (str, 0, strlen (other.str) + 1);
                        memcpy (str, other.str, strlen (other.str)
                                                + 1);
                      }
                  }
                else
                  {
                    flags = 0;
                    str = nullptr;
                  }
              break;

              default:
                flags = 0;
              uuid = other.uuid;
            }

        }

        inline bool
        isErrorValue () const
        {
          return (tag & TAG_ERROR) == TAG_ERROR;
        }

        inline char *
        toString () const
        {

#if FLAKE_DEBUG_LOGGING

          char *_str = nullptr;

          char tagStr[30];

#if FLAKE_NAMED_TAG_SUPPORT
          if (name.length() > 0) {
                sprintf(tagStr, "(%s) %s", name.substr(0,16).c_str(), tagToString(tag));
            } else {
#endif
          sprintf (tagStr, "%s", tagToString (tag));
#if FLAKE_NAMED_TAG_SUPPORT
          }
#endif
          if (IS_ERROR(tag))
            {
              _str = new char[120];

              sprintf (_str, "%30s: %s", tagStr, errorToString (
                  (long) err));
              return _str;
            }

          if ((tag & TAG_ARRAY) == TAG_ARRAY)
            {
              _str = new char[120];
              sprintf (_str, "%30s: Array of %d %d-byte elements ",
                       tagStr, arr.numValues, arr.cValueSize);
              return _str;
            }

          switch (TAG_TYPE(tag))
            {
          case TT_BOOL:
            _str = new char[120];
              sprintf (_str, "%30s: %s", tagStr, b ? "true" : "false");
              break;
          case TT_UINT8:
          case TT_UINT16:
          case TT_UINT32:
            _str = new char[120];
              sprintf (_str, "%30s: %" UINT32_FMT, tagStr, u32);
              break;
          case TT_INT8:
            _str = new char[120];
              sprintf (_str, "%30s: %" INT8_FMT, tagStr, i8);
              break;
          case TT_INT16:
            _str = new char[120];
              sprintf (_str, "%30s: %" INT16_FMT, tagStr, i16);
              break;
          case TT_INT32:
            _str = new char[120];
              sprintf (_str, "%30s: %" INT32_FMT, tagStr, i32);
              break;
          case TT_DATETIME:
            _str = new char[120];
              sprintf (_str, "%30s: %s", tagStr, timeToString ((long) t));
              break;
          case TT_BIN:
            _str = new char[120];
              sprintf (_str, "%30s: %d Bytes", tagStr, bin.cb);
              break;

          case TT_UUID:
            {
              _str = new char[120];
              sprintf (_str, "%30s: %s", tagStr, uuid.toString ());
            }
              break;
          case TT_STRING:
            {
              if (str != nullptr)
                {
                  _str = new char[strlen (str) + 60];
                  memset (_str, 0, strlen (str) + 1);
                  sprintf (_str, "%30s: %s", tagStr, str);
                }
              else
                {
                  _str = new char[60];
                  sprintf (_str, "%30s: NULL", tagStr);
                }
            }
              break;

          case TT_FLOAT:
            {
              _str = new char[80];

              sprintf (_str, "%30s: %f", tagStr, f);
            }
              break;
          default:
            break;
            }

          return _str;
#else
          return 0;
#endif
        }

    };

/**
 * document me
 */
    struct PropArray : public Serializable, private vector<Property> {

     public:
      Property VAL_ERR_NOT_FOUND;

      inline virtual
      ~PropArray ()
      {

      }

      inline
      PropArray ()
      {
        VAL_ERR_NOT_FOUND.tag = TAG_ERROR;
        VAL_ERR_NOT_FOUND.err = E_NOT_FOUND;
      }

      inline
      PropArray (const PropArray &va)
          : PropArray ()
      {
        copyFrom (va);
      }

      inline const Property &
      operator[] (size_type __n) const
      {
        return at (__n);
      }

      inline PropArray &
      operator= (const PropArray &other)
      {
        copyFrom (other);
        return *this;
      }
      /**
         * document me
         */
      inline void
      copyFrom (const PropArray &other)
      {

        if (this != &other)
          {

            clear ();

            for (uint32_t i = 0; i < other.size (); i++)
              push_back (other[i]);

          }

      }

      inline void
      clear ()
      {
        vector<Property>::clear ();
      }

      /**
         * document me
         */
      inline const Property &
      get (uint32_t tag) const
      {
        for (uint32_t i = 0; i < size (); i++)
          {
            if (TAG_ID(at (i).tag) == TAG_ID(tag))
              return at (i);
          }

        return VAL_ERR_NOT_FOUND;
      }

      /**
         * document me
         */
      inline bool
      has (uint32_t tag) const
      {
        for (uint32_t i = 0; i < size (); i++)
          {
            if (TAG_ID(at (i).tag) == TAG_ID(tag))
              return true;
          }

        return false;
      }

      /**
         * document me
         */
      void
      set (const Property &val)
      {
        for (uint32_t i = 0; i < this->size (); i++)
          {
            if ((at (i).tag & 0xffff0000) == (val.tag & 0xffff0000))
              {
                at (i).copyFrom (val);

                return;
              }
          }

        push_back (val);
      }

      /**
         * document me
         */
      inline char *
      toString ()
      {

#if FLAKE_DEBUG_LOGGING

        string s;

        for (uint32_t i = 0; i < size (); i++)
          {

            char *str = at (i).toString ();
            if (str != nullptr)
              {
                s.append (str);
                if (i < size () - 1)
                  s.append ("\n");
                delete[] str;
              }

          }

        char *res = new char[s.length () + 1];

        memset (res, 0, s.length () + 1);
        if (s.length () > 0)
          memcpy (res, s.c_str (), s.length ());

        return res;

#else
        return 0;
#endif
      }
      /**
         * document me
         */

      inline size_type
      count () const
      {
        return size ();
      }

      inline bool
      serialize (uint16_t *len, flake::byte **buf) override
      {
        *len = 0;
        *buf = 0;

        if (size () == 0)
          return true;

        push_ push (len, buf);

        uint16_t numTags = (uint16_t) size ();
        push (&numTags, sizeof (uint16_t), false);

        for (uint16_t i = 0; i < size (); i++)
          {
            Property &v = at (i);
            (void) push (&v.tag, sizeof (uint32_t), false);

            if (ERROR_VAL(v))
              {
                // max_size = max<size_t>(max_size, sizeof(tt_err_t));
                push (&v.err, sizeof (tt_err_t), false);
                continue;
              }
            if ((v.tag & TAG_ARRAY) == TAG_ARRAY)
              {
                uint32_t arraySize = v.arr.numValues
                                     * v.arr.cValueSize + 6U;
                if (arraySize > 0xffff)
                  {
                    v.tag |= TAG_ERROR;
                    v.err = E_NO_ALLOC;
                    push (&v.err, sizeof (tt_err_t), false);
                    continue;
                  }

                // max_size = max<size_t>(max_size, arraySize);
                push (&arraySize, sizeof (uint16_t), false);
                push (&v.arr.numValues, sizeof (uint16_t), false);
                push (&v.arr.cValueSize, sizeof (uint16_t), false);
                push (&v.arr.valueType, sizeof (uint16_t), false);
                push (v.arr.lpValues, (uint16_t) (arraySize - 6U), false);
                continue;
              }

            switch (TAG_TYPE(v.tag))
              {
                case TT_FLOAT:
                  // max_size = max<size_t>(max_size, sizeof(tt_float_t));
                  push (&v.f, sizeof (tt_float_t), false);
                break;
                case TT_UINT8:
                  // max_size = max<size_t>(max_size, sizeof(uint8_t));
                  push (&v.u8, sizeof (uint8_t), false);
                break;
                case TT_UINT16:
                  // max_size = max<size_t>(max_size, sizeof(uint16_t));
                  push (&v.u16, sizeof (uint16_t), false);
                break;
                case TT_UINT32:
                  // max_size = max<size_t>(max_size, sizeof(uint32_t));
                  push (&v.u32, sizeof (uint32_t), false);
                break;
                case TT_INT8:
                  // max_size = max<size_t>(max_size, sizeof(int8_t));
                  push (&v.i8, sizeof (int8_t), false);
                break;
                case TT_INT16:
                  // max_size = max<size_t>(max_size, sizeof(int16_t));
                  push (&v.i16, sizeof (int16_t), false);
                break;
                case TT_INT32:
                  // max_size = max<size_t>(max_size, sizeof(int32_t));
                  push (&v.i32, sizeof (int32_t), false);
                break;
                case TT_BOOL:
                  //  max_size = max<size_t>(max_size, 1);
                  push (&v.b, 1, false);
                break;
                case TT_DATETIME:
                  // max_size = max<size_t>(max_size, sizeof(tt_time_t));
                  push (&v.t, sizeof (tt_time_t), false);
                break;
                case TT_STRING:
                  if (v.str != nullptr && strlen (v.str) < 0xffff)
                    {
                      //     max_size = max<size_t>(max_size, strlen(v.str) + 1);
                      push (v.str, (uint16_t) (strlen (v.str)));
                    }
                  else
                    {
                      char nullchar = '\0';
                      // max_size = max<size_t>(max_size, 1);
                      push (&nullchar, 0); // yes that'S right...
                    }
                break;
                case TT_UUID:
                  //  max_size = max<size_t>(max_size, sizeof(tt_uuid_t));
                  push (&v.uuid, sizeof (tt_uuid_t), false);
                break;
                case TT_BIN:
                  //  max_size = max<size_t>(max_size, v.bin.cb);
                  push (v.bin.lpb, v.bin.cb);
                break;

                default:
                  break;
              }

          }

        return true;
      }
      /**
         * document me
         */
      inline bool
      unserialize (uint16_t len, flake::byte *buf) override
      {

        if (len == 0)
          return true;

        uint16_t c = 0;
        uint16_t size = 0;

        pop_ pop (&buf, &c);

        uint16_t numTags = 0;
        pop (&numTags, sizeof (uint16_t));

        for (uint16_t i = 0; i < numTags && c < len; i++)
          {
            Property v;

            pop (&v.tag, sizeof (uint32_t));

            if (ERROR_VAL(v))
              {
                pop (&v.err, sizeof (tt_err_t));
                push_back (v);
                continue;
              }

            if ((v.tag & TAG_ARRAY) == TAG_ARRAY)
              {
                pop (&size, sizeof (uint16_t));
                pop (&v.arr.numValues, sizeof (uint16_t));
                pop (&v.arr.cValueSize, sizeof (uint16_t));
                pop (&v.arr.valueType, sizeof (uint16_t));
                if (size - 6 > 0 && v.arr.cValueSize > 0)
                  {
                    v.__setDeallocSelf ();
                    v.arr.lpValues = (byte *) malloc (size - 6 * sizeof (byte));
                    pop (v.arr.lpValues, (uint16_t) (size - 6));
                  }
                push_back (v);
                continue;
              }

            switch (TAG_TYPE(v.tag))
              {
                case TT_FLOAT:
                  pop (&v.f, sizeof (float));
                break;
                case TT_UINT8:
                  pop (&v.u8, sizeof (uint8_t));
                break;
                case TT_UINT16:
                  pop (&v.u16, sizeof (uint16_t));
                break;
                case TT_UINT32:
                  pop (&v.u32, sizeof (uint32_t));
                break;
                case TT_INT8:
                  pop (&v.i8, sizeof (int8_t));
                break;
                case TT_INT16:
                  pop (&v.i16, sizeof (int16_t));
                break;
                case TT_INT32:
                  pop (&v.i32, sizeof (int32_t));
                break;
                case TT_BOOL:
                  pop (&v.b, 1);
                break;
                case TT_DATETIME:
                  pop (&v.t, sizeof (tt_time_t));
                break;
                case TT_STRING:
                  pop (&size, sizeof (uint16_t));
                if (size > len)
                  return false;
                if (size > 0)
                  {
                    v.__setDeallocSelf ();
                    v.str = (char *) malloc ((size + 1) * sizeof (char));
                    memset (v.str, 0, size + 1);
                    pop (v.str, size);
                  }
                break;
                case TT_UUID:
                  pop (&v.uuid, sizeof (uniqueId_t));
                break;
                case TT_BIN:
                  {

                    pop (&size, sizeof (uint16_t));
                    if (size > len)
                      return false;
                    v.bin.cb = size;

                    if (size > 0)
                      {
                        v.__setDeallocSelf ();
                        v.bin.lpb = (byte *) malloc (size * sizeof (byte));
                        pop (v.bin.lpb, size);
                      }
                  }
                break;
                default:
                  break;

              }

            this->push_back (v);

          }

        return true;

      }

    };

    }

    namespace MessageType
    {

// Requests

        const uint8_t connect = 0x01;               // no indi
        const uint8_t disconnect = 0x02;            // no indi

        const uint8_t createObject = 0x03;                          // Parent
        const uint8_t queryObjects = 0x04;          // no indi      // Parent          //    CONF: PL
        const uint8_t destroyObject = 0x05;                         // Obj Addr

        const uint8_t createProperty = 0x06;                        // Obj Addr         //    REQ: PL
        const uint8_t setProperties = 0x07;                         // Obj Addr         //    REQ/CONF: PL
        const uint8_t getProperties = 0x08;                         // Obj Addr         //    REQ/CONF: PL

        const uint8_t joinGroup = 0x09;                             // Group Addr (or Obj Addr)
        const uint8_t leaveGroup = 0x0A;                            // Group Addr (or Obj Addr)
        const uint8_t custom = 0x0B;                                // Src -> Dst        //    REQ/CONF: PL

        const uint8_t openProperty = 0x0C;          // no indi       // Obj Add,  --> object / table / stream
        const uint8_t deleteProperty = 0x0D;                         // Obj Addr
        const uint8_t deleteObject = 0x0E;                           // Obj Addr


// Indications

        const uint8_t setPropertiesReq = 0x17;      // Obj Addr          //    INDI/RESP: PL
        const uint8_t getPropertiesReq = 0x18;      // Obj Addr          //    INDI/RESP: PL
        const uint8_t customMsgReceived = 0x1B;     // Src & Dst needed  //    INDI/RESP: PL

        const uint8_t proeprtyDeleted = 0x1D;       // Obj Addr          //    INDI/RESP: PL
        const uint8_t objectDeleted = 0x1E;         // Obj Addr

        const uint8_t objectCreated = 0x13;         // [parent -> group]  //    INDI: PL
        const uint8_t propertyCreated = 0x16;       // [object -> group]  //    INDI: PL
        const uint8_t left = 0x1A;                  // [object -> group]
        const uint8_t joined = 0x19;                // [object -> group]
        const uint8_t changed = 0x1F;               // [object -> group]  //    INDI: PL
        const uint8_t destroyed = 0x15;             // [object -> group]

        const uint8_t seek = 0x22;                  // potentially 0x32 when client->srv
        const uint8_t saveChanges = 0x23;
        const uint8_t configure = 0x24;             // enable disable stuff, e.g format of setProperties Confirmation

        const uint8_t ping = 0x30;                  // 0x20 when client->srv or client->obj
        const uint8_t auth = 0x31;

        const char *toString (uint8_t mt);

    }

    inline
    uint32_t preprocessTag (uint32_t t)
    {
      if (t > 0xffff) return (TAG_ID(t) << 16 | (t & 0xf000U)); else return (t << 16) | (t & 0xf000U);
    }

    inline
    Property PropMakeBool (uint32_t t, tt_bool_t v)
    {
      Property val (preprocessTag (t) | TT_BOOL);
      val.b = v;
      return val;
    }

    inline
    Property PropMakeU8 (uint32_t t, tt_uint8_t v)
    {
      Property val (preprocessTag (t) | TT_UINT8);
      val.u8 = v;
      return val;
    }

    inline
    Property PropMakeU16 (uint32_t t, tt_uint16_t v)
    {
      Property val (preprocessTag (t) | TT_UINT16);
      val.u16 = v;
      return val;
    }

    inline
    Property PropMakeU32 (uint32_t t, tt_uint32_t v)
    {
      Property val (preprocessTag (t) | TT_UINT32);
      val.u32 = v;
      return val;
    }

    inline
    Property PropMakeI8 (uint32_t t, tt_int8_t v)
    {
      Property val (preprocessTag (t) | TT_INT8);
      val.i8 = v;
      return val;
    }

    inline
    Property PropMakeI16 (uint32_t t, tt_int16_t v)
    {
      Property val (preprocessTag (t) | TT_INT16);
      val.i16 = v;
      return val;
    }

    inline
    Property PropMakeI32 (uint32_t t, tt_int32_t v)
    {
      Property val (preprocessTag (t) | TT_INT32);
      val.i32 = v;
      return val;
    }

    inline
    Property PropMakeDateTime (uint32_t t, tt_time_t v)
    {
      Property val (preprocessTag (t) | TT_DATETIME);
      val.t = v;
      return val;
    }

    inline
    Property PropMakeFloat (uint32_t t, tt_float_t v)
    {
      Property val (preprocessTag (t) | TT_FLOAT);
      val.f = v;
      return val;
    }

    inline
    Property PropMakeUUID (uint32_t t, tt_uuid_t v)
    {
      Property val (preprocessTag (t) | TT_UUID);
      val.uuid = v;
      return val;
    }

    inline
    Property PropMakeString (uint32_t t, tt_str_t v)
    {
      Property val (preprocessTag (t) | TT_STRING);
      val.str = v;
      return val;
    }

    inline
    Property PropMakeBin (uint32_t t, tt_bin_t v)
    {
      Property val (preprocessTag (t) | TT_BIN);
      val.bin = v;
      return val;
    }

}

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

#endif /* FLAKETYPES_H_ */

