<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<!-- XSL Stylesheet to transform kvtml-2 files to html
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

Copyright 2007: Frederik Gladhorn <frederik.gladhorn@kdemail.net>

The easiest way to use the stylesheet is to include it in the .kvtml file:
<?xml-stylesheet type="text/xsl" href="kvtml_html_stylesheet.xsl.xsl"?>

 -->

<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 3.2 Final//EN"/>


	<xsl:template match="/">
	<html>
	<style type="text/css">
		td {
			border: solid 1px black;
			border-collapse: collapse;
		}
		table {
			border: solid 1px black;
			border-collapse: collapse;
			width: <xsl:value-of select="100 div count(/kvtml/identifiers/identifier)"/>%;
		}
		tr[languageheader] {
			color: black;
			font-size: 1.5em;
			background-color: lightblue;
		}
		.comment {
			color: blue;
		}
	</style>
	
	<script type="text/javascript">
    	function toggle_visibility(id) {
       		var e = document.getElementById(id);
       		if(e.style.visibility == 'visible')
          		e.style.visibility = 'hidden';
       		else
          		e.style.visibility = 'visible';
    	}
	</script>

	<head>
		<title><xsl:value-of select="kvtml/information/title"/></title>
	</head>
	
	<body><xsl:apply-templates select="kvtml" /></body>
	</html>
	</xsl:template>

	<xsl:template match="kvtml">
		<h2><xsl:value-of select="information/title"/></h2>
		<h4 onclick="toggle_visibility(5)">Click in the empty cells to reveal the answer!</h4>
		<xsl:apply-templates select="lessons"/>
	</xsl:template>


	<xsl:template match="lessons">
		<xsl:for-each select="container">
			<a href="#{name}"><xsl:value-of select="name"/></a>
			<br/>
		</xsl:for-each>

		<xsl:apply-templates select="container"/>
	</xsl:template>
 
	<xsl:template match="container">
		<h3>Lesson: <a name="{name}"><xsl:value-of select="name"/></a> (
			<a href="#" onclick="var answers=document.getElementsByTagName('div');var i=answers.length;while(i--)answers[i].setAttribute('style','visibility:visible;');return false;">reveal all</a> | 
			<a href="#" onclick="var answers=document.getElementsByTagName('div');var i=answers.length;while(i--)answers[i].setAttribute('style','visibility:hidden;');return false;">hide all</a>
		)</h3>
		<table border="1" cellspacing="30" cellpadding="30">
			<tr languageheader="true">
				<td>No.</td>
				<xsl:apply-templates select="/kvtml/identifiers"/>
			</tr>
			<xsl:apply-templates select="entry"/>

			<xsl:for-each select="entry">
				<xsl:variable name="id" select="@id"/>
				<xsl:apply-templates select="/kvtml/entries/entry[@id=$id]"/>
			</xsl:for-each>	
		</table>
	</xsl:template>
	
	<xsl:template match="identifiers/identifier">
		<td><xsl:value-of select="name"/></td>
	</xsl:template>
	
	<xsl:template match="/kvtml/entries/entry">
		<tr><td align="right"><xsl:value-of select="@id+1" />.</td>
		<xsl:apply-templates select="translation"/>
		</tr>
	</xsl:template>
	
	<xsl:template match="translation">
		<td onclick="toggle_visibility('{../@id}')">
			<xsl:choose> <!-- check if we have the question or the answer -->
				<xsl:when test="@id = 1"> <!-- ID 1 in <translation> is the answer, we want to hide-->
					<xsl:variable name="style" select="'hidden'" />
					<div id="{../@id}" style="visibility:{$style};">
						<xsl:value-of select="text"/>
						<xsl:apply-templates select="comment"/>
					</div>
				</xsl:when>
				<xsl:otherwise>
						<xsl:value-of select="text"/>
						<xsl:apply-templates select="comment"/>
				</xsl:otherwise>
			</xsl:choose>
			
		</td>
	</xsl:template>

	<xsl:template match="comment">
		<span class="comment">
			<br/><xsl:value-of select="."/>
		</span>
	</xsl:template>
</xsl:stylesheet>






