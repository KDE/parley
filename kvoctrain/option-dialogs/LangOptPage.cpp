/***************************************************************************

    $Id$

                  language options dialog page

    -----------------------------------------------------------------------

    begin                : Thu Jun 3 18:09:06 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.14  2001/12/13 18:40:12  arnold
    added phonetic alphabet stuff

    Revision 1.13  2001/12/08 10:28:31  arnold
    added structure with iso language names

    Revision 1.12  2001/11/25 11:11:43  arnold
    switch for inline edit, splitted kv_resource.h

    Revision 1.11  2001/11/24 11:46:21  arnold
    fix for kde3, better sorting of regions

    Revision 1.10  2001/11/21 18:30:17  arnold
    quick fix for change in KLocale class

    Revision 1.9  2001/11/10 22:29:11  arnold
    removed compatibility for kde1

    Revision 1.8  2001/11/10 17:35:03  arnold
    fixed language property dialog page

    Revision 1.7  2001/11/09 10:40:46  arnold
    removed ability to display a different font for each column

    Revision 1.6  2001/11/02 04:31:42  waba
    Add include to make it work with KDE CVS.

    Revision 1.5  2001/10/25 17:34:19  arnold
    replaced qtarch dialog files by qtdesigner

    Revision 1.4  2001/10/21 15:29:50  arnold
    removed all the 'charset' stuff

    Revision 1.3  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:44:04  arnold
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


#include <qpixmap.h>
#include <qkeycode.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qpopupmenu.h>

#include <kfiledialog.h>
#include <kapp.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstddirs.h>

#include "LangOptPage.h"

#include <langset.h>
#include <kv_resource.h>
#include <kvoctraincore.h>
#include <kvoctraindoc.h>

#include <map.h>
#include <algo.h>

// automatically extracted from http://www.loc.gov/standards/iso639-2/englangn.html
// with .../tools/extractlang

struct KV_ISO639_Code {
   const char *iso1code;
   const char *iso2code;
   const char *langname;
};

// german language names at:
//    http://www.rn.inf.tu-dresden.de/scripts_lsrn/lehre/rnd/www/iso639.htm

KV_ISO639_Code kv_iso639_1[] = {
	{{"aa"}, {"aar"}, {I18N_NOOP("Afar")}},
	{{"ab"}, {"abk"}, {I18N_NOOP("Abkhazian")}},
	{{"ae"}, {"ave"}, {I18N_NOOP("Avestan")}},
	{{"af"}, {"afr"}, {I18N_NOOP("Afrikaans")}},
	{{"am"}, {"amh"}, {I18N_NOOP("Amharic")}},
	{{"ar"}, {"ara"}, {I18N_NOOP("Arabic")}},
	{{"as"}, {"asm"}, {I18N_NOOP("Assamese")}},
	{{"ay"}, {"aym"}, {I18N_NOOP("Aymara")}},
	{{"az"}, {"aze"}, {I18N_NOOP("Azerbaijani")}},
	{{"ba"}, {"bak"}, {I18N_NOOP("Bashkir")}},
	{{"be"}, {"bel"}, {I18N_NOOP("Belarusian")}},
	{{"bg"}, {"bul"}, {I18N_NOOP("Bulgarian")}},
	{{"bh"}, {"bih"}, {I18N_NOOP("Bihari")}},
	{{"bi"}, {"bis"}, {I18N_NOOP("Bislama")}},
	{{"bn"}, {"ben"}, {I18N_NOOP("Bengali")}},
	{{"bo"}, {"tib"}, {I18N_NOOP("Tibetan")}},
	{{"br"}, {"bre"}, {I18N_NOOP("Breton")}},
	{{"bs"}, {"bos"}, {I18N_NOOP("Bosnian")}},
	{{"ca"}, {"cat"}, {I18N_NOOP("Catalan")}},
	{{"ce"}, {"che"}, {I18N_NOOP("Chechen")}},
	{{"ch"}, {"cha"}, {I18N_NOOP("Chamorro")}},
	{{"co"}, {"cos"}, {I18N_NOOP("Corsican")}},
	{{"cs"}, {"cze"}, {I18N_NOOP("Czech")}},
	{{"cu"}, {"chu"}, {I18N_NOOP("Church Slavic")}},
	{{"cv"}, {"chv"}, {I18N_NOOP("Chuvash")}},
	{{"cy"}, {"wel"}, {I18N_NOOP("Welsh")}},
	{{"da"}, {"dan"}, {I18N_NOOP("Danish")}},
	{{"de"}, {"ger"}, {I18N_NOOP("German")}},
	{{"dz"}, {"dzo"}, {I18N_NOOP("Dzongkha")}},
	{{"el"}, {"gre"}, {I18N_NOOP("Greek")}},
	{{"en"}, {"eng"}, {I18N_NOOP("English")}},
	{{"eo"}, {"epo"}, {I18N_NOOP("Esperanto")}},
	{{"es"}, {"spa"}, {I18N_NOOP("Spanish")}},
	{{"et"}, {"est"}, {I18N_NOOP("Estonian")}},
	{{"eu"}, {"baq"}, {I18N_NOOP("Basque")}},
	{{"fa"}, {"per"}, {I18N_NOOP("Persian")}},
	{{"fi"}, {"fin"}, {I18N_NOOP("Finnish")}},
	{{"fj"}, {"fij"}, {I18N_NOOP("Fijian")}},
	{{"fo"}, {"fao"}, {I18N_NOOP("Faroese")}},
	{{"fr"}, {"fre"}, {I18N_NOOP("French")}},
	{{"fy"}, {"fry"}, {I18N_NOOP("Frisian")}},
	{{"ga"}, {"gle"}, {I18N_NOOP("Irish")}},
	{{"gd"}, {"gla"}, {I18N_NOOP("Gaelic")}},
	{{"gd"}, {"gla"}, {I18N_NOOP("Scottish Gaelic")}},
	{{"gl"}, {"glg"}, {I18N_NOOP("Gallegan")}},
	{{"gn"}, {"grn"}, {I18N_NOOP("Guarani")}},
	{{"gu"}, {"guj"}, {I18N_NOOP("Gujarati")}},
	{{"gv"}, {"glv"}, {I18N_NOOP("Manx")}},
	{{"ha"}, {"hau"}, {I18N_NOOP("Hausa")}},
	{{"he"}, {"heb"}, {I18N_NOOP("Hebrew")}},
	{{"hi"}, {"hin"}, {I18N_NOOP("Hindi")}},
	{{"ho"}, {"hmo"}, {I18N_NOOP("Hiri Motu")}},
	{{"hr"}, {"scr"}, {I18N_NOOP("Croatian")}},
	{{"hu"}, {"hun"}, {I18N_NOOP("Hungarian")}},
	{{"hy"}, {"arm"}, {I18N_NOOP("Armenian")}},
	{{"hz"}, {"her"}, {I18N_NOOP("Herero")}},
	{{"id"}, {"ind"}, {I18N_NOOP("Indonesian")}},
	{{"ie"}, {"ile"}, {I18N_NOOP("Interlingue")}},
	{{"ik"}, {"ipk"}, {I18N_NOOP("Inupiaq")}},
	{{"is"}, {"ice"}, {I18N_NOOP("Icelandic")}},
	{{"it"}, {"ita"}, {I18N_NOOP("Italian")}},
	{{"iu"}, {"iku"}, {I18N_NOOP("Inuktitut")}},
	{{"ja"}, {"jpn"}, {I18N_NOOP("Japanese")}},
	{{"jv"}, {"jav"}, {I18N_NOOP("Javanese")}},
	{{"ka"}, {"geo"}, {I18N_NOOP("Georgian")}},
	{{"ki"}, {"kik"}, {I18N_NOOP("Kikuyu")}},
	{{"kj"}, {"kua"}, {I18N_NOOP("Kuanyama")}},
	{{"kk"}, {"kaz"}, {I18N_NOOP("Kazakh")}},
	{{"kl"}, {"kal"}, {I18N_NOOP("Kalaallisut")}},
	{{"km"}, {"khm"}, {I18N_NOOP("Khmer")}},
	{{"kn"}, {"kan"}, {I18N_NOOP("Kannada")}},
	{{"ko"}, {"kor"}, {I18N_NOOP("Korean")}},
	{{"ks"}, {"kas"}, {I18N_NOOP("Kashmiri")}},
	{{"ku"}, {"kur"}, {I18N_NOOP("Kurdish")}},
	{{"kv"}, {"kom"}, {I18N_NOOP("Komi")}},
	{{"kw"}, {"cor"}, {I18N_NOOP("Cornish")}},
	{{"ky"}, {"kir"}, {I18N_NOOP("Kirghiz")}},
	{{"la"}, {"lat"}, {I18N_NOOP("Latin")}},
	{{"lb"}, {"ltz"}, {I18N_NOOP("Letzeburgesch")}},
	{{"ln"}, {"lin"}, {I18N_NOOP("Lingala")}},
	{{"lo"}, {"lao"}, {I18N_NOOP("Lao")}},
	{{"lt"}, {"lit"}, {I18N_NOOP("Lithuanian")}},
	{{"lv"}, {"lav"}, {I18N_NOOP("Latvian")}},
	{{"mg"}, {"mlg"}, {I18N_NOOP("Malagasy")}},
	{{"mh"}, {"mah"}, {I18N_NOOP("Marshall")}},
	{{"mi"}, {"mao"}, {I18N_NOOP("Maori")}},
	{{"mk"}, {"mac"}, {I18N_NOOP("Macedonian")}},
	{{"ml"}, {"mal"}, {I18N_NOOP("Malayalam")}},
	{{"mn"}, {"mon"}, {I18N_NOOP("Mongolian")}},
	{{"mo"}, {"mol"}, {I18N_NOOP("Moldavian")}},
	{{"mr"}, {"mar"}, {I18N_NOOP("Marathi")}},
	{{"ms"}, {"may"}, {I18N_NOOP("Malay")}},
	{{"mt"}, {"mlt"}, {I18N_NOOP("Maltese")}},
	{{"my"}, {"bur"}, {I18N_NOOP("Burmese")}},
	{{"na"}, {"nau"}, {I18N_NOOP("Nauru")}},
	{{"nb"}, {"nob"}, {I18N_NOOP("Bokmål")}},
	{{"nd"}, {"nde"}, {I18N_NOOP("Ndebele, North")}},
	{{"ne"}, {"nep"}, {I18N_NOOP("Nepali")}},
	{{"ng"}, {"ndo"}, {I18N_NOOP("Ndonga")}},
	{{"nl"}, {"dut"}, {I18N_NOOP("Dutch")}},
	{{"nn"}, {"nno"}, {I18N_NOOP("Norwegian Nynorsk")}},
	{{"no"}, {"nor"}, {I18N_NOOP("Norwegian")}},
	{{"nr"}, {"nbl"}, {I18N_NOOP("Ndebele, South")}},
	{{"nv"}, {"nav"}, {I18N_NOOP("Navajo")}},
	{{"ny"}, {"nya"}, {I18N_NOOP("Chichewa")}},
	{{"ny"}, {"nya"}, {I18N_NOOP("Nyanja; Chichewa")}},
	{{"oc"}, {"oci"}, {I18N_NOOP("Occitan")}},
	{{"oc"}, {"oci"}, {I18N_NOOP("Provencal")}},
	{{"om"}, {"orm"}, {I18N_NOOP("Oromo")}},
	{{"or"}, {"ori"}, {I18N_NOOP("Oriya")}},
	{{"os"}, {"oss"}, {I18N_NOOP("Ossetic")}},
	{{"pa"}, {"pan"}, {I18N_NOOP("Panjabi")}},
	{{"pi"}, {"pli"}, {I18N_NOOP("Pali")}},
	{{"pl"}, {"pol"}, {I18N_NOOP("Polish")}},
	{{"ps"}, {"pus"}, {I18N_NOOP("Pushto")}},
	{{"pt"}, {"por"}, {I18N_NOOP("Portuguese")}},
	{{"qu"}, {"que"}, {I18N_NOOP("Quechua")}},
	{{"rm"}, {"roh"}, {I18N_NOOP("Raeto-Romance")}},
	{{"rn"}, {"run"}, {I18N_NOOP("Rundi")}},
	{{"ro"}, {"rum"}, {I18N_NOOP("Romanian")}},
	{{"ru"}, {"rus"}, {I18N_NOOP("Russian")}},
	{{"rw"}, {"kin"}, {I18N_NOOP("Kinyarwanda")}},
	{{"sa"}, {"san"}, {I18N_NOOP("Sanskrit")}},
	{{"sc"}, {"srd"}, {I18N_NOOP("Sardinian")}},
	{{"sd"}, {"snd"}, {I18N_NOOP("Sindhi")}},
	{{"se"}, {"sme"}, {I18N_NOOP("Northern Sami")}},
	{{"sg"}, {"sag"}, {I18N_NOOP("Sango")}},
	{{"si"}, {"sin"}, {I18N_NOOP("Sinhalese")}},
	{{"sk"}, {"slo"}, {I18N_NOOP("Slovak")}},
	{{"sl"}, {"slv"}, {I18N_NOOP("Slovenian")}},
	{{"sm"}, {"smo"}, {I18N_NOOP("Samoan")}},
	{{"sn"}, {"sna"}, {I18N_NOOP("Shona")}},
	{{"so"}, {"som"}, {I18N_NOOP("Somali")}},
	{{"sq"}, {"alb"}, {I18N_NOOP("Albanian")}},
	{{"sr"}, {"scc"}, {I18N_NOOP("Serbian")}},
	{{"ss"}, {"ssw"}, {I18N_NOOP("Swati")}},
	{{"st"}, {"sot"}, {I18N_NOOP("Sotho, Southern")}},
	{{"su"}, {"sun"}, {I18N_NOOP("Sundanese")}},
	{{"sv"}, {"swe"}, {I18N_NOOP("Swedish")}},
	{{"sw"}, {"swa"}, {I18N_NOOP("Swahili")}},
	{{"ta"}, {"tam"}, {I18N_NOOP("Tamil")}},
	{{"te"}, {"tel"}, {I18N_NOOP("Telugu")}},
	{{"tg"}, {"tgk"}, {I18N_NOOP("Tajik")}},
	{{"th"}, {"tha"}, {I18N_NOOP("Thai")}},
	{{"ti"}, {"tir"}, {I18N_NOOP("Tigrinya")}},
	{{"tk"}, {"tuk"}, {I18N_NOOP("Turkmen")}},
	{{"tl"}, {"tgl"}, {I18N_NOOP("Tagalog")}},
	{{"tn"}, {"tsn"}, {I18N_NOOP("Tswana")}},
	{{"to"}, {"ton"}, {I18N_NOOP("Tonga")}},
	{{"tr"}, {"tur"}, {I18N_NOOP("Turkish")}},
	{{"ts"}, {"tso"}, {I18N_NOOP("Tsonga")}},
	{{"tt"}, {"tat"}, {I18N_NOOP("Tatar")}},
	{{"tw"}, {"twi"}, {I18N_NOOP("Twi")}},
	{{"ty"}, {"tah"}, {I18N_NOOP("Tahitian")}},
	{{"ug"}, {"uig"}, {I18N_NOOP("Uighur")}},
	{{"uk"}, {"ukr"}, {I18N_NOOP("Ukrainian")}},
	{{"ur"}, {"urd"}, {I18N_NOOP("Urdu")}},
	{{"uz"}, {"uzb"}, {I18N_NOOP("Uzbek")}},
	{{"vi"}, {"vie"}, {I18N_NOOP("Vietnamese")}},
	{{"vo"}, {"vol"}, {I18N_NOOP("Volap&#252;k")}},
	{{"wo"}, {"wol"}, {I18N_NOOP("Wolof")}},
	{{"xh"}, {"xho"}, {I18N_NOOP("Xhosa")}},
	{{"yi"}, {"yid"}, {I18N_NOOP("Yiddish")}},
	{{"yo"}, {"yor"}, {I18N_NOOP("Yoruba")}},
	{{"za"}, {"zha"}, {I18N_NOOP("Zhuang")}},
	{{"zh"}, {"chi"}, {I18N_NOOP("Chinese")}},
	{{"zu"}, {"zul"}, {I18N_NOOP("Zulu")}},
	{{0}, {0}, {0}}
};

/* only when really needed

KV_ISO639_Code kv_iso639_2[] = {
	{{"ab"}, {"abk"}, {I18N_NOOP("Abkhazian")}},
	{{""}, {"ace"}, {I18N_NOOP("Achinese")}},
	{{""}, {"ach"}, {I18N_NOOP("Acoli")}},
	{{""}, {"ada"}, {I18N_NOOP("Adangme")}},
	{{"aa"}, {"aar"}, {I18N_NOOP("Afar")}},
	{{""}, {"afh"}, {I18N_NOOP("Afrihili")}},
	{{"af"}, {"afr"}, {I18N_NOOP("Afrikaans")}},
	{{""}, {"afa"}, {I18N_NOOP("Afro-Asiatic (Other)")}},
	{{""}, {"aka"}, {I18N_NOOP("Akan")}},
	{{""}, {"akk"}, {I18N_NOOP("Akkadian")}},
	{{"sq"}, {"alb"}, {I18N_NOOP("Albanian")}},
	{{""}, {"ale"}, {I18N_NOOP("Aleut")}},
	{{""}, {"alg"}, {I18N_NOOP("Algonquian languages")}},
	{{""}, {"tut"}, {I18N_NOOP("Altaic (Other)")}},
	{{"am"}, {"amh"}, {I18N_NOOP("Amharic")}},
	{{""}, {"apa"}, {I18N_NOOP("Apache languages")}},
	{{"ar"}, {"ara"}, {I18N_NOOP("Arabic")}},
	{{""}, {"arc"}, {I18N_NOOP("Aramaic")}},
	{{""}, {"arp"}, {I18N_NOOP("Arapaho")}},
	{{""}, {"arn"}, {I18N_NOOP("Araucanian")}},
	{{""}, {"arw"}, {I18N_NOOP("Arawak")}},
	{{"hy"}, {"arm"}, {I18N_NOOP("Armenian")}},
	{{""}, {"art"}, {I18N_NOOP("Artificial (Other)")}},
	{{"as"}, {"asm"}, {I18N_NOOP("Assamese")}},
	{{""}, {"ath"}, {I18N_NOOP("Athapascan languages")}},
	{{""}, {"aus"}, {I18N_NOOP("Australian languages")}},
	{{""}, {"map"}, {I18N_NOOP("Austronesian (Other)")}},
	{{""}, {"ava"}, {I18N_NOOP("Avaric")}},
	{{"ae"}, {"ave"}, {I18N_NOOP("Avestan")}},
	{{""}, {"awa"}, {I18N_NOOP("Awadhi")}},
	{{"ay"}, {"aym"}, {I18N_NOOP("Aymara")}},
	{{"az"}, {"aze"}, {I18N_NOOP("Azerbaijani")}},
	{{""}, {"ban"}, {I18N_NOOP("Balinese")}},
	{{""}, {"bat"}, {I18N_NOOP("Baltic (Other)")}},
	{{""}, {"bal"}, {I18N_NOOP("Baluchi")}},
	{{""}, {"bam"}, {I18N_NOOP("Bambara")}},
	{{""}, {"bai"}, {I18N_NOOP("Bamileke languages")}},
	{{""}, {"bad"}, {I18N_NOOP("Banda")}},
	{{""}, {"bnt"}, {I18N_NOOP("Bantu (Other)")}},
	{{""}, {"bas"}, {I18N_NOOP("Basa")}},
	{{"ba"}, {"bak"}, {I18N_NOOP("Bashkir")}},
	{{"eu"}, {"baq"}, {I18N_NOOP("Basque")}},
	{{""}, {"btk"}, {I18N_NOOP("Batak (Indonesia)")}},
	{{""}, {"bej"}, {I18N_NOOP("Beja")}},
	{{"be"}, {"bel"}, {I18N_NOOP("Belarusian")}},
	{{""}, {"bem"}, {I18N_NOOP("Bemba")}},
	{{"bn"}, {"ben"}, {I18N_NOOP("Bengali")}},
	{{""}, {"ber"}, {I18N_NOOP("Berber (Other)")}},
	{{""}, {"bho"}, {I18N_NOOP("Bhojpuri")}},
	{{"bh"}, {"bih"}, {I18N_NOOP("Bihari")}},
	{{""}, {"bik"}, {I18N_NOOP("Bikol")}},
	{{""}, {"bin"}, {I18N_NOOP("Bini")}},
	{{"bi"}, {"bis"}, {I18N_NOOP("Bislama")}},
	{{"nb"}, {"nob"}, {I18N_NOOP("Bokm&aring;l, Norwegian; Norwegian Bokm&aring;l")}},
	{{"bs"}, {"bos"}, {I18N_NOOP("Bosnian")}},
	{{""}, {"bra"}, {I18N_NOOP("Braj")}},
	{{"br"}, {"bre"}, {I18N_NOOP("Breton")}},
	{{""}, {"bug"}, {I18N_NOOP("Buginese")}},
	{{"bg"}, {"bul"}, {I18N_NOOP("Bulgarian")}},
	{{""}, {"bua"}, {I18N_NOOP("Buriat")}},
	{{"my"}, {"bur"}, {I18N_NOOP("Burmese")}},
	{{""}, {"cad"}, {I18N_NOOP("Caddo")}},
	{{""}, {"car"}, {I18N_NOOP("Carib")}},
	{{"es"}, {"spa"}, {I18N_NOOP("Castilian; Spanish")}},
	{{"ca"}, {"cat"}, {I18N_NOOP("Catalan")}},
	{{""}, {"cau"}, {I18N_NOOP("Caucasian (Other)")}},
	{{""}, {"ceb"}, {I18N_NOOP("Cebuano")}},
	{{""}, {"cel"}, {I18N_NOOP("Celtic (Other)")}},
	{{""}, {"chg"}, {I18N_NOOP("Chagatai")}},
	{{""}, {"cmc"}, {I18N_NOOP("Chamic languages")}},
	{{"ch"}, {"cha"}, {I18N_NOOP("Chamorro")}},
	{{"ce"}, {"che"}, {I18N_NOOP("Chechen")}},
	{{""}, {"chr"}, {I18N_NOOP("Cherokee")}},
	{{""}, {"chy"}, {I18N_NOOP("Cheyenne")}},
	{{""}, {"chb"}, {I18N_NOOP("Chibcha")}},
	{{"ny"}, {"nya"}, {I18N_NOOP("Chichewa; Nyanja")}},
	{{"zh"}, {"chi"}, {I18N_NOOP("Chinese")}},
	{{""}, {"chn"}, {I18N_NOOP("Chinook jargon")}},
	{{""}, {"chp"}, {I18N_NOOP("Chipewyan")}},
	{{""}, {"cho"}, {I18N_NOOP("Choctaw")}},
	{{"za"}, {"zha"}, {I18N_NOOP("Chuang; Zhuang")}},
	{{"cu"}, {"chu"}, {I18N_NOOP("Church Slavic")}},
	{{""}, {"chk"}, {I18N_NOOP("Chuukese")}},
	{{"cv"}, {"chv"}, {I18N_NOOP("Chuvash")}},
	{{""}, {"cop"}, {I18N_NOOP("Coptic")}},
	{{"kw"}, {"cor"}, {I18N_NOOP("Cornish")}},
	{{"co"}, {"cos"}, {I18N_NOOP("Corsican")}},
	{{""}, {"cre"}, {I18N_NOOP("Cree")}},
	{{""}, {"mus"}, {I18N_NOOP("Creek")}},
	{{""}, {"crp"}, {I18N_NOOP("Creoles and pidgins (Other)")}},
	{{"hr"}, {"scr"}, {I18N_NOOP("Croatian")}},
	{{""}, {"cus"}, {I18N_NOOP("Cushitic (Other)")}},
	{{"cs"}, {"cze"}, {I18N_NOOP("Czech")}},
	{{""}, {"dak"}, {I18N_NOOP("Dakota")}},
	{{"da"}, {"dan"}, {I18N_NOOP("Danish")}},
	{{""}, {"day"}, {I18N_NOOP("Dayak")}},
	{{""}, {"del"}, {I18N_NOOP("Delaware")}},
	{{""}, {"din"}, {I18N_NOOP("Dinka")}},
	{{""}, {"div"}, {I18N_NOOP("Divehi")}},
	{{""}, {"doi"}, {I18N_NOOP("Dogri")}},
	{{""}, {"dgr"}, {I18N_NOOP("Dogrib")}},
	{{""}, {"dra"}, {I18N_NOOP("Dravidian (Other)")}},
	{{""}, {"dua"}, {I18N_NOOP("Duala")}},
	{{"nl"}, {"dut"}, {I18N_NOOP("Dutch")}},
	{{""}, {"dum"}, {I18N_NOOP("Dutch, Middle (ca. 1050-1350)")}},
	{{""}, {"dyu"}, {I18N_NOOP("Dyula")}},
	{{"dz"}, {"dzo"}, {I18N_NOOP("Dzongkha")}},
	{{""}, {"efi"}, {I18N_NOOP("Efik")}},
	{{""}, {"egy"}, {I18N_NOOP("Egyptian (Ancient)")}},
	{{""}, {"eka"}, {I18N_NOOP("Ekajuk")}},
	{{""}, {"elx"}, {I18N_NOOP("Elamite")}},
	{{"en"}, {"eng"}, {I18N_NOOP("English")}},
	{{""}, {"enm"}, {I18N_NOOP("English, Middle (1100-1500)")}},
	{{""}, {"ang"}, {I18N_NOOP("English, Old (ca.450-1100)")}},
	{{"eo"}, {"epo"}, {I18N_NOOP("Esperanto")}},
	{{"et"}, {"est"}, {I18N_NOOP("Estonian")}},
	{{""}, {"ewe"}, {I18N_NOOP("Ewe")}},
	{{""}, {"ewo"}, {I18N_NOOP("Ewondo")}},
	{{""}, {"fan"}, {I18N_NOOP("Fang")}},
	{{""}, {"fat"}, {I18N_NOOP("Fanti")}},
	{{"fo"}, {"fao"}, {I18N_NOOP("Faroese")}},
	{{"fj"}, {"fij"}, {I18N_NOOP("Fijian")}},
	{{"fi"}, {"fin"}, {I18N_NOOP("Finnish")}},
	{{""}, {"fiu"}, {I18N_NOOP("Finno-Ugrian (Other)")}},
	{{""}, {"fon"}, {I18N_NOOP("Fon")}},
	{{"fr"}, {"fre"}, {I18N_NOOP("French")}},
	{{""}, {"frm"}, {I18N_NOOP("French, Middle (ca.1400-1600)")}},
	{{""}, {"fro"}, {I18N_NOOP("French, Old (842-ca.1400)")}},
	{{"fy"}, {"fry"}, {I18N_NOOP("Frisian")}},
	{{""}, {"fur"}, {I18N_NOOP("Friulian")}},
	{{""}, {"ful"}, {I18N_NOOP("Fulah")}},
	{{""}, {"gaa"}, {I18N_NOOP("Ga")}},
	{{"gd"}, {"gla"}, {I18N_NOOP("Gaelic; Scottish Gaelic")}},
	{{"gl"}, {"glg"}, {I18N_NOOP("Gallegan")}},
	{{""}, {"lug"}, {I18N_NOOP("Ganda")}},
	{{""}, {"gay"}, {I18N_NOOP("Gayo")}},
	{{""}, {"gba"}, {I18N_NOOP("Gbaya")}},
	{{""}, {"gez"}, {I18N_NOOP("Geez")}},
	{{"ka"}, {"geo"}, {I18N_NOOP("Georgian")}},
	{{"de"}, {"ger"}, {I18N_NOOP("German")}},
	{{""}, {"nds"}, {I18N_NOOP("German, Low; Saxon, Low; Low German; Low Saxon")}},
	{{""}, {"gmh"}, {I18N_NOOP("German, Middle High (ca.1050-1500)")}},
	{{""}, {"goh"}, {I18N_NOOP("German, Old High (ca.750-1050)")}},
	{{""}, {"gem"}, {I18N_NOOP("Germanic (Other)")}},
	{{"ki"}, {"kik"}, {I18N_NOOP("Gikuyu; Kikuyu")}},
	{{""}, {"gil"}, {I18N_NOOP("Gilbertese")}},
	{{""}, {"gon"}, {I18N_NOOP("Gondi")}},
	{{""}, {"gor"}, {I18N_NOOP("Gorontalo")}},
	{{""}, {"got"}, {I18N_NOOP("Gothic")}},
	{{""}, {"grb"}, {I18N_NOOP("Grebo")}},
	{{""}, {"grc"}, {I18N_NOOP("Greek, Ancient (to 1453)")}},
	{{"el"}, {"gre"}, {I18N_NOOP("Greek, Modern (1453-)")}},
	{{"gn"}, {"grn"}, {I18N_NOOP("Guarani")}},
	{{"gu"}, {"guj"}, {I18N_NOOP("Gujarati")}},
	{{""}, {"gwi"}, {I18N_NOOP("Gwich&#180;in")}},
	{{""}, {"hai"}, {I18N_NOOP("Haida")}},
	{{"ha"}, {"hau"}, {I18N_NOOP("Hausa")}},
	{{""}, {"haw"}, {I18N_NOOP("Hawaiian")}},
	{{"he"}, {"heb"}, {I18N_NOOP("Hebrew")}},
	{{"hz"}, {"her"}, {I18N_NOOP("Herero")}},
	{{""}, {"hil"}, {I18N_NOOP("Hiligaynon")}},
	{{""}, {"him"}, {I18N_NOOP("Himachali")}},
	{{"hi"}, {"hin"}, {I18N_NOOP("Hindi")}},
	{{"ho"}, {"hmo"}, {I18N_NOOP("Hiri Motu")}},
	{{""}, {"hit"}, {I18N_NOOP("Hittite")}},
	{{""}, {"hmn"}, {I18N_NOOP("Hmong")}},
	{{"hu"}, {"hun"}, {I18N_NOOP("Hungarian")}},
	{{""}, {"hup"}, {I18N_NOOP("Hupa")}},
	{{""}, {"iba"}, {I18N_NOOP("Iban")}},
	{{"is"}, {"ice"}, {I18N_NOOP("Icelandic")}},
	{{""}, {"ibo"}, {I18N_NOOP("Igbo")}},
	{{""}, {"ijo"}, {I18N_NOOP("Ijo")}},
	{{""}, {"ilo"}, {I18N_NOOP("Iloko")}},
	{{""}, {"inc"}, {I18N_NOOP("Indic (Other)")}},
	{{""}, {"ine"}, {I18N_NOOP("Indo-European (Other)")}},
	{{"id"}, {"ind"}, {I18N_NOOP("Indonesian")}},
	{{"ie"}, {"ile"}, {I18N_NOOP("Interlingue")}},
	{{"iu"}, {"iku"}, {I18N_NOOP("Inuktitut")}},
	{{"ik"}, {"ipk"}, {I18N_NOOP("Inupiaq")}},
	{{""}, {"ira"}, {I18N_NOOP("Iranian (Other)")}},
	{{"ga"}, {"gle"}, {I18N_NOOP("Irish")}},
	{{""}, {"mga"}, {I18N_NOOP("Irish, Middle (900-1200)")}},
	{{""}, {"sga"}, {I18N_NOOP("Irish, Old (to 900)")}},
	{{""}, {"iro"}, {I18N_NOOP("Iroquoian languages")}},
	{{"it"}, {"ita"}, {I18N_NOOP("Italian")}},
	{{"ja"}, {"jpn"}, {I18N_NOOP("Japanese")}},
	{{"jv"}, {"jav"}, {I18N_NOOP("Javanese")}},
	{{""}, {"jrb"}, {I18N_NOOP("Judeo-Arabic")}},
	{{""}, {"jpr"}, {I18N_NOOP("Judeo-Persian")}},
	{{""}, {"kab"}, {I18N_NOOP("Kabyle")}},
	{{""}, {"kac"}, {I18N_NOOP("Kachin")}},
	{{"kl"}, {"kal"}, {I18N_NOOP("Kalaallisut")}},
	{{""}, {"kam"}, {I18N_NOOP("Kamba")}},
	{{"kn"}, {"kan"}, {I18N_NOOP("Kannada")}},
	{{""}, {"kau"}, {I18N_NOOP("Kanuri")}},
	{{""}, {"kaa"}, {I18N_NOOP("Kara-Kalpak")}},
	{{""}, {"kar"}, {I18N_NOOP("Karen")}},
	{{"ks"}, {"kas"}, {I18N_NOOP("Kashmiri")}},
	{{""}, {"kaw"}, {I18N_NOOP("Kawi")}},
	{{"kk"}, {"kaz"}, {I18N_NOOP("Kazakh")}},
	{{""}, {"kha"}, {I18N_NOOP("Khasi")}},
	{{"km"}, {"khm"}, {I18N_NOOP("Khmer")}},
	{{""}, {"khi"}, {I18N_NOOP("Khoisan (Other)")}},
	{{""}, {"kho"}, {I18N_NOOP("Khotanese")}},
	{{"ki"}, {"kik"}, {I18N_NOOP("Kikuyu; Gikuyu")}},
	{{""}, {"kmb"}, {I18N_NOOP("Kimbundu")}},
	{{"rw"}, {"kin"}, {I18N_NOOP("Kinyarwanda")}},
	{{"ky"}, {"kir"}, {I18N_NOOP("Kirghiz")}},
	{{"kv"}, {"kom"}, {I18N_NOOP("Komi")}},
	{{""}, {"kon"}, {I18N_NOOP("Kongo")}},
	{{""}, {"kok"}, {I18N_NOOP("Konkani")}},
	{{"ko"}, {"kor"}, {I18N_NOOP("Korean")}},
	{{""}, {"kos"}, {I18N_NOOP("Kosraean")}},
	{{""}, {"kpe"}, {I18N_NOOP("Kpelle")}},
	{{""}, {"kro"}, {I18N_NOOP("Kru")}},
	{{"kj"}, {"kua"}, {I18N_NOOP("Kuanyama")}},
	{{""}, {"kum"}, {I18N_NOOP("Kumyk")}},
	{{"ku"}, {"kur"}, {I18N_NOOP("Kurdish")}},
	{{""}, {"kru"}, {I18N_NOOP("Kurukh")}},
	{{""}, {"kut"}, {I18N_NOOP("Kutenai")}},
	{{""}, {"lad"}, {I18N_NOOP("Ladino")}},
	{{""}, {"lah"}, {I18N_NOOP("Lahnda")}},
	{{""}, {"lam"}, {I18N_NOOP("Lamba")}},
	{{"lo"}, {"lao"}, {I18N_NOOP("Lao")}},
	{{"la"}, {"lat"}, {I18N_NOOP("Latin")}},
	{{"lv"}, {"lav"}, {I18N_NOOP("Latvian")}},
	{{"lb"}, {"ltz"}, {I18N_NOOP("Letzeburgesch")}},
	{{""}, {"lez"}, {I18N_NOOP("Lezghian")}},
	{{"ln"}, {"lin"}, {I18N_NOOP("Lingala")}},
	{{"lt"}, {"lit"}, {I18N_NOOP("Lithuanian")}},
	{{""}, {"nds"}, {I18N_NOOP("Low German; Low Saxon; German, Low; Saxon, Low")}},
	{{""}, {"nds"}, {I18N_NOOP("Low Saxon; Low German; Saxon, Low; German, Low")}},
	{{""}, {"loz"}, {I18N_NOOP("Lozi")}},
	{{""}, {"lub"}, {I18N_NOOP("Luba-Katanga")}},
	{{""}, {"lua"}, {I18N_NOOP("Luba-Lulua")}},
	{{""}, {"lui"}, {I18N_NOOP("Luiseno")}},
	{{""}, {"lun"}, {I18N_NOOP("Lunda")}},
	{{""}, {"luo"}, {I18N_NOOP("Luo (Kenya and Tanzania)")}},
	{{""}, {"lus"}, {I18N_NOOP("Lushai")}},
	{{"mk"}, {"mac"}, {I18N_NOOP("Macedonian")}},
	{{""}, {"mad"}, {I18N_NOOP("Madurese")}},
	{{""}, {"mag"}, {I18N_NOOP("Magahi")}},
	{{""}, {"mai"}, {I18N_NOOP("Maithili")}},
	{{""}, {"mak"}, {I18N_NOOP("Makasar")}},
	{{"mg"}, {"mlg"}, {I18N_NOOP("Malagasy")}},
	{{"ms"}, {"may"}, {I18N_NOOP("Malay")}},
	{{"ml"}, {"mal"}, {I18N_NOOP("Malayalam")}},
	{{"mt"}, {"mlt"}, {I18N_NOOP("Maltese")}},
	{{""}, {"mnc"}, {I18N_NOOP("Manchu")}},
	{{""}, {"mdr"}, {I18N_NOOP("Mandar")}},
	{{""}, {"man"}, {I18N_NOOP("Mandingo")}},
	{{""}, {"mni"}, {I18N_NOOP("Manipuri")}},
	{{""}, {"mno"}, {I18N_NOOP("Manobo languages")}},
	{{"gv"}, {"glv"}, {I18N_NOOP("Manx")}},
	{{"mi"}, {"mao"}, {I18N_NOOP("Maori")}},
	{{"mr"}, {"mar"}, {I18N_NOOP("Marathi")}},
	{{""}, {"chm"}, {I18N_NOOP("Mari")}},
	{{"mh"}, {"mah"}, {I18N_NOOP("Marshall")}},
	{{""}, {"mwr"}, {I18N_NOOP("Marwari")}},
	{{""}, {"mas"}, {I18N_NOOP("Masai")}},
	{{""}, {"myn"}, {I18N_NOOP("Mayan languages")}},
	{{""}, {"men"}, {I18N_NOOP("Mende")}},
	{{""}, {"mic"}, {I18N_NOOP("Micmac")}},
	{{""}, {"min"}, {I18N_NOOP("Minangkabau")}},
	{{""}, {"mis"}, {I18N_NOOP("Miscellaneous languages")}},
	{{""}, {"moh"}, {I18N_NOOP("Mohawk")}},
	{{"mo"}, {"mol"}, {I18N_NOOP("Moldavian")}},
	{{""}, {"mkh"}, {I18N_NOOP("Mon-Khmer (Other)")}},
	{{""}, {"lol"}, {I18N_NOOP("Mongo")}},
	{{"mn"}, {"mon"}, {I18N_NOOP("Mongolian")}},
	{{""}, {"mos"}, {I18N_NOOP("Mossi")}},
	{{""}, {"mul"}, {I18N_NOOP("Multiple languages")}},
	{{""}, {"mun"}, {I18N_NOOP("Munda languages")}},
	{{""}, {"nah"}, {I18N_NOOP("Nahuatl")}},
	{{"na"}, {"nau"}, {I18N_NOOP("Nauru")}},
	{{"nv"}, {"nav"}, {I18N_NOOP("Navajo")}},
	{{"nd"}, {"nde"}, {I18N_NOOP("Ndebele, North")}},
	{{"nr"}, {"nbl"}, {I18N_NOOP("Ndebele, South")}},
	{{"ng"}, {"ndo"}, {I18N_NOOP("Ndonga")}},
	{{"ne"}, {"nep"}, {I18N_NOOP("Nepali")}},
	{{""}, {"new"}, {I18N_NOOP("Newari")}},
	{{""}, {"nia"}, {I18N_NOOP("Nias")}},
	{{""}, {"nic"}, {I18N_NOOP("Niger-Kordofanian (Other)")}},
	{{""}, {"ssa"}, {I18N_NOOP("Nilo-Saharan (Other)")}},
	{{""}, {"niu"}, {I18N_NOOP("Niuean")}},
	{{""}, {"non"}, {I18N_NOOP("Norse, Old")}},
	{{""}, {"nai"}, {I18N_NOOP("North American Indian (Other)")}},
	{{"se"}, {"sme"}, {I18N_NOOP("Northern Sami")}},
	{{"no"}, {"nor"}, {I18N_NOOP("Norwegian")}},
	{{"nb"}, {"nob"}, {I18N_NOOP("Norwegian Bokm&aring;l; Bokm&aring;l, Norwegian")}},
	{{"nn"}, {"nno"}, {I18N_NOOP("Norwegian Nynorsk; Nynorsk, Norwegian")}},
	{{""}, {"nub"}, {I18N_NOOP("Nubian languages")}},
	{{""}, {"nym"}, {I18N_NOOP("Nyamwezi")}},
	{{"ny"}, {"nya"}, {I18N_NOOP("Nyanja; Chichewa")}},
	{{""}, {"nyn"}, {I18N_NOOP("Nyankole")}},
	{{"nn"}, {"nno"}, {I18N_NOOP("Nynorsk, Norwegian; Norwegian Nynorsk")}},
	{{""}, {"nyo"}, {I18N_NOOP("Nyoro")}},
	{{""}, {"nzi"}, {I18N_NOOP("Nzima")}},
	{{"oc"}, {"oci"}, {I18N_NOOP("Occitan (post 1500); Proven&ccedil;al")}},
	{{""}, {"oji"}, {I18N_NOOP("Ojibwa")}},
	{{"or"}, {"ori"}, {I18N_NOOP("Oriya")}},
	{{"om"}, {"orm"}, {I18N_NOOP("Oromo")}},
	{{""}, {"osa"}, {I18N_NOOP("Osage")}},
	{{"os"}, {"oss"}, {I18N_NOOP("Ossetian; Ossetic")}},
	{{"os"}, {"oss"}, {I18N_NOOP("Ossetic; Ossetian")}},
	{{""}, {"oto"}, {I18N_NOOP("Otomian languages")}},
	{{""}, {"pal"}, {I18N_NOOP("Pahlavi")}},
	{{""}, {"pau"}, {I18N_NOOP("Palauan")}},
	{{"pi"}, {"pli"}, {I18N_NOOP("Pali")}},
	{{""}, {"pam"}, {I18N_NOOP("Pampanga")}},
	{{""}, {"pag"}, {I18N_NOOP("Pangasinan")}},
	{{"pa"}, {"pan"}, {I18N_NOOP("Panjabi")}},
	{{""}, {"pap"}, {I18N_NOOP("Papiamento")}},
	{{""}, {"paa"}, {I18N_NOOP("Papuan (Other)")}},
	{{"fa"}, {"per"}, {I18N_NOOP("Persian")}},
	{{""}, {"peo"}, {I18N_NOOP("Persian, Old (ca.600-400 B.C.)")}},
	{{""}, {"phi"}, {I18N_NOOP("Philippine (Other)")}},
	{{""}, {"phn"}, {I18N_NOOP("Phoenician")}},
	{{""}, {"pon"}, {I18N_NOOP("Pohnpeian")}},
	{{"pl"}, {"pol"}, {I18N_NOOP("Polish")}},
	{{"pt"}, {"por"}, {I18N_NOOP("Portuguese")}},
	{{""}, {"pra"}, {I18N_NOOP("Prakrit languages")}},
	{{"oc"}, {"oci"}, {I18N_NOOP("Proven&#231;al; Occitan (post 1500)")}},
	{{""}, {"pro"}, {I18N_NOOP("Proven&#231;al, Old (to 1500)")}},
	{{"ps"}, {"pus"}, {I18N_NOOP("Pushto")}},
	{{"qu"}, {"que"}, {I18N_NOOP("Quechua")}},
	{{"rm"}, {"roh"}, {I18N_NOOP("Raeto-Romance")}},
	{{""}, {"raj"}, {I18N_NOOP("Rajasthani")}},
	{{""}, {"rap"}, {I18N_NOOP("Rapanui")}},
	{{""}, {"rar"}, {I18N_NOOP("Rarotongan")}},
	{{""}, {"qaa-qtz"}, {I18N_NOOP("Reserved for local use")}},
	{{""}, {"roa"}, {I18N_NOOP("Romance (Other)")}},
	{{"ro"}, {"rum"}, {I18N_NOOP("Romanian")}},
	{{""}, {"rom"}, {I18N_NOOP("Romany")}},
	{{"rn"}, {"run"}, {I18N_NOOP("Rundi")}},
	{{"ru"}, {"rus"}, {I18N_NOOP("Russian")}},
	{{""}, {"sal"}, {I18N_NOOP("Salishan languages")}},
	{{""}, {"sam"}, {I18N_NOOP("Samaritan Aramaic")}},
	{{""}, {"smi"}, {I18N_NOOP("Sami languages (Other)")}},
	{{"sm"}, {"smo"}, {I18N_NOOP("Samoan")}},
	{{""}, {"sad"}, {I18N_NOOP("Sandawe")}},
	{{"sg"}, {"sag"}, {I18N_NOOP("Sango")}},
	{{"sa"}, {"san"}, {I18N_NOOP("Sanskrit")}},
	{{""}, {"sat"}, {I18N_NOOP("Santali")}},
	{{"sc"}, {"srd"}, {I18N_NOOP("Sardinian")}},
	{{""}, {"sas"}, {I18N_NOOP("Sasak")}},
	{{""}, {"sco"}, {I18N_NOOP("Scots")}},
	{{"gd"}, {"gla"}, {I18N_NOOP("Scottish Gaelic; Gaelic")}},
	{{""}, {"sel"}, {I18N_NOOP("Selkup")}},
	{{""}, {"sem"}, {I18N_NOOP("Semitic (Other)")}},
	{{"sr"}, {"scc"}, {I18N_NOOP("Serbian")}},
	{{""}, {"srr"}, {I18N_NOOP("Serer")}},
	{{""}, {"shn"}, {I18N_NOOP("Shan")}},
	{{"sn"}, {"sna"}, {I18N_NOOP("Shona")}},
	{{""}, {"sid"}, {I18N_NOOP("Sidamo")}},
	{{""}, {"sgn"}, {I18N_NOOP("Sign languages")}},
	{{""}, {"bla"}, {I18N_NOOP("Siksika")}},
	{{"sd"}, {"snd"}, {I18N_NOOP("Sindhi")}},
	{{"si"}, {"sin"}, {I18N_NOOP("Sinhalese")}},
	{{""}, {"sit"}, {I18N_NOOP("Sino-Tibetan (Other)")}},
	{{""}, {"sio"}, {I18N_NOOP("Siouan languages")}},
	{{""}, {"den"}, {I18N_NOOP("Slave (Athapascan)")}},
	{{""}, {"sla"}, {I18N_NOOP("Slavic (Other)")}},
	{{"sk"}, {"slo"}, {I18N_NOOP("Slovak")}},
	{{"sl"}, {"slv"}, {I18N_NOOP("Slovenian")}},
	{{""}, {"sog"}, {I18N_NOOP("Sogdian")}},
	{{"so"}, {"som"}, {I18N_NOOP("Somali")}},
	{{""}, {"son"}, {I18N_NOOP("Songhai")}},
	{{""}, {"snk"}, {I18N_NOOP("Soninke")}},
	{{""}, {"wen"}, {I18N_NOOP("Sorbian languages")}},
	{{""}, {"nso"}, {I18N_NOOP("Sotho, Northern")}},
	{{"st"}, {"sot"}, {I18N_NOOP("Sotho, Southern")}},
	{{""}, {"sai"}, {I18N_NOOP("South American Indian (Other)")}},
	{{"es"}, {"spa"}, {I18N_NOOP("Spanish; Castilian")}},
	{{""}, {"suk"}, {I18N_NOOP("Sukuma")}},
	{{""}, {"sux"}, {I18N_NOOP("Sumerian")}},
	{{"su"}, {"sun"}, {I18N_NOOP("Sundanese")}},
	{{""}, {"sus"}, {I18N_NOOP("Susu")}},
	{{"sw"}, {"swa"}, {I18N_NOOP("Swahili")}},
	{{"ss"}, {"ssw"}, {I18N_NOOP("Swati")}},
	{{"sv"}, {"swe"}, {I18N_NOOP("Swedish")}},
	{{""}, {"syr"}, {I18N_NOOP("Syriac")}},
	{{"tl"}, {"tgl"}, {I18N_NOOP("Tagalog")}},
	{{"ty"}, {"tah"}, {I18N_NOOP("Tahitian")}},
	{{""}, {"tai"}, {I18N_NOOP("Tai (Other)")}},
	{{"tg"}, {"tgk"}, {I18N_NOOP("Tajik")}},
	{{""}, {"tmh"}, {I18N_NOOP("Tamashek")}},
	{{"ta"}, {"tam"}, {I18N_NOOP("Tamil")}},
	{{"tt"}, {"tat"}, {I18N_NOOP("Tatar")}},
	{{"te"}, {"tel"}, {I18N_NOOP("Telugu")}},
	{{""}, {"ter"}, {I18N_NOOP("Tereno")}},
	{{""}, {"tet"}, {I18N_NOOP("Tetum")}},
	{{"th"}, {"tha"}, {I18N_NOOP("Thai")}},
	{{"bo"}, {"tib"}, {I18N_NOOP("Tibetan")}},
	{{""}, {"tig"}, {I18N_NOOP("Tigre")}},
	{{"ti"}, {"tir"}, {I18N_NOOP("Tigrinya")}},
	{{""}, {"tem"}, {I18N_NOOP("Timne")}},
	{{""}, {"tiv"}, {I18N_NOOP("Tiv")}},
	{{""}, {"tli"}, {I18N_NOOP("Tlingit")}},
	{{""}, {"tpi"}, {I18N_NOOP("Tok Pisin")}},
	{{""}, {"tkl"}, {I18N_NOOP("Tokelau")}},
	{{""}, {"tog"}, {I18N_NOOP("Tonga (Nyasa)")}},
	{{"to"}, {"ton"}, {I18N_NOOP("Tonga (Tonga Islands)")}},
	{{""}, {"tsi"}, {I18N_NOOP("Tsimshian")}},
	{{"ts"}, {"tso"}, {I18N_NOOP("Tsonga")}},
	{{"tn"}, {"tsn"}, {I18N_NOOP("Tswana")}},
	{{""}, {"tum"}, {I18N_NOOP("Tumbuka")}},
	{{"tr"}, {"tur"}, {I18N_NOOP("Turkish")}},
	{{""}, {"ota"}, {I18N_NOOP("Turkish, Ottoman (1500-1928)")}},
	{{"tk"}, {"tuk"}, {I18N_NOOP("Turkmen")}},
	{{""}, {"tvl"}, {I18N_NOOP("Tuvalu")}},
	{{""}, {"tyv"}, {I18N_NOOP("Tuvinian")}},
	{{"tw"}, {"twi"}, {I18N_NOOP("Twi")}},
	{{""}, {"uga"}, {I18N_NOOP("Ugaritic")}},
	{{"ug"}, {"uig"}, {I18N_NOOP("Uighur")}},
	{{"uk"}, {"ukr"}, {I18N_NOOP("Ukrainian")}},
	{{""}, {"umb"}, {I18N_NOOP("Umbundu")}},
	{{""}, {"und"}, {I18N_NOOP("Undetermined")}},
	{{"ur"}, {"urd"}, {I18N_NOOP("Urdu")}},
	{{"uz"}, {"uzb"}, {I18N_NOOP("Uzbek")}},
	{{""}, {"vai"}, {I18N_NOOP("Vai")}},
	{{""}, {"ven"}, {I18N_NOOP("Venda")}},
	{{"vi"}, {"vie"}, {I18N_NOOP("Vietnamese")}},
	{{"vo"}, {"vol"}, {I18N_NOOP("Volap&#252;k")}},
	{{""}, {"vot"}, {I18N_NOOP("Votic")}},
	{{""}, {"wak"}, {I18N_NOOP("Wakashan languages")}},
	{{""}, {"wal"}, {I18N_NOOP("Walamo")}},
	{{""}, {"war"}, {I18N_NOOP("Waray")}},
	{{""}, {"was"}, {I18N_NOOP("Washo")}},
	{{"cy"}, {"wel"}, {I18N_NOOP("Welsh")}},
	{{"wo"}, {"wol"}, {I18N_NOOP("Wolof")}},
	{{"xh"}, {"xho"}, {I18N_NOOP("Xhosa")}},
	{{""}, {"sah"}, {I18N_NOOP("Yakut")}},
	{{""}, {"yao"}, {I18N_NOOP("Yao")}},
	{{""}, {"yap"}, {I18N_NOOP("Yapese")}},
	{{"yi"}, {"yid"}, {I18N_NOOP("Yiddish")}},
	{{"yo"}, {"yor"}, {I18N_NOOP("Yoruba")}},
	{{""}, {"ypk"}, {I18N_NOOP("Yupik languages")}},
	{{""}, {"znd"}, {I18N_NOOP("Zande")}},
	{{""}, {"zap"}, {I18N_NOOP("Zapotec")}},
	{{""}, {"zen"}, {I18N_NOOP("Zenaga")}},
	{{"za"}, {"zha"}, {I18N_NOOP("Zhuang; Chuang")}},
	{{"zu"}, {"zul"}, {I18N_NOOP("Zulu")}},
	{{""}, {"zun"}, {I18N_NOOP("Zuni")}},
	{{0}, {0}, {0}}
};
*/

LangOptPage::LangOptPage
(
        QString    _deflang,
        LangSet    &_langset,
        QString    &lastPix,
	QWidget* parent,
	const char* name
)
	:
	LangOptPageForm( parent, name ),
        langset (_langset),
        lastPixName(lastPix)
{
  langset_popup = 0;
  deflang = _deflang;

  connect( b_langDel, SIGNAL(clicked()), SLOT(slotDeleteClicked()) );
  connect( b_langNew, SIGNAL(clicked()), SLOT(slotNewClicked()) );
  connect( b_langPixmap, SIGNAL(clicked()), SLOT(slotPixmapClicked()) );

  connect( e_newName, SIGNAL(textChanged(const QString&)), SLOT(slotNewNameChanged(const QString&)) );
  connect( d_shortName, SIGNAL(activated(const QString&)), SLOT(slotShortActivated(const QString&)) );
  connect( e_langLong, SIGNAL(textChanged(const QString&)), SLOT(slotLangChanged(const QString&)) );
  connect( e_shortName2, SIGNAL(textChanged(const QString&)), SLOT(slotShort2Changed(const QString&)) );

  loadCountryData();
  b_lang_kde->setPopup(langset_popup);
  b_langNew->setEnabled(false); // activate after data is entered

  setCaption(i18n("Options" ));

  for (int i = 0; i < (int) langset.size() && i < MAX_LANGSET; i++)
    d_shortName->insertItem (langset.shortId(i));

  enableLangWidgets();

  if (d_shortName->count() ) {
    d_shortName->setCurrentItem(0);
    e_langLong->setText(langset.longId(0));
    e_shortName2->setText(langset.shortId2(0));
    if (!langset.PixMapFile(0).isEmpty() ) {
      QPixmap pix(langset.PixMapFile(0));
      if (!pix.isNull() ) {
        lastPixName = langset.PixMapFile(0);
        b_langPixmap->setPixmap (pix);
      }
      else
        b_langPixmap->setText (i18n("Picture is invalid"));
    }
    else
      b_langPixmap->setText  (i18n("No picture selected"));
  }
  else {
    b_langPixmap->setText  (i18n("No picture selected"));
    b_langPixmap->setEnabled(false);
  }

//  label_newName->setBuddy(e_newName);
  label_shortName2->setBuddy(e_shortName2);
  label_shortName->setBuddy(d_shortName);
  label_langLong->setBuddy(e_langLong);
  label_langPixmap->setBuddy(b_langPixmap);

  b_lang_iso1->setEnabled(false);

  // FIXME enable after kde3 release
  b_lang_iso1->hide();
}


LangOptPage::~LangOptPage ()
{
 delete langset_popup;
}


void LangOptPage::initFocus() const
{
  d_shortName->setFocus();
}


void LangOptPage::slotDeleteClicked()
{
  if (d_shortName->count() != 0) {
    langset.erase (d_shortName->currentItem());
    d_shortName->removeItem (d_shortName->currentItem());
    if (d_shortName->count() != 0)
      d_shortName->setCurrentItem (0);
  }

  if (d_shortName->count() != 0) {
    setPixmap(langset.PixMapFile(d_shortName->currentItem()));
    e_langLong->setText(langset.longId(d_shortName->currentItem()));
    e_shortName2->setText(langset.shortId2(d_shortName->currentItem()));
  }
  else {
    b_langPixmap->setText (i18n("No picture selected"));
    e_langLong->setText("");
    e_shortName2->setText("");
    b_langPixmap->setEnabled(false);
  }
  enableLangWidgets();
}


void LangOptPage::enableLangWidgets()
{
  bool enabled = d_shortName->count() != 0;
  b_langDel->setEnabled(enabled);
  b_langPixmap->setEnabled(enabled);
  d_shortName->setEnabled(enabled);
  e_langLong->setEnabled(enabled);
  e_shortName2->setEnabled(enabled);
}


void LangOptPage::slotNewNameChanged(const QString& _s)
{
   QString s = _s;
   b_langNew->setEnabled(s.stripWhiteSpace().length() >= 2);
}


void LangOptPage::slotNewClicked()
{
   QString s = e_newName->text();
   d_shortName->insertItem(s.stripWhiteSpace());
   d_shortName->setCurrentItem(d_shortName->count()-1);
   enableLangWidgets();
   slotShortActivated(s);
   e_newName->setText("");
   e_langLong->setFocus();
}


void LangOptPage::slotShortActivated(const QString& _id)
{
   int i = 0;
   QString id = _id.stripWhiteSpace();
   if (d_shortName->count() > (int) langset.size() ) {
     // avoid doublettes with language code
     for (i = 0; i < d_shortName->count(); i++)
       if (d_shortName->text(i).isNull() ) {
         d_shortName->removeItem(d_shortName->currentItem());
         d_shortName->setCurrentItem(0);
       }
  
     for (i = 0; i < d_shortName->count()-1; i++)  // omit last
       if (id == d_shortName->text(i)) {
         d_shortName->removeItem(d_shortName->currentItem());
         d_shortName->setCurrentItem(i);
         return;
       }
  
     if (d_shortName->count() > (int) langset.size()
         && langset.size() < MAX_LANGSET ) {
       langset.addSet (id, "", "");
     }
   }

   if (d_shortName->count() != 0) {
     deflang = id;
     b_langPixmap->setEnabled(true);
     e_langLong->setText (langset.longId(d_shortName->currentItem()));
     e_shortName2->setText (langset.shortId2(d_shortName->currentItem()));

     if (!langset.PixMapFile(d_shortName->currentItem()).isEmpty() ) {
       QPixmap pix (langset.PixMapFile(d_shortName->currentItem()));
       if (!pix.isNull() )
         b_langPixmap->setPixmap (pix);
       else
         b_langPixmap->setText (i18n("Picture is invalid"));
     }
     else
       b_langPixmap->setText (i18n("No picture selected"));
   }
}


void LangOptPage::slotLangChanged(const QString& s)
{
  if (   d_shortName->count() != 0
      && d_shortName->currentItem() < (int) langset.size() )
    langset.setLongId(s, d_shortName->currentItem());
}


void LangOptPage::slotShort2Changed(const QString& s)
{
  if (   d_shortName->count() != 0
      && d_shortName->currentItem() < (int) langset.size() )
    langset.setShortId2(s, d_shortName->currentItem());
}


bool LangOptPage::setPixmap(QString pm)
{
  if (d_shortName->count() ) {
    QPixmap pix (pm);
    if (!pix.isNull() ) {
      langset.setPixMapFile(pm, d_shortName->currentItem());
      b_langPixmap->setPixmap (pix);
      return true;
    }
  }
  return false;

}


void LangOptPage::slotPixmapClicked()
{
  if (langset.size() > 0 ) {
    if (lastPixName.isNull() || QPixmap(lastPixName).isNull()) {
      QString s;
      if (!langset.shortId(d_shortName->currentItem()).isNull() ) {
        s = langset.shortId(d_shortName->currentItem());
        lastPixName = locate ("locale", "l10n/"+s+"/flag.png");
        if (lastPixName.isNull()) {
          lastPixName = locate ("locale", "l10n/");
        }
      }
      else {
        lastPixName = locate ("locale", "l10n/");
      }
    }
    else {
      QFileInfo fi (lastPixName);
      lastPixName = fi.dirPath()+"/flag.png";
    }
    QString s = KFileDialog::getOpenFileName (lastPixName, "*.png *.xpm *.gif *.xbm");
    if (!s.isEmpty() ) {
      if (setPixmap (s) )
        lastPixName = s;
      else {
        b_langPixmap->setText (i18n("Picture is invalid"));
        KMessageBox::sorry(this,
           i18n("File does not contain a valid graphics format\n"),
           kapp->makeStdCaption(QString::null),
           false);
      }
    }
  }
}

void LangOptPage::keyPressEvent( QKeyEvent *e )
{
   if (e->state() & AltButton & ControlButton & ShiftButton == 0) {
     if (  e->key() == Key_Escape )
       emit reject();
     else if (  e->key() == Key_Enter
              ||e->key() == Key_Return)
       emit accept();
     else
       e->ignore();
   }
   else
     e->ignore();
}


LangSet LangOptPage::getLangSet () const
{
  return langset;
}


class sortByRegion : public binary_function<LangOptPage::LangRef,
                                            LangOptPage::LangRef, bool>
{
 public:

  sortByRegion () {}

  bool operator() (const LangOptPage::LangRef &x, const LangOptPage::LangRef &y) const {
      return (QString::compare(x.region.upper(), y.region.upper() ) < 0);
  }
};


void LangOptPage::loadCountryData()
{

  // temperary use of our locale as the global locale
  KLocale *lsave = KGlobal::_locale;
  QString curr_lang = lsave->language();

//  KLocale locale("kvoctrain");
  KLocale locale(QString::null);
  locale.setLanguage(curr_lang);
  KGlobal::_locale = &locale;

  globalLangs.clear();
  QString sub = QString::fromLatin1("l10n/");
  regionlist = KGlobal::dirs()->findAllResources("locale",
                                 sub + QString::fromLatin1("*.desktop"));

  typedef pair<QString, int> regionpair;
  typedef map<QString, int> regionmap_t;

  regionmap_t regionmap;
  for ( QStringList::ConstIterator it = regionlist.begin();
    it != regionlist.end();
    ++it )
  {
    QString tag = *it;
    int index;

    index = tag.findRev('/');
    if (index != -1) tag = tag.mid(index + 1);

    index = tag.findRev('.');
    if (index != -1) tag.truncate(index);

    KSimpleConfig entry(*it);
    entry.setGroup(QString::fromLatin1("KCM Locale"));
    QString name = entry.readEntry(QString::fromLatin1("Name"), i18n("without name"));

    regionmap.insert(regionpair(tag, globalLangs.size()));
    globalLangs.push_back(LangRef(name, LangSet()));
  }

  // add all languages to the list
  countrylist = KGlobal::dirs()->findAllResources("locale",
                               sub + QString::fromLatin1("*/entry.desktop"));
  countrylist.sort();

  for ( QStringList::ConstIterator it = countrylist.begin();
    it != countrylist.end(); ++it )
  {
    KSimpleConfig entry(*it);
    entry.setGroup(QString::fromLatin1("KCM Locale"));
    QString name = entry.readEntry(QString::fromLatin1("Name"),
                       i18n("without name"));
    QString submenu = entry.readEntry(QString::fromLatin1("Region"));

    QString tag = *it;
    int index = tag.findRev('/');
    tag.truncate(index);
    index = tag.findRev('/');
    tag = tag.mid(index+1);

    QString all_langs = entry.readEntry(QString::fromLatin1("Languages"));
    if (tag == all_langs)
      all_langs = "";

    // search language name and 3char alternative from "official" list
    KV_ISO639_Code *iso639 = kv_iso639_1;
    while (iso639->iso2code != 0) {
      if (strcmp(iso639->iso1code, tag.latin1()) == 0) {
        name = i18n(iso639->langname);
        if (strlen(iso639->iso2code) > 0)
          all_langs = iso639->iso2code;
        break;
      }
      ++iso639;
    }

    QString pixmap = *it;
    index = pixmap.findRev('/');
    pixmap.truncate(index);
    pixmap += "/flag.png";

    regionmap_t::const_iterator it = regionmap.find(submenu);
    if (it != regionmap.end())
      globalLangs[(*it).second].langs.addSet (tag, all_langs, name, pixmap);
  }

  std::sort (globalLangs.begin(), globalLangs.end(), sortByRegion() );

  int idx = 0;
  global_langset.clear();
  delete langset_popup;
  langset_popup = new QPopupMenu();
  connect(langset_popup, SIGNAL(activated(int)), this, SLOT(slotLangFromGlobalActivated(int)));
  for ( unsigned i = 0; i < globalLangs.size(); ++i) {
    QPopupMenu *regpop = new QPopupMenu();
    connect(regpop, SIGNAL(activated(int)), this, SLOT(slotLangFromGlobalActivated(int)));
    langset_popup->insertItem(globalLangs[i].region, regpop);
    for (unsigned j = 0; j < globalLangs[i].langs.size(); ++j) {
      regpop->insertItem(QPixmap(globalLangs[i].langs.PixMapFile(j)),
                                 globalLangs[i].langs.longId(j),
                                 idx++);
      global_langset.addSet(globalLangs[i].langs.shortId(j),
                            globalLangs[i].langs.shortId2(j),
                            globalLangs[i].langs.longId(j),
                            globalLangs[i].langs.PixMapFile(j));
    }
  }

  // restore the old global locale
  KGlobal::_locale = lsave;
}


void LangOptPage::slotLangFromGlobalActivated(int i)
{
  if (i < (int)global_langset.size()) {
    QString s = global_langset.shortId(i);
    d_shortName->insertItem(s.stripWhiteSpace());
    d_shortName->setCurrentItem(d_shortName->count()-1);
    slotShortActivated(s);
    enableLangWidgets();

    e_shortName2->setText(global_langset.shortId2(i));
    slotShort2Changed(global_langset.shortId2(i));

    e_langLong->setText(global_langset.longId(i));
    slotLangChanged(global_langset.longId(i));

    setPixmap(global_langset.PixMapFile(i));
    e_newName->setText("");
    e_langLong->setFocus();
    e_langLong->selectAll();
  }
}


#include "LangOptPage.moc"
