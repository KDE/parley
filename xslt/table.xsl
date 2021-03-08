<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"  xmlns="http://www.w3.org/1999/xhtml"
  xmlns:xhtml="http://www.w3.org/1999/xhtml">
<!-- XSL Stylesheet to transform kvtml-2 files to html
    SPDX-FileCopyrightText: Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later

The easiest way to use the stylesheet is to include it in the .kvtml file:
<?xml-stylesheet type="text/xsl" href="kvtml_html_stylesheet.xsl"?>
 -->
 
	<xsl:output method="xml" doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN" doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"
		encoding="UTF-8" omit-xml-declaration="yes" version="1.0" />

	<xsl:template match="/">
	<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<style type="text/css">
		td, th {
			border: solid 1px black;
			border-collapse: collapse;
		}
		table {
			border: solid 1px black;
			border-collapse: collapse;
			width: 100%;
		}
		.comment {
			font-style: italic;
		}
		.firstcolumn {
			width: 5%;
		}
		.contentcell {
			width: <xsl:value-of select="95 div count(/kvtml/identifiers/identifier)"/>%;
		}
		#buttons, #buttons td {
			width: auto;
			border: none;
		}
		@media print {
			.noprint {
				display: none;
			}
		}
		</style>
	
		<script type="text/javascript">
	<xsl:text disable-output-escaping="yes"><![CDATA[//<![CDATA[
	function toggle_visibility(id) {
		var e = document.getElementById(id);
		if(e.style.visibility == 'visible')
			e.style.visibility = 'hidden';
		else
			e.style.visibility = 'visible';
	}

	function set_column_visibility(table, column, visibility) {
		if (table == null) {
			tables = document.getElementsByTagName("table");
			for(var i = 0; i < tables.length; i++) {
				if (tables[i].id != "buttons") {
					set_column_visibility(tables[i].firstChild, column, visibility);
				}
			}
			return;
		}
		
		var tr = table.firstChild;
		while (tr != null) {
			var td = tr.firstChild;
			while (td != null) {
				if (td.nodeName == "TD" && td.childNodes.length == 1 && td.childNodes[0].nodeName == "DIV") {
					if(column == -1 || td.childNodes[0].id.split("-")[1] == column) {
						td.childNodes[0].style.visibility = visibility;
					}
				}
				td = td.nextSibling
			}
			tr = tr.nextSibling;
		}
	}
	]]>//]]&gt;</xsl:text>
		</script>
		<title><xsl:value-of select="kvtml/information/title"/></title>
	</head>
	
	<body><xsl:apply-templates select="kvtml" /></body>
	</html>
	</xsl:template>

	<xsl:template match="kvtml">
		<h1><xsl:value-of select="information/title"/></h1>
		<table id="buttons" class="noprint">
			<xsl:apply-templates select="/kvtml/identifiers" mode="buttons"/>
		</table>
		<xsl:apply-templates select="lessons"/>
	</xsl:template>


	<xsl:template match="lessons">
		<xsl:apply-templates select="container" mode="toc"/>
		<xsl:apply-templates select="container" mode="entries"/>
	</xsl:template>

	<xsl:template match="container" mode="toc">
		<ul class="noprint">
			<li>
				<a href="#lesson{count(preceding::container) + count(ancestor::container) + 1}"><xsl:value-of select="name"/></a>
				<xsl:apply-templates select="container" mode="toc"/>
			</li>
		</ul>
	</xsl:template>

	<xsl:template match="container" mode="entries">
		<xsl:choose>
			<xsl:when test="count(ancestor::container) = 0">
				<h2><a name="lesson{count(preceding::container) + count(ancestor::container) + 1}"><xsl:value-of select="name"/></a></h2>
			</xsl:when>
			<xsl:when test="count(ancestor::container) = 1">
				<h3><a name="lesson{count(preceding::container) + count(ancestor::container) + 1}"><xsl:value-of select="name"/></a></h3>
			</xsl:when>
			<xsl:when test="count(ancestor::container) = 2">
				<h4><a name="lesson{count(preceding::container) + count(ancestor::container) + 1}"><xsl:value-of select="name"/></a></h4>
			</xsl:when>
			<xsl:otherwise>
				<h5><a name="lesson{count(preceding::container) + count(ancestor::container) + 1}"><xsl:value-of select="name"/></a></h5>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:if test="count(child::entry) != 0">
			<table>
				<tr>
					<th></th>
					<xsl:apply-templates select="/kvtml/identifiers" mode="header"/>
				</tr>
				<tr class="noprint">
					<td></td>
					<xsl:apply-templates select="/kvtml/identifiers" mode="buttonrow"/>
				</tr>
				<xsl:apply-templates select="entry"/>

				<xsl:for-each select="entry">
					<xsl:variable name="id" select="@id"/>
					<xsl:apply-templates select="/kvtml/entries/entry[@id=$id]"/>
				</xsl:for-each>	
			</table>
		</xsl:if>
		<xsl:apply-templates select="container" mode="entries"/>
	</xsl:template>
	
	<xsl:template match="identifiers/identifier" mode="header">
		<th class="numbercell"><xsl:value-of select="name"/></th>
	</xsl:template>

	<xsl:template match="identifiers/identifier" mode="buttons">
		<tr>
			<td><xsl:value-of select="name"/>:</td>
			<td>
				<input type="button" value="Hide all" onclick="set_column_visibility(null, {@id}, 'hidden')" /><xsl:text> </xsl:text>
				<input type="button" value="Show all" onclick="set_column_visibility(null, {@id}, 'visible')" />
			</td>
		</tr>
	</xsl:template>

	<xsl:template match="identifiers/identifier" mode="buttonrow">
		<td>
			<input type="button" value="Hide all" onclick="set_column_visibility(this.parentNode.parentNode.parentNode, {@id}, 'hidden')" /><xsl:text> </xsl:text>
			<input type="button" value="Show all" onclick="set_column_visibility(this.parentNode.parentNode.parentNode, {@id}, 'visible')" />
		</td>
	</xsl:template>
	
	<xsl:template match="/kvtml/entries/entry">
		<tr><td align="right" class="numbercell"><xsl:value-of select="@id+1" />.</td>
		<xsl:apply-templates select="translation"/>
		</tr>
	</xsl:template>
	
	<xsl:template match="translation">
		<td class="contentcell" onclick="toggle_visibility('i{../@id}-{@id}')">
			<div id="i{../@id}-{@id}" style="visibility: visible;">
				<xsl:value-of select="text"/>
				<xsl:apply-templates select="comment"/>
			</div>
		</td>
	</xsl:template>

	<xsl:template match="comment">
		<span class="comment">
			<br/><xsl:value-of select="."/>
		</span>
	</xsl:template>
</xsl:stylesheet>
