#include "doctest/doctest.h"
#include <hikyuu/StockManager.h>

#include <hikyuu/utilities/ConnectPool.h>
#include <hikyuu/utilities/db_connect/DBConnect.h>
#include <hikyuu/utilities/db_connect/mysql/MySQLConnect.h>

using namespace hku;

TEST_CASE("test_temp") {
    Parameter param;
    param.set<string>("pwd", "Hikyuu2020*");

    auto con = MySQLConnect(param);
    CHECK(con.ping());

    con.exec(format("create database if not exists test; use test;"));
    CHECK(con.tableExist("t2018") == false);
    con.exec("create table t2018 (name VARCHAR(20), age INT)");
    CHECK(con.tableExist("t2018") == true);
    con.exec("drop table t2018");
    CHECK(con.tableExist("t2018") == false);

    if (!con.tableExist("t2019")) {
        con.exec(
          "create table t2019 (id INTEGER PRIMARY KEY AUTO_INCREMENT, "
          "name VARCHAR(50), "
          "data_int32 INT, data_int64 BIGINT, data_double DOUBLE, data_float FLOAT)");
    }

    class T2019 {
        TABLE_BIND5(t2019, name, data_int32, data_int64, data_double, data_float);

    public:
        T2019()
        : name(Null<string>()),
          data_int32(Null<int32>()),
          data_int64(Null<int64>()),
          data_double(Null<double>()),
          data_float(Null<float>()) {}

        void reset() {
            name = "";
            data_int32 = Null<int32>();
            data_int64 = Null<int64>();
            data_double = Null<double>();
            data_float = Null<float>();
        }

    public:
        string name;
        int32 data_int32;
        int64 data_int64;
        double data_double;
        float data_float;
    };

    T2019 x;
    x.name = "Davis";
    x.data_int32 = 32;
    x.data_int64 = 3147483647;
    x.data_double = 3.1415926;
    x.data_float = 3.14f;
    con.save(x);

    /*T2019 rx;
    con.load(rx);

    CHECK(rx.name == x.name);
    CHECK(rx.data_int32 == x.data_int32);
    CHECK(rx.data_int64 == x.data_int64);
    CHECK(std::abs(rx.data_double - x.data_double) < 0.00001);
    CHECK(std::abs(rx.data_float - x.data_float) < 0.00001);*/

    // con.exec(format("drop database if exists test;"));
}
