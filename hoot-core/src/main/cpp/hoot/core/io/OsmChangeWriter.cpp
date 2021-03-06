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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmChangeWriter.h"

// hoot
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/OsmUtils.h>

// Qt
#include <QFile>
#include <QXmlStreamWriter>

namespace hoot
{

OsmChangeWriter::OsmChangeWriter()
{
  _precision = ConfigOptions().getWriterPrecision();
}

void OsmChangeWriter::write(QString path, ChangeSetProviderPtr cs)
{
  LOG_INFO("Writing changeset to " << path);

  QFile f;
  f.setFileName(path);
  if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw HootException(QObject::tr("Error opening %1 for writing").arg(path));
  }

  write(f, cs);

  f.close();
}

void OsmChangeWriter::write(QIODevice &d, ChangeSetProviderPtr cs)
{
  QXmlStreamWriter writer(&d);
  writer.setCodec("UTF-8");
  writer.setAutoFormatting(true);
  writer.writeStartDocument();

  writer.writeStartElement("osmChange");
  writer.writeAttribute("version", "0.6");
  writer.writeAttribute("generator", "hootenanny");

  Change::ChangeType last = Change::Unknown;

  while (cs->hasMoreChanges())
  {
    Change c = cs->readNextChange();

    if (c.type != last)
    {
      if (last != Change::Unknown)
      {
        writer.writeEndElement();
      }
      switch (c.type)
      {
      case Change::Create:
        writer.writeStartElement("create");
        break;
      case Change::Delete:
        writer.writeStartElement("delete");
        break;
      case Change::Modify:
        writer.writeStartElement("modify");
        break;
      default:
        throw IllegalArgumentException("Unexepected change type.");
      }
      last = c.type;
    }

    switch (c.e->getElementType().getEnum())
    {
    case ElementType::Node:
      writeNode(writer, dynamic_pointer_cast<const Node>(c.e));
      break;
    case ElementType::Way:
      writeWay(writer, dynamic_pointer_cast<const Way>(c.e));
      break;
    case ElementType::Relation:
      writeRelation(writer, dynamic_pointer_cast<const Relation>(c.e));
      break;
    default:
      throw IllegalArgumentException("Unexpected element type.");
    }
  }

  if (last != Change::Unknown)
  {
    writer.writeEndElement();
  }
  writer.writeEndElement();
  writer.writeEndDocument();
}

void OsmChangeWriter::writeNode(QXmlStreamWriter& writer, ConstNodePtr n)
{
  writer.writeStartElement("node");
  writer.writeAttribute("id", QString::number(n->getId()));
  if (n->getTimestamp() != 0)
  {
    writer.writeAttribute("timestamp", OsmUtils::toTimeString(n->getTimestamp()));
  }
  writer.writeAttribute("version", QString::number(n->getVersion()));

  writer.writeAttribute("lat", QString::number(n->getY(), 'f', _precision));
  writer.writeAttribute("lon", QString::number(n->getX(), 'f', _precision));

  const Tags& tags = n->getTags();
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); it++)
  {
    if (it.key().isEmpty() == false && it.value().isEmpty() == false)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", OsmWriter().removeInvalidCharacters(it.key()));
      writer.writeAttribute("v", OsmWriter().removeInvalidCharacters(it.value()));
      writer.writeEndElement();
    }
  }

  // turn this on when we start using node circularError.
  if (n->hasCircularError() &&
      n->getTags().getNonDebugCount() > 0)
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", "error:circular");
    writer.writeAttribute("v", QString("%1").arg(n->getCircularError()));
    writer.writeEndElement();
  }

  writer.writeEndElement();
}

void OsmChangeWriter::writeWay(QXmlStreamWriter& writer, ConstWayPtr w)
{
  writer.writeStartElement("way");
  writer.writeAttribute("id", QString::number(w->getId()));
  if (w->getTimestamp() != 0)
  {
    writer.writeAttribute("timestamp", OsmUtils::toTimeString(w->getTimestamp()));
  }
  writer.writeAttribute("version", QString::number(w->getVersion()));

  for (size_t j = 0; j < w->getNodeCount(); j++)
  {
    writer.writeStartElement("nd");
    writer.writeAttribute("ref", QString::number(w->getNodeId(j)));
    writer.writeEndElement();
  }

  const Tags& tags = w->getTags();
  for (Tags::const_iterator tit = tags.constBegin(); tit != tags.constEnd(); ++tit)
  {
    if (tit.key().isEmpty() == false && tit.value().isEmpty() == false)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", OsmWriter().removeInvalidCharacters(tit.key()));
      writer.writeAttribute("v", OsmWriter().removeInvalidCharacters(tit.value()));
      writer.writeEndElement();
    }
  }

  if (w->hasCircularError())
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", "error:circular");
    writer.writeAttribute("v", QString("%1").arg(w->getCircularError()));
    writer.writeEndElement();
  }

  writer.writeEndElement();
}

void OsmChangeWriter::writeRelation(QXmlStreamWriter& writer, ConstRelationPtr r)
{
  writer.writeStartElement("relation");
  writer.writeAttribute("id", QString::number(r->getId()));
  if (r->getTimestamp() != 0)
  {
    writer.writeAttribute("timestamp", OsmUtils::toTimeString(r->getTimestamp()));
  }
  writer.writeAttribute("version", QString::number(r->getVersion()));

  const vector<RelationData::Entry>& members = r->getMembers();
  for (size_t j = 0; j < members.size(); j++)
  {
    const RelationData::Entry& e = members[j];
    writer.writeStartElement("member");
    writer.writeAttribute("type", e.getElementId().getType().toString().toLower());
    writer.writeAttribute("ref", QString::number(e.getElementId().getId()));
    writer.writeAttribute("role", OsmWriter().removeInvalidCharacters(e.role));
    writer.writeEndElement();
  }

  const Tags& tags = r->getTags();
  for (Tags::const_iterator tit = tags.constBegin(); tit != tags.constEnd(); ++tit)
  {
    if (tit.key().isEmpty() == false && tit.value().isEmpty() == false)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", OsmWriter().removeInvalidCharacters(tit.key()));
      writer.writeAttribute("v", OsmWriter().removeInvalidCharacters(tit.value()));
      writer.writeEndElement();
    }
  }

  if (r->getType() != "")
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", "type");
    writer.writeAttribute("v", OsmWriter().removeInvalidCharacters(r->getType()));
    writer.writeEndElement();
  }

  if (r->hasCircularError())
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", "error:circular");
    writer.writeAttribute("v", QString("%1").arg(r->getCircularError()));
    writer.writeEndElement();
  }

  writer.writeEndElement();
}

}
