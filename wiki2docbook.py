#!/usr/bin/env python
# -*- coding: utf-8 -*-
import re, sys, os, getopt

headinglevelsconsecutive=['chapter', 'sect1', 'sect2', 'sect3', 'sect4', 'sect5']
toplevel=0 #sect1
#toplevel=0 #chapter

userbase_content_marker='\n<!--userbase-content-->\n'

def usage():
    print '\nUsage: python %s [OPTION] path/to/userbasepagedumpfile' %os.path.basename(sys.argv[0])
    print '       generates docbook output file with extension *.new.docbook'
    print 'Options: -h, --help              : usage'
    print '         -t, --toplevel          : toplevel heading 0=chapter  1=sect1'
    print '         -r file, --replace file : index.docbook file with kde docbook header + footer'
    print '                                   and body from a previous userbase page dump to replace it'
    print '                                   if empty a default kde docbook header + footer is used'
    sys.exit(2)
try:
    opts, args = getopt.getopt(sys.argv[1:], "ht:r:", ["help", "toplevel=", 'replace='])
except getopt.GetoptError:
    usage() # print help information and exit

replacefile=''
for o,a in opts:
    if o in ("-h", "--help"):
        usage()
    if o in ("-t", "--toplevel"):
        toplevel=a
    if o in ("-r", "--replace"):
        replacefile=a
if len(args) != 1:
    usage()
inputfile=args[0]
if not os.path.isfile(inputfile):
    sys.exit('input file %s not found' %inputfile) 

docbookheader,docbookfooter='',''
if not os.path.isfile(replacefile):
  if replacefile!='':
    print 'docbook file with previous userbase page dump not found, generating a docbook with template header + footer'
    replacefile=''
else:
  text=open(replacefile,"r").read()
  try:
    docbookheader,userpagebody,docbookfooter=text.split(userbase_content_marker)
  except:
    print 'docbook file has no previous userbase page dump, generating a docbook with template header + footer'
    replacefile=''
if docbookheader=='': 
  print 'generating a docbook with template header + footer, edit the defaults'
  text=open('/home/kdedev/svn/work/doc/parley/template-wiki2docbook.docbook',"r").read()
  docbookheader,userpagebody,docbookfooter=text.split(userbase_content_marker)
  replacefile=''

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
    text=text.replace(treffer, '%s id="%s"><title>%s</title>' %(beginmarkup,sectionanchor,sectiontext))
    #output of headinglevels
    #print text
    return text
    
text=open(inputfile,"r").read()
remuster='<timestamp>.*?</timestamp>'
such=re.compile(remuster,re.DOTALL)
if len(such.findall(text))!=1:
  print 'missing userbase timestamp'
  outtext=userbase_content_marker
else: 
  outtext='<!--userbase %s-->\n%s' %(such.findall(text)[0], userbase_content_marker)

textlines = open(inputfile,"rw").readlines()
#outtext=''
level=0
equalcharcount=1
equalcharjumpstart=1
fixequalcharcount=0
headinglevels=list(headinglevelsconsecutive)
skip=False
initemizedlist=False

for line in textlines:
  if '&lt;/translate&gt;' in line: skip=True
  #strip off: &lt;!--T:1--&gt
  remuster='&lt;.*&gt;'
  line=re.sub(remuster,'',line)

  #fix non consecutive heading levels
  if line[0]=='=':
    newequalcharcount=line.count('=',0,5)
    if newequalcharcount-equalcharcount>1:
      fixequalcharcount=newequalcharcount-equalcharcount-1
      equalcharjumpstart=equalcharcount
      equalcharcount=newequalcharcount
      headinglevels.insert(equalcharjumpstart,"jump")
      #print headinglevels,headinglevelsconsecutive
    #print newequalcharcount,equalcharjumpstart,fixequalcharcount
    if newequalcharcount-1<=equalcharjumpstart:
      fixequalcharcount=0
      headinglevels=list(headinglevelsconsecutive)
      #print headinglevels
  #outtext+=line
  if line[0]=='=' and line[1]!='=':
    closemarkup=''
    if level>3:closemarkup+='</%s>\n' %headinglevels[toplevel+3]
    if level>2:closemarkup+='</%s>\n' %headinglevels[toplevel+2]
    if level>1:closemarkup+='</%s>\n' %headinglevels[toplevel+1]
    if level>0:closemarkup+='</%s>\n' %headinglevels[toplevel+0]
    #if level>3:closemarkup+='</sect4>\n'
    #if level>2:closemarkup+='</sect3>\n'
    #if level>1:closemarkup+='</sect2>\n'
    #if level>0:closemarkup+='</sect1>\n'
    level=1
    repl=sectionheader(line,1,'<%s' %headinglevels[toplevel-1+1])
    #repl=sectionheader(line,1,'<sect')
    outtext+='%s%s' %(closemarkup,repl)
  elif line[0:2]=='==' and line[2]!='=':
    closemarkup=''
    if level>3:closemarkup+='</%s>\n' %headinglevels[toplevel+3]
    if level>2:closemarkup+='</%s>\n' %headinglevels[toplevel+2]
    if level>1:closemarkup+='</%s>\n' %headinglevels[toplevel+1]
    #if level>3:closemarkup+='</sect4>\n'
    #if level>2:closemarkup+='</sect3>\n'
    #if level>1:closemarkup+='</sect2>\n'
    level=2
    repl=sectionheader(line,2,'<%s' %headinglevels[toplevel-1+2])
    #repl=sectionheader(line,2,'<sect')
    outtext+='%s%s' %(closemarkup,repl)
  elif line[0:3]=='===' and line[3]!='=':
    closemarkup=''
    if level>3:closemarkup+='</%s>\n' %headinglevels[toplevel+3]
    if level>2:closemarkup+='</%s>\n' %headinglevels[toplevel+2]
    #if level>3:closemarkup+='</sect4>\n'
    #if level>2:closemarkup+='</sect3>\n'
    level=3
    repl=sectionheader(line,3,'<%s' %headinglevels[toplevel-1+3])
    #repl=sectionheader(line,3,'<sect')
    outtext+='%s%s' %(closemarkup,repl)
  elif line[0:4]=='====' and line[4]!='=':
    closemarkup=''
    if level>3:closemarkup+='</%s>\n' %headinglevels[toplevel+3]
    #if level>3:closemarkup+='</sect4>\n'
    level=4
    repl=sectionheader(line,4,'<%s' %headinglevels[toplevel-1+4])
    #repl=sectionheader(line,4,'<sect')
    outtext+='%s%s' %(closemarkup,repl)
  else: #level="para"
     if line !='\n' and level!=0 and skip==False:
       if line[0]=='*': #itemizedlist
         if initemizedlist==False:
           initemizedlist=True
           outtext+='<itemizedlist>\n'
         else: 
           outtext=outtext.rstrip('\n')
           outtext=outtext[:-len('</itemizedlist>')]
         outtext+='<listitem><para>%s</para></listitem>\n</itemizedlist>\n' %line[1:]
       elif line[0:2]=='{|':  #begin table
         tabletext='<para>\n<table>\n  <title> </title>\n  <tgroup cols="%d">\n  <tbody>\n'
         #colsnumber=0
         rownumber=0
       elif line[0:2]=='| ':  #in table in row
         tabletext+='    <entry>%s</entry>\n' %line[1:].lstrip(' ').rstrip(' \n')
         colsnumber+=1
       elif line[0:2]=='|-': #in table new row
         if rownumber>0:
           tabletext+='  </row>\n'
         rownumber+=1
         tabletext+='  <row>\n'
         colsnumber=0
       elif line[0:2]=='|}': #end table
         tabletext=tabletext %colsnumber
         outtext+="%s    </row>\n</tbody>\n  </tgroup>\n</table>\n</para>\n" %(tabletext)
       else:
         if initemizedlist==True:
           initemizedlist=False
#           outtext+='</itemizedlist>\n'
         outtext+='<para>%s</para>\n' %line

outtext+='</%s>\n</%s>' %(headinglevels[level-1], headinglevels[toplevel])
#outtext+='</sect%d>\n</sect1>' %level
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
    anchor=anchor.replace( '_','-')
    anchor=anchor.lower()
    anchortext=linkimagesplit[1].replace(']]','')
    repl='<link linkend="%s">%s</link>' %(anchor,anchortext)
    #print linkimage,repl
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
  linkwobracket=link.replace('[','').replace(']','')
  linksplit=linkwobracket.split(' ')
  anchor = linksplit[0]
  anchortext=''
  for i in range(1,len(linksplit)): anchortext+=linksplit[i]+' '
  repl='<ulink url="%s">%s</ulink>' %(anchor,anchortext)
  #print repl
  outtext=outtext.replace(link,repl)

#print outtext

#fix empty chapter/sections
def comment_empty_sections(outtext, sectname):
  remuster="<%s.*?</%s" %(sectname, sectname)
  such=re.compile(remuster,re.DOTALL)
  for section in such.findall(outtext):
    body=section.lstrip('<%s' %sectname)
    body=body.rstrip('</%s' %sectname)
    if ("<sect" in body or "<para" in body)==False:
      repl="\n<!-- empty %s\n%s\n--" %(sectname, section)
      #print section,repl
      outtext=outtext.replace(section,repl)
  return outtext

for section in ['chapter', 'sect1', 'sect2', 'sect3', 'sect4']:
  outtext=comment_empty_sections(outtext, section)

outtext+=userbase_content_marker

if replacefile!='':
  filemodified = "%s.new.docbook" %replacefile
else:
 filemodified = "%s.new.docbook" %inputfile

modifiedtext=open(filemodified,"w")
modifiedtext.write(docbookheader+outtext+docbookfooter)
modifiedtext.close()

