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
    Revision 1.5  2001/12/29 10:40:24  arnold
    merged fixes from POST-branch

    Revision 1.4  2001/12/26 15:11:53  mueller
    CVSSILINT: fixincludes

    Revision 1.3  2001/11/25 11:11:23  arnold
    switch for inline edit, splitted kv_resource.h

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

#include <kapplication.h>
#include <klocale.h>

#include <kvoctraindoc.h>
#include <iostream.h>
#include <vector.h>

vector<QString> UsageManager::userUsages;

// user usages are strings like this: #1

struct internalRelation_t
{
  const char *ident,
             *shortId,
             *longId;
};


static internalRelation_t InternalUsageRelations [] =
{
  { UL_AM,        I18N_NOOP("Am."),      I18N_NOOP("Americanism")},
  { UL_ABBR,      I18N_NOOP("abbr."),    I18N_NOOP("abbreviation")},
  { UL_ANAT,      I18N_NOOP("anat."),    I18N_NOOP("anatomy")},
  { UL_ASTR,      I18N_NOOP("astr."),    I18N_NOOP("astronomy")},
  { UL_BIOL,      I18N_NOOP("biol."),    I18N_NOOP("biology")},
  { UL_BS,        I18N_NOOP("b.s."),     I18N_NOOP("bad sense")},
  { UL_CONTP,     I18N_NOOP("contp."),   I18N_NOOP("contemptuously")},
  { UL_ECCL,      I18N_NOOP("eccl."),    I18N_NOOP("ecclesiastical")},
  { UL_FIG,       I18N_NOOP("fig."),     I18N_NOOP("figuratively")},
  { UL_GEOL,      I18N_NOOP("geol."),    I18N_NOOP("geology")},
  { UL_HIST,      I18N_NOOP("hist."),    I18N_NOOP("historical")},
  { UL_ICHT,      I18N_NOOP("icht."),    I18N_NOOP("ichthyology")},
  { UL_IFML,      I18N_NOOP("ifml."),    I18N_NOOP("informal")},
  { UL_IRONIC,    I18N_NOOP("iro."),     I18N_NOOP("ironic")},
  { UL_IRR,       I18N_NOOP("irr."),     I18N_NOOP("irregular")},
  { UL_LIT,       I18N_NOOP("lit."),     I18N_NOOP("literary")},
  { UL_META,      I18N_NOOP("metall."),  I18N_NOOP("metallurgy")},
  { UL_METEO,     I18N_NOOP("meteor."),  I18N_NOOP("meteorology")},
  { UL_MIN,       I18N_NOOP("min."),     I18N_NOOP("mineralogy")},
  { UL_MOT,       I18N_NOOP("mot."),     I18N_NOOP("motoring")},
  { UL_MOUNT,     I18N_NOOP("mount."),   I18N_NOOP("mountaineering")},
  { UL_MYT,       I18N_NOOP("myth."),    I18N_NOOP("mythology")},
  { UL_NPR,       I18N_NOOP("npr."),     I18N_NOOP("proper name")},
  { UL_OPT,       I18N_NOOP("opt."),     I18N_NOOP("optics")},
  { UL_ORN,       I18N_NOOP("orn."),     I18N_NOOP("ornithology")},
  { UL_OS,        I18N_NOOP("o.s."),     I18N_NOOP("oneself")},
  { UL_P,         I18N_NOOP("P."),       I18N_NOOP("person")},
  { UL_PARL,      I18N_NOOP("parl."),    I18N_NOOP("parliamentary")},
  { UL_PHARM,     I18N_NOOP("pharm."),   I18N_NOOP("pharmacy")},
  { UL_PHLS,      I18N_NOOP("phls."),    I18N_NOOP("philosophy")},
  { UL_PHOT,      I18N_NOOP("phot."),    I18N_NOOP("photography")},
  { UL_PHYS,      I18N_NOOP("phys."),    I18N_NOOP("physics")},
  { UL_PHYSIOL,   I18N_NOOP("physiol."), I18N_NOOP("physiology")},
  { UL_PL,        I18N_NOOP("pl."),      I18N_NOOP("plural")},
  { UL_POET,      I18N_NOOP("poet."),    I18N_NOOP("poetry")},
  { UL_POL,       I18N_NOOP("pol."),     I18N_NOOP("politics")},
  { UL_PROV,      I18N_NOOP("prov."),    I18N_NOOP("provincialism")},
  { UL_PSYCH,     I18N_NOOP("psych."),   I18N_NOOP("psychology")},
  { UL_RHET,      I18N_NOOP("rhet."),    I18N_NOOP("rhetoric")},
  { UL_SURV,      I18N_NOOP("surv."),    I18N_NOOP("surveying")},
  { UL_TEL,       I18N_NOOP("tel."),     I18N_NOOP("telegraphy")},
  { UL_TELEPH,    I18N_NOOP("teleph."),  I18N_NOOP("telephony")},
  { UL_THEA,      I18N_NOOP("thea."),    I18N_NOOP("theatre")},
  { UL_TYP,       I18N_NOOP("typ."),     I18N_NOOP("typography")},
  { UL_UNIV,      I18N_NOOP("univ."),    I18N_NOOP("university")},
  { UL_VET,       I18N_NOOP("vet."),     I18N_NOOP("veterinary medicine")},
  { UL_ZO,        I18N_NOOP("zo."),      I18N_NOOP("zoology")},
//{ UL_,          I18N_NOOP(""),        I18N_NOOP("")},
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
