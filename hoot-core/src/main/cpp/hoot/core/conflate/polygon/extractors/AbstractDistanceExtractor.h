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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ABSTRACTDISTANCEEXTRACTOR_H
#define ABSTRACTDISTANCEEXTRACTOR_H

// geos
#include <geos/geom/Envelope.h>
#include <geos/geom/Point.h>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/extractors/FeatureExtractor.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * @author RoadMatcher
 * @copyright GPL
 * http://www.vividsolutions.com/products.asp?catg=spaapp&code=roadmatcher
 * The ideas were shamelessly taken from RoadMatcher, but reimplemented in C++ with Hootenanny
 * appropriate data structures.
 */
class AbstractDistanceExtractor : public FeatureExtractor
{
public:
  static string className() { return "hoot::AbstractDistanceExtractor"; }

  virtual double combinedEnvelopeDiagonalDistance(const OsmMap& map,
    const shared_ptr<const Element>& target,
    const shared_ptr<const Element>& candidate) const
  {
    ConstOsmMapPtr m = map.shared_from_this();
    auto_ptr<Envelope> env(target->getEnvelope(m));
    auto_ptr<Envelope> candidateEnv(candidate->getEnvelope(m));
    env->expandToInclude(candidateEnv.get());
    return sqrt(env->getWidth() * env->getWidth() + env->getHeight() * env->getHeight());
  }

  virtual double distance(const OsmMap& map, const shared_ptr<const Element>& target,
    const shared_ptr<const Element>& candidate) const = 0;

  virtual double extract(const OsmMap& map, const shared_ptr<const Element>& target,
    const shared_ptr<const Element>& candidate) const
  {
    return 1 - distance(map, target, candidate) /
      combinedEnvelopeDiagonalDistance(map, target, candidate);
  }

  virtual DataFrame::FactorType getFactorType() const { return DataFrame::Numerical; }

  virtual DataFrame::NullTreatment getNullTreatment() const
  {
    return DataFrame::NullAsMissingValue;
  }

};

}

#endif // ABSTRACTDISTANCEEXTRACTOR_H
