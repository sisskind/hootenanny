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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "TagCriterion.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, TagCriterion)

TagCriterion::TagCriterion()
{
  setConfiguration(conf());
}

void TagCriterion::setConfiguration(const Settings &s)
{
  QString kvp = ConfigOptions(s).getTagCriterionKvp();
  if (!kvp.isEmpty())
  {
    QStringList l = kvp.split(",");
    if (l.size() != 2)
    {
      throw IllegalArgumentException(
            QString("Expected '%1' to be populated with 'key,value'.").
            arg(ConfigOptions::getTagCriterionKvpKey()));
    }
    _k = l[0];
    _v = l[1];
  }
}

}