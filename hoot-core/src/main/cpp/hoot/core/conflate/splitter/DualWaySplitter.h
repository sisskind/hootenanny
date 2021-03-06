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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef DUALWAYSPLITTER_H
#define DUALWAYSPLITTER_H

// GEOS
#include <geos/geom/Coordinate.h>

// Hoot
#include <hoot/core/Units.h>
#include <hoot/core/ops/OsmMapOperation.h>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{
  using namespace geos::geom;

  class OsmMap;
  class Way;

/**
 * Splits all "divided=yes" highways into two oneway streets. All intersections should be split
 * before this method is called. Failure to do so will result in undefined behavior.
 */
class DualWaySplitter : public OsmMapOperation
{
public:

  static string className() { return "hoot::DualWaySplitter"; }

  typedef enum DrivingSide
  {
    Left,
    Right
  } DrivingSide;

  DualWaySplitter();

  DualWaySplitter(shared_ptr<const OsmMap> map, DrivingSide drivingSide, Meters splitSize);

  shared_ptr<OsmMap> splitAll();

  static shared_ptr<OsmMap> splitAll(shared_ptr<const OsmMap> map, DrivingSide drivingSide,
                                     Meters defaultSplitSize);

  void apply(shared_ptr<OsmMap>& map);

private:
  Meters _defaultSplitSize;
  DrivingSide _drivingSide;
  shared_ptr<const OsmMap> _map;
  shared_ptr<OsmMap> _result;

  // temporary variables for convenience
  shared_ptr<Way> _left, _right;
  shared_ptr<const Way> _working;
  Meters _splitSize;

  void _addConnector(long nodeId);

  shared_ptr<Way> _createOneWay(shared_ptr<const Way> w, Meters bufferSize, bool left);


  void _createStub(shared_ptr<Way> dividedWay, long centerNodeId, long edgeNodeId);

  double _dotProduct(const Coordinate& c1, const Coordinate& c2) const;

  void _fixLanes(shared_ptr<Way> w);

  /**
   * Returns the node id of the nearest node to nid on w
   */
  long _nearestNode(long nid, shared_ptr<const Way> w);

  Coordinate _normalizedVector(long nid1, long nid2);

  bool _onRight(long intersectionId, shared_ptr<Way> inbound, long leftNn, long rightNn);

  void _reconnectEnd(long centerNodeId, shared_ptr<Way> edge);

  void _splitIntersectingWays(long nid);

  void _splitWay(long wid);
};

}

#endif // DUALWAYSPLITTER_H
