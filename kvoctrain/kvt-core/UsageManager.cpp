/***************************************************************************

                            manage usage labels

    -----------------------------------------------------------------------

    begin                : Fri Mar 31 20:50:53 MET 1999

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

#include "UsageManager.h"

#include <klocale.h>


#include <iostream>
#include <vector>
using namespace std;

vector<QString> UsageManager::userUsages;

// user usages are strings like this: #1

struct internalRelation_t
{
  const char *ident,
             *context,
             *shortid,
             *longId;
};

#ifndef I18N_NOOP2
#define I18N_NOOP2(context, str) context, str
#endif

static internalRelation_t InternalUsageRelations [] =
{
  { UL_AM,        0,   I18N_NOOP("Am."),      I18N_NOOP("Americanism")},
  { UL_ABBR,      0,   I18N_NOOP("abbr."),    I18N_NOOP("abbreviation")},
  { UL_ANAT,      0,   I18N_NOOP("anat."),    I18N_NOOP("anatomy")},
  { UL_ASTR,      0,   I18N_NOOP("astr."),    I18N_NOOP("astronomy")},
  { UL_BIOL,      0,   I18N_NOOP("biol."),    I18N_NOOP("biology")},
  { UL_BS,        0,   I18N_NOOP("b.s."),     I18N_NOOP("bad sense")},
  { UL_CONTP,     0,   I18N_NOOP("contp."),   I18N_NOOP("contemptuously")},
  { UL_ECCL,      0,   I18N_NOOP("eccl."),    I18N_NOOP("ecclesiastical")},
  { UL_FIG,       0,   I18N_NOOP("fig."),     I18N_NOOP("figuratively")},
  { UL_GEOL,      0,   I18N_NOOP("geol."),    I18N_NOOP("geology")},
  { UL_HIST,      0,   I18N_NOOP("hist."),    I18N_NOOP("historical")},
  { UL_ICHT,      0,   I18N_NOOP("icht."),    I18N_NOOP("ichthyology")},
  { UL_IFML,      0,   I18N_NOOP("ifml."),    I18N_NOOP("informal")},
  { UL_IRONIC,    0,   I18N_NOOP("iro."),     I18N_NOOP("ironic")},
  { UL_IRR,       0,   I18N_NOOP("irr."),     I18N_NOOP("irregular")},
  { UL_LIT,       0,   I18N_NOOP("lit."),     I18N_NOOP("literary")},
  { UL_META,      0,   I18N_NOOP("metall."),  I18N_NOOP("metallurgy")},
  { UL_METEO,     0,   I18N_NOOP("meteor."),  I18N_NOOP("meteorology")},
  { UL_MIN,       0,   I18N_NOOP("min."),     I18N_NOOP("mineralogy")},
  { UL_MOT,       0,   I18N_NOOP("mot."),     I18N_NOOP("motoring")},
  { UL_MOUNT,     0,   I18N_NOOP("mount."),   I18N_NOOP("mountaineering")},
  { UL_MYT,       0,   I18N_NOOP("myth."),    I18N_NOOP("mythology")},
  { UL_NPR,       0,   I18N_NOOP2("abbreviation: proper name",  "npr."),  I18N_NOOP("proper name")},
  { UL_OPT,       0,   I18N_NOOP("opt."),     I18N_NOOP("optics")},
  { UL_ORN,       0,   I18N_NOOP("orn."),     I18N_NOOP("ornithology")},
  { UL_OS,        0,   I18N_NOOP("o.s."),     I18N_NOOP("oneself")},
  { UL_P,         0,   I18N_NOOP("pers."),    I18N_NOOP("person")},
  { UL_PARL,      0,   I18N_NOOP("parl."),    I18N_NOOP("parliamentary")},
  { UL_PHARM,     0,   I18N_NOOP("pharm."),   I18N_NOOP("pharmacy")},
  { UL_PHLS,      0,   I18N_NOOP("phls."),    I18N_NOOP("philosophy")},
  { UL_PHOT,      0,   I18N_NOOP("phot."),    I18N_NOOP("photography")},
  { UL_PHYS,      0,   I18N_NOOP("phys."),    I18N_NOOP("physics")},
  { UL_PHYSIOL,   0,   I18N_NOOP("physiol."), I18N_NOOP("physiology")},
  { UL_PL,        0,   I18N_NOOP("pl."),      I18N_NOOP("plural")},
  { UL_POET,      0,   I18N_NOOP("poet."),    I18N_NOOP("poetry")},
  { UL_POL,       0,   I18N_NOOP("pol."),     I18N_NOOP("politics")},
  { UL_PROV,      0,   I18N_NOOP("prov."),    I18N_NOOP("provincialism")},
  { UL_PSYCH,     0,   I18N_NOOP("psych."),   I18N_NOOP("psychology")},
  { UL_RHET,      0,   I18N_NOOP("rhet."),    I18N_NOOP("rhetoric")},
  { UL_SURV,      0,   I18N_NOOP("surv."),    I18N_NOOP("surveying")},
  { UL_TEL,       0,   I18N_NOOP("tel."),     I18N_NOOP("telegraphy")},
  { UL_TELEPH,    0,   I18N_NOOP("teleph."),  I18N_NOOP("telephony")},
  { UL_THEA,      0,   I18N_NOOP("thea."),    I18N_NOOP("theater")},
  { UL_TYP,       0,   I18N_NOOP("typ."),     I18N_NOOP("typography")},
  { UL_UNIV,      0,   I18N_NOOP("univ."),    I18N_NOOP("university")},
  { UL_VET,       0,   I18N_NOOP("vet."),     I18N_NOOP("veterinary medicine")},
  { UL_ZO,        0,   I18N_NOOP("zo."),      I18N_NOOP("zoology")},
//{ UL_,          0,   I18N_NOOP(""),         I18N_NOOP("")},
  { 0, 0, 0, 0 } // the end
};


UsageManager::UsageManager ()
{
}


vector<UsageRelation> UsageManager::getRelation ()
{
  vector<UsageRelation> vec;
  for (int i = 0; i < (int) userUsages.size(); i++) {
    QString s;
    s.setNum(i+1);
    s.insert(0, UL_USER_USAGE);
    vec.push_back(UsageRelation(s, userUsages[i], userUsages[i]));
  }

  internalRelation_t *usage = InternalUsageRelations;
  while (usage->ident != 0) {
    QString s;
    if (usage->context == 0)
      s = i18n(usage->shortid);
    else
      s = i18n(usage->context, usage->shortid);
    vec.push_back(UsageRelation(usage->ident,
                                s,
                                i18n(usage->longId)));
    usage++;
  }

  return vec;
}


bool UsageManager::contains (const QString& label, const QString& collection)
{
   QString s = collection;
   int pos;
   while ((pos = s.find(UL_USAGE_DIV)) >= 0) {
     if (s.left(pos) == label)
       return true;
     s.remove (0, pos+1);
   }

   return s == label;
}


void UsageManager::setUsageNames (vector<QString> names)
{
  userUsages = names;
}
