/*
 * FlakeConst.h
 *
 *  Created on: 14.10.2010
 *      Author: root
 */

#ifndef FLAKECONST_H_
#define FLAKECONST_H_

#ifndef FLAKE_DEBUG_LOGGING
#define FLAKE_DEBUG_LOGGING             1
#endif

#ifndef FLAKE_AUTH_SUPPORT
#define FLAKE_AUTH_SUPPORT              0
#endif

#ifndef FLAKE_STREAM_SUPPORT
#define FLAKE_STREAM_SUPPORT            0
#endif

#ifndef FLAKE_NAMED_TAG_SUPPORT
#define FLAKE_NAMED_TAG_SUPPORT         0
#endif

#ifndef FLAKE_TLS
#define FLAKE_TLS                       1
#endif

#ifndef FLAKE_DTLS
#define FLAKE_DTLS                      0
#endif

#ifndef FLAKE_DEFAULT_TIMEOUT_MS
#define FLAKE_DEFAULT_TIMEOUT_MS        10000
#endif

#define MAX_PENDING_ASYNC_REQUESTS      5
#define MAX_PENDING_SYNC_REQUESTS       5
// Pending indications should relate to the number of concurrent tcp connecitons
#define MAX_PENDING_INDICATIONS         20

#define E_OK                       (0)
#define E_FAILED                  (-1)
#define E_NO_ALLOC                (-2)
#define E_NOT_FOUND               (-3)
#define E_READ_ONLY               (-4)
#define E_NOT_IMPL                (-5)
#define E_PENDING                 (-6)
#define E_OBJECT_NOT_INITIALIZED  (-7)
#define E_UNREGISTERED_OBJECT     (-8)
#define E_IGNORED                 (-9)
#define E_NO_CHANGES              (-10)
#define E_PARTIAL_SUCCESS         (-11)
#define E_INCOMPLETE              (-12)
#define E_UNSUPPORTED             (-13)
#define E_UNAUTHORIZED            (-14)

#define E_TIMEOUT                 (-101)
#define E_NOT_CONNECTED           (-102)
#define E_REFUSED                 (-103)

#define E_DONT_DISPATCH           (-104)
#define E_DESTINATION_UNREACHABLE (-105)

// Internal Wire Error Messages

#define E_FCNTL                   (-1000)
#define E_LISTEN                  (-1001)
#define E_SOCK                    (-1002)
#define E_BIND                    (-1003)


#define FLAKE_SUCCEEDED(e) ({ int result = (e); result == E_OK || result == E_READ_ONLY || result == E_NO_CHANGES; })


#endif /* FLAKECONST_H_ */
