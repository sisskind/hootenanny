
SHELL=/bin/bash

# If the silent flag is passed to make then make hoot quiet too.
ifneq (,$(findstring s,$(MAKEFLAGS)))
  HOOT_OPTS=--warn
  TIME=
  ECHO=true
endif

CWD=$(shell pwd)

$(shell $(HOOT_HOME)/scripts/ReplaceEnvironmentVariables.sh DbSettings.inc.in DbSettings.inc)
-include DbSettings.inc

HOOT_OPTS+= -D osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor
HOOT_OPTS+= -D hootapi.db.writer.overwrite.map=true -D hootapi.db.writer.create.user=true
HOOT_OPTS+= -D "hootapi.db.writer.email=test@test.com" -D "hootapi.db.reader.email=test@test.com"

test:
	echo Running user interface tests...
ifneq (,$(findstring s,$(MAKEFLAGS)))
	$(HOOT_HOME)/scripts/DeployTomcat.sh --quiet 2> >( $(FILTER) >&2 )
else
	$(HOOT_HOME)/scripts/DeployTomcat.sh
endif
	rm -f tmp/TestRun.log*
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/DcGisRoads.osm $(DB_URL)/DcGisRoadsCucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/DcTigerRoads.osm $(DB_URL)/DcTigerRoadsCucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/conflate/unified/AllDataTypesA.osm $(DB_URL)/AllDataTypesACucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/conflate/unified/AllDataTypesB.osm $(DB_URL)/AllDataTypesBCucumber
	mkdir -p tmp
ifneq (,$(findstring s,$(MAKEFLAGS)))
	./SetupDisplay.sh cucumber &> tmp/TestRun.log.tmp && mv tmp/TestRun.log.tmp tmp/TestRun.log
else
	./SetupDisplay.sh cucumber
endif
	# $(HOOT_HOME)/scripts/StopTomcat.sh
	echo ""

clean:
	rm -rf tmp
	

