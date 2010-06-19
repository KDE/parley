#!/usr/bin/env python
# -*- coding: utf-8 -*-
import re, sys, os, getopt

headinglevels=['chapter', 'sect1', 'sect2', 'sect3', 'sect4', 'sect5']
#toplevel=1 #sect1
toplevel=0 #chapter

userbase_content_marker='\n<!--userbase-content-->\n'

def usage():
    print '\nUsage: python %s [OPTION] path/to/userbasepagedumpfile' %os.path.basename(sys.argv[0])
    print '       generates docbook output file with extension *.new.docbook'
    print 'Options: -h, --help              : usage'
    print '         -t, --toplevel          : toplevel heading 0=chapter  1=sect1'
    print '         -c, --check             : check heading levels and print them to stout'
    print '         -n, --noheaderfooter    : no header/footer from template'
    print '         -r file, --replace file : *.docbook file with kde docbook header + footer'
    print '                                   and body from a previous userbase page dump to replace it'
    print '                                   if no replace file was found a default simple kde docbook header + footer is used'
    print 'Default                          : try to get the userbase data and generate a docbook with a simple header and footer'
    sys.exit(2)
try:
    opts, args = getopt.getopt(sys.argv[1:], "ht:r:cn", ["help", "toplevel=", 'replace=','check', 'noheaderfooter'])
except getopt.GetoptError:
    usage() # print help information and exit

replacefile=''
checklevels=False
noheaderfooter=False
for o,a in opts:
    if o in ("-h", "--help"):
        usage()
    if o in ("-t", "--toplevel"):
        toplevel=a
    if o in ("-r", "--replace"):
        replacefile=a
    if o in ("-c", "--check"):
        checklevels=True
    if o in ("-n", "--noheaderfooter"):
        noheaderfooter=True

if len(args) != 1:
    usage()
inputfile=args[0]
if not os.path.isfile(inputfile):
    sys.exit('input file %s not found' %inputfile) 

docbookheader,docbookfooter='',''

if replacefile!='':
  if not os.path.isfile(replacefile):
    print 'docbook file with previous userbase page dump not found, generating a docbook with template header + footer'
    replacefile=''
else:
  try:
    text=open(replacefile,"r").read()
    docbookheader,userpagebody,docbookfooter=text.split(userbase_content_marker)
  except:
    print 'docbook file has no previous userbase page dump, generating a docbook with simple template header + footer'
    replacefile=''

def sectionheader(text,level,beginmarkup):
  #output of headinglevels to console
  if checklevels: print text.rstrip('\n')
  levelstr='='*level
  remuster='%s.*?%s' %(levelstr,levelstr)
  such=re.compile(remuster,re.DOTALL)
  for treffer in such.findall(text):
    sectiontitle=treffer.replace(levelstr,'')
    sectiontitle=sectiontitle.strip(' ')
    sectionid=sectiontitle.replace("'",'')
    sectionid=sectionid.replace("?",'')
    sectionid=sectionid.replace(",",'')
    sectionid=sectionid.replace("/",'-')
    sectionid=sectionid.replace(' ','-')
    sectionid=sectionid.replace(':','-')
    sectionid=sectionid.replace('.','-')
    sectionid=sectionid.replace('<quote>','').replace('</quote>','')
    sectionid=sectionid.replace('&quot;','')
    sectionid=sectionid.replace('&nbsp;','')
    sectionid=sectionid.replace('&amp;','')
    sectionid=sectionid.replace('(','').replace(')','')
    sectionid=sectionid.lower()
    text=text.replace(treffer, '%s id="%s"><title>%s</title>' %(beginmarkup,sectionid,sectiontitle))
    return text
    
text=open(inputfile,"r").read()
remuster='<timestamp>.*?</timestamp>'
such=re.compile(remuster,re.DOTALL)
if len(such.findall(text))!=1:
  print 'missing userbase timestamp'
  outtext=userbase_content_marker
else:
  userbase_timestamp='<!--userbase %s-->\n' %(such.findall(text)[0])
  remuster='<timestamp>.*?T'
  such=re.compile(remuster,re.DOTALL)
  releasedate=such.findall(text)[0].replace('<timestamp>','').replace('T','')
  outtext='%s%s' %(userbase_timestamp, userbase_content_marker)

if userbase_timestamp in docbookheader: # it is the same page dump version, remove it
  docbookheader=docbookheader.replace(userbase_timestamp,'')

userbase_header=''
userbase_header,text = text.split('<text') #strip strip userbaser export page header off
text=text.split('</text')[0] #strip strip userbaser export page footer off

if noheaderfooter==False:
  #userbase header scan
  abstracttext,pagename,abstractscreenshotlink='','',''
  remuster="'''.*?'''"
  such=re.compile(remuster,re.DOTALL)
  if len(such.findall(userbase_header))!=1:
    abstracttext=''
  else:
    abstracttext=such.findall(userbase_header)[0].strip("'''")
  remuster='\[\[.*?\]\]'
  #we don't use screenshot for now
  #such=re.compile(remuster,re.DOTALL)
  #abstractscreenshotlink=such.findall(userbase_header)[0].strip("[[Image:").split('|')[0]
  pagename=re.findall('<title>.*?</title>',userbase_header)[0].replace('<title>','').replace('</title>','')
  authorname='This documentation was converted from the KDE UserBase '\
    +'<ulink url=" http://userbase.kde.org/%s">%s</ulink> page.' %(pagename,pagename)
  docbookheader='<?xml version="1.0" ?>\n'\
    +'<!DOCTYPE book PUBLIC "-//KDE//DTD DocBook XML V4.2-Based Variant V1.1//EN" "dtd/kdex.dtd" [\n'\
    +'  <!ENTITY % addindex "IGNORE">\n  <!ENTITY % English "INCLUDE">\n]>\n<book lang="&language;">\n'
  docbookheader+='<bookinfo>\n<title>The %s Handbook</title>\n<authorgroup>\n<author>\n<personname>\n\
    <firstname>%s</firstname>\n<surname></surname>\n</personname>\n\
    </author>\n<!-- TRANS:ROLES_OF_TRANSLATORS -->\n\
    </authorgroup>\n<legalnotice>&FDLNotice;</legalnotice>\n<date>%s</date>\n\
    <releaseinfo>&kde; SC 4.5</releaseinfo>\n<abstract>\n<para>%s</para>\n\
    </abstract>\n\
    <keywordset>\n<keyword>KDE</keyword>\n</keywordset>\n</bookinfo>\n' %(pagename,authorname,releasedate,abstracttext)
  docbookfooter='<chapter id="credits">\n<title>Credits and License</title>\n\
    <para>\nDocumentation Copyright see the UserBase \
    <ulink url="http://userbase.kde.org/index.php?title=%s&amp;action=history">%s page history</ulink></para>\n\
    <!-- TRANS:CREDIT_FOR_TRANSLATORS -->\n&underFDL;\n</chapter>\n&documentation.index;\n</book>' %(pagename,pagename)
  if releasedate=='':print 'releasedate not found'
  if userbase_header=='':print 'userbase_header not found'
  if abstracttext=='':print 'abstracttext not found'
  #if abstractscreenshotlink=='':print 'abstractscreenshotlink not found'
  if pagename=='':print 'pagename not found'

#&lt;nowiki&gt;== Subpages of {{FULLPAGENAME}}==
#{{Special:PrefixIndex/{{FULLPAGENAME}}/}}&lt;/nowiki&gt;
nowikimarkup_id,nowikimarkup_idtext=[],[]
nowikimarkup_id_marker='nowikimarkup_id_marker__'
nowikimarkup_id_number=0
remuster='&lt;nowiki&gt;.*?&lt;/nowiki&gt;' #&lt;pre&gt; &lt;/pre&gt;
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  if '\n' in treffer:
    newmarkup='screen'
  else:
    newmarkup='command'#'literal'
  repl=treffer.replace('&lt;nowiki&gt;','<%s>'%newmarkup)
  repl=repl.replace('&lt;/nowiki&gt;','</%s>'%newmarkup)
  nowikimarkup_id_marker_text='nowikimarkup_id_marker__%dx' %nowikimarkup_id_number
  nowikimarkup_id.append(nowikimarkup_id_marker_text)
  nowikimarkup_idtext.append(repl)
  nowikimarkup_id_number+=1
  text=text.replace(treffer,nowikimarkup_id_marker_text)#'<literal>%s</literal>'%repl)
  #what to do with this?

premarkup_id,premarkup_idtext=[],[]
premarkup_id_marker='premarkup_id_marker__'
premarkup_id_number=0
remuster='&lt;pre&gt;.*?&lt;/pre&gt;' #&lt;pre&gt; &lt;/pre&gt;
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  if '\n' in treffer:
    newmarkup='screen'
  else:
    newmarkup='command'#'literal'
  repl=treffer.replace('&lt;pre&gt;','<%s>'%newmarkup)
  repl=repl.replace('&lt;/pre&gt;','</%s>'%newmarkup)
  premarkup_id_marker_text='premarkup_id_marker__%dx' %premarkup_id_number
  premarkup_id.append(premarkup_id_marker_text)
  premarkup_idtext.append(repl)
  premarkup_id_number+=1
  text=text.replace(treffer,premarkup_id_marker_text)#'<literal>%s</literal>'%repl)
  #what to do with this?

#strip off some header footer stuff WRONG!!!!
remuster='\{\|style.*?\|\}'
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  if 'nowikimarkup_id_marker__' in treffer:
    such=re.compile('nowikimarkup_id_marker__[0-9]*?x',re.DOTALL)
    suchtext=such.findall(treffer)[0]
    text=text.replace(treffer,'nowikimarkup_id_marker__%dx' %nowikimarkup_id.index(suchtext))
  elif 'premarkup_id_marker__' in treffer:
    such=re.compile('premarkup_id_marker__[0-9]*?x',re.DOTALL)
    suchtext=such.findall(treffer)[0]
    text=text.replace(treffer,'premarkup_id_marker__%dx' %premarkup_idtext.index(suchtext))
  else:
     text=text.replace(treffer,'')

#strip off {{Construction}}
remuster='\{\{Construction\}\}'
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  text=text.replace(treffer,'')

#strip off {{KDE3}} + {{KDE4}}
remuster='\{\{KDE[34]\}\}'
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  text=text.replace(treffer,'')

#strip off {{Being_Edited}}
remuster='\{\{Being_Edited\}\}'
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  text=text.replace(treffer,'')

#strip off {{Community-app-footnote}}
remuster='\{\{Community-app-footnote\}\}'
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  text=text.replace(treffer,'')

#strip off {{Community-app}}
remuster='\{\{Community-app\}\}'
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  text=text.replace(treffer,'')

#strip off {{FULLPAGENAME}}
remuster='\{\{FULLPAGENAME\}\}'
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  #print 1 ,treffer, treffer.replace(treffer,'')
  text=text.replace(treffer,'')

#{{Special:PrefixIndex/{{FULLPAGENAME}}/}} etc ???
# FULLPAGENAME is already stripped off
remuster='\{\{Special:PrefixIndex//\}\}'
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  text=text.replace(treffer,'')

#note
#::{|cellpadding="2" |[[Image:Im-status-message-edit.png]]||'''Note'''||Some important information |}
# docbook <note>
#warning
#::{| |[[Image:dialog-warning.png|32px]]||'''Warning'''||This is dangerous |}
#tip
#::{| |[[Image:help-hint.png]]||'''Tip'''||Something useful to remember |}
# docbook <tip>
remuster='\{\|.*?\|\}'
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  treffersplit=treffer.split('||')
  if len(treffersplit)>1:
    markuptype, markuptext= treffersplit[1],treffersplit[2]
    markuptype=markuptype.strip("'")
    markuptext=markuptext.rstrip('|}')
    if markuptype.lower()=='Note':
      repl='<note><para>%s</para></note>' %markuptext
      text=text.replace(treffer,repl)
    elif markuptype.lower()=='Warning':
      repl='<warning><para>%s</para></warning>' %markuptext
      text=text.replace(treffer,repl)
    elif markuptype.lower()=='Tip':
      repl='<tip><para>%s</para></tip>' %markuptext
      text=text.replace(treffer,repl)

#warning
#{{warning|This is a very dangerous thing to do}}
# docbook <warning>
#{{info|This is another way to display your information}}
# docbook <note>?
remuster='\{\{.*?\}\}'
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  warningnote=treffer.lstrip('{').rstrip('}')
  repl=''
  warningnotesplit=warningnote.split('|')
  warningnote,warningnotetext=warningnotesplit[0],' '.join(warningnotesplit[1:])
  if warningnote.lower()=='warning':
    repl='<warning><para>%s</para></warning>' %warningnotetext
  elif warningnote.lower()=='info':
    repl='<note><para>%s</para></note>' %warningnotetext
  if repl!='':text=text.replace(treffer,repl)

#textlines = open(inputfile,"rw").readlines()
textlines = text.split('\n')

level=0
skip=False
initemizedlist=False
intable=False
innumberedlist=False
minequalno=5
#fix non consecutive heading levels
prevno,jumpno,deltano=1,0,0
remuster='^={1,5}'
for i in range(0,len(textlines)):
  line=textlines[i]
  such=re.compile(remuster)
  if len(such.findall(line))>0:
    actualno=line.count('=',0,5)
    minequalno=min(minequalno,actualno)
    if actualno-prevno>1:
      deltano=actualno-prevno-1
      jumpno=prevno
    prevno=actualno
    if actualno<=jumpno:
      jumpno=0
      deltano=0
    #print line.rstrip('\n'),actualno,jumpno,deltano,line.replace('='*actualno,'='*(actualno-deltano)).rstrip('\n')
    if deltano>0: textlines[i]=line.replace('='*actualno,'='*(actualno-deltano))
if minequalno>1:
  delta=minequalno-1
  for i in range(0,len(textlines)):
    line=textlines[i]
    such=re.compile(remuster)
    if len(such.findall(line))>0:
      actualno=line.count('=',0,5)
      textlines[i]=line.replace('='*actualno,'='*(actualno-delta))
      #print line,textlines[i]

for line in textlines:
  line=line+'\n' 
  #need that cause switch from #textlines = open(inputfile,"rw").readlines() to textlines = text.split('\n')
  
  #debugging of lines
  #print '>>\n%s<<'%line 
  
  if '&lt;/translate&gt;' in line or '[[Category:' in line: skip=True
  #strip off: &lt;!--T:1--&gt
  remuster='&lt;.*&gt;'
  line=re.sub(remuster,'',line)

  if line[0:1]=='=' and line[1]!='=':
    closemarkup=''
    if level>3:closemarkup+='</%s>\n' %headinglevels[toplevel+3]
    if level>2:closemarkup+='</%s>\n' %headinglevels[toplevel+2]
    if level>1:closemarkup+='</%s>\n' %headinglevels[toplevel+1]
    if level>0:closemarkup+='</%s>\n' %headinglevels[toplevel+0]
    level=1
    repl=sectionheader(line,1,'<%s' %headinglevels[toplevel-1+1])
    outtext+='%s%s' %(closemarkup,repl)
    initemizedlist=False
  elif line[0:2]=='==' and line[2]!='=':
    closemarkup=''
    if level>3:closemarkup+='</%s>\n' %headinglevels[toplevel+3]
    if level>2:closemarkup+='</%s>\n' %headinglevels[toplevel+2]
    if level>1:closemarkup+='</%s>\n' %headinglevels[toplevel+1]
    level=2
    repl=sectionheader(line,2,'<%s' %headinglevels[toplevel-1+2])
    outtext+='%s%s' %(closemarkup,repl)
    initemizedlist=False
  elif line[0:3]=='===' and line[3]!='=':
    closemarkup=''
    if level>3:closemarkup+='</%s>\n' %headinglevels[toplevel+3]
    if level>2:closemarkup+='</%s>\n' %headinglevels[toplevel+2]
    level=3
    repl=sectionheader(line,3,'<%s' %headinglevels[toplevel-1+3])
    outtext+='%s%s' %(closemarkup,repl)
    initemizedlist=False
  elif line[0:4]=='====' and line[4]!='=':
    closemarkup=''
    if level>3:closemarkup+='</%s>\n' %headinglevels[toplevel+3]
    level=4
    repl=sectionheader(line,4,'<%s' %headinglevels[toplevel-1+4])
    outtext+='%s%s' %(closemarkup,repl)
    initemizedlist=False
  else: #level="para"
     if line !='\n' and level!=0 and skip==False:
       line=line.lstrip(':') #strip off intending for the moment, how translate that to docbook?
       if line[0]=='*' or line[0:2]==':*': #itemizedlist
         listitemtext=line.lstrip(' :*').rstrip(' ')
         if initemizedlist==False:
           initemizedlist=True
           outtext+='<itemizedlist>\n'
         else: 
           outtext=outtext.rstrip('\n')
           outtext=outtext[:-len('</itemizedlist>')]
           #alternativ outtext.relace('</itemizedlist><listitem>','<listitem>') later!!
         outtext+='<listitem><para>%s</para></listitem>\n</itemizedlist>\n' %listitemtext
       elif line[0:2]=='{|':  #begin table
         tabletext='<para>\n<table>\n  <title> </title>\n  <tgroup cols="%d">\n  <tbody>\n'
         colsnumber=0
         rownumber=0
         intable=True
         #print line
       elif line[0:2]=='| ':  #in table in row
         if intable:
           tabletext+='    <entry>%s</entry>\n' %line[1:].lstrip(' ').rstrip(' \n')
           colsnumber+=1
       elif line[0:2]=='|-': #in table new row
         if intable:
           if rownumber>0:
             tabletext+='  </row>\n'
           rownumber+=1
           tabletext+='  <row>\n'
           colsnumber=0
       elif line[0:2]=='|}': #end table
         #print line
         if intable:
           tabletext=tabletext %colsnumber
           outtext+="%s    </row>\n</tbody>\n  </tgroup>\n</table>\n</para>\n" %(tabletext)
           intable=False
       else:
         if initemizedlist==True:
           initemizedlist=False
         outtext+='<para>%s</para>\n' %line
outtext+='</%s>' %(headinglevels[level-1])
if level>1: outtext+='\n</%s>' %(headinglevels[toplevel])

#''''' -> ''' bold+italic -> gui
outtext=outtext.replace("'''''","'''")

#'''[[#Vocabulary Practice|Practice]]'''
# guilabel-link-guilabel not allowed in docbook
# move ''' inside for document internal links
remuster="'''\[\[#.*?\]]'''"
such=re.compile(remuster,re.DOTALL)
for guilinkgui in such.findall(outtext):
  repl=guilinkgui.replace("'''[[","[[")
  repl=repl.replace("|","|'''")
  repl=repl.replace("]]'''","''']]")
  outtext=outtext.replace(guilinkgui,repl)
remuster="''\[\[#.*?\]]''"
such=re.compile(remuster,re.DOTALL)
for guilinkgui in such.findall(outtext):
  repl=guilinkgui.replace("''[[","[[")
  repl=repl.replace("|","|''")
  repl=repl.replace("]]''","'']]")
  outtext=outtext.replace(guilinkgui,repl)
#strip off all other occurencies
outtext=outtext.replace("'''[[","[[")
outtext=outtext.replace("]]'''","]]")
outtext=outtext.replace("''[[","[[")
outtext=outtext.replace("]]''","]]")


#&quot;context menu&quot;
#<quote>context menu</quote>
remuster='&quot;.*?&quot;'
such=re.compile(remuster)#,re.DOTALL)
for quot in such.findall(outtext):
  repl=quot.replace('&quot;','')
  repl='<quote>%s</quote>' %(repl.strip(' '))
  outtext=outtext.replace(quot,repl)

#&amp;nbsp; -> &nbsp; make it a valid entity
outtext=outtext.replace('&amp;nbsp;','&nbsp;')

#-&amp;gt; -> -&gt;
# proper fix would bee to add markup menuchoice..., but how to detect if the menu or menuitem has one or more words?
outtext=outtext.replace('-&amp;gt;','-&gt;')

#<quote> in title not allowed, strip it off
remuster='<title>.*?</title>'
such=re.compile(remuster)#,re.DOTALL)
for title in such.findall(outtext):
  repl=title.replace('<quote>','').replace('</quote>','')
  outtext=outtext.replace(title,repl)

#''View-&amp;gt;Lessons.''
#<menuchoice><guimenu>View</guimenu><guimenuitem>Lessons</guimenuitem></menuchoice>
remuster="'{2,3}.*?-&amp;gt;.*?'{2,3}"
such=re.compile(remuster)#,re.DOTALL)
for guimenu in such.findall(outtext):
  repl=guimenu.strip("'")
  repl=repl.split('-&amp;gt;')
  repl='<menuchoice><guimenu>%s</guimenu><guimenuitem>%s</guimenuitem></menuchoice>' %(repl[0].strip(' '),repl[1].strip(' '))
  outtext=outtext.replace(guimenu,repl)

#'''File→Properties'''
#<menuchoice><guimenu>File</guimenu><guimenuitem>Properties</guimenuitem></menuchoice>
remuster="'{2,3}.*?→.*?'{2,3}"
such=re.compile(remuster)#,re.DOTALL)
for guimenu in such.findall(outtext):
  repl=guimenu.strip("'")
  repl=repl.split('→')
  repl='<menuchoice><guimenu>%s</guimenu><guimenuitem>%s</guimenuitem></menuchoice>' %(repl[0].strip(' '),repl[1].strip(' '))
  outtext=outtext.replace(guimenu,repl)

#'''Start Page'''
#<guilabel>Start Page</guilabel>
remuster="'''.*?'''"
such=re.compile(remuster)#,re.DOTALL)
for guilabel in such.findall(outtext):
  repl=guilabel.replace("'''",'')
  repl='<guilabel>%s</guilabel>' %(repl.strip(' '))
  outtext=outtext.replace(guilabel,repl)

#''Vocabulary Collection''
#<emphasis>Vocabulary Collection</emphasis>
remuster="''.*?''"
such=re.compile(remuster)#,re.DOTALL)
for guilabel in such.findall(outtext):
  repl=guilabel.replace("''",'')
  repl='<emphasis>%s</emphasis>' %(repl.strip(' '))
  outtext=outtext.replace(guilabel,repl)

#userbase internal link
#[[Parley#Vocabulary_Collections|Vocabulary Collections]]
#<ulink url=" http://userbase.kde.org/Parley#Vocabulary_Collections">Vocabulary Collections</ulink>
#[[Parley/FileFormats|Parley File Formats on userbase.kde.org]
#<ulink url=" http://userbase.kde.org/Parley/FileFormats">Parley File Formats on userbase.kde.org</ulink>
#[[Akregator]]
#<ulink url=" http://userbase.kde.org/Akregator">Akregator</ulink>

#document internal link
#[[#Vocabulary Editing|an Editor for Vocabulary Collections]]
#<link linkend="vocabulary-editing">Editor for Vocabulary Collections</link>
##Vocabulary_Editing also valid in wiki

#images
#[[File:Parley welcome screen.png|thumb|500px]
#"Parley_welcome_screen.png" format="PNG"
#[[Image:Parley edit main.png|thumb|500px]]
#"Parley_edit_main.png" format="PNG"
#[[File:Parley new_document_lang_tenses.png|thumb|500px]]
#Parley_new_document_lang_tenses.png" format="PNG"
#[[File:Parley articles and gender 0-9-4 003.png|thumb|500px]]
#"Parley_articles_and_gender_0-9-4_003.png" format="PNG"

screenshot_template='\n<screenshot>\n\
<screeninfo></screeninfo>\n\
  <mediaobject>\n\
    <imageobject>\n\
      <imagedata fileref="%s" format="PNG"/>\n\
    </imageobject>\n\
    <textobject>\n\
      <phrase></phrase>\n\
    </textobject>\n\
  </mediaobject>\n\
</screenshot>'

remuster='\[\[.*?\]\]'
such=re.compile(remuster,re.DOTALL)
for linkimage in such.findall(outtext):
  if '[[File:' in linkimage: #screenshot
    repl=linkimage.split('|')[0]
    repl=repl.replace('[[File:','')
    repl=repl.replace(' ','_')
    repl=repl.rstrip(']')
    repl=repl.capitalize() #falsch für k3b
    repl=repl.replace('.jpeg','.png')
    repl=screenshot_template %repl.strip(' ')
  elif '[[Image:' in linkimage: #screenshot
    repl=linkimage.split('|')[0]
    repl=repl.replace('[[Image:','')
    repl=repl.replace(' ','_')
    repl=repl.rstrip(']')
    repl=repl.capitalize()
    repl=repl.replace('.jpeg','.png')
    repl=screenshot_template %repl.strip(' ')
  elif '[[#' in linkimage: #document internal link
    linkimagesplit=linkimage.split('|')
    anchor=linkimagesplit[0].replace('[[#','')
    anchor=anchor.replace( ' ','-')
    anchor=anchor.replace( '_','-')
    anchor=anchor.lower()
    anchortext=linkimagesplit[1].replace(']]','')
    repl='<link linkend="%s">%s</link>' %(anchor.strip(' '),anchortext.strip(' '))
    #print linkimage,repl
  elif '[[http' in linkimage: #it is an external link, process that later
    pass
  else:                   #userbase internal link
    linkimagesplit=linkimage.split('|')
    anchor=linkimagesplit[0].lstrip('[')
    anchor=anchor.rstrip(']')
    if len(linkimagesplit)>1:
      anchortext=linkimagesplit[1].replace(']]','')
    else:
      anchortext=anchor
    repl='<ulink url="http://userbase.kde.org/%s">%s</ulink>' %(anchor.strip(' '),anchortext.strip(' '))
  outtext=outtext.replace(linkimage,repl)
    
#external link
#[http://en.wikipedia.org/wiki/Flashcard flash card learning approach]
#<ulink url="http://en.wikipedia.org/wiki/Flashcard">flash card learning approach</ulink>
#[[https://mail.kde.org/mailman/listinfo/kdepim-users subscribe to kdepim-users]]
#<ulink url="https://mail.kde.org/mailman/listinfo/kdepim-users">subscribe to kdepim-users</ulink>
remuster='\[{1,2}http.*?\]{1,2}'
such=re.compile(remuster)#,re.DOTALL)
for link in such.findall(outtext):
  linkwobracket=link.replace('[','').replace(']','')
  linksplit=linkwobracket.split(' ')
  anchor = linksplit[0]
  anchortext=''
  for i in range(1,len(linksplit)): anchortext+=linksplit[i]+' '
  repl='<ulink url="%s">%s</ulink>' %(anchor,anchortext.rstrip(' '))
  outtext=outtext.replace(link,repl)

#fix empty chapter/sections
def comment_empty_sections(outtext, sectname):
  remuster="<%s.*?</%s" %(sectname, sectname)
  such=re.compile(remuster,re.DOTALL)
  for section in such.findall(outtext):
    body=section.lstrip('<%s' %sectname)
    body=body.rstrip('</%s' %sectname)
    if ("<sect" in body or "<para" in body)==False:
      repl="\n<!-- empty %s\n%s\n--" %(sectname, section)
      outtext=outtext.replace(section,repl)
  return outtext

for section in headinglevels:
  outtext=comment_empty_sections(outtext, section)

for i in range(0,nowikimarkup_id_number):
  outtext=outtext.replace('%s__%dx' %(nowikimarkup_id_marker.split('__')[0],i), nowikimarkup_idtext[i])

for i in range(0,premarkup_id_number):
  outtext=outtext.replace('%s__%dx' %(premarkup_id_marker.split('__')[0],i), premarkup_idtext[i])

outtext+=userbase_content_marker

if replacefile!='':
  filemodified = "%s.new.docbook" %replacefile
else:
  filemodified = "%s.new.docbook" %inputfile

modifiedtext=open(filemodified,"w")
if noheaderfooter:
  modifiedtext.write(outtext)
else:
  modifiedtext.write(docbookheader+outtext+docbookfooter)
modifiedtext.close()

print "output written to %s" %filemodified
