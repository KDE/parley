/***************************************************************************

    $Id$

                            manage usage labels

    -----------------------------------------------------------------------

    begin                : Fri Mar 31 20:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.2  2001/11/10 22:28:46  arnold
    removed compatibility for kde1

    Revision 1.1  2001/10/05 15:42:01  arnold
    import of version 0.7.0pre8 to kde-edu

 
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
#include "kvoctraincore.h"

#include <kapp.h>
#include <klocale.h>

#include <kvoctraindoc.h>
#include <iostream.h>
#include <vector.h>

vector<QString> UsageManager::userUsages;

#ifndef i18n_noop
# define i18n_noop(x) (x)
#endif

// user usages are strings like this: #1

struct internalRelation_t
{
  const char *ident,
             *shortId,
             *longId;
};


static internalRelation_t InternalUsageRelations [] =
{
  { UL_AM,        i18n_noop("Am."),      i18n_noop("Americanism")},
  { UL_ABBR,      i18n_noop("abbr."),    i18n_noop("abbreviation")},
  { UL_ANAT,      i18n_noop("anat."),    i18n_noop("anatomy")},
  { UL_ASTR,      i18n_noop("astr."),    i18n_noop("astronomy")},
  { UL_BIOL,      i18n_noop("biol."),    i18n_noop("biology")},
  { UL_BS,        i18n_noop("b.s."),     i18n_noop("bad sense")},
  { UL_CONTP,     i18n_noop("contp."),   i18n_noop("contemptously")},
  { UL_ECCL,      i18n_noop("eccl."),    i18n_noop("ecclesiastical")},
  { UL_FIG,       i18n_noop("fig."),     i18n_noop("figurativley")},
  { UL_GEOL,      i18n_noop("geol."),    i18n_noop("geology")},
  { UL_HIST,      i18n_noop("hist."),    i18n_noop("historical")},
  { UL_ICHT,      i18n_noop("icht."),    i18n_noop("ichthyology")},
  { UL_IFML,      i18n_noop("ifml."),    i18n_noop("informal")},
  { UL_IRONIC,    i18n_noop("iro."),     i18n_noop("ironic")},
  { UL_IRR,       i18n_noop("irr."),     i18n_noop("irregular")},
  { UL_LIT,       i18n_noop("lit."),     i18n_noop("literary")},
  { UL_META,      i18n_noop("metall."),  i18n_noop("metallurgy")},
  { UL_METEO,     i18n_noop("meteor."),  i18n_noop("meteorology")},
  { UL_MIN,       i18n_noop("min."),     i18n_noop("mineralogie")},
  { UL_MOT,       i18n_noop("mot."),     i18n_noop("motoring")},
  { UL_MOUNT,     i18n_noop("mount."),   i18n_noop("mountaineering")},
  { UL_MYT,       i18n_noop("myth."),    i18n_noop("mythology")},
  { UL_NPR,       i18n_noop("npr."),     i18n_noop("proper name")},
  { UL_OPT,       i18n_noop("opt."),     i18n_noop("optics")},
  { UL_ORN,       i18n_noop("orn."),     i18n_noop("ornithology")},
  { UL_OS,        i18n_noop("o.s."),     i18n_noop("oneself")},
  { UL_P,         i18n_noop("P."),       i18n_noop("person")},
  { UL_PARL,      i18n_noop("parl."),    i18n_noop("parliamentary")},
  { UL_PHARM,     i18n_noop("pharm."),   i18n_noop("pharmacy")},
  { UL_PHLS,      i18n_noop("phls."),    i18n_noop("philosophy")},
  { UL_PHOT,      i18n_noop("phot."),    i18n_noop("photography")},
  { UL_PHYS,      i18n_noop("phys."),    i18n_noop("physics")},
  { UL_PHYSIOL,   i18n_noop("physiol."), i18n_noop("physiology")},
  { UL_PL,        i18n_noop("pl."),      i18n_noop("plural")},
  { UL_POET,      i18n_noop("poet."),    i18n_noop("poetry")},
  { UL_POL,       i18n_noop("pol."),     i18n_noop("politics")},
  { UL_PROV,      i18n_noop("prov."),    i18n_noop("provincialism")},
  { UL_PSYCH,     i18n_noop("psych."),   i18n_noop("psychology")},
  { UL_RHET,      i18n_noop("rhet."),    i18n_noop("rhetoric")},
  { UL_SURV,      i18n_noop("surv."),    i18n_noop("surveying")},
  { UL_TEL,       i18n_noop("tel."),     i18n_noop("telegraphy")},
  { UL_TELEPH,    i18n_noop("teleph."),  i18n_noop("telephony")},
  { UL_THEA,      i18n_noop("thea."),    i18n_noop("theatre")},
  { UL_TYP,       i18n_noop("typ."),     i18n_noop("typography")},
  { UL_UNIV,      i18n_noop("univ."),    i18n_noop("university")},
  { UL_VET,       i18n_noop("vet."),     i18n_noop("veterinary medicine")},
  { UL_ZO,        i18n_noop("zo."),      i18n_noop("zoology")},
//{ UL_,          i18n_noop(""),        i18n_noop("")},
  { 0, 0, 0 } // the end
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
    vec.push_back(UsageRelation(usage->ident,
                                i18n(usage->shortId),
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
