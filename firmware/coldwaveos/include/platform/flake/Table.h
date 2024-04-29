/**
 * @file Table.h
 * @version Alpha
 * @author IMAGINEON
 * @date 11/3/2010
 * @since Alpha
 */


#ifndef TABLE_H_
#define TABLE_H_

#include "FlakeTypes.h"
#include "macro.h"
#include "Object.h"

namespace flake
{

    typedef TagArray ColumnSet;
    typedef PropArray TableRow;

    typedef enum
    {
        TCO_EQ
    } TableCompareOperation;

    typedef enum
    {
        RES_UNRESTRICTED = 0, RES_COMPARE
    } TableRestrictionType;

    struct TableCompareRestriction
    {
        TableCompareOperation op;
        uint32_t tag;
        value_t v;
    };

    /**
     * document me
     */
#ifndef EXCLUDE_FROM_DOCS
    class TableRestriction
    {
    public:
        TableRestriction()
        {
            restrictionType = RES_UNRESTRICTED;
        }
        TableRestrictionType restrictionType;
        union
        {
            TableCompareRestriction rtCompare;
        };



    };
#endif
    namespace Column = BaseProperties;

    /**
     * document me
     */

    class Table
    {
     protected:
      ~Table()   =default;

     public:
        /**
         * document me
         */
        virtual uint64_t
        getNumRows() = 0;

        /**
         * document me
         */
        virtual uint64_t
        getNumColumns() = 0;

        /**
         * document me
         */
        virtual void
        getRow(uint64_t index, PropArray& row) = 0;

        /**
         * document me
         */
        virtual void
        sort() = 0;


    };

}

#endif /* TABLE_H_ */