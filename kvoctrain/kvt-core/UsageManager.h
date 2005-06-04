/***************************************************************************

                          manage usage labels

    -----------------------------------------------------------------------

    begin                : Sun Dec 19 11:26:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team

    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef UsageManager_included
#define UsageManager_included

#include <vector>
using namespace std;

#include "kvoctraindoc.h"

class kvoctrainDoc;
class KConfig;
class kvoctrainExpr;

// internal usage labels, never change !

#define UL_AM             "Am"
#define UL_ABBR           "abbr"
#define UL_ANAT           "anat"
#define UL_ASTR           "astr"
#define UL_BIOL           "biol"
#define UL_BS             "bs"
#define UL_CONTP          "contp"
#define UL_ECCL           "eccl"
#define UL_FIG            "fig"
#define UL_GEOL           "geol"
#define UL_HIST           "hist"
#define UL_ICHT           "icht"
#define UL_IFML           "ifm"
#define UL_IRONIC         "iro"
#define UL_IRR            "irr"
#define UL_LIT            "lit"
#define UL_META           "metal"
#define UL_METEO          "meteo"
#define UL_MIN            "miner"
#define UL_MOT            "mot"
#define UL_MOUNT          "mount"
#define UL_MYT            "myth"
#define UL_NPR            "npr"
#define UL_OPT            "opt"
#define UL_ORN            "orn"
#define UL_OS             "os"
#define UL_P              "p"
#define UL_PARL           "parl"
#define UL_PHARM          "pharm"
#define UL_PHLS           "phls"
#define UL_PHOT           "phot"
#define UL_PHYS           "phys"
#define UL_PHYSIOL        "physio"
#define UL_PL             "pl"
#define UL_POET           "poet"
#define UL_POL            "pol"
#define UL_PROV           "prov"
#define UL_PSYCH          "psych"
#define UL_RHET           "rhet"
#define UL_SURV           "surv"
#define UL_TEL            "telg"
#define UL_TELEPH         "telph"
#define UL_THEA           "thea"
#define UL_TYP            "typo"
#define UL_UNIV           "univ"
#define UL_VET            "vet"
#define UL_ZO             "zoo"

// usage delimiters
#define UL_USER_USAGE  "#"   // designates number of user type
#define UL_USAGE_DIV   ":"   // divide different labels

struct UsageRelation
{
 public:

  UsageRelation (const QString & id, const QString & shorty, const QString & longy)
    :ident(id), shortId(shorty), longId(longy) {}

  QString identStr() const { return ident; }
  QString shortStr() const { return shortId; }
  QString longStr()  const { return longId;  }

 protected:
  QString ident,
          shortId,
          longId;
};


class UsageManager
{
 public:

  UsageManager ();

  static bool contains (const QString& label, const QString& collection);
  static vector<UsageRelation> getRelation ();
  static void setUsageNames (vector<QString> names);

 protected:

 private:
  static vector<QString> userUsages;
};

#endif // UsageManager_included
