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

#ifndef WAYTAGMERGEMANIPULATION_H
#define WAYTAGMERGEMANIPULATION_H

#include "WayMergeManipulation.h"

namespace hoot
{

/**
 * This manipulation will add the tags from Unknown2 to Unknown1 and remove Unknown2 from the map.
 */
class WayTagMergeManipulation : public WayMergeManipulation
{
public:

  WayTagMergeManipulation(long leftId, long rightId, shared_ptr<const OsmMap> map,
    Meters minSplitSize);

  virtual void applyManipulation(shared_ptr<OsmMap> wm, set<ElementId>& impactedElements,
    set<ElementId>& newElements) const;

  virtual const set<long>& getImpactedWayIds(shared_ptr<const OsmMap> map) const;

protected:

};

}

#endif // WAYTAGMERGEMANIPULATION_H
