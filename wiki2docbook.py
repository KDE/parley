#!/usr/bin/env python
# -*- coding: utf-8 -*-
import re

headinglevels=['chapter', '</sect1>' '</sect2>', '</sect3>', '</sect4>']

def sectionheader(text,level,beginmarkup):
  levelstr='='*level
  remuster='%s.*?%s' %(levelstr,levelstr)
  such=re.compile(remuster,re.DOTALL)
  for treffer in such.findall(text):
    sectiontext=treffer.replace(levelstr,'')
    sectiontext=sectiontext.strip(' ')
    sectionanchor=sectiontext.replace("'",'')
    sectionanchor=sectionanchor.replace(",",'')
    sectionanchor=sectionanchor.replace("/",'-')
    sectionanchor=sectionanchor.replace(' ','-')
    sectionanchor=sectionanchor.lower()
    text=text.replace(treffer, '%s%d id="%s"><title>%s</title>' %(beginmarkup,level,sectionanchor,sectiontext))
    print text
    return text
    
parleyfile = "/tmp/ParleyManualUserbase.original"
#parleyfile = "/tmp/ParleyManual.modifiedheadinglevels"

textlines = open(parleyfile,"rw").readlines()
outtext=''
closemarkup=''
level=0
skip=False
initemizedlist=False
intable=False
for line in textlines:
  if '&lt;/translate&gt;' in line: skip=True
  #strip off: &lt;!--T:1--&gt
  remuster='&lt;.*&gt;'
  line=re.sub(remuster,'',line)
  #outtext+=line
  if line[0]=='=' and line[1]!='=':
    closemarkup=''
    if level>3:closemarkup+='</sect4>\n'
    if level>2:closemarkup+='</sect3>\n'
    if level>1:closemarkup+='</sect2>\n'
    if level>0:closemarkup+='</sect1>\n'
    level=1
    repl=sectionheader(line,1,'<sect')
    outtext+='%s%s' %(closemarkup,repl)
  elif line[0:2]=='==' and line[2]!='=':
    closemarkup=''
    if level>3:closemarkup+='</sect4>\n'
    if level>2:closemarkup+='</sect3>\n'
    if level>1:closemarkup+='</sect2>\n'
    level=2
    repl=sectionheader(line,2,'<sect')
    outtext+='%s%s' %(closemarkup,repl)
  elif line[0:3]=='===' and line[3]!='=':
    closemarkup=''
    if level>3:closemarkup+='</sect4>\n'
    if level>2:closemarkup+='</sect3>\n'
    level=3
    repl=sectionheader(line,3,'<sect')
    outtext+='%s%s' %(closemarkup,repl)
  elif line[0:4]=='====' and line[4]!='=':
    closemarkup=''
    if level>3:closemarkup+='</sect4>\n'
    level=4
    repl=sectionheader(line,4,'<sect')
    outtext+='%s%s' %(closemarkup,repl)
  else: #level="para"
     if line !='\n' and level!=0 and skip==False:
       if line[0]=='*': #itemizedlist
         if initemizedlist==False:
           initemizedlist=True
           outtext+='<itemizedlist>\n'
         outtext+='<listitem><para>%s</para></listitem>\n' %line[1:]
#       elif line[0:2]=='{|': #begin table
#         if intable==False:
#	   intable=True
#	   outtext+='<table>\n  <title> </title>\n  <tgroup cols="3">\n  <tbody>'
#
       else:
         if initemizedlist==True:
	   initemizedlist=False
	   outtext+='</itemizedlist>\n'
#	 elif intable==True:
#	   intable=False
#           outtext+="  </tbody>\n  </tgroup>\n</table></para>"
         outtext+='<para>%s</para>\n' %line
#       pass
outtext+='</sect%d>\n</sect1>' %level
#print outtext

#''''' -> '''
outtext=outtext.replace("'''''","''")

#'''[[#Vocabulary Practice|Practice]]'''
# gui link gui not allowed in docbook
# move ''' inside for document internal links
remuster="'''\[\[#.*?\]]'''"
such=re.compile(remuster,re.DOTALL)
for guilinkgui in such.findall(outtext):
  repl=guilinkgui.replace("'''[[","[[")
  repl=repl.replace("|","|'''")
  repl=repl.replace("]]'''","''']]")
  outtext=outtext.replace(guilinkgui,repl+'a')
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
  repl='<quote>%s</quote>' %(repl)
  outtext=outtext.replace(quot,repl)

#''View-&amp;gt;Lessons.''
#<menuchoice><guimenu>View</guimenu><guimenuitem>Lessons</guimenuitem></menuchoice>
remuster="'{2,3}.*?-&amp;gt;.*?'{2,3}"
such=re.compile(remuster)#,re.DOTALL)
for guimenu in such.findall(outtext):
  repl=guimenu.strip("'")
  repl=repl.split('-&amp;gt;')
  repl='<menuchoice><guimenu>%s</guimenu><guimenuitem>%s</guimenuitem></menuchoice>' %(repl[0],repl[1])
  outtext=outtext.replace(guimenu,repl)


#'''File→Properties'''
#<menuchoice><guimenu>File</guimenu><guimenuitem>Properties</guimenuitem></menuchoice>
remuster=u"'{2,3}.*→.*?'{2,3}"
such=re.compile(remuster)#,re.DOTALL)
for guimenu in such.findall(outtext):
  repl=guimenu.strip("'")
  repl=repl.split(u'→')
  repl='<menuchoice><guimenu>%s</guimenu><guimenuitem>%s</guimenuitem></menuchoice>' %(repl[0],repl[1])
  outtext=outtext.replace(guimenu,repl)


#'''Start Page'''
#<guilabel>Start Page</guilabel>
remuster="'''.*?'''"
such=re.compile(remuster)#,re.DOTALL)
for guilabel in such.findall(outtext):
  repl=guilabel.replace("'''",'')
  repl='<guilabel>%s</guilabel>' %(repl)
  outtext=outtext.replace(guilabel,repl)


#''Vocabulary Collection''
#<emphasis>Vocabulary Collection</emphasis>
remuster="''.*?''"
such=re.compile(remuster)#,re.DOTALL)
for guilabel in such.findall(outtext):
  repl=guilabel.replace("''",'')
  repl='<emphasis>%s</emphasis>' %(repl)
  outtext=outtext.replace(guilabel,repl)

#userbase internal link
#[[Parley#Vocabulary_Collections|Vocabulary Collections]]
#<ulink url=" http://userbase.kde.org/Parley#Vocabulary_Collections">Vocabulary Collections</ulink>
#[[Parley/FileFormats|Parley File Formats on userbase.kde.org]
#<ulink url=" http://userbase.kde.org/Parley/FileFormats">Parley File Formats on userbase.kde.org</ulink>

#document internal link
#[[#Vocabulary Editing|an Editor for Vocabulary Collections]]
#<link linkend="vocabulary-editing">Editor for Vocabulary Collections</link>

#images
#[[File:Parley welcome screen.png|thumb|500px]
#"Parley_welcome_screen.png" format="PNG"
#[[Image:Parley edit main.png|thumb|500px]]
#"Parley_edit_main.png" format="PNG"
#[[File:Parley new_document_lang_tenses.png|thumb|500px]]
#Parley_new_document_lang_tenses.png" format="PNG"
#[[File:Parley articles and gender 0-9-4 003.png|thumb|500px]]
#"Parley_articles_and_gender_0-9-4_003.png" format="PNG"

screenshot_template='<screenshot>\n\
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

remuster='\[\[.*?\]]'
such=re.compile(remuster,re.DOTALL)
for linkimage in such.findall(outtext):
  #print linkimage
  if '[[File:' in linkimage: #screenshot
    repl=linkimage.split('|')[0]
    repl=repl.replace('[[File:','')
    repl=repl.replace(' ','_')
    repl=repl.rstrip(']')
    repl=screenshot_template %repl
    #print repl
  elif '[[Image:' in linkimage: #screenshot
    repl=linkimage.split('|')[0]
    repl=repl.replace('[[Image:','')
    repl=repl.replace(' ','_')
    repl=repl.rstrip(']')
    repl=screenshot_template %repl
    #print repl
  elif '[[#' in linkimage: #document internal link
    linkimagesplit=linkimage.split('|')
    anchor=linkimagesplit[0].replace('[[#','')
    anchor=anchor.replace( ' ','-')
    anchor=anchor.lower()
    anchortext=linkimagesplit[1].replace(']]','')
    repl='<link linkend="%s">%s</link>' %(anchor,anchortext)
    print linkimage,repl
  else:                   #userbase internal link
    linkimagesplit=linkimage.split('|')
    anchor=linkimagesplit[0].replace('[[','')
    if len(linkimagesplit)>1:
      anchortext=linkimagesplit[1].replace(']]','')
    else:
      anchortext=''
    repl='<ulink url="http://userbase.kde.org/%s">%s</ulink>' %(anchor,anchortext)
    #print repl
  outtext=outtext.replace(linkimage,repl)
    
#external link
#[http://en.wikipedia.org/wiki/Flashcard flash card learning approach]
#<ulink url="http://en.wikipedia.org/wiki/Flashcard">flash card learning approach</ulink>

remuster='\[http.*\]'
such=re.compile(remuster)#,re.DOTALL)
for link in such.findall(outtext):
  #print link
  linkwobracket=link.replace('[','').replace(']','')
  linksplit=linkwobracket.split(' ')
  anchor = linksplit[0]
  anchortext=''
  for i in range(1,len(linksplit)): anchortext+=linksplit[i]+' '
  repl='<ulink url="%s">%s</ulink>' %(anchor,anchortext)
  #print repl
  outtext=outtext.replace(link,repl)

#print outtext



parleyfilemodified = "/tmp/ParleyManualUserbase.original.modified"

modifiedtext=open(parleyfilemodified,"w")
modifiedtext.write(outtext)
modifiedtext.close()



"""
#strip off: &lt;!--T:1--&gt
remuster='&lt;.*&gt;'
text=re.sub(remuster,'',text)
#print text

remuster='====.*?===='
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  sectiontext=treffer.replace('====','')
  sectiontext=sectiontext.strip(' ')
  sectionanchor=sectiontext.replace("'",'')
  sectionanchor=sectionanchor.replace(' ','-')
  sectionanchor=sectionanchor.lower()
  text=text.replace(treffer, '</sect4>\n<sect4 id="%s"><title>%s<title>' %(sectionanchor, sectiontext))
remuster='===.*?==='
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  sectiontext=treffer.replace('===','')
  sectiontext=sectiontext.strip(' ')
  sectionanchor=sectiontext.replace("'",'')
  sectionanchor=sectionanchor.replace(' ','-')
  sectionanchor=sectionanchor.lower()
#  text=text.replace(treffer, '</sect3>\n<sect3 id="%s"><title>%s<title>' %(sectionanchor, sectiontext))
remuster='==.*?=='
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  sectiontext=treffer.replace('==','')
  sectiontext=sectiontext.strip(' ')
  sectionanchor=sectiontext.replace("'",'')
  sectionanchor=sectionanchor.replace(' ','-')
  sectionanchor=sectionanchor.lower()
#  text=text.replace(treffer, '</sect2>\n<sect2 id="%s"><title>%s<title>' %(sectionanchor, sectiontext))
remuster='=.*?='
such=re.compile(remuster,re.DOTALL)
for treffer in such.findall(text):
  sectiontext=treffer.replace('=','')
  sectiontext=sectiontext.strip(' ')
  sectionanchor=sectiontext.replace("'",'')
  sectionanchor=sectionanchor.replace(' ','-')
  sectionanchor=sectionanchor.lower()
#  text=text.replace(treffer, '</sect1>\n<sect1 id="%s"><title>%s<title>' %(sectionanchor, sectiontext))
#text=text.replace('\n','<para>\n')
print text

#userbase internal link
#[[Parley#Vocabulary_Collections|Vocabulary Collections]]
#<ulink url=" http://userbase.kde.org/Parley#Vocabulary_Collections">Vocabulary Collections</ulink>
#[[Parley/FileFormats|Parley File Formats on userbase.kde.org]
#<ulink url=" http://userbase.kde.org/Parley/FileFormats">Parley File Formats on userbase.kde.org</ulink>

#document internal link
#[[#Vocabulary Editing|an Editor for Vocabulary Collections]]
#<link linkend="vocabulary-editing">Editor for Vocabulary Collections</link>

#images
#[[File:Parley welcome screen.png|thumb|500px]
#"Parley_welcome_screen.png" format="PNG"
#[[Image:Parley edit main.png|thumb|500px]]
#"Parley_edit_main.png" format="PNG"
#[[File:Parley new_document_lang_tenses.png|thumb|500px]]
#Parley_new_document_lang_tenses.png" format="PNG"
#[[File:Parley articles and gender 0-9-4 003.png|thumb|500px]]
#"Parley_articles_and_gender_0-9-4_003.png" format="PNG"

screenshot_template='<screenshot>\n\
<screeninfo></screeninfo>\n\
  <mediaobject>\n\
    <imageobject>\n\
      <imagedata fileref="%s="PNG"/>\n\
    </imageobject>\n\
    <textobject>\n\
      <phrase></phrase>\n\
    </textobject>\n\
  </mediaobject>\n\
</screenshot>'

remuster='\[\[.*?\]]'
such=re.compile(remuster,re.DOTALL)
for linkimage in such.findall(text):
  print linkimage
  if '[[File:' in linkimage: #screenshot
    repl=linkimage.split('|')[0]
    repl=repl.replace('[[File:','')
    repl=repl.replace(' ','_')
    repl=screenshot_template %repl
    print repl
  elif '[[Image:' in linkimage: #screenshot
    repl=linkimage.split('|')[0]
    repl=repl.replace('[[File:','')
    repl=repl.replace(' ','_')
    print repl
  elif '[[#' in linkimage: #document internal link
    linkimagesplit=linkimage.split('|')
    anchor=linkimagesplit[0].replace('[[#','')
    anchor=anchor.replace( ' ','-')
    anchor=anchor.lower()
    anchortext=linkimagesplit[1].replace(']]','')
    repl='<link linkend="%s">%s</link>' %(anchor,anchortext)
    print repl
  else:                   #userbase internal link
    linkimagesplit=linkimage.split('|')
    anchor=linkimagesplit[0].replace('[[','')
    if len(linkimagesplit)>1:
      anchortext=linkimagesplit[1].replace(']]','')
    else:
      anchortext=''
    repl='<ulink url="http://userbase.kde.org/%s">%s</ulink>' %(anchor,anchortext)
    print repl
  text=text.replace(linkimage,repl)
    
#external link
#[http://en.wikipedia.org/wiki/Flashcard flash card learning approach]
#<ulink url="http://en.wikipedia.org/wiki/Flashcard">flash card learning approach</ulink>

remuster='\[http.*\]'
such=re.compile(remuster)#,re.DOTALL)
for link in such.findall(text):
  print link
  linkwobracket=link.replace('[','').replace(']','')
  linksplit=linkwobracket.split(' ')
  anchor = linksplit[0]
  anchortext=''
  for i in range(1,len(linksplit)): anchortext+=linksplit[i]+' '
  repl='<ulink url="%s">%s</ulink>' %(anchor,anchortext)
  print repl
  text=text.replace(link,repl)

#print text

parleyfilemodified = "/tmp/ParleyManualUserbase.original.modified"

modifiedtext=open(parleyfilemodified,"w")
modifiedtext.write(text)
modifiedtext.close()
"""