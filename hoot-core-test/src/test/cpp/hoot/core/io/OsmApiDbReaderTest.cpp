/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/io/ServicesDbReader.h>
#include <hoot/core/io/ServicesDbWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/OsmMap.h>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"


namespace hoot
{

class OsmApiDbReaderTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(OsmApiDbReaderTest);
  CPPUNIT_TEST(runReadOsmApiTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "OsmApiDbReaderTest@hoottestcpp.org"; }

  void tearDown()
  {
    // Services DB
    ServicesDbTestUtils::deleteUser(userEmail());

    // Osm Api DB
    OsmApiDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());
    database.deleteData();
    database.close();
  }

  void verifyFullReadOutput(shared_ptr<OsmMap> map)
  {
    //nodes
    HOOT_STR_EQUALS(true, map->containsNode(500));
    shared_ptr<Node> node = map->getNode(500);
    CPPUNIT_ASSERT_EQUAL((long)500, node->getId());
    CPPUNIT_ASSERT_EQUAL(38.4, node->getY());
    CPPUNIT_ASSERT_EQUAL(-106.5, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());
  }

  void runReadOsmApiTest()
  {
    ServicesDbReader reader;
    shared_ptr<OsmMap> map(new OsmMap());

    // parse out the osm api dbname, dbuser, and dbpassword
    //example: postgresql://hoot:hoottest@localhost:5432/osmapi_test
    QUrl dbUrl = ServicesDbTestUtils::getOsmApiDbUrl();
    QString dbUrlString = dbUrl.toString();
    QStringList dbUrlParts = dbUrlString.split("/");
    QString dbName = dbUrlParts[dbUrlParts.size()-1];
    QStringList userParts = dbUrlParts[dbUrlParts.size()-2].split(":");
    QString dbUser = userParts[0];
    QString dbPassword = userParts[1].split("@")[0];
    QString dbHost = userParts[1].split("@")[1];
    QString dbPort = userParts[2];

    LOG_DEBUG("Name="+dbName+", user="+dbUser+", pass="+dbPassword+", port="+dbPort+", host="+dbHost);

    ////////////////////////////////////////
    // insert simple test data
    ////////////////////////////////////////
    QString auth = "-h "+dbHost+" -p "+dbPort+" -U "+dbUser;
    QString cmd = "export PGPASSWORD="+dbPassword+"; export PGUSER="+dbUser+"; export PGDATABASE="+dbName+";\
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/users.sql > /dev/null 2>&1; \
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/changesets.sql > /dev/null 2>&1; \
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/nodesReadTest.sql > /dev/null 2>&1";

    if( std::system(cmd.toStdString().c_str()) != 0 )
    {
      LOG_WARN("Failed postgres command.  Exiting test.");
      return;
    }

    ///////////////////////////////////////
    // test the reader
    ///////////////////////////////////////

    OsmApiDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());

    Settings s = conf();
    reader.open(ConfigOptions(s).getServicesDbTestUrlOsmapi());
    reader.read(map);
    verifyFullReadOutput(map);
    reader.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiDbReaderTest, "slow");

}
