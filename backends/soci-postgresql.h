//
// Copyright (C) 2005 Maciej Sobczak
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//

#ifndef SOCI_POSTGRESQL_H_INCLUDED
#define SOCI_POSTGRESQL_H_INCLUDED

#include "soci-common.h"
#include <libpq-fe.h>


namespace SOCI
{

struct PostgreSQLStatementBackEnd;
struct PostgreSQLStandardIntoTypeBackEnd : details::StandardIntoTypeBackEnd
{
    PostgreSQLStandardIntoTypeBackEnd(PostgreSQLStatementBackEnd &st)
        : statement_(st) {}

    virtual void defineByPos(int &position,
        void *data, details::eExchangeType type);

    virtual void preFetch();
    virtual void postFetch(bool gotData, bool calledFromFetch,
        eIndicator *ind);

    virtual void cleanUp();

    PostgreSQLStatementBackEnd &statement_;
};

struct PostgreSQLVectorIntoTypeBackEnd : details::VectorIntoTypeBackEnd
{
    PostgreSQLVectorIntoTypeBackEnd(PostgreSQLStatementBackEnd &st)
        : statement_(st) {}

    virtual void defineByPos(int &position,
        void *data, details::eExchangeType type);

    virtual void preFetch();
    virtual void postFetch(bool gotData, eIndicator *ind);

    virtual void resize(std::size_t sz);
    virtual std::size_t size();

    virtual void cleanUp();

    PostgreSQLStatementBackEnd &statement_;
};

struct PostgreSQLStandardUseTypeBackEnd : details::StandardUseTypeBackEnd
{
    PostgreSQLStandardUseTypeBackEnd(PostgreSQLStatementBackEnd &st)
        : statement_(st) {}

    virtual void bindByPos(int &position,
        void *data, details::eExchangeType type);
    virtual void bindByName(std::string const &name,
        void *data, details::eExchangeType type);

    virtual void preUse(eIndicator const *ind);
    virtual void postUse(bool gotData, eIndicator *ind);

    virtual void cleanUp();

    PostgreSQLStatementBackEnd &statement_;
};

struct PostgreSQLVectorUseTypeBackEnd : details::VectorUseTypeBackEnd
{
    PostgreSQLVectorUseTypeBackEnd(PostgreSQLStatementBackEnd &st)
        : statement_(st) {}

    virtual void bindByPos(int &position,
        void *data, details::eExchangeType type);
    virtual void bindByName(std::string const &name,
        void *data, details::eExchangeType type);

    virtual void preUse(eIndicator const *ind);

    virtual std::size_t size();

    virtual void cleanUp();

    PostgreSQLStatementBackEnd &statement_;
};

struct PostgreSQLSessionBackEnd;
struct PostgreSQLStatementBackEnd : details::StatementBackEnd
{
    PostgreSQLStatementBackEnd(PostgreSQLSessionBackEnd &session);

    virtual void alloc();
    virtual void cleanUp();
    virtual void prepare(std::string const &query);

    virtual execFetchResult execute(int number);
    virtual execFetchResult fetch(int number);

    virtual int getNumberOfRows();

    virtual int prepareForDescribe();
    virtual void describeColumn(int colNum, eDataType &dtype,
        std::string &columnName, int &size, int &precision, int &scale,
        bool &nullOk);

    virtual PostgreSQLStandardIntoTypeBackEnd * makeIntoTypeBackEnd();
    virtual PostgreSQLStandardUseTypeBackEnd * makeUseTypeBackEnd();
    virtual PostgreSQLVectorIntoTypeBackEnd * makeVectorIntoTypeBackEnd();
    virtual PostgreSQLVectorUseTypeBackEnd * makeVectorUseTypeBackEnd();

    PostgreSQLSessionBackEnd &session_;

    PGresult *result_;
    std::string query_;
};

struct PostgreSQLRowIDBackEnd : details::RowIDBackEnd
{
    PostgreSQLRowIDBackEnd(PostgreSQLSessionBackEnd &session);

    ~PostgreSQLRowIDBackEnd();
};

struct PostgreSQLBLOBBackEnd : details::BLOBBackEnd
{
    PostgreSQLBLOBBackEnd(PostgreSQLSessionBackEnd &session);

    ~PostgreSQLBLOBBackEnd();

    virtual std::size_t getLen();
    virtual std::size_t read(std::size_t offset, char *buf,
        std::size_t toRead);
    virtual std::size_t write(std::size_t offset, char const *buf,
        std::size_t toWrite);
    virtual std::size_t append(char const *buf, std::size_t toWrite);
    virtual void trim(std::size_t newLen);

    PostgreSQLSessionBackEnd &session_;
};

struct PostgreSQLSessionBackEnd : details::SessionBackEnd
{
    PostgreSQLSessionBackEnd(std::string const &connectString);

    ~PostgreSQLSessionBackEnd();

    virtual void commit();
    virtual void rollback();

    void cleanUp();

    virtual PostgreSQLStatementBackEnd * makeStatementBackEnd();
    virtual PostgreSQLRowIDBackEnd * makeRowIDBackEnd();
    virtual PostgreSQLBLOBBackEnd * makeBLOBBackEnd();

    PGconn *conn_;
};

} // namespace SOCI

#endif // SOCI_POSTGRESQL_H_INCLUDED
